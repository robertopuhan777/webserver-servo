#include <Preferences.h>
Preferences preferences;
#include "SPIFFS.h"
#include <Arduino.h>
#include <ezButton.h>
// Import required libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Servo.h>
Servo myServo1;  // create servo object to control a servo
Servo myServo2;  // create servo object to control a servo
#include <Arduino_JSON.h>
// Replace with your network credentials
const char* ssid = "ADD_YOUR_SSID";
const char* password = "ADD_SSID_PASSWORD";
const char* http_username = "ADD_HTTP_USERNAME";
const char* http_password = "ADD_HTTP_PASSWORD";
static const int PIN_SERVO1 = 2;
static const int PIN_SERVO2 = 5;
String sliderValue1 = "90";
String lastsliderValue1 = "90";
String sliderValue2 = "90";
String lastsliderValue2 = "90";
// setting PWM properties
String SLIDERVALUE1;
String SLIDERVALUE2;
String jsonBuffer;
String endpoint;
const char* PARAM_INPUT1 = "value";
const char* PARAM_INPUT2 = "value";

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 250;
unsigned long last_time = 0;
unsigned long timer_delay = 250;
// Create AsyncWebServer object on port 80
String WiFiSignal() {
  float Signal = WiFi.RSSI();
  Signal = 90 / 40.0 * Signal + 212.5; // From Signal = 100% @ -50dBm and Signal = 10% @ -90dBm and y = mx + c
  if (Signal > 100) Signal = 100;
  return " " + String(Signal, 0) + "";
}
float Signal;
String lastWiFiSignal;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");
// Json Variable to Hold Sensor Readings
JSONVar readings;
JSONVar owmreadings1;
// Timer variables
unsigned long lastTime = 0;
unsigned long lastTimeAcc1 = 0;
unsigned long OWMapDelay1 = 250;
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted succefully");
}
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  // Your Domain name with URL path or IP addre with path
  http.begin(client, serverName);
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  String payload = "{}";
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();
  return payload;
}
String getSERVO_readings() {
    jsonBuffer = httpGETRequest(endpoint.c_str());
    JSONVar myObject = JSON.parse(jsonBuffer);
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
    }

  owmreadings1["SLIDERVALUE1"] = String(sliderValue1);
  owmreadings1["SLIDERVALUE2"] = String(sliderValue2);
  String jsonString1 = JSON.stringify(owmreadings1);
  Serial.println(jsonString1);
  return jsonString1;
}
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Oxanium"> 
 <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.5.0/css/font-awesome.min.css"/>
  <title>ESP 32 Web Server - Servo motors</title>
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous"/>
<script src="https://code.jquery.com/jquery-3.2.1.min.js"></script>

<link rel="stylesheet" href="sliders.css"> 

</head>
<body style="font-family: 'Oxanium', sans-serif;background:inherit;color:inherit;">
<button title="Logout" style="font-family: 'Oxanium', sans-serif;z-index:59595959;font-size:25px;background:none;border:none;position:fixed;bottom:1px;right:1px;cursor:pointer;color:inherit;" onclick="logoutButton()"><i class="fas fa-sign-out-alt"></i></button>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  
}
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
</script>

<div id="time" style='position:fixed;top:1px;right:5px;'><i title='WiFi Signal &percnt;' style='font-weight:900;font-size:20px;' class='fas fa-wifi'></i><span style='font-family:'Oxanium';font-size:20px;'>%WiFiSignal%</span><a class='spanner'></a></div><script src="time.js"></script>
 <btn1 id='btn1' title='Night Mode' style='display:block;font-size:22px;cursor:pointer;position:fixed;left:1px;top:1px;z-index:5000;'><i class='fas fa-moon'></i></btn1>
<script src='btn1.js'></script>
<btn2 id='btn2' title='Day Mode' style='display:block;font-size:22px;cursor:pointer;position:fixed;left:40px;top:1px;z-index:5000;'><i class='fas fa-sun'></i></btn2>
<script src='btn2.js'></script>  
 <div><div style="width:300px;position:absolute;left:20px;top:10px;" title="SERVO A"><canvas class="canvas" style="font-family: 'Oxanium';width:300px;height:300px;border-radius:5px;opacity:0.85;color:inherit;" id="gauge-servoA"></canvas></div>
