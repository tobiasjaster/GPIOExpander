/* mbed Microcontroller Library
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
#include "ExpDigitalOut.h"

#if DEVICE_EXPANSION

ExpDigitalOut::ExpDigitalOut(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment();
	}
}

ExpDigitalOut::ExpDigitalOut(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin, int value) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment();
		write(value);
	}
}

ExpDigitalOut::~ExpDigitalOut() {
	if (_isConnected && _resetAttachment()){
		_setDirection(OUTPUT);
	}
}

void ExpDigitalOut::write(int value) {
	int value_new;
	int value_old;
	int pinposition = 1 << (char)_pin;
	int pinvalue = value << (char)_pin;
	_exp->read(_port, &value_old);
	value_new = (value_old & ~pinposition) | pinvalue;
	if (value_new != value_old){
		_exp->write(_port, value_new);
	}
}

int ExpDigitalOut::read(void) {
	int value;
	int pinposition = 1 << (char)_pin;
	_exp->read(_port, &value);
	return (int)((value&pinposition) >> (char)_pin);
}

int ExpDigitalOut::is_connected() {
	return (int)_isConnected;
}

ExpDigitalOut &ExpDigitalOut::operator= (int value) {
    write(value);
    return *this;
}

ExpDigitalOut &ExpDigitalOut::operator= (ExpDigitalOut &rhs) {
    write(rhs.read());
    return *this;
}

ExpDigitalOut::operator int()
{
    // Underlying call is atomic
    return read();
}

bool ExpDigitalOut::_checkAttachment(void) {
	return _exp->isAttached(_port, _pin);
}

bool ExpDigitalOut::_setAttachment(void){
	_exp->attach(_port, _pin, NULL, 0);
	return true;
}

bool ExpDigitalOut::_resetAttachment(void){
	_exp->detach(_port, _pin);
	return true;
}

bool ExpDigitalOut::_setDirection(ExpDigitalDirection direction){
	write(0);
	int direction_new;
	int direction_old;
	int pinposition = 1 << (char)_pin;
	int pinvalue = (int)direction << (char)_pin;
	_exp->getDirection(_port, &direction_old);
	direction_new = (direction_old & ~pinposition) | pinvalue;
	if (direction_new != direction_old){
		_exp->setDirection(_port, direction_new);
	}
	_exp->getDirection(_port, &direction_old);
	return (direction_new != direction_old) ? false : true;
}

#endif
