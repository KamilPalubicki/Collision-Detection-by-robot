#include <NewPing.h>          



#define TRIGGER_PIN  2  
#define ECHO_PIN     3   
#define MAX_DISTANCE 100 

#define SERVO_PIN    4  



#define TARGET_FOUND_ANY     0   
#define TARGET_LEFT          1
#define TARGET_LEFT_CENTER   2
#define TARGET_CENTER        3
#define TARGET_RIGHT_CENTER  4
#define TARGET_RIGHT         5

#define TARGET_ARRAY_SIZE    6

#define TARGET_TOO_CLOSE     25  // cm


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

// motor one
int enA = 10;
int inA = 9;
int inB = 8;
// motor two
int enB = 5;
int inC = 7;
int inD = 6;

unsigned int uS;  
unsigned int cm;   
unsigned int cm_now; 

int TargetArray[TARGET_ARRAY_SIZE];  
int DirectionsToLook = 3;  
int ServoDirectionData[3] = { 2500, 1600, 600};


enum {  START_STATE,
        STOP_LOOK_STATE,
        TARGET_CENTER_STATE,
        TARGET_LEFT_STATE,
        TARGET_RIGHT_STATE
     } RobotState = START_STATE;

void setup ()
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Start");
  
  pinMode(SERVO_PIN, OUTPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(inC, OUTPUT);
  pinMode(inD, OUTPUT);
 
}

void loop()
{

  switch (RobotState)
  {
    case START_STATE: 
      Serial.println("+++ Zaczyna sprawdzania kolizji");
      ForwardFast();  
      delay(25);
      ForwardSlow();  
      delay(600);
      RobotState = STOP_LOOK_STATE ;  
      break;

    case STOP_LOOK_STATE:  
      Serial.println("+++ Szukanie przeszkod");
      Stop();  
      
      for (int i = 0; i < TARGET_ARRAY_SIZE; i++) TargetArray[i] = false;
      LookAround();     

      if      (TargetArray[TARGET_CENTER ] == true)  RobotState = TARGET_CENTER_STATE;
      else if (TargetArray[TARGET_LEFT ]   == true)  RobotState = TARGET_LEFT_STATE;
      else if (TargetArray[TARGET_RIGHT ]  == true)  RobotState = TARGET_RIGHT_STATE;
      else     RobotState = START_STATE;
      break;

    case TARGET_CENTER_STATE:  
      Serial.println("***** Cel to jechac do tylu");
      Stop();
      BackwardSlow(500);
      SpinLeft(1100);  // 180 stopni obrotu
      delay(500);
      RobotState = START_STATE;
      break;

    case TARGET_LEFT_STATE:      
      Serial.println("***** Cel to jechac na lewo");
      Stop();
      BackwardSlow(500);
      SpinRight(500);
      delay(500);
      RobotState = START_STATE;
      break;

    case TARGET_RIGHT_STATE:     
      Serial.println("*****Cel to jechac na prawo");
      Stop();
      delay(500);
      BackwardSlow(500);
      SpinLeft(500);
      delay(500);
      RobotState = START_STATE;
      break;
  }  


} 



void ForwardSlow()
{
    analogWrite(enA, 250);
  analogWrite(enB, 250);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, HIGH); 
   delay(1000);
}
/*---------------------------*/

void ForwardMedium()
{
   analogWrite(enA, 250);
  analogWrite(enB, 250);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, HIGH); 
  delay(1000);
}
/*---------------------------*/
void ForwardFast()
{
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, HIGH); 
  delay(1000);
}
/*---------------------------*/
void BackwardSlow(int HowMuch)
{ 
  analogWrite(enA, 250);
  analogWrite(enB, 250);
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);
  digitalWrite(inC, HIGH);
  digitalWrite(inD, LOW);
   delay(1000);
}
/*---------------------------*/
void BackwardMedium(int HowMuch)
{
   analogWrite(enA, 250);
  analogWrite(enB, 250);
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);
  digitalWrite(inC, HIGH);
  digitalWrite(inD, LOW);
   delay(1000);
}
/*---------------------------*/
void BackwardFast(int HowMuch)
{
 analogWrite(enA, 250);
  analogWrite(enB, 250);
  digitalWrite(inA, HIGH);
  digitalWrite(inB, LOW);
  digitalWrite(inC, HIGH);
  digitalWrite(inD, LOW);
   delay(1000);
}
/*---------------------------*/
void Stop()
{
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, LOW);
  delay(1000);
}
/*---------------------------*/
void SpinLeft(int HowMuch)
{
  analogWrite(enA, 250);
  analogWrite(enB, 0);
  digitalWrite(inA, LOW);
  digitalWrite(inB, HIGH);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, LOW); 
  delay(1000);

}
/*---------------------------*/
void SpinRight(int HowMuch)
{
  analogWrite(enA, 0);
  analogWrite(enB, 250);
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);  
  digitalWrite(inC, LOW);
  digitalWrite(inD, HIGH); 
  delay(1000);
 

}
/*---------------------------*/

unsigned int PingBlink()
{
  uS = sonar.ping(); 
  cm = uS / US_ROUNDTRIP_CM; 
  Serial.print("  cm = ");
  Serial.print(cm, DEC);

  if ((cm < 20) && (cm > 0))
  {
    return (cm);
  }
  else
  {
    return (100); 
  }
}

void PointServo(int ServoAngle)
{

  for (int i = 0; i < 20; i++) 
  {
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(ServoAngle);
    digitalWrite(SERVO_PIN, LOW);
    delay(20);
  }
}


void LookAround()  
{
  for (int Direction = 0; Direction < DirectionsToLook ; Direction ++)
  {
    Serial.print("Kierunek = ");
    Serial.print(Direction, DEC);
    PointServo(ServoDirectionData[Direction]); 
    delay(200);
    cm_now = PingBlink();    

    Serial.print(" cm = ");
    Serial.println(cm_now, DEC);
    if (cm_now < TARGET_TOO_CLOSE) 

    if (cm_now < 30)
    {
      TargetArray[TARGET_FOUND_ANY ] = true;
    }
    else TargetArray[TARGET_FOUND_ANY ] = false;

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 2))
    {
      TargetArray[TARGET_LEFT ] = true;
      Serial.println("*****Przeszkoda z prawej strony");
    }

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 1)) 
    {
      TargetArray[TARGET_CENTER ] = true;
      Serial.println("*****Przeszkoda z przodu");
    }

    if ((cm_now < TARGET_TOO_CLOSE) && (Direction == 0)) 
    {
      TargetArray[TARGET_RIGHT ] = true;
      Serial.println("*****Przeszkoda z lewej strony");
    }

  }

}

