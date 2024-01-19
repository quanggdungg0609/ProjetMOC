package com.grp2.projetmoc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class Main2Activity extends AppCompatActivity {


    private FirebaseAuth mAuth;
    private FirebaseUser user;
    FirebaseDatabase db=FirebaseDatabase.getInstance("https://projet-lse-group-2-default-rtdb.europe-west1.firebasedatabase.app");
    DatabaseReference ref=db.getReference("2bd6419e-c040-4310-abcf-2e7226820d2f");
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        String userId = getIntent().getStringExtra("userId");

        // Initialisez Firebase
        mAuth = FirebaseAuth.getInstance();

        Log.w("TAG", mAuth.toString());


        TextView tvNom =findViewById(R.id.NomModele);
        TextView tvEtatCapteur =findViewById(R.id.etatCapteur);

        TextView tvDist = findViewById(R.id.dist);
        TextView tvTemp = findViewById(R.id.temp);
        ref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot ds : dataSnapshot.getChildren()) {
                    String name = ds.getKey().toString();
                    String value = ds.getValue().toString();

                    if (name.equals("model")) {
                        Log.i("model", value);
                        tvNom.setText(value);
                    }
                    if (name.equals("etat-capteur")) {
                        tvEtatCapteur.setText(value);
                    }

                }

                if (dataSnapshot.exists()) {
                    // Vérifiez si le nœud "dist" existe dans le nœud "value"
                    Log.i("dataSnapshot", String.valueOf(dataSnapshot));
                    if (!dataSnapshot.child("data").child("dist").getValue(String.class).equals("")) {

                        String distValue = dataSnapshot.child("data").child("dist").getValue(String.class);
                        tvDist.setText("Distance : " + distValue);
                    } else {
                        tvDist.setText("Distance : NaN");
                    }

                    // Vérifiez si le nœud "temp" existe dans le nœud "value"
                    Log.i("dataSnapshot", String.valueOf(dataSnapshot.child("data").child("temp").getValue(String.class).equals("")));
                    Log.i("dataSnapshot", dataSnapshot.child("data").child("temp").getValue(String.class));
                    if (!dataSnapshot.child("data").child("temp").getValue(String.class).equals("")) {
                        String tempValue = dataSnapshot.child("data").child("temp").getValue(String.class);
                        tvTemp.setText("Température : " + tempValue);
                    } else {
                        tvTemp.setText( "Température: NaN");
                    }
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
            }
        });

        ;


        Button onButton = findViewById(R.id.on_btn);
        onButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Ajoutez la valeur à la base de données
                on();
            }
        });

        Button offButton = findViewById(R.id.off_btn);
        offButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                off();
            }
        });

        Button logout = findViewById(R.id.logout);
        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Appeler la méthode logout()
                logout();
            }
        });

    }
public void on(){


    // Ajoutez la valeur à la base de données
    ref.child("etat-capteur").setValue("ON");

}
public void off(){
        ref.child("etat-capteur").setValue("OFF");
    }
public void logout(){
    Intent intent = new Intent(Main2Activity.this, MainActivity.class);
    // Ajoutez les données de l'utilisateur à l'Intent

    Log.i("logout", String.valueOf(intent));

    // Démarrez la nouvelle activité
    startActivity(intent);

}



}