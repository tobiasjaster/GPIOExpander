/* MCP23S17 - drive the Microchip MCP23S17 16-bit Port Extender using SPI
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed.h"
#include "mbed_error.h"
#include "MCP23S17.h"

MCP23S17::MCP23S17(PinName mosi,
		PinName miso,
		PinName sclk,
		PinName cs,
		char writeOpcode,
		PinName interrupt,
		ExpPortWidth pw) : _cs(cs), _int(interrupt) {
	_config.id = 0;
	_config.portWidth = pw;
	_config.opcode = writeOpcode;
	_config.interrupt = interrupt;
	_spi = new mbed::SPI(mosi, miso, sclk);
	_spi->frequency(400000);
	_spi->format(8,0);
//	_writeOpcode = writeOpcode;
//    _readOpcode = _writeOpcode | 1; // low order bit = 1 for read
    _init();
}

MCP23S17::MCP23S17(mbed::SPI* spi,
		PinName cs,
		char writeOpcode,
		PinName interrupt,
		ExpPortWidth pw) : _cs(cs), _int(interrupt) {
	_config.id = 0;
	_config.portWidth = pw;
	_config.opcode = writeOpcode;
	_config.interrupt = interrupt;
	_spi = spi;
	_spi->frequency(400000);
	_spi->format(8,0);
//    _writeOpcode = writeOpcode;
//    _readOpcode = _writeOpcode | 1; // low order bit = 1 for read
    _init();
}

MCP23S17::~MCP23S17(void) {
	_deinit();
}

void MCP23S17::_init(void) {
	_config.intConfigured = true;
	if (_config.interrupt!=-1){
	    _write(IOCON, (IOCON_BYTE_MODE | IOCON_MIRROR | IOCON_INT));
//	    _intA.rise(callback(this, &MCP23S17::interruptControl));
	}
	else{
	    _write(IOCON, (IOCON_BYTE_MODE));
	    _config.intConfigured = false;
	}
	_enableCallback(true);
	_interrupt = false;
	_threadrunning = true;
	_intThread.start(callback(this, &MCP23S17::_threadControl));
 // Hardware addressing on, operations toggle between A and B registers
}

void MCP23S17::_deinit(void) {
	setDirection(EXPPORT_A, 0xFF);
	setDirection(EXPPORT_B, 0xFF);
	_enableCallback(false);
	if (_config.intConfigured == true){
		setInterruptEnable(EXPPORT_A, 0, 0, 0);
		setInterruptEnable(EXPPORT_B, 0, 0, 0);
	}
	_threadrunning = false;
	_intThread.join();
}

void MCP23S17::_write(char address, char data) {
    _mutex.lock();
	_cs = 0;
    _spi->write(_config.opcode);
    _spi->write(address);
    _spi->write(data);
    _cs = 1;
    _mutex.unlock();
}

char MCP23S17::_read(char address) {
    _mutex.lock();
	_cs = 0;
    _spi->write(_config.opcode | 1);
    _spi->write(address);
    char result = _spi->write(0);
    _cs = 1;
    _mutex.unlock();
    return result;
}

void MCP23S17::_enableCallback(bool state) {
	if (_config.interrupt!=-1){
		if (state) {
		    _int.rise(callback(this, &MCP23S17::interruptControl));
		}
		else{
		    _int.rise(NULL);
		}
	}
}

void MCP23S17::_threadControl(void) {
	while(_threadrunning){
		if (_interrupt){
			_interruptControl();
			_interrupt = false;
		}
		if (_config.interrupt!=-1 && _int){
			_read(GPIO | 0);
			_read(GPIO | 1);
		}
	}
}

void MCP23S17::_interruptControl(void) {
	char intMask = 0;
	char gpio = 0;
	int position = 0;
	gpio_irq_event event;
	std::list<expGPIO_t>::iterator it;
	if (_config.intConfigured == true) {
		for(int i = EXPPORT_A; i <= EXPPORT_B; i++) {
			intMask = _read(INTF | i);
			if (intMask != 0){
				for(int j = 0; j < _config.portWidth; j++){
					if (intMask >> j & 0x01){
						position = j;
						break;
					}
				}
				gpio = _read(GPIO | i);
				if ((gpio&intMask) != 0) {
					event = IRQ_RISE;
				}else{
					event = IRQ_FALL;
				}
				if (position != 0){
					it = _getExpGPIO((ExpPortName)i, (ExpPinName)(position));
					if (it != _intList.end() && it->func){
						it->func(it->id,event);
					}
//					_intArray[i<<4 | intMask].func();
					break;
				}
			}
		}
	}
}

std::list<expGPIO_t>::iterator MCP23S17::_getExpGPIO(ExpPortName port, ExpPinName pin) {
	std::list<expGPIO_t>::iterator it;
	if (!_intList.empty()) {
		for(it = _intList.begin(); it != _intList.end(); ++it){
			if (it->pin == pin && it->port == port){
				break;
			}
		}
	}
	return it;
}
ExpPortWidth MCP23S17::getPortWidth(void){
	return _config.portWidth;
}

ExpError MCP23S17::getDirection(ExpPortName port, int *data) {
	*data = (int)_read(IODIR | (char)port);
	return ExpError_OK;
}

ExpError MCP23S17::setDirection(ExpPortName port, int directionMask) {
	if (directionMask > 0xFF) {
		return ExpError_MASK;
	}
    _write(IODIR | (char)port, (char)directionMask);
	return ExpError_OK;
}

ExpError MCP23S17::getConfigureMode(ExpPortName port, PinMode mode, int *data) {
	if (mode != PullUp) {
		return ExpError_MASK;
	}
	*data = (int)_read(GPPU | (char)port);
	return ExpError_OK;
}

ExpError MCP23S17::setConfigureMode(ExpPortName port, PinMode mode, int pullupMask) {
	if (pullupMask > 0xFF || mode != PullUp) {
		return ExpError_MASK;
	}
    _write(GPPU | (char)port, (char)pullupMask);
	return ExpError_OK;
}

ExpError MCP23S17::getInterruptEnable(ExpPortName port, int *data) {
	_enableCallback(false);
	*(data+0) = (int)_read(INTCON | (char)port);
	*(data+1) = (int)_read(DEFVAL | (char)port);
	*(data+2) = (int)_read(GPINTEN | (char)port);
    _enableCallback(true);
	return ExpError_OK;
}
ExpError MCP23S17::setInterruptEnable(ExpPortName port, int interruptsEnabledMask, int risingEdgeMask, int fallingEdgeMask) {
	if (interruptsEnabledMask > 0xFF || risingEdgeMask > 0xFF || fallingEdgeMask > 0xFF) {
		return ExpError_MASK;
	}
	_enableCallback(false);
	_write(INTCON | (char)port, (char)risingEdgeMask^(char)fallingEdgeMask);
    _write(DEFVAL | (char)port, (char)fallingEdgeMask);
    _write(GPINTEN | (char)port, (char)interruptsEnabledMask);
    _enableCallback(true);
	return ExpError_OK;
}

void MCP23S17::interruptControl(void) {
	_interrupt = true;
}

ExpError MCP23S17::read(ExpPortName port, int *data) {
	*data = (int)_read(GPIO | (char)port);
	return ExpError_OK;
}

ExpError MCP23S17::write(ExpPortName port, int data) {
	if (data > 0xFF) {
		return ExpError_MASK;
	}
    _write(OLAT | (char)port, (char)data);
	return ExpError_OK;
}

ExpError MCP23S17::attach(ExpPortName port, ExpPinName pin, Callback<void(uint32_t,gpio_irq_event)> func, uint32_t id) {
    if (!((pin <= EXPPIN_7) && (port == EXPPORT_A || port == EXPPORT_B))) {
//    	MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT), "Buffer pointer is Null" );
    	return ExpError_MASK;
    }
    std::list<expGPIO_t>::iterator it = _getExpGPIO(port, pin);
    if (it !=  _intList.end()){
    	it->id = id;
    	it->func = func;
    }
    else{
    	expGPIO_t temp;
    	temp.id = id;
    	temp.port = port;
    	temp.pin = pin;
    	temp.func = func;
    	_intList.push_back(temp);
    }
	return ExpError_OK;
}

bool MCP23S17::isAttached(ExpPortName port, ExpPinName pin) {
    if (!((pin <= EXPPIN_7) && (port == EXPPORT_A || port == EXPPORT_B))) {
    	MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT), "Buffer pointer is Null" );
//    	return ExpError_MASK;
    }
    std::list<expGPIO_t>::iterator it = _getExpGPIO(port, pin);
    if (it != _intList.end()){
    	return true;
    }
    return false;
}

//bool MCP23S17::isAttached(ExpPortName port) {
//	char pin;
//	expGPIO_t temp;
//	for(pin = 0; pin < 8; pin++){
//		temp = _getExpGPIO((ExpPinName)(pin | (port << 4)));
//		if (temp.pin != EXP_NP) {
//			return true;
//		}
//	}
//    return false;
//}

ExpError MCP23S17::detach(ExpPortName port, ExpPinName pin) {
    if (!((pin <= EXPPIN_7) && (port == EXPPORT_A || port == EXPPORT_B))) {
//    	MBED_ERROR( MBED_MAKE_ERROR(MBED_MODULE_APPLICATION, MBED_ERROR_CODE_INVALID_ARGUMENT), "Buffer pointer is Null" );
    	return ExpError_MASK;
    }
    if (isAttached(port, pin)){
    	std::list<expGPIO_t>::iterator it;
    	for(it = _intList.begin(); it != _intList.end(); ++it){
    		if (it->pin == pin && it->port == port){
    			it = _intList.erase(it);
    			break;
    		}
    	}
        return ExpError_OK;
    }
	return ExpError_NOTINITIALIZED;
}
