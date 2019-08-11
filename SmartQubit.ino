//import all libraries as per requirement

include<ESP8266WiFi.h>
include<FirebaseArduino.h>
include<WiFiConnector.h>
include<WiFiClient.h>
include<ESP8266WebServer.h>
include<ESP8266mDNS.h>


WiFiConnector *wifi;

//set the credentials and secret token of firebase for connection to arduino

define FIREBASE_HOST "qubit-app.firebaseio.com"
define FIREBASE_AUTH "uao4hatNe2zV7O9SDIBtNLszrQWaSaYDOvwINDwa"

//set the below value to connect to your preferred wifi hotspot
const char*  WIFI_SSID="qubit";
const char* WIFI_PASSWORD= "dubeyjii";

float power();


const int SensorIn=A0;
float testFrequency=50;
float windowLength=20.0/testFrequency;
int sensorValue=0;
float intercept=-0.1129; //to be adjusted after calibration
float slope =0.0405; //to be adjusted based on calibration
float current_amps;
unsigned long printPeriod =1000;
unsigned long previousMillis=0;
float powr;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
         delay(200);
           // WiFi Connect
  Serial.println();      
  Serial.print("CONNECTING TO ");
  Serial.println(wifi->SSID());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);       
         
   wifi->on_connecting([&](const void *message){
   char buffer[70];
   //sprintf(buffer, "[%d] connecting -> %s ", wifi->counter, (char*) message);
   //Serial.println(buffer);  
   delay(500);
   Serial.print(".");
   });

   wifi->on_smartconfig_waiting([&](const void* message){
        
  });
    wifi->on_connected([&](const void* message){
    // Print the IP address
    Serial.print("WIFI CONNECTED ");
    Serial.println(WiFi.localIP());
  });

  wifi->connect();
  
  
  //WiFi.begin(ssid, password);
  Serial.println("");                 // Clear the buffer.
  
    
 while (WiFi.status() != WL_CONNECTED) {    // Wait for connection
         Serial.println("");
         Serial.println("Your ESP is connected!");  
        // Serial.println("Your IP address is: ");
         //Serial.println(WiFi.localIP());
      delay(1000);
  }
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 

}

void loop() {
            RunningStatistics inputStats;                 // create statistics to look at the raw test signal
            inputStats.setWindowSecs( windowLength );
   
            while( true ) {   
            sensorValue = analogRead(A0);  
            inputStats.input(sensorValue);  // log to Stats function
        
            if((unsigned long)(millis() - previousMillis) >= printPeriod) {
            previousMillis = millis();   // update time
      
            // display current values to the screen
            Serial.print( "\n" );
            // output sigma or variation values associated with the inputValue itsel
            //Serial.print( "\tsigma: " ); Serial.print( inputStats.sigma() );
            // convert signal sigma value to current in amps
            current_amps = intercept + slope * inputStats.sigma();
            Serial.print( "\tamps: " ); Serial.println( current_amps );
            float x=-1*power();
            Serial.print(x);
            Serial.print("Watt");
         //String name=Firebase.pushFloat("number",x);
        Firebase.setFloat("number",x);
        delay(2000);
        if (Firebase.failed()) {
        Serial.print("/n");
        Serial.print("pushing /logs failed:");
        Serial.println(Firebase.error());  
        return;
        }
        Serial.println();
        Serial.print("pushed: /logs/");
        //Serial.println(name);
        delay(1000);
    }
}

}
 float power()
 {
  float voltage=220.0;
  powr=(voltage*current_amps)*1.414;
  Serial.println(powr);
  Serial.println("Watt");
  return powr;
 }

