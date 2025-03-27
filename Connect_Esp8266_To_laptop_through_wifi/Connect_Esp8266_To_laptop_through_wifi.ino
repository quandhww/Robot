#include <ESP8266WiFi.h>

#define MAX_NUMBER_OF_MSG 4

const char* ssid = "RP_Hotspot";     // Replace with your Wi-Fi name
const char* password = "test1234"; // Replace with your Wi-Fi password
const char* serverIP = "192.168.50.1";  // Replace with your laptop's IP
const int serverPort = 1369;            // Port to connect


WiFiClient g_client;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    int countForTryConnecting = 0;
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        ++countForTryConnecting;
        Serial.print("Connecting to WiFi... ");
        Serial.print(countForTryConnecting);
        Serial.println(" time");
    }
    Serial.println("Connected to WiFi!");
    Serial.println(WiFi.localIP());
    countForTryConnecting = 0;
    // Try to connect to server
    while(g_client.connect(serverIP, serverPort) == false)
    {
        Serial.print("Connection failed, error code: ");
        Serial.println(g_client.status());  // Prints error code
        ++countForTryConnecting;
        Serial.println("Try again...");  // Prints error code
        Serial.print(countForTryConnecting);
        Serial.println(" time");
        delay(1000);
    }
    Serial.println("Connected to server!");
}

bool g_isClientReconnectionRequested = false;
bool g_isWorkDone = false;
int numberOfMsgSent = 0;

void Reconnect()
{
  g_client.stop();
  int count = 0;
  while(g_client.connect(serverIP, serverPort) == false)
  {
      Serial.print("Re-connection server failed, error code: ");
      Serial.println(g_client.status());  // Prints error code
      ++count;
      Serial.println("Try re-connect server again...");  // Prints error code
      Serial.print(count);
      Serial.println(" time");
      delay(1000);
  }
  Serial.println("Re-connected to server!");

}

bool IsMsgSentToServer(String& msg, unsigned long& timeDelayAfterSendingMsg)
{
    unsigned long timeout = millis() + 5000;  // 5-second timeout
    int count_for_waiting_for_ack = 0;
    while (g_client.available() == 0) {
      if (millis() > timeout) {
        Serial.println("No msg received back! Disconnect and Re-try connecting");
        return false;
      }
      ++count_for_waiting_for_ack;
      Serial.print("Send msg again! ");
      Serial.print(count_for_waiting_for_ack);
      Serial.println("time.");
      g_client.print(msg);
      delay(timeDelayAfterSendingMsg);
    }
    return true;
}

int g_countForSendingMsgToServer = 0;
unsigned long g_timeDelayAfterSendingMsgToServer = 100;
void loop() {


    if(g_isClientReconnectionRequested == true)
    {
      Reconnect();
      g_isClientReconnectionRequested = false;
    }
    
    if(g_isWorkDone == false && g_client.connected() == false)
    {
      g_isClientReconnectionRequested = true;
      return;
    }

    if(numberOfMsgSent == MAX_NUMBER_OF_MSG) return;
    // Send data to Laptop (Server)
    if(numberOfMsgSent == MAX_NUMBER_OF_MSG-1)
    {
        String msg = "Disconnect from ESP!";
        g_client.print(msg);
        delay(100);
        if(IsMsgSentToServer(msg, g_timeDelayAfterSendingMsgToServer) == false)
        {
          g_isClientReconnectionRequested = true;
          return;
        }
        if (g_client.available()) {
            Serial.print("Laptop: ");
            while (g_client.available()) {  // Ensure we read all data
                char c = g_client.read();
                Serial.print(c);
            }
            Serial.println();
        }
        g_client.stop();
        g_isWorkDone = true;
        Serial.println("DONE STOP");
        ++numberOfMsgSent;
        return;
    }

    Serial.println("START");

    ++g_countForSendingMsgToServer;
    String msg = String("Hello from ESP8266! ") + String(g_countForSendingMsgToServer) + " time.";
    g_client.print(msg);
    g_client.flush();
    delay(g_timeDelayAfterSendingMsgToServer);  // Wait for response
    
    //read response from server
    if(IsMsgSentToServer(msg, g_timeDelayAfterSendingMsgToServer) == false){
      g_isClientReconnectionRequested = true;
      return;
    } 

    if (g_client.available()) {
        Serial.print("Laptop: ");
        while (g_client.available()) {  // Ensure we read all data
            char c = g_client.read();
            Serial.print(c);
        }
        Serial.println();
    }
    else
    {
        Serial.println("No response received");
    }
    
    ++numberOfMsgSent;
    delay(3000); // Wait before sending again
}



