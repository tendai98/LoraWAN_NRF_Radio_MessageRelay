package com.mercy.hit.messageterminal;

import androidx.appcompat.app.AppCompatActivity;

import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.Request.Method;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.util.ArrayList;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

    private RequestQueue messageQueue;
    private EditText messageOutputView, messageInputView;
    private ArrayList<String> messageList;
    private boolean isLinkActive = false;
    private Handler handler;
    private Message msgObject;

    private final Thread messageThread = new Thread(new Runnable() {
        @Override
        public void run() {
            while (true){
                if(isLinkActive) {
                    try {
                        StringRequest message = new StringRequest(Method.GET, "http://192.168.4.1/rx_packet", response -> {
                            if(!response.contains("RADIO: NO RX DATA")) {
                                msgObject = new Message();
                                msgObject.obj = response.replace("Z", "");
                                handler.handleMessage(msgObject);
                            }

                            }, error -> {
                            });

                            messageQueue.add(message);
                        } catch (Exception ignored) {
                    }
                }
                try{
                    Thread.sleep(300);
                }catch (Exception ignored){}
            }
        }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        messageList = new ArrayList<>();
        messageOutputView = findViewById(R.id.message_output);
        messageInputView = findViewById(R.id.message_input);

        Button sendButton = findViewById(R.id.send_button);
        Button clearButton = findViewById(R.id.clear_button);
        Button linkButton = findViewById(R.id.link_button);

        messageQueue = Volley.newRequestQueue(getApplicationContext());

        handler = new Handler(Looper.getMainLooper()){
            @Override
            public void handleMessage(Message message){
                String response = message.obj.toString();
                messageList.add(String.format("RECV MSG <<< %s", response.replace("Z", "")));

                if (messageList.size() > 12) {
                    messageList.clear();
                }

                StringBuilder stringBuilder = new StringBuilder();

                for (String messageText : messageList) {
                    stringBuilder.append(messageText);
                    stringBuilder.append("\n");
                }

                messageOutputView.setText(stringBuilder);
            }
        };

        sendButton.setOnClickListener(view -> {
                if(isLinkActive) {
                    String msg = messageInputView.getText().toString();

                    String messageUrl = String.format(Locale.ENGLISH, "http://192.168.4.1/tx_packet?d=%s", Uri.encode(msg + "Z"));
                    StringRequest message = new StringRequest(Method.GET, messageUrl, response -> {
                        messageList.add(String.format("MSG SENT >>> %s", msg));

                        if (messageList.size() > 12) {
                            messageList.clear();
                        }

                        StringBuilder stringBuilder = new StringBuilder();

                        for (String messageText : messageList) {
                            stringBuilder.append(messageText);
                            stringBuilder.append("\n");
                        }

                        messageOutputView.setText(stringBuilder);
                        messageInputView.setText("");

                        Toast.makeText(getApplicationContext(), "Message Sent", Toast.LENGTH_SHORT).show();
                    }, error -> {
                        Log.e("ERROR: ", error.toString());
                        Toast.makeText(getApplicationContext(), "Message Not Sent", Toast.LENGTH_SHORT).show();
                    });

                    messageQueue.add(message);
                }else{
                    Toast.makeText(getApplicationContext(), "Not Connected", Toast.LENGTH_SHORT).show();
                }

        });

        clearButton.setOnClickListener(view -> {
            messageInputView.setText("");
            messageOutputView.setText("");
        });

        linkButton.setOnClickListener(view -> {
            if(isLinkActive){
                isLinkActive = false;
                linkButton.setText("OFFLINE");
                Toast.makeText(getApplicationContext(), "LINK OFFLINE", Toast.LENGTH_LONG).show();
            }else{
                isLinkActive = true;
                linkButton.setText("ONLINE");
                Toast.makeText(getApplicationContext(), "LINK ONLINE", Toast.LENGTH_LONG).show();
            }
        });

        messageThread.start();
    }
}