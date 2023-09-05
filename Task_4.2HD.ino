#include <WiFiNINA.h>


const char* ssid = "4b";      //network SSID (name)
const char* password = "Guggi6266";   //network password

WiFiServer server(80);

// Variable to store HTTP request
String httpReq;

// current output state of LEDs
String Red = "off";
String Green = "off";
String Yellow = "off";

// setting value of LED to pins
const int redLED = 4;
const int greenLED = 11;
const int yellowLED = 12;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time
const long timeout = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  // Set outputs to LOW
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
    WiFiClient client = server.available(); // Listen for incoming clients

if (client) { 
Serial.println("New Client."); // print a message "New Client" out in the serial port
String currentLine = ""; // String to hold incoming data from the client
currentTime = millis();
previousTime = currentTime;
while (client.connected() && currentTime - previousTime <= timeout) {
currentTime = millis(); 
if (client.available()) { // if there's something to still available
char c = client.read(); //read the information
Serial.write(c); // print it out the serial monitor
httpReq += c;
if (c == '\n') { // if byte is a newline character
// if the current line is blank, you got two newline characters in a row.
// that's the end of the client HTTP request, so send a response:
if (currentLine.length() == 0) {
// HTTP Requests always start with a response code (e.g. HTTP/1.1 200 OK)
// and a content-type so the client knows what's coming, then a blank line:
client.println("HTTP/1.1 200 OK");
client.println("Content-type:text/html");
client.println("Connection: close");
client.println();

// turns the LEDs on and off
if (httpReq.indexOf("GET /2/on") >= 0) {
Serial.println("RED LED is on");
Red = "on";
digitalWrite(redLED, HIGH);
} else if (httpReq.indexOf("GET /2/off") >= 0) {
Serial.println("RED LED is off");
Red = "off";
digitalWrite(redLED, LOW);
} else if (httpReq.indexOf("GET /4/on") >= 0) {
Serial.println("Green LED is on");
Green = "on";
digitalWrite(greenLED, HIGH);
} else if (httpReq.indexOf("GET /4/off") >= 0) {
Serial.println("Green LED is off");
Green = "off";
digitalWrite(greenLED, LOW);
} else if (httpReq.indexOf("GET /5/on") >= 0) {
Serial.println("Yellow LED is on");
Yellow = "on";
digitalWrite(yellowLED, HIGH);
} else if (httpReq.indexOf("GET /5/off") >= 0) {
Serial.println("Yellow LED is off");
Yellow = "off";
digitalWrite(yellowLED, LOW);
}

// Display the HTML web page
client.println("<!DOCTYPE html><html>");
client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
client.println("<link rel=\"icon\" href=\"data:,\">");
// CSS 
client.println("<style>html { font-family: New Times Roman; display: inline-block; text-align: center;}");
client.println(".buttonToggle { background-color: #ff0000; border: 2px solid grey ;padding: 16px 40px; border-radius: 60%;");
client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}</style></head>");


// Web Page Heading
client.println("<body><h1>My LED Control Server</h1>");

// Display current state, and ON/OFF buttons for GPIO 2 Red LED 
client.println("<p>Red LED is " + Red + "</p>");
// If the Red is off, it displays the OFF button 
if (Red=="off") {
client.println("<p><a href=\"/2/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/2/off\"><button class=\"button buttonRed\">ON</button></a></p>");
} 

// Display current state, and ON/OFF buttons for GPIO 4 Green LED 
client.println("<p>Green LED is " + Green + "</p>");
// If the Green is off, it displays the OFF button 
if (Green =="off") {
client.println("<p><a href=\"/4/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/4/off\"><button class=\"button buttonGreen\">ON</button></a></p>");
}
client.println("</body></html>");

// Display current state, and ON/OFF buttons for GPIO 5 Yellow LED 
client.println("<p>Yellow LED is " + Yellow + "</p>");
// If the Yellow is off, it displays the OFF button 
if (Yellow =="off") {
client.println("<p><a href=\"/5/on\"><button class=\"button buttonOff\">OFF</button></a></p>");
} else {
client.println("<p><a href=\"/5/off\"><button class=\"button buttonYellow\">ON</button></a></p>");
}
client.println("</body></html>");

// The HTTP response ends with another blank line
client.println();
// Break out of the while loop
break;
} else { // if you got a newline, then clear currentLine
currentLine = "";
}
} else if (c != '\r') { // if you got anything else but a carriage return character,
currentLine += c; // add it to the end of the currentLine
}
}
}
// Clear the header variable
httpReq = "";
// Close the connection
client.stop();
Serial.println("Client disconnected.");
Serial.println("");
}
}