// #include <ESP8266WiFi.h>

// const char* ssid = "RP_Hotspot";     // Replace with your Wi-Fi name
// const char* password = "test1234"; // Replace with your Wi-Fi password
// const char* serverIP = "192.168.50.1";  // Replace with your laptop's IP
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
//     // if(i == 3)
//     // {
//     //     return;
//     // }
//     Serial.println("START");
//     // ++i;
//     client.print("Hello from ESP8266!");
//     delay(500);  // Wait for response
//     // Read response from Laptop
//     if (client.available()) {
//         Serial.print("Laptop: ");
//         while (client.available()) {  // Ensure we read all data
//             char c = client.read();
//             Serial.print(c);
//         }
//         Serial.println();
//     }
//     else
//     {
//         Serial.println("No response received");
//     }
    
//     // if(i == 3)
//     // {
//     //   client.stop();
//     //   Serial.println("DONE STOP");
//     // }
//     client.stop();
//     Serial.println("DONE STOP");

//     delay(3000); // Wait before sending again
// }






// /*CODE FOR connecting to wifi hotspot without using DHcP from rasp*/
// #include <ESP8266WiFi.h>

// const char* ssid = "RP_Hotspot";     // Replace with your Wi-Fi SSID
// const char* password = "test1234";       // Replace with your Wi-Fi password

// WiFiClient client;
// const char* serverIP_wlan0 = "192.168.50.1";
// const char* serverIP = serverIP_wlan0;  // Replace with your laptop's IP
// const int serverPort = 1369;            // Port to connect

// IPAddress local_IP(192, 168, 137, 2);   // Manually assigned IP for ESP8266
// IPAddress gateway(192, 168, 137, 1);    // Laptop's IP (hotspot gateway)
// IPAddress subnet(255, 255, 255, 0);     // Subnet mask
// IPAddress dns(8, 8, 8, 8);              // Google DNS (or use your laptop's DNS)

// int i = 0;

// void setup() {
//     Serial.begin(115200);

//     // Manually configure static IP
//     if (!WiFi.config(local_IP, gateway, subnet, dns)) {
//         Serial.println("Failed to configure static IP!");
//     }

//     WiFi.begin(ssid, password);
    
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(1000);
//         Serial.println("Connecting to WiFi...");
//     }

//     Serial.println("Connected to WiFi!");
//     Serial.print("ESP8266 IP: ");
//     Serial.println(WiFi.localIP()); // Should print 192.168.137.2

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
// /*========================*/





/*Code for testing connection*/
// #include <ESP8266WiFi.h>

// const char* ssid = "RP_Hotspot";     
// const char* password = "test1234";   

// void setup() {
//     Serial.begin(115200);
//     Serial.println("Connecting to Wi-Fi...");

//     WiFi.begin(ssid, password);

//     int attempt = 0;
//     while (WiFi.status() != WL_CONNECTED && attempt < 20) {  // Try for 20 seconds
//         delay(1000);
//         Serial.print(".");
//         attempt++;
//     }

//     Serial.println();
//     if (WiFi.status() == WL_CONNECTED) {
//         Serial.println("Connected!");
//         Serial.print("ESP8266 IP Address: ");
//         Serial.println(WiFi.localIP());  // Should be in 192.168.50.x range
//     } else {
//         Serial.println("Connection failed. Check credentials and DHCP settings.");
//     }
// }

// void loop() {}















