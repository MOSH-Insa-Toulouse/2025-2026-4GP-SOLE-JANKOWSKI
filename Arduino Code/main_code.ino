
//// LIBRARIES ////

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> //FOR IMAGES
#include <SPI.h>

//// PINS AND OLED DISPLAY ////

#define encoder0PinA  3  //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  4  //DT Output B
#define Switch 2 // Switch connection if available
#define pin_graphite_sensor A0
#define pin_flex_sensor A1


#define width_OLED 128         
#define height_OLED 64       
#define reset_pin_OLED -1         
#define address_I2C_OLED 0x3C        


//POSITION VARIABLES

volatile int button = 0;
volatile int lastButton = 0;
volatile int encoder0Pos = 1;

int ValADC = 0;
const int R1 = 100000;
const int R3 = 100000;
const int R5 = 1000;
const int Vcc = 5;

volatile long resistanceWB = 6375;

const byte csPin           = 10;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 50000;   // 50k pot resistance between terminals A and B, 
const byte pot_OFFSET      = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    
int potValue = 32;  // start in the middle (0–255) //prover a lage en global variable for bruk i potentiometre  HAS TO BE CHANGED

//// ________OBJECT________ ////

Adafruit_SSD1306 displayOLED(width_OLED, height_OLED, &Wire, reset_pin_OLED);

////

int menuSize = 3;

int menuIndex = 0;
String menuItems[3] = {"DigiPot","Mode","Link Github"};

int modeIndex = 0;
String modes[2] = {"Graphite","Flex"};


//// FUNCTIONS ///
void setPotWiper(int addr, int pos) {
  
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  resistanceWB = ((rAB * pos) / maxPositions ) + pot_OFFSET;
}

void doEncoder() {
  if (!button){
    if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
      if (encoder0Pos < 3) encoder0Pos++;
      else encoder0Pos = 1;
    } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
      if (encoder0Pos > 1) encoder0Pos--;
      else encoder0Pos = 3;
    }
  }
  else if (encoder0Pos == 1){
    if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
      potValue ++;
    } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
      potValue--;
    }
    setPotWiper(pot0, potValue);
    Serial.print(potValue);
  }
  else if (encoder0Pos == 2){
    modeIndex = !modeIndex;
  }
} 

float ReadValueGraphite(){
  ValADC = analogRead(A0);
  return ValADC*Vcc/1023.0; // returns a value in volt
}

float GraphiteResistance(float Voltage){
  if (Voltage < 0.01) return 0; // so there will be no division by 0
  else return R1*(1+(R3)/(resistanceWB))*((Vcc)/(Voltage))-R1-R5; 
}


void setup() { 
  Serial.begin(9600);

  ////INITIALISING THE DIGITAL POTENTIOMETER////

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();
  delay(100);
  setPotWiper(pot0, potValue);
  //// INITIALISING THE OLED SCREEN ////

  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(Switch, INPUT_PULLUP);          //the button
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, RISING);
  
    // Initialisation de l'écran OLED
  if(!displayOLED.begin(SSD1306_SWITCHCAPVCC, address_I2C_OLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

    displayOLED.display();                            // Transfert le buffer à l'écran
    delay(2000); 
}

void loop(){

  ////OLED////
  
  if (!digitalRead(Switch)){
    button = !button;
    Serial.println(button);
  }

  // the menu which contains the pages 

  screen_OLED();

  ////ANALOG READ THE TWO SENSORS /////

  if (modeIndex == 0) {
    float sensorValue = ReadValueGraphite();
    Serial.print("Graphite Vadc: ");
    Serial.println(sensorValue);
    
    delay(100);

    float sensorResistance = GraphiteResistance(sensorValue);
    Serial.print("Graphite Resistance: ");
    Serial.println(sensorResistance);
    }
  else if (modeIndex == 1){
    int flexValue = analogRead(A1); // example pin
    Serial.print("Flex: ");
    Serial.println(flexValue);
  }
  delay(500);
}


//// DISPLAYING ON OLED ////

void screen_OLED() {
  if (button == 0) {
    displayOLED.clearDisplay();
    displayOLED.setTextSize(2);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    displayOLED.setCursor(0, 0); 
    displayOLED.setTextColor(SSD1306_WHITE);  
    displayOLED.println("DigiPot");
    displayOLED.println("Modes");
    displayOLED.println("GitHub Link");
    if(encoder0Pos == 1) {
      displayOLED.setCursor(0, 0); 
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.println("DigiPOt");
    }
    else if(encoder0Pos == 2) {
      displayOLED.setCursor(0, 15); 
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.println("Modes");
    }
    else if(encoder0Pos == 3) {
      displayOLED.setCursor(0, 30); 
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.println("GitHub Link");
      }
    }
  else {
    if(encoder0Pos == 1) {
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(1);

      displayOLED.println("DigiPotValue");

      displayOLED.print("Pot value out of 255: ");
      displayOLED.println(potValue);

      displayOLED.print("R (ohms): ");
      displayOLED.println(resistanceWB);

      //displayOLED.display();
    }
    else if(encoder0Pos == 2) {
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(2);

      displayOLED.print("Mode: ");
      displayOLED.println(modes[modeIndex]);

      //displayOLED.display();
      delay(200);
    }
    else if(encoder0Pos == 3) {
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(1);
      displayOLED.println("Moi je crois pas qu'il ait des bonne ou de mauvaise MOSH");
    } 
  }
  displayOLED.display();
}