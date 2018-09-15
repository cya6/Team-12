/*
  Blink

 comment
*/

#include <Servo.h>

// bot left corner: 

Servo left ;
Servo right ;
int t = 0 ;
int on = 0 ;
int off = 90 ; 
int leftS = 0; //analog values, 0 to 1023, read off of sensors
int rightS = 0;
int thresh = 800; //potential threshold value, to stay on the lines
int count=0; 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600) ;
  //pinMode(3, OUTPUT) ;
  
  left.attach(5) ;
  right.attach(3) ;
}

// the loop function runs over and over again forever
void loop() {

  //Serial.println(3) ;

  //int pin = analogRead(0);

  //int duty = pin/4 ;
  //analogWrite(3, duty) ;

  //int y = map(pin, 0, 1024, 0, 180) ;
  //Serial.println(y) ;
  
  //move left wheel if t=0, else right
  rightS = analogRead(0) ;
  leftS = analogRead(1) ;

  Serial.println(rightS) ;
  Serial.println(leftS) ;
  
  if (rightS < thresh && leftS < thresh) 
  { // go straight
    if (count < 4)
      {
        left.write(off) ;
        right.write(180-on) ;
        delay(1250);
      }
    else
    {
      left.write(on) ;
      right.write(off);
      delay(1250);
    }
    count ++ ;
  }
  else if (rightS > thresh && leftS <= thresh) 
  { // turn left??
    //delay(500)
    left.write(85) ; //ccw slow down
    right.write(180-on) ; //ccw
    //delay(500);
  }
  else if (rightS <= thresh && leftS > thresh)
  { // turn right??
    //delay(500)
    left.write(on); //ccw
    right.write(180-85); //ccw slow down
  }
  else
  { // go straight
    left.write(on);
    right.write(180-on);
  }

  if (count > 7)
  {
    count = 0;
  }

  /*
  digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second*/
}
