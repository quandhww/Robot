// #include <ESP8266WiFi.h>

// const char* ssid = "laptop_hotspot";     // Replace with your Wi-Fi name
// const char* password = "test1234"; // Replace with your Wi-Fi password
// const char* serverIP = "192.168.137.1";  // Replace with your laptop's IP
// const int serverPort = 1369;            // Port to connect
// int i = 0;

// WiFiClient client;

// void setup() {
//     Serial.begin(115200);
//     WiFi.begin(ssid, password);

//     // Wait for connection
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Connecting to WiFi...");
//     }
//     Serial.println("Connected to WiFi!");
//     Serial.println(WiFi.localIP());

//     // Try to connect to server
//     if (client.connect(serverIP, serverPort)) {
//         Serial.println("Connected to server!");
//     } else {
//         Serial.print("Connection failed, error code: ");
//         Serial.println(client.status());  // Prints error code
//     }
// }

// void loop() {
//     // Send data to Laptop (Server)
//     Serial.println("START");
//     ++i;
//     client.print("Hello from ESP8266!");

//     // Read response from Laptop
//     if (client.available()) {
//         String response = client.readStringUntil('\n');
//         Serial.print("Laptop: ");
//         Serial.println(response);
//     }
    
//     if(i == 3)
//     {
//       client.stop();
//       Serial.println("DONE STOP");
//     }

//     delay(3000); // Wait before sending again
// }






/*CODE FOR testing if vpn blocks DHCP*/
#include <ESP8266WiFi.h>

const char* ssid = "laptop_hotspot";     // Replace with your Wi-Fi SSID
const char* password = "test1234";       // Replace with your Wi-Fi password

IPAddress local_IP(192, 168, 137, 2);   // Manually assigned IP for ESP8266
IPAddress gateway(192, 168, 137, 1);    // Laptop's IP (hotspot gateway)
IPAddress subnet(255, 255, 255, 0);     // Subnet mask
IPAddress dns(8, 8, 8, 8);              // Google DNS (or use your laptop's DNS)

WiFiClient client;
const char* serverIP = "192.168.137.1";  // Replace with your laptop's IP
const int serverPort = 1369;            // Port to connect
int i = 0;


void setup() {
    Serial.begin(115200);

    // Manually configure static IP
    if (!WiFi.config(local_IP, gateway, subnet, dns)) {
        Serial.println("Failed to configure static IP!");
    }

    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi!");
    Serial.print("ESP8266 IP: ");
    Serial.println(WiFi.localIP()); // Should print 192.168.137.2

    // Try to connect to server
    if (client.connect(serverIP, serverPort)) {
        Serial.println("Connected to server!");
    } else {
        Serial.print("Connection failed, error code: ");
        Serial.println(client.status());  // Prints error code
    }

}

void loop() {
    // Send data to Laptop (Server)
    Serial.println("START");
    ++i;
    client.print("Hello from ESP8266!");

    // Read response from Laptop
    if (client.available()) {
        String response = client.readStringUntil('\n');
        Serial.print("Laptop: ");
        Serial.println(response);
    }
    
    if(i == 3)
    {
      client.stop();
      Serial.println("DONE STOP");
    }

    delay(3000); // Wait before sending again
}
/*========================*/






/*Code for pinging laptop*/
// #include <ESP8266WiFi.h>

// void setup() {
//     Serial.begin(115200);
//     WiFi.begin("laptop_hotspot", "test1234");

//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Connecting to Wi-Fi...");
//     }

//     Serial.println("Connected! Pinging laptop...");

//     IPAddress serverIP;
//     if (WiFi.hostByName("192.168.137.1", serverIP)) {
//         Serial.print("Resolved IP: ");
//         Serial.println(serverIP);
//     } else {
//         Serial.println("Ping failed!");
//     }
// }

// void loop() {}