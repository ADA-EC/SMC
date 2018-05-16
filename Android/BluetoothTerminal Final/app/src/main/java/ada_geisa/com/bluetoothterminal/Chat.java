package ada_geisa.com.bluetoothterminal;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ActivityNotFoundException;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import ada_geisa.com.bluetoothterminal.File_Creation;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

/**
 * Created by henri on 31/08/2017.
 */

public class Chat extends AppCompatActivity implements Bluetooth.CommunicationCallback {
    private String name;
    private Bluetooth b;
    private EditText message;
    private Button send;
    private Button opt1;
    private Button opt2;
    private TextView text;
    private ScrollView scrollView;
    private boolean registered=false;
    private Button create;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1000);
        }

        text = (TextView)findViewById(R.id.text);
        message = (EditText)findViewById(R.id.message);
        send = (Button)findViewById(R.id.send);
        scrollView = (ScrollView) findViewById(R.id.scrollView);
        opt1 = (Button)findViewById(R.id.opt1);
        opt2 = (Button)findViewById(R.id.opt2);
        create = (Button)findViewById(R.id.create);

        text.setMovementMethod(new ScrollingMovementMethod());
        send.setEnabled(false);

        b = new Bluetooth(this);
        b.enableBluetooth();

        b.setCommunicationCallback(this);

        int pos = getIntent().getExtras().getInt("pos");
        name = b.getPairedDevices().get(pos).getName();

        Display("Conectando...");
        b.connectToDevice(b.getPairedDevices().get(pos));

        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String msg = message.getText().toString().trim();
                message.setText("");
                String msg2 = msg + "\r";
                b.send(msg2);
                Display("Você: "+msg);
                if(msg.equals("mandatudo")){
                    DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH'h'mm'm'ss's'");
                    Date date = new Date();
                    Toast.makeText(Chat.this, dateFormat.format(date), Toast.LENGTH_SHORT).show();
                    saveTextAsFile(dateFormat.format(date), "funf");
                }
                else;
            }
        });

        opt1.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                message.setText("leituraatual");
            }
        });

        opt2.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                message.setText("mandatudo");
            }
        });

        create.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent i = new Intent(Chat.this, File_Creation.class);
                startActivity(i);
            }
        });

        IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(mReceiver, filter);
        registered=true;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(registered) {
            unregisterReceiver(mReceiver);
            registered=false;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_main, menu);
        return true;
    }


    public void Display(final String s){
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                text.append(s + "\n");
                scrollView.fullScroll(View.FOCUS_DOWN);
            }
        });
    }

    @Override
    public void onConnect(BluetoothDevice device) {
        Display("Conectado à "+device.getName()+" - "+device.getAddress());
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                send.setEnabled(true);
            }
        });
    }

    @Override
    public void onDisconnect(BluetoothDevice device, String message) {
        Display("Desconectado!");
        Display("Conectando novamente...");
        b.connectToDevice(device);
    }

    @Override
    public void onMessage(String message) {
        Display(name+": "+message);
    }

    @Override
    public void onError(String message) {
        Display("Erro: "+message);
    }

    @Override
    public void onConnectError(final BluetoothDevice device, String message) {
        Display("Erro: "+message);
        Display("Tentando de novo em 3 segundos.");
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        b.connectToDevice(device);
                    }
                }, 2000);
            }
        });
    }

    private void saveTextAsFile(String filename, String content){
        String fileName = filename + ".csv";

        //Create file
        File mediaStorageDir = new File(Environment.getExternalStorageDirectory(), "Geisa Planilhas");

        if (!mediaStorageDir.exists()) {
            if (!mediaStorageDir.mkdirs()) {
                Log.d("App", "failed to create directory");
            }
        }
        File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/Geisa Planilhas/", fileName);
        if (file.exists()) {
            file.delete();
        }
        try {
            file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }

        //write to file
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(content.getBytes());
            fos.close();
            Toast.makeText(this, "Salvo", Toast.LENGTH_SHORT).show();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Toast.makeText(this, "Arquivo não encontrado", Toast.LENGTH_SHORT).show();
        } catch (IOException e){
            e.printStackTrace();
            Toast.makeText(this, "Erro ao salvar", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch(requestCode){
            case 1000:
                if(grantResults[0] == PackageManager.PERMISSION_GRANTED){
                    Toast.makeText(this, "Permissão concedida", Toast.LENGTH_SHORT).show();
                } else{
                    Toast.makeText(this, "Permissão não concedida", Toast.LENGTH_SHORT).show();
                    finish();
                }
        }
    }


    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);
                Intent intent1 = new Intent(Chat.this, Select.class);

                switch (state) {
                    case BluetoothAdapter.STATE_OFF:
                        if(registered) {
                            unregisterReceiver(mReceiver);
                            registered=false;
                        }
                        startActivity(intent1);
                        finish();
                        break;
                    case BluetoothAdapter.STATE_TURNING_OFF:
                        if(registered) {
                            unregisterReceiver(mReceiver);
                            registered=false;
                        }
                        startActivity(intent1);
                        finish();
                        break;
                }
            }
        }
    };
}
