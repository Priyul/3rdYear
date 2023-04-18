import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class AppointmentServer {
    private static final int PORT = 12345;

    public static void main(String[] args) {
        AppointmentDatabase appointmentDatabase = new AppointmentDatabase();
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Appointment server started. Listening on port " + PORT);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Client connected: " + clientSocket.getRemoteSocketAddress());
                ClientHandler clientHandler = new ClientHandler(clientSocket, appointmentDatabase);
                clientHandler.start();
            }
        } catch (IOException e) {
            System.out.println("Error starting server: " + e.getMessage());
        }
    }
}
