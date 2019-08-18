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

#ifndef MBED_DIGITALINOUTINTERFACE_H_
#define MBED_DIGITALINOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */

/** A digital input/output, used for setting or reading a bi-directional pin
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class DigitalInOutInterface {

public:
    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
	virtual void write(int value) = 0;

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin if it is an output,
     *    or read the input if set as an input
     */
    virtual int read() {
        // Thread safe / atomic HAL call
        return 0;
    }

    /** Set as an output
     */
    virtual void output() = 0;

    /** Set as an input
     */
    virtual void input() = 0;

    /** Set the input pin mode
     *
     *  @param pull PullUp, PullDown, PullNone, OpenDrain
     */
    virtual void mode(PinMode pull) = 0;

    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    virtual int is_connected() {
        // Thread safe / atomic HAL call
        return 0;
    }
};

} // namespace mbed

#endif /* MBED_DIGITALINOUTINTERFACE_H_ */
