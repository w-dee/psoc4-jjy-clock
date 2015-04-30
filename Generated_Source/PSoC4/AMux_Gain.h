/*******************************************************************************
* File Name: AMux_Gain.h
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

#if !defined(CY_AMUX_AMux_Gain_H)
#define CY_AMUX_AMux_Gain_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_Gain_Start(void) ;
#define AMux_Gain_Init() AMux_Gain_Start()
void AMux_Gain_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_Gain_Stop(void); */
/* void AMux_Gain_Select(uint8 channel); */
/* void AMux_Gain_Connect(uint8 channel); */
/* void AMux_Gain_Disconnect(uint8 channel); */
/* void AMux_Gain_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_Gain_CHANNELS  2u
#define AMux_Gain_MUXTYPE   1
#define AMux_Gain_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_Gain_NULL_CHANNEL 0xFFu
#define AMux_Gain_MUX_SINGLE   1
#define AMux_Gain_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_Gain_MUXTYPE == AMux_Gain_MUX_SINGLE
# if !AMux_Gain_ATMOSTONE
#  define AMux_Gain_Connect(channel) AMux_Gain_Set(channel)
# endif
# define AMux_Gain_Disconnect(channel) AMux_Gain_Unset(channel)
#else
# if !AMux_Gain_ATMOSTONE
void AMux_Gain_Connect(uint8 channel) ;
# endif
void AMux_Gain_Disconnect(uint8 channel) ;
#endif

#if AMux_Gain_ATMOSTONE
# define AMux_Gain_Stop() AMux_Gain_DisconnectAll()
# define AMux_Gain_Select(channel) AMux_Gain_FastSelect(channel)
void AMux_Gain_DisconnectAll(void) ;
#else
# define AMux_Gain_Stop() AMux_Gain_Start()
void AMux_Gain_Select(uint8 channel) ;
# define AMux_Gain_DisconnectAll() AMux_Gain_Start()
#endif

#endif /* CY_AMUX_AMux_Gain_H */


/* [] END OF FILE */
