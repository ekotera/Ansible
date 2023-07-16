#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Stepper.h>
#define RXD2 16
#define TXD2 17
#define ONBOARD_LED 2

int Pin1 = 13;//IN1 is connected to 13 
int Pin2 = 12;//IN2 is connected to 12  
int Pin3 = 14;//IN3 is connected to 14 
int Pin4 = 27;//IN4 is connected to 27 

int  poleStep = 0; 
int  lastStatus = 0;
int  doorSta = 0;// stores door status 0= closed

String buttonTitles[] ={"CLOSE", "IN-OUT", "OUT-IN", "OPEN"};
String argId[] ={"door", "pet"};
String tag ="";
// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, Pin1, Pin3, Pin2, Pin4);

const char *ssid = "xtera_ext";
const char *password = "!!XteraVlaskamp258!!";

WebServer server(80);

void handleRoot() {

 String HTML ="<!DOCTYPE html>\
  <html>\
  <head>\
  \t\n<title>PET Door Control</title>\
  \t\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  \n<style>\
 \nhtml,body{\t\nwidth:100%\;\nheight:100%\;\nmargin:0}\n*{box-sizing:border-box}\n.colorAll{\n\tbackground-color:#90ee90}\n.colorBtn{\n\tbackground-color:#add8e6}\n.angleButtdon,a{\n\tfont-size:30px\;\nborder:1px solid #ccc\;\ndisplay:table-caption\;\npadding:7px 10px\;\ntext-decoration:none\;\ncursor:pointer\;\npadding:5px 6px 7px 10px}a{\n\tdisplay:block}\n.btn{\n\tmargin:5px\;\nborder:none\;\ndisplay:inline-block\;\nvertical-align:middle\;\ntext-align:center\;\nwhite-space:nowrap}\n";
   
  HTML +="</style>\n\n</head>\n\n<body>\n<h1>PET Door Control </h1>\n";
  switch (doorSta) {
    case 1:
      {
       HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">The door is OUT-IN</span></h2>\n";    
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?"+argId[0]+"=1\">"+buttonTitles[1]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=2\">"+buttonTitles[2]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=3\">"+buttonTitles[3]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=0\">"+buttonTitles[0]+"</a>\t\n\t</div>\n\n"; 
      }
      break;
    case 2:
      {
       HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">The door is IN-OUT</span></h2>\n";    
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=1\">"+buttonTitles[1]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?"+argId[0]+"=2\">"+buttonTitles[2]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=3\">"+buttonTitles[3]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=0\">"+buttonTitles[0]+"</a>\t\n\t</div>\n\n"; 
      }
      break;
      case 3:
      {
       HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">The door is BOTH OPEN</span></h2>\n";    
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=1\">"+buttonTitles[1]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=2\">"+buttonTitles[2]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?"+argId[0]+"=3\">"+buttonTitles[3]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=0\">"+buttonTitles[0]+"</a>\t\n\t</div>\n\n"; 
      }
      break;
    default:
      {
       HTML +="\n\t<h2><span style=\"background-color: #FFFF00\">The door is CLOSED</span></h2>\n";    
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=1\">"+buttonTitles[1]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=2\">"+buttonTitles[2]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#90ee90\"  href=\"/motor?"+argId[0]+"=3\">"+buttonTitles[3]+"</a>\t\n\t</div>\n\n"; 
       HTML +="\t<div class=\"btn\">\n\t\t<a class=\"angleButton\" style=\"background-color:#f56464\"  href=\"/motor?"+argId[0]+"=0\">"+buttonTitles[0]+"</a>\t\n\t</div>\n\n"; 
      }
      break;
  }
  HTML +="\t<div>\n\t\t"+tag+"\t\n\t</div>\n\n"; 
  HTML +="\t\n</body>\n</html>\n";
  server.send(200, "text/html", HTML);  
}//handleRoot()

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}//end of handleNotFound()

