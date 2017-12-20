#include<LiquidCrystal.h>
#include<MsTimer2.h>
#define TRIG_PIN 6
#define ECHO_PIN 10


int INA = 9;
int INB = 8;
int LED = 7;


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

double R1=10000.0;
double V_IN =5.0;
double A = 0.001129148;
double B= 0.000234125;
double C = 0.0000000876741;
double adcRaw;
double Vout;
double R_th;
double kelvin;
double celsius;
unsigned char moterPwm;
int stop1=0;
unsigned char count;
unsigned char seconds;
unsigned char seconds2;
unsigned char minutes;
unsigned char hours;
boolean ledState=0;


void setup() {
  lcd.begin(8,2);
 lcd.setCursor(0,0);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(INA,OUTPUT);
  pinMode(INB,OUTPUT);
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
  moterPwm=10;

  MsTimer2::set(10,clock);
  MsTimer2::start();

}

void loop() {

   adcRaw = analogRead(0);
 Vout = adcRaw /1024 * V_IN;
 R_th = (R1 * Vout) / (V_IN - Vout);
 kelvin = SteinhartHart(R_th);
 celsius = kelvin - 273.15;

  lcd.setCursor(0,0);
 displayDigits(hours);
 displayDigits(minutes);
 displayDigits(seconds);
 printTemp(celsius);
 delay(1000);
int distance = 0;

digitalWrite(TRIG_PIN,HIGH);
delayMicroseconds(500); 
digitalWrite(TRIG_PIN,LOW); 
distance = pulseIn(ECHO_PIN,HIGH)/58.2;
Serial.print("distance:"+(String)distance);
Serial.println("cm");

 if(distance <= 10) {
    abc();
 } 
 
 else {
  if(seconds2<=10){
    seconds = 10 - seconds2;
   if(seconds<=3)
      digitalWrite(LED,HIGH);
 
  }
  
  else {
    seconds=0;
     digitalWrite(INA,LOW);
      digitalWrite(INB,LOW);
      digitalWrite(LED,LOW);
  }
   
 }


 //delay(200);
  
}

void displayDigits(int digits) {
  if(digits < 10) lcd.print('0');
  lcd.print(digits);
  lcd.print(":");
}

void clock() {
  count++;
  if(count == 100) {
    count = 0;
    seconds++;
    seconds2++;
    ledState = !ledState;
   // digitalWrite(14, ledState);
    
  }
  if(seconds == 60) {
    minutes++;
    seconds = 0;
    if(minutes == 60) {
      hours++;
      minutes = 0;
      if(hours == 24) {
        hours = 0;
      }
    }
  }
}

void abc(){
  seconds2=0;
  seconds = 10;
  minutes = 0;
   hours = 0;
  moterPwm += 5;
  if(celsius<25) moterPwm = 60;
  else moterPwm = 200;
  analogWrite(INA,moterPwm);
  analogWrite(INB,LOW);
  // digitalWrite(INA,HIGH);
  // digitalWrite(INB,LOW);
}
double SteinhartHart(double R)
{
  double logR = log(R);
  double logR3 = logR  *logR *logR ;
  return 1.0 / (A + B *logR + C *logR3);
}

void printTemp(double temp) {
  lcd.setCursor(1,1);
  lcd.print(temp);
  lcd.print("C ");
}
