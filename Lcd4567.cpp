#include "Lcd4567.h"

Lcd4567::Lcd4567(void)
{
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	_rs_pin = 12;
	_en_pin = 11;
	_data_pins[0] = 5;
	_data_pins[1] = 4;
	_data_pins[2] = 3;
	_data_pins[3] = 2;
}
void Lcd4567::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
	pinMode(_rs_pin, OUTPUT);
	pinMode(_en_pin, OUTPUT);
	pinMode(_data_pins[0], OUTPUT);
	pinMode(_data_pins[1], OUTPUT);
	pinMode(_data_pins[2], OUTPUT);
	pinMode(_data_pins[3], OUTPUT);
	if (lines > 1)
	{
		_displayfunction |= LCD_2LINE;
	}
	_numlines = lines;
	_currline = 0;
	if ((dotsize != 0) && (lines == 1))
	{
		_displayfunction |= LCD_5x10DOTS;
	}
	delayMicroseconds(4500);
	digitalWrite(_rs_pin, LOW);
	digitalWrite(_data_pins[3], 0x00);
	digitalWrite(_data_pins[2], 0x00);
	digitalWrite(_data_pins[1], 0x01);
	digitalWrite(_data_pins[0], 0x01);
	pulse();
	delayMicroseconds(4500);
	pulse();
	delayMicroseconds(150);
	digitalWrite(_data_pins[0], 0x00);
	pulse();
	command(LCD_FUNCTIONSET | _displayfunction);
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	display();
	clear();
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}
void Lcd4567::pulse(void)
{
	digitalWrite(_en_pin, LOW);
	delayMicroseconds(150);
	digitalWrite(_en_pin, HIGH);
	delayMicroseconds(150);
	digitalWrite(_en_pin, LOW);
	delayMicroseconds(100);
}
void Lcd4567::send(uint8_t value, uint8_t mode)
{
	if (mode)
	{
		digitalWrite(_rs_pin, HIGH);
		digitalWrite(_data_pins[3], (value >> 7) & 0x01);
		digitalWrite(_data_pins[2], (value >> 6) & 0x01);
		digitalWrite(_data_pins[1], (value >> 5) & 0x01);
		digitalWrite(_data_pins[0], (value >> 4) & 0x01);
		pulse();
		digitalWrite(_data_pins[3], (value >> 3) & 0x01);
		digitalWrite(_data_pins[2], (value >> 2) & 0x01);
		digitalWrite(_data_pins[1], (value >> 1) & 0x01);
		digitalWrite(_data_pins[0], (value >> 0) & 0x01);
		pulse();
	}
	else
	{
		digitalWrite(_rs_pin, LOW);
		digitalWrite(_data_pins[3], (value >> 7) & 0x01);
		digitalWrite(_data_pins[2], (value >> 6) & 0x01);
		digitalWrite(_data_pins[1], (value >> 5) & 0x01);
		digitalWrite(_data_pins[0], (value >> 4) & 0x01);
		pulse();
		digitalWrite(_data_pins[3], (value >> 3) & 0x01);
		digitalWrite(_data_pins[2], (value >> 2) & 0x01);
		digitalWrite(_data_pins[1], (value >> 1) & 0x01);
		digitalWrite(_data_pins[0], (value >> 0) & 0x01);
		pulse();
	}
}
inline void Lcd4567::command(uint8_t value)
{
	send(value, LOW);
}
inline size_t Lcd4567::write(uint8_t value)
{
	send(value, HIGH);
	return 1;
}
void Lcd4567::clear()
{
	command(LCD_CLEARDISPLAY);
	delayMicroseconds(2000);
}
void Lcd4567::home()
{
	command(LCD_RETURNHOME);
	delayMicroseconds(2000);
}
void Lcd4567::setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = {
		0x00,
		0x40,
		0x14,
		0x54
	};
	if (row > _numlines)
	{
		row = _numlines - 1;
	}
	command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
void Lcd4567::noDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::display()
{
	_displaycontrol |= LCD_DISPLAYON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::cursor()
{
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::blink()
{
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Lcd4567::scrollDisplayLeft(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Lcd4567::scrollDisplayRight(void)
{
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
void Lcd4567::leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}
void Lcd4567::rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}
void Lcd4567::autoscroll(void)
{
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}
void Lcd4567::noAutoscroll(void)
{
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}
void Lcd4567::createChar(uint8_t location, uint8_t charmap[])
{
	location &= 0x7;
	command(LCD_SETCGRAMADDR | (location << 3));
	for (int i = 0; i < 8; i++)
	{
		write(charmap[i]);
	}
}

