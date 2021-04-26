int Dir1Pin_A = 4;      // 제어신호 1핀
int Dir2Pin_A = 3;      // 제어신호 2핀
int Dir3Pin_A = 2;
int Dir4Pin_A = 7;
int SpeedPin_A = 8;    // PWM제어를 위한 핀
 
void setup() {
  pinMode(Dir1Pin_A, OUTPUT);             // 제어 1번핀 출력모드 설정
  pinMode(Dir2Pin_A, OUTPUT);             // 제어 2번핀 출력모드 설정
  pinMode(SpeedPin_A, OUTPUT);            // PWM제어핀 출력모드 설정
}
 
void loop() {
  
  digitalWrite(Dir1Pin_A, HIGH);         //모터가 정방향으로 회전
  digitalWrite(Dir2Pin_A, LOW);
  digitalWrite(Dir3Pin_A, HIGH);         //모터가 정방향으로 회전
  digitalWrite(Dir4Pin_A, LOW);
  analogWrite(SpeedPin_A, 100);          //모터 속도를 최대로 설정
  delay(1000);
  
  digitalWrite(Dir1Pin_A, HIGH);          //모터가 역방향으로 회전 
  digitalWrite(Dir2Pin_A, LOW);
  digitalWrite(Dir3Pin_A, HIGH);         //모터가 정방향으로 회전
  digitalWrite(Dir2Pin_A, LOW);          
  analogWrite(SpeedPin_A, 51);          //모터 속도를 1/5로 설정
  delay(3000);
 
  digitalWrite(Dir1Pin_A, LOW);         //모터가 정방향으로 회전
  digitalWrite(Dir2Pin_A, HIGH);
  digitalWrite(Dir3Pin_A, LOW);         //모터가 정방향으로 회전
  digitalWrite(Dir4Pin_A, HIGH);
  analogWrite(SpeedPin_A, 100);          //모터 속도를 최대로 설정
  delay(1000);
  
  digitalWrite(Dir1Pin_A, LOW);          //모터가 역방향으로 회전 
  digitalWrite(Dir2Pin_A, HIGH);          
  digitalWrite(Dir3Pin_A, LOW);          //모터가 역방향으로 회전 
  digitalWrite(Dir4Pin_A, HIGH);
  analogWrite(SpeedPin_A, 51);          //모터 속도를 1/5로 설정
  delay(3000);   
}
