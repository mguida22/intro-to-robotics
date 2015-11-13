package robotics.jeeves;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.inputmethodservice.InputMethodService;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.ListView;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.UUID;


public class MainActivity extends Activity {

    private ArrayList<BluetoothDevice> devices = new ArrayList<>();
    private BluetoothAdapter mBluetooth;
    private ArrayAdapter mAdapter;
    private ArrayList<String> names = new ArrayList<>();

    private AdapterView.OnItemClickListener mMessageHandler = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            bluetooth = new BTConnection(devices.get(position));
        }
    };

    private final BroadcastReceiver btReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if(device != null && device.getName() != null){
                    names.add(device.getName());
                    devices.add(device);
                }
            }
        }
    };

    private class BTConnection extends Thread {
        private BluetoothSocket sock = null;
        private InputStream mmInStream = null;
        private OutputStream mmOutStream = null;

        public BTConnection(BluetoothDevice newDevice) {
            BluetoothSocket tmp = null;
            try {
                tmp = newDevice.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805f9b34fb"));
                Log.i("Jeeves", "Created socket with device");
            } catch (IOException e) {
                Log.e("Jeeves", "Failed in socket creation");
                e.printStackTrace();
            }
            sock = tmp;
        }

        public void run() {
            byte[] buffer = new byte[1024];
            int bytes;

            try{
                sock.connect();
                Log.i("Jeeves", "Connected socket with device");
                mmInStream = sock.getInputStream();
                Log.i("Jeeves", "Connected inputstream");
                mmOutStream = sock.getOutputStream();
                Log.i("Jeeves", "Connected outputstream");
            } catch (IOException e){
                e.printStackTrace();
            }

            while(true) {
                try {
                    if (mmInStream != null && mmInStream.available() > 0) {
                        bytes = mmInStream.read(buffer);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void park() {
            try {
                mmOutStream.write("1".getBytes(), 0, 1);
                mmOutStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void retrieve() {
            try {
                mmOutStream.write("0".getBytes(), 0, 1);
                mmOutStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    private BTConnection bluetooth;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, names);

        ListView mListView = (ListView) findViewById(R.id.listView);
        ((AdapterView<ListAdapter>) mListView).setAdapter(mAdapter);
        mListView.setOnItemClickListener(mMessageHandler);

        //registering a filter allows us to catch specific actions
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(btReceiver, filter);

        //the bluetooth adapter is where we discover devices
        mBluetooth = BluetoothAdapter.getDefaultAdapter();
        devices = new ArrayList<>();

        //this is where we ask to enable bluetooth
        Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 0);
        startActivity(discoverableIntent);

        //start discovering devices -- this is handled in the broadcast receiver
        mBluetooth.startDiscovery();
    }
}
