/*******************************************************************************
* File Name: GainAMux.h
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

#if !defined(CY_AMUX_GainAMux_H)
#define CY_AMUX_GainAMux_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cyfitter_cfg.h"


/***************************************
*        Function Prototypes
***************************************/

void GainAMux_Start(void) ;
#define GainAMux_Init() GainAMux_Start()
void GainAMux_FastSelect(uint8 channel) ;
/* The Stop, Select, Connect, Disconnect and DisconnectAll functions are declared elsewhere */
/* void GainAMux_Stop(void); */
/* void GainAMux_Select(uint8 channel); */
/* void GainAMux_Connect(uint8 channel); */
/* void GainAMux_Disconnect(uint8 channel); */
/* void GainAMux_DisconnectAll(void) */


/***************************************
*         Parameter Constants
***************************************/

#define GainAMux_CHANNELS  2u
#define GainAMux_MUXTYPE   1
#define GainAMux_ATMOSTONE 0

/***************************************
*             API Constants
***************************************/

#define GainAMux_NULL_CHANNEL 0xFFu
#define GainAMux_MUX_SINGLE   1
#define GainAMux_MUX_DIFF     2


/***************************************
*        Conditional Functions
***************************************/

#if GainAMux_MUXTYPE == GainAMux_MUX_SINGLE
# if !GainAMux_ATMOSTONE
#  define GainAMux_Connect(channel) GainAMux_Set(channel)
# endif
# define GainAMux_Disconnect(channel) GainAMux_Unset(channel)
#else
# if !GainAMux_ATMOSTONE
void GainAMux_Connect(uint8 channel) ;
# endif
void GainAMux_Disconnect(uint8 channel) ;
#endif

#if GainAMux_ATMOSTONE
# define GainAMux_Stop() GainAMux_DisconnectAll()
# define GainAMux_Select(channel) GainAMux_FastSelect(channel)
void GainAMux_DisconnectAll(void) ;
#else
# define GainAMux_Stop() GainAMux_Start()
void GainAMux_Select(uint8 channel) ;
# define GainAMux_DisconnectAll() GainAMux_Start()
#endif

#endif /* CY_AMUX_GainAMux_H */


/* [] END OF FILE */
