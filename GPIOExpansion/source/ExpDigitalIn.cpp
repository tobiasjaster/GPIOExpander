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
#include "ExpDigitalIn.h"

#if DEVICE_EXPANSION

ExpDigitalIn::ExpDigitalIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment();
		_setMode(PullNone);
	}
}

ExpDigitalIn::ExpDigitalIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin, PinMode mode) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment();
		_setMode(mode);
	}
}

ExpDigitalIn::~ExpDigitalIn(){
	if (_isConnected && _resetAttachment()){
		_setMode(PullNone);
	}
}
int ExpDigitalIn::read(void) {
	int value;
	int pinposition = 1 << (char)_pin;
	_exp->read(_port, &value);
	return (int)((value&pinposition) >> (char)_pin);
}

void ExpDigitalIn::mode(PinMode pull) {
	if (_isConnected) {
		_setMode(pull);
	}
}

int ExpDigitalIn::is_connected() {
	return (int)_isConnected;
}


ExpDigitalIn::operator int()
{
    // Underlying call is atomic
    return read();
}

bool ExpDigitalIn::_checkAttachment(void) {
	return _exp->isAttached(_port, _pin);
}

bool ExpDigitalIn::_setAttachment(void){
	_exp->attach(_port, _pin, NULL, 0);
	return true;
}

bool ExpDigitalIn::_resetAttachment(void){
	_exp->detach(_port, _pin);
	return true;
}

bool ExpDigitalIn::_setDirection(ExpDigitalDirection direction){
	int pinposition = 1 << (char)_pin;
	_exp->write(_port,0);
	int direction_new;
	int direction_old;
	_exp->getDirection(_port, &direction_old);
	if (direction == INPUT){
		direction_new = direction_old | pinposition;
	}
	else{
		direction_new = direction_old & ~pinposition;
	}
	if (direction_new != direction_old){
		_exp->setDirection(_port, direction_new);
	}
	_exp->getDirection(_port, &direction_old);
	if (direction_new != direction_old){
		return false;
	}
	return true;
}

bool ExpDigitalIn::_setMode(PinMode mode){
//	if (mode != PullNone && mode !=PullUp){
//		return false;
//	}
	int pinmode_old;
	int pinmode_new;
	int pinposition = 0x01 << (char)_pin;
	int mask;
	_exp->getConfigureMode(_port, mode, &pinmode_old);
	mask = (mode != PullNone) ? 0xFFFFFFFF : 0;
	pinmode_new = (pinmode_old & ~pinposition) | (mask | pinposition);
	if (pinmode_new != pinmode_old) {
		_exp->setConfigureMode(_port, mode, pinmode_new);
	}
	_exp->getConfigureMode(_port, mode, &pinmode_old);
	return (pinmode_new != pinmode_old) ? false: true;
}

#endif
