/*
  21.04.26 
    1. 스텝 모터 28BYJ-48 테스트 -> 구동확인 but 상에 대한 개념 부족
    2. 리니어 액츄에이터 1.8 각 , 1.7 A 스텝 모터 구동
      문제점 -> 스텝모터 A,A-,B,B- 선 위치 및 L298N 선 연결 위치 인식못함
            -> 구동되지 않을 때 해결방안으로 시리얼모니터, 오실로스코프 못함 
      깨달은점 -> 아두이노 사용 시 디버깅을 하기위한 시리얼모니터 사용하기
              -> HIGH , LOW 대신 16진수로 표현하기
              -> 코드를 누구나 알아볼 수 있도록 짜기 ( Ex : 변수명, 함수명 등등)

    
    이 버젼의 문제점은 char(8 bit) 의 값이 64bit 으로 확장되면서 MSB 값이 1이므로 앞에 나머지 값들이 1로 확장되어 우리가 원하는 대로 실행되지 않았다. 
    ex) 0xFFFFFF88 -> 0xFFFFFFC4
    
    이 문제를 해결하기 위해서    
    pCW_Value = pCW_Value&0x0F; // 0x0F를 통해 첫번째 헥사 코드를 초기화 하고
    pCW_Value |= ((pCW_Value&0x0F)<<4); // 앞에 있는 코드를 통해 우리가 실질적으로 사용하는 2번째 헥사 코드를 앞에다가 복사하므로써, 예를 들어 0000 0100 -> 0100 0100
    다음을 추가하여 다음버젼을 업데이트 하겠다.        
    
*/

#define Steps 200

int SetpAp =  8;    // Step Motor A  빨강
int SetpAn =  9;    // Step Motor A- 초록
int SetpBp = 10;    // Step Motor B  노랑
int SetpBn = 11;    // Step Motor B- 파랑
int inByte = 0;
int i;

char pCW_Value = 0x88;
char cCW_Value = 0x11;

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
  
  
  //establishContact(); 
  // send a byte to establish contact until receiver responds
  MenuDisplay();
}

void UP_OneStep()  // 1상 여자방식 -> 전류를 적게 먹는다. 1000 0100 0010 0001 
{
  char temp;
  //pCW_Value = 0x88;
  temp = pCW_Value & 0x01;
  //Serial.println(temp, HEX);  
  pCW_Value >>= 1;  // Right Shift 1-bit
  pCW_Value = pCW_Value&0x0F;
  pCW_Value |= ((pCW_Value&0x0F)<<4);
  pCW_Value = pCW_Value | (temp<<7);
  
  digitalWrite(SetpAp, pCW_Value&0x08 );
  digitalWrite(SetpBp, pCW_Value&0x04 );
  digitalWrite(SetpAn, pCW_Value&0x02 );
  digitalWrite(SetpBn, pCW_Value&0x01 );
  
}

void UP_TwoStep()  // 2상 여자방식 -> 1100 0110 0011 1001
{
  char temp;
  pCW_Value |= 0x44;
  temp = pCW_Value & 0x01;
  pCW_Value >>= 1;  // Right Shift 1-bit
  pCW_Value = pCW_Value&0x0F;
  pCW_Value |= ((pCW_Value&0x0F)<<4);
  pCW_Value = pCW_Value | (temp<<7);
  
  digitalWrite(SetpAp, pCW_Value&0x08 );
  digitalWrite(SetpBp, pCW_Value&0x04 );
  digitalWrite(SetpAn, pCW_Value&0x02 );
  digitalWrite(SetpBn, pCW_Value&0x01 );
}