<script src="gauge.min.js"></script>
<script src="servoA.js"></script>
<div style="position:absolute;left:1px;top:300px;"><p><span class="textsliderValue1" id="textsliderValue1">%SLIDERVALUE1%</span></p>
<p><input title="SERVO A" type="range" onchange="updateSliderPWM(this)" id="pwmSlider1" min="0" max="180" value="%SLIDERVALUE1%" step="1" class="slider"></p></div>
<div style="width:300px;position:absolute;right:30px;top:10px;"><canvas class="canvas" style="color:inherit;font-family: 'Oxanium';width:300px;height:300px;border-radius:5px;opacity:0.85;" id="gauge-servoB"></canvas></div>
<div style="position:absolute;right:1px;top:300px;"><p><span class="textsliderValue2" id="textsliderValue2">%SLIDERVALUE2%</span></p>
<p><input  title="SERVO B" type="range" onchange="updateSliderPWM(this)" id="pwmSlider2" min="0" max="180" value="%SLIDERVALUE2%" step="1" class="slider1"></p></div>
</div>
<script src="gauge.min.js"></script>
<script src="servoB.js"></script>
<script>
function updateSliderPWM(element) {
  var sliderValue1 = document.getElementById("pwmSlider1").value;
  document.getElementById("textsliderValue1").innerHTML = sliderValue1;
  console.log(sliderValue1);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider1?value="+sliderValue1, true);
  xhr.send();
  var sliderValue2 = document.getElementById("pwmSlider2").value;
  document.getElementById("textsliderValue2").innerHTML = sliderValue2;
  console.log(sliderValue2);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider2?value="+sliderValue2, true);
  xhr.send();
}
</script>
<script src="script.js"></script>

<script src='fullscreen.js'></script>

<button class='btn btn-info ' id='show_hide_bt' title='FullScreen' onclick='view_fullscreen(this);fullscreen();' style='z-index:59595959;text-decoration:none;color:inherit;border:none;background:none;left:1px;cursor:pointer;position:fixed;bottom:10px;font-size:14px;'>
<i class='fa fa-eye'></i> Show</button> 
<center><div><iframe class='iframe1' scrolling='no' src='time.html'></iframe></div></center>
<script>
var currentTime = new Date().getHours();

if (document.body) {
    if (8 <= currentTime && currentTime < 21) {
    document.body.style.backgroundColor = 'white';
document.body.style.color = 'black';
$('meta[name=theme-color]').remove();
$('head').append( '<meta name="theme-color" content="white">' );
document.body.style.transition = "background 0.2s,color 0.2s";
document.body.style.backgroundColor = "#ffffff";
document.body.style.color = "black";
document.getElementById("btn2").style.color = "darkorange";
document.getElementById("btn1").style.color = "black";
document.getElementById(btn1).style.color = "none";
    }
    else {
    document.body.style.backgroundColor = 'black';
document.body.style.color = 'white';
$('meta[name=theme-color]').remove();
$('head').append( '<meta name="theme-color" content="black">' );
document.body.style.transition = "background 0.2s,color 0.2s";
document.body.style.backgroundColor = "#000000";
document.body.style.color = "white";
document.getElementById("btn1").style.color = "darkorange";
document.getElementById("btn2").style.color = "white";
document.getElementById(btn2).style.color = "none";
  
    }
}



</script>
</body>
</html>
)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>
<link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Oxanium">    
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous"/>
</head>
<body style="background: inherit;font-family: 'Oxanium', sans-serif;overflow-x:hidden;">
<div style="posotion:fixed;left:50%;transform:translateX(-50%);"><a style="position:inherit;top:50%;transform:translateY(-50%);font-size: 20px;" href="/">return to homepage</a></div>
<script>
var currentTime = new Date().getHours();

if (document.body) {
    if (8 <= currentTime && currentTime < 21) {
    document.body.style.backgroundColor = 'white';
document.body.style.color = 'black';
$('meta[name=theme-color]').remove();
$('head').append( '<meta name="theme-color" content="white">' );
document.body.style.transition = "background 0.2s,color 0.2s";
document.body.style.backgroundColor = "#ffffff";
document.body.style.color = "black";
document.getElementById("btn2").style.color = "darkorange";
document.getElementById("btn1").style.color = "black";
document.getElementById(btn1).style.color = "none";
    }
    else {
    document.body.style.backgroundColor = 'black';
document.body.style.color = 'white';
$('meta[name=theme-color]').remove();
$('head').append( '<meta name="theme-color" content="black">' );
document.body.style.transition = "background 0.2s,color 0.2s";
document.body.style.backgroundColor = "#000000";
document.body.style.color = "white";
document.getElementById("btn1").style.color = "darkorange";
document.getElementById("btn2").style.color = "white";
document.getElementById(btn2).style.color = "none";
  
    }
}



