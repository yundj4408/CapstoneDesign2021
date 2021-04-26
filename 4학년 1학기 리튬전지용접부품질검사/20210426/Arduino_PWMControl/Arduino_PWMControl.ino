int PWM_A = 2;
int PWM_B = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PWM_A, HIGH);
  for (int i=1; i<6; i++) {
    analogWrite(PWM_B, 50*i);
    delay(1000);  
  }
  
  
}
