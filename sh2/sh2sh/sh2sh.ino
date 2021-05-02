#include <Servo.h> 

#include <uccbst.h>
#include <sh1tmr.h>
#include <uccbcrc.h>

#include <RunningAverage.h>
#include <md18v25.h>
#include <limits.h>

#define SH2_VERSION        "2.0.1"

//battery
#define UCCB_BATTV_PORT              A1
#define UCCB_BATTA_PORT              A0
int g_battV=-1;
int g_battA=-1;
unsigned long g_batt_read_tmo=25;
float g_batt_curr_cutoff=1.0;     //tottal current cutoff limit in A

//temperature
#define UCCB_TEMPERATURE_PORT       A6
int g_temperature=-1;
MYTMR g_tmr_temperature={0};

//general
unsigned long g_millis=0;
unsigned long g_loop_cnt=0;         //loop counter
unsigned long g_loop_cps=0;         //loop counter per sec
unsigned long g_loop_ct=0;
int g_recv_ready=0;                 //uccb command received

//UCCB

//battery
int g_cb_battV=-1;

//joy
int g_cb_tsX=-1;
int g_cb_tsY=-1;

int g_cb_fsX=-1;
int g_cb_fsY=-1;
int g_cb_fsZ=-1;
int g_cb_fsBS=-1;
int g_cb_fsBE=-1;

//10p switch
int g_cb_sw10p=-1;

//6p button
int g_cb_b6p=-1;
int g_cb_b6pBS=-1;
int g_cb_b6pBE=-1;

//comm
unsigned long g_cb_w_commpkt_counter=0;

//servo
int g_cb_tsxp=0;
int g_cb_tsyp=0;

int g_cb_rdd=0;

//lights

#define SH1_POSLIGHT_PORT        26
int g_cb_lightpos=UCCB_PL_OFF;
MYTMR g_tmr_lightpos={0};

//motor
int g_cb_mls=0;
int g_cb_m2s=0;

uint16_t g_rpm_m1=0;
uint16_t g_rpm_m2=0;
int8_t g_dir_m1=0;
int8_t g_dir_m2=0;
uint8_t g_state_ml=0;
uint8_t g_state_mr=0;

MYTMR g_tmr_checkmc={0};

//ACS709
#define SH2SH_ACS709_VIOUTR_PORT              A2    //sensor output
#define SH2SH_ACS709_VZCRR_PORT               A4    //voltage reference output

#define SH2SH_ACS709_VIOUTL_PORT              A3    //sensor output
#define SH2SH_ACS709_VZCRL_PORT               A5    //voltage reference output

MYTMR g_tmr_acs709_s1={0};

//ACS770LCB-100U-PFF
float g_ACS770_FACTOR=(40.0/1000.0);
float g_ACS770_QOV=0.5;
float g_ACS770_cutoff=((g_ACS770_FACTOR)/g_batt_curr_cutoff);
