package com.wistone.app.drivers;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.os.Bundle;

import com.wistone.app.StateMachine;
import com.wistone.app.utilities.Logging;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.concurrent.BlockingQueue;

/**
 * Created by yagel on 28/07/2014.
 * TODO bug: when restarting lose the queue
 */
public class COMPort implements Runnable {
    transient public static         COMPort _instance = null;
    transient private static        Thread _thread = null;
    private                         UsbManager mUsbManager = null;
    private boolean                 readInProgress = false;
    transient public                Context mContext = null;
    private                         UsbInterface intf = null;
    private                         UsbEndpoint epIN = null;
    private                         UsbEndpoint epOUT = null;
    private                         UsbDeviceConnection connection = null;
    private                         UsbDevice stDevice = null;
    private static final int        _MAX_OPEN_SOCKET_RETRIES = 5;
    private static final int        _OPEN_SOCKET_SLEEP_INTERVAL_MS = 10000;
    private static final int        _READ_INTERVAL_MS = 1000;
    private static final int         _WRITE_INTERVAL_MS = 500;
    private static final int        _INPUT_BUFFER_SIZE = 1024;
    private static final int        _PRODUCT_ID = 10;
    private static final int        _VENDOR_ID = 1240;
    private String                  inputString;
    private String                  internal_connecting_status = "";
    private int                     totalByesRead=0;
    private boolean                 hasConnection = false;
    private static final String     _TAG = "COMPort";
    private static                  BlockingQueue<String> _queue;

    private COMPort(){ }

    public static COMPort get_instance(){
        if (_instance == null) {
            _instance = new COMPort(); // if new fails - an exception is thrown
            _thread = new Thread(_instance);
            _thread.setName("COMPortThread");
            _thread.start();

        }
        return _instance;
    }

