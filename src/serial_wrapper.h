/*
 * serial_wrapper.h
 *
 *  Created on: 21 févr. 2017
 *      Author: theo
 */

#ifndef SRC_SERIAL_WRAPPER_H_
#define SRC_SERIAL_WRAPPER_H_

#include "banc.h"
#include "Arduino.h"

class Serial_wrapper
{

private:
	serial_port used_port;

public:
	Serial_wrapper();

	serial_port getPort();
	void setPort(serial_port port);
	void begin(int baudrate);
	int available();
	size_t readBytes( char *buffer, size_t length); // read chars from stream into buffer
	size_t readBytes( uint8_t *buffer, size_t length) { return readBytes((char *)buffer, length); }
	size_t println(const char c[]);
	size_t print(const char str[]);
};

extern Serial_wrapper Serials;

#endif /* SRC_SERIAL_WRAPPER_H_ */
