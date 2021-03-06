package ada_geisa.com.bluetoothterminal.activity;

import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

import ada_geisa.com.bluetoothterminal.utils.Bluetooth;
import ada_geisa.com.bluetoothterminal.R;


/**
 * Created by henri on 31/08/2017.
 */

public class ScanActivity extends AppCompatActivity implements Bluetooth.DiscoveryCallback, AdapterView.OnItemClickListener{
    private Bluetooth bluetooth;
    private ListView listView;
    private ArrayAdapter<String> adapter;
    private TextView state;
    private ProgressBar progress;
    private Button scan;
    private List<BluetoothDevice> devices;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.scan);

        listView = (ListView)findViewById(R.id.scan_list);
        state = (TextView) findViewById(R.id.scan_state);
        progress = (ProgressBar) findViewById(R.id.scan_progress);
        scan = (Button) findViewById(R.id.scan_scan_again);

        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, new ArrayList<String>());
        listView.setAdapter(adapter);
        listView.setOnItemClickListener(this);

        bluetooth = new Bluetooth(this);
        bluetooth.setDiscoveryCallback(this);

        bluetooth.scanDevices();
        progress.setVisibility(View.VISIBLE);
        state.setText("Procurando...");
        listView.setEnabled(false);

        scan.setEnabled(false);
        devices = new ArrayList<>();

        scan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        adapter.clear();
                        scan.setEnabled(false);
                    }
                });

                devices = new ArrayList<>();
                progress.setVisibility(View.VISIBLE);
                state.setText("Procurando...");
                bluetooth.scanDevices();
            }
        });
    }

    private void setText(final String txt){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                state.setText(txt);
            }
        });
    }

    private void setProgressVisibility(final int id){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                progress.setVisibility(id);
            }
        });
    }

    @Override
    public void onFinish() {
        setProgressVisibility(View.INVISIBLE);
        setText("Procura finalizada!");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                scan.setEnabled(true);
                listView.setEnabled(true);
            }
        });
    }

    @Override
    public void onDevice(final BluetoothDevice device) {
        final BluetoothDevice tmp = device;
        devices.add(device);

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                adapter.add(tmp.getAddress()+" - "+tmp.getName());
            }
        });
    }

    @Override
    public void onPair(BluetoothDevice device) {
        setProgressVisibility(View.INVISIBLE);
        setText("Pareado!");
        Intent i = new Intent(ScanActivity.this, SelectActivity.class);
        startActivity(i);
        finish();
    }

    @Override
    public void onUnpair(BluetoothDevice device) {
        setProgressVisibility(View.INVISIBLE);
        setText("Pareado!");
    }

    @Override
    public void onError(String message) {
        setProgressVisibility(View.INVISIBLE);
        setText("Erro: "+message);
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
        setProgressVisibility(View.VISIBLE);
        setText("Pareando...");
        bluetooth.pair(devices.get(position));
    }
}
