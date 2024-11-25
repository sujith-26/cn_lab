import java.io.*;
import java.net.*;
public class TCPClient {
    public static void main(String[] args) {
        try (Socket socket = new Socket("localhost", 12345); // Server IP and port number
             BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in))) {
            System.out.println("Connected to the server.");
            String userInputLine;
            while ((userInputLine = userInput.readLine()) != null) {
                out.println(userInputLine); // Send user input to the server
                System.out.println(in.readLine()); // Read and display server response
            }
        } catch (IOException e) {
            System.err.println("Error in client communication: " + e.getMessage());
        }
    }
}
