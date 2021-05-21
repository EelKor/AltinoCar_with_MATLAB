#include <Arduino.h>



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