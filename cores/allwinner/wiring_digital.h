/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIRING_DIGITAL_
#define _WIRING_DIGITAL_

#ifdef __cplusplus
 extern "C" {
#endif

#include "Arduino.h"

extern void pinMode(pin_size_t pinNumber, PinMode pinMode);
extern void digitalWrite(pin_size_t pinNumber, PinStatus status);
extern PinStatus digitalRead(pin_size_t pinNumber);

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_DIGITAL_ */
