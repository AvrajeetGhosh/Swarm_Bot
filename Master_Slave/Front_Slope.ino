#define sensor 12
#include<Servo.h>
Servo myservo;
int pos = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(sensor,INPUT);
  myservo.attach(4);
}

void loop ()
{
  int detect = digitalRead(sensor);
  if ( detect == 0)
  {
     digitalWrite(13,LOW);
   for (pos = 0; pos<=180; pos+=1)
   {
    myservo.write (pos);
   }
  }
  else
  {
    digitalWrite(13,HIGH);
  }
}
 
