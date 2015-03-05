package com.wistone.app;



import com.wistone.app.utilities.Logging;

import java.util.concurrent.BlockingQueue;

/**
 * Created by yagel on 28/07/2014.
 */



public class Control implements Runnable {
    private BlockingQueue<String> _queue;
    private String id=null;
    private static String _ServerIP = "10.0.0.7";
    public static String lastMassageFromCP ="";
    public static String lastMassageFromTAP ="";
    private static final String     _TAG = "Controller";
    public Control(BlockingQueue<String> Q, String idIn){
        _queue=Q;
        id = idIn;

    }
    public void run(){
        try{
            while(true){
                String input_str = _queue.take();
                handleInputStr(input_str);
            }
        }catch (InterruptedException e){
            Logging.write(Logging.MessageType.ERROR, "run():_queue.take()", _TAG, e);
        }

    }
    private void handleInputStr(String input_str){
        if (id=="TCPClient"){
            lastMassageFromTAP = input_str;
            handleInputFromTCP(input_str);
        }
        if (id=="COMPORT"){
            lastMassageFromCP = input_str;
            handleInputFromCP(input_str);
        }

    }
    private void handleInputFromTCP(String input_str){
        String[] cmd_toks = input_str.split(" ");

        if ( cmd_toks[0].equals("hub".toString())) {//command for the android

            if (cmd_toks[1].equals("gstat".toString())){
                Logging.write(Logging.MessageType.DEBUG_SEVERE, "handleInputFromTCP() : hub gstat - OK" , _TAG);
                StateMachine.tcpClient.write(getAndroidStatus());
                if (StateMachine.COMPortConnection != null) {
                    StateMachine.COMPortConnection.write("plug stat".toString());
                }
            }
        }
        else if ( cmd_toks[0].equals("plug".toString())) {
            if (StateMachine.COMPortConnection != null) {
                if (cmd_toks[1].equals("phone_reset".toString())) {
                    StateMachine.COMPortConnection.write("plug phone_reset".toString());
                    StateMachine.tcpClient.write("Sent CP phone reset\r\n".toString());
                }
            }
        }
        else {//command is transferred to the CP
            if (StateMachine.COMPortConnection != null) {
                StateMachine.COMPortConnection.write(input_str);

            }
        }
    }
    private void handleInputFromCP(String input_str){
        Logging.write(Logging.MessageType.DEBUG_SEVERE, "handleInputFromCP: msg: " + input_str, _TAG);
        if (StateMachine.tcpClient != null && StateMachine.tcpClient.is_Connected()) {
            StateMachine.tcpClient.write(input_str);
        }
    }


    public static String  getAndroidStatus(){
        String status = "\r\nHub status: \r\n";
        if (StateMachine.tcpClient != null) {
            status = status + "TCP Connection: " + String.valueOf(StateMachine.tcpClient.is_Connected()) + "; \r\n";
        }
        if (StateMachine.COMPortConnection != null) {
            status = status + "COM PORT Connection: " + String.valueOf(StateMachine.COMPortConnection.GetStatus()) + "; \r\n";
        }
        status = status + "Battery Level: " + String.valueOf(StateMachine._sensor.getPhoneBatteryLevel()) + ";\r\n";
        status = status + "Battery Temperature: " + String.valueOf(StateMachine._sensor.getPhoneBatteryTemperature()) + "; \r\n";
        status = status + "Charging Status: " + String.valueOf(StateMachine._sensor.getPhoneBatteryChargingStatus()) + "; \r\n";
        return status;
    }
    public static String getServerIP(){
        return _ServerIP;
    }
    public static void updateServerIP(String ServerIP){

        _ServerIP = ServerIP;//TODO add check for legal IP
        StateMachine.tcpClient.restartTheClient();
    }


}
