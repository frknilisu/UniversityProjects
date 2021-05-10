package com.frknilisu.workflow;

import android.os.AsyncTask;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by frkn on 03.12.2017.
 */

public class Client extends AsyncTask<Object, Object, String> {

    private static final String ServerAddress = "192.168.1.129";
    private static final int ServerPort = 4236;

    private Action action;
    private int fileId, clientVersion, serverVersion;
    private String content;
    private String response;

    private DataOutputStream outToServer;
    private DataInputStream inFromServer;

    AsyncResponse delegate = null;

    Client(Action action) {
        this.action = action;
    }

    @Override
    protected String doInBackground(Object... params) {

        Socket socket = null;

        try {

            // Create client socket, connect to server
            socket = new Socket(ServerAddress, ServerPort);

            // Create output stream attached to socket
            outToServer = new DataOutputStream(socket.getOutputStream());

            // Create input stream attached to socket
            inFromServer = new DataInputStream(socket.getInputStream());


            switch (action) {
                case LIST:

                    outToServer.writeByte(Action.LIST.code());
                    response = receiveMessage();

                    break;

                case READ:

                    // Send line to server
                    outToServer.writeByte(Action.READ.code());
                    sendMessage(Integer.toString(fileId));
                    serverVersion = Integer.valueOf(receiveMessage());
                    clientVersion = serverVersion;

                    System.out.println("clientVersion: " + clientVersion);
                    System.out.println("serverVersion: " + serverVersion);

                    sendMessage("OK");
                    response = receiveMessage();
                    response += ("\nclientVersion: " + clientVersion);
                    //System.out.println ("FROM SERVER: " + response);

                    break;


                case UPDATE:

                    // Send line to server
                    outToServer.writeByte(Action.UPDATE.code());
                    sendMessage(Integer.toString(fileId));
                    receiveMessage();
                    sendMessage(Integer.toString(clientVersion));
                    serverVersion = Integer.valueOf(receiveMessage());

                    System.out.println("clientVersion: " + clientVersion);
                    System.out.println("serverVersion: " + serverVersion);

                    if(clientVersion < serverVersion){

                        sendMessage("OLD");
                        String serverContent = receiveMessage();

                        response = "===== Server Version " + serverVersion + " =====\n";
                        response += serverContent;
                        response += "\n\n===== Your Version " + clientVersion + " =====\n";
                        response += content;

                    } else {

                        sendMessage(content);
                        response = receiveMessage();
                        clientVersion += 1;
                        response = "clientVersion: " + Integer.toString(clientVersion);
                    }

                    break;

            }

        } catch (UnknownHostException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            response = "UnknownHostException: " + e.toString();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            response = "IOException: " + e.toString();
        } finally {
            if (socket != null) {
                try {
                    outToServer.close();
                    inFromServer.close();
                    socket.close();
                } catch (IOException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                    response = "IOException: " + e.toString();
                }
            }
        }
        return response;
    }

    @Override
    protected void onPostExecute(String response) {
        delegate.processFinish(response);
    }

    private void sendMessage(String message) throws IOException {
        // Write out line to socket
        outToServer.write(message.getBytes("UTF-8"));
        outToServer.flush();
    }

    private String receiveMessage() throws IOException {

        // Read in line from socket
        while (inFromServer.available() <= 0) ;
        int length = inFromServer.available();
        byte[] buff = new byte[length];
        inFromServer.read(buff, 0, length);
        return new String(buff);
    }

    public void setFileId(int fileId) {
        this.fileId = fileId;
    }

    public void setVersionId(int version) {
        this.clientVersion = version;
    }

    public void setContent(String content) {
        this.content = content;
    }
}