void UP_OneTwoStep()  // 1상2상 여자방식 -> 1000 1100 0100 0110 0010 0011 0001 1001   pcw_value1= 1000 1000,value2= 1000 1100  
{
  char temp;
  char pre_Value;
  int i = 1;
  if  (i%2 == 1)
  {
    pre_Value = pCW_Value;
    temp = pCW_Value & 0x01;
    pCW_Value >>= 1;
    pCW_Value = pCW_Value&0x0F;
    pCW_Value |= ((pCW_Value&0x0F)<<4);
    pCW_Value = pCW_Value | (temp<<7);
    pCW_Value |= pre_Value; 
  }
  else
  {
    pre_Value = pCW_Value;
    temp = pCW_Value & 0x01;
    pCW_Value >>= 1;
    pCW_Value = pCW_Value | (temp<<7);
    pCW_Value &= pre_Value;     
  }
  i++;
  
  digitalWrite(SetpAp, pCW_Value&0x08 );
  digitalWrite(SetpBp, pCW_Value&0x04 );
  digitalWrite(SetpAn, pCW_Value&0x02 );
  digitalWrite(SetpBn, pCW_Value&0x01 );
  Serial.println(pCW_Value, HEX);
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
      for( i=0; i< Steps; i++)
        UP_OneStep();
      MenuDisplay();
    }
    else if (inByte == 2) 
    {
      Serial.print("2을 수행합니다.\n");
      for( i=0; i< Steps; i++)
        UP_TwoStep();
      MenuDisplay();
    }
    else if (inByte == 3) 
    {
      Serial.print("3을 수행합니다.\n");
      for( i=0; i< Steps; i++)
        UP_OneTwoStep();
      MenuDisplay();
    }    
  }
}

void MenuDisplay(){
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("1: UniPolar One Step Clockwise\n");   // send a capital A
  Serial.print("2: UniPolar Two Step Clockwise\n");
  Serial.print("3: UniPolar One Two Step Clockwise\n");
  Serial.print("=====================================\n");   // send a capital A
  Serial.print("입력하시오 : ");      
}
void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print("Waiting For Connection.\n");   // send a capital A
    delay(300);
  }
}

/* Processing sketch to run with this example:

  // This example code is in the public domain.

  import processing.serial.*;

  int bgcolor;           // Background color
  int fgcolor;           // Fill color
  Serial myPort;                       // The serial port
  int[] serialInArray = new int[3];    // Where we'll put what we receive
  int serialCount = 0;                 // A count of how many bytes we receive
  int xpos, ypos;                // Starting position of the ball
  boolean firstContact = false;        // Whether we've heard from the microcontroller

  void setup() {
    size(256, 256);  // Stage size
    noStroke();      // No border on the next thing drawn

    // Set the starting position of the ball (middle of the stage)
    xpos = width / 2;
    ypos = height / 2;

    // Print a list of the serial ports for debugging purposes
    // if using Processing 2.1 or later, use Serial.printArray()
    println(Serial.list());

    // I know that the first port in the serial list on my Mac is always my FTDI
    // adaptor, so I open Serial.list()[0].
    // On Windows machines, this generally opens COM1.
    // Open whatever port is the one you're using.
    String portName = Serial.list()[0];
    myPort = new Serial(this, portName, 9600);
  }

  void draw() {
    background(bgcolor);
    fill(fgcolor);
    // Draw the shape
    ellipse(xpos, ypos, 20, 20);
  }

  void serialEvent(Serial myPort) {
    // read a byte from the serial port:
    int inByte = myPort.read();
    // if this is the first byte received, and it's an A, clear the serial
    // buffer and note that you've had first contact from the microcontroller.
    // Otherwise, add the incoming byte to the array:
    if (firstContact == false) {
      if (inByte == 'A') {
        myPort.clear();          // clear the serial port buffer
        firstContact = true;     // you've had first contact from the microcontroller
        myPort.write('A');       // ask for more
      }
    }
    else {
      // Add the latest byte from the serial port to array:
      serialInArray[serialCount] = inByte;
      serialCount++;

      // If we have 3 bytes:
      if (serialCount > 2 ) {
        xpos = serialInArray[0];
        ypos = serialInArray[1];
        fgcolor = serialInArray[2];

        // print the values (for debugging purposes only):
        println(xpos + "\t" + ypos + "\t" + fgcolor);

        // Send a capital A to request new sensor readings:
        myPort.write('A');
        // Reset serialCount:
        serialCount = 0;
      }
    }
  }

*/

