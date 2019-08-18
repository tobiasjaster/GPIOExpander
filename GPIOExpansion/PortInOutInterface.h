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
 
#ifndef MBED_PORTINOUTINTERFACE_H_
#define MBED_PORTINOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {
/** \addtogroup drivers */

/** A multiple pin digital in/out used to set/read multiple bi-directional pins
 *
 * @note Synchronization level: Interrupt safe
 * @ingroup drivers
 */
class PortInOutInterface {
public:

    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding port pin
     */
    virtual void write(int value) = 0;

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated port pin setting
     */
    virtual int read() {
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
     *  @param mode PullUp, PullDown, PullNone, OpenDrain
     */
    virtual void mode(PinMode mode) = 0;
};

} // namespace mbed

#endif /* MBED_PORTINOUTINTERFACE_H_ */
