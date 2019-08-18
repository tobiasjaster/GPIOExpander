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
#include "ExpInterruptIn.h"

#if DEVICE_EXPANSION

ExpInterruptIn::ExpInterruptIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	_irqEnabled = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment((&ExpInterruptIn::_irq_handler), (uint32_t)this);
        _setInterrupt();
		_setMode(PullNone);
	}
}

ExpInterruptIn::ExpInterruptIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin, PinMode mode) {
	_exp = exp;
	_port = port;
	_pin = pin;
	_isConnected = false;
	_irqEnabled = false;
	if (_checkAttachment() == false && _setDirection(INPUT)) {
		_isConnected = _setAttachment((&ExpInterruptIn::_irq_handler), (uint32_t)this);
        _setInterrupt();
		_setMode(mode);
	}
}

ExpInterruptIn::~ExpInterruptIn(){
	if (_isConnected && _resetAttachment()){
		_isConnected = false;
		_setMode(PullNone);
	}
}
int ExpInterruptIn::read(void) {
	int value;
	int pinposition = 1 << (char)_pin;
	_exp->read(_port, &value);
	return (int)((value&pinposition) >> (char)_pin);
}

//int ExpInterruptIn::is_connected() {
//	return (int)_isConnected;
//}

ExpInterruptIn::operator int()
{
    // Underlying call is atomic
    return read();
}

void ExpInterruptIn::rise(Callback<void()> func) {
    if (func) {
        _rise = func;
        _setInterrupt();
        enable_irq();
    } else {
        _rise = NULL;
        _setInterrupt();
        disable_irq();
    }
}

void ExpInterruptIn::fall(Callback<void()> func) {
    if (func) {
        _fall = func;
        _setInterrupt();
        enable_irq();
    } else {
    	_fall = NULL;
        _setInterrupt();
        disable_irq();
    }
}

void ExpInterruptIn::mode(PinMode pull) {
	if (_isConnected) {
		_setMode(pull);
	}
}

void ExpInterruptIn::enable_irq(void) {
	int pinposition = 1 << (char)_pin;
	int interrupt_old[3];
	int interrupt_new[3];
    if (_irqEnabled == false) {
    	_exp->getInterruptEnable(_port, interrupt_old);
    	if ((interrupt_old[0]&pinposition) == 0){
    		interrupt_new[0] = interrupt_old[0] | pinposition;
    		interrupt_new[1] = interrupt_old[1];
    		interrupt_new[2] = interrupt_old[2];
    	}
    	if (interrupt_new[0] != interrupt_old[0]){
    		_exp->setInterruptEnable(_port, interrupt_new[0], interrupt_new[1], interrupt_new[2]);
    	}
//    	_exp->getInterruptEnable(_port, interrupt_old);
//    	if (interrupt_new[0] != interrupt_old[0]){
//    		return false;
//    	}
//    	return true;
    	_irqEnabled = true;
    }
}

void ExpInterruptIn::disable_irq(void) {
	int pinposition = 1 << (char)_pin;
	int interrupt_old[3];
	int interrupt_new[3];
    if (_irqEnabled == true) {
    	_exp->getInterruptEnable(_port, interrupt_old);
    	if ((interrupt_old[0]&pinposition) != 0){
    		interrupt_new[0] = interrupt_old[0] & ~pinposition;
    		interrupt_new[1] = interrupt_old[1];
    		interrupt_new[2] = interrupt_old[2];
    	}
    	if (interrupt_new[0] != interrupt_old[0]){
    		_exp->setInterruptEnable(_port, interrupt_new[0], interrupt_new[1], interrupt_new[2]);
    	}
//    	_exp->getInterruptEnable(_port, interrupt_old);
//    	if (interrupt_new[0] != interrupt_old[0]){
//    		return false;
//    	}
//    	return true;
    	_irqEnabled = false;
    }
}

void ExpInterruptIn::_irq_handler(uint32_t id, gpio_irq_event event){
	ExpInterruptIn *handler = (ExpInterruptIn *)id;
	switch (event) {
		case IRQ_RISE:
			if (handler->_rise) {
				handler->_rise();
			}
			break;
		case IRQ_FALL:
			if (handler->_fall) {
				handler->_fall();
			}
			break;
		case IRQ_NONE:
			break;
	}
}

bool ExpInterruptIn::_checkAttachment(void) {
	return _exp->isAttached(_port, _pin);
}

bool ExpInterruptIn::_setAttachment(Callback<void(uint32_t,gpio_irq_event)> func, uint32_t id) {
	_exp->attach(_port, _pin, func, id);
	return true;
}

bool ExpInterruptIn::_resetAttachment(void){
	_exp->detach(_port, _pin);
	return true;
}

bool ExpInterruptIn::_setDirection(ExpDigitalDirection direction) {
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

bool ExpInterruptIn::_setMode(PinMode mode){
//	if (mode != PullNone && mode !=PullUp){
//		return false;
//	}
	int pinposition = 0x01 << (char)_pin;
	int pinmode_old;
	int pinmode_new;
	int mask;
	_exp->getConfigureMode(_port, mode, &pinmode_old);
	mask = (mode != PullNone) ? 0xFFFFFFFF : 0;
	pinmode_new = (pinmode_old & ~pinposition) | (mask | pinposition);
	if (pinmode_new != pinmode_old) {
		_exp->setConfigureMode(_port, mode, pinmode_new);
	}
	_exp->getConfigureMode(_port, mode, &pinmode_old);
	if (pinmode_new != pinmode_old) {
		return false;
	}
	return true;
}

bool ExpInterruptIn::_setInterrupt(void) {
	int pinposition = 1 << (char)_pin;
	int interrupt_old[3];
	int interrupt_new[3];
    _exp->getInterruptEnable(_port, interrupt_old);
    interrupt_new[0] = interrupt_old[0];
    if (_rise && ((interrupt_old[1]&pinposition) == 0)) {
    	interrupt_new[1] = interrupt_old[1] | pinposition;
    }
    else if (!_rise && ((interrupt_old[1]&pinposition) != 0)) {
    	interrupt_new[1] = interrupt_old[1] & ~pinposition;
    }
    if (_fall && ((interrupt_old[2]&pinposition) == 0)) {
    	interrupt_new[2] = interrupt_old[2] | pinposition;
    }
    else if (!_fall && ((interrupt_old[2]&pinposition) != 0)) {
    	interrupt_new[2] = interrupt_old[2] & ~pinposition;
    }
    if ((interrupt_new[1] != interrupt_old[1]) || (interrupt_new[2] != interrupt_old[2])){
    	_exp->setInterruptEnable(_port, interrupt_new[0], interrupt_new[1], interrupt_new[2]);
    }
    _exp->getInterruptEnable(_port, interrupt_old);
    if ((interrupt_new[1] != interrupt_old[1]) || (interrupt_new[2] != interrupt_old[2])){
    	return false;
    }
    return true;
}

#endif
