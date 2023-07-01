#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <NewPing.h>
  
// const char* ssid = "TP-LINK_BFA3";
// const char* password =  "62554602";

// const char* ssid = "CGIS-M02";
// const char* password =  "Parola.1";

const char* ssid = "kvWHyNPpmqohQrpRhMSL";
const char* password =  "Santana123";

// // set the pins
#define RXD1 4
#define TXD1 5

#define RXD2 16
#define TXD2 17

#define RXD3 19 // trig 
#define TXD3 18 // echo

#define RXD4 32
#define TXD4 33

#define MAX_DISTANCE 400
  
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

NewPing sonar1 = NewPing(RXD2, TXD2, MAX_DISTANCE);
NewPing sonar2 = NewPing(RXD1, TXD1, MAX_DISTANCE);
NewPing sonar3 = NewPing(RXD3, TXD3, MAX_DISTANCE);
NewPing sonar4 = NewPing(RXD4, TXD4, MAX_DISTANCE);

NewPing* getSonarById(int id){
  NewPing* returnSonar = NULL;
  switch(id){
    case 1:
      returnSonar = &sonar1;
      break;
    case 2:
      returnSonar = &sonar2;
      break;
    case 3:
      returnSonar = &sonar3;
      break;
    case 4:
      returnSonar = &sonar4;
      break;
    default:
      break;
  }
  return returnSonar;
}

String str_measureSonarValues(int id){
  NewPing* sonar = getSonarById(id);
  String ret_str = "";
  int distance = sonar->ping_cm();
  if(distance != 0){
    ret_str = ret_str + "Sensor " + id + ": " + distance;
  }
  delay(100);
  return ret_str;
}
  
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  
  if(type == WS_EVT_CONNECT){
  
    Serial.println("Websocket client connection received");
     
  } else if(type == WS_EVT_DISCONNECT){
 
    Serial.println("Client disconnected");
  
  }
}
  
void setup(){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  Serial.println(WiFi.localIP());
  
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  
  server.begin();
}
  
void loop(){

  String message_sens1 = str_measureSonarValues(1);
  String message_sens2 = str_measureSonarValues(2);
  String message_sens3 = str_measureSonarValues(3);
  String message_sens4 = str_measureSonarValues(4);

  // ws.textAll("Broadcasted Message");
  ws.textAll(message_sens1);
  ws.textAll(message_sens2);
  ws.textAll(message_sens3);
  ws.textAll(message_sens4);

  // delay(300);
}