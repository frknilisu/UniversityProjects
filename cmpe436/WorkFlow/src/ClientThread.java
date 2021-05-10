import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.*;
import java.net.Socket;


public class ClientThread implements Runnable {

    private Socket clientSocket;

    private DataOutputStream outToClient;
    private DataInputStream inFromClient;

    int fileId, clientVersion, serverVersion;


    public ClientThread(Socket socket) throws IOException {
        this.clientSocket = socket;

        // Create output stream, attached to socket
        outToClient = new DataOutputStream(clientSocket.getOutputStream());

        // Create input stream, attached to socket
        inFromClient = new DataInputStream(clientSocket.getInputStream());
    }

    @Override
    public void run() {
        byte actionByte = 0;

        try {

            actionByte = inFromClient.readByte();

            if (actionByte == Action.LIST.code()) {

                System.out.println(Helper.fileJson.toString());
                sendMessage(Helper.fileJson.toString());

            } else if (actionByte == Action.READ.code()) {

                fileId = Integer.valueOf(receiveMessage());

                serverVersion = getVersionOfFile(fileId);
                sendMessage(Integer.toString(serverVersion));

                receiveMessage();

                System.out.println(fileId);

                ReaderWriter theSemaphore = Helper.getSemaphoreOfTheFile(fileId);

                theSemaphore.startRead();
                System.out.println("I am in CS");
                String serverContent = readTheFile(fileId);
                theSemaphore.endRead();
                sendMessage(serverContent);
                System.out.println("I am not in CS");

            } else if (actionByte == Action.UPDATE.code()) {

                fileId = Integer.valueOf(receiveMessage());
                sendMessage("OK");
                clientVersion = Integer.valueOf(receiveMessage());

                serverVersion = getVersionOfFile(fileId);
                sendMessage(Integer.toString(serverVersion));

                ReaderWriter theSemaphore = Helper.getSemaphoreOfTheFile(fileId);

                if (clientVersion < serverVersion){
                    receiveMessage();

                    theSemaphore.startRead();
                    System.out.println("I am in CS");
                    String serverContent = readTheFile(fileId);
                    theSemaphore.endRead();
                    sendMessage(serverContent);
                    System.out.println("I am not in CS");

                } else {

                    String newContent = receiveMessage();

                    theSemaphore.startWrite();
                    System.out.println("I am in CS");
                    updateTheFile(fileId, newContent);
                    increaseVersionOfFile(fileId);
                    updateTheInitFile();
                    theSemaphore.endWrite();
                    sendMessage(String.valueOf(Action.UPDATE.code()));
                    System.out.println("I am not in CS");
                }
            }


        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        } finally {
            try {
                inFromClient.close();
                outToClient.close();
                clientSocket.close();
                System.out.println("Socket is closed");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

    }

    private void updateTheInitFile() throws IOException {
        FileOutputStream fout = new FileOutputStream("./db/init.json");

        byte[] contentInBytes = Helper.fileJson.toString().getBytes();
        fout.write(contentInBytes);
        fout.flush();
        fout.close();
    }

    private void increaseVersionOfFile(int fid) throws JSONException {
        Helper.fileJson.getJSONObject(fid).put("version", serverVersion+1);
    }

    private int getVersionOfFile(int fid) throws JSONException {
        return Helper.fileJson.getJSONObject(fid).getInt("version");
    }

    private String readTheFile(int fid) throws IOException, JSONException {

        String fname = Helper.fileJson.getJSONObject(fid).getString("name");
        FileInputStream fin = new FileInputStream("./db/" + fname);

        int length = fin.available();
        byte[] buff = new byte[length];

        while (fin.read(buff) != -1) ;
        fin.close();

        return new String(buff);
    }

    private void updateTheFile(int fid, String content) throws IOException, JSONException {

        String fname = Helper.fileJson.getJSONObject(fid).getString("name");
        FileOutputStream fout = new FileOutputStream("./db/" + fname);

        byte[] contentInBytes = content.getBytes();
        fout.write(contentInBytes);
        fout.flush();
        fout.close();
    }

    private void sendMessage(String message) throws IOException {
        // Write out line to socket
        outToClient.write(message.getBytes("UTF-8"));
        outToClient.flush();
    }

    private String receiveMessage() throws IOException {

        // Read in line from socket
        while (inFromClient.available() <= 0) ;
        int length = inFromClient.available();
        byte[] buff = new byte[length];
        inFromClient.read(buff, 0, length);
        return new String(buff);
    }
}
