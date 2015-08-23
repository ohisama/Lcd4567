#include <Lcd4567.h>

Lcd4567 lcd;

void setup()
{
	pinMode(13, OUTPUT);
	lcd = Lcd4567();
	lcd.begin(20, 4);
	lcd.print("hello, world!");
}
void loop()
{
	digitalWrite(13, HIGH);
	delay(1000);
	digitalWrite(13, LOW);
	delay(1000);
}
