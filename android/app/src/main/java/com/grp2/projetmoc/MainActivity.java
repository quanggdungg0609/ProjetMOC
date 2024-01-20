package com.grp2.projetmoc;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity {
    private FirebaseAuth mAuth=FirebaseAuth.getInstance();
    private FirebaseUser user;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    public void auth(View view){
        EditText mail=findViewById(R.id.edtMail);
        EditText pwd=findViewById(R.id.edtMDP);
        if(mail.getText().toString().equals("") || pwd.getText().toString().equals("") ){
            Log.i("USER", ""+user);
            Toast.makeText(getApplicationContext(), "veuillez saisir l'@mail et le mot de passe!", Toast.LENGTH_SHORT).show();
        }else{
            Log.i("USER", ""+user);
            if (user != null){
                Toast.makeText(getApplicationContext(), "deja authentifie!", Toast.LENGTH_SHORT).show();
            }else{
                Task<AuthResult> taskAuth = mAuth.signInWithEmailAndPassword(mail.getText().toString(), pwd.getText().toString());
                taskAuth.addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            user = mAuth.getCurrentUser();
                            String uid = mAuth.getUid();
                            Log.i("TAG", "OK");
                            Toast.makeText(getApplicationContext(), "ok", Toast.LENGTH_SHORT).show();
                            Intent intent = new Intent(MainActivity.this, Main2Activity.class);
                            // Ajoutez les données de l'utilisateur à l'Intent
                            intent.putExtra("userId", user.getUid());

                            // Démarrez la nouvelle activité
                            startActivity(intent);

                        } else {
                            Log.w("TAG", "échoué", task.getException());
                            Toast.makeText(getApplicationContext(), "échoué!", Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }
        }
    }
}