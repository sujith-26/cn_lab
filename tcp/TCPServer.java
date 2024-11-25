import java.io.*;
import java.net.*;
public class TCPServer {
    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(12345)) { // Port number
            System.out.println("Server started. Waiting for a client...");
            try (Socket clientSocket = serverSocket.accept();
                 BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
                 PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {
                System.out.println("Client connected: " + clientSocket);
                String message;
                while ((message = in.readLine()) != null) {
                    System.out.println("Client: " + message);
                    out.println("Server: " + message); // Echo message back to the client
                }
            } catch (IOException e) {
                System.err.println("Error in client communication: " + e.getMessage());
            }
        } catch (IOException e) {
            System.err.println("Could not start server: " + e.getMessage());
        }
    }
}
