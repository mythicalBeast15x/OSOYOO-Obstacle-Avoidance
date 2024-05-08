#include <Servo.h>
//motor pin definitions
#define rightSpeed  4 //3
#define leftSpeed  6
#define rightForwards  12
#define rightBackwards  11
#define leftForwards  7
#define leftBackwards  8
#define loopCounter  2
#define servo  9
//sensor pin definitions
#define echo_pin  1//2
#define trig_pin  10

#define buzzer  13
// constant speeds
#define FAST_SPEED 250
#define SPEED 120
#define TURN_SPEED 200
#define BACK_SPEED1 255
#define BACK_SPEED2 90



int thereis;
const int testNum = 20;
Servo head;
const int threshold = 10;
void forwards(void)  //Forward
{
  digitalWrite(rightForwards, HIGH);
  digitalWrite(rightBackwards,LOW);
  digitalWrite(leftForwards,HIGH);
  digitalWrite(leftBackwards,LOW);
}
void rotateLeft()  //Turn left
{
  digitalWrite(rightForwards, HIGH);
  digitalWrite(rightBackwards,LOW);
  digitalWrite(leftForwards,LOW);
  digitalWrite(leftBackwards,HIGH);
}
void rotateRight()  //Turn right
{
  digitalWrite(rightForwards, LOW);
  digitalWrite(rightBackwards,HIGH);
  digitalWrite(leftForwards,HIGH);
  digitalWrite(leftBackwards,LOW);
}
void backwards()  //Reverse
{
  digitalWrite(rightForwards, LOW);
  digitalWrite(rightBackwards,HIGH);
  digitalWrite(leftForwards,LOW);
  digitalWrite(leftBackwards,HIGH);
}
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(leftSpeed,speed_L); 
  analogWrite(rightSpeed,speed_R);   
}
void stop_Stop()    //Stop
{
  digitalWrite(rightForwards, LOW);
  digitalWrite(rightBackwards,LOW);
  digitalWrite(leftForwards,LOW);
  digitalWrite(leftBackwards,LOW);
  set_Motorspeed(0,0);
}

void disableBuzzer(){
  digitalWrite(buzzer, HIGH);
}

void soundAlarm(){
  digitalWrite(buzzer, LOW);
  delay(500);
  disableBuzzer();
}

int getDistance(){
  long avgDist = 0;
  for(int x = 0; x < testNum; x++){
    long elapsed_time;
    digitalWrite(trig_pin, LOW);
    delayMicroseconds(5);
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(15);
    digitalWrite(trig_pin, LOW);
    
    elapsed_time = pulseIn(echo_pin, HIGH);
    long distance = (elapsed_time * 0.0135039)/2;
    avgDist += distance;
  }
  //Serial.println(avgDist);
  avgDist /= testNum;
  return avgDist;

}


void setup() {
  // put your setup code here, to run once:
  /*setup L298N pin mode*/
  pinMode(rightForwards, OUTPUT); 
  pinMode(rightBackwards, OUTPUT); 
  pinMode(rightSpeed, OUTPUT);  
  pinMode(leftForwards, OUTPUT);
  pinMode(leftBackwards, OUTPUT); 
  pinMode(leftSpeed, OUTPUT); 
  stop_Stop();//stop move
  /*init HC-SR04*/
  pinMode(trig_pin, OUTPUT); 
  pinMode(echo_pin,INPUT); 
  /*init buzzer*/
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, HIGH);  
  

  digitalWrite(trig_pin,LOW);
  /*init servo*/
  head.attach(servo); 
  head.write(90);
  delay(2000);
  disableBuzzer();
  //Serial.begin(115200);
  Serial.begin(9600);
}

void loop() {
  while(getDistance() > threshold){
    set_Motorspeed(SPEED,SPEED);
    forwards();
  }
  stop_Stop();
  soundAlarm();

  head.write(50);//right
  delay(1000);
  bool rightIsBlocked = getDistance() < threshold;

  head.write(130);//left
  delay(1000);
  bool leftIsBlocked = getDistance() < threshold;

  head.write(90); //recenter
  delay(1000);
  bool centerIsBlocked = getDistance() < threshold;
  
  if((leftIsBlocked && !rightIsBlocked) || (!leftIsBlocked && !rightIsBlocked)){
    //go right
    set_Motorspeed(TURN_SPEED, TURN_SPEED);
    rotateRight();
    delay(300);
    stop_Stop();
  }else if(!leftIsBlocked && rightIsBlocked){
    //go left
    set_Motorspeed(TURN_SPEED, TURN_SPEED);
    rotateLeft();
    delay(300);
    stop_Stop();
  }else if(leftIsBlocked && rightIsBlocked && centerIsBlocked){
    //go back 1 ft
    set_Motorspeed(TURN_SPEED, TURN_SPEED);
    backwards();
    delay(500);
    stop_Stop();
    //choose which way to turn
    head.write(50);//right
    delay(1000);
    int rightDistance = getDistance();

    head.write(130);//left
    delay(1000);
    int leftDistance = getDistance();

    head.write(90); //re-center
    delay(1000);
    
    if(leftDistance > rightDistance){
      //turn right
      set_Motorspeed(TURN_SPEED, TURN_SPEED);
      rotateRight();
      delay(300);
    }else{
      //turn left
      set_Motorspeed(TURN_SPEED, TURN_SPEED);
      rotateLeft();
      delay(300);
    }
    stop_Stop();
  }

  delay(500);

}
