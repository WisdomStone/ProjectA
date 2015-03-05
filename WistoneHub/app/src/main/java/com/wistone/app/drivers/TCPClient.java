package com.wistone.app.drivers;

import android.content.Context;
import android.text.TextUtils;

import com.wistone.app.Control;
import com.wistone.app.StateMachine;
import com.wistone.app.utilities.Logging;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Serializable;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/***************************************************************************************************
 * Class: TCPClient
 * ================
 *
 * This class operates the TCP connection between the Android and the Remote Control Centre:
 * - it establishes the connection,
 * - uses writer function to send the outgoing data,
 * - uses reader ISR to receive the incoming data,
 * - and eventually terminates the connection
 *
 * Created by: Yulia Loudman
 * Creation date: 29/03/2014
 **************************************************************************************************/

public class TCPClient implements Runnable, Serializable { // Serializable is used only when debugOn

    transient public    static  TCPClient       _instance = null; // using transient keyword only when debugOn
    transient private   static  Thread          _thread = null;
    private             static  boolean         _isOn = false;
    private             static  boolean         _isPausedRead = false;
    transient private   static  ClientTimer     _clientTimer = null;
    transient private   static  Socket          _socket = null;
    transient public    static  InputStream     _input = null;
    transient public    static  OutputStream    _output = null;
    private             static BlockingQueue<String> _queue = null;
    transient public    Context                 _context = null; // when debugOn
    public              OnMessageReceived       _listener = null; // when debugOn
    private static final long                   _serialVersionUID = 1L; // when debugOn

    private static final boolean    _debugOn = true;
    private static final String     _HELLO = "hello";
    private static final int        _HELLO_LENGTH = _HELLO.length();
    private static final String     _PING = "!";
    private static final int        _PING_LENGTH = _PING.length();
    private static final int        _MAX_PING_SERVER_RETRIES = 5;
    private static final int        _MAX_OPEN_SOCKET_RETRIES = 5;
    private static final int        _PING_SERVER_SLEEP_INTERVAL_MS = 1000;
    private static final int        _OPEN_SOCKET_SLEEP_INTERVAL_MS = 10000;
    private static final int        _PAUSED_READ_SLEEP_INTERVAL_MS = 1000;
    private static String           _server_ip = "10.0.0.7";//"192.168.1.100";//"192.168.56.1"; // 10.0.0.1 // "192.168.56.1"; VirtualBox Host-Only Network IPv4 (Genymotion); use ipconfig to manually set local IPv4 address
    private static final int        _SERVER_PORT = 5555;
    private static final String     _TAG = "TCPClient";

