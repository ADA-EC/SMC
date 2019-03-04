package ada_geisa.com.bluetoothterminal.activity;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import ada_geisa.com.bluetoothterminal.R;
import ada_geisa.com.bluetoothterminal.utils.FileUtils;

public class FileCreationActivity extends AppCompatActivity {

    private EditText editNomeArquivo, editConteudo;
    private Button buttonSalvar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_filecreation);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] {Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1000);
        }

        editNomeArquivo = (EditText)findViewById(R.id.editNomeArquivo);
        editConteudo = (EditText)findViewById(R.id.editConteudo);
        buttonSalvar = (Button)findViewById(R.id.buttonSalvar);

        buttonSalvar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String filename = editNomeArquivo.getText().toString();
                String content = editConteudo.getText().toString();

                if(!filename.equals("")) {
                    if(!content.equals("")) {
                        FileUtils.saveTextAsFile(filename, ".txt", content, FileCreationActivity.this);
                        finish();
                    }
                    else{
                        Toast.makeText(FileCreationActivity.this, "Insira um conteúdo para o arquivo.", Toast.LENGTH_SHORT).show();
                    }
                }
                else{
                    Toast.makeText(FileCreationActivity.this, "Insira um nome para o arquivo.", Toast.LENGTH_SHORT).show();
                }
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
}
