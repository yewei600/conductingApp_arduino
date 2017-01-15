#include <NewPing.h>


#define TRIGGER_PIN  4
#define ECHO_PIN     5
#define MAX_DISTANCE 60



// UltraSonic Sensor interfacing to Arduino .
int triggerPins[3] = {2, 4, 6};
int echoPins[3] = {3, 5, 7};
int buzzer = 8;
int duration;  int distance;
int stopDuration;
int s2Cnt = 0;
int valCnt = 0;
int sensor;
int lastSensor = 1;  int lastDistance = 0;
long startTime;                    // start time for stop watch
long elapsedTime;                  // elapsed time for stop watch
bool goingUp, goingDown, isStop = false;

long upStart, downStart, stopStart;
int upTimer = 0;
int downTimer = 0;
int stopTimer = 0;
int upCount = 0;
int downCount = 0;
int stopCount = 0;

boolean actionStart = false;
String lastAction;
int upCnt = 0;
int downCnt = 0;
int rightCnt = 0;
int leftCnt = 0;

int upDistance = 0;
int downDistance = 0;

NewPing sonar[3] = {NewPing (triggerPins[0], echoPins[0], MAX_DISTANCE),
                    NewPing (triggerPins[1], echoPins[1], MAX_DISTANCE),
                    NewPing (triggerPins[2], echoPins[2], MAX_DISTANCE)
                   };


void setup() {

  Serial.begin(115200); //we'll start serial comunication, so we can see the distance on the serial monitor Serial.println("Tech Ponder's UltraSonic Sensor Tutorial");


}

void printAction(String action, long duration, int sensor) {
  Serial.print(action);
  Serial.print(" ");  Serial.print(duration);
  Serial.print(" ");  Serial.print(sensor);
  Serial.println();
}

void printUpDown(String action, long duration, int sensor) {
  Serial.print(action);
  Serial.print(" ");  Serial.print(duration);
  Serial.print(" ");  Serial.print(sensor);
  Serial.println();
}

void loop() {
  //Serial.println(lastSensor);
  if (!actionStart) {
    startTime = millis();
    actionStart = true;
  }
  //delay(100);

  for (int i = 0; i < 3; i++) {
    //    duration = sonar[i].ping_median(5);
    //    distance = sonar[i].convert_cm(duration); //transforming the number to cm(if you want inches, you have to change the 29.1 with a suitable
    delay(10);
 
    if (i == 0 || i == 2) {
      duration = sonar[i].ping();
      distance = duration / US_ROUNDTRIP_CM; //transforming the number to cm(if you want inches, you have to change the 29.1 with a suitable
    }
    else if (i == 1) {
      duration = sonar[i].ping_median(3);
      distance = sonar[i].convert_cm(duration);
    }

    sensor = i;


    if (distance < 60 && distance > 0) {        // for up and down motion

      if (sensor == 1) {
        
        if (abs(lastDistance - distance) >= 2) {
          if (lastDistance < distance ) {  //going up
            if (goingUp == false) {
              //reset starttime
              upStart = millis();
            }

            goingUp = true;
            goingDown = false;
            isStop = false;
            if (goingUp == true) {
              upTimer = millis() - upStart;
            }

            elapsedTime = millis() - startTime;
            //Serial.print(sensor);
            // Serial.println(" sensor");
            printUpDown("u", upTimer, i);
            // actionStart = false;
          }

          else if (lastDistance > distance) { //going down
            if (goingDown == false) {
              downStart = millis();
            }
            goingUp = false;
            goingDown = true;
            isStop = false;
            if (goingDown == true) {
              downTimer = millis() - downStart;
            }
            elapsedTime = millis() - startTime;
            printUpDown("d", downTimer, i);
            //actionStart = false;
          }
        }
        else if (abs(lastDistance - distance) <= 2) {
          if (isStop == false) {
            stopStart = millis();
          }
          goingUp = false;
          goingDown = false;
          isStop = true;
          if (isStop == true) {
            stopTimer = millis() - stopStart;
          }

          printAction("S", stopTimer, i);
          //actionStart = false;
        }
        actionStart = false;
      
        
      }

      else if (sensor == lastSensor && sensor != 1) {
        if (isStop == false) {
          stopStart = millis();
          isStop = true;
        }
        if (isStop == true) {
          stopTimer = millis() - stopStart;
        }
        printAction("S", stopTimer, i);
        actionStart = false;
      }

       if (sensor != lastSensor ) {  //sensor changed

        isStop = false;
        goingUp = false;
        goingDown = false;
        if (lastSensor > sensor) { //going left
          elapsedTime = millis() - startTime;
         // if (elapsedTime < 3000) {
            printAction("l", elapsedTime, i);
            actionStart = false;
         // }
        }

        else if (lastSensor < sensor) { //going right
          elapsedTime = millis() - startTime;
         // if (elapsedTime < 3000) {
            printAction("r", elapsedTime, i);
            actionStart = false;
          //}
        }
        //Serial.println("changing last sensor");
        lastSensor = sensor;       //update the lastSensor value
      }
    
      lastDistance = distance;
    }


  }

}
