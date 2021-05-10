import java.net.*;
import java.io.*;

public class TCPClient {

    public static void main(String argv[]) throws IOException{
        String sentence;
        String modifiedSentence;

        // Create input stream
        BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));

        // Create client socket, connect to server
        Socket clientSocket = new Socket("localhost",4236);

        // Create output stream attached to socket
        DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());

        // Create input stream attached to socket
        BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));

        sentence = inFromUser.readLine();

        // Send line to server
        outToServer.writeBytes(sentence + '\n');

        // Read line from server
        modifiedSentence = inFromServer.readLine();

        System.out.println("FROM SERVER: " + modifiedSentence);

        clientSocket.close();
    }

}
