#include <PS2MouseHandler.h>
#include <math.h>
#define MOUSE_DATA 10
#define MOUSE_CLOCK 3




PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);




int redPin = 6;
int greenPin = 5;
int bluePin = 9;




float col[3];
float hue = 0.0;
float sat = 0.0;
float val = 1.0;




float x = 0;
float y = 0;
int scroll = 0;


float testcolor[3] = {0.0, 1.0, 1.0};




void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}




unsigned long last_run = millis();




void loop() {


  hsv2rgb(hue, sat, 1.0, testcolor);
  setColor(testcolor);
  if (millis() - last_run > 200) {
    last_run = millis();
    mouse.get_data();
    if (mouse.button(2) == 1) {
      x += mouse.x_movement();
      y += mouse.y_movement();
    }
    scroll += mouse.z_movement();
    if (scroll < 0) {
      scroll = 0.0;
    }
    else if (scroll > 1.0) {
      scroll = 1.0;
    }
  }
  sat = sqrt(x*x + y*y)/2000.0;
  hue = (atan2(y, x) + PI) / 6.28318530718;
  val = scroll;
  if (sat > 1.0) {
    sat = 1.0;
  }
  Serial.println(mouse.z_movement());
}




void setColor(float *rgb) {
  analogWrite(redPin, (int)((rgb[0]) * 255));
  analogWrite(greenPin, (int)((rgb[1]) * 255));
  analogWrite(bluePin, (int)((rgb[2]) * 255));  
}


float* hsv2rgb(float h, float s, float b, float* rgb) {
  rgb[0] = b * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[1] = b * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  rgb[2] = b * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  return rgb;
}
