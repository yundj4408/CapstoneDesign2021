#define sm_LH {digitalWrite(IN1,LOW); digitalWrite(IN2,HIGH); digitalWrite(IN3,HIGH); digitalWrite(IN4,LOW);}
#define sm_LL {digitalWrite(IN1,LOW); digitalWrite(IN2,LOW); digitalWrite(IN3,HIGH); digitalWrite(IN4,HIGH);}
#define sm_HL {digitalWrite(IN1,HIGH); digitalWrite(IN2,LOW); digitalWrite(IN3,LOW); digitalWrite(IN4,HIGH);}
#define sm_HH {digitalWrite(IN1,HIGH); digitalWrite(IN2,HIGH); digitalWrite(IN3,LOW); digitalWrite(IN4,LOW);}

#define delayT {delayMicroseconds(delay_time);}
#define forward {sm_LH; delayT; sm_LL; delayT; sm_HL; delayT; sm_HH; delayT;}
#define backward {sm_HH; delayT; sm_HL; delayT; sm_LL; delayT; sm_LH; delayT;}

#define delay_time 1500
#define steps 200*5

//모터 드라이버 신호선 연결
int IN1=2;
int IN2=3;
int IN3=4;
int IN4=5;

void setup() {
  Serial.begin(9600);
  pinMode(IN1,OUTPUT); pinMode(IN2,OUTPUT); pinMode(IN3,OUTPUT); pinMode(IN4,OUTPUT);

}

void loop() 
{
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    int direct=Serial.parseInt();
    if(direct==1)
    {
      Serial.println("Normal Direction");
      for(int i=0;i<steps;i++)
        forward
    }
    else if(direct==2)
    {
      Serial.println("Reverse Direction");
      for(int i=0;i<steps;i++)
        backward        
    }
 
    
    else
      Serial.println("Choose dir '1' or '2');
  }

}