    private TCPClient() {

        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "TCPClient(): c-tor", _TAG); // private c-tor for singleton class
    }

    public static TCPClient getInstance() {

        if (_instance == null) {
            _instance = new TCPClient(); // if new fails - an exception is thrown
            _thread = new Thread(_instance);
            _thread.setName("TCPClientThread");
            _thread.start();
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "getInstance(): new thread", _TAG);
        }
        return _instance;
    }


    public void run() {

        int openSocketRetries = 0;
        try {
            _queue= StateMachine.TCPClient_queue;
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "run(): start TCPClient thread", _TAG);
            Thread.currentThread().setPriority(Thread.NORM_PRIORITY);
            while (openSocketRetries < _MAX_OPEN_SOCKET_RETRIES) {
                if (open()) {
                    _isOn = true;
                    _isPausedRead = false;
                    read();
                    return;
                }
                openSocketRetries++;
                Thread.sleep(_OPEN_SOCKET_SLEEP_INTERVAL_MS);
            }
        } catch (Exception e) {
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "run(): unhandled exception", _TAG, e);
        }
        restartTheClient(); // YL 24.6 - to get restart retries when the server is down - until the reconnect succeeds (why the timer doesn't take care of it?)
    }

    private boolean open() {

        try {
            // close old socket if exists:
            if (_socket != null) {
                _socket.close();
                _socket = null;
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): closed old socket", _TAG);
            }
            _server_ip = Control.getServerIP();
            _socket = new Socket(_server_ip, _SERVER_PORT);
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): socket - OK", _TAG);

            // close input stream if exists:
            if (_input != null) {
                _input.close();
                _input = null;
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): closed old _input", _TAG);
            }
            _input = _socket.getInputStream();
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): _input - OK", _TAG);

            // close output stream if exists:
            if (_output != null) {
                _output.close();
                _output = null;
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): closed old _output", _TAG);
            }
            _output = _socket.getOutputStream();
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): _output - OK", _TAG);

            // start the timer:
            _clientTimer = new ClientTimer();
            _clientTimer.open();

            // send hello message to the server and receive hello back:
            return (sendHelloToServer() && receiveHelloFromServer());
        } catch(UnknownHostException e) {
            Logging.write(Logging.MessageType.ERROR, "open(): socket, host", _TAG, e);
        } catch(IOException e) {
            Logging.write(Logging.MessageType.ERROR, "open(): socket, io", _TAG, e);
        }
        return false;
    }

    private void close() {

        try {
            _isOn = false;
            _isPausedRead = true;
            if (_clientTimer != null) {
                _clientTimer.close();
                _clientTimer = null;
            }
            if (_socket != null) {
                _socket.close();
                _socket = null;
            }
            if (_input != null) {
                _input.close();
                _input = null;
            }
            if (_output != null) {
                _output.close();
                _output = null;
            }
            _instance = null;
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "close(): OK", _TAG);
        } catch (IOException e) {
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "close(): _socket close()", _TAG, e);
        }
    }

    private void read() {

        while(_isOn) {
            try {
                if (_isPausedRead) {
                    Thread.sleep(_PAUSED_READ_SLEEP_INTERVAL_MS);
                    continue;
                }
                DataInputStream dataInputStream = new DataInputStream(_input);
                int serverMessageLength = dataInputStream.available();
                byte[] serverMessageRaw = new byte[serverMessageLength];
                dataInputStream.readFully(serverMessageRaw);
                String serverMessageString = new String(serverMessageRaw, "UTF-8");
                if (!TextUtils.isEmpty(serverMessageString)) {
                    Logging.write(Logging.MessageType.DEBUG_VERBOSE, "read(): readFully() received - " + serverMessageString, _TAG);
                    //TODO possible deadlock
                    _queue.put(serverMessageString);

                    _clientTimer.resetClientTimer();
                }
                if (_debugOn) {
                    if (_listener != null) { // && (serverMessageString != null)
                        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "_listener: debugOn - " + serverMessageString, _TAG);
                        _listener.messageReceived(serverMessageString);
                    }
                }
            } catch(IOException e) {
            } catch (InterruptedException e) {
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "read(): sleep", _TAG, e);
            }
        }
    }

    public void write(String clientMessage) {

        DataOutputStream dataOutputStream = new DataOutputStream(_output);
        try {
            dataOutputStream.writeBytes(clientMessage);
            _clientTimer.resetClientTimer();
        } catch(IOException e) {
            Logging.write(Logging.MessageType.ERROR, "write(): writeBytes() failed to send message", _TAG, e);
        }
    }

    private boolean receiveHelloFromServer() {

        DataInputStream dataInputStream = new DataInputStream(_input);
        byte[] helloMessageRaw = new byte[_HELLO_LENGTH];
        try {
            dataInputStream.readFully(helloMessageRaw);
            String helloMessageString = new String(helloMessageRaw, "UTF-8");
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "receiveHelloFromServer(): readFully() received - " + helloMessageString, _TAG);
            if (helloMessageString.equals(_HELLO)) {
                return true;
            }
        } catch(IOException e) {
            Logging.write(Logging.MessageType.ERROR, "receiveHelloFromServer(): readFully() failed to receive hello", _TAG, e);
        }
        return false;
    }

    private boolean sendHelloToServer() {

        DataOutputStream dataOutputStream = new DataOutputStream(_output);
        try {
            dataOutputStream.writeBytes(_HELLO);
            return true;
        } catch(IOException e) {
            Logging.write(Logging.MessageType.ERROR, "sendHelloToServer(): writeBytes() failed to send hello", _TAG, e);
        }
        return false;
    }

    private boolean pingTheServer() {

        int pingServerRetries = 0;
        _isPausedRead = true;
        while (pingServerRetries < _MAX_PING_SERVER_RETRIES) {
            DataOutputStream dataOutputStream = new DataOutputStream(_output);
            DataInputStream dataInputStream = new DataInputStream(_input);
            byte[] pingMessageRaw = new byte[_PING_LENGTH];
            try {
                dataOutputStream.writeBytes(_PING);
                dataInputStream.readFully(pingMessageRaw);
                String pingMessageString = new String(pingMessageRaw, "UTF-8");
                if (pingMessageString.equals(_PING)) {
                    _isPausedRead = false;
                    return true;
                }
                else {
                    Thread.sleep(_PING_SERVER_SLEEP_INTERVAL_MS);
                }
            } catch (IOException e) {
                Logging.write(Logging.MessageType.ERROR, "pingTheServer(): writeBytes() or readFully() failed", _TAG, e);
            } catch (InterruptedException e) {
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "pingTheServer(): sleep", _TAG, e);
            }
            pingServerRetries++;
        }
        _isPausedRead = false;
        return false;
    }

    public void restartTheClient() {

        _instance.close();
        StateMachine.openTCPClient();
    }
    public boolean is_Connected(){
        if (_socket==null) return false;
        return _socket.isConnected();
    }



