/*
  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted, provided that the
  above copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
  SOFTWARE.
 */

#ifndef WebUSB_h
#define WebUSB_h

#include <stdint.h>
#include <Arduino.h>
#include "PluggableUSB.h"
#include <avr/wdt.h>
#include <util/atomic.h>

#define USB_BOS_DESCRIPTOR_TYPE		15
#define WEBUSB_REQUEST_GET_ALLOWED_ORIGINS		0x01
#define WEBUSB_REQUEST_GET_URL			0x02

typedef struct 
{
	InterfaceDescriptor dif;
	EndpointDescriptor  in;
	EndpointDescriptor  out;
} WebUSBDescriptor;

class WebUSB : public PluggableUSBModule, public Stream
{
public:
	WebUSB(void);
	void begin(unsigned long);
	void begin(unsigned long, uint8_t);
	void end(void);

	virtual int available(void);
	virtual int peek(void);
	virtual int read(void);
	int availableForWrite(void);
	virtual void flush(void);
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t*, size_t);
	using Print::write; // pull in write(str) and write(buf, size) from Print
	operator bool();

	volatile uint8_t _rx_buffer_head;
	volatile uint8_t _rx_buffer_tail;
	unsigned char _rx_buffer[SERIAL_BUFFER_SIZE];

	int32_t readBreak();

	uint32_t baud();
	uint8_t stopbits();
	uint8_t paritytype();
	uint8_t numbits();
	bool dtr();
	bool rts();
	enum {
		ONE_STOP_BIT = 0,
		ONE_AND_HALF_STOP_BIT = 1,
		TWO_STOP_BITS = 2,
	};
	enum {
		NO_PARITY = 0,
		ODD_PARITY = 1,
		EVEN_PARITY = 2,
		MARK_PARITY = 3,
		SPACE_PARITY = 4,
	};

protected:
	// Implementation of the PluggableUSBModule
	int getInterface(uint8_t* interfaceCount);
	int getDescriptor(USBSetup& setup);
	bool setup(USBSetup& setup);
	uint8_t getShortName(char* name);

private:
	bool VendorControlRequest(USBSetup& setup);

	uint8_t epType[2];
	uint16_t descriptorSize;
	uint8_t protocol;
	uint8_t idle;
	int peek_buffer;
};

extern WebUSB WebSerial;

#endif // WebUSB_h
