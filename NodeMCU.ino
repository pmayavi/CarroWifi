#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "carrowifi-e260d-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "qiyxouWLFQT9F27406VP6oAvBDyh6vKOny3mA181"
#define WIFI_SSID "MAYA"
#define WIFI_PASScomD "71666348"

String command = "";
String commandOLD = "";
String data = "";
float dis;
bool luz;
bool lin;
bool aut;

float dis0 = 0;
bool luz0 = false;
bool lin0 = false;
bool aut0 = false;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASScomD);
  //Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //Serial.println();
  //Serial.print("connected: ");
  //Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(1000);
  dis = Firebase.getFloat("Distancia");
  luz = Firebase.getBool("Luz");
  lin = Firebase.getBool("Linea");
  aut = Firebase.getBool("Auto");
}

void loop() {
  getCommand();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  readSerial();
  Send();
  delay(50);
}

void getCommand() {
  command = Firebase.getString("command");
  command.replace("\"", "");
  if (command != commandOLD) {
    commandOLD = command;
    Serial.println(command);
  }
}

void readSerial() {
  if (Serial.available()) {
    if (Serial.read() == '[') {
      data = Serial.readStringUntil(']');
      //Serial.println(data);
      if (data.indexOf("dis=") != -1)
        dis = data.substring(data.indexOf("dis=") + 4, data.indexOf("luz=")).toFloat();
      if (data.indexOf("luz=") != -1)
        luz = data.substring(data.indexOf("luz=") + 4, data.indexOf("lin=")).toInt() == 1;
      if (data.indexOf("lin=") != -1)
        lin = data.substring(data.indexOf("lin=") + 4, data.indexOf("aut=")).toInt() == 1;
      if (data.indexOf("aut=") != -1)
        aut = data.substring(data.indexOf("aut=") + 4, data.indexOf("Fin")).toInt() == 1;
      Serial.flush();
    }
  }
}

void Send() {
  if (dis != dis0) {
    dis0 = dis;
    Firebase.setFloat("Distancia", dis);
  } if (luz != luz0) {
    luz0 = luz;
    Firebase.setBool("Luz", luz);
  } if (lin != lin0) {
    lin0 = lin;
    Firebase.setBool("Linea", lin);
  } if (aut != aut0) {
    aut0 = aut;
    Firebase.setBool("Auto", aut);
  }
}
