package com.wistone.app;

import android.app.Activity;
import android.os.Handler;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.wistone.app.utilities.Logging;

/***************************************************************************************************
 * Class: MainActivity
 * ===================
 *
 * This class operates the main activity
 *
 * Created by: Yulia Loudman
 * Creation date: 29/03/2014
 **************************************************************************************************/

public class MainActivity extends Activity {

    private static MainActivity    _mainActivity = null;
    private static Handler         _handler = null; // looper
    private static final String    _TAG = "MainActivity";
    private Button bt_read_cp,bt_write_cp , bt_write_mcu, bt_read_mcu;
    private EditText et_to_cp;
    private TextView tv_read_from_cp;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        try {
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "onCreate(): start", _TAG);
            super.onCreate(savedInstanceState);
            setContentView(R.layout.activity_main); // auto-generated
            bt_write_cp = (Button)findViewById(R.id.bt_send_to_cp);
            bt_read_cp = (Button)findViewById(R.id.bt_read_last_cp);
            bt_write_mcu = (Button)findViewById(R.id.bt_mcu_write);
            bt_read_mcu = (Button)findViewById(R.id.bt_mcu_read);
            et_to_cp = (EditText)findViewById(R.id.et_send_to_cp);
            tv_read_from_cp = (TextView)findViewById(R.id.tv_read_cp);


            if (_mainActivity == null) {
                Logging.write(Logging.MessageType.INFO, "onCreate(): new _mainActivity", _TAG);
                _mainActivity = this;
                startApplication();
            } else {
                Logging.write(Logging.MessageType.INFO, "onCreate(): old _mainActivity", _TAG);
            }
        } catch(Exception e) {
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "onCreate(): unhandled exception", _TAG, e);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) { // auto-generated

        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) { // auto-generated

        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        switch (id){
            case R.id.Restart_setting : stopApplication();
                break;
            case R.id.getStatus_setting :
                tv_read_from_cp.setText(Control.getAndroidStatus());
                break;
            case R.id.getCP_connection_status_setting :
                tv_read_from_cp.setText(StateMachine.COMPortConnection.getCP_ConnectingStatus());
                break;
            case R.id.update_server_IP_setting :
                //TODO add to the controller

                Control.updateServerIP(et_to_cp.getText().toString());
                break;

        }
        return ((id == R.id.Restart_setting) || super.onOptionsItemSelected(item));
    }

    private void startApplication() {

        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "startApplication(): start", _TAG);
        _handler = new Handler();
        _handler.post(new Runnable() {
            @Override
            public void run() {
                try {
                    if (!StateMachine.singleStep(MainActivity.this)) {
                        Logging.write(Logging.MessageType.ERROR, "startApplication(): singleStep() failed", _TAG);
                        stopApplication();
                    }
                } catch(Exception e) {
                    Logging.write(Logging.MessageType.DEBUG_SEVERE, "startApplication(): unhandled exception", _TAG, e);
                }
            }
        });
    }

    private void stopApplication() {

        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "stopApplication(): start", _TAG);
        try {
           super.onDestroy();
           _mainActivity.finish(); // explicit call to prevent the user to return to this instance of activity
           Runtime.getRuntime().exit(0); // terminates the VM
        } catch(Exception e) {
            Logging.write(Logging.MessageType.DEBUG_SEVERE, "stopApplication(): unhandled exception", _TAG, e);
        }
    }
    public void onClickReadCP(View v){
        tv_read_from_cp.setText(Control.lastMassageFromCP);
    }
    public void onClickWriteCP (View v){
        StateMachine.COMPortConnection.write(et_to_cp.getText().toString());
    }
    public void onClickWriteTCPClient(View v){
        StateMachine.tcpClient.write(et_to_cp.getText().toString());
    }
    public void onClickReadTCPClient(View v){
        tv_read_from_cp.setText(Control.lastMassageFromTAP);
    }

}
