#include <AFMotor.h>
#include <Servo.h>

#define lin1 A8
#define lin2 A9
#define lin3 A10
#define echoPin 52
#define trigPin 50
#define LUZ A15
#define ojo 42
#define pi 38
#define RED 28
#define GREEN 29
#define BLUE 27

// DC motor on M2
AF_DCMotor motori2(1);
AF_DCMotor motord2(2);
AF_DCMotor motord1(3);
AF_DCMotor motori1(4);
// DC hobby servo
Servo servo1;
Servo servo2;

int command;
int acelera = 200;
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
  servo1.attach(10);
  servo2.attach(9);
  servo1.write(0);
  servo2.write(180);
  // turn on motor #2
  motori1.setSpeed(acelera);
  motord1.setSpeed(acelera);
  motori2.setSpeed(acelera);
  motord2.setSpeed(acelera);
  motori1.run(RELEASE);
  motord1.run(RELEASE);
  motori2.run(RELEASE);
  motord2.run(RELEASE);

  pinMode(pi, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


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
  pinMode(33,OUTPUT);
  digitalWrite(33,LOW);
  pinMode(43,OUTPUT);
  digitalWrite(43,LOW);
}

void loop() {
  motord1.run(FORWARD);
  //select();
  //data();
  //Send();
  //react();
  delay(50);
}

void select() {
  if (Serial3.available() > 0) {
    command = Serial3.read();
    Serial3.flush();
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
      //case '-': while(Serial3.available()) Serial3.read(); delay(10); break;
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
  Serial.println(dis);
}

void Send() {
  Serial3.println(" [dis=" + String(dis) + "luz=" + String(luz) + "lin=" + String(lin) + "aut=" + String(automatic) + "Fin] ");
}

void react() {
  if (dis < 10) {
    detente();
  }
  if (automatic) {
    led(0,0,255);
    if (luz) digitalWrite(ojo, LOW);
    else digitalWrite(ojo, HIGH);
    
    motori1.setSpeed(acelera/4);
    motord1.setSpeed(acelera/4);
    motori2.setSpeed(acelera/4);
    motord2.setSpeed(acelera/4);
    
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
  motori1.run(FORWARD);
  motord1.run(FORWARD);
  motori2.run(FORWARD);
  motord2.run(FORWARD);
}
void atras() {
  motori1.run(BACKWARD);
  motord1.run(BACKWARD);
  motori2.run(BACKWARD);
  motord2.run(BACKWARD);
}
void izquierda() {
  motori1.run(FORWARD);
  motord1.run(BACKWARD);
  motori2.run(FORWARD);
  motord2.run(BACKWARD);
}
void derecha() {
  motori1.run(BACKWARD);
  motord1.run(FORWARD);
  motori2.run(BACKWARD);
  motord2.run(FORWARD);
}
void diagonalIzquierda() {
  adelante();
  motori1.setSpeed(acelera);
  motord1.setSpeed(acelera / 2);
  motori2.setSpeed(acelera);
  motord2.setSpeed(acelera / 2);
}
void diagonalDerecha() {
  adelante();
  motori1.setSpeed(acelera / 2);
  motord1.setSpeed(acelera);
  motori2.setSpeed(acelera / 2);
  motord2.setSpeed(acelera);
}
void retocederIzquierda() {
  atras();
  motori1.setSpeed(acelera / 2);
  motord1.setSpeed(acelera);
  motori2.setSpeed(acelera / 2);
  motord2.setSpeed(acelera);
}
void retrocederDerecha() {
  atras();
  motori1.setSpeed(acelera);
  motord1.setSpeed(acelera / 2);
  motori2.setSpeed(acelera);
  motord2.setSpeed(acelera / 2);
}
void detente() {
  motori1.run(RELEASE);
  motord1.run(RELEASE);
  motori2.run(RELEASE);
  motord2.run(RELEASE);
  motori1.setSpeed(acelera);
  motord1.setSpeed(acelera);
  motori2.setSpeed(acelera);
  motord2.setSpeed(acelera);
  digitalWrite(pi, LOW);
  servo1.write(0);
  servo2.write(180);
  dance = false;
  automatic = false;
}
