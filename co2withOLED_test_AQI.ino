//libraries needed for a c02 sensor and featherwing
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_SCD30.h>

Adafruit_SCD30  scd30;

Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

// OLED FeatherWing buttons map to different pins depending on board:

 // 32u4, M0, M4, nrf52840, esp32-s2 and 328p
#define BUTTON_A  9
#define BUTTON_B  6
#define BUTTON_C  5

//set a variable for air quality. This will be used for the emoji choice later in the loop  
  float air_quality = 500;
//  char emoji[] = ":|";

void setup() {


  //Serial.begin(115200);
//  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens
  //Serial.println("128x64 OLED FeatherWing and Adafruit SCD30 test!");
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default

  //Serial.println("OLED begun");


  // Try to initialize the co2 monitor!
  if (!scd30.begin()) {
    display.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }
  //Serial.println("SCD30 Found!");

  //Serial.print("Measurement Interval: "); 
  //Serial.print(scd30.getMeasurementInterval()); 
  //Serial.println(" seconds");

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setRotation(1);
  //Serial.println("Button test");

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  //display.print("Connecting to SSID\n'adafruit':");
  //display.print("connected!");
  //display.println("IP: 10.0.1.23");
  //display.println("Sending val #0");
  display.display(); // actually display all of the above
}



void loop() {

if (scd30.dataReady()){
    //Serial.println("Data available!");

    if (!scd30.read()){ Serial.println("Error reading sensor data"); return; }

  //reset display cursor so it shows at the top.   
    display.clearDisplay();
    display.setCursor(0,0);

  //reset text to size 1
    display.setTextSize(1);



  //c02
    display.print("CO2: ");
    display.print(scd30.CO2, 3);
    display.println(" ppm");
    display.println("");
    air_quality = (scd30.CO2);

  //temp    
    display.print("Temp: ");
    display.print(scd30.temperature);
    display.println("C");
    display.println("");

  //humidity
    display.print("Rel Humidity: ");
    display.print(scd30.relative_humidity);
    display.println(" %");
    display.println("");
  

  //display amoji based on air quality 
  //0-700 is :D
  //701-800 is :)
  //801-1000 is :|
  //1001-1200 is :(
  //1201-1400 is :C
  //1401-2000 is >:O
  //2001 and up is LEAVE

  display.print("Air Quality Index");
  display.println("");

    //make emoji bigger and move to bottom right corner
    display.setCursor(100,50);
    display.setTextSize(2);


    if (air_quality <= 700){
        display.print(":D");

    }
    else if ((air_quality >=701) && (air_quality<= 800)) {
        display.print(":)");
    }
    else if ((air_quality >=801) && (air_quality<= 1000)) {
        display.print(":|");
    }
    else if ((air_quality >=1001) && (air_quality<= 1200)) {
        display.print(":(");
    }
    else if ((air_quality >=1201) && (air_quality<= 1400)) {
        display.print(":C");

    }
    else if ((air_quality >=1400) && (air_quality<= 2000)) {
        display.print(">:O");

    }
    else
    {
        display.print("LEAVE");

    }

  
  //display everything above  
    display.display();

  } else {
    display.println("No data");
  }
  //wait and then loop again.  
  delay(100);
  }

