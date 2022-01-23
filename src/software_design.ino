#include "OneWire.h"
#include "spark-dallas-temperature.h"
#include "analogEC.h"

#include <Ubidots.h>           
const char *WEBHOOK_NAME = "temp_ec";//webhook name
Ubidots ubidots("webhook", UBI_PARTICLE); //standard

AnalogEC EC_sensor(A1);

double ec = 0.0;

OneWire oneWire( D2 );
DallasTemperature dallas(&oneWire);
double tempC = 0.0;

//=================functions activated from ubidots=====================
int led = D7;
int switchBuiltinLed(String command) {
// look for the matching argument "ON"
    if (command == "ON") {
        digitalWrite(led, HIGH);
        return 1;
    }
    // look for the matching argument "OFF"
    else if (command == "OFF") {
        digitalWrite(led, LOW);
        return 1;
    }
    return -1;
}
//========================================================================
#include "JsonParserGeneratorRK.h"
void Pub_To_G_Sheet(float temp,float ecVal) 
{
  JsonWriterStatic<256> jw;
  {
   //insert key-value pairs for each reading
   JsonWriterAutoObject obj(&jw);
   jw.insertKeyValue("tempC", temp);
   jw.insertKeyValue("ec", ecVal);
   
  }
  Particle.publish("EC_temp_sensors", jw.getBuffer(), PRIVATE);//trigger webhook of google sheets
  delay(100);
  String json_form = jw.getBuffer();
  Particle.publish("air_table", "{\"records\": [{\"fields\":"+json_form+"}]}",PRIVATE); //trigger webhook of airtable
}

void setup()
{
  Serial.begin(9600);
  Particle.function("Switch", switchBuiltinLed);
  pinMode(led, OUTPUT);
  dallas.begin();
  Particle.variable("temp", tempC);
  Particle.variable("EC", ec);
}

void loop()
{
   delay(2000);
//=========================get temp================================

   dallas.requestTemperatures();
   tempC = dallas.getTempCByIndex(0);

//=========================get EC==================================

   ec = EC_sensor.getValue(tempC);

//==================send data to ubidots===========================
   ubidots.add("temp", tempC);
   ubidots.add("ec",ec);
   bool bufferSent = false;
   bufferSent = ubidots.send(WEBHOOK_NAME, PUBLIC);

   if (bufferSent){
        // Do something if values were sent properly
        Serial.println("Values sent by the device");
    }

//=================send data to google sheet=======================

   Pub_To_G_Sheet(tempC, ec);

//=================print on terminal===============================

   Serial.println("temp = "+String(tempC)+" C");
   Serial.println("EC val = "+String(ec)+" ms/cm");

//===============wait for 10 sec====================================

   delay(5*60000);
}