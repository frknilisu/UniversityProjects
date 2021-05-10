package com.frknilisu.workflow;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

import org.json.JSONArray;
import org.json.JSONException;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    ListView listView;
    SwipeRefreshLayout swipeLayout;

    ArrayList<String> fileList;
    ArrayList<Integer> filesVersion;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        listView = (ListView) findViewById(R.id.listView);

        getListOfFiles();

        swipeLayout = (SwipeRefreshLayout) findViewById(R.id.swipe_container_main);
        swipeLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
            @Override
            public void onRefresh() {
                getListOfFiles();
            }
        });

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {

                Intent i = new Intent(getApplicationContext(), FileEditorActivity.class);
                i.putExtra("fileId", position);
                i.putExtra("fileVersion", filesVersion.get(position));
                startActivity(i);

            }
        });

    }

    private void getListOfFiles() {
        fileList = new ArrayList<>();
        filesVersion = new ArrayList<>();
        Client client = new Client(Action.LIST);
        client.execute();

        client.delegate = new AsyncResponse() {
            @Override
            public void processFinish(String response) {
                JSONArray jsonArray = null;
                try {

                    jsonArray = new JSONArray(response);
                    for (int i = 0; i < jsonArray.length(); i++) {
                        fileList.add(jsonArray.getJSONObject(i).getString("name"));
                        filesVersion.add(jsonArray.getJSONObject(i).getInt("version"));
                    }

                    ArrayAdapter<String> listAdapter = new ArrayAdapter<String>
                            (getApplicationContext(), R.layout.list_item, fileList);

                    listView.setAdapter(listAdapter);
                    swipeLayout.setRefreshing(false);

                } catch (JSONException e) {
                    e.printStackTrace();
                }
            }
        };
    }

}
