/*
 * serial_wrapper.cpp
 *
 *  Created on: 21 févr. 2017
 *      Author: theo
 */


#include "serial_wrapper.h"


Serial_wrapper Serials;

Serial_wrapper::Serial_wrapper():used_port(NONE)
{

}

serial_port Serial_wrapper::getPort()
{
	return used_port;
}
void Serial_wrapper::setPort(serial_port port)
{
	used_port = port;
}

int Serial_wrapper::available()
{
	if(used_port == USB) return Serial.available();
	else if(used_port == HC05) return Serial1.available();
	else return 0;
}


size_t Serial_wrapper::readBytes( char *buffer, size_t length)
{
	if(used_port == USB) return Serial.readBytes(buffer, length);
	else if(used_port == HC05) return Serial1.readBytes(buffer, length);
	else return 0;

}

void Serial_wrapper::begin(unsigned long baudrate)
{
	Serial1.begin(baudrate);
	Serial.begin(baudrate);
}

size_t Serial_wrapper::println(const char c[])
{
	if(used_port == USB) return Serial.println(c);
	else if(used_port == HC05) return Serial1.println(c);
	else return 0;
}

size_t Serial_wrapper::print(const char str[])
{
	if(used_port == USB) return Serial.print(str);
	else if(used_port == HC05) return Serial1.print(str);
	else return 0;
}

