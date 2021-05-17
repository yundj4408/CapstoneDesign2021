//참고 : https://m.blog.naver.com/PostView.nhn?blogId=roboholic84&logNo=221142584042&proxyReferer=https:%2F%2Fwww.google.com%2F
//왜 reset과 sleep을 연결하고? 왜 5V를 VDD에 주는지? (이건 모터드라이브 자체에도 전압이 필요하기 때문이겠지 ㅎ)
//역방향   -> 1A(초) 1B(검) 2A(파) 2B(빨)
//정방향   -> 1A(빨) 1B(파) 2A(초) 2B(검)
//
//데이터시트 17HS4401 모터 -> A(Black), A-(Green), B(Red), B-(Blue)
//
int dirPin = 3;
int stepPin = 4;

void setup(){
  pinMode(dirPin,OUTPUT);
  pinMode(stepPin,OUTPUT);
  digitalWrite(dirPin,HIGH);
}

void loop(){
  digitalWrite(stepPin,HIGH);
  delayMicroseconds(500);
  digitalWrite(stepPin,LOW);
  delay(10);
}   
