#include <Arduino.h>
#include <Altino.h>
#include <math.h>
unsigned char tx_d[28] = { 0, };
unsigned char Sendbuf[28] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned char rx_data[300]= { 0, };
unsigned char rx_d[31] = { 0, };
unsigned char rx_d56[56]= { 0, };
unsigned char rx_d_sensor1[31] = { 0, };
unsigned char rx_d_sensor2[31] = { 0, };
int len = 0;
long rxcnt=0;


unsigned char check_sum_tx_calcuration(int u16_cnt)
{
  int u16_tx_check_sum = 0;
  int u16_tx_cnt;
  u16_tx_check_sum = u16_tx_check_sum + tx_d[1];
  for (u16_tx_cnt = 3; u16_tx_cnt <= u16_cnt; u16_tx_cnt++) {
    u16_tx_check_sum = u16_tx_check_sum + tx_d[u16_tx_cnt];
  }
  u16_tx_check_sum = u16_tx_check_sum % 256;
  return (byte)(u16_tx_check_sum);
}

void SendData(unsigned char *Sendbuf)
{

  int k;
  tx_d[0] = 0x2;
  tx_d[1] = 28;

  tx_d[3] = 1;
  tx_d[4] = Sendbuf[4];
  tx_d[5] = Sendbuf[5];
  tx_d[6] = Sendbuf[6];
  tx_d[7] = Sendbuf[7];
  tx_d[8] = Sendbuf[8];
  tx_d[9] = Sendbuf[9];
  tx_d[10] = Sendbuf[10];
  tx_d[11] = Sendbuf[11];
  tx_d[12] = Sendbuf[12];
  tx_d[13] = Sendbuf[13];
  tx_d[14] = Sendbuf[14];
  tx_d[15] = Sendbuf[15];
  tx_d[16] = Sendbuf[16];
  tx_d[17] = Sendbuf[17];
  tx_d[18] = Sendbuf[18];
  tx_d[19] = Sendbuf[19];
  tx_d[20] = Sendbuf[20];
  tx_d[21] = Sendbuf[21];

  if (Sendbuf[4] == 1 || Sendbuf[4]==10) {
	    Sendbuf[21] = Sendbuf[21] | 0x01;
  }
  else  {
	    Sendbuf[21] = Sendbuf[21]&0xFE;
  }

  tx_d[22] = Sendbuf[22];
  tx_d[23] = Sendbuf[23];
  tx_d[24] = Sendbuf[24];
  tx_d[25] = Sendbuf[25];
  tx_d[26] = 255;
  tx_d[27] = 0x3;

  tx_d[2] = check_sum_tx_calcuration(27);
  Serial.write(tx_d, 28);
  delay(20);
}

void Go(int left, int right)
{
  unsigned int temp=32768;
  unsigned int left_int, right_int;


  if (left < 0) {
    left_int = temp - left;
  }
  else{
	  left_int = left;
  }

  if (right<0){
    right_int = temp - right;
  }
  else {
	  right_int=right;
  }

  if (right==0)
    Sendbuf[6] = 255;
  else
    Sendbuf[6] = 0;

  Sendbuf[7] = (byte)(right_int / 256);
  Sendbuf[8] = (byte)(right_int % 256);

  if (left == 0)
    Sendbuf[9] = 255;
  else
    Sendbuf[9] = 0;

  Sendbuf[10] = (byte)(left_int / 256);
  Sendbuf[11] = (byte)(left_int % 256);

  SendData(Sendbuf);
}


void Steering(int steeringvalue)
{
	Sendbuf[24] =0;
	Sendbuf[5] = (byte)steeringvalue;
	SendData(Sendbuf);
}

void Steering2(int value1, int value2)
{
	if(value1>127)
		value1=127;
	if(value2>127)
		value2-127;
	if(value1<-127)
		value1=-127;
	if(value2<-127)
		value2=-127;
	if(value1<0)
		value1=128-value1;
	if(value2<0)
		value2=128-value2;

	Sendbuf[5] = (byte)value1;
	Sendbuf[24] = 1;
	Sendbuf[25] = (byte)value2;
	SendData(Sendbuf);
}

