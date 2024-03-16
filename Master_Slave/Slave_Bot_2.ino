
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define In1 13  //D13
#define In2 12  //D12
#define In3 14   //D14
#define In4 27  //D27
#define sensor 5//D5


static const int RXPin = 34;
static const int TXPin = 35;

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps; 
WidgetMap myMap(V0);  

SoftwareSerial ss(RXPin, TXPin);  

BlynkTimer timer;

float spd;       
float sats;      
String bearing;  

char auth[] = "i5jAPuKZkscxC4Cc-2pTuwQI-XOvU8U-";              
char ssid[] = "Airtel-MyWiFi-AMF-311WW-84CA";                                       
char pass[] = "Monkey_mind";                                      

unsigned int move_index = 1;       

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS);  
  
  Serial.begin(9600);
  pinMode(In1,OUTPUT);
  pinMode(In2,OUTPUT);
  pinMode(In3,OUTPUT);
  pinMode(In4,OUTPUT);
  pinMode(sensor,INPUT);
  delay(2000);
}

void checkGPS()
{
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  
  }
}


void loop()
{
int detect = digitalRead(sensor);
  
 if ( detect == 0)
 {
  Stop();
  delay(100);


  while (ss.available() > 0) 
    {
        
        if (gps.encode(ss.read()))
          displayInfo();
    }
  Blynk.run();
  timer.run();  
 }
 else
 {
  Forward();
 }
}




void moveStop() 
{
  digitalWrite(In1, LOW); 
  digitalWrite(In2,LOW); 
  digitalWrite(In3, LOW); 
  digitalWrite(In4, LOW);
  } 
  
void Forward() 
{ 
  digitalWrite(In1,LOW);
  digitalWrite(In2,HIGH);                       
  digitalWrite(In3,LOW);
  digitalWrite(In4,HIGH); 
}

void Backward() 
{
  digitalWrite(In1,HIGH);
  digitalWrite(In2,LOW);                         
  digitalWrite(In3,HIGH);
  digitalWrite(In4,LOW);
}  

void TurnRight() 
{
  Serial.println("Turn right..");
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);
} 
 
void TurnLeft() 
{
 Serial.println("Turn left..");
 
 digitalWrite(In1, HIGH);
 digitalWrite(In2, LOW);
 digitalWrite(In3, LOW);
 digitalWrite(In4, HIGH); 
}  
void Stop()
{
 digitalWrite(In1, LOW);
 digitalWrite(In2, LOW);
 digitalWrite(In3, LOW);
 digitalWrite(In4, LOW); 
}

void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());    
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6); 
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
       
       sats = gps.satellites.value();    //get number of satellites
       Blynk.virtualWrite(V4, sats);

       bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
       Blynk.virtualWrite(V5, bearing);                   
  }
  
 Serial.println();
}
