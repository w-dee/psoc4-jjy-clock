/*******************************************************************************
* File Name: AMux_CapSw.h
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

#if !defined(CY_AMUX_AMux_CapSw_H)
#define CY_AMUX_AMux_CapSw_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void AMux_CapSw_Start(void) ;
#define AMux_CapSw_Init() AMux_CapSw_Start()
void AMux_CapSw_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void AMux_CapSw_Stop(void); */
/* void AMux_CapSw_Select(uint8 channel); */
/* void AMux_CapSw_Connect(uint8 channel); */
/* void AMux_CapSw_Disconnect(uint8 channel); */
/* void AMux_CapSw_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define AMux_CapSw_CHANNELS  2u
#define AMux_CapSw_MUXTYPE   1
#define AMux_CapSw_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define AMux_CapSw_NULL_CHANNEL 0xFFu
#define AMux_CapSw_MUX_SINGLE   1
#define AMux_CapSw_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if AMux_CapSw_MUXTYPE == AMux_CapSw_MUX_SINGLE
# if !AMux_CapSw_ATMOSTONE
#  define AMux_CapSw_Connect(channel) AMux_CapSw_Set(channel)
# endif
# define AMux_CapSw_Disconnect(channel) AMux_CapSw_Unset(channel)
#else
# if !AMux_CapSw_ATMOSTONE
void AMux_CapSw_Connect(uint8 channel) ;
# endif
void AMux_CapSw_Disconnect(uint8 channel) ;
#endif

#if AMux_CapSw_ATMOSTONE
# define AMux_CapSw_Stop() AMux_CapSw_DisconnectAll()
# define AMux_CapSw_Select(channel) AMux_CapSw_FastSelect(channel)
void AMux_CapSw_DisconnectAll(void) ;
#else
# define AMux_CapSw_Stop() AMux_CapSw_Start()
void AMux_CapSw_Select(uint8 channel) ;
# define AMux_CapSw_DisconnectAll() AMux_CapSw_Start()
#endif

#endif /* CY_AMUX_AMux_CapSw_H */


/* [] END OF FILE */