/* Max/MSP version 5 patch to run with this example:

----------begin_max5_patcher----------
3908.3oc6ckziiaiE9b0+J3XjCIXpp.WzZNMURv.jCInQ5fYNjNngrDssRKK
4nkp6JA4+973hrkrsjncKu0SRiXasQ83G+dKj7QV+4qtaxzrOxKlf9Zzuft6
t+7U2cm7ThSbm936lrL3igIAExaaRJ+CYS+sI2qtTI+ikxSuBMKNojm+N3D4
Aua5KkPwpuoUAkgKhSm+tbdXo5cQXVOhuGwrohuHD4WT7iXzupen3HY4BuqG
rH0kzrrzxzfkb4kdJONHo9JoUKiSS3kRgjt4jYUk0mkznPJh+CYgHewpSqty
xWVwUh3jIqkEYEfmqQEMr.ETbB+YddQbVZix+tIAqV03z203QDX4ukIKHm6W
ep3T0ovqOUN+435m2Rcx+5U0E+FTzVBh9xOsHXIh5YuADg1x4IYgumG0r3mj
shmFmtJmWvSKCJ0um0WNhOKnJo7c6GmZe8YAg7Ne381Rc2j44wQYoBgn0SJN
c8qCHH1RhQqJi7NRCVsmGt.pGUESCxE31zDdCV.PRyxRZeo0MU.WOHMdYPIu
LVIrT75BMd4p73zxVuHdZ.TFKJByyRRZUTpq77dtRDzZFx+PbT4BYY0DJgaO
dUcSvj0XTT7bdQY6yUFLun8YZo71jl0TIt042RYNLa4RfCTWfsznKWDWfJpl
tJHrbgV6t.AZInfzWP.4INpJHA8za91u+6QN1nk7hh.PpQwonxEbTAWzpilV
MimilkmsDtPbo3TPiUdY0pGa9ZShS4gYUJz1pwE1iwCpxbAgJI9DGGwWNzFT
ksLf3z7M0MybG6Hj1WngsD7VEXS8j5q7Wu5U0+39ir8QJJS5GMHdtRimL4m1
0e1EVX0YsE2YssINriYRoFRyWVMoRRUGQvnkmms3pnXDYHbBKMPpIOL5i1s8
3rMPwFcRCsGRyPH780.8HBnpWz.vlEQBWJ+0CSunehJSmJxiIZRtNGhhDYrU
jt3ZQyA2fHJhZDifXIQHUHH8oGYgOREI5nqHIzhFWUndPyBdB3VzHJGwUhkV
rgvRl2UCVNMHcd234lf1DN16HFEIdHt99A5hrp7v5WWMSBQZgMP.Tkwoqig8
W1.Sn1f3h3nn1wLpBypPDzlJ7XinEGkLiMPloWOhrgR7dpZWJQV1faDy35Qj
MThMFkWFGsJChQPqrQp8iorV6Q28HBVF4nMVDJj7f1xyYACFScisg.ruLHOW
uMUS4Am4pI4PTnHi.6bi02HNzSYnDBe4cgAgKzRk1jc8PJLoH3Ydz6.Q.7K8
tfxx73oUkJq1MGuCy5TpAi.POWZ3AenidLOOIaZPhdjZVW3sdk6LXEGzHb7p
Mfr7SEy3SXHyBSxJ3J2ncNNYVJsXG6Me10nj4cfCRFdTFjLo7q3SiCpjjEDM
.nvra.GN39.E2CDTHWXPo8.xzfqrHCHKnf5QUYUVdoZPUjCSC7LU8.XtTUXl
X8vr51GjwFGLC2AlMdLkU4RiaRrnmJuiudnDk0ZW+9p6TuKBe433JUCzp6fU
iOF0SUk2UQYUPNTEkiZubvKa1tsmgL5SCTXGHnnG0CceLpkpR9Rs28IUESWl
EwWNKfHlg.zj6Ee7S+nE8A+m9F7Cu40u9gMm+aRp3kYYkKd3GDOz5y+c7b96
K9gfvuIK68uNO6g2vUUL80WxihCVFD9vlB30e2SOrmxUb527RZ3nZNrljGrR
70vs1J9suWuZ3zaHVdG3RIJLgGj2Gfn6TcGcstEfvtH.hpFLlnBndjOLGQAI
z98BXc6yQxghmOn6gZqj0ShPOXhynLOjzCESt+XwE8TxrCvrdXo16rqnLgvb
HaFmbh29QD+K0DyNdjDwvzQL.NXpoMvoOBxkger0HwMRQbpbCh91fjjG9Idw
prTH9SzaSea5a.GQEPnnh43WNefMlsOgx18n.vgUNO.tKl7tDyI3iHzafJHZ
VVNedVEbGgYIY42i93prB0i7B7KT1LnnCiyAiinpBnsPV7OG.tYKfBsrJOkG
UG5aq26iJw6GyJ4eM5mEgEKaNQPMEBUp.t8.krplOVTlZdJAW27bjvGK7p2p
HQPgLOSJDYv4E9gQBYBjMUselRxDy+4WplIzm9JQAWOEmfb.E364B43CAwp5
uRRDEv8hWXprjADMUOYpOg9.bVQpEfhKgGCnAnk.rghBJCdTVICA3sDvAhE5
oU4hf67ea5zWPuILqrD8uiK+i477fjHIt9y.V88yy3uMsZUj7wnxGKNAdPx5
fAZMErDZOcJU4M01WFQokix.pKa+JE1WacmnKFeYd7b.0PeIzB8Kk+5WIZpB
Ejt34KJeHgOCh4HK8Y3QiAkAfs8TRhhOkG7AAGQf0qxyfmQxa+PLb8Ex.2PS
4BdO5GB9Hvg+cfJCMofAIMu9Qz+UPCjckqVJlEmyA8Bf.rC6.3hAEuG8TdTU
bZljQ0nr1ayIqmTwQYfyRGafZhur5vfuyMSqYNWmtAPwWHalDSuUgT0Bosh.
JpAR89Y6Ez5QEfPTQO4J0DHLInIliz8BZV2JfV3Bd36qsQwAVVXbr1BGXp6s
Sd5sSDruo74wofx.HxUgxQwTnMLqTXvRmiGh2PUZr5pBynKChjl6feNUjSRn
hEUfRPT1GfG9Ik4TQBm.hEZZ.bc38HjAMKGzDRijEm1ifx1dbgzQyKh6FZc3
wOCkRJH+KUh0daWs6wzltWx1puXxlWW6NZWY2JiTBzzILRIANku02NourySM
VI1VJTvQZff32AJr+dS9e34QAoA6EGXlGFH9yk7yyQAlVd3SR94g+TxOu1sU
Flgd6ICI96LzazyPu1cgqsZ8r74SgF.65+efbMf4pGHT7lgHh30Sha3N5Ia.
oqjMf7nsuMwycf7iYDybiAAVr3eC.oTMjpzEr8GDRc9bFRGHYXDrzg.Tlx+q
NW8TY1IkzCfZ2IftkQstbB08HUezoDS+oFyI.cWIhWBaDiUo7qIrDO7f.L6n
AXqCmyNT9act.z+Iv.GR0uES0ZXfjdz.IczAxQOUR+zvRsUTigRxmyPYeNlj
yXv8Peef2ZFzuLzWPPeAE8ELzWXYlhe8WzAcUg+b1UkIoCLzIH60zwASGXau
a1Dq2nUY.sox4vng+m0nACePngC9lEMLZMBPodOxf+yx5d4uMCTHm3kJvIIG
jcLMedEQldkjpoBkQyjY1Hk.hmSY95Iwos8NDb9VSlIWOIntqgxryUjL6bCJ
y1lli5tWWxrQ7YmqGYlc6shK1iY2dr0wtNjYxgHyzaq0OznY235awCr8zSz6
EGd1QNUKf.74dADTBbTbeotjpW95IolY0WpKYONY8M83Rx2MChx3fL+iG5Mm
tXpdmvXj8uTvaAL1WjbbarQD4Z6kXBpnm6a69oKV2PY9WY174IbC3CaRQ9iK
Q4sYGQpwdtZ5wFrc7n569.M83OOR5ydSB1ZcAWCxdbKuavz9LILxfD.wWO.W
Nq+Zu4Es+AP6s5p9jDWH8ET+c85+XbW0.N1nDCTD7U4DGc6ohnU019fS7kQ0
o43luuOGjv5agHp0DT.CysOfgLR3xXlXTUKm16RivRsn3z0O6cl3YScAvtrb
hwekGB7BZuqESUzBJWmCvK7t9HF8Ts6cUAPoFWso3aP8ApWyJ3wqOPo2pJDC
BQ0NI0Pj8QCQ2r1L5vKaU5lDRYX7yRur1UYYZmJQ9iDHwN9dndB5n5ejflmm
UsBwLHnDkKXWRuAkb3NeuzqRstiQGP.fCQFdHNzaE.8u58Nz9svFE9SGIE1X
kv9Iwfl1BdNWjA7xcThsWCS847loyFD8pZq2E2F04lYULzBTDYhrFSDDJdjo
fisN2NUN26e4xRu51zD5ZseJ4HC63WyIX6jRqsp0jangBnK.Qlo58PCpWevt
ahzqK7fbKsdX6R64aao8LmWhBPh9jKVAPMzb5a2cV6opdWHneMmqMEmAGsPh
ieigIjV+4gF1GgbMNXg+NH44YaRYyd..S1ThHzKhFwwGRaWVITqyj9FvPqMT
d0pDuSqDrOGF.Uogf.juCFi9WAUkYR+rFPanDcPG8SbrtjyG03ZQ8m3AqC5H
NcUUoXSwVrqXKVcZu.5ZnkwIfIVdXVZTwAuTTUiYuxwjZDK6ZgnRtYV8tJmP
hEcuXgz2Goxyaiw35UkaWbpqtfzD02oUkkYqi.YQbZqIIWrIljFolsdmMKFR
wCJ2+DTn.9QlkOld+d9Qy9IJdpLfy05Ik2b8GsG9h8rdm1ZFx1FrmmlA2snw
qI9Mcdi2nr6q3Gc87nLawurbw1dda+tMyGJ9HaQmlkGwy6davisMgrkM65oz
eulfYCzG46am8tSDK144xV4cEvVMTRXq9CIX8+ALNWb6sttKNkiZetnbz+lx
cQnb1Nds2C0tvLNe14hwQtxYbxhqc17qHfamUcZZ3NYSWqjJuiDoizZ+ud2j
naRK4k3346IIVdR1kKiQjM39adMamvc6n+Xp36Yf3SIGh3uKbquqs1JksTII
kuJ7RrZSFb2Cn9j5a6DT8cMo0iczU+lsYaU8YNVh5k5uzJLU26ZcfuJE6XLY
0mcRp9NTCp+L+Ap+in7Xf3b9jFQBLtIY06PbrGhcrU6N00Qlaf9N0+QPo9nS
P6qsI7aYNLSNOHpsAxis0ggnZLjYqyyFkdSqinVsPaqSDZaYBZ6c93uLCjGm
iCroJVLzU45iNE.pIUfs3TWb.0FejHp9uANr0GcJPTroFDNOHpkIweLnI1QT
dHl3P7LhOF3Ahd9rnvLwAMy5JSdNezGlsIsW9mW44r26js+alhxjlkdhN0YE
YqiH5MTeWo6D4Qm.ieLS7OynmuVGSbmbFUlnWWhiQlhOeN+Yl35bq.tGo9JR
cj8AVqdz7nSgVB9zNj.FTOU68o5d9KO5TUOGxVMw+jTO8T6wqD0hEiHsOJO5
TTOMoS.zlqN0SpZjz6GcH05ylVM0jwuidlkmAif374ih5M5QPfccr8Hqifff
otN8pt3hUcaWu8nosBhwmD0Epw5KmoF.poxy4YHbnjqfPJqcM3Y2vun7nS.i
f3eETiqcRX2LR.4QmhZrkoCSGwzZrqKHrVR8caari+55d2caPqmq5n.ywe8Q
WrZL9fpwVXeaogMByE6y1SMdjk+gbavbN7fYvVtt1C2XwHJSzpk+tidUO25H
UB9onw9mlFQ10fhpZBaDatcMTTEGcJpwzqg92qqiVtM6Cu0IRQ0ndEdfCAqV
l0qYAUmPrctbxO4XCuPMa1asYzKDks1D52ZCne6Mednz9qW8+.vfqkDA
-----------end_max5_patcher-----------

*/
