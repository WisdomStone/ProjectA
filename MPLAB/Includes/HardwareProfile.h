#ifndef __HARDWAREPROFILE_H__					
#define __HARDWAREPROFILE_H__

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
#ifdef WISTONE_BOARD
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
#if defined MAINTENANCE_CARD
//Maintenance card
//YM 5/9/14
//control outputs pins to set the relays 
//the maintenance can control the following:
// -phone power switch,
// -phone battery
// -VBUS (+5V between phone and android)
// -12V power supply

//battary
#define MAINTENANCE_LAT_BAT_RELAY		LATBbits.LATB9
#define MAINTENANCE_TRIS_BAT_RELAY 		TRISBbits.TRISB9
//switch
#define MAINTENANCE_LAT_POWER_RELAY		LATAbits.LATA14
#define MAINTENANCE_TRIS_POWER_RELAY	TRISAbits.TRISA14
//USB
#define MAINTENANCE_LAT_USB_RELAY	LATBbits.LATB12
#define MAINTENANCE_TRIS_USB_RELAY	TRISBbits.TRISB12
//12V
#define MAINTENANCE_LAT_12V_RELAY	LATBbits.LATB1
#define MAINTENANCE_TRIS_12V_RELAY	TRISBbits.TRISB1

//charge control inputs:
//input indicating is the battery is charging (Chrg_Charge)
#define CG_PORT_CHARGE_STATUS		PORTDbits.RD15
#define CG_TRIS_CG_CHARGE_STATUS    TRISDbits.TRISD15
//input indicating battery status ("0,1"-80%, "1,1"-50%, "1,0"-25%)
#define CG_PORT_0_BAT_LEVEL			PORTCbits.RC2
#define CG_TRIS_0_BAT_LEVEL 	   	TRISCbits.TRISC2
#define CG_PORT_1_BAT_LEVEL			PORTEbits.RE7
#define CG_TRIS_1_BAT_LEVEL   		TRISEbits.TRISE7

#endif
//i2c**************************************************************************/
//YL 25.8: I2C1 (accelerometer) refers to RA14, RA15; I2C2 (ERT) refers to RA2, RA3 
#define I2C2_TRIS_SDA 				TRISAbits.TRISA3 		
#define I2C2_LAT_SDA 				LATAbits.LATA3   	
#define I2C2_PORT_SDA 				PORTAbits.RA3		
#define I2C2_TRIS_SCL				TRISAbits.TRISA2 	
#define I2C2_LAT_SCL 				LATAbits.LATA2 

//led**************************************************************************/
//YL 27.8 meanwhile RB3 is used only for led, and RB11 - only for switch
//#define LED_TRIS_1 				TRISBbits.TRISB11 	//same as SWITCH1
//#define LED_PORT_1 				LATBbits.LATB11	
#define LED_TRIS_1 					LED_TRIS_2			
#define LED_PORT_1 					LED_PORT_2

#define LED_TRIS_2 					TRISBbits.TRISB3 	//same as SWITCH2	
#define LED_PORT_2 					LATBbits.LATB3		

//switch***********************************************************************/
//YL 27.8 meanwhile RB3 is used only for led, and RB11 - only for switch
#define SWITCH_TRIS_1 				TRISBbits.TRISB11 	
#define SWITCH_PORT_1 				PORTBbits.RB11 	  	

#define SWITCH_TRIS_2 				SWITCH_TRIS_1		
#define SWITCH_PORT_2 				SWITCH_PORT_1
//#define SWITCH_TRIS_2 			TRISBbits.TRISB3 	//same as LED2		
//#define SWITCH_PORT_2 			PORTBbits.RB3 	  	 	

//buzzer***********************************************************************/
#define BUZZER_TRIS 				TRISBbits.TRISB4  	
#define BUZZER_PORT 				LATBbits.LATB4

//lcd**************************************************************************/
#define mLCD_E          			LATDbits.LATD4
#define mLCD_RS         			LATBbits.LATB15
#define mLCD_R_W       				LATDbits.LATD5
#define mLCD_D4        				LATEbits.LATE4
#define mLCD_D5         			LATEbits.LATE5
#define mLCD_D6         			LATEbits.LATE6
#define mLCD_D7         			LATEbits.LATE7        

