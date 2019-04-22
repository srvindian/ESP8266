#include<SoftwareSerial.h>
#define DEBUG true
const int relayPin=4;
SoftwareSerial esp8266(2,3);    //(TX,RX)

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  sendData("AT+RST\r\n",2000,DEBUG); // reset module
  sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSTART=0,\"UDP\",\"192.168.4.1\",80,80,2\r\n",1000,DEBUG);
  sendData("AT+CIPSEND=0,7,\"192.168.4.1\",80",1000,DEBUG); // turn on server on port 80
}
String command,response;
int choice;

void loop() {
  //command=Serial.readString();
  command=esp8266.read();
  command+="\r\n";
  if(command!="\r\n"){
    //Serial.print(command);
    sendData(command,2000,true);
  } 
  
}


String sendData(String command, const int timeout, boolean debug)
{
    String response="";
    
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        if(c!='\0')
        //
          response+=c;
      }  
    }
    
    if(debug)
    {
      if(response.indexOf("+")>=0 && response.indexOf(":")>0)
      {
        //Serial.println(response);
        String str = response.substring(response.indexOf(":")+1,response.length());
        Serial.println(str);
        
        if (str=="0") {
          digitalWrite(relayPin, HIGH);
          
        } else if(str=="1") {
          digitalWrite(relayPin, LOW);
        }
        
      }
    }
    
    return response;
}
