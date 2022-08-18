//통합 코드
// CDS, Dust, Sonar, vibration

// Pin 번호 설명
/*

Sonar 센서 trig 13  echo 12
CDS 센서 analog A0  Digital 2
Dust 센서 A2  3
Vibration 센서 A1

*/
#include <Time.h>
#include <TimeLib.h>

#include <avr/wdt.h>

//Pin set

// sonar( 13, 12 )
int trigPin = 13;
int echoPin = 12;
//CDS (A0, 2)
int cdsPin_anal = A0;   //조도센서 핀번호 설정
int cdsPin_digit = 2;
int cdsVal;
int cdsVal_digit;
//Dust sensor
int dustPin = A3;    
int ledP = 3;   
float voMeasured = 0;  
float calcVoltage = 0;  
float dustDensity = 0;  
float smoothDensity = 0;  

//vibration
int shake =A1;
int data;

//ground
int groundPin = A2;
int groundValue = 0;
//time check
int sec;
//int count=0;
int Time=0;

int T = 1000;  ///측정시간변화

int count2 = 0;
int Minute =0;
int Hour = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("CLEARDATA"); //초기화
  Serial.println("LABEL,Time,Distance, illumination_analog,illumination_digital,Dust,Vibration,Ground,Time2"); //라벨 작성
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(cdsPin_digit,INPUT);
  pinMode(ledP,OUTPUT);
  pinMode(shake, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

//Sonar
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58;

//Dust
 digitalWrite(ledP,LOW); // power on the LED  
 delayMicroseconds(280);  
 voMeasured = analogRead(dustPin); // read the dust value  
 delayMicroseconds(40);  
 digitalWrite(ledP,HIGH); // turn the LED off  
 delayMicroseconds(9680);  
 // 0 - 5V mapped to 0 - 1023 integer values  
 calcVoltage = voMeasured * (5.0 / 1024.0);  
 dustDensity = 0.17 * calcVoltage - 0.1;  
 dustDensity = dustDensity*1000; // ug/m3  
 smoothDensity = dustDensity * 0.05 + smoothDensity * 0.95; 
 if(smoothDensity <0)
 {
  smoothDensity = 0; 
 }
//Vibration
data = analogRead(shake);

   sec = second();
   Minute = minute();
   Hour = hour();
   Time = Hour*3600+Minute*60+sec;

//CDS
   cdsVal = analogRead(cdsPin_anal);        //조도값 측정
   if(cdsVal >= 700)   
    cdsVal_digit = 1;
   else
    cdsVal_digit = 0;

//ground
  groundValue = analogRead(groundPin);
  
//Data print
  Serial.print("DATA,TIME,");
  Serial.print(distance);
  Serial.print(", ");
  Serial.print(cdsVal);//조도값 출력
  Serial.print(", ");
  Serial.print(cdsVal_digit);
  Serial.print(", ");
  Serial.print(smoothDensity);
  Serial.print(", ");
  Serial.print(data);
  Serial.print(", ");
  Serial.print(groundValue);
  Serial.print(", ");
  Serial.println(Time);
  delay(T);

   

}
