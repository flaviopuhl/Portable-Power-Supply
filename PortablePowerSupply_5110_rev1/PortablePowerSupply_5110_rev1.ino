#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

  // Software SPI (slower updates, more flexible pin options):
  // pin 3 - Serial clock out (SCLK) PWM INT
  // pin 4 - Serial data out (DIN)
  // pin 5 - Data/Command select (D/C)PWM
  // pin 7 - LCD chip select (CS) 
  // pin 6 - LCD reset (RST) PWM

  Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 7, 6);


int voltagePin = A0;    // Voltage sensor
int currentPin = A1;    // Current sensor
int battPin = A2;      // Battery voltage

int voltageValue = 0; 
int currentValue = 0;
int battValue = 0;

float ratio = 0.167;

int b=0;     //screen position

static const unsigned char PROGMEM p000[] =
{ B00001111, B11110000,
  B00001000, B00010000,
  B00001000, B00010000,
  B01111000, B00011110,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01111111, B11111110,
  B00000000, B00000000 };
  
  static const unsigned char PROGMEM p025[] =
{ B00001111, B11110000,
  B00001000, B00010000,
  B00001000, B00010000,
  B01111000, B00011110,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01111111, B11111110,
  B00000000, B00000000 };

    static const unsigned char PROGMEM p050[] =
{ B00001111, B11110000,
  B00001000, B00010000,
  B00001000, B00010000,
  B01111000, B00011110,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01111111, B11111110,
  B00000000, B00000000 };

    static const unsigned char PROGMEM p075[] =
{ B00001111, B11110000,
  B00001000, B00010000,
  B00001000, B00010000,
  B01111000, B00011110,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01111111, B11111110,
  B00000000, B00000000 };

    static const unsigned char PROGMEM p100[] =
{ B00001111, B11110000,
  B00001000, B00010000,
  B00001011, B11010000,
  B01111011, B11011110,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01011111, B11111010,
  B01011111, B11111010,
  B01000000, B00000010,
  B01111111, B11111110,
  B00000000, B00000000 };

void setup() {
  Serial.begin(9600);

  display.begin();
  display.setContrast(50);  // Adjust for your display
  delay(500);               // Adafruit logo
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(18,1);
  display.println("Portable");
  display.setCursor(8,11);
  display.println("Power Supply");
  display.setCursor(25,23);
  display.println("beta_1");
  display.setCursor(10,35);
  display.println("flavio_puhl");
  display.display();
  delay(1500);
  display.clearDisplay();
}

void loop() {
  
  // DAQ
  voltageValue = analogRead(voltagePin);
  currentValue = analogRead(currentPin);
  battValue = analogRead(battPin);
  
  float voltageValue_V = voltageValue * (5.0 / 1023.0)* ratio;
  float currentValue_A = (2.5 - (currentValue * (5.0 / 1024.0)) )/0.185;
  float battValue_V = battValue * (5.0 / 1023.0);
  float powerValue_W = voltageValue_V*currentValue_A;
  
  // Display Volts
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(77,7);
  display.print("V");
  
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(24,0);
  display.print(voltageValue_V);
  
  //Display Amps
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(77,24);
  display.print("A");

  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(24,17);
  display.print(currentValue_A);

  //Display Watts
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(77,41);
  display.print("W");

  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(24,34);
  display.print(powerValue_W);
  
  
  //Display batt charge
  if (battValue_V>=3.6){  //p100
    display.drawBitmap(0, 33,  p100, 16, 16, BLACK);
  }
  
  if (battValue_V<3.6 && battValue_V>=3.4){ // p075
     display.drawBitmap(0, 33,  p075, 16, 16, BLACK);
  }
  
  if (battValue_V<3.4 && battValue_V>=3.2){  //p050
    display.drawBitmap(0, 33,  p050, 16, 16, BLACK);
  }
  
  if (battValue_V<3.2 && battValue_V>=3.0){  //p025
    display.drawBitmap(0, 33,  p025, 16, 16, BLACK);
  }
  
  if (battValue_V<3.0){   //p000
    display.drawBitmap(0, 33,  p000, 16, 16, BLACK);
  }
 
  
   
  display.display();  
  delay(300);
  display.clearDisplay();
  
}
 
