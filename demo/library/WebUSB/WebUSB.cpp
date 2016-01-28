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

#include "WebUSB.h"

WebUSB WebSerial;

const uint8_t BOS_DESCRIPTOR[30] PROGMEM = {
// BOS descriptor header.
0x05, 0x0F, 0x1D, 0x00, 0x01,

// WebUSB Platform Capability descriptor (bVendorCode == 0x01).
0x18, 0x10, 0x05, 0x00, 0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,
0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65, 0x00, 0x01, 0x01, 0x01
};

const uint8_t WEBUSB_ALLOWED_ORIGINS[7] PROGMEM = {
0x07, 0x00, 0x07, 0x00, 0x00, 0x01, 0x02
};

// This descriptor is also used as the landing page which is why it is more than
// just an origin.
const uint8_t WEBUSB_ORIGIN_1[30] PROGMEM = {
0x1e, 0x03, 0x01, 'w', 'i', 'c', 'g', '.', 'g', 'i', 't', 'h', 'u', 'b', '.',
'i', 'o', '/', 'w', 'e', 'b', 'u', 's', 'b', '/', 'd', 'e', 'm', 'o', '/'
};

const uint8_t WEBUSB_ORIGIN_2[17] PROGMEM = {
0x11, 0x03, 0x00, 'l', 'o', 'c', 'a', 'l', 'h', 'o', 's', 't', ':', '8', '0',
'0', '0'
};

typedef struct
{
	u32	dwDTERate;
	u8	bCharFormat;
	u8 	bParityType;
	u8 	bDataBits;
	u8	lineState;
} LineInfo;

static volatile LineInfo _usbLineInfo = { 57600, 0x00, 0x00, 0x00, 0x00 };
static volatile int32_t breakValue = -1;

int WebUSB::getInterface(uint8_t* interfaceCount)
{
	*interfaceCount += 1; // uses 1 interface
	WebUSBDescriptor webUSBInterface = {
		D_INTERFACE(pluggedInterface, 2, 0xff, 0, 0),
		D_ENDPOINT(USB_ENDPOINT_OUT(pluggedEndpoint),USB_ENDPOINT_TYPE_BULK,0x40,0),
		D_ENDPOINT(USB_ENDPOINT_IN (pluggedEndpoint+1 ),USB_ENDPOINT_TYPE_BULK,0x40,0)
	};
	return USB_SendControl(0, &webUSBInterface, sizeof(webUSBInterface));
}

int WebUSB::getDescriptor(USBSetup& setup)
{
	if (USB_BOS_DESCRIPTOR_TYPE == setup.wValueH)
	{
		if (setup.wValueL == 0 && setup.wIndex == 0) {
			return USB_SendControl(TRANSFER_PGM, &BOS_DESCRIPTOR, sizeof(BOS_DESCRIPTOR));
		}
	}
	return 0;
}

uint8_t WebUSB::getShortName(char* name)
{
	memcpy(name, "WUART", 5);
	return 5;
}

bool WebUSB::VendorControlRequest(USBSetup& setup)
{
	if (setup.bmRequestType == (REQUEST_DEVICETOHOST | REQUEST_VENDOR | REQUEST_DEVICE) &&
		  setup.bRequest == 0x01) {
    if (setup.wIndex == WEBUSB_REQUEST_GET_ALLOWED_ORIGINS)
		{
			USB_SendControl(TRANSFER_PGM, &WEBUSB_ALLOWED_ORIGINS, sizeof(WEBUSB_ALLOWED_ORIGINS));
			return true;
		}
		else if (setup.wIndex == WEBUSB_REQUEST_GET_URL)
		{
			if (setup.wValueL == 1)
			{
				USB_SendControl(TRANSFER_PGM, &WEBUSB_ORIGIN_1, sizeof(WEBUSB_ORIGIN_1));
				return true;
			}
			else if (setup.wValueL == 2)
			{
				USB_SendControl(TRANSFER_PGM, &WEBUSB_ORIGIN_2, sizeof(WEBUSB_ORIGIN_2));
				return true;
			}
		}
	}
	return false;
}


