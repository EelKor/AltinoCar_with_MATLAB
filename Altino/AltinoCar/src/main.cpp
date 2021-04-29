/*  알티노 블루투스 제어   */
/* ********************** */
// 작성일: 2021-04-28
// 설명
// 매트랩 으로 부터 10 바이트 패킷을 수신 받음


/*****  패킷 프로토콜 ******/
// STX  0 0 0 1 1 1 x x ETX
// 000 = 전진 속도
// 111 = 조향값 이때 원래 조향값은 -127 ~ 127 이나
// 시리얼 통신으로 받는 값은 0 ~ 255
// 따라서 Steering2(x-127, ) 이런 식으로 대입
// xx = 임시 바이트
// ETX




#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Altino.h>
#include <math.h>

// 알티노 센서 구조체 선언
SensorData sdata;

// 블루투스 Tx Rx 핀이 아두이노에 연결된 위치
int blueTx = 5;
int blueRx = 6;
SoftwareSerial bluetooth(blueTx, blueRx);

// 시리얼 통신 데이터 합칠 임시 데이터
char data[10] = "\0";
bool receiveComplete = 0;
char success = '0';

//자동자 제어 변수
int throttle = 0;
unsigned int steering = 0;
unsigned int steeringTrim = 127;


void setup() {
    Serial.begin(115200);
    bluetooth.begin(9600);

}

void loop() {

// 블루투스 버퍼크기 10까지 대기
  if (bluetooth.available() >= 10) {

      // 수신 패킷 STX 확인
      if (bluetooth.read() == 0x02) {
        // 수신 받은 데이터 합치기
        for (int i = 0; i<7; i++) {
            data[i] = bluetooth.read();
          }

      // 속도 데이터를 숫자로 변환
        throttle = 0;
        throttle = ((int)data[0] << 8) + (int)data[1];

        // 조향 데이터를 숫자로 변
        steering = 0;
        steering = ((int)data[2] << 8) + (int)data[3];
        // 알티노 제
        Go(throttle,throttle);
        Steering2(steering-127, steeringTrim-127);

        bluetooth.println("Success");
      }

      // 잘못된 데이터 하니씩 제거
      else  {
        bluetooth.read();
      }
      }
    }
