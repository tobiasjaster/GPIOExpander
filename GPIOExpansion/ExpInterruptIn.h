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
#ifndef EXPINTERRUPTIN_H
#define EXPINTERRUPTIN_H

#include "mbed.h"
#include "InterruptInInterface.h"
#include "GPIOExpansionInterface.h"

#if DEVICE_EXPANSION || defined(DOXYGEN_ONLY)

namespace mbed {

/** An external digital interrupt input, used to call a function on a rising or falling edge
 *
 * @note Synchronization level: Interrupt safe
 */
class ExpInterruptIn : InterruptInInterface {

public:

    /** Create an ExpInterruptIn connected to the specified pin
     *
     *  @param exp 	ExpansionInterface which controls the external pin
		 *  @param port ExpInterruptIn port to connect to
		 *  @param pin 	ExpInterruptIn pin to connect to
     */
	ExpInterruptIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin);

	  /** Create an ExpInterruptIn connected to the specified pin
	   *
	   *  @param exp 	ExpansionInterface which controls the external pin
		 *  @param port ExpInterruptIn port to connect to
		 *  @param pin 	ExpInterruptIn pin to connect to
		 *  @param mode the initial mode of the pin
	   */
	ExpInterruptIn(GPIOExpansionInterface *exp, ExpPortName port, ExpPinName pin, PinMode mode);

    virtual ~ExpInterruptIn();

    /** Read the input, represented as 0 or 1 (int)
     *
     *  @returns
     *    An integer representing the state of the input pin,
     *    0 for logical 0, 1 for logical 1
     */
    int read();

    /** An operator shorthand for read()
     */
    operator int();


    /** Attach a function to call when a rising edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    void rise(Callback<void()> func);

    /** Attach a member function to call when a rising edge occurs on the input
     *
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to be called
     *  @deprecated
     *      The rise function does not support cv-qualifiers. Replaced by
     *      rise(callback(obj, method)).
     */
    template<typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "The rise function does not support cv-qualifiers. Replaced by "
                          "rise(callback(obj, method)).")
    void rise(T *obj, M method)
    {
        core_util_critical_section_enter();
        rise(callback(obj, method));
        core_util_critical_section_exit();
    }

    /** Attach a function to call when a falling edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    void fall(Callback<void()> func);

    /** Attach a member function to call when a falling edge occurs on the input
     *
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to be called
     *  @deprecated
     *      The rise function does not support cv-qualifiers. Replaced by
     *      rise(callback(obj, method)).
     */
    template<typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "The fall function does not support cv-qualifiers. Replaced by "
                          "fall(callback(obj, method)).")
    void fall(T *obj, M method)
    {
        core_util_critical_section_enter();
        fall(callback(obj, method));
        core_util_critical_section_exit();
    }

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, PullDefault
     *  See PinNames.h for your target for definitions)
     */
    void mode(PinMode pull);

    /** Enable IRQ. Set the GPIOExpander Interrupt Register
     */
    void enable_irq();

    /** Disable IRQ. Reset the GPIOExpander Interrupt Register
     */
    void disable_irq();

    static void _irq_handler(uint32_t id, gpio_irq_event event);
#if !defined(DOXYGEN_ONLY)
protected:
    bool _checkAttachment(void);
    bool _setAttachment(Callback<void(uint32_t,gpio_irq_event)> func, uint32_t id);
    bool _resetAttachment(void);
    bool _setDirection(ExpDigitalDirection direction);
    bool _setMode(PinMode mode);
    bool _setInterrupt(void);
    bool _isConnected;
    bool _irqEnabled;
    GPIOExpansionInterface *_exp;
	ExpPortName _port;
	ExpPinName _pin;
	Callback<void()> _rise;
	Callback<void()> _fall;
#endif
};

} // namespace mbed

#endif

#endif