bool WebUSB::setup(USBSetup& setup)
{
	uint8_t r = setup.bRequest;
	uint8_t requestType = setup.bmRequestType;

	if (REQUEST_CLASS == (requestType & REQUEST_TYPE) && (pluggedInterface == setup.wIndex)) {
		if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType)
		{
			if (CDC_GET_LINE_CODING == r)
			{
				USB_SendControl(0,(void*)&_usbLineInfo,7);
				return true;
			}
		}

		if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
		{
			if (CDC_SEND_BREAK == r)
			{
				breakValue = ((uint16_t)setup.wValueH << 8) | setup.wValueL;
			}

			if (CDC_SET_LINE_CODING == r)
			{
				USB_RecvControl((void*)&_usbLineInfo,7);
			}

			if (CDC_SET_CONTROL_LINE_STATE == r)
			{
				_usbLineInfo.lineState = setup.wValueL;
			}
			return true;
		}
	} else if (REQUEST_VENDOR == (requestType & REQUEST_TYPE)) {
		return VendorControlRequest(setup);
	}
	return false;
}

WebUSB::WebUSB(void) : PluggableUSBModule(2, 1, epType)
{
	// one interface, 2 endpoints
	epType[0] = EP_TYPE_BULK_OUT;
	epType[1] = EP_TYPE_BULK_IN;
	PluggableUSB().plug(this);
}

void WebUSB::begin(unsigned long /* baud_count */)
{
	peek_buffer = -1;
}

void WebUSB::begin(unsigned long /* baud_count */, byte /* config */)
{
	peek_buffer = -1;
}

void WebUSB::end(void)
{
}

int WebUSB::available(void)
{
	if (peek_buffer >= 0) {
		return 1 + USB_Available(pluggedEndpoint);
	}
	return USB_Available(pluggedEndpoint);
}

int WebUSB::peek(void)
{
	if (peek_buffer < 0)
		peek_buffer = USB_Recv(pluggedEndpoint);
	return peek_buffer;
}

int WebUSB::read(void)
{
	if (peek_buffer >= 0) {
		int c = peek_buffer;
		peek_buffer = -1;
		return c;
	}
	return USB_Recv(pluggedEndpoint);
}

int WebUSB::availableForWrite(void)
{
	return USB_SendSpace(pluggedEndpoint+1);
}

void WebUSB::flush(void)
{
	USB_Flush(pluggedEndpoint+1);
}

size_t WebUSB::write(uint8_t c)
{
	return write(&c, 1);
}

size_t WebUSB::write(const uint8_t *buffer, size_t size)
{
	/* only try to send bytes if the high-level CDC connection itself 
	 is open (not just the pipe) - the OS should set lineState when the port
	 is opened and clear lineState when the port is closed.
	 bytes sent before the user opens the connection or after
	 the connection is closed are lost - just like with a UART. */
	
	// TODO - ZE - check behavior on different OSes and test what happens if an
	// open connection isn't broken cleanly (cable is yanked out, host dies
	// or locks up, or host virtual serial port hangs)
	if (_usbLineInfo.lineState > 0)	{
		int r = USB_Send(pluggedEndpoint+1,buffer,size);
		if (r > 0) {
			return r;
		} else {
			setWriteError();
			return 0;
		}
	}
	setWriteError();
	return 0;
}

// This operator is a convenient way for a sketch to check whether the
// port has actually been configured and opened by the host (as opposed
// to just being connected to the host).  It can be used, for example, in 
// setup() before printing to ensure that an application on the host is
// actually ready to receive and display the data.
// We add a short delay before returning to fix a bug observed by Federico
// where the port is configured (lineState != 0) but not quite opened.
WebUSB::operator bool() {
	bool result = false;
	if (_usbLineInfo.lineState > 0) 
		result = true;
	delay(10);
	return result;
}

unsigned long WebUSB::baud() {
	// Disable interrupts while reading a multi-byte value
	uint32_t baudrate;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		baudrate =  _usbLineInfo.dwDTERate;
	}
	return baudrate;
}

uint8_t WebUSB::stopbits() {
	return _usbLineInfo.bCharFormat;
}

uint8_t WebUSB::paritytype() {
	return _usbLineInfo.bParityType;
}

uint8_t WebUSB::numbits() {
	return _usbLineInfo.bDataBits;
}

bool WebUSB::dtr() {
	return _usbLineInfo.lineState & 0x1;
}

bool WebUSB::rts() {
	return _usbLineInfo.lineState & 0x2;
}

int32_t WebUSB::readBreak() {
	int32_t ret;
	// Disable IRQs while reading and clearing breakValue to make
	// sure we don't overwrite a value just set by the ISR.
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		ret = breakValue;
		breakValue = -1;
	}
	return ret;
}
