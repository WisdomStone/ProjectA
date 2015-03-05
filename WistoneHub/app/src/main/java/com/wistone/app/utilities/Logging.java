package com.wistone.app.utilities;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

/***************************************************************************************************
 * Class: Logging
 * ==============
 *
 * This class operates the logging
 *
 * Created by: Yulia Loudman
 * Creation date: 29/03/2014
 **************************************************************************************************/

public class Logging {

    public enum MessageType {
        DEBUG_VERBOSE,
        DEBUG_SEVERE,
        DEBUG_OFF,
        ERROR,
        INFO
    }
    private static BufferedWriter   _logFile = null;
    private static String           _logFileName = File.separator + "log.txt";
    private static String           _logFileFolderName = File.separator + "log";
    private static MessageType      _messageType = MessageType.DEBUG_VERBOSE;

    private static final String     _TAG = "Logging";
    public  static final String     _EOL = System.getProperty("line.separator");

    public static synchronized void open(final Activity context) {

        try {
            String _logFileFolderPath = openFolder(_logFileFolderName, context);
            String logFilePath = _logFileFolderPath + _logFileName;
            _logFile = (BufferedWriter)openFile(logFilePath, 'w');
        } catch (Exception e) {
            write(MessageType.DEBUG_SEVERE, "open(): unhandled exception", _TAG, e);
            close();
        }
    }

    public static void close() {

        try {
            if (_logFile != null) {
                _logFile.flush();
                _logFile.close();
            }
        } catch(IOException e) {
            write(MessageType.ERROR, "close(): flush() or close() failed", _TAG, e);
        }
    }

    public static synchronized void write(MessageType messageType, String message, String TAG) {

        if (messageType.ordinal() < _messageType.ordinal()) {
            return;
        }
        String fullMessage;
        if (messageType == MessageType.DEBUG_OFF) {
            fullMessage = "ERROR: unexpected messageType" + _EOL;
            Log.e(_TAG, fullMessage);
            return;
        }
        fullMessage =  message + _EOL;
        if (messageType != MessageType.ERROR) {
            Log.d(TAG, fullMessage);
        } else {
            fullMessage = "ERROR: " + fullMessage;
            Log.e(TAG, fullMessage);
        }
        try {
            if (_logFile != null) {
                _logFile.write(fullMessage);
                _logFile.flush();
            }
        } catch(IOException e) {
            Log.e(_TAG, "ERROR: write to file failed" + _EOL);
        }
    }

    public static synchronized void write(MessageType messageType, String message, String TAG, Exception exception) {

        message = message + " " + exception.toString();
        write(messageType, message, TAG);
    }

    /**********************************************************************************************
     * openFile:
     *
     * - FileInputStream - to let read bytes from file
     * - DataInputStream - wraps the byte reader to let read Java data types
     * - InputStreamReader - decodes bytes to characters
     **********************************************************************************************/
    private static Object openFile(String fileName, char mode) {

        File file = new File(fileName);
        if (!file.exists()) {
            try {
                file.createNewFile();
            } catch (IOException e) {
                write(MessageType.ERROR, "openFile(): new File failed", _TAG, e);
            }
        }
        if (mode == 'r') {
            try {
                BufferedReader bufferedReader = new BufferedReader(
                        new InputStreamReader(new DataInputStream(new FileInputStream(fileName))));
                write(MessageType.DEBUG_VERBOSE, "openFile: reader - OK", _TAG);
                return bufferedReader; // can't be null?
            } catch (FileNotFoundException e) {
                write(MessageType.ERROR, "openFile(): new FileInputStream failed", _TAG, e);
            }
        } else if (mode == 'w') {
            try {
                BufferedWriter bufferedWriter = new BufferedWriter(
                        new OutputStreamWriter(new DataOutputStream(new FileOutputStream(fileName, true)))); // true in FileOutputStream c-tor: if the file already exists - the stream is appended to it
                write(MessageType.INFO, "NEW SESSION:", _TAG);
                write(MessageType.DEBUG_VERBOSE, "openFile(): writer - OK", _TAG);
                return bufferedWriter; // can't be null?
            } catch (FileNotFoundException e) {
                write(MessageType.ERROR, "openFile(): new FileOutputStream failed", _TAG, e);
            }
        } else {
            write(MessageType.DEBUG_SEVERE, "openFile(): unexpected mode", _TAG);
        }
        return null;
    }

    /**********************************************************************************************
     * openFolder
     **********************************************************************************************/
    private static String openFolder(String folderName, final Activity context) {

        ApplicationInfo applicationInfo = context.getApplicationInfo();
        // check to avoid null pointer exception:
        if (applicationInfo == null) {
            return null;
        }
        // get the current directory and create a new folder within:
        String folderPath = applicationInfo.dataDir + folderName;
        File folder = new File(folderPath);
        if (!folder.mkdir()) {
            write(MessageType.ERROR, "openFolder(): mkdir() failed", _TAG);
        }
        else {
            write(MessageType.DEBUG_VERBOSE, "openFolder(): mkdir() - OK", _TAG);
        }
        return folderPath;
    }
}

