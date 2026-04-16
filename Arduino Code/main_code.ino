////   LIBRARIES   ////
// Import display, graphics, and SPI communication libraries

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h> //FOR GRAPHICS
#include <SPI.h>          // COMMUNICATION WITH DIGITAL POTENTIOMETER

////   PINS AND OLED DISPLAY   ////
// Define all pins used for encoder, sensors, and OLED display

#define encoder0PinA  3   //CLK Output A Do not use other pin for clock as we are using interrupt
#define encoder0PinB  4   //DT Output B
#define Switch 2          // Switch connection if available

#define pin_graphite_sensor A0
#define pin_flex_sensor A1

//OLED configuration
#define width_OLED 128         
#define height_OLED 64       
#define reset_pin_OLED -1         
#define address_I2C_OLED 0x3C        


////  GLOBAL VARIABLES ////
// Store system state, electrical constants, and sensor parameters

// Encoder and UI state
volatile int button = 0;
volatile int encoder0Pos = 1;

int ValADC = 0;

//Circuit values matching KiCad
const int R1 = 100000;
const int R3 = 100000;
const int R5 = 1000;
const int Vcc = 5;

//Flex sensor calibration
const float R_DIV = 45600;    // resistance used for dividing the voltage
const float FlexflatR = 23800;  // resistance when flat
const float FlexbendR = 60000;  // resistance at 90 deg


//Digital potentiometer parameters
volatile long resistanceWB = 6375;

const byte csPin           = 10;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper moves between 0-255
const long rAB             = 50000;   // 50k pot resistance between terminals A and B 
const byte pot_OFFSET      = 125;     // 125 ohms pot wiper resistance (internal)
const byte pot0            = 0x11;    // potentiometer address
const byte pot0Shutdown    = 0x21; 

int potValue = 32;  // Initial potentioøeter value

////   OBJECT   ////

Adafruit_SSD1306 displayOLED(width_OLED, height_OLED, &Wire, reset_pin_OLED);

////   MENU   //// 
// Define menu structure and available modes

int menuSize = 3;
int menuIndex = 0;

String menuItems[3] = {"DigiPot","Modes","Github"};

int modeIndex = 0;
String modes[2] = {"Graphite","Flex"};

////   FUNCTIONS   ///
// Function to set potentiometer value via SPI and compute resistance

void setPotWiper(int addr, int pos) {
  
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  //Calculate equivalent resistance
  resistanceWB = ((rAB * pos) / maxPositions ) + pot_OFFSET;
}


////   HANDLING THE ENCODER   ////
// Interrupt routine to navigate menus and adjust parameters
void doEncoder() {

  //Here we navigate the main menu
  if (!button){
    if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
      if (encoder0Pos < 3) encoder0Pos++;
      else encoder0Pos = 1;
    } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
      if (encoder0Pos > 1) encoder0Pos--;
      else encoder0Pos = 3;
    }
  }

  //Here we enter the submenu of DigiPot
  else if (encoder0Pos == 1){
    if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==HIGH) {
      potValue ++;
    } else if (digitalRead(encoder0PinA)==HIGH && digitalRead(encoder0PinB)==LOW) {
      potValue--;
    }
    setPotWiper(pot0, potValue);
    Serial.print(potValue);
  }

  //Here we enter the submenu of Modes
  else if (encoder0Pos == 2){
    modeIndex = !modeIndex;
  }
} 

////   SENSOR FUNCTIONS   ////

//Analog reading of the graphite sensor voltage
float ReadValueGraphite(){
  ValADC = analogRead(A0);
  return ValADC*Vcc/1023.0; // returns a value in volt
}

//Calculations of the graphite sensor resistance
float GraphiteResistance(float Voltage){
  if (Voltage < 0.01) return 0; // Avoiding division by 0
  else return R1*(1+(R3)/(resistanceWB))*((Vcc)/(Voltage))-R1-R5; 
}

////   SETUP   ////
// Initialize communication, peripherals, and hardware components

void setup() { 

  Serial.begin(9600);

  // INITIALISING THE DIGITAL POTENTIOMETER
  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();
  
  delay(100);
  setPotWiper(pot0, potValue);

  //INITIALISING THE OLED SCREEN
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(Switch, INPUT_PULLUP);          //the button
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor
  pinMode(pin_flex_sensor, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoder0PinA), doEncoder, RISING);
  
    // Initialising the OLED display
  if(!displayOLED.begin(SSD1306_SWITCHCAPVCC, address_I2C_OLED))
    while(1);   
                              
  displayOLED.display();                         
  delay(2000); 
}

////   LOOP   ////
// Handle user input, update display, and perform sensor measurements

void loop(){
  
  // Switch between menu and submenu on button press
  if (!digitalRead(Switch)){
    button = !button;
    Serial.println(button);
  }

  // Upfating the OLED
  screen_OLED();

  //ANALOG READ THE TWO SENSORS
  if (modeIndex == 0) {
    // Graphite sensor measurement
    float sensorValue = ReadValueGraphite();
    Serial.print("Graphite Vadc: ");
    Serial.println(sensorValue);
    
    delay(100);

    float sensorResistance = GraphiteResistance(sensorValue);
    Serial.print("Graphite Resistance: ");
    Serial.println(sensorResistance);
    }
  
  else if (modeIndex == 1){
    // Flex sensor measurement (may require calibration)
    float ADCflexValue = analogRead(pin_flex_sensor);
    float Vflex = ADCflexValue * (Vcc / 1023.0);
    float Rflex = R_DIV * (Vcc / (Vflex - 1.0));
    Serial.print("Flex sensor:");
    Serial.println(Rflex);
  }
  delay(500);
}


////   DISPLAYING ON OLED   ////
// Show menu and submenu content on the OLED screen

void screen_OLED() {
  if (button == 0) {
    displayOLED.clearDisplay();
    displayOLED.setTextSize(2);               
    displayOLED.setCursor(0, 0); 
    displayOLED.setTextColor(SSD1306_WHITE);

    //Actual displayed names
    displayOLED.println("DigiPot");
    displayOLED.println("Modes");
    displayOLED.println("GitHub");

    //Highlighting the selected item on the menu
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
    // Submenu display
    if(encoder0Pos == 1) {
      // DigiPot info
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(1);

      displayOLED.println("DigiPotValue");

      displayOLED.print("Pot value out of 255: ");
      displayOLED.println(potValue);

      displayOLED.print("R (ohms): ");
      displayOLED.println(resistanceWB);
    }
    else if(encoder0Pos == 2) {
      // Mode selection display
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(2);

      displayOLED.print("Mode: ");
      displayOLED.println(modes[modeIndex]);

      delay(200);
    }
    else if(encoder0Pos == 3) {
      // GitHub link display
      displayOLED.clearDisplay();
      displayOLED.setCursor(0, 0);
      displayOLED.setTextColor(SSD1306_BLACK, SSD1306_WHITE);   // Couleur du texte, et couleur du fond
      displayOLED.setTextSize(1);
      displayOLED.println("Project Repository : ");
      displayOLED.println("https://github.com/MOSH-Insa-Toulouse/2025-2026-4GP-SOLE-JANKOWSKI.git ");
    } 
  }
  displayOLED.display();
}

