#define swPin 5 //��� ��������� ���������
#define rxPin 2 //��� ���������

void grab();

void setup()
{
	pinMode(rxPin, INPUT);
	// ������ ������������� ��� ��������� ��������� � 0 � �� ������� ��� ������
	pinMode(swPin, OUTPUT);
	digitalWrite(swPin, LOW);

	Serial.begin(9600);  // ��� ��������� ���� ��������
	Serial.println("Came started");

	attachInterrupt(0, grab, CHANGE); // �������� �������� �� ������ �������� - ���� � ��� ������ �������, �� ����� ���� �� 1, � 0!
	interrupts();

}

volatile static long code = 0;
long lastCode = 0;
int i, j;
long variant;
long someCode = 1234;
void SendCame4(long Code);

void loop()
{

}

// ���� ��� ���� - �����

#define MAX_DELTA 200
// ������������ ��������� ����� ������� � ����������� �� ������ �������� - ���� ������ 200 ���
boolean CheckValue(unsigned int base, unsigned int value)
{
	return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
volatile static byte cameCounter = 0;    // count of bits stored
volatile static long cameCode = 0;       // code itself

void grab()
{
	state = digitalRead(rxPin);
	if (state == HIGH)
		lolen = micros() - prevtime;
	else
		hilen = micros() - prevtime;
	prevtime = micros();

	if (state == LOW)
	{
		// ������������������ �����������
		if (CheckValue(320, hilen) && CheckValue(640, lolen)) // valid 1
		{
			cameCode = (cameCode << 1) | 1;
			cameCounter++;
		}
		else if (CheckValue(640, hilen) && CheckValue(320, lolen)) // valid 0
		{
			cameCode = (cameCode << 1) | 0;
			cameCounter++;
		}
		else cameCounter = 0;
	}
	else
		if (lolen > 1000 &&
			(cameCounter == 12 || cameCounter == 13) &&
			((cameCode & 0xfff) != 0xfff))
		{
			code = cameCode & 0xfff;
			cameCounter = 0;
			cameCode = 0;
		}
	if (code != 0)
	{
		if (code == lastCode)
			Serial.print("*");
		Serial.println(code);
		lastCode = code;
		code = 0;
	}
}