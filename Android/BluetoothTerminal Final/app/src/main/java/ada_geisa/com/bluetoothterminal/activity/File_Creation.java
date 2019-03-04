package ada_geisa.com.bluetoothterminal.activity;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import ada_geisa.com.bluetoothterminal.R;

public class File_Creation extends AppCompatActivity {

    EditText file_name, file_content;
    Button button_save;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_creation);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1000);
        }

        file_name = (EditText)findViewById(R.id.file_name);
        file_content = (EditText)findViewById(R.id.file_content);
        button_save = (Button)findViewById(R.id.save);

        button_save.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String filename = file_name.getText().toString();
                String content = file_content.getText().toString();

                if(!filename.equals("") && !content.equals("")) {
                    saveTextAsFile(filename, content);
                }
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
}