</script>
</body>
</html>
)rawliteral";
// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if (var == "SLIDERVALUE1"){
    return sliderValue1;
    return lastsliderValue1;
  }
  else if (var == "SLIDERVALUE2"){
    return sliderValue2;
    return lastsliderValue2;
  }
  else if(var == "WiFiSignal"){
    return lastWiFiSignal;
  }
  else if(var == "BUTTONPLACEHOLDER"){
   
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
   

   Serial.println(F("Initialize System"));
  initSPIFFS();

 
  
    myServo1.attach(PIN_SERVO1);  // attaches the servo on the PIN_SERVO1 to the servo object
    myServo2.attach(PIN_SERVO2);  // attaches the servo on the PIN_SERVO1 to the servo object
  // configure LED PWM functionalitites
  
  // attach the channel to the GPIO to be controlled
  
  myServo1.write(sliderValue1.toInt());
  myServo2.write(sliderValue2.toInt());

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage1>
    if (request->hasParam(PARAM_INPUT1)) {
      if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
      inputMessage1 = request->getParam(PARAM_INPUT1)->value();
      sliderValue1 = inputMessage1;
      myServo1.write(sliderValue1.toInt());
    }
    else {
      inputMessage1 = "No message sent";
    }
    Serial.println(inputMessage1);
    request->send(200, "text/plain", "OK");
  });
  server.on("/slider2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage2;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage2>
    if (request->hasParam(PARAM_INPUT2)) {
      if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
      inputMessage2 = request->getParam(PARAM_INPUT2)->value();
      sliderValue2 = inputMessage2;
      myServo2.write(sliderValue2.toInt());
    }
    else {
      inputMessage2 = "No message sent";
    }
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  server.on("/fullscreen.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/fullscreen.js", "text/javascript");
  });
  server.on("/servoA.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/servoA.js", "text/javascript");
  });
  server.on("/servoB.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/servoB.js", "text/javascript");
  });
  server.on("/time.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/time.js", "text/javascript");
  });
  server.on("/time.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/time.html", "text/html");
  });
   server.on("/sliders.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/sliders.css", "text/css");
  });
   server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/gauge.min.js", "text/javascript");
  });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });
  server.on("/submit.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/submit.js", "text/javascript");
  });
  server.on("/day.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/day.js", "text/javascript");
  });
  server.on("/night.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/night.js", "text/javascript");
  });
  server.on("/storage.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/storage.js", "text/javascript");
  });
  server.on("/result.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/result.html", "text/html");
  });
  server.on("/compare.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/compare.html", "text/html");
  });
   server.on("/compare1.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/compare1.html", "text/html");
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });

  server.serveStatic("/", SPIFFS, "/");
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
    sliderValue1 = String(SLIDERVALUE1);
    sliderValue2 = String(SLIDERVALUE2);
    request->send(200, "text/plain", "OK");
  });
  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient * client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last meage ID that it got is: %u\n", client->lastId());
    }
   // send event with meage "CircuitDigest", id current millis
    // and set reconnect delay to 1 second
   // client->send("iHOME", NULL, millis(), 250);
  });
  server.addHandler(&events);

  server.begin();
}
void looper() {
  
   preferences.begin("my-app", false);
 lastsliderValue1 = String(sliderValue1);
  Serial.println(sliderValue1);
  
  // Remove all preferences under the opened namespace
  //preferences.clear();

  // Or remove the counter key only
  //preferences.remove("counter");

  // Get the counter value, if the key does not exist, return a default value of 0
  // Note: Key name is limited to 15 chars.
  String counter = preferences.getString("counter", sliderValue1);


  // Print the counter to Serial Monitor
  Serial.printf("Current value: %u\n", sliderValue1.toInt());

  // Store the counter to the Preferences
  preferences.putString("counter", sliderValue1);

  // Close the Preferences
  preferences.end();
  delay(100);
  } 
void loop() { 
  looper();
  if ((millis() - lastTimeAcc1) > OWMapDelay1) {
    lastWiFiSignal = String(WiFiSignal());
    events.send(getSERVO_readings().c_str(), "SERVO_readings", millis());
    lastTimeAcc1 = millis();
   }
   
}
