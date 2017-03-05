#define txPin 3
#define button 13
//

void setup()
{
	pinMode(txPin, OUTPUT);
	pinMode(button, INPUT);

	Serial.begin(9600);
	Serial.println("Came started");
}

volatile static long lastCode = 0;
int i = 0, j = 0;
long variant;
long someCode = 1234;
void SendCame4(long Code);

//Brut
unsigned int startTime;
long BrutCode = 0;
void SendCameBitBrut(byte bBrut);
void SendCameBrut(long CodeBrut);

void loop() {

	if (digitalRead(button) == HIGH)
	{
		delay(500);
		if (digitalRead(button) == LOW)
		{
			SendCame4(someCode);
			Serial.println();
			Serial.print(someCode);
			Serial.println(" was transmitted");
			return;
		}
		/*else
		{
			delay(995);
			if (digitalRead(button) == LOW)
			{
				do
				{
					if (digitalRead(button) == HIGH)
						return;

					startTime = micros();
					SendCameBrut(BrutCode);
					BrutCode++;
					if (BrutCode == 4095)
					{
						BrutCode = 0;
						break;
					}
				} while (digitalRead(button) == LOW);
			}
		}*/
	}
}


void SendCame4(long Code) {
	for (int j = 0; j < 4; j++) {
		digitalWrite(txPin, HIGH);
		delayMicroseconds(320);
		digitalWrite(txPin, LOW);
		for (int i = 12; i > 0; i--) {
			byte b = bitRead(Code, i - 1);
			if (b) {
				digitalWrite(txPin, LOW); // 1
				delayMicroseconds(640);
				digitalWrite(txPin, HIGH);
				delayMicroseconds(320);
			}
			else {
				digitalWrite(txPin, LOW); // 0
				delayMicroseconds(320);
				digitalWrite(txPin, HIGH);
				delayMicroseconds(640);
			}
		}
		digitalWrite(txPin, LOW);
		delayMicroseconds(11520);
	}
}

void SendCameBitBrut(byte bBrut)
{
	delayMicroseconds(320);
	if (!bBrut) digitalWrite(txPin, HIGH);
	delayMicroseconds(320);
	digitalWrite(txPin, HIGH);
	delayMicroseconds(320);
	digitalWrite(txPin, LOW);
}

void SendCameBrut(long CodeBrut)
{
	Serial.println();
	Serial.print("***BRUT***        ");
	Serial.print(CodeBrut);
	for (int j = 0; j < 4; j++)
	{
		digitalWrite(txPin, HIGH);
		delayMicroseconds(320);
		digitalWrite(txPin, LOW);

		for (byte i = 12; i>0; i--)
		{
			SendCameBitBrut(bitRead(CodeBrut, i - 1));
		}
		delay(16);
	}
}
