//#include <DFRobot_WT61PC.h> // library for the Serial 6-axis accelerometer
#include <SPI.h>
#include <TFT_eSPI.h>       // Include the graphics library
TFT_eSPI tft = TFT_eSPI();  // Create object "tft"
//DFRobot_WT61PC sensor(&Serial1); // define the accelerometer

#include "roll_images.h" // 81 fullscreen (240x240) images are stored in this header file

void setup()
{

  // pins 18 and 21 are used as power for the sensor, since the pin headers do not include 3.3V pin
  // this is not be best solution, but it seems to be working
  pinMode(18, OUTPUT); // set pinmode to output so we can set it HIGH or LOW
  pinMode(21, OUTPUT);  // set pinmode to output so we can set it HIGH or LOW
  digitalWrite(18, HIGH); // 3.3V
  digitalWrite(21, LOW); // GND
  
  Serial.begin(115200); // Use Serial as debugging serial port (sending data back to PC)
  //Serial1.begin(9600, SERIAL_8N1, /*rx =*/14, /*tx =*/15); // Use Serial1 to connect it to DFRobot sensor
  //Serial2.begin(115200, SERIAL_8N1, /*rx =*/16, /*tx =*/17); // Use Serial2 to connect it second display

  //sensor.modifyFrequency(FREQUENCY_10HZ); // set sensor frequency to 10HZ

  tft.init(); // initialize the display
  //tft.setRotation(1); // set the display rotation
  tft.setRotation(4); // set the display rotation
  tft.fillScreen(TFT_DARKGREY); // fill the display with a dark grey color
  tft.setTextFont(4); // set the font, font number 4 is quite big
  tft.setSwapBytes(true); // Swap the colour byte order when rendering 

  // debug only - draw label "main"
  tft.setCursor(30, 60);
  tft.print("MAIN");
}

// pitch and roll value calculated from the DFRobot sensor 
int roll_value = 0; // roll value -40° .. +40°
int pitch_value = 0; // pitch value -40° .. +40°

// index of the image to be displayed, main display = pitch, secondary display = roll
int roll_value_img = 0; // image to be displayed, going from 0 - 80
int pitch_value_img = 0; // image to be displayed, going from 0 - 80

//test vars

int frame = 0;
int frame_inc = 1;


void loop()
{

  //if (sensor.available()) { // if the tilt sensor is sending any data

 /*   //  debug only - send values to PC using serial port (serial port 0)
    Serial.print("Acc\t"); Serial.print(sensor.Acc.X); Serial.print("\t");
    Serial.print(sensor.Acc.Y); Serial.print("\t"); Serial.println(sensor.Acc.Z); //acceleration information of X,Y,Z
    Serial.print("Gyro\t"); Serial.print(sensor.Gyro.X); Serial.print("\t");
    Serial.print(sensor.Gyro.Y); Serial.print("\t"); Serial.println(sensor.Gyro.Z); //angular velocity information of X,Y,Z
    Serial.print("Angle\t"); Serial.print(sensor.Angle.X); Serial.print("\t");
    Serial.print(sensor.Angle.Y); Serial.print("\t"); Serial.println(sensor.Angle.Z); //angle information of X, Y, Z 
    Serial.println();*/

    // get the pitch and roll values from the sensor
   // roll_value = round(sensor.Angle.X);
    //pitch_value = round(sensor.Angle.Y);

    // remap 0° .. 360° to -180° .. +180°
    if (roll_value > 180) {roll_value = roll_value - 360;}
    if (pitch_value > 180) {pitch_value = pitch_value - 360;}    

    // constrain the values to -40° .. +40°
    roll_value = constrain(roll_value, -40, 40);
    pitch_value = constrain(pitch_value, -40, 40);

    // images should go between 0-80 (81 images)
    roll_value_img = roll_value + 40;
    pitch_value_img = pitch_value + 40;

  // debug only - printing pitch and roll values are labels to the display
  // print the pitch value to the display
/*  tft.setCursor(30,100);
  tft.print("Pitch: ");
  tft.print(pitch_value);
  tft.print("       ");

  // print the roll value to the display
  tft.setCursor(30,130);
  tft.print("Roll: ");
  tft.print(roll_value);
  tft.print("       ");  

  // print the roll value to the display
  tft.setCursor(30,160);
  tft.print("Roll img: ");
  tft.print(roll_value_img);
  tft.print("       ");    */
  frame += frame_inc;
  if((frame > 80) && (frame_inc > 0)){
    frame = 80;
    frame_inc = -1;
  }
  else if ((frame < 0) && (frame_inc < 0)) {
    frame = 0;
    frame_inc = 1;
  }
  tft.pushImage(0, 0, 240, 240, epd_bitmap_allArray[frame]); // draw the fullscreen image sized 240x240px on position 0,0px

  //Serial2.println(roll_value_img); // send the roll_value_img to the second display
}
