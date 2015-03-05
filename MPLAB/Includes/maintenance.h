#ifndef __MAINTENANCE_H__	
#define __MAINTENANCE_H__

#include "wistone_main.h"

//#if defined COMMUNICATION_PLUG
#ifdef MAINTENANCE_CARD
//this holds the value of the 12V battery voltage
//the value is assigned at analog block
extern unsigned int	g_12vbat_level;
#include "GenericTypeDefs.h"

#define IDLE				0
#define BAT_OFF				1		
#define BAT_ON_POWER_ON		2		
#define POWER_OFF			3
#define PHONE_START			4
#define USB_OFF				5		
#define USB_ON				6

/***** FUNCTION PROTOTYPES: ***************************************************/
void init_maintenance(void);
void print_charger_status(void);
int get_charger_status(void);
void set_bat_relay(int x);
void set_power_relay(int x);
void set_usb_relay(int x);
void set_12V_relay(int x);
int  get_bat12v_level(void);
void print_bat12v_level(void);
void maintenance_status(void);
void print_12V_bat_status(void);
void print_relay_status(void);
void restart_phone(unsigned int);


//#endif

#endif
#endif //__MAINTENANCE_H__
