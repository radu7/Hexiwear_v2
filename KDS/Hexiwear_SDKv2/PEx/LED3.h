/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : LED3.h
**     Project     : Hexiwear_PEx_2_SDKv2
**     Processor   : MK64FN1M0VDC12
**     Component   : LED
**     Version     : Component 01.072, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-11-25, 18:26, # CodeGen: 24
**     Abstract    :
**          This component implements a universal driver for a single LED.
**     Settings    :
**          Component name                                 : LED3
**          SDK                                            : KSDK1
**          Turned On with initialization                  : no
**          HW Interface                                   : 
**            Anode on port side, HIGH is ON               : yes
**            On/Off                                       : Enabled
**              Pin                                        : SDK_BitIO
**            PWM                                          : Disabled
**          Shell                                          : Disabled
**     Contents    :
**         On         - void LED3_On(void);
**         Off        - void LED3_Off(void);
**         Neg        - void LED3_Neg(void);
**         Get        - uint8_t LED3_Get(void);
**         Put        - void LED3_Put(uint8_t val);
**         SetRatio16 - void LED3_SetRatio16(uint16_t ratio);
**         Deinit     - void LED3_Deinit(void);
**         Init       - void LED3_Init(void);
**
**     License   : Open Source (LGPL)
**     Copyright : Erich Styger, 2013-2016, all rights reserved.
**     Web       : www.mcuoneclipse.com
**     This an open source software implementing a driver using Processor Expert.
**     This is a free software and is opened for education, research and commercial developments under license policy of following terms:
**     * This is a free software and there is NO WARRANTY.
**     * No restriction on use. You can use, modify and redistribute it for personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
**     * Redistributions of source code must retain the above copyright notice.
** ###################################################################*/
/*!
** @file LED3.h
** @version 01.00
** @brief
**          This component implements a universal driver for a single LED.
*/         
/*!
**  @addtogroup LED3_module LED3 module documentation
**  @{
*/         

#ifndef __LED3_H
#define __LED3_H

/* MODULE LED3. */
#include "KSDK1.h" /* include SDK */

#if KSDK1_SDK_VERSION_USED == KSDK1_SDK_VERSION_NONE
  /* Include shared modules, which are used for whole project */
  #include "PE_Types.h"
  #include "PE_Error.h"
  #include "PE_Const.h"
  #include "IO_Map.h"
#endif

/* Include inherited components */
#include "KSDK1.h"
#include "LEDpin6.h"

#define LED3_ClrVal()    LEDpin6_ClrVal() /* put the pin on low level */
#define LED3_SetVal()    LEDpin6_SetVal() /* put the pin on high level */
#define LED3_SetInput()  LEDpin6_SetInput() /* use the pin as input pin */
#define LED3_SetOutput() LEDpin6_SetOutput() /* use the pin as output pin */

#define LED3_PARSE_COMMAND_ENABLED  0 /* set to 1 if method ParseCommand() is present, 0 otherwise */


#define LED3_On() LEDpin6_SetVal()
/*
** ===================================================================
**     Method      :  LED3_On (component LED)
**     Description :
**         This turns the LED on.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED3_Off() LEDpin6_ClrVal()
/*
** ===================================================================
**     Method      :  LED3_Off (component LED)
**     Description :
**         This turns the LED off.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED3_Neg() LEDpin6_NegVal()
/*
** ===================================================================
**     Method      :  LED3_Neg (component LED)
**     Description :
**         This negates/toggles the LED
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED3_Get() LEDpin6_GetVal()
/*
** ===================================================================
**     Method      :  LED3_Get (component LED)
**     Description :
**         This returns logical 1 in case the LED is on, 0 otherwise.
**     Parameters  : None
**     Returns     :
**         ---             - Status of the LED (on or off)
** ===================================================================
*/

void LED3_Init(void);
/*
** ===================================================================
**     Method      :  LED3_Init (component LED)
**     Description :
**         Performs the LED driver initialization.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define LED3_Put(val)  ((val) ? LED3_On() : LED3_Off())
/*
** ===================================================================
**     Method      :  LED3_Put (component LED)
**     Description :
**         Turns the LED on or off.
**     Parameters  :
**         NAME            - DESCRIPTION
**         val             - value to define if the LED has to be on or
**                           off.
**     Returns     : Nothing
** ===================================================================
*/

void LED3_Deinit(void);
/*
** ===================================================================
**     Method      :  LED3_Deinit (component LED)
**     Description :
**         Deinitializes the driver
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void LED3_SetRatio16(uint16_t ratio);
/*
** ===================================================================
**     Method      :  LED3_SetRatio16 (component LED)
**     Description :
**         Method to specify the duty cycle. If using a PWM pin, this
**         means the duty cycle is set. For On/off pins, values smaller
**         0x7FFF means off, while values greater means on.
**     Parameters  :
**         NAME            - DESCRIPTION
**         ratio           - Ratio value, where 0 means 'off' and
**                           0xffff means 'on'
**     Returns     : Nothing
** ===================================================================
*/

/* END LED3. */

#endif
/* ifndef __LED3_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/