void Steering3(int value1)
{
	if(value1>127)
		value1=127;

	if(value1<-127)
		value1=-127;

	if(value1<0)
		value1=128-value1;

	Sendbuf[5] = (byte)value1;
	Sendbuf[24] = 1;
	Sendbuf[25] = (byte)0;
	SendData(Sendbuf);
}


void Sound(unsigned char buzzer)
{
	Sendbuf[22] = (byte)buzzer;
	SendData(Sendbuf);
}

void Display(unsigned char ASCII)
{
    if(ASCII>128)
    {
        ASCII = ASCII-128;
    }
    else if(ASCII>0 && ASCII <128)
    {
        ASCII = 128+ ASCII;
    }

    Sendbuf[12] = ASCII;
	Sendbuf[13] = 0;
	Sendbuf[14] = 0;
	Sendbuf[15] = 0;
	Sendbuf[16] = 0;
	Sendbuf[17] = 0;
	Sendbuf[18] = 0;
	Sendbuf[19] = 0;
	Sendbuf[20] = 0;
	SendData(Sendbuf);
}

void DisplayLine(unsigned char dot0, unsigned char dot1, unsigned char dot2, unsigned char dot3, unsigned char dot4, unsigned char dot5, unsigned char dot6, unsigned char dot7)
{
	Sendbuf[12] = 0;
	Sendbuf[13] = dot7;
	Sendbuf[14] = dot6;
	Sendbuf[15] = dot5;
	Sendbuf[16] = dot4;
	Sendbuf[17] = dot3;
	Sendbuf[18] = dot2;
	Sendbuf[19] = dot1;
	Sendbuf[20] = dot0;

    SendData(Sendbuf);

}

void Displayon(int x, int y)
{
    unsigned char dotx;
	Sendbuf[12] = 0;
	dotx=Sendbuf[21-x];

    Sendbuf[21-x] = dotx | (unsigned char)(0x01<<(y-1));

	SendData(Sendbuf);
}

void Displayoff(int x, int y)
{
    unsigned char dotx;
	Sendbuf[12] = 0;
	dotx=Sendbuf[21-x];

    Sendbuf[21-x] = dotx & ((unsigned char)(255-(0x01<<y-1)));

	SendData(Sendbuf);
}

void Led(long led)
{
	Sendbuf[23] = (byte)(led%256);
	Sendbuf[21] = (byte)(led/256);
	SendData(Sendbuf);
}

void Light(long led)
{
    Led(led);
}

