package ada_geisa.com.bluetoothterminal.activity;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import ada_geisa.com.bluetoothterminal.utils.Bluetooth;
import ada_geisa.com.bluetoothterminal.R;
import ada_geisa.com.bluetoothterminal.utils.FileUtils;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * Created by henri on 31/08/2017.
 */

public class ChatActivity extends AppCompatActivity implements Bluetooth.CommunicationCallback {
    private String name;
    private List<String> mensagemEnviada = new ArrayList<>();
    private Bluetooth b;
    private EditText editMensagem;
    private Button botaoEnviar, botaoOpcaoLeituraAtual, botaoMandaTudo, botaoCriarArquivo, botaoSalvarTerminal, botaoLimparTela;
    private TextView textTerminal;
    private ScrollView scrollView;
    private boolean registered = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_chat);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1000);
        }

        textTerminal = (TextView)findViewById(R.id.textTitulo);
        editMensagem = (EditText)findViewById(R.id.editMensagem);

        scrollView = (ScrollView)findViewById(R.id.scrollView);

        botaoOpcaoLeituraAtual = (Button)findViewById(R.id.botaoOpcaoLeituraAtual);
        botaoMandaTudo = (Button)findViewById(R.id.botaoMandaTudo);
        botaoEnviar = (Button)findViewById(R.id.botaoEnviar);
        botaoCriarArquivo = (Button)findViewById(R.id.botaoCriarArquivo);
        botaoSalvarTerminal = (Button)findViewById(R.id.botaoSalvarTerminal);
        botaoLimparTela = (Button)findViewById(R.id.botaoLimparTela);

        textTerminal.setMovementMethod(new ScrollingMovementMethod());
        //botaoEnviar.setEnabled(false);

        /*
        b = new Bluetooth(this);
        b.enableBluetooth();

        b.setCommunicationCallback(this);

        int pos = getIntent().getExtras().getInt("pos");
        name = b.getPairedDevices().get(pos).getName();

        b.connectToDevice(b.getPairedDevices().get(pos));
        */

        Display("Conectando...");

        botaoEnviar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String pegarMensagem = editMensagem.getText().toString().trim();
                if(!pegarMensagem.isEmpty()){
                    mensagemEnviada.add(pegarMensagem);
                    editMensagem.setText("");
                    String msg2 = mensagemEnviada.get(mensagemEnviada.size()-1) + "\r";
                    //b.send(msg2);
                    Display("Você: "+mensagemEnviada.get(mensagemEnviada.size()-1));
                }

                //Debug do array list
                //Toast.makeText(ChatActivity.this, mensagemEnviada.toString(), Toast.LENGTH_SHORT).show();
            }
        });

        botaoLimparTela.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                textTerminal.setText("");
            }
        });

        botaoOpcaoLeituraAtual.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                editMensagem.setText("leituraatual");
            }
        });

        botaoMandaTudo.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                editMensagem.setText("mandatudo");
            }
        });

        botaoCriarArquivo.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent i = new Intent(ChatActivity.this, FileCreationActivity.class);
                startActivity(i);
            }
        });

        botaoSalvarTerminal.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String textoCompletoTerminal = textTerminal.getText().toString();

                DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
                Date date = new Date();

                if(!textoCompletoTerminal.isEmpty())
                    FileUtils.saveTextAsFile(dateFormat.format(date), ".txt", textoCompletoTerminal, ChatActivity.this);
                else
                    Toast.makeText(ChatActivity.this, "Terminal vazio.", Toast.LENGTH_SHORT).show();
            }
        });

        IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(mReceiver, filter);
        registered = true;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if(registered) {
            unregisterReceiver(mReceiver);
            registered=false;
        }
    }

    public void Display(final String s){
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                textTerminal.append(s + "\n");
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
                botaoEnviar.setEnabled(true);
            }
        });
    }

    @Override
    public void onDisconnect(BluetoothDevice device, String message) {
        Display("Desconectado.");
        Display("Conectando novamente...");
        b.connectToDevice(device);
    }

    @Override
    public void onMessage(String message) {
        Display(name+": "+message);

        //Se for mandatudo, salva o retorno
        if(mensagemEnviada.get(0).equals("mandatudo")){
            Thread thread = new Thread(new SalvarCSVEmThread(message));
            thread.start();
        }

        mensagemEnviada.remove(0);
    }

    @Override
    public void onError(String message) {
        Display("Erro: "+message);
    }

    @Override
    public void onConnectError(final BluetoothDevice device, String message) {
        Display("Erro ao conectar: "+message);
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

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch(requestCode){
            case 1000:
                if(grantResults[0] != PackageManager.PERMISSION_GRANTED){
                    Toast.makeText(this, "A permissão é necessária para execução correta do app.", Toast.LENGTH_SHORT).show();
                    finish();
                }
                break;
        }
    }


    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);
                Intent i = new Intent(ChatActivity.this, SelectActivity.class);

                switch (state) {
                    case BluetoothAdapter.STATE_OFF:
                    case BluetoothAdapter.STATE_TURNING_OFF:
                        if(registered) {
                            unregisterReceiver(mReceiver);
                            registered=false;
                        }
                        startActivity(i);
                        finish();
                        break;
                }
            }
        }
    };

    class SalvarCSVEmThread implements Runnable{

        private String conteudo;

        public SalvarCSVEmThread(String conteudo){
            this.conteudo = conteudo;
        }

        @Override
        public void run() {
            DateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd_HH:mm:ss");
            Date date = new Date();
            //Toast.makeText(ChatActivity.this, dateFormat.format(date), Toast.LENGTH_SHORT).show();

            FileUtils.saveTextAsFile(dateFormat.format(date), ".csv", conteudo, ChatActivity.this);
        }
    }
}
