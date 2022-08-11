

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Dialog 4G 086";
const char* password = "9e36e3d1";

const char* mqtt_server = "test.mosquitto.org";
const char* outTopic = "Out/supun";

const char* inTopic = "ENTC/EN2560/in/000001";
String new1,new2,new3,new4,new5,st,ab,msg1="";
 unsigned long time1;
String bc="00";
String logic="";
String logic1="11";
const int pin0=D0;
const int pin8=D8;
const int pin1=D1;
const int pin2=D2;
const int pin5=A0;
const int pin6=D5;
const int pin7=D6;
const int pin9=D7;
int pin15;




WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
//#define MSG_BUFFER_SIZE	(50)
//char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Initialize the BUILTIN_LED pin as an output
  pinMode(BUILTIN_LED, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
   pinMode(pin0, INPUT);
   pinMode(pin8, INPUT);
   pinMode(pin1, INPUT);
   pinMode(pin2, INPUT);
   pinMode(pin5, INPUT);
   pinMode(pin6, INPUT);
   pinMode(pin7, INPUT);
   pinMode(pin9, INPUT);
   pinMode(pin0, OUTPUT);
  
}

void loop() {
  digitalWrite(pin0,LOW);
  
  if(analogRead(pin5)>1000){
    pin15=1;  
  }
  else{
    pin15=0;
  }
  Serial.println(analogRead(pin5));
  msg1=String(digitalRead(pin0))+String(digitalRead(pin8))+String(digitalRead(pin1))+String(digitalRead(pin2))+String(pin15)+String(digitalRead(pin6))+String(digitalRead(pin7))+String(digitalRead(pin9));
  Serial.println(msg1);
  new1=digitalRead(pin7);
  
  delay(1000);
  new2=digitalRead(pin7);
  delay(1000); 
  new3=digitalRead(pin7);
  
  new4=digitalRead(pin9);
  delay(300); 
  new5=digitalRead(pin9);
  
  ab=String(digitalRead(pin0))+String(digitalRead(pin8));
  if((ab=="10")&&(bc=="00")){
    logic="1";
  }
  else if((ab=="11")&&(bc="10")){
    logic="2";
  }
  else if((ab=="10")&&(bc=="11")){
    logic="3";
   
  }
  else if((ab=="00")&&(bc=="10")){
    logic="4";
  }
  else if((ab=="00")&&(bc=="00")){
  if(digitalRead(pin1)==1){
    logic="6";
  }
  else if((new1!=new2)&&(new2!=new3)){
    logic="5";
  }
  else if((digitalRead(pin2)==1)&&(digitalRead(pin6)==1)){
    logic="7";
  }
  else if(digitalRead(pin2)==1){
     logic="8";
  }

  else if((new4!=new5)&&(pin15==1)){
    logic="9";
  }
  else if((digitalRead(pin9)==1)&&(pin15==1)){
    logic="10";
  }
  }
  st=logic+"#"+logic1+"#"+String(float((millis()-time1))/60000);
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    byte arrsize=st.length()+1;
    char msg[arrsize];
    st.toCharArray(msg,arrsize);
    if(logic!=logic1){
      time1=millis();
      Serial.println(msg);
    client.publish(outTopic, msg);
    st="";
    }
  }
  bc=ab;
  logic1=logic;
}
