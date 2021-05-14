/***********   알티노 블루투스 제어   ************/
/* ******************************************* */
// 작성일: 2021-04-28
// 작성자: 경상대학교 기계융합공학과 이승신 (2018013246)
// 목  적: 공학프로그래밍 프로젝트
// 설  명
// * 매트랩 으로 부터 10 바이트 패킷을 수신 받아 알티노의 원격 제어 구현
//
//
// Bluetooth Serial BaudRate = 9600bps
// HardWare Serial BaudRate =115200bps



/****************  매트랩 -> 알티노 패킷 프로토콜   ***************/
// 설  명
// 매트랩 에서 10바이트 패킷을 전송
//
// 구  조
// STX / Speed_H/ Speed_L /Steer_H /Steer_L/ None /None /None /None /ETX

// STX = 시작 바이트

// Speed_H = 전진 속도 100의 자리 이상
// Speed_L = 전진 속도 100의 자리 미만
// 코드 상에서 두개를 더함
// Ex) STX 1 55
// Speed = 155

// Steer_H = 조향값 100의 자리 이상
// Steer_L = 조향값 100의 자리 미만
// 코드 상에서 두개를 더하여 최종 조향값이 나옴

// None = 아직 할당 되지 않은 바이트

// ETX = 종료 바이트



/****************   알티노 -> 매트랩 패킷 프로토콜    **************/
// 설  명
// 알티노 에서 매트랩으로 응답 패킷 전송
//
// 구  조
// STX / Front_IR_H / Front_IR_L / Left_IR_H / Left_IR_L / Right_IR_H / Right_IR_L / Rear_IR_H / Rear_IR_L / ETX
//
// STX = 시작바이트
//
// Front_IR_H = 전방 적외선 센서값 100의 자리 이상
// Front_IR_L = 전방 적외선 센서값 100의 자리 미만

// 나머지 값들도 같은 원리로 명명.


/*******************  디버그 모드  **********************/
/***디버그 용으로 컴퓨터로 데이터가 전송 ( 시리얼 모니터)* */
/* ! 주의 ! 디버그 모드시 알티노는 작동 되지 않음         */
/* 사용하려면 주석 해제                                 */
/******************************************************/
/*                                                    */
//#define DEBUG_MODE
/*                                                    */
/******************************************************/


#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Altino.h>

int bufferflush();
int makeResponsePacket(long *IRSensor_data, unsigned char *response, int code);
int ObstacleAvoid();


// 알티노 센서 구조체 선언 및 구조체 포인터 선언
SensorData sdata;

// 블루투스 Tx Rx 핀이 아두이노에 연결된 위치
int blueTx = 5;
int blueRx = 6;
SoftwareSerial bluetooth(blueTx, blueRx);


// 시리얼 통신 데이터 합칠 임시 데이터
unsigned char data[8];
unsigned char response[15] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3};

//자동자 제어 변수
int throttle = 0;
int steering = 127;   //127 값이 중앙값 0 ~ 255
int steeringTrim = 127; // 127 값이 중앙 0 ~ 255

void setup()    
{
    Serial.begin(115200);
    bluetooth.begin(9600);
}

