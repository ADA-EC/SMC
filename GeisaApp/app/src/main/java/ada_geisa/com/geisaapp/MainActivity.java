package ada_geisa.com.geisaapp;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageButton;


public class MainActivity extends AppCompatActivity {

    public ImageButton myButton;

    public ImageButton myButton2;

    public ImageButton myButton3;

    public ImageButton myButton4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        myButton = (ImageButton) findViewById(R.id.imageButton);
        myButton2 = (ImageButton) findViewById(R.id.imageButton2);
        myButton3 = (ImageButton) findViewById(R.id.imageButton3);
        myButton4 = (ImageButton) findViewById(R.id.imageButton4);

        myButton.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v1) {
                Intent intentLoadNewActivity = new Intent(MainActivity.this, NewActivity.class);
                startActivity(intentLoadNewActivity);
            }
        });
        myButton2.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v2){
                Intent intentLoadNewActivity2 = new Intent(MainActivity.this, NewActivity2.class);
                startActivity(intentLoadNewActivity2);
            }
        });
        myButton3.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v3){
                Intent intentLoadNewActivity3 = new Intent(MainActivity.this, NewActivity3.class);
                startActivity(intentLoadNewActivity3);
            }
        });
        myButton4.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v4){
                Intent intentLoadNewActivity4 = new Intent(MainActivity.this, NewActivity4.class);
                startActivity(intentLoadNewActivity4);
            }
        });
    }
}
