
class SensorData {
    public:
	long IRSensor[6];
	long TorqueSensor[2];
	long TemperatureSensor;
	long CDSSensor;
	long GSensor[3];
	long MSensor[3];
	long GySensor[3];
	long SteeringVar;
	long SteeringTorque;
	long Battery;
};

void Go(int left, int right);
void Steering(int steeringvalue);
void Steering2(int value1, int value2);
void Steering3(int value1);
void Sound(unsigned char buzzer);
void Display(unsigned char ASCII);
void DisplayLine(unsigned char dot0, unsigned char dot1, unsigned char dot2, unsigned char dot3, unsigned char dot4, unsigned char dot5, unsigned char dot6, unsigned char dot7);
void Led(long led);
void Light(long led);
void Displayon(int x, int y);
void Displayoff(int x, int y);
SensorData Sensor(int command);

