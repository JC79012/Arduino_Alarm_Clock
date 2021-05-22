#include <Wire.h>
#include <Time.h>
#include<LiquidCrystal.h>
#include <IRremote.h>
#include <DS3231.h>

//sets time
int alarm_hour = 0;
int alarm_min = 0;
int off_code = 0;

//pins for the lcd 
int rs = 12; 
int en = 11;
int d4 = 5;
int d5 = 7;
int d6 = 3;
int d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int buzzer = 9; 

//pin for IR 
const int IRpin = 6;
IRrecv IR(IRpin);
decode_results cmd;

//pin for the led lights
int led_pin = 13;

//RTC set up
DS3231 rtc(SDA,SCL);
Time  timer;
int Hour;
int Minute;
int Second;

void setup()

{  
  Wire.begin();
  rtc.begin();
  pinMode(buzzer, OUTPUT);

  lcd.begin(16,2);     
//  lcd.print("hwllo");
//  rtc.setTime(2, 11, 0);     
//  rtc.setDate(19,01, 2021);  

  IR.enableIRIn(); 
  pinMode(led_pin, OUTPUT);

}

void loop()

{
  if (IR.decode(&cmd)){
        decode_input();
        IR.resume();
  }

 print_time();

}
void TurnOnAlarm()
{
     digitalWrite(buzzer, HIGH);
    digitalWrite(led_pin,HIGH); 
    delay(400);
    digitalWrite(led_pin,LOW); 
    delay(400);
    digitalWrite(led_pin,HIGH); 

    
}
void TurnOffAlarm()
{
  digitalWrite(led_pin, LOW);
  digitalWrite(buzzer, LOW);
}

void print_time()
{
  timer = rtc.getTime();
  Hour = timer.hour;
  Minute = timer.min;
  Second = timer.sec; 
  lcd.setCursor(0,0);
  lcd.print("Time:  ");
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0,1);
  lcd.print("Date: ");
  lcd.print(rtc.getDateStr());

 if(Hour == alarm_hour && Minute == alarm_min)
 {
    TurnOnAlarm();
 }
}

void decode_input()
{
  switch(cmd.value)
  {
    case(0xFF906F): //up
      setHourUp();
      break;
      case(0xFFE01F): //down
      setHourDown();
      break;
     case(0xFF629D): //Vol++
      setMinUp();
      break;
     case(0xFFA857): //Vol--
      setMinDown();
      break; 
      case(0xFF42BD): //7
       firstPin();
        break;
      case(0xFF6897): //0
       secondPin();
        break;
      case(0xFF30CF): //1
       thirdPin();
        break;
      case(0xFF18E7): //2
       lastPin();
        break;
  }
}
void firstPin()
{
  off_code = 0;
  off_code++; 
}
void secondPin()
{
  off_code++;
}
void thirdPin()
{
  off_code++;
}
void lastPin()
{
  off_code++;
  if(off_code == 4)
  {
    off_code = 0;
    TurnOffAlarm();
    
  }
  
}
void setHourUp()
{
      alarm_hour++;
      if(alarm_hour > 23)
        alarm_hour = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set Alarm:");
      lcd.print(alarm_hour);
      lcd.print(":");
      lcd.print(alarm_min);
      delay(600);
}
void setHourDown()
{
      alarm_hour--;
      if(alarm_hour < 0)
        alarm_hour = 23;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set Hour:");
      lcd.print(alarm_hour);
      lcd.print(":");
      lcd.print(alarm_min);
      delay(600);
}
void setMinUp()
{
  alarm_min++;
  if(alarm_min > 59)
    alarm_min = 0;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Set Alarm:");
   lcd.print(alarm_hour);
   lcd.print(":");
   lcd.print(alarm_min);
   delay(400); 
}
void setMinDown()
{
  alarm_min--;
  if(alarm_min < 0)
    alarm_min = 59;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Set Alarm:");
   lcd.print(alarm_hour);
   lcd.print(":");
   lcd.print(alarm_min);
   delay(400); 
}

//Time set up using
//https://maker.pro/arduino/projects/arduino-alarm-clock-using-real-time-clock-lcd-screen/
