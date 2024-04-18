#include <DHT.h>
#include <ESP8266WiFi.h>

const char *ssid = "Heritage";
const char *pass = "heritage@2008";

#define DHTPIN 0
DHT dht(DHTPIN, DHT11);
WiFiClient client;
const int httpPort = 80;
int sm;
int outputpin= A0; 
const int red = 4;        
const int green = 14;     

void setup() 
{
  Serial.begin(115200);
  WiFi.disconnect();
  delay(10);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  dht.begin();
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Serial.print("Connection Established");
  Serial.print("\n");
}

int i = 0;
void loop() 
{
  int analogValue = analogRead(outputpin);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  sm = analogValue;
  sm = map(sm,750,180,0,100);
  Serial.print("\n\nData Serial No: ");
  Serial.println(i);
  i++;
  Serial.print("Soil moisture content: ");

  if(sm <0)
  {
    sm = 0;  
  }
  Serial.print(sm);
  Serial.print("%\n");
  Serial.print("Connection Established");
  Serial.print("\n");

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celcius, Humidity: ");
  Serial.println(h);

  if(h>=45)
  {
    Serial.print("\nChances of Rainfall - Rechecking after 5 secs. ");
    digitalWrite(red, HIGH);   
    digitalWrite(green, HIGH); 
    delay(5000);
  }
  else
  {
    if(sm>=60)
    {
      Serial.print("\nMoisture content sufficient - Rechecking after 5 secs. ");
      digitalWrite(red, HIGH);   
      digitalWrite(green, HIGH);       
      delay(5000);
    }
    else
    {
      if(t>25)
      {
        Serial.print("\nPump on for 10 secs. ");
        digitalWrite(red, LOW);   
        digitalWrite(green, LOW); 
        delay(10000);
      }
      else
      {
        Serial.print("\nPump on for 8 secs. ");
        digitalWrite(red, LOW);   
        digitalWrite(green, LOW); 
        delay(8000);
      }
    }
  }
  delay(3000);
}
