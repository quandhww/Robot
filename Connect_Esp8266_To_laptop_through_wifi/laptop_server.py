'''
netsh advfirewall set allprofiles state on
=> turn on or off firewall
'''
'''
const char* ssid = "RP_Hotspot";     // Replace with your Wi-Fi name
const char* password = "test1234"; // Replace with your Wi-Fi password
'''


import socket
import signal
import sys



# Server settings
serverIP_wlan0 = "192.168.50.1" # Ip address of wifi hotspot on Raspberry Pi 4
HOST = serverIP_wlan0  # Listen on all available interfaces
PORT = 1369       # Port number

# Create a socket (IPv4, TCP)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))  # Bind to address & port
server_socket.listen(1)  # Listen for incoming connections

print(f"Listening on {HOST}:{PORT}...")



# Handle Ctrl+C gracefully
def cleanup_and_exit(sig, frame):
    print("\nShutting down server...")
    client_socket.close()
    server_socket.close()
    sys.exit(0)
try:  
    while True:
        print("Waiting for connection!")
        # Accept connection from ESP8266
        client_socket, addr = server_socket.accept()
        print(f"Connected by {addr}")
        # **Set a timeout for receiving data**
        client_socket.settimeout(5.0)  # Prevents blocking indefinitely
        while True:
            try:
                # Receive data from ESP8266
                data = client_socket.recv(1024).decode()
                if data == "Disconnect from ESP!":
                    print("Client disconnected")
                    break  # Stop
                print(f"ESP8266: {data}")

                # Send a response to ESP8266
                response = "Hello from Raspberry Pi!"
                client_socket.send(response.encode())
            except socket.timeout:
                print("Timeout: Close client and reconnecting...!")
                client_socket.close()
                break
except KeyboardInterrupt:
    cleanup_and_exit(None, None)



# Cleanup
client_socket.close()
server_socket.close()