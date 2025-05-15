#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String Red_pinState = "off";
String Blue_pinState = "off";
String Green_pinState = "off";

// Assign output variables to GPIO pins
const int Red_pin = 16;
const int Blue_pin = 19;
const int Green_pin = 33;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(Red_pin, OUTPUT);
  pinMode(Blue_pin, OUTPUT);
  pinMode(Green_pin, OUTPUT);
  // Set outputs to LOW
  digitalWrite(Red_pin, LOW);
  digitalWrite(Blue_pin, LOW);
  digitalWrite(Green_pin, LOW);

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

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /16/on") >= 0) {
              Serial.println("Red_pin on");
              Red_pinState = "on";
              digitalWrite(Red_pin, HIGH);
            } else if (header.indexOf("GET /16/off") >= 0) {
              Serial.println("Red_pin off");
              Red_pinState = "off";
              digitalWrite(Red_pin, LOW);
            } else if (header.indexOf("GET /19/on") >= 0) {
              Serial.println("Blue_pin on");
              Blue_pinState = "on";
              digitalWrite(Blue_pin, HIGH);
            } else if (header.indexOf("GET /19/off") >= 0) {
              Serial.println("Blue_pin off");
              Blue_pinState = "off";
              digitalWrite(Blue_pin, LOW);
            } else if (header.indexOf("GET /33/on") >= 0) {
              Serial.println("Green_pin on");
              Green_pinState = "on";
              digitalWrite(Green_pin, HIGH);
            } else if (header.indexOf("GET /33/off") >= 0) {
              Serial.println("Blue_pin off");
              Green_pinState = "off";
              digitalWrite(Green_pin, LOW);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button3 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>GPIO 16 - State " + Red_pinState + "</p>");
            // If the Red_pinState is off, it displays the ON button       
            if (Red_pinState=="off") {
              client.println("<p><a href=\"/16/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/16/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 19 - State " + Blue_pinState + "</p>");
            // If the Blue_pinState is off, it displays the ON button       
            if (Blue_pinState=="off") {
              client.println("<p><a href=\"/19/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/19/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            client.println("<p>GPIO 33 - State " + Green_pinState + "</p>");
            // If the Blue_pinState is off, it displays the ON button       
            if (Green_pinState=="off") {
              client.println("<p><a href=\"/33/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/33/off\"><button class=\"button button3\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}