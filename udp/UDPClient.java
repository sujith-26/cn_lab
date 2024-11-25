import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Scanner;

public class UDPClient {

    public static void main(String[] args) {
        DatagramSocket socket = null;
        try {
            socket = new DatagramSocket();
            InetAddress serverAddress = InetAddress.getByName("localhost");
            int serverPort = 9876;
            Scanner scanner = new Scanner(System.in);

            while (true) {
                System.out.print("Enter message: ");
                String message = scanner.nextLine();
                byte[] sendBuffer = message.getBytes();
                DatagramPacket sendPacket = new DatagramPacket(sendBuffer, sendBuffer.length, serverAddress, serverPort);
                socket.send(sendPacket);

                byte[] receiveBuffer = new byte[1024];
                DatagramPacket receivePacket = new DatagramPacket(receiveBuffer, receiveBuffer.length);
                socket.receive(receivePacket);
                String responseMessage = new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println("Received response: " + responseMessage);
            }
        } catch (IOException e) {
        } finally {
            if (socket != null && !socket.isClosed()) {
                socket.close();
            }
        }
    }
}