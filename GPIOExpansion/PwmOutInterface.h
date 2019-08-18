/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_PWMOUTINTERFACE_H_
#define MBED_PWMOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/**
 * \defgroup drivers_PwmOut PwmOut class
 * \ingroup drivers-public-api-gpio
 * @{
 */
class PwmOutInterface {

public:

    /** Set the output duty-cycle, specified as a percentage (float)
     *
     *  @param value A floating-point value representing the output duty-cycle,
     *    specified as a percentage. The value should lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.
     */
    virtual void write(float value) = 0;

    /** Return the current output duty-cycle setting, measured as a percentage (float)
     *
     *  @returns
     *    A floating-point value representing the current duty-cycle being output on the pin,
     *    measured as a percentage. The returned value will lie between
     *    0.0f (representing on 0%) and 1.0f (representing on 100%).
     *
     *  @note
     *  This value may not match exactly the value set by a previous write().
     */
    virtual float read() {
    	return 0;
    }

    /** Set the PWM period, specified in seconds (float), keeping the duty cycle the same.
     *
     *  @param seconds Change the period of a PWM signal in seconds (float) without modifying the duty cycle
     *  @note
     *   The resolution is currently in microseconds; periods smaller than this
     *   will be set to zero.
     */
    virtual void period(float seconds) = 0;

    /** Set the PWM period, specified in milliseconds (int), keeping the duty cycle the same.
     *  @param ms Change the period of a PWM signal in milliseconds without modifying the duty cycle
     */
    virtual void period_ms(int ms) = 0;

    /** Set the PWM period, specified in microseconds (int), keeping the duty cycle the same.
     *  @param us Change the period of a PWM signal in microseconds without modifying the duty cycle
     */
    virtual void period_us(int us) = 0;

    /** Set the PWM pulsewidth, specified in seconds (float), keeping the period the same.
     *  @param seconds Change the pulse width of a PWM signal specified in seconds (float)
     */
    virtual void pulsewidth(float seconds) = 0;

    /** Set the PWM pulsewidth, specified in milliseconds (int), keeping the period the same.
     *  @param ms Change the pulse width of a PWM signal specified in milliseconds
     */
    virtual void pulsewidth_ms(int ms) = 0;

    /** Set the PWM pulsewidth, specified in microseconds (int), keeping the period the same.
     *  @param us Change the pulse width of a PWM signal specified in microseconds
     */
    virtual void pulsewidth_us(int us) = 0;
};

/** @}*/

} // namespace mbed

#endif /* MBED_PWMOUTINTERFACE_H_ */
