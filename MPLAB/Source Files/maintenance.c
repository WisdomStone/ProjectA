/*******************************************************************************

maintenance.c
====================================

	Revision History:
	=================
 ver 1.00, date: 5.9.14
		- Initial revision		
		
********************************************************************************
	General:
	========
This block handle all maintenance at the HUB.
This block support:
-Switch ON/OFF phone battery relay 
-Switch ON/OFF phone power relay 
-Read 12V battery voltage use analog pin
-read 3bit charge control status:
	-2bit for battery level
	-1bit for charging status
*******************************************************************************/

/***** INCLUDE FILES: *********************************************************/
#include "wistone_main.h"	//Application
#ifdef COMMUNICATION_PLUG
#if defined MAINTENANCE_CARD
#include "ports.h"
#include "system.h"
#include "error.h"				//Application
#include "HardwareProfile.h"	//Common
#include "usb.h"
#include "maintenance.h"

//this holds the value of the 12V battary voltage
//the value is assined at analog block
unsigned int    g_12vbat_level;
BOOL 			phone_reseting;
/*******************************************************************************
// init_maintenance()
	-init power and pattary relay
	-init charge control inputs
*******************************************************************************/
void init_maintenance(){
	//set all relay pins to 0 off and all tris for output
	MAINTENANCE_TRIS_BAT_RELAY = 0;// set pin to output
	MAINTENANCE_LAT_BAT_RELAY  = 0;

	MAINTENANCE_TRIS_USB_RELAY = 0;// set pin to output
	MAINTENANCE_LAT_USB_RELAY  = 0;

	MAINTENANCE_TRIS_12V_RELAY = 0;// set pin to output
	MAINTENANCE_LAT_12V_RELAY  = 0;

	MAINTENANCE_TRIS_POWER_RELAY = 0; // set pin to output
	MAINTENANCE_LAT_POWER_RELAY	= 0;

	//set pins from chaarge control for input
	CG_TRIS_CG_CHARGE_STATUS = 1;
	CG_TRIS_0_BAT_LEVEL =1;
	CG_TRIS_1_BAT_LEVEL	=1;

	//In case that the phone is OFF, when the plug start it start the phone as well
	phone_reseting  = TRUE;
}

/*******************************************************************************/
// set_bat_relay()
// This function set the battery relay for ON or OFF
/*******************************************************************************/
void set_bat_relay(int x){
	m_write("Battery relay is ");
	if (x){
		MAINTENANCE_LAT_BAT_RELAY = 1;
		m_write("ON\r\n");
	}
	else{
		MAINTENANCE_LAT_BAT_RELAY = 0;
		m_write("OFF\r\n");
	}
}
/*******************************************************************************/
// set_usb_relay()
// This function set the usb relay for ON or OFF
/*******************************************************************************/
void set_usb_relay(int x){
	m_write("USB relay is ");
	if (x){
		MAINTENANCE_LAT_USB_RELAY	= 1;
		m_write("ON\r\n");
	}
	else{
		MAINTENANCE_LAT_USB_RELAY	= 0;
		m_write("OFF\r\n");
	}
}
/*******************************************************************************/
// set_12V_relay()
// This function set the 12V relay for ON or OFF
/*******************************************************************************/
void set_12V_relay(int x){
	m_write("12V relay is ");
	if (x){
		MAINTENANCE_LAT_12V_RELAY	= 1;
		m_write("ON\r\n");
	}
	else{
		MAINTENANCE_LAT_12V_RELAY	= 0;
		m_write("OFF\r\n");
	}
}
/*******************************************************************************/
// set_power_relay()
// This function set the power switch relay ON or OFF
/*******************************************************************************/
void set_power_relay(int x){
	m_write("Power relay is ");
	if (x){
		MAINTENANCE_LAT_POWER_RELAY	= 1;
		m_write("ON\r\n");
	}
	else{
		MAINTENANCE_LAT_POWER_RELAY	= 0;
		m_write("OFF\r\n");
	}
}
/*******************************************************************************/
// restart_phone()
// When restarting the android device, we first desconnect the 12V power supply
// and then disconnect and connect the phone battary.
// then, start the phone, and by reconnecting the 12V suppply, the connection is
// become active
/*******************************************************************************/
void restart_phone(unsigned int state){
	
	switch (state) {
		case BAT_OFF:
			play_buzzer(1);
			MAINTENANCE_LAT_12V_RELAY	= 1;
			MAINTENANCE_LAT_BAT_RELAY = 1;			
			phone_reseting = TRUE;
			break;
		case BAT_ON_POWER_ON:
			MAINTENANCE_LAT_BAT_RELAY = 0;
			break;
		case POWER_OFF:
			MAINTENANCE_LAT_POWER_RELAY	= 1;
			break;
		case USB_OFF:
			MAINTENANCE_LAT_POWER_RELAY	= 0;
			break;
		case USB_ON:
			MAINTENANCE_LAT_12V_RELAY	= 0;
			play_buzzer(1);
			phone_reseting = FALSE;	
			break;
		default: break;
	}		
	
}

