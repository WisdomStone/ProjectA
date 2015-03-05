package com.wistone.app;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;

import com.wistone.app.utilities.Logging;

import java.io.RandomAccessFile;

/**
 * Created by yagel on 29/07/2014.
 */
public class Sensors {
    private int current_temp;
    private int current_bat_level;
    private int charge_status;
    private static final String     _TAG = "Sensor";
    private BroadcastReceiver mBatInfoReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context _context, Intent intent) {
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "onReceive(): - OK", _TAG);
            current_bat_level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, 0);
            current_temp = intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE, 0);
            charge_status =  intent.getIntExtra(BatteryManager.EXTRA_STATUS,0) ;
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "onReceive(): temp: " + String.valueOf(current_temp) + " battary: " + String.valueOf(current_bat_level) +"charging: " + String.valueOf(charge_status), _TAG);
            current_temp /= 10;
        }
    };
    public Sensors(){
        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "Sensors(): c-tor - OK", _TAG);
        StateMachine._context.registerReceiver(this.mBatInfoReceiver, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
    }
    public int getPhoneBatteryLevel(){
        return current_bat_level;
    }
    public int getPhoneBatteryTemperature(){
        return current_temp;
    }
    public int getPhoneBatteryChargingStatus(){
        return charge_status==2?1:0;
    }
    public void getCPUUsage(){
        try{
            RandomAccessFile reader = new RandomAccessFile("/proc/stat/", "r");
            String load = reader.readLine();
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "getCPUUsage():" + load, _TAG);
        } catch (Exception e) {
            Logging.write(Logging.MessageType.ERROR, "getCPUUsage():", _TAG, e);
        }
    }
}
