#ifndef __LED_BUZZER_H__	
#define __LED_BUZZER_H__

/***** DEFINE: ****************************************************************/
#define LED_1 1
#define LED_2 2
#define SWITCH_1 1
#define SWITCH_2 2
#if defined MAINTENANCE_CARD
extern BOOL phone_reseting;
#endif
/***** FUNCTION PROTOTYPES: ***************************************************/
void init_timer4(void);
void init_buzzer(void);
void init_leds(void);
int	 play_buzzer(int period);
int  set_led(int state, int led_num);
int  get_switch(int switch_num);

#endif //__LED_BUZZER_H__