void chekdata()
{
	long rx_check_sum;

	if ((rx_d[0] == 2) && (rx_d[30] == 3) && (rx_d[1] == 31)) {
		rx_check_sum = rx_d[0];
		rx_check_sum = rx_check_sum + rx_d[1];
		rx_check_sum = rx_check_sum + rx_d[3];
		rx_check_sum = rx_check_sum + rx_d[4];
		rx_check_sum = rx_check_sum + rx_d[5];
		rx_check_sum = rx_check_sum + rx_d[6];
		rx_check_sum = rx_check_sum + rx_d[7];
		rx_check_sum = rx_check_sum + rx_d[8];
		rx_check_sum = rx_check_sum + rx_d[9];
		rx_check_sum = rx_check_sum + rx_d[10];
		rx_check_sum = rx_check_sum + rx_d[11];
		rx_check_sum = rx_check_sum + rx_d[12];
		rx_check_sum = rx_check_sum + rx_d[13];
		rx_check_sum = rx_check_sum + rx_d[14];
		rx_check_sum = rx_check_sum + rx_d[15];
		rx_check_sum = rx_check_sum + rx_d[16];
		rx_check_sum = rx_check_sum + rx_d[17];
		rx_check_sum = rx_check_sum + rx_d[18];
		rx_check_sum = rx_check_sum + rx_d[19];
		rx_check_sum = rx_check_sum + rx_d[20];
		rx_check_sum = rx_check_sum + rx_d[21];
		rx_check_sum = rx_check_sum + rx_d[22];
		rx_check_sum = rx_check_sum + rx_d[23];
		rx_check_sum = rx_check_sum + rx_d[24];
		rx_check_sum = rx_check_sum + rx_d[25];
		rx_check_sum = rx_check_sum + rx_d[26];
		rx_check_sum = rx_check_sum + rx_d[27];
		rx_check_sum = rx_check_sum + rx_d[28];
		rx_check_sum = rx_check_sum + rx_d[29];
		rx_check_sum = rx_check_sum + rx_d[30];
		rx_check_sum = rx_check_sum % 256;

		if (rx_check_sum == rx_d[2])
		{
			if (rx_d[4] == 1)
			{
				rx_d_sensor1[7] = rx_d[7];
				rx_d_sensor1[8] = rx_d[8];
				rx_d_sensor1[9] = rx_d[9];
				rx_d_sensor1[10] = rx_d[10];
				rx_d_sensor1[11] = rx_d[11];
				rx_d_sensor1[12] = rx_d[12];
				rx_d_sensor1[13] = rx_d[13];
				rx_d_sensor1[14] = rx_d[14];
				rx_d_sensor1[15] = rx_d[15];
				rx_d_sensor1[16] = rx_d[16];
				rx_d_sensor1[17] = rx_d[17];
				rx_d_sensor1[18] = rx_d[18];
				rx_d_sensor1[19] = rx_d[19];
				rx_d_sensor1[20] = rx_d[20];
				rx_d_sensor1[21] = rx_d[21];
				rx_d_sensor1[22] = rx_d[22];
				rx_d_sensor1[23] = rx_d[23];
				rx_d_sensor1[24] = rx_d[24];
				rx_d_sensor1[25] = rx_d[25];
				rx_d_sensor1[26] = rx_d[26];
			}
			else
			{
				rx_d_sensor2[7] = rx_d[7];
				rx_d_sensor2[8] = rx_d[8];
				rx_d_sensor2[9] = rx_d[9];
				rx_d_sensor2[10] = rx_d[10];
				rx_d_sensor2[11] = rx_d[11];
				rx_d_sensor2[12] = rx_d[12];
				rx_d_sensor2[13] = rx_d[13];
				rx_d_sensor2[14] = rx_d[14];
				rx_d_sensor2[15] = rx_d[15];
				rx_d_sensor2[16] = rx_d[16];
				rx_d_sensor2[17] = rx_d[17];
				rx_d_sensor2[18] = rx_d[18];
				rx_d_sensor2[19] = rx_d[19];
				rx_d_sensor2[20] = rx_d[20];
				rx_d_sensor2[21] = rx_d[21];
				rx_d_sensor2[22] = rx_d[22];
				rx_d_sensor2[23] = rx_d[23];
				rx_d_sensor2[24] = rx_d[24];
				rx_d_sensor2[25] = rx_d[25];
			}
		}
	}
}

