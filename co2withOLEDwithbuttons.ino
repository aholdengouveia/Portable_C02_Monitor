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

//set a variable for air quality. This will be used for the emoji choice later in the loop, and displaying values, these are just baseline values for initial setup  
  float air_c02 = 500;
  float air_temp = 500;
  float air_hum = 500;
  float air_quality = 500;


void setup() {


  //Serial.begin(115200);
//  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens leave commented out if you want to make your display portable
  //Serial.println("128x64 OLED FeatherWing and Adafruit SCD30 test!"); use if you want to test
  delay(250); // wait for the OLED to power up
  display.begin(0x3C, true); // Address 0x3C default



  // Try to initialize the co2 monitor!
  if (!scd30.begin()) {
    display.println("Failed to find SCD30 chip");
    while (1) { delay(10); }
  }


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
  display.display(); // actually display all of the above
}



void loop() {
  //reset display cursor so everything shows at the top.   
    display.clearDisplay();
    display.setCursor(0,0);

  if (scd30.dataReady()){

//creates a boolean (true or false), then checks if the sensor is reading the data. MUST include this or value will be 0.00
  bool readok = scd30.read();
  if(!readok)
  {
    //error
  }
  //setting actual values to the floats created earlier
    air_c02 = (scd30.CO2);
    air_quality = (scd30.CO2);
    air_temp = (scd30.temperature);
    air_hum = (scd30.relative_humidity);

//setting the display to be cleared, and the cursor to the top of the screen
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);

    //yield will say to the board that it can yield the processor to other processes before moving on.
    yield();

    
//if statement for the 3 buttons, A, B and C each button will make sure the text is larger, the screen is rotated to the right orientation, and then print a large version of the value from the monitor
  if(!digitalRead(BUTTON_A)) {
    display.setRotation(1);
    display.setTextSize(3);
    display.print("CO2: ");
    display.println("");
    display.print(air_c02);
    display.setTextSize(2);
    display.println(" ppm");
    display.println("");
    //this will display everything above
    display.display();
    //this will pause the display for 5 seconds with the display of the info
    delay(5000);

  }

  if(!digitalRead(BUTTON_B)) {
    display.setRotation(1);
    display.setTextSize(2);
    display.print("Humidity: ");
    display.println("");
    display.setTextSize(3);
    display.print(air_hum);
    display.println("");
    //this will display everything above
    display.display();
    //this will pause the display for 5 seconds with the display of the info
    delay(5000);

  }    

  if(!digitalRead(BUTTON_C)) {
    display.setRotation(1);
    display.setTextSize(2);
    display.print("Temp: ");
    display.println("");
    display.setTextSize(3);
    display.print(air_temp);
    display.setTextSize(2);
    display.println("C");
    display.println("");
    //this will display everything above
    display.display();
    //this will pause the display for 5 seconds with the display of the info
    delay(5000);
  }

//this should clear the display for the aqi splash that is the emoji
  display.clearDisplay();
  display.display();
  yield();

  //resets the cursor to the top of the screen
  display.setCursor(0,0);


//general splash screen on boot after Adafruit logo goes away, will say air quality index in smaller print, with a larger emoji, this will make sure the roation is the right place, cursor is the right place
  display.setRotation(1);
  display.setCursor(0,0);

//makes the text smaller 
  display.setTextSize(1);
  display.print("Air Quality Index:");
  display.println("");


    //make emoji bigger and move to middle of the screen
    display.setTextSize(4);
    display.setRotation(2);
    display.setCursor(10,75);


    //if statement to show emoji based on air quality
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
        display.setTextSize(3);
        display.print(">:O");
    }
    else
    {
      //resets textsize to 3 so the full word will show up, and resets rotation so the word shows up right
        display.setTextSize(3);
        display.setRotation(1);
        display.print("LEAVE");

    }

     //display everything above and make sure screen cleared
      yield();
      display.display();
      //display.clearDisplay();


}
}
