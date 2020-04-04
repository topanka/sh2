#define SH1_RUDDERL_CENTER              1500
//nagy kerek a szervon
//#define SH1_RUDDERL_MIN                 900
//#define SH1_RUDDERL_MAX                 2100
#define SH1_RUDDERL_MIN                 1000
#define SH1_RUDDERL_MAX                 1950

#define SH1_RUDDERR_CENTER              1500
//nagy kerek a szervon
//#define SH1_RUDDERR_MIN                 900
//#define SH1_RUDDERR_MAX                 2100
#define SH1_RUDDERR_MIN                 1050
#define SH1_RUDDERR_MAX                 2000

#define SH1_HOLDERPAN_CENTER            1480
#define SH1_HOLDERPAN_MIN               650
#define SH1_HOLDERPAN_MAX               2350

#define SH1_HOLDERTILT_CENTER            1470
#define SH1_HOLDERTILT_MIN               1250
#define SH1_HOLDERTILT_MAX               1650

Servo rudderl;
Servo rudderr; 
Servo holderpan; 
Servo holdertilt; 

int g_sh1_holderpan_pos=SH1_HOLDERPAN_CENTER;
int g_sh1_holdertilt_pos=SH1_HOLDERTILT_CENTER;
unsigned long g_sh1_holder_t=0;


int servo_setup(void)
{
  rudderl.attach(5,SH1_RUDDERL_MIN,SH1_RUDDERL_MAX);
  rudderr.attach(4,SH1_RUDDERR_MIN,SH1_RUDDERR_MAX);
  holderpan.attach(2,SH1_HOLDERPAN_MIN,SH1_HOLDERPAN_MAX);
  holdertilt.attach(3,SH1_HOLDERTILT_MIN,SH1_HOLDERTILT_MAX);
  
  rudderl.writeMicroseconds(SH1_RUDDERL_CENTER);
  rudderr.writeMicroseconds(SH1_RUDDERR_CENTER);
  holderpan.writeMicroseconds(SH1_HOLDERPAN_CENTER);
  holdertilt.writeMicroseconds(SH1_HOLDERTILT_CENTER);
  
  return(0);
}

int servo_holder(void)
{
  int vx,vy;
  
  vx=constrain(g_cb_tsxp,-100,100);
  vy=constrain(g_cb_tsyp,-100,100);
  if((vx == 0) && (vy == 0)) return(0);
  
  if((g_millis-g_sh1_holder_t) < 40) return(0);
  
  vx=vx/3;
  g_sh1_holderpan_pos+=vx;
  if(g_sh1_holderpan_pos < SH1_HOLDERPAN_MIN) g_sh1_holderpan_pos=SH1_HOLDERPAN_MIN;
  else if(g_sh1_holderpan_pos > SH1_HOLDERPAN_MAX) g_sh1_holderpan_pos=SH1_HOLDERPAN_MAX;
  holderpan.writeMicroseconds(g_sh1_holderpan_pos);
  
  vy=-vy/3;
  g_sh1_holdertilt_pos+=vy;
  if(g_sh1_holdertilt_pos < SH1_HOLDERTILT_MIN) g_sh1_holdertilt_pos=SH1_HOLDERTILT_MIN;
  else if(g_sh1_holdertilt_pos > SH1_HOLDERTILT_MAX) g_sh1_holdertilt_pos=SH1_HOLDERTILT_MAX;
  holdertilt.writeMicroseconds(g_sh1_holdertilt_pos);
  
  g_sh1_holder_t=g_millis;
  
  return(0);
}

int servo_rudder(void)
{
  int v,rl,rr;
  
  if(g_recv_ready != 1) return(0);
  
  v=constrain(g_cb_rdd,-UCCB_RDD_MAXPOS,UCCB_RDD_MAXPOS);

  if(v >= 0) {
    rl=map(v,0,UCCB_RDD_MAXPOS,SH1_RUDDERL_CENTER,SH1_RUDDERL_MIN);
    rr=map(v,0,UCCB_RDD_MAXPOS,SH1_RUDDERR_CENTER,SH1_RUDDERR_MIN);
  } else {
    rl=map(v,0,-UCCB_RDD_MAXPOS,SH1_RUDDERL_CENTER,SH1_RUDDERL_MAX);
    rr=map(v,0,-UCCB_RDD_MAXPOS,SH1_RUDDERR_CENTER,SH1_RUDDERR_MAX);
  }
  rl=constrain(rl,SH1_RUDDERL_MIN,SH1_RUDDERL_MAX);
  rr=constrain(rr,SH1_RUDDERR_MIN,SH1_RUDDERR_MAX);
  
  rudderl.writeMicroseconds(rl);
  rudderr.writeMicroseconds(rr);
  
  return(0);
}

