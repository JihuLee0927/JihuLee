#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //핀 번호 주의!

#define ECHO 8
#define TRIG 9

//여기서부터 한글(삼색)전광판에 관련된 부분
int cur = 0;

long time_prev=0;
long time_cur=0;

char key_01 = ' ';
char key_02 = ' ';
  
String str_result;
//여기까지가 한글(삼색)전광판에 관련된 부분

void setup() 
{ 
  //한글(삼색)전광판 초기 설정
  mySerial.begin(9600);
  delay(100);

  init_dotmatrix();
  delay(2000);
  str_result = "'<0,0,22222:좋은 하루'>";
  hdot_println(str_result);
  delay(1000); 

  //초음파센서 초기 설정
  pinMode(TRIG, OUTPUT); //초음파 센서의 송신  
  pinMode(ECHO, INPUT);  //초음파센서의 수신

  //디지털입력 초기 설정
  pinMode(2,INPUT); //2^2 비트 
  pinMode(3,INPUT); //2^3 비트
  }


void loop() 
{ 

  int message;
  float distance; 

  digitalWrite(TRIG, HIGH); 
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW); 
   
  message = digitalRead(3)*8 + digitalRead(2)*4;
  distance = pulseIn(ECHO, HIGH) * 17/1000;

  if(distance < 10){
   str_result = "'<0,0,11111:조심하세요'>";
   
   hdot_println(str_result);

   //delay(500);
  }

  else if(message == 4){
   str_result = "'<0,0,33333:감사합니다'>";
   
   hdot_println(str_result);

   //delay(500);
  }

  else if(message == 8){
   str_result = "'<0,0,33333:죄송합니다'>";
   
   hdot_println(str_result);
   
   //delay(500);
  }

  else if (message == 12){
   str_result = "'<0,0,22222:먼저가세요'>";
   
   hdot_println(str_result);
   
   //delay(500); 
  }

  else{
   str_result = "'<0,0,22222:좋은 하루'>";
   
   hdot_println(str_result);
   
   //delay(500); 
  }

}

//여기서부터 한글(삼색)전광판 관련 함수
void init_dotmatrix()
{
  hdot_println("'<0,0,1:'>");
}

void hdot_println(String arg_msg) {
  time_cur = millis();
  mySerial.listen();
  mySerial.println(arg_msg);
  delay(1000);
  recv_check(); 
}

void recv_check() {
  
  while(1)
  {
    mySerial.listen();
    key_01 = key_02;
    key_02 = mySerial.read();      
    
    if(key_01 == 'o' && key_02 == 'k')
    {
      time_prev = time_cur;
      time_cur = millis();
      long time_value = time_cur - time_prev;
      break;
    }
    delay(1);
  }
  delay(100);

  while(mySerial.read() != -1);
}
