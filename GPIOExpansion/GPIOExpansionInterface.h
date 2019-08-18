/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#ifndef MBED_GPIOEXPANSIONINTERFACE_H
#define MBED_GPIOEXPANSIONINTERFACE_H

#define DEVICE_EXPANSION 1

#include "platform/platform.h"
#include "platform/Callback.h"

/** @file ExpansionInterface.h Expander Interface base class */

 /** typedef enum for Expansion Port
  *
  *  Enum to get Port of GPIO Expander
  */
typedef enum {
	EXPPORT_A = 0x00,
	EXPPORT_B = 0x01,
	EXPPORT_C = 0x02,
	EXPPORT_D = 0x03,
	EXPPORT_E = 0x04,
	EXPPORT_F = 0x05,
	EXPPORT_G = 0x06,
	EXPPORT_H = 0x07,
	EXPPORT_I = 0x08,
	EXPPORT_NP= 0xFF,
} ExpPortName;

/** typedef enum for Expansion Port Width
 *
 *  Enum to discribe PortWidth of GPIO Expander
 */
typedef enum {
	PW_8  = 0x08,
	PW_16 = 0x10,
	PW_24 = 0x18,
	PW_32 = 0x20,
} ExpPortWidth;

/** typedef enum for Expansion direction
 *
 */
typedef enum {
	INPUT  = 0x00,
	OUTPUT = 0x01,
} ExpDigitalDirection;

/** typedef enum for Expansion pins
 *
 *  Has the ability to control 32bit width GPIO ports
 */
typedef enum {
	EXPPIN_0  = 0x00, EXPPIN_1  = 0x01, EXPPIN_2  = 0x02, EXPPIN_3  = 0x03,
	EXPPIN_4  = 0x04, EXPPIN_5  = 0x05, EXPPIN_6  = 0x06, EXPPIN_7  = 0x07,
	EXPPIN_8  = 0x08, EXPPIN_9  = 0x09, EXPPIN_10 = 0x0A, EXPPIN_11 = 0x0B,
	EXPPIN_12 = 0x0C, EXPPIN_13 = 0x0D, EXPPIN_14 = 0x0E, EXPPIN_15 = 0x0F,
	EXPPIN_16 = 0x10, EXPPIN_17 = 0x11, EXPPIN_18 = 0x12, EXPPIN_19 = 0x03,
	EXPPIN_20 = 0x14, EXPPIN_21 = 0x15, EXPPIN_22 = 0x16, EXPPIN_23 = 0x07,
	EXPPIN_24 = 0x18, EXPPIN_25 = 0x19, EXPPIN_26 = 0x1A, EXPPIN_27 = 0x0B,
	EXPPIN_28 = 0x1C, EXPPIN_29 = 0x1D, EXPPIN_30 = 0x1E, EXPPIN_31 = 0x0F,
	EXPPIN_NP = 0xFF,
} ExpPinName;

/** typedef enum for Expansion Error handling
 *
 */
typedef enum {
	ExpError_OK 			= 0x00,
	ExpError_READ 			= 0x01,
	ExpError_WRITE 			= 0x02,
	ExpError_NOTINITIALIZED = 0x03,
	ExpError_MASK 			= 0x04,
} ExpError;

/** Common interface that is shared between Expansion Drivers.
 *
 */
class GPIOExpansionInterface {
public:

