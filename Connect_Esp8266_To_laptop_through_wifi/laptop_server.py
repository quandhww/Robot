'''
netsh advfirewall set allprofiles state on
=> turn of or off firewall
'''

import socket

# Server settings
serverIP_wlan0 = "192.168.50.1"
HOST = serverIP_wlan0  # Listen on all available interfaces
PORT = 1369       # Port number

# Create a socket (IPv4, TCP)
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))  # Bind to address & port
server_socket.listen(1)  # Listen for incoming connections

print(f"Listening on {HOST}:{PORT}...")

# Accept connection from ESP8266
client_socket, addr = server_socket.accept()
print(f"Connected by {addr}")

while True:
    # Receive data from ESP8266
    data = client_socket.recv(1024).decode()
    if not data:
        break  # Stop if no data
    print(f"ESP8266: {data}")

    # Send a response to ESP8266
    response = "Hello from Laptop!"
    client_socket.send(response.encode())

# Cleanup
client_socket.close()
server_socket.close()