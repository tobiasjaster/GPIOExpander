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
#ifndef MBED_EXPDIGITALIN_H
#define MBED_EXPDIGITALIN_H

#include "mbed.h"
#include "DigitalInInterface.h"
#include "GPIOExpansionInterface.h"

#if DEVICE_EXPANSION || defined(DOXYGEN_ONLY)

/** An external digital input, used for reading the state of a pin
 *
 * @note Synchronization level: Interrupt safe
 *
 */

class ExpDigitalIn : public DigitalInInterface{

public:
    /** Create an ExpDigitalIn connected to the specified pin
     *
     *  @param exp 	ExpansionInterface which controls the external pin
     *  @param port ExpDigitalIn port to connect to
     *  @param pin 	ExpDigitalIn pin to connect to
     */
	ExpDigitalIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin);

    /** Create an ExpDigitalIn connected to the specified pin
     *
     *  @param exp 	ExpansionInterface which controls the external pin
     *  @param port ExpDigitalIn port to connect to
     *  @param pin 	ExpDigitalIn pin to connect to
     *  @param mode the initial mode of the pin
     */
	ExpDigitalIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin, PinMode mode);

	~ExpDigitalIn();
    /** Read the input, represented as 0 or 1 (int)
     *
     *  @returns
     *    An integer representing the state of the input pin,
     *    0 for logical 0, 1 for logical 1
     */
    int read();

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, OpenDrain
     */
    void mode(PinMode pull);

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    int is_connected();

    /** An operator shorthand for read()
     * \sa ExpDigitalIn::read()
     * @code
     *      ExpDigitalIn  button(BUTTON1);
     *      ExpDigitalOut led(LED1);
     *      led = button;   // Equivalent to led.write(button.read())
     * @endcode
     */
    operator int();

protected:

#if !defined(DOXYGEN_ONLY)
    bool _checkAttachment(void);
    bool _setAttachment(void);
    bool _resetAttachment(void);
    bool _setDirection(ExpDigitalDirection direction);
    bool _setMode(PinMode mode);
    bool _isConnected;
    GPIOExpansionInterface *_exp;
	ExpPortName _port;
	ExpPinName _pin;
#endif
};

#endif

#endif
