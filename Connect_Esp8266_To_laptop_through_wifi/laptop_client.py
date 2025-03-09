import socket
import time

# Change this to your laptop's IP address
SERVER_IP = "192.168.137.1"  # Replace with your laptop's actual IP
SERVER_PORT = 1369            # Must match the server's port

# Create a TCP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Try to connect to the server
    client_socket.connect((SERVER_IP, SERVER_PORT))
    print(f"Connected to {SERVER_IP}:{SERVER_PORT}")

    while True:
        # Send a test message
        message = "Hello from Python client!"
        client_socket.sendall(message.encode())

        # Receive response from server
        response = client_socket.recv(1024).decode()
        print(f"Server: {response}")    
        time.sleep(3)
    

except Exception as e:
    print(f"Connection failed: {e}")

finally:
    client_socket.close()