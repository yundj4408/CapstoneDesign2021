/*
  21.04.26 
    OnePahse Driver
    TwoPahse Driver
*/

// 1상 여자방식 -> 전류를 적게 먹는다. 1000 0100 0010 0001 
#define SmP1_1 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }
#define SmP1_2 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }
#define SmP1_3 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn,  LOW); }
#define SmP1_4 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn, HIGH); }

// 1-2상 여자방식 -> 1000 1100 0100 0110 0010 0011 0001 1001   pcw_value1= 1000 1000, value2= 1000 1100  
#define SmP12_1 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }
#define SmP12_2 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }
#define SmP12_3 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }
#define SmP12_4 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn,  LOW); }
#define SmP12_5 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn,  LOW); }
#define SmP12_6 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn, HIGH); }
#define SmP12_7 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn, HIGH); }
#define SmP12_8 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn, HIGH); }

// 2상 여자방식 -> 1100 0110 0011 1001
#define SmP2_1 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn,  LOW); }
#define SmP2_2 {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn, HIGH); digitalWrite(SetpBn, HIGH); }
#define SmP2_3 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn, HIGH); }
#define SmP2_4 {digitalWrite(SetpAp, HIGH); digitalWrite(SetpBp, HIGH); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }

// All Zero Output 0000
#define SmP0   {digitalWrite(SetpAp,  LOW); digitalWrite(SetpBp,  LOW); digitalWrite(SetpAn,  LOW); digitalWrite(SetpBn,  LOW); }

#define Steps 200
#define RunSteps Steps*5
#define StepDelay 1500 // Each Step Delay uSec
//int StepDelay = 1500
#define DelayT { delayMicroseconds(StepDelay);}
#define DelayH { delayMicroseconds(StepDelay/2);}

#define SmP1_Forward  {SmP1_1; DelayT; SmP1_2; DelayT; SmP1_3; DelayT; SmP1_4; DelayT;}
#define SmP1_Backward {SmP1_4; DelayT; SmP1_3; DelayT; SmP1_2; DelayT; SmP1_1; DelayT;}
#define SmP2_Forward  {SmP2_1; DelayT; SmP2_2; DelayT; SmP2_3; DelayT; SmP2_4; DelayT;}
#define SmP2_Backward {SmP2_4; DelayT; SmP2_3; DelayT; SmP2_2; DelayT; SmP2_1; DelayT;}
#define SmP12_Forward  {SmP12_1; DelayH; SmP12_2; DelayH; SmP12_3; DelayH; SmP12_4; DelayH; SmP12_5; DelayH; SmP12_6; DelayH; SmP12_7; DelayH; SmP12_8; DelayH;}
#define SmP12_Backward {SmP12_8; DelayH; SmP12_7; DelayH; SmP12_6; DelayH; SmP12_5; DelayH; SmP12_4; DelayH; SmP12_3; DelayH; SmP12_2; DelayH; SmP12_1; DelayH;}

// Step Motor Driver Pin
int SetpAp =  8;    // Step Motor A  빨강
int SetpAn =  9;    // Step Motor A- 초록
int SetpBp = 10;    // Step Motor B  노랑
int SetpBn = 11;    // Step Motor B- 파랑

int inByte = 0;
int i;

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(SetpAp, OUTPUT);   // digital sensor is on digital pin 2
  pinMode(SetpAn, OUTPUT);   // digital sensor is on digital pin 2
  pinMode(SetpBp, OUTPUT);   // digital sensor is on digital pin 2
  pinMode(SetpBn, OUTPUT);   // digital sensor is on digital pin 2
  
  MenuDisplay();
}


void loop() {
  while(Serial.available() > 0) 
  {
    inByte = Serial.parseInt();
    if ((inByte==0x0A)||(inByte==0x0D)) // Enter Key
      break;
    Serial.write(inByte);
    if (inByte == 1) 
    {
      Serial.print("1을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP1_Forward;
      //SmP0;
      MenuDisplay();
    }
    else if (inByte == 2) 
    {
      Serial.print("2을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP1_Backward;
      //SmP0;
      MenuDisplay();
    }
    else if (inByte == 3) 
    {
      Serial.print("3을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP2_Forward;
      //SmP0;
      MenuDisplay();
    }    
    else if (inByte == 4) 
    {
      Serial.print("4을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP2_Backward;
      //SmP0;
      MenuDisplay();
    }    
    else if (inByte == 5) 
    {
      Serial.print("5을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP12_Forward;
      //SmP0;
      MenuDisplay();
    }    
    else if (inByte == 6) 
    {
      Serial.print("3을 수행합니다.\n");
      for( i=0; i< RunSteps; i++)
        SmP12_Backward;
      //SmP0;
      MenuDisplay();
    }    
  }
}

void MenuDisplay(){
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("1: 1상 Clockwise\n");   // send a capital A
  Serial.print("2: 1상 Count Clockwise\n");
  Serial.print("3: 2상 Clockwise\n");   // send a capital A
  Serial.print("4: 2상 Count Clockwise\n");
  Serial.print("5: 12상 Clockwise\n");   // send a capital A
  Serial.print("6: 12상 Count Clockwise\n");
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("입력하시오 : ");      
}
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("Waiting For Connection.\n");   // send a capital A
    delay(300);
  }
}
