import org.json.JSONException;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class TCPServer {

    private static final int ServerPort = 4236;


    public static void main(String[] args) {

        System.out.println("Server running at [" + ServerPort + "]");

        try {

            Helper.init();

            // Create listener Server Socket(passive socket) at port 4236
            ServerSocket listener = new ServerSocket(ServerPort);

            while (true) {

                // Wait, on welcoming socket for contact by client

                Socket aClient = listener.accept();     // active socket

                // print out client's address
                System.out.println("Connection from " + aClient.getInetAddress().getHostAddress());

                ClientThread clientThread = new ClientThread(aClient);
                clientThread.run();

                // loop back and wait for another client connection
            }

        } catch (IOException e) {
            System.err.println("Server aborted:" + e);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

}
