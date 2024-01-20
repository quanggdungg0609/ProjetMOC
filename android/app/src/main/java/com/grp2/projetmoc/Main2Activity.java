package com.grp2.projetmoc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
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
        mAuth = FirebaseAuth.getInstance(); // peut etre verifier
        TextView tvModel = findViewById(R.id.tvModel);
        TextView tvId = findViewById(R.id.tvID);
        TextView tvDate = findViewById(R.id.tvDateMO);
        TextView tvEtatCapteur = findViewById(R.id.tvEtatCapteur);
        TextView tvEtatHardWare = findViewById(R.id.tvEtatHW);
        TextView tvDistance = findViewById(R.id.tvDist);
        TextView tvTemp = findViewById(R.id.tvTemp);

        Button btnOn = findViewById(R.id.btnOn);
        Button btnOff = findViewById(R.id.btnOff);
        ImageButton btnDisconnect = findViewById(R.id.btnLogout);

        ref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                for (DataSnapshot ds : dataSnapshot.getChildren()) {
                    String key = ds.getKey().toString();
                    String value = ds.getValue().toString();

                    if (key.equals("model")){
                        tvModel.setText(value);
                    }

                    if(key.equals("num-serie")){
                        tvId.setText(value);
                    }

                    if(key.equals("date-commencer")){
                        tvDate.setText("Date MO: "+value);
                    }

                    if(key.equals("etat-capteur")){
                        tvEtatCapteur.setText("État Capteur: "+ value);
                    }

                    if(key.equals("etat-hardware")){
                        tvEtatHardWare.setText("État HW: "+value);
                    }
                }

                if (dataSnapshot.exists()) {
                    // Vérifiez si le nœud "dist" existe dans le nœud "value"

                    if (!dataSnapshot.child("data").child("dist").getValue(String.class).equals("")) {

                        String distValue = dataSnapshot.child("data").child("dist").getValue(String.class);
                        tvDistance.setText("Distance : " + distValue);
                    } else {
                        tvDistance.setText("Distance : NaN");
                    }

                    // Vérifiez si le nœud "temp" existe dans le nœud "value"

                    if (!dataSnapshot.child("data").child("temp").getValue(String.class).equals("")) {
                        String tempValue = dataSnapshot.child("data").child("temp").getValue(String.class);
                        tvTemp.setText("Température : " + tempValue);
                    } else {
                        tvTemp.setText( "Température: NaN");
                    }
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });

        btnOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Ajoutez la valeur à la base de données
                on();
            }
        });

        btnOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                off();
            }
        });

        btnDisconnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Appeler la méthode logout()
                logout();
            }
        });
    }

    public void on(){
        ref.child("etat-capteur").setValue("ON");
    }
    public void off(){
        ref.child("etat-capteur").setValue("OFF");
    }


    public void logout(){
        Intent intent = new Intent(Main2Activity.this, MainActivity.class);
        // Ajoutez les données de l'utilisateur à l'Intent
        // Démarrez la nouvelle activité
        startActivity(intent);
    }
}