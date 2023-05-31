/***************************************************************************************
* Description:  Remote Control Sockets
* Sketch:       ESP32_NOW_RECEIVE_SOCKETS.ino
* Version:      1.0
* Version Desc: Original Draft Code
* Board:        XIAO_ESP32-C3 
* Author:       Steve Fuller
* Website:      Version control by GitHUB https://github.com/SF1960/remote_socket_control/tree/main
* Local:        C:\Documents\Github\remote_socket_control
* Comments:     This is the main code to receive to the remote sockets. This revision 
* drives are relay and uses the NO contact to check that the relay has engaged
****************************************************************************************

/***************************************************************************************
  ESP-NOW Demo - Receive The RESPONDER
  ESP32_NOW_RECEIVE.ino
  Reads data from Initiator (Transmitter)
  
  DroneBot Workshop 2022
  https://dronebotworkshop.com/esp-now/

  Load this code to the RESPONDER

  ** The MAC Address(es) of THIS device MUST be loaded in the Transmit Sketch **
  See https://docs.google.com/spreadsheets/d/1zeHhUQ800quf7LlVyviluqHowV3Big6YN5qjG9SoFbc/edit#gid=0

*****************************************************************************************/

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>

#define BAUD 115200             // this can be any baud rate
#define LED 25                  // to indicate that the output is ON
#define RELAY_1 26              // output to energise relay (for feedback)
#define RELAY_2 27              // output to energise relay (mains power)

// THIS SOCKET'S ID. This code WITH THIS NUMBER MUST be loaded to the ESP32 attached to the socket
// broadcastAddress_1[] = {0x7C, 0x87, 0xCE, 0x30, 0x8D, 0x0C}; //#14 socket 1
// broadcastAddress_2[] = {0x94, 0xB5, 0x55, 0xf8, 0x30, 0x70}; //#8  socket 2 
// broadcastAddress_3[] = {0x7C, 0x87, 0xCE, 0x30, 0x91, 0xCC}; //#16 socket 3

/*******************************************************************************************************
*    CHANGE THE SOCKET NUMBER AND UPLOAD TO EACH UNIQUE CONTROLLER                                     */
const int THIS_SOCKET = 2;     
/*******************************************************************************************************/

// the broadcast socket check variable
//int socket = -1;
bool ledStatus = false;

// A data structure that MUST match the transmitter's structure
// In the future this will just include POWER_ON and POWER_OFF boolean commands
typedef struct socket_message {
  bool socket_state;            // On or Off
  int socket_number;            // which socket 1 to 3
} socket_message;

// Create an object called myData 
//struct_message myData;
socket_message myData;

// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

  memcpy(&myData, incomingData, sizeof(myData));

  if (myData.socket_number == THIS_SOCKET){ // if the socket number matches then perform the code
    Serial.println();
    Serial.print("Socket ID      : ");  Serial.println(myData.socket_number);
    Serial.print("Bytes received : ");  Serial.println(len);
    Serial.print("Boolean Value  : ");  Serial.println(myData.socket_state);
    Serial.println();
    
    ledStatus = myData.socket_state;
    if (ledStatus){
      digitalWrite(LED, HIGH);    // LED ON
      digitalWrite(RELAY_1,LOW);  // RELAY 1 ON
      digitalWrite(RELAY_2,LOW);  // RELAY 2 ON
    } else {
      digitalWrite(LED, LOW);     // LED OFF
      digitalWrite(RELAY_1,HIGH); // RELAY 1 OFF
      digitalWrite(RELAY_2,HIGH); // RELAY 2 OFF
    }

  }else {
    Serial.print("Not for socket : ");
    Serial.println(myData.socket_number);
  }

}

void setup() {

  Serial.begin(BAUD);                 // Set up Serial Monitor
  delay(1500);
  
  WiFi.mode(WIFI_STA);                // Set ESP32 as a Wi-Fi Station


  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
      Serial.println("ESP-NOW initialised");
      Serial.print("WiFi Channel: ");
      Serial.println(WiFi.channel());
  }

  // Register the ESP32 NOW callback function to a sketch function
  esp_now_register_recv_cb(OnDataRecv);

  pinMode(LED, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);

  digitalWrite(LED, LOW);      // TURN OFF LED
  digitalWrite(RELAY_1, HIGH); // TURN OFF RELAY 1
  digitalWrite(RELAY_2, HIGH); // TURN OFF RELAY 2

  Serial.println("Setup complete");
}

// No code needed for this basic sketch
void loop() {


}