/*******************************************************************************/
// maintenance_status()
// send via USB port maintenance ststus:
// - 12V battery level form ADC (in volts)
// - relays status
// - charging status (from the charge control)
// - 12V battery level (from the charge control)
/*******************************************************************************/
void maintenance_status(void){
	m_write("Maintenance Status:\r\n");
 	print_12V_bat_status();
	print_relay_status();
	print_charger_status();
	print_bat12v_level();
}
/*******************************************************************************/
// print_12V_bat_status()
// print the 12V AVC input
// each 50unit is 1.5V
/*******************************************************************************/
void print_12V_bat_status(void){
	static unsigned int offset = 60474;
	unsigned int tmp_power = g_12vbat_level; //YM TBD - each 50~1.5V 
	unsigned int iter=0;
	unsigned int temp=0;
	char ch[6];
	//tmp_power *= (unsigned int)30; 
	tmp_power -= (unsigned int)offset;

	m_write("12V Battery level:\t");
	
	for (iter = 0 ; iter < 5 ; iter++){
		temp = tmp_power % 10;
		tmp_power /= 10;
		ch[4-iter] = num2char_2(temp);
		
	}
	ch[5] = '\0';
	m_write(ch);
	m_write("\r\n");

}
/*******************************************************************************/
// print_relay_status()
// send via USB port maintenance ststus:
// - status regarding power and battery relay
/*******************************************************************************/
void print_relay_status(void){
	m_write("Battery relay status: ");
	if (MAINTENANCE_LAT_BAT_RELAY){
		m_write("ON\r\n");
	}
	else{
		m_write("OFF\r\n");
	}
	m_write("Power relay status: ");
	if (MAINTENANCE_LAT_POWER_RELAY){
		m_write("ON\r\n");
	}
	else{
		m_write("OFF\r\n");
	}
	m_write("USB relay status: ");
	if (MAINTENANCE_LAT_USB_RELAY){
		m_write("ON\r\n");
	}
	else{
		m_write("OFF\r\n");
	}
	m_write("12V relay status: ");
	if (MAINTENANCE_LAT_12V_RELAY){
		m_write("ON\r\n");
	}
	else{
		m_write("OFF\r\n");
	}

}
/*******************************************************************************
// init_charge_controll_maintenance()
// Set all pins for inputs
*******************************************************************************/
void init_charge_controll_maintenance(){

}
/*******************************************************************************
// print_charger_status()
// read the charging status from charge control led
*******************************************************************************/
void print_charger_status(void){
		m_write("Charging status: ");
	if(CG_PORT_CHARGE_STATUS){
		m_write("OFF\r\n");
	}
	else{
		m_write("NO\r\n");
	}
}
/*******************************************************************************
// get_charger_status()\
// read the charging status from charge control led
//	0=chargin
*******************************************************************************/
int get_charger_status(void){
	if(CG_PORT_CHARGE_STATUS){
		return 0;
	}
	return 1;
}

/*******************************************************************************/
// int get_bat12v_leve(void)
// by using the two leds from the charge control we know what is the batery level
// CG_PORT_0_BAT_LEVEL | CG_PORT_1_BAT_LEVEL | battery level | return
// 			0		   |		1			 |		80%		 |	2
//			0		   |		0			 |		50%		 |  1
//			1		   | 		0			 | 		25%		 |  0
// 
/*******************************************************************************/
int get_bat12v_level(void){
		if (CG_PORT_1_BAT_LEVEL) {
			return 2;
}
		else return !CG_PORT_0_BAT_LEVEL;
}
/*******************************************************************************/
//void print_charger_status(void){
// 
/*******************************************************************************/
void print_bat12v_level(void){
		m_write("12V batary level: ");
	if (!CG_PORT_0_BAT_LEVEL && CG_PORT_1_BAT_LEVEL){
		m_write("80%");
	}
	else if (!CG_PORT_0_BAT_LEVEL && !CG_PORT_1_BAT_LEVEL){
		m_write("50%");
	}
	else if (CG_PORT_0_BAT_LEVEL && !CG_PORT_1_BAT_LEVEL){
		m_write("25%");
	}
	else{
		m_write("No battery connected");
	}
	m_write(" \r\n");
}

#endif
#endif
