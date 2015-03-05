package com.wistone.app;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;

import com.wistone.app.drivers.COMPort;
import com.wistone.app.drivers.TCPClient;
import com.wistone.app.utilities.Logging;

import java.util.concurrent.ArrayBlockingQueue;

/***************************************************************************************************
 * Class: StateMachine
 * ===================
 *
 * This class maintains the state machine of the Android (Wistone Hub):
 * - it initializes the Android connections:
 *      - the TCP connection to the Remote Control Center
 *      - the COM port with the Communication Plug
 * - performs the periodical tasks to maintain the Android connections
 * - shuts the system down if needed, and goes back to reinitialize it
 *
 * Created by: Yulia Loudman
 * Creation date: 29/03/2014
 **************************************************************************************************/

public class StateMachine {

    public enum State {
        INIT_UTILITIES,
        INIT_TCP_CONNECTION,        // the TCP connection to the Remote Control Center
        INIT_COM_PORT,              // the COM port with the Communication Plug
        RUN_SESSION,
        SHUTDOWN_SESSION
    }

    private static State _state = State.INIT_UTILITIES;
    private static final int COMPORT_QUEUE_SIZE = 5;
    private static final int TCPClient_QUEUE_SIZE = 20;
    public static ArrayBlockingQueue<String> CP_queue;
    public static ArrayBlockingQueue<String> TCPClient_queue;
    public static Context _context = null; // when debugOn
    private static final boolean _debugOn = true;
    private static final String _TAG = "StateMachine";
    public  static TCPClient tcpClient = null;
    public  static COMPort COMPortConnection = null;
    public static Sensors _sensor = null;


    private StateMachine() {

        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "StateMachine(): c-tor", _TAG); // private c-tor for singleton class
    }

    public static boolean singleStep(final Activity context) {


        _context = context;

        try {
            if (_state == State.INIT_UTILITIES) {
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "singleStep():State.INIT_UTILITIES", _TAG);
                Logging.open(context);
                _sensor = new Sensors();
                updateState();
            }
            if (_state == State.INIT_TCP_CONNECTION) {
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "singleStep():State.INIT_TCP_CONNECTION", _TAG);

                if (!openTCPClient()) {
                    Logging.write(Logging.MessageType.ERROR, "singleStep(): openTCPClient() failed", _TAG);
                    return false;
                }
                updateState();
            }
            if (_state == State.INIT_COM_PORT) {
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "singleStep():State.INIT_COM_PORT", _TAG);
                if (!openCOMPORT()){
                    Logging.write(Logging.MessageType.ERROR, "singleStep(): openCOMPORT() failed", _TAG);
                    return false;
               }
               updateState();
            }
            if (_state == State.RUN_SESSION){
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "singleStep():State.RUN_SESSION", _TAG);
                initController();//TBD end session
                /*if (_debugOn) {
                    tcpClient._context = _context;
                    Intent intent = new Intent(_context, DebugActivity.class);
                    intent.putExtra("tcpClient", tcpClient);
                    _context.startActivity(intent);
                    Logging.write(Logging.MessageType.DEBUG_VERBOSE, "openTCPClient(): back", _TAG);
                }*/
            }
        } catch (Exception e) {
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "singleStep(): unhandled exception", _TAG, e);
            Logging.close();
            return false;
        }
        return true;
    }

    private static void updateState() {

        switch (_state) {
            case INIT_UTILITIES:
                _state = State.INIT_TCP_CONNECTION;
                break;
            case INIT_TCP_CONNECTION:
                _state = State.INIT_COM_PORT;
                break;
            case INIT_COM_PORT:
                _state = State.RUN_SESSION;
                break;
            case RUN_SESSION:
                _state = State.SHUTDOWN_SESSION;
                break;
            case SHUTDOWN_SESSION:
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "updateState(): illegal case", _TAG);
        }
    }

    private static void updateState(State state) {

        _state = state;
        updateState();
    }

    public static boolean openTCPClient() {
        //Init TCP connection queue
        TCPClient_queue = new  ArrayBlockingQueue <String>(TCPClient_QUEUE_SIZE);
        Control TCPClient_Q = new Control(TCPClient_queue , "TCPClient");
        new Thread (TCPClient_Q).start();
        tcpClient = TCPClient.getInstance();
        if (tcpClient == null) {
            Logging.write(Logging.MessageType.ERROR, "openTCPClient(): failed", _TAG);
            return false;
        }

        return true;

    }

    public static boolean openCOMPORT() {
        //Init COMPORT Queue
        CP_queue =  new ArrayBlockingQueue <String> (COMPORT_QUEUE_SIZE);
        Control CPQueue = new Control(CP_queue , "COMPORT");
        new Thread (CPQueue).start();
        COMPortConnection = COMPort.get_instance();
        COMPortConnection.mContext = _context;
        if (COMPortConnection == null){
            Logging.write(Logging.MessageType.ERROR, "COMPortConnection(): failed", _TAG);
            return false;
        }
    return true;
    }
    private static void initController(){



    }

}
