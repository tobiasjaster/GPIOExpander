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
#ifndef EXPPORTOUT_H
#define EXPPORTOUT_H

#include "mbed.h"
#include "ExpanderInterface.h"

#if DEVICE_EXPANDER || defined(DOXYGEN_ONLY)

namespace mbed {
/** \addtogroup drivers */
/** A multiple pin digital output
 *
 * @note Synchronization level: Interrupt safe
 */
class ExpPortOut {
public:

     /** Create an ExpPortOut connected to the specified pin
 		 *
 		 *  @param exp 	ExpansionInterface which controls the external pin
 		 *  @param port ExpPortOut port to connect to
     *  @param mask Bitmask defines which port pins should be an input (0 - ignore, 1 - include)
 		 */
    ExpPortOut(ExpanderInterface *exp, ExpPortName port, int mask = 0xFFFFFFFF);

    ~ExpPortOut();
    /** Write the value to the output port
     *
     *  @param value An integer specifying a bit to write for every corresponding ExpPortOut pin
     */
    void write(int value);

    /** Read the value currently output on the port
     *
     *  @returns
     *    An integer with each bit corresponding to associated pin value
     */
    int read();

    /** A shorthand for write()
     * \sa ExpPortOut::write()
     */
    ExpPortOut &operator= (int value);

    /** A shorthand for read()
     * \sa ExpPortOut::read()
     */
    ExpPortOut &operator= (ExpPortOut &rhs);

    /** A shorthand for read()
     * \sa ExpPortOut::read()
     */
    operator int();

#if !defined(DOXYGEN_ONLY)
protected:
    bool _checkAttachment(void);
    bool _setAttachment(void);
    bool _resetAttachment(void);
    bool _setDirection(ExpDigitalDirection direction);
    ExpanderInterface *_exp;
    ExpPortName _port;
    int _mask;
    bool _isConnected;
#endif
};

} // namespace mbed

#endif

#endif
