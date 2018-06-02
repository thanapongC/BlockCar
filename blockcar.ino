
#include "Wire.h"
#include "SPI.h"  
#include "RTClib.h"
#include <Servo.h>
#include <EEPROM.h>

Servo myservo; 
RTC_DS3231 RTC;

#define ALCOHOL_DAT A0
#define HEATER_SEL  A1


int val11;
float val2;
int redPin = 8; // Red LED, connected to digital pin 9
int grnPin = 9; // Green LED, connected to digital pin 10
int bluPin = 10; // Blue LED, connected to digital pin 11
int heatpin = 6;
int testLpin = 7;

int address = 0;
byte value;

void setup () {

    myservo.attach(11); 
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    RTC.adjust(DateTime(__DATE__, __TIME__));

    if (! RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      RTC.adjust(DateTime(__DATE__, __TIME__));
    }

  
    pinMode(redPin, OUTPUT); 
    pinMode(grnPin, OUTPUT);
    pinMode(bluPin, OUTPUT);
    pinMode(heatpin, INPUT_PULLUP);
    pinMode(testLpin, INPUT_PULLUP);

    DateTime now = RTC.now();

    pinMode(HEATER_SEL,OUTPUT);// 
    digitalWrite(HEATER_SEL,LOW);
    pinMode(ALCOHOL_DAT,INPUT);

    //เปิด
    myservo.write(-180);
    delay(1000);
    
    EEPROM.write(0, 0);

}

void loop () {

  
  value = EEPROM.read(address);

  Serial.print(address);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  


 int heatpinvalue = digitalRead(heatpin);
 int testLpinvalue = digitalRead(testLpin);
 int onoff = analogRead(A4);
 Serial.println(heatpinvalue);
 Serial.println(testLpinvalue);
 Serial.println(onoff);                      
                        float temp;
                        val11=analogRead(3);
                        temp=val11/4.092;
                        val11=(int)temp;//
                        val2=((val11%100)/10.0);
                        Serial.println(val2);
                        
                        DateTime now = RTC.now();
                        
                        Serial.print(now.hour(), DEC);
                        Serial.print(':');
                        Serial.print(now.minute(), DEC);
                        Serial.print(':');
                        Serial.println(now.second(), DEC);
            //start car
     //ถ้ารถสตาร์ทอยู่ให้ปิดรูกุญเเจ
                if(val2 > 12){      
                  Serial.println("Start System");

                  if(now.hour() >= 0 && now.minute() >= 0 && now.hour() <= 6){
                   Serial.println("Time Start");
                   
                                          //สมมุติว่ารูปิด
                                          value = EEPROM.read(address);
                                          while(value == 1){
                                                myservo.write(-180);
                                                delay(1000);
                                                EEPROM.update(0, 0);
                                          }
                                        
                                        
                   digitalWrite(bluPin, HIGH);//สีน้ำเงิน
                   delay(1000);
                   digitalWrite(bluPin, LOW);//สีน้ำเงิน
                   delay(1000);

                //กดเพื่อ heat
                    
                    if(heatpinvalue == 0){
                          digitalWrite(HEATER_SEL,LOW); //ไม่ทำงานเปลี่ยนเป็น High
                            Serial.println("Start to heat the sensor, please wait 5~10min befor exposure to alcohol");
                            Serial.println("Ready to use");
                            
                            digitalWrite(bluPin, HIGH);//สีน้ำเงิน
                            digitalWrite(redPin, LOW); 
                            digitalWrite(grnPin, LOW); 
                           
                                      //เริ่มเป่า
                                    
                                    if(testLpinvalue == 0){
                                       Serial.println("Start test");
                                      int sensorValue;
                                      sensorValue = analogRead(ALCOHOL_DAT); //read the analog value
                                      int value = 1023 - sensorValue;
                                      Serial.println(value);
                                      if(value < 200){
                                        //ไม่เกิน
                                        Serial.println("min");
                                        digitalWrite(grnPin, HIGH); //สีเขียว
                                        digitalWrite(redPin, LOW); 
                                        digitalWrite(bluPin, LOW); 

                                         value = EEPROM.read(address);
                                         while(value == 0){
                                            myservo.write(180);
                                            delay(1000);
                                            EEPROM.update(0, 1);
                                         }
                                      }
                                      else if((value > 300) && (value < 500)){
                                        //เกิน
                                       Serial.println("max");
                                       digitalWrite(redPin, HIGH); //สีเเดง
                                       digitalWrite(grnPin, LOW); 
                                       digitalWrite(bluPin, LOW); 

                                       value = EEPROM.read(address);
                                       while(value == 1){
                                          myservo.write(-180);
                                          delay(1000);
                                          EEPROM.update(0, 0);
                                       }
                                       
                                      }
                                      delay(100);
                                   }   
                      }  
                                       
                    }else if(now.hour() >= 6 && now.minute() >= 0){
                       //myservo.write(50);    
                       Serial.println("Not use");
                       digitalWrite(redPin, HIGH);//สีเเดง
                       delay(1000);
                       digitalWrite(redPin, LOW);//สีเเดง
                       delay(1000);

                       value = EEPROM.read(address);
                          //เปิด
                         while(value == 0){
                          myservo.write(180);
                          delay(1000);
                          EEPROM.update(0, 1);
                         }
                        
                   
                      }
                }else{
    //เปิด
    while(value == 0){
    myservo.write(180);
    delay(1000);
    EEPROM.update(0, 1);
    }
                }          

  delay(1000);
}



