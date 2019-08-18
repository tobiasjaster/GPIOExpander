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
#ifndef MBED_ANALOGOUTINTERFACE_H_
#define MBED_ANALOGOUTINTERFACE_H_

#include "platform/platform.h"

namespace mbed {

class AnalogOutInterface {

public:

    /** Set the output voltage, specified as a percentage (float)
     *
     *  @param value A floating-point value representing the output voltage,
     *    specified as a percentage. The value should lie between
     *    0.0f (representing 0v / 0%) and 1.0f (representing 3.3v / 100%).
     *    Values outside this range will be saturated to 0.0f or 1.0f.
     */
    virtual void write(float value) = 0;

    /** Set the output voltage, represented as an unsigned short in the range [0x0, 0xFFFF]
     *
     *  @param value 16-bit unsigned short representing the output voltage,
     *            normalized to a 16-bit value (0x0000 = 0v, 0xFFFF = 3.3v)
     */
    virtual void write_u16(unsigned short value) = 0;

    /** Return the current output voltage setting, measured as a percentage (float)
     *
     *  @returns
     *    A floating-point value representing the current voltage being output on the pin,
     *    measured as a percentage. The returned value will lie between
     *    0.0f (representing 0v / 0%) and 1.0f (representing 3.3v / 100%).
     *
     *  @note
     *    This value may not match exactly the value set by a previous write().
     */
    virtual float read() {
    	return 0;
    }
};

/** @}*/

} // namespace mbed

#endif /* MBED_ANALOGOUTINTERFACE_H_ */
