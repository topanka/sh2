//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <UTouch.h>
#include <SpeedTrig.h>

#include <cpmdn.h>
#include <sh1tmr.h>
#include <uccbst.h>
#include <uccbcrc.h>

//general
unsigned long g_loop_cps=0;         //loop counter per sec

//touch screen
// Declare which fonts we will be using
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

// Set the pins to the correct ones for your development shield
// ------------------------------------------------------------
// Arduino Uno / 2009:
// -------------------
// Standard Arduino Uno/2009 shield            : <display model>,A5,A4,A3,A2
// DisplayModule Arduino Uno TFT shield        : <display model>,A5,A4,A3,A2
//
// Arduino Mega:
// -------------------
// Standard Arduino Mega/Due shield            : <display model>,38,39,40,41
// CTE TFT LCD/SD Shield for Arduino Mega      : <display model>,38,39,40,41
//
// Remember to change the model parameter to suit your display module!
//UTFT myGLCD(ITDB32S,38,39,40,41);
UTFT myGLCD(ITDB50,38,39,40,41);
UTouch  myTouch( 6, 5, 4, 3, 2);

word g_dsp_background=VGA_BLACK;
int g_idx=0,g_touchx=0,g_touchy=0;
long val=0,g_oval=0;
unsigned char g_puff[24];
CPMDN g_xmdn,g_ymdn;
unsigned long g_millis=0;

MYTMR g_tmr_touch={0};
MYTMR g_tmr_comm={0};

int g_force_send=0;

//UCCB

//battery
int g_cb_battV=-1,go_cb_battV=-1;

//joy
int g_cb_tsX=-1,go_cb_tsX=-1;
int g_cb_tsY=-1,go_cb_tsY=-1;

int g_cb_fsX=-1,go_cb_fsX=-1;
int g_cb_fsY=-1,go_cb_fsY=-1;
int g_cb_fsZ=-1,go_cb_fsZ=-1;
int g_cb_fsBS=-1,go_cb_fsBS=-1;
int g_cb_fsBE=-1,go_cb_fsBE=-1;

//10p switch
int g_cb_sw10p=-1,go_cb_sw10p=-1;

//6p button
int g_cb_b6p=-1,go_cb_b6p=-1;
int g_cb_b6pBS=-1,go_cb_b6pBS=-1;
int g_cb_b6pBE=-1,go_cb_b6pBE=-1;

//comm
unsigned long g_cb_w_commpkt_counter=0,go_cb_w_commpkt_counter=0;

//servo
int g_cb_tsxp=0,go_cb_tsxp=0;
int g_cb_tsyp=0,go_cb_tsyp=0;

int g_cb_rdd=0,go_cb_rdd=-1;

//lights

int g_cb_poslight=UCCB_PL_OFF,go_cb_poslight=UCCB_PL_OFF;

//motor
int g_cb_mLs=0,go_cb_mLs=-1;
int g_cb_mRs=0,go_cb_mRs=-1;
uint16_t g_sh1_mLrpm=0,go_sh1_mLrpm=0;
uint16_t g_sh1_mRrpm=0,go_sh1_mRrpm=0;

#define WHEEL_X0    200
#define WHEEL_Y0    200
#define WHEEL_R0    30
#define WHEEL_R1    140
#define WHEEL_R2    150
#define WHEEL_A1    5
#define WHEEL_B     25

#define POWER_X0    (WHEEL_X0+190)
#define POWER_Y0    (WHEEL_Y0+180)
#define POWER_A     60
#define POWER_B     360

int g_tscr_power=0;
int g_tscr_rudder=0;
char g_tscr_takeover=0;
word g_tscr_power_color=VGA_FUCHSIA;
