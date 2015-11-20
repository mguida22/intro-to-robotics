#include <Sparki.h> // include the sparki library

// 50.45 diameter of the wheel
// 85.86 axle length

//states: 0 -> queued

int startTime, endTime, lineLeft, lineCenter, lineRight;
int state = 0;
const int THRESHOLD = 700;

float halfRadius = 12.6125;
float radiusOverAxle = 0.29379;
int pingValue;

float theta = 0.0;
float x = 0.0;
float y = 0.0;

float rightWheel = 0.0;
float leftWheel = 0.0;

float theInverseD = 0.00116469;
float halfLR = 0.0;

int loopTime = 100;

void setup()
{
  Serial1.begin(9600);
  sparki.servo(0);
}

void loop() {
  startTime = millis();
  if(Serial1.available()){
    int inByte = Serial1.read();
    char received = (char)inByte;
    if(received == '1'){
      if(state == 0){
        state = 1;
        //begin parking
      }
    }else {
    }
  }
  
  lineLeft   = sparki.lineLeft();
  lineCenter = sparki.lineCenter(); 
  lineRight  = sparki.lineRight();
  pingValue = sparki.ping();
  
  if(pingValue >= 0 && pingValue < 15){
    sparki.moveStop();
  } else{
    if(state == 1){
      if(lineCenter < THRESHOLD && lineRight < THRESHOLD && lineLeft < THRESHOLD){
        state = 2;
        sparki.moveStop();
        x = 0; //reset odometry
        y = 0;
      } else {
        if ( lineCenter < THRESHOLD ) // if line is below left line sensor
        {
          rightWheel = 0.0278551532;
          leftWheel = 0.0278551532;
          
          sparki.moveForward(); // move forward
        }
        else{
          if ( lineLeft < THRESHOLD ) // if line is below left line sensor
          {
            rightWheel = 0.0278551532;
            leftWheel = -0.0278551532;
            
            sparki.moveLeft(); // turn left
          }
        
          if ( lineRight < THRESHOLD ) // if line is below right line sensor
          {  
            rightWheel = -0.0278551532;
            leftWheel = 0.0278551532;
            
            sparki.moveRight(); // turn right
          }
        }
      }
    } else if(state == 2){
      sparki.moveLeft(90);
      rightWheel = 0.0278551532;
      leftWheel = -0.0278551532;
      state = 3;
    } else if(state == 3){
      if(lineCenter < THRESHOLD && lineRight < THRESHOLD && lineLeft < THRESHOLD){
        state = 4;
        sparki.moveStop();
        x = 0; //reset odometry
        y = 0;
      }else{
        sparki.moveForward();
        rightWheel = 0.0278551532;
        leftWheel = 0.0278551532;
      }
    } else if(state == 4){
      sparki.servo(SERVO_LEFT);
      int ping = sparki.ping();
      if(ping == -1 || ping > 20){
         sparki.moveRight(90);
         state = 7;
      }else{
        state = 5;
      }
    }else if(state == 5){
      sparki.servo(SERVO_RIGHT);
      delay(100);
      int ping = sparki.ping();
      if(ping == -1 || ping > 20){
         sparki.moveLeft(90);
         state = 7;
      }else{
        state = 6;
      }
    } else if (state == 6){
      state = 3;
      sparki.servo(SERVO_CENTER);
      sparki.moveForward();
    }else if(state == 7){
      state = 8;
      sparki.servo(SERVO_CENTER);
    }else if(state == 8){
      sparki.moveBackward(10);
      sparki.moveStop();
      state = 9;
    }
  }
  if(state != 4 && state != 5){
    halfLR = (rightWheel + leftWheel) * 0.5;
    
    x += cos(theta) * halfLR * loopTime;
    y += sin(theta) * halfLR * loopTime;
    theta += (rightWheel - leftWheel) * theInverseD * loopTime * 10;

    endTime = millis();
  
    if (loopTime - (endTime - startTime) > 0) {
      delay(loopTime - (endTime - startTime));
    }
  }
}
