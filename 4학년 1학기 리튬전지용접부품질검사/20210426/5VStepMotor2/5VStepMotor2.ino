//2상 여자를 통해 한 바퀴를 돌려보니 1상보다는 그래도 빠르게 돌아간다. Step 최소 간격 
//1상 여자를 사용할 땐


#define motorPin1 2 // IN1
#define motorPin2 3 // IN2
#define motorPin3 4 // IN3
#define motorPin4 5 // IN4
 
#define step 2048 // 1바퀴 스텝수
 
// 모터 회전 Loop 문 구현을 간단하게 하기위해 핀 배열 선언
int pinArray[4] = { motorPin1, motorPin2, motorPin3, motorPin4 };
// CW 시계방향 데이터 전송
int CW[4] = {
                0b1100,
                0b0110,
                0b0011,
                0b1001
            };
 
int CCW[4] = {
                0b1001,
                0b0011,
                0b0110,
                0b1100
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
        digitalWrite(pinArray[0], bitRead(CW[temp], 0));        //bitRead(x, n) x:읽을 숫자, n:읽을 비트, LSB(맨 오른쪽 비트)가 0, 왼쪽으로 갈수록 1씩 증가
        digitalWrite(pinArray[1], bitRead(CW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CW[temp], 3));
        delay(0.1);
    }
    delay(1000);
 
    for(int i = 0 ; i < step ; i++)
    {
        temp = i % 4;
        digitalWrite(pinArray[0], bitRead(CCW[temp], 0));
        digitalWrite(pinArray[1], bitRead(CCW[temp], 1));
        digitalWrite(pinArray[2], bitRead(CCW[temp], 2));
        digitalWrite(pinArray[3], bitRead(CCW[temp], 3));
        delay(2);
    }
    delay(1000);
}