	/** Get the PortWidth of GPIOExpander
	 *
	 *  Provide Port Width for iteration throw int return values
	 *
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpPortWidth getPortWidth(void)
	{
		return PW_8;
	};

	/** Get the direction register of expected port
	 *
	 *  Provide port register to pointer destination.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param data		int-pointer to write data throw
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpError getDirection(ExpPortName port, int *data)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Set the direction register of expected port
	 *
	 *  Used to write port register to set direction.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port							Typdef Enum of ExpPortName for selecting port register
	 *  @param directionMask		int for writing direction to port
	 *  @return									Typedef Enum of ExpPortWidth
	 */
	virtual ExpError setDirection(ExpPortName port, int directionMask)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Get the PullUp register of expected port
	 *
	 *  Provide port register to pointer destination.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param mode		PinMode param to select, which mode register should be read
	 *  @param data		int-pointer to write data throw
	 *  @return				Typedef Enum of ExpPortWidth
	*/
	virtual ExpError getConfigureMode(ExpPortName port, PinMode mode, int *data)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Set the Pullup register of expected port
	 *
	 *  Used to write port register to set pullup.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param mode		PinMode param to select, which mode register should be changed
	 *  @param mask		int to write pullup configuration to port
	 *  @return			Typedef Enum of ExpPortWidth
	 */
	virtual ExpError setConfigureMode(ExpPortName port, PinMode mode, int modeMask)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Get the Interrupt controlling register of expected port
	 *
	 *  Provide port register for InterruptEnabling, risingEdge and fallingEdge to pointer destination.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param data		int-pointer to write data throw. Therefore an int[3]-array will be filled
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpError getInterruptEnable(ExpPortName port, int *data)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Set the Interrupt register of expected port
	 *
	 *  Used to write port register to set InterruptEnabling, risingEdge and fallingEdge.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port										Typdef Enum of ExpPortName for selecting port register
	 *  @param interruptsEnabledMask	int to write InterruptEnable configuration to port
	 *  @param risingEdgeMask					int to write risingEdge configuration to port
	 *  @param fallingEdgeMask				int to write fallingEdge configuration to port
	 *  @return												Typedef Enum of ExpPortWidth
	 */
	virtual ExpError setInterruptEnable(ExpPortName port, int interruptsEnabledMask, int risingEdgeMask, int fallingEdgeMask)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Provide function for Interrupt execution
	 *
	 *  Used to select callback functions of attached pins
	 */
	virtual void interruptControl(void) = 0;

	/** Get the Input register of expected port
	 *
	 *  Provide port register for ChannelInput to pointer destination.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param data		int-pointer to write data throw
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpError read(ExpPortName port, int *data)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Set the Ouput register of expected port
	 *
	 *  Used to write port register to set ChannelOutput.
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param data		int to write InterruptEnable configuration to port
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpError write(ExpPortName port, int data)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Attach selected pin of special port to internal list
	 *
	 *  Update attached configuration if available
	 *  Save function callback and object-identification for InterruptIn to provide callback throw
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param pin		Typdef Enum of ExpPinName for selecting pin
	 *  @param func		Callback<void(uint32_t,gpio_irq_event)> object for InterruptIn
	 *  @param id			uint32_t for identification of Interrupt Callback Object
	 *  @return				Typedef Enum of ExpPortWidth
	 */
	virtual ExpError attach(ExpPortName port, ExpPinName pin, Callback<void(uint32_t,gpio_irq_event)> func, uint32_t id)
	{
		return ExpError_NOTINITIALIZED;
	};

	/** Get state of attachment of pin at port
	 *
	 *  Provide boolean, whether pin at port is registered or not
	 *  For ErrorHandling function returns typdef enum ExpError
	 *
	 *  @param port		Typdef Enum of ExpPortName for selecting port register
	 *  @param pin		Typdef Enum of ExpPinName for selecting pin
	 *  @return				Boolean of registration state
	 */
	virtual bool isAttached(ExpPortName port, ExpPinName pin) = 0;

//	virtual bool isAttached(ExpPortName port) = 0;


  /** Detach selected pin of special port from internal list
   *
   *  Remove attached configuration if available
   *  For ErrorHandling function returns typdef enum ExpError
   *
   *  @param port		Typdef Enum of ExpPortName for selecting port register
   *  @param pin		Typdef Enum of ExpPinName for selecting pin
   *  @return				Typedef Enum of ExpPortWidth
   */
	virtual ExpError detach(ExpPortName port, ExpPinName pin)
	{
		return ExpError_NOTINITIALIZED;
	};
};

#endif  /* MBED_EXPANSIONINTERFACE_H */
