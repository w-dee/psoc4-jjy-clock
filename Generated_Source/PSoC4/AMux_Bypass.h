/*******************************************************************************
* File Name: AMux_Bypass.h
* Version 1.80
*
*  Description:
*    This file contains the constants and function prototypes for the Analog
*    Multiplexer User Module AMux.
*
*   Note:
*
********************************************************************************
* Copyright 2008-2010, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_AMUX_AMux_Bypass_H)
#define CY_AMUX_AMux_Bypass_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_Bypass_Start(void) ;
#define AMux_Bypass_Init() AMux_Bypass_Start()
void AMux_Bypass_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Bypass_Stop(void); */
/* void AMux_Bypass_Select(uint8 channel); */
/* void AMux_Bypass_Connect(uint8 channel); */
/* void AMux_Bypass_Disconnect(uint8 channel); */
/* void AMux_Bypass_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_Bypass_CHANNELS  2u
#define AMux_Bypass_MUXTYPE   1
#define AMux_Bypass_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_Bypass_NULL_CHANNEL 0xFFu
#define AMux_Bypass_MUX_SINGLE   1
#define AMux_Bypass_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_Bypass_MUXTYPE == AMux_Bypass_MUX_SINGLE
# if !AMux_Bypass_ATMOSTONE
#  define AMux_Bypass_Connect(channel) AMux_Bypass_Set(channel)
# endif
# define AMux_Bypass_Disconnect(channel) AMux_Bypass_Unset(channel)
#else
# if !AMux_Bypass_ATMOSTONE
void AMux_Bypass_Connect(uint8 channel) ;
# endif
void AMux_Bypass_Disconnect(uint8 channel) ;
#endif

#if AMux_Bypass_ATMOSTONE
# define AMux_Bypass_Stop() AMux_Bypass_DisconnectAll()
# define AMux_Bypass_Select(channel) AMux_Bypass_FastSelect(channel)
void AMux_Bypass_DisconnectAll(void) ;
#else
# define AMux_Bypass_Stop() AMux_Bypass_Start()
void AMux_Bypass_Select(uint8 channel) ;
# define AMux_Bypass_DisconnectAll() AMux_Bypass_Start()
#endif

#endif /* CY_AMUX_AMux_Bypass_H */


/* [] END OF FILE */
