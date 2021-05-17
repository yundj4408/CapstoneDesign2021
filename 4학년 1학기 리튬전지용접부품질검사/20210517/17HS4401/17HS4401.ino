//---------------------------------------------------참고-------------------------------------------------------------------------------------------------------------------------------------

//    : https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=221142584042&proxyReferer=https:%2F%2Fwww.google.com%2F
//    : https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/
//    : http://wiki.ywrobot.net/index.php?title=(SKU:ARD040401)JoyStick_Shield%E6%B8%B8%E6%88%8F%E6%91%87%E6%9D%86%E6%8C%89%E9%94%AE%E6%89%A9%E5%B1%95%E6%9D%BF_%E9%81%A5%E6%8E%A7%E5%99%A8

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//데이터시트 17HS4401 모터 -> A(Black), A-(Green), B(Red), B-(Blue)
//                         Step Angle: 1.8도, 1.7A
//모터 드라이브 1상 -> 
//역방향   -> 1A(초) 1B(검) 2A(파) 2B(빨)
//정방향   -> 1A(빨) 1B(파) 2A(초) 2B(검)
//StepPin을 High를 500us만큼 주고 LOw를 2ms만큼 줘서 PWM제어하는 것과 같이 사용할 수 있다.
//DirPin은 high를 줬을 때 clockwise, low를 줬을 때 counter clockwise로 동작한다. 
//DirPin default값은 그라운드 -> 연결을 1A(초) 1B(검) 2A(파) 2B(빨)
//STEP 핀과 DIR 핀은 내부에 풀업, 풀다운 저항에 연결되지 않았으므로 floating 상태(비연결 상태)로 둬서는 안됩니다.
//EnaPin은 Default값이 Low이기 때문에 항상 On이 되어 있다. 
//SLP과 RST은 Pin이 똑같이 active low input이다. RST가 LOW가 되면 모든 Step이 무시된다. 그래서 두개 다 high로 만들어줘야 한다. 
//The RST pin is floating. If you are not using the pin, you can connect it to the adjacent SLP/SLEEP pin to bring it high and enable the driver.
//1상은 없고 2상, 1-2상, W1-2상, 2W1-2상, 4W1-2상
//Microstep Select (MSx): 
//
//Full Step - 200, Half Step(1/2) - 400, Quarter step(1/4) - 800, Eighth step(1/8) - 1600, Sixteenth Step(1/16) - 3200

//
//-----------------------------------------------------------------------------------------------------------------

//-------------Pin Setting---------------
#define dirPin        3
#define stepPin       4
#define MS1           5
#define MS2           6
#define MS3           7
#define x_axis        A0

#define SW1           8
//-------------Step Setting-----------------------------------------------------------------------
#define Steps                 200
#define DelayH                {delayMicroseconds(StepDelay);}            
#define DelayL                {delay(StepDelay/50);}
#define Run                   {digitalWrite(stepPin, HIGH); DelayH; digitalWrite(stepPin, LOW); DelayL;}
//#define CWRun                   {digitalWrite(stepPin, LOW); DelayH; digitalWrite(stepPin, HIGH); DelayL;}
    
//-------------Phase Setting------------------------------------------------------------------------
#define TwoPhase              {digitalWrite(MS1, LOW); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}     //Full Step
#define OneTwoPhase           {digitalWrite(MS1, HIGH); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}    //Half Step
#define WOneTwoPhase          {digitalWrite(MS1, LOW); digitalWrite(MS2, HIGH); digitalWrite(MS3, LOW);}    //Quarter Step
#define W2OneTwoPhase         {digitalWrite(MS1, HIGH); digitalWrite(MS2, HIGH); digitalWrite(MS3, LOW);}    //Eighth Step
#define W4OneTwoPhase         {digitalWrite(MS1, HIGH); digitalWrite(MS2, HIGH); digitalWrite(MS3, HIGH);}    //Sixteenth Step



int inByte = 0;
int i = 0;
int StepDelay = 500;

void setup(){
  Serial.begin(9600);
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);
  pinMode(SW1, INPUT);
  //digitalWrite(dirPin,HIGH);
  //digitalWrite(dirPin,LOW);
  MenuDisplay();
}

void loop(){
  Run; 
  
  int x = map(analogRead(x_axis),0,1023,500,2000);          //To get the x-axis Value from joystick
  StepDelay = x;

  if (x < 1200) digitalWrite(dirPin, LOW);
  else if (x > 1300)
  {
    StepDelay = 2000 - x;
    digitalWrite(dirPin, HIGH);
  }
  
  Serial.println(digitalRead(SW1));
  
  //MenuSelect();
}

void MenuDisplay(){
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("1: TwoPhase\n");   // send a capital A
  Serial.print("2: OneTwoPhase\n");
  Serial.print("3: WOneTwoPhase\n");   // send a capital A
  Serial.print("4: 2WOneTwoPhase\n");
  Serial.print("5: 4WOneTwoPhase\n");   // send a capital A
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("입력하시오 : ");      
}

void MenuSelect(){

  
  while(Serial.available() > 0) 
  {
    inByte = Serial.parseInt();
    if ((inByte==0x0A)||(inByte==0x0D)) // Enter Key
      break;
    Serial.write(inByte);
    if (inByte == 1) 
    {
      Serial.print("1을 수행합니다.\n");
      TwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    else if (inByte == 2) 
    {
      Serial.print("2을 수행합니다.\n");
      OneTwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    else if (inByte == 3) 
    {
      Serial.print("3을 수행합니다.\n");
      WOneTwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    else if (inByte == 4) 
    {
      Serial.print("4을 수행합니다.\n");
      W2OneTwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    else if (inByte == 5) 
    {
      Serial.print("5을 수행합니다.\n");
      W4OneTwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    
  }
}
