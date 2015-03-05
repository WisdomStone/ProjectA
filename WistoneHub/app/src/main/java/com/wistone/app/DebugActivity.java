package com.wistone.app;

import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import com.wistone.app.drivers.TCPClient;
import com.wistone.app.utilities.Logging;

import java.util.ArrayList;

/***************************************************************************************************
 * Class: DebugActivity
 * ====================
 *
 * This class operates the debug activity
 *
 * Created by: Yulia Loudman
 * Creation date: 8/06/2014
 **************************************************************************************************/

public class DebugActivity extends ActionBarActivity {

    private                 ArrayList _arrayListItems = null;
    private                 DebugAdapter _debugAdapter = null;
    public static           TCPClient _tcpClient = null;
    private static final    String _TAG = "DebugActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "onCreate(): start", _TAG);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_debug);

        // connect the xml resources to java objects:
        final ListView listViewMessages = (ListView) findViewById(R.id.listView_messages);
        final EditText editTextMessage = (EditText) findViewById(R.id.editText_message);
        final Button buttonSendMessage = (Button) findViewById(R.id.button_sendMessage);

        // set the adapter:
        _arrayListItems = new ArrayList();
        _debugAdapter = new DebugAdapter(this, _arrayListItems);
        listViewMessages.setAdapter(_debugAdapter);



        // get the tcpClient:
        Intent intent = getIntent();
        _tcpClient = (TCPClient)intent.getSerializableExtra("tcpClient");
        // start debug task:
        new DebugTask().execute();
        // onClick event listener for "Send Message" button:
        buttonSendMessage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                String message = editTextMessage.getText().toString();

                // send the message to the server:
                if (_tcpClient != null) {
                    _tcpClient.write(message);
                }

                // add the message to the items of the adapter and refresh the text box:
                _arrayListItems.add("client: " + message);
                _debugAdapter.notifyDataSetChanged();
                editTextMessage.setText("");
            }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {  // auto-generated

        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.debug, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {  // auto-generated

        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        return ((id == R.id.action_settings) || super.onOptionsItemSelected(item));
    }

/***************************************************************************************************
 * Class: DebugAdapter
 **************************************************************************************************/
    private class DebugAdapter extends BaseAdapter {

        private ArrayList _arrayListItems;
        private LayoutInflater _layoutInflater;

        public DebugAdapter(Context context, ArrayList arrayList) {
            _arrayListItems = arrayList;
            _layoutInflater = (LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        }

        @Override
        public int getCount() {
            return _arrayListItems.size();
        }

        @Override
        public Object getItem(int i) { // auto-generated
            return null;
        }

        @Override
        public long getItemId(int i) { // auto-generated
            return 0;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {

            if (view == null) { // if the view isn't null - it can be reused
                view = _layoutInflater.inflate(R.layout.activity_debug_list_item, null);
            }
            // extract the message in position "i":
            String message = (String)_arrayListItems.get(i);
            if (message != null) {
                TextView textViewListItem = (TextView)view.findViewById(R.id.textView_listItem);
                if (textViewListItem != null) {
                    textViewListItem.setText(message);
                }
            }
            return view; // return the view with the message
        }
    }

/***************************************************************************************************
 * Class: DebugTask
 **************************************************************************************************/
    private class DebugTask extends AsyncTask<Void, String, Void> { // AsyncTask<doInBackground params, onProgressUpdate params, onPostExecute params>

        @Override
        protected Void doInBackground(Void... v) { // asynchronously updates the adapter with newly received server messages
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "DebugTask: doInBackground: ", _TAG);
            if (_tcpClient != null) {
                Logging.write(Logging.MessageType.DEBUG_VERBOSE, "DebugTask: _tcpClint is not NULL: ", _TAG);
                _tcpClient._listener = new TCPClient.OnMessageReceived() {
                    @Override
                    public void messageReceived(String serverMessage) { // messageReceived implementation
                        Logging.write(Logging.MessageType.DEBUG_VERBOSE, "DebugTask: serverMessage: " + serverMessage, _TAG);
                        _arrayListItems.add("Server: " + serverMessage);
                        _debugAdapter.notifyDataSetChanged();
                        publishProgress(serverMessage); // calls onProgressUpdate


                    }
                };
                //_tcpClient.run();
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(String... serverMessages) {
            Logging.write(Logging.MessageType.DEBUG_VERBOSE, "DebugTask: onProgressUpdate: " + serverMessages, _TAG);
            super.onProgressUpdate(serverMessages);

            // add the message to the items of the adapter and notify the adapter of the change:
            _arrayListItems.add(serverMessages[0]);
            _debugAdapter.notifyDataSetChanged();

        }
    }
}
