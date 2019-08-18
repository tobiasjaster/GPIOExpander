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

#ifndef MBED_INTERRUPTININTERFACE_H_
#define MBED_INTERRUPTININTERFACE_H_

#include "platform/platform.h"
#include "platform/Callback.h"
#include "platform/NonCopyable.h"
#include "hal/gpio_irq_api.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital interrupt input, used to call a function on a rising or falling edge
 *
 * @note Synchronization level: Interrupt safe
 *
 * Example:
 * @code
 * // Flash an LED while waiting for events
 *
 * #include "mbed.h"
 *
 * InterruptIn event(p16);
 * DigitalOut led(LED1);
 *
 * void trigger() {
 *     printf("triggered!\n");
 * }
 *
 * int main() {
 *     event.rise(&trigger);
 *     while(1) {
 *         led = !led;
 *         wait(0.25);
 *     }
 * }
 * @endcode
 * @ingroup drivers
 */
class InterruptInInterface : private NonCopyable<InterruptInInterface> {

public:

    /** Read the input, represented as 0 or 1 (int)
     *
     *  @returns
     *    An integer representing the state of the input pin,
     *    0 for logical 0, 1 for logical 1
     */
    virtual int read() {

    	return 0;
    }


    /** Attach a function to call when a rising edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    virtual void rise(Callback<void()> func) = 0;

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
    void rise(T *obj, M method);

    /** Attach a function to call when a falling edge occurs on the input
     *
     *  @param func A pointer to a void function, or 0 to set as none
     */
    virtual void fall(Callback<void()> func) = 0;

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
    void fall(T *obj, M method);

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone
     */
    virtual void mode(PinMode pull) = 0;

    /** Enable IRQ. This method depends on hw implementation, might enable one
     *  port interrupts. For further information, check gpio_irq_enable().
     */
    virtual void enable_irq() = 0;

    /** Disable IRQ. This method depends on hw implementation, might disable one
     *  port interrupts. For further information, check gpio_irq_disable().
     */
    virtual void disable_irq() = 0;

    static void _irq_handler(uint32_t id, gpio_irq_event event);
};

} // namespace mbed

#endif /* MBED_OS_DRIVERS_INTERRUPTININTERFACE_H_ */