/***************************************************************************************************
 * Class: ClientTimer
 **************************************************************************************************/

    private class ClientTimer {

        private int _counter = 0;
        private boolean _counterLock = false;
        private ScheduledExecutorService _scheduler = null;

        private final long  _MAX_COUNTER        = 3;
        private final long  _INITIAL_DELAY_SEC  = 60; // 5 * 60
        private final long  _INTERVAL_DELAY_SEC = 30; // 2 * 60
        private final int   _POOL_SIZE          = 2; // max number of _scheduler threads that are allowed to run concurrently (e.g. - when restart)

        private ClientTimer() {

            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "ClientTimer(): c-tor", _TAG); // private c-tor for singleton class
        }

        private void open() {

            Runnable clientTimerTask = new ClientTimerTask();
            _scheduler = Executors.newScheduledThreadPool(_POOL_SIZE);
            _scheduler.scheduleAtFixedRate(clientTimerTask, _INITIAL_DELAY_SEC, _INTERVAL_DELAY_SEC, TimeUnit.SECONDS);
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "ClientTimer(): open - OK", _TAG);
        }

        public void close() {

            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "ClientTimer(): close - OK", _TAG);
        }

        private void resetClientTimer() {

            while (_counterLock);
            _counterLock = true;
            _counter = 0;
            _counterLock = false;
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "ClientTimer(): resetClientTimer", _TAG);
        }

        private class ClientTimerTask implements Runnable {

            @Override
            public void run() {

                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "run(): ClientTimerTask thread", _TAG);
                Thread.currentThread().setPriority(Thread.NORM_PRIORITY);
                _counterLock = true;
                _counter++;
                if (_counter >= _MAX_COUNTER) {
                    Logging.write(Logging.MessageType.DEBUG_VERBOSE, "run(): _MAX_COUNTER", _TAG);
                    boolean result = pingTheServer();
                    if (result) {
                        _counter = 0;
                    }
                    else {
                        restartTheClient();
                    }
                }
                _counterLock = false;
            }
        }
    }

    public interface OnMessageReceived { // when debugOn

        public void messageReceived(String serverMessage); // the method is implemented inside doInBackground method in DebugActivity
    }
}
