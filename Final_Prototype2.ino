#include <OOCSI.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

const char* ssid = "lab-id";
const char* password = "ID-LAB-PSK";
const char* OOCSIName = "ESP_Receiver_Alan";
const char* hostserver = "oocsi.id.tue.nl";

int Brightness,  Fade;
int i, j;
int EventType;
int Check = 0;

#define RING_PIN 2
#define RING_NUMBER 12

OOCSI oocsi = OOCSI();

Adafruit_NeoPixel RING = Adafruit_NeoPixel(RING_NUMBER, RING_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  oocsi.setActivityLEDPin(LED_BUILTIN);

  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);
  
  RING.begin();
  RING.show();
}

void loop() {
  
  for(i = 0; i < (RING_NUMBER); i++){
    RING.setPixelColor(i, 0, 0, 0);
    RING.show();
  }
  oocsi.check();
  EventType = oocsi.getInt("EventType", 0);
  
  switch (EventType) {
    case 1: // Event Scenario 1, Prototype on table
            RING.setPixelColor(11, 255, 130, 0);
            RING.setPixelColor(7, 255, 130, 0);
            RING.show();
            break;
    case 2: // Event Scenario 2, Agreement
            for( Fade = 0;  Fade <= 255;  Fade+=4){ 
              for(i = 0; i < RING_NUMBER; i++){
                RING.setPixelColor(i, Fade,  Fade/2,  0);
              }
              delay(50);
              RING.show();
            }          
            break; 
    case 3: // Event Scenario 3, Greeting
            for(j = 0; j < 3; j++){
              for( Fade = 10;  Fade <= 255;  Fade+=4){ 
                for(i = 0; i < (RING_NUMBER); i++){
                  RING.setPixelColor(i, Fade, Fade/2, 0);
                }
                delay(50);
                RING.show();
              }
              for( Fade = 250;  Fade >= 0;  Fade-=4){ 
                for(i = 0; i < (RING_NUMBER); i++){
                  RING.setPixelColor(i, Fade, Fade/2, 0);
                }
                delay(50);
                RING.show();
              }
            }
            break;
            
    case 4: // Event Scenario 4, Chore 1 at house
            RING.setPixelColor(2, 0, 0, 255);
            RING.show();
            break;
    case 5: // Event Scenario 4, Chore 2 at house 
            RING.setPixelColor(3, 0, 0, 255);
            RING.show();
            break;
    case 6: // Event Scenario, Family at our house 
            RING.setPixelColor(5, 255, 130, 0);
            RING.show();
            break;      
    case 7: // Reset
            for(i = 0; i < ( RING_NUMBER); i++){
              RING.setPixelColor(i, 10, 10, 10);
            } 
            RING.show();
            break;
    case 8: // Event Scenario, Avoid/Approach at home
            RING.setPixelColor(5, 255, 0, 0);
            RING.show();
            break;   
    default:
            for(i = 0; i < ( RING_NUMBER); i++){
              RING.setPixelColor(i, 0, 0, 0);
            } 
            RING.show();
  }
}

void processOOCSI() {  
  Serial.print("EventType: ");
  Serial.print(oocsi.getInt("EventType", 0));
}
