#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char* ssid = "ssid";//name of wifi
const char* password = "password";//wifi password

const char* mqtt_broker = "hivemq.cloud_url";
const int mqtt_port = 8883; 
const char* mqtt_topic_subscribe = "home/garage/control";

const char* mqtt_username = "user_name"; 
const char* mqtt_password = "password"; 
const char* mqtt_client_id = "your client_id";

//download sertificate fron https://letsencrypt.org/certs/isrgrootx1.pem
const char* root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
certificate
-----END CERTIFICATE-----
)EOF";
const unsigned short int ARROW_UP = 4;
const unsigned short int ARROW_DOWN = 15;

const unsigned short int ARROW_LEFT = 17;
const unsigned short int ARROW_RIGHT = 5;

WiFiClientSecure espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  int message = payload[0];

  Serial.println(message);
  switch(message) {
    case 48:
         //ArrowUp
         digitalWrite(ARROW_DOWN, LOW);
         digitalWrite(ARROW_LEFT, LOW);
         digitalWrite(ARROW_RIGHT, LOW);
         digitalWrite(ARROW_UP, HIGH);
      break;
    case 49:
         //ArrowDown
         digitalWrite(ARROW_UP, LOW);
         digitalWrite(ARROW_LEFT, LOW);
         digitalWrite(ARROW_RIGHT, LOW);         
         digitalWrite(ARROW_DOWN, HIGH);  
      break;
    case 50:
          //ArrowLeft
         digitalWrite(ARROW_RIGHT, LOW); 
         digitalWrite(ARROW_UP, LOW);
         digitalWrite(ARROW_DOWN, LOW);
         digitalWrite(ARROW_LEFT, HIGH);
      break;
    case 51:
         //ArrowRight
         digitalWrite(ARROW_LEFT, LOW);  
         digitalWrite(ARROW_UP, LOW);
         digitalWrite(ARROW_DOWN, LOW);
         digitalWrite(ARROW_RIGHT, HIGH);
      break;      
    case 52:
         //left_up
         digitalWrite(ARROW_DOWN, LOW);  
         digitalWrite(ARROW_RIGHT, LOW);
         digitalWrite(ARROW_LEFT, HIGH);  
         digitalWrite(ARROW_UP, HIGH);
      break;
    case 53:
         //right_up
         digitalWrite(ARROW_DOWN, LOW);  
         digitalWrite(ARROW_LEFT, LOW);
         digitalWrite(ARROW_RIGHT, HIGH);  
         digitalWrite(ARROW_UP, HIGH);
      break;
    case 54:
         //left_down
         digitalWrite(ARROW_UP, LOW);  
         digitalWrite(ARROW_RIGHT, LOW);
         digitalWrite(ARROW_LEFT, HIGH);  
         digitalWrite(ARROW_DOWN, HIGH);
      break;
    case 55:
         //left_dright_downown
         digitalWrite(ARROW_UP, LOW);  
         digitalWrite(ARROW_LEFT, LOW);
         digitalWrite(ARROW_RIGHT, HIGH);  
         digitalWrite(ARROW_DOWN, HIGH);
      break;
    default:
        lowAll();
        
        Serial.println("from default");
      break;
  } 
}
void lowAll() {
   digitalWrite(ARROW_RIGHT, LOW);
   digitalWrite(ARROW_LEFT, LOW);
   digitalWrite(ARROW_UP, LOW);
   digitalWrite(ARROW_DOWN, LOW);
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect(mqtt_client_id, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic_subscribe);
      Serial.print("Subscribed to topic: ");
      Serial.println(mqtt_topic_subscribe);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  //speed
  pinMode(ARROW_UP, OUTPUT); //ArrowUp
  pinMode(ARROW_DOWN, OUTPUT);//ArrowDown

  //rotation
  pinMode(ARROW_RIGHT, OUTPUT);  //ArrowRight
  pinMode(ARROW_LEFT, OUTPUT); //ArrowLeft

  setup_wifi();

  espClient.setCACert(root_ca); 

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}