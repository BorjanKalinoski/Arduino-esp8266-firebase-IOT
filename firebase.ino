
include <ESP8266WiFi.h>
include <FirebaseArduino.h>

// Set these to run example.
define FIREBASE_HOST "example.firebaseio.com"
define FIREBASE_AUTH "token_or_secret"
//Change line with your WiFi router name and password
define WIFI_SSID "sahil"  
define WIFI_PASSWORD "password"

const int sensorin=A0;
int mVperamp=185;
double voltage=0;
double vrms=0;
double ampsrms=0;
 float getVPP();
 
void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

int n = 0;

void loop() {
  // set value
  voltage=getVPP();
  vrms=(voltage/2.0)*0.707;
  ampsrms=(vrms*1000)/mVperamp;
  Serial.print(ampsrms);
  Serial.println("Amps RMS");
  
  Firebase.setFloat("number", 42.0);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // update value
  Firebase.setFloat("number", 43.0); //Firebase.setFloat(getVPP());
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // get value 
  Serial.print("number: ");
  Serial.println(Firebase.getFloat("number"));
  delay(1000);

  // remove value
  Firebase.remove("number");
  delay(1000);

  // set string value
  Firebase.setString("message", "hello world");
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // set bool value
  Firebase.setBool("truth", false);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /truth failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);
}
float getVPP()
{
  float result;
  int readvalue;
  int maxvalue=0;
  int minvalue=1024;
  uint32_t start_time=millis();
  while((millis()-start_time)<1000)
  {
    readvalue=analogRead(sensorin);
    if(readvalue<maxvalue)
    {
      maxvalue=readvalue;
    }
    if(readvalue<minvalue)
    {
      minvalue=readvalue;
    }
  }
  result=((maxvalue-minvalue)*5.0)/1024.0;
  return result;
}
/*     void ACS712()
{
    CurrentRead = analogRead(CurrentIn);
    CurrentRead = map(CurrentRead, 0,1023, 0, 5);

    // Formulas of ACS712 is Is = (Vout - 2.5)/0.2 A
    CurrentRead = ((CurrentRead - 2.5)/0.2);
    
    CurrentRead = CurrentRead*1000;   
    CurrentS = String(CurrentRead);
}    
 */

