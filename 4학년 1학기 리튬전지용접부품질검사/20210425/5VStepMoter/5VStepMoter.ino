//현재는 하나의 자석에 스테이더 하나씩 여자하여 로터를 회전시키는 방식이였어서 속도가 많이 느렸다. 
//다음 버전에서는 두 개의 자석에 스테이더 여자하여 로터를 회전 시킬 것이다. 



#define motorPin1 2 // IN1
#define motorPin2 3 // IN2
#define motorPin3 4 // IN3
#define motorPin4 5 // IN4
 
#define step 2048 // 1봐퀴 스텝수
 
// 모터 회전 Loop 문 구현을 간단하게 하기위해 핀 배열 선언
int pinArray[4] = { motorPin1, motorPin2, motorPin3, motorPin4 };
// CW 시계방향 데이터 전송
int CW[4] = {
                0b1000,
                0b0100,
                0b0010,
                0b0001
            };
 
int CCW[4] = {
                0b0001,
                0b0010,
                0b0100,
                0b1000
            };
 
void setup(){
    // 스텝모터 드라이브 보드의 IN 연결핀 출력으로 설정
    for(int i = 0 ; i < 4 ; i++ ) pinMode(pinArray[i], OUTPUT);
}
 
void loop(){
    // CW방향 1바퀴 후, CCW 1바퀴 
    int temp;
    for(int i = 0 ; i < step ; i++)
    {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CW[temp], 3));
        delay(3);
    }
    delay(1000);
 
    for(int i = 0 ; i < step ; i++)
    {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CCW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CCW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CCW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CCW[temp], 3));
        delay(3);
    }
    delay(1000);
}