void loop() 
{

    // 센서 값 읽어오기
    #ifndef DEBUG_MODE
    sdata = Sensor(1);

    // 주행 안전 장치
    // 비상 제동 장치 및 회피 장치
    // 장애물 인식 
    int index = 140;
    if (throttle > 0) 
    {

        // 정면에 장애물
        if (sdata.IRSensor[0] > index && sdata.IRSensor[1] > index && sdata.IRSensor[2] > index ) 
        {
            Go(0,0);
            throttle = 0;

            Sound(23);
            delay(500);
            Sound(0);

            ObstacleAvoid();

        }

        // 좌측에 장애물
        else if(sdata.IRSensor[0] > index && sdata.IRSensor[1] < index && sdata.IRSensor[2] < index)  
        {
            Go(0,0);
            throttle = 0;
            Sound(20);
            delay(500);
            Sound(0);
        }

        // 우측에 장애물
        else if(sdata.IRSensor[0] < index && sdata.IRSensor[1] < index && sdata.IRSensor[2] > index)   
        {
            Go(0,0);
            throttle = 0;
            Sound(26);
            delay(500);
            Sound(0);

        }
        else
        {

        }
    }

    else  
    {
    Sound(0);
    }
    #endif

    /* ********** 블루투스 통신 부분 *********** */
    if (bluetooth.available() >= 9)
    {

        if (bluetooth.read() == 2)
        {
            bluetooth.readBytes(data, 8);
        }
        else   
        {
            // Flush, STX Error Send
            bufferflush();
            makeResponsePacket(&sdata.IRSensor[0], &response[0], 2);
            bluetooth.write(response,15);
        }


        if (bluetooth.available() == 0)
        {
            if (data[7] == 3)
            {
                // 속도 데이터를 숫자로 변환
                throttle = 1000;
                //(int)data[0] * 100 연산의 최적화 버
                throttle = (int)((data[0] << 7) + data[1]);
                throttle -= 1000;

                // 조향 데이터를 숫자로 변환
                // 수신받는 데이터 범위는 0 ~ 255 이나
                // 실제 사용은 -127 ~ 127 이므로 변환
                steering = 0;
                steering = (int)((data[2] << 7) + data[3]);
                steering -= 127;

                // 조향 Trim
                // 향후 기능 추가 예정
                steeringTrim = 127;
                steeringTrim -= 127;

                // 디버그 모드: 알티노 제어 변수값 출력
                #ifdef DEBUG_MODE
                Serial.println("Control Variables");
                Serial.print("int throttle = ");
                Serial.print(throttle);
                Serial.print("\n");
                Serial.print("unsigned int steering = ");
                Serial.print(steering);
                Serial.print("\n");
                Serial.print("unsigned int steeringTrim = ");
                Serial.println(steeringTrim);
                #endif

                // 릴리즈 모드: 알티노 제어
                #ifndef DEBUG_MODE
                Go(throttle,throttle);
                Steering2(steering, steeringTrim);
                #endif

                // 센서값 전송
                // 센서값 업데이트
                makeResponsePacket(&sdata.IRSensor[0], &response[0], 4);
                bluetooth.write(response,15);
            }
            else
            {
                //ETX Error, Flush
                bufferflush();
                makeResponsePacket(&sdata.IRSensor[0], &response[0], 3);
                bluetooth.write(response,15);

            }
            
        }
        else
        {
            if (data[7] == 3)
            {
                // Flush
                //정상인 경우

                
                // 속도 데이터를 숫자로 변환
                throttle = 1000;
                //(int)data[0] * 100 연산의 최적화 버
                throttle = (int)((data[0] << 7) + data[1]);
                throttle -= 1000;

                // 조향 데이터를 숫자로 변환
                // 수신받는 데이터 범위는 0 ~ 255 이나
                // 실제 사용은 -127 ~ 127 이므로 변환
                steering = 0;
                steering = (int)((data[2] << 7) + data[3]);
                steering -= 127;

                // 조향 Trim
                // 향후 기능 추가 예정
                steeringTrim = 127;
                steeringTrim -= 127;

                // 디버그 모드: 알티노 제어 변수값 출력
                #ifdef DEBUG_MODE
                Serial.println("Control Variables");
                Serial.print("int throttle = ");
                Serial.print(throttle);
                Serial.print("\n");
                Serial.print("unsigned int steering = ");
                Serial.print(steering);
                Serial.print("\n");
                Serial.print("unsigned int steeringTrim = ");
                Serial.println(steeringTrim);
                #endif

                // 릴리즈 모드: 알티노 제어
                #ifndef DEBUG_MODE
                Go(throttle,throttle);
                Steering2(steering, steeringTrim);
                #endif


                makeResponsePacket(&sdata.IRSensor[0], &response[0], 4);
                bluetooth.write(response,15);
                bufferflush();
            }
            else
            {
                // ETX Error, Flush
                bufferflush();
                makeResponsePacket(&sdata.IRSensor[0], &response[0], 3);
                bluetooth.write(response,15);
            }
            

        }
        

        
    }

    else  {

        if (bluetooth.available() != 0)
        {
            // flush, Dataloss Error Send
            bufferflush();
            makeResponsePacket(&sdata.IRSensor[0], &response[0], 1);
            bluetooth.write(response,15);
        }
        else
        {
            // No input Statement
            //makeResponsePacket(&sdata.IRSensor[0], &response[0], 0);
            //bluetooth.write(response,15);
        }
        
    }
    
}





 // 알티노 -> 매트랩 패킷 생성
int makeResponsePacket(long *IRSensor_data ,unsigned char *response, int code)  
{
    long hundred;
    long temp = 0;
    for(int i=0; i<6; i++)  
    {
        hundred = 0;
        temp = *(IRSensor_data + i);
        //bluetooth.println(temp);

        hundred = temp >> 8;
        *(response + (2*i+2)) = (char)hundred;
        *(response + (2*i+3)) = (char)(temp - (hundred << 8));

    }
    #ifdef DEBUG_MODE
    Serial.println("Func: int makeResponsePacket(long*, char*)");
    Serial.print("Response Packet\n");
    Serial.print(*IRSensor_data);
    Serial.print("\t");
    Serial.print(*IRSensor_data+1);
    Serial.print("\t");
    Serial.print(*IRSensor_data+2);
    Serial.print("\t");
    Serial.print(*IRSensor_data+3);
    Serial.print("\n");
    #endif

    switch (code)
    {

    // No Input Statement
    case 0:
        *(response+1) = 0;
        break;
    
    // Dataloss Error
    case 1:
        *(response+1) = 1;
        break;

    // STX Error
    case 2:
        *(response+1) = 2;
        break;

    // ETX Error
    case 3:
        *(response+1) = 3;
        break;

    // Ok
    case 4:
        *(response+1) = 4;
        break;

    
    default:
        break;
    }

    return 0;

}

// 블루투스 버퍼 비우는 함수
int bufferflush() 
{
    while (bluetooth.available()) 
    {
        bluetooth.read();
    }

    return 0;
}

int ObstacleAvoid()
{

    return 0;
}