void setup(void) {
 /*
 pinMode(Pin1, OUTPUT);//define pin for ULN2003 in1 
 pinMode(Pin2, OUTPUT);//define pin for ULN2003 in2   
 pinMode(Pin3, OUTPUT);//define pin for ULN2003 in3   
 pinMode(Pin4, OUTPUT);//define pin for ULN2003 in4   
*/
  pinMode(ONBOARD_LED, OUTPUT);  
  Serial.begin(115200);//initialize the serial monitor
  Serial.println("PET Door Control");
   
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: http://");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("PETDoor")) { Serial.println("MDNS responder started"); }

  server.on("/", handleRoot);
  server.on("/motor", HTTP_GET, doorControl);           
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial Txd is on pin: "+String(TX));
  Serial.println("Serial Rxd is on pin: "+String(RX));

}//end of setup

void loop(void) {  

  while (Serial2.available()) {
    Serial.print(char(Serial2.read()));
    tag +=Serial2.read();
    for (uint8_t i = 0; i < 5; i++){digitalWrite(ONBOARD_LED,HIGH);delay(200);digitalWrite(ONBOARD_LED,LOW);delay(100);}
    digitalWrite(ONBOARD_LED,HIGH);
    delay(1000);
  }

  server.handleClient(); 
}//end of loop

void doorControl() {
    if(server.arg(argId[0]) != "")
    {
      doorSta = (server.arg(argId[0])).toInt();               
    }
  switch (doorSta) {
    case 1:
      {
        if ( lastStatus == 0){ myStepper.setSpeed(10); myStepper.step(509); }
        if ( lastStatus == 1){ }
        if ( lastStatus == 2){ myStepper.setSpeed(10); myStepper.step(-509); }
        if ( lastStatus == 3){ myStepper.setSpeed(10); myStepper.step(1018); }
        lastStatus = doorSta;
        for (uint8_t i = 0; i < 1; i++){digitalWrite(ONBOARD_LED,HIGH);delay(100);digitalWrite(ONBOARD_LED,LOW);delay(200);}
      }
      break;
    case 2:
      {
        if ( lastStatus == 0){ myStepper.setSpeed(10); myStepper.step(1018); }
        if ( lastStatus == 1){ myStepper.setSpeed(10); myStepper.step(509); }
        if ( lastStatus == 2){ }
        if ( lastStatus == 3){ myStepper.setSpeed(10); myStepper.step(1527); }
        lastStatus = doorSta;
        dfor (uint8_t i = 0; i < 2; i++){digitalWrite(ONBOARD_LED,HIGH);delay(100);digitalWrite(ONBOARD_LED,LOW);delay(200);}
      }
      break;
      case 3:
      {
        if ( lastStatus == 0){ myStepper.setSpeed(10); myStepper.step(-509); }
        if ( lastStatus == 1){ myStepper.setSpeed(10); myStepper.step(-1018); }
        if ( lastStatus == 2){ myStepper.setSpeed(10); myStepper.step(-1527); }
        if ( lastStatus == 3){ }     
        lastStatus = doorSta;
        for (uint8_t i = 0; i < 3; i++){digitalWrite(ONBOARD_LED,HIGH);delay(100);digitalWrite(ONBOARD_LED,LOW);delay(200);}      
      }
      break;
      case 0:
      {
        if ( lastStatus == 0){ } 
        if ( lastStatus == 1){ myStepper.setSpeed(10); myStepper.step(-509); }
        if ( lastStatus == 2){ myStepper.setSpeed(10); myStepper.step(-1018); }
        if ( lastStatus == 3){ myStepper.setSpeed(10); myStepper.step(509); }
        lastStatus = doorSta;
        for (uint8_t i = 0; i < 4; i++){digitalWrite(ONBOARD_LED,HIGH);delay(100);digitalWrite(ONBOARD_LED,LOW);delay(200);}
      }
      break;
    default:
      {

      }
      break;
  }
  handleRoot();
}//motorControl end
/*
void driveStepper(int c)
{
     digitalWrite(Pin1, pole1[c]);  
     digitalWrite(Pin2, pole2[c]); 
     digitalWrite(Pin3, pole3[c]); 
     digitalWrite(Pin4, pole4[c]);   
}//driveStepper end here
*/