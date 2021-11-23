
#include <Servo.h>

#define lin1 A15
#define lin2 A14
#define lin3 A13
#define echoPin 52
#define trigPin 50
#define LUZ A12
#define ojo 46
#define pi 11
#define RED 3
#define GREEN 5
#define BLUE 7

Servo servo1;
Servo servo2;

int command;
float dis;
bool luz;
bool lin;
bool automatic = false;
bool dance = false;
int danceVal = 40;

void setup() {
  Serial3.begin(115200);
  Serial.begin(9600);

  // turn on servo
  servo1.attach(2);
  servo2.attach(9);
  servo1.write(0);
  servo2.write(180);

  pinMode(pi, OUTPUT);
  pinMode(ojo, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for(int i=30; i<34; i++){
    pinMode(i, OUTPUT);
  }


//INTENTAR QUITAARMIERDAS
  pinMode(48,OUTPUT);
  digitalWrite(48,HIGH);
  pinMode(51,OUTPUT);
  digitalWrite(51,HIGH);
  pinMode(53,OUTPUT);
  digitalWrite(53,HIGH);
  pinMode(22,OUTPUT);
  digitalWrite(22,LOW);
  pinMode(23,OUTPUT);
  digitalWrite(23,LOW);
  pinMode(26,OUTPUT);
  digitalWrite(26,LOW);
  pinMode(8,OUTPUT);
  digitalWrite(8,LOW);
}

void loop() {
  select();
  data();
  Send();
  react();
  delay(50);
}

void select() {
  if (Serial3.available() > 0) {
    command = Serial3.read();
    switch (command) {
      case 'F': adelante(); break;
      case 'B': atras(); break;
      case 'L': izquierda(); break;
      case 'R': derecha(); break;
      case 'G': diagonalIzquierda(); break;
      case 'I': diagonalDerecha(); break;
      case 'H': retocederIzquierda(); break;
      case 'J': retrocederDerecha(); break;
      case 'W': digitalWrite(pi, HIGH); break;
      case 'A': automatic = true; break;
      case 'V': digitalWrite(ojo, HIGH); break;
      case 'v': digitalWrite(ojo, LOW); break;
      case 'D': servo1.write(150); break;
      case 'Z': servo2.write(30); break;
      case 'X': dance = true; break;
      case 'S': detente(); break;
      case '-': while(Serial3.available()) Serial3.read(); delay(10); break;
    }
    char y = command;
    Serial.println(y);
  }
}

void data() {
  luz = analogRead(LUZ) < 500;
  lin = analogRead(lin2) < 35;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  dis = pulseIn(echoPin, HIGH) * 0.034 / 2.0;
  //Serial.println(dis);
}

void Send() {
  Serial3.println(" [dis=" + String(dis) + "luz=" + String(luz) + "lin=" + String(lin) + "aut=" + String(automatic) + "Fin] ");
  Serial.println(" [dis=" + String(dis) + "luz=" + String(luz) + "lin=" + String(lin) + "aut=" + String(automatic) + "Fin] ");
}

void react() {
  if (dis < 10) {
    detente();
  }
  if (automatic) {
    led(0,0,255);
    if (luz) digitalWrite(ojo, LOW);
    else digitalWrite(ojo, HIGH);
    
    while (lin) {
      if (luz) digitalWrite(ojo, LOW);
      else digitalWrite(ojo, HIGH);
      
      while (analogRead(lin1) < 35) {
        izquierda();
      }
      while (analogRead(lin3) < 35) {
        derecha();
      }
      adelante();
      data();
    }
  }else led(0,255,0);
  if(dance){
    led(255,0,0);
    servo1.write(danceVal);
    servo2.write(danceVal);
    
    danceVal += 100;
    if(danceVal > 140) danceVal = 40;
    delay(500);
  }
}

void led(int r, int g, int b){
  analogWrite(RED,r);
  analogWrite(GREEN,g);
  analogWrite(BLUE,b);
}

void adelante() {
  digitalWrite(33,HIGH);
}
void atras() {
  digitalWrite(32,HIGH);
}
void izquierda() {
  digitalWrite(31,HIGH);
}
void derecha() {
  digitalWrite(30,HIGH);
}
void diagonalIzquierda() {
  digitalWrite(33,HIGH);
  digitalWrite(31,HIGH);
}
void diagonalDerecha() {
  digitalWrite(33,HIGH);
  digitalWrite(30,HIGH);
}
void retocederIzquierda() {
  digitalWrite(32,HIGH);
  digitalWrite(31,HIGH);
}
void retrocederDerecha() {
  digitalWrite(32,HIGH);
  digitalWrite(30,HIGH);
}
void detente() {
  digitalWrite(33,LOW);
  digitalWrite(32,LOW);
  digitalWrite(31,LOW);
  digitalWrite(30,LOW);
  digitalWrite(pi, LOW);
  servo1.write(0);
  servo2.write(180);
  dance = false;
  automatic = false;
}