    public void run() {
        int openSocketRetries = 0;

        try {
            _queue = StateMachine.CP_queue;
            _queue.put("Connecting to CP...");
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "run(): start COMPORT thread", _TAG);
            Thread.currentThread().setPriority(Thread.NORM_PRIORITY);
            while (openSocketRetries < _MAX_OPEN_SOCKET_RETRIES) {
                if (open()) {
                    readInProgress = true;
                    _queue.put("Connected to CP");
                    read();
                    return;
                }
                openSocketRetries++;

                Thread.sleep(_OPEN_SOCKET_SLEEP_INTERVAL_MS);

            }
            restartConnection();
            _queue.put("Can't connect to CP. Restarting...");
            internal_connecting_status = "restartConnection";
        } catch (Exception e) {
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "run(): unhandled exception", _TAG, e);
            internal_connecting_status = "InterruptedException";
        }

    }

    private boolean open() throws InterruptedException {
        internal_connecting_status = internal_connecting_status+"Try to open COM Port\n";
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "open(): COMPORT - OK", _TAG);

            // close input stream if exists:
           // if (_input != null) {
           //     _input.close();
            //    _input = null;
            //    Logging.write(Logging.MessageType.DEBUG_VERBOSE, "open(): closed old _input", _TAG);
            //}
            mUsbManager = (UsbManager)mContext.getSystemService (mContext.USB_SERVICE);
            if (mUsbManager == null){
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "open(): mUsbManager==null", _TAG);
                internal_connecting_status = internal_connecting_status+ "USB Manager is NULL\n";
                return  false;
            }
            HashMap<String,UsbDevice> deviceList = mUsbManager.getDeviceList();

            internal_connecting_status = internal_connecting_status+ "deviceList.isEmpty() " + String.valueOf(deviceList.isEmpty()) + "\n";
            for (HashMap.Entry<String,UsbDevice> entry : deviceList.entrySet()) {
                UsbDevice aDevice = entry.getValue();
                //if ((aDevice.getProductId() == _PRODUCT_ID) && (aDevice.getVendorId() == _VENDOR_ID)) {
                internal_connecting_status = internal_connecting_status + "productID: " +  String.valueOf(aDevice.getProductId()) + " VendorID: " + String.valueOf(aDevice.getVendorId()) + "\n";

                if (mUsbManager.hasPermission(aDevice)) {
                    internal_connecting_status = internal_connecting_status + "hasPermission\n ";
                        this.stDevice = aDevice;
                        connection = mUsbManager.openDevice(stDevice);
                        if (connection != null) {
                            this.intf = stDevice.getInterface(1);
                            this.connection.claimInterface(intf, true);
                            connection.controlTransfer(0x40, 0x03, 0x001A, 0, null, 0, 0);// baud rate 115200
                            connection.controlTransfer(0x40, 0x04, 0x0008, 0, null, 0, 0);//8 bit
                            for (int e = 0; e < intf.getEndpointCount(); e++) {
                                if (intf.getEndpoint(e).getDirection() == UsbConstants.USB_DIR_OUT) {
                                    this.epOUT = intf.getEndpoint(e);
                                }
                                if (intf.getEndpoint(e).getDirection() == UsbConstants.USB_DIR_IN) {
                                    this.epIN = intf.getEndpoint(e);
                                }
                            }
                            hasConnection = true;
                            return true;
                        }


                    }
                //}
            }

        hasConnection = false;
        return false;
    }

    private void close(){
        //close the connection first
        if (connection !=null){
            connection.close();
            connection = null;
        }
        mUsbManager = null;
        stDevice = null;
        intf = null;
        hasConnection = false;
        epOUT = null;
        epIN = null;
        readInProgress = false;
        _instance = null;
    }
    private void read() {
        internal_connecting_status = internal_connecting_status + "Start read from CP...\n ";
        totalByesRead =0;
        byte[] buffer = new byte[_INPUT_BUFFER_SIZE];
        boolean massageReceived = false;
        String s = "";
        int NumberOfByresRead = 0;
        Logging.write(Logging.MessageType.DEBUG_SEVERE, "read(): ", _TAG);
        while (readInProgress) {
            int num_of_input_bytes = connection.bulkTransfer(epIN, buffer, _INPUT_BUFFER_SIZE, _READ_INTERVAL_MS);
            NumberOfByresRead += num_of_input_bytes;
            if (num_of_input_bytes >= 1 && NumberOfByresRead<=_INPUT_BUFFER_SIZE) {

                totalByesRead += num_of_input_bytes;//for dbg
                for (int i = 0; i < num_of_input_bytes; i++) {
                    s = s + (char) (buffer[i]);
                }

                massageReceived = true;
            }
            else{
                if (massageReceived){
                    //hare s contain the string inputs from the com port
                    inputString = s;
                    s = "";
                    NumberOfByresRead = 0;
                    Logging.write(Logging.MessageType.DEBUG_SEVERE, "read(): full message:\n" + s , _TAG);
                    massageReceived = false;
                    try {
                        _queue.put(inputString);
                    }catch (InterruptedException e){
                        Logging.write(Logging.MessageType.ERROR, "read(): _queue.put()", _TAG,e);
                        internal_connecting_status = "InterruptedException in read CP";

                    }
                }
                try {
                    Thread.sleep(_READ_INTERVAL_MS);
                } catch (InterruptedException e) {
                    Logging.write(Logging.MessageType.ERROR, "read(): Thread.sleep", _TAG, e);
                }
            }
        }
    }
    public void write(String massage) {
        if (hasConnection) {
            if (connection.claimInterface(intf, true)) {
                massage += "\r";
                byte[] buf = massage.getBytes();

                int retVal = connection.bulkTransfer(epOUT, buf, buf.length, _WRITE_INTERVAL_MS);
                //check data:
                if (retVal != buf.length) {
                    Logging.write(Logging.MessageType.DEBUG_SEVERE, "write(): COMPORT - ERROR", _TAG);
                } else {
                    Logging.write(Logging.MessageType.DEBUG_SEVERE, "write(): COMPORT - OK", _TAG);
                }
            }
        }
    }

    public void restartConnection() throws InterruptedException {
        _queue.put("Restart connection with COMPORT");
        Logging.write(Logging.MessageType.DEBUG_SEVERE, "restartConnection(): ", _TAG);
        _instance.close();
        StateMachine.openCOMPORT();

    }
    public boolean GetStatus(){
        boolean CPStatus = false;
        if (mUsbManager != null && stDevice != null) {
            CPStatus = mUsbManager.hasPermission(stDevice);
        }

        return CPStatus;
    }
    public String getCP_ConnectingStatus(){
        return internal_connecting_status + "Total Byes read: " + String.valueOf(totalByesRead);
    }

}