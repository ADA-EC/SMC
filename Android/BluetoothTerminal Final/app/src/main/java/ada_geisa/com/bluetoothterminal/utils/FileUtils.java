package ada_geisa.com.bluetoothterminal.utils;

import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.widget.Toast;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/*
    infos do .csv:
    Data, Hora, Temperatura, Umidade, Metano
*/

public class FileUtils {

    public static void saveTextAsFile(String filename, String extension, String content, Context context){
        String fileName = filename + extension;

        //Criar arquivo
        File mediaStorageDir = new File(Environment.getExternalStorageDirectory(), "Geisa arquivos");

        if (!mediaStorageDir.exists()) {
            if (!mediaStorageDir.mkdirs()) {
                Log.d("App", "Falha em criar o diretório.");
                Toast.makeText(context, "Falha em criar o diretório.", Toast.LENGTH_SHORT).show();
            }
        }
        File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/Geisa arquivos/", fileName);
        if (file.exists()) {
            file.delete();
        }
        try {
            if(!file.createNewFile()){
                Toast.makeText(context, "Falha em criar o arquivo.", Toast.LENGTH_SHORT).show();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        //Escrever no arquivo
        try {
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(content.getBytes());
            fos.close();
            Toast.makeText(context, "Salvo com sucesso!", Toast.LENGTH_SHORT).show();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Toast.makeText(context, "Arquivo não encontrado.", Toast.LENGTH_SHORT).show();
        } catch (IOException e){
            e.printStackTrace();
            Toast.makeText(context, "Erro ao salvar no arquivo.", Toast.LENGTH_SHORT).show();
        }
    }

}
