package com.frknilisu.workflow;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class FileEditorActivity extends AppCompatActivity {

    int fileId;
    public static int clientVersion;
    String fileContent;
    EditText fileEditText;
    FloatingActionButton floatingActionButton;
    SwipeRefreshLayout swipeLayout;

    boolean startEditFlag = false;

    Context context;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_editor);

        context = this;

        fileEditText = (EditText) findViewById(R.id.fileEditText);
        //fileEditText.setEnabled(false);
        fileEditText.setFocusable(false);
        //fileEditText.setClickable(false);
        //fileEditText.setFocusableInTouchMode(false);
        //fileEditText.setCursorVisible(false);
        //fileEditText.setActivated(false);
        fileEditText.setTextColor(getResources().getColor(R.color.pasiveColor));

        fileId = getIntent().getIntExtra("fileId", 1);
        clientVersion = getIntent().getIntExtra("fileVersion", 1);
        getTheFileContent();

        swipeLayout = (SwipeRefreshLayout) findViewById(R.id.swipe_container_editor);
        swipeLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                getTheFileContent();
            }
        });


        floatingActionButton = (FloatingActionButton) findViewById(R.id.floatingActionButton);
        floatingActionButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                if (!startEditFlag) {
                    fileEditText.setEnabled(true);
                    fileEditText.setFocusable(true);
                    fileEditText.setClickable(true);
                    fileEditText.setFocusableInTouchMode(true);
                    fileEditText.setCursorVisible(true);
                    fileEditText.setActivated(true);
                    startEditFlag = true;

                    fileEditText.setTextColor(getResources().getColor(R.color.activeColor));

                    floatingActionButton.setImageResource(R.drawable.ic_done_white_48dp);
                } else {
                    //fileEditText.setEnabled(false);
                    fileEditText.setFocusable(false);
                    //fileEditText.setClickable(false);
                    //fileEditText.setFocusableInTouchMode(false);
                    //fileEditText.setCursorVisible(false);
                    //fileEditText.setActivated(false);
                    startEditFlag = false;

                    fileEditText.setTextColor(getResources().getColor(R.color.pasiveColor));

                    floatingActionButton.setImageResource(R.drawable.ic_mode_edit_white_48dp);

                    Client client = new Client(Action.UPDATE);
                    client.setFileId(fileId);
                    client.setVersionId(clientVersion);
                    client.setContent(fileEditText.getText().toString());
                    client.execute();

                    client.delegate = new AsyncResponse() {
                        @Override
                        public void processFinish(String response) {
                            int index = response.indexOf("clientVersion: ");
                            System.out.println("index: " + index);
                            System.out.println("substr: " + response.substring(index + 15));
                            System.out.println("Response: " + response);
                            if(index != -1) {
                                clientVersion = Integer.parseInt(response.substring(index + 15));
                                System.out.println("client version updated: " + clientVersion);
                            } else {
                                fileEditText.setText(response);

                                Toast.makeText(getApplicationContext(),
                                            "Please copy your changes\n" +
                                            "Update the file to last version by refreshing page\n" +
                                            "After that you can make your changes", Toast.LENGTH_LONG).show();
                            }

                        }
                    };
                }

            }
        });

    }


    private void getTheFileContent() {
        Client client = new Client(Action.READ);
        client.setFileId(fileId);
        client.execute();
        client.delegate = new AsyncResponse() {
            @Override
            public void processFinish(String response) {

                int index = response.indexOf("clientVersion: ");
                System.out.println("index: " + index);
                System.out.println("substr: " + response.substring(index + 15));
                if(index != -1) {
                    clientVersion = Integer.parseInt(response.substring(index + 15));
                    System.out.println("client version updated: " + clientVersion);
                }

                fileContent = response.subSequence(0, index).toString();
                fileEditText.setText(fileContent);
                swipeLayout.setRefreshing(false);
            }
        };
    }

}