void chekdata56()
{
	long rx_check_sum=0;


	if ((rx_d56[0] == 2) && (rx_d56[55] == 3) && (rx_d56[1] == 56)) {
		rx_check_sum = rx_d56[0];
		rx_check_sum = rx_check_sum + rx_d56[1];
		rx_check_sum = rx_check_sum + rx_d56[3];
		rx_check_sum = rx_check_sum + rx_d56[4];
		rx_check_sum = rx_check_sum + rx_d56[5];
		rx_check_sum = rx_check_sum + rx_d56[6];
		rx_check_sum = rx_check_sum + rx_d56[7];
		rx_check_sum = rx_check_sum + rx_d56[8];
		rx_check_sum = rx_check_sum + rx_d56[9];
		rx_check_sum = rx_check_sum + rx_d56[10];
		rx_check_sum = rx_check_sum + rx_d56[11];
		rx_check_sum = rx_check_sum + rx_d56[12];
		rx_check_sum = rx_check_sum + rx_d56[13];
		rx_check_sum = rx_check_sum + rx_d56[14];
		rx_check_sum = rx_check_sum + rx_d56[15];
		rx_check_sum = rx_check_sum + rx_d56[16];
		rx_check_sum = rx_check_sum + rx_d56[17];
		rx_check_sum = rx_check_sum + rx_d56[18];
		rx_check_sum = rx_check_sum + rx_d56[19];
		rx_check_sum = rx_check_sum + rx_d56[20];
		rx_check_sum = rx_check_sum + rx_d56[21];
		rx_check_sum = rx_check_sum + rx_d56[22];
		rx_check_sum = rx_check_sum + rx_d56[23];
		rx_check_sum = rx_check_sum + rx_d56[24];
		rx_check_sum = rx_check_sum + rx_d56[25];
		rx_check_sum = rx_check_sum + rx_d56[26];
		rx_check_sum = rx_check_sum + rx_d56[27];
		rx_check_sum = rx_check_sum + rx_d56[28];
		rx_check_sum = rx_check_sum + rx_d56[29];
		rx_check_sum = rx_check_sum + rx_d56[30];
		rx_check_sum = rx_check_sum + rx_d56[31];
		rx_check_sum = rx_check_sum + rx_d56[32];
		rx_check_sum = rx_check_sum + rx_d56[33];
		rx_check_sum = rx_check_sum + rx_d56[34];
		rx_check_sum = rx_check_sum + rx_d56[35];
		rx_check_sum = rx_check_sum + rx_d56[36];
		rx_check_sum = rx_check_sum + rx_d56[37];
		rx_check_sum = rx_check_sum + rx_d56[38];
		rx_check_sum = rx_check_sum + rx_d56[39];
		rx_check_sum = rx_check_sum + rx_d56[40];
		rx_check_sum = rx_check_sum + rx_d56[41];
		rx_check_sum = rx_check_sum + rx_d56[42];
		rx_check_sum = rx_check_sum + rx_d56[43];
		rx_check_sum = rx_check_sum + rx_d56[44];
		rx_check_sum = rx_check_sum + rx_d56[45];
		rx_check_sum = rx_check_sum + rx_d56[46];
		rx_check_sum = rx_check_sum + rx_d56[47];
		rx_check_sum = rx_check_sum + rx_d56[48];
		rx_check_sum = rx_check_sum + rx_d56[49];
		rx_check_sum = rx_check_sum + rx_d56[50];
		rx_check_sum = rx_check_sum + rx_d56[51];
		rx_check_sum = rx_check_sum + rx_d56[52];
		rx_check_sum = rx_check_sum + rx_d56[53];
		rx_check_sum = rx_check_sum + rx_d56[54];
		rx_check_sum = rx_check_sum + rx_d56[55];

		rx_check_sum = rx_check_sum % 256;

		if (rx_check_sum==rx_d56[2])
		{
            rx_d_sensor1[7] = rx_d56[7]; //ir1
            rx_d_sensor1[8] = rx_d56[8]; //ir1
            rx_d_sensor1[9] = rx_d56[9];  //ir2
            rx_d_sensor1[10] = rx_d56[10]; //ir2
            rx_d_sensor1[11] = rx_d56[11]; //ir3
            rx_d_sensor1[12] = rx_d56[12]; //ir3
            rx_d_sensor1[13] = rx_d56[13]; //ir4
            rx_d_sensor1[14] = rx_d56[14]; //ir4
            rx_d_sensor1[15] = rx_d56[15]; //ir5
            rx_d_sensor1[16] = rx_d56[16]; //ir5
            rx_d_sensor1[17] = rx_d56[17]; //ir6
            rx_d_sensor1[18] = rx_d56[18]; //ir6

            rx_d_sensor1[19] = rx_d56[19]; //right torque
            rx_d_sensor1[20] = rx_d56[20]; //right torque

            rx_d_sensor1[21] = rx_d56[21]; //left torque
            rx_d_sensor1[22] = rx_d56[22]; //left torque

            rx_d_sensor1[23] = rx_d56[49];  //temperature
            rx_d_sensor1[24] = rx_d56[50];  //temperature

            rx_d_sensor1[25] = rx_d56[43]; //cds
            rx_d_sensor1[26] = rx_d56[44]; //cds

            rx_d_sensor2[7] = rx_d56[25]; //acc x
            rx_d_sensor2[8] = rx_d56[26]; //acc x
            rx_d_sensor2[9] = rx_d56[27]; //acc y
            rx_d_sensor2[10] = rx_d56[28]; //acc y
            rx_d_sensor2[11] = rx_d56[29]; //acc z
            rx_d_sensor2[12] = rx_d56[30]; //acc z

            rx_d_sensor2[13] = rx_d56[31]; //mag x
            rx_d_sensor2[14] = rx_d56[32]; //mag x
            rx_d_sensor2[15] = rx_d56[33]; //mag y
            rx_d_sensor2[16] = rx_d56[34]; //mag y
            rx_d_sensor2[17] = rx_d56[35]; //mag z
            rx_d_sensor2[18] = rx_d56[36]; //mag z

            rx_d_sensor2[19] = rx_d56[45]; //steering var
            rx_d_sensor2[20] = rx_d56[46]; //steering var
            rx_d_sensor2[21] = rx_d56[23]; //steering torque
            rx_d_sensor2[22] = rx_d56[24]; //steering torque
            rx_d_sensor2[23] = rx_d56[47]; //battery
            rx_d_sensor2[24] = rx_d56[48]; //battery
            rx_d_sensor2[25] = rx_d56[51]; //remote control
            rx_d_sensor2[26] = rx_d56[52]; //M temperature

            rx_d_sensor2[27] = rx_d56[37]; //gyro sensor x
            rx_d_sensor2[28] = rx_d56[38]; //gyro sensor x

            rx_d_sensor2[29] = rx_d56[39]; //gyro sensor y
            rx_d_sensor2[30] = rx_d56[40]; //gyro sensor y

            rx_d_sensor2[5] = rx_d56[41]; //gyro sensor x
            rx_d_sensor2[6] = rx_d56[42]; //gyro sensor y

		}
	}
}

