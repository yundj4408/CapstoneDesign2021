//참고 : https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=221142584042&proxyReferer=https:%2F%2Fwww.google.com%2F
//    : https://lastminuteengineers.com/a4988-stepper-motor-driver-arduino-tutorial/
//왜 reset과 sleep을 연결하고? 왜 5V를 VDD에 주는지? (이건 모터드라이브 자체에도 전압이 필요하기 때문이겠지 ㅎ)
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

//-------------Pin Setting---------------
#define dirPin        3
#define stepPin       4
#define MS1           5
#define MS2           6
#define MS3           7
//-------------Step Setting-----------------------------------------------------------------------
#define Steps                 200
#define StepDelay             500
#define DelayH                {delayMicroseconds(StepDelay);}            
#define DelayL                {delay(StepDelay/50);}
#define Run                   {digitalWrite(stepPin, HIGH); DelayH; digitalWrite(stepPin, LOW); DelayL;}            
//-------------Phase Setting------------------------------------------------------------------------
#define TwoPhase              {digitalWrite(MS1, LOW); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}     //Full Step
#define OneTwoPhase           {digitalWrite(MS1, HIGH); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}    //Half Step
#define OneTwoPhase           {digitalWrite(MS1, HIGH); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}    //Half Step
#define OneTwoPhase           {digitalWrite(MS1, HIGH); digitalWrite(MS2, LOW); digitalWrite(MS3, LOW);}    //Half Step


int inByte = 0;
int i = 0;

void setup(){
  Serial.begin(9600);
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);
  digitalWrite(dirPin,HIGH);
  MenuDisplay();
}

void loop(){

   while(Serial.available() > 0) 
  {
    inByte = Serial.parseInt();
    if ((inByte==0x0A)||(inByte==0x0D)) // Enter Key
      break;
    Serial.write(inByte);
    if (inByte == 1) 
    {
      Serial.print("1을 수행합니다.\n");
      OneTwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
      MenuDisplay();
    }
    else if (inByte == 2) 
    {
      Serial.print("2을 수행합니다.\n");
      TwoPhase;
      for (i = 0; i < Steps; i++)
      {
        Run;
      }
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