//power board******************************************************************/
#define PWR_SHUTDOWN				LATDbits.LATD14
#define PWR_SHUTDOWN_DETECT			LATGbits.LATG1
#define PWR_CHRG_STAT1				PORTDbits.RD13
#define PWR_CHRG_STAT2				PORTEbits.RE3
#define PWR_EN_5V					LATEbits.LATE2
#define PWR_EN_12V					LATGbits.LATG13
#define PWR_EN_M12V					LATGbits.LATG12
#define PWR_VBAT_STAT				LATBbits.LATB10 // AN10
#define PWR_CHRG_USBPG				PORTEbits.RE1
#define PWR_CHRG_ACPG				PORTGbits.RG14

#define PWR_SHUTDOWN_TRIS			TRISDbits.TRISD14
#define PWR_SHUTDOWN_DETECT_TRIS	TRISGbits.TRISG1
#define PWR_CHRG_STAT1_TRIS			TRISDbits.TRISD13
#define PWR_CHRG_STAT2_TRIS			TRISEbits.TRISE3
#define PWR_EN_5V_TRIS				TRISEbits.TRISE2
#define PWR_EN_12V_TRIS				TRISGbits.TRISG13
#define PWR_EN_M12V_TRIS			TRISGbits.TRISG12
#define PWR_VBAT_STAT_TRIS			TRISBbits.TRISB10
#define PWR_CHRG_USBPG_TRIS			TRISEbits.TRISE1
#define PWR_CHRG_ACPG_TRIS			TRISGbits.TRISG14

//ads1282**********************************************************************/
#define DIN_MCU_1282_TRIS			TRISBbits.TRISB14
#define DIN_MCU_1282_LAT			LATBbits.LATB14
#define DOUT_1282_MCU_TRIS			TRISBbits.TRISB1
#define DOUT_1282_MCU_PORT			PORTBbits.RB1
#define ADS1282_SCLK_TRIS			TRISGbits.TRISG8
#define ADS1282_SCLK_LAT			LATGbits.LATG8
#define ADS1282_SYNC_TRIS			TRISFbits.TRISF12
#define ADS1282_SYNC_LAT			LATFbits.LATF12
#define ADS1282_PWDN_TRIS			TRISEbits.TRISE9
#define ADS1282_PWDN_LAT			LATEbits.LATE9
#define ADS1282_RESET_TRIS			TRISBbits.TRISB8
#define ADS1282_RESET_LAT			LATBbits.LATB8

//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
#elif defined EXPLORER16
//OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\

//i2c**************************************************************************/
#define I2C2_TRIS_SDA 	TRISAbits.TRISA3	
#define I2C2_LAT_SDA 	LATAbits.LATA3
#define I2C2_PORT_SDA 	PORTAbits.RA3
#define I2C2_TRIS_SCL	TRISAbits.TRISA2	
#define I2C2_LAT_SCL 	LATAbits.LATA2

//led**************************************************************************/
#define LED_TRIS_1 		TRISAbits.TRISA0 	
#define LED_PORT_1 		LATAbits.LATA0		

//switch***********************************************************************/
#define SWITCH_TRIS 	TRISBbits.TRISD13 	
#define SWITCH_PORT 	PORTBbits.RD13 		

//lcd**************************************************************************/
#define mLCD_E          LATDbits.LATD4
#define mLCD_RS         LATBbits.LATB15
#define mLCD_R_W        LATDbits.LATD5
#define mLCD_D4         LATEbits.LATE4
#define mLCD_D5         LATEbits.LATE5
#define mLCD_D6         LATEbits.LATE6
#define mLCD_D7         LATEbits.LATE7

//LCD Pin allocation:
//	- pin#1 - vss  - GND
//	- pin#2 - vdd  - 3.3V power supply voltage
//	- pin#3 - vo   - 3.3V power supply voltage for LCD
//	- pin#4 - RS   - Register Select: H - data, L - instruction
//	- pin#5 - R/W  - Read/Write
//	- pin#6 - E    - Read/Write enable
//	- pin#7 - DB0  - DB0:DB7 - data bits
//	- pin#8 - DB1   
//	- pin#9 - DB2   
//	- pin#10- DB3  
//	- pin#11- DB4  
//	- pin#12- DB5   
//	- pin#13- DB6   
//	- pin#14- DB7   
//PIC pin allocation in PIC 16 Explorer board:
//	- E 	- RD4  - output
//	- R/W	- RD5  - output
//	- RS	- RB15 - output
//	- DB4	- RE4  - output
//	- DB5	- RE5  - output
//	- DB6	- RE6  - output
//	- DB7	- RE7  - output

#endif	// #if defined WISTONE_BOARD



#endif 	//__HARDWAREPROFILE_H__