SensorData Sensor(int command)
{

	String sCommand;
	SensorData sensordata1;
	long i,j;

	Sendbuf[4] = (byte)command;

	SendData(Sendbuf);

	if(Sendbuf[4]==10)
	{

		while(1) {
			if(Serial.available()>0){
				rx_data[rxcnt]=Serial.read();
				rxcnt++;

			}
			else {
				break;
			}
		}

		for(i=0; i<rxcnt; i++) {
			for(j=0; j<55; j++) {
				rx_d56[j]=rx_d56[j+1];
			}
			rx_d56[55]=rx_data[i];
			if(rx_d56[0]==0x02 && rx_d56[55]==0x03 && rx_d56[1]==56)
			{
				chekdata56();
				//Sound(37);
				for(j=0; j<56; j++){
					rx_d56[j]=0;
				}
			}
		}
		rxcnt=0;

		Serial.flush();
	}
	else if(Sendbuf[4]==1 || Sendbuf[4]==2)
	{

		while(1) {
			if(Serial.available()>0){
				rx_data[rxcnt]=Serial.read();
				rxcnt++;
			}
			else {
				break;
			}
		}

		for(i=0; i<rxcnt; i++) {
			for(j=0; j<30; j++) {
				rx_d[j]=rx_d[j+1];
			}
			rx_d[30]=rx_data[i];
			if(rx_d[0]==0x02 && rx_d[30]==0x03 && rx_d[1]==31)
			{
				chekdata();

				for(j=0; j<31; j++){
					rx_d[j]=0;
				}
			}
		}
		rxcnt=0;
		Serial.flush();
	}


	sensordata1.IRSensor[0] = rx_d_sensor1[7] * 256 + rx_d_sensor1[8];
	sensordata1.IRSensor[1] = rx_d_sensor1[9] * 256 + rx_d_sensor1[10];
	sensordata1.IRSensor[2] = rx_d_sensor1[11] * 256 + rx_d_sensor1[12];
	sensordata1.IRSensor[3] = rx_d_sensor1[13] * 256 + rx_d_sensor1[14];
	sensordata1.IRSensor[4] = rx_d_sensor1[15] * 256 + rx_d_sensor1[16];
	sensordata1.IRSensor[5] = rx_d_sensor1[17] * 256 + rx_d_sensor1[18];

	sensordata1.TorqueSensor[0] = rx_d_sensor1[19] * 256 + rx_d_sensor1[20];
	sensordata1.TorqueSensor[1] = rx_d_sensor1[21] * 256 + rx_d_sensor1[22];
	sensordata1.TemperatureSensor = rx_d_sensor1[23] * 256 + rx_d_sensor1[24];
	sensordata1.CDSSensor = rx_d_sensor1[25] * 256 + rx_d_sensor1[26];

	sensordata1.GSensor[0] = (long)rx_d_sensor2[7] * 256 + (long)rx_d_sensor2[8];
	sensordata1.GSensor[1] = (long)rx_d_sensor2[9] * 256 + (long)rx_d_sensor2[10];
	sensordata1.GSensor[2] = (long)rx_d_sensor2[11] * 256 + (long)rx_d_sensor2[12];

	long temp=0;

	temp=sensordata1.GSensor[0]/16;
	sensordata1.GSensor[0]=temp;
	if(temp>2048) sensordata1.GSensor[0]=sensordata1.GSensor[0]-4096;

	temp=sensordata1.GSensor[1]/16;
	sensordata1.GSensor[1]=temp;
	if(temp>2048) sensordata1.GSensor[1]=sensordata1.GSensor[1]-4096;

	temp=sensordata1.GSensor[2]/16;
	sensordata1.GSensor[2]=temp;
	if(temp>2048) sensordata1.GSensor[2]=sensordata1.GSensor[2]-4096;


	sensordata1.MSensor[0] = (long)rx_d_sensor2[13] * 256 + (long)rx_d_sensor2[14];
	sensordata1.MSensor[1] = (long)rx_d_sensor2[15] * 256 + (long)rx_d_sensor2[16];
	sensordata1.MSensor[2] = (long)rx_d_sensor2[17] * 256 + (long)rx_d_sensor2[18];

	sensordata1.SteeringVar = rx_d_sensor2[19] * 256 + rx_d_sensor2[20];
	sensordata1.SteeringTorque = rx_d_sensor2[21] * 256 + rx_d_sensor2[22];
	sensordata1.Battery = rx_d_sensor2[23] * 256 + rx_d_sensor2[24];

	sensordata1.GySensor[0] = (long)rx_d_sensor2[27]*256 +(long)rx_d_sensor2[28];
	sensordata1.GySensor[1] = (long)rx_d_sensor2[29] * 256 + (long)rx_d_sensor2[30];
	sensordata1.GySensor[2] = (long)rx_d_sensor2[5] * 256 + (long)rx_d_sensor2[6];

	if(sensordata1.GSensor[0]>32768)
        sensordata1.GSensor[0]=sensordata1.GSensor[0]-65535;

    if(sensordata1.GSensor[1]>32768)
        sensordata1.GSensor[1]=sensordata1.GSensor[1]-65535;

	if(sensordata1.GSensor[2]>32768)
        sensordata1.GSensor[2]=sensordata1.GSensor[2]-65535;

    if(sensordata1.MSensor[0]>32768)
        sensordata1.MSensor[0]=sensordata1.MSensor[0]-65535;

    if(sensordata1.MSensor[1]>32768)
        sensordata1.MSensor[1]=sensordata1.MSensor[1]-65535;

    if(sensordata1.MSensor[2]>32768)
        sensordata1.MSensor[2]=sensordata1.MSensor[2]-65535;


    if(sensordata1.GySensor[0]>32768)
        sensordata1.GySensor[0]=sensordata1.GySensor[0]-65535;

    if(sensordata1.GySensor[1]>32768)
        sensordata1.GySensor[1]=sensordata1.GySensor[1]-65535;

	if(sensordata1.GySensor[2]>32768)
        sensordata1.GySensor[2]=sensordata1.GySensor[2]-65535;

	return sensordata1;
}
