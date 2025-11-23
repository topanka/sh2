#include "uccbtscr.h"

byte g_wts_commbuf[100]={0};
unsigned long g_wts_sendtime=0;
unsigned long g_wts_commpkt_counter=0;        //total counter of sent packets

byte g_rts_commbuf[100]={0};
int g_rts_state=TSCR_PST_INIT;
unsigned int g_rts_len=0;
 
int comm_setup(void)
{
  Serial2.begin(250000);
  buildCRCTable();
//  tmr_init(&g_tmr_comm,250);
  tmr_init(&g_tmr_comm,200);

  return(0);
}

int tscr_comm_pack1(byte *d, uint16_t l, byte *buf, uint16_t *len)
{
  if((*len+l) > sizeof(g_wts_commbuf)) return(-1);
  memcpy((void*)(buf+*len),(void*)d,l);
  (*len)+=l;
  return(0);
}

int tscr_comm_packtscr(uint16_t *len)
{
  byte lead=TSCR_TSCR_LEAD;
  byte crc8;
  unsigned int m1c=0,m2c=0;
  int16_t rpm1,rpm2;
  
  *len=0;
  

/*  
  Serial.print(rpm1);
  Serial.print(" ");
*/  
//  Serial.println(g_tscr_power);

  tscr_comm_pack1((byte*)&lead,sizeof(lead),g_wts_commbuf,len);      //1:1
  tscr_comm_pack1((byte*)&g_loop_cps,sizeof(g_loop_cps),g_wts_commbuf,len);    //4:5
  tscr_comm_pack1((byte*)&g_touchx,sizeof(g_touchx),g_wts_commbuf,len);    //2:7
  tscr_comm_pack1((byte*)&g_touchy,sizeof(g_touchy),g_wts_commbuf,len);    //2:9
  tscr_comm_pack1((byte*)&g_tscr_power,sizeof(g_tscr_power),g_wts_commbuf,len);    //2:11
  tscr_comm_pack1((byte*)&g_tscr_rudder,sizeof(g_tscr_rudder),g_wts_commbuf,len);    //2:13
  tscr_comm_pack1((byte*)&g_cb_poslight,sizeof(g_cb_poslight),g_wts_commbuf,len);    //2:15
  crc8=getCRC(g_wts_commbuf,*len);
  tscr_comm_pack1((byte*)&crc8,sizeof(crc8),g_wts_commbuf,len);    //1:16
  
//14 byte long  
  
  return(0);
}

int comm_send(void)
{
  unsigned int len;

//  g_wts_commpkt_counter++;

  if(g_force_send == 0) {
    if(tmr_do(&g_tmr_comm) != 1) return(0);
  }
  g_force_send=0;
  
  tscr_comm_packtscr(&len);
  
  Serial2.write((byte*)&g_wts_commbuf[0],len);
  
//  g_w_commpkt_counter=0;

  return(1);
}

int comm_read(int *state, unsigned char *buf, unsigned int *len)
{
  int rval=-1,nr=0;
  unsigned char c1;
  unsigned char crc8;
//  static unsigned long xx=0;

  while(Serial2.available()) {
    c1=(unsigned char)Serial2.read();
    
    if(++nr >= 100) break;
    switch(*state) {
      case TSCR_PST_INIT:
      case TSCR_PST_READY:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" init/ready");
*/  
        if(c1 == TSCR_CBOX_LEAD) {
          *len=0;
          buf[*len]=c1;
          *state=TSCR_PST_DATA;
        } else {
          *state=TSCR_PST_INIT;
        }
        break;
      case TSCR_PST_DATA:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" data");
*/  
        (*len)++;
        buf[*len]=c1;
        if(*len == TSCR_CBOX_PKTLAST) {
          *state=TSCR_PST_CRC;
        }
        break;
      case TSCR_PST_CRC:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" crc1");
*/  
        if(*len != TSCR_CBOX_PKTLAST) {
          *state=TSCR_PST_INIT;
          break;
        }
        (*len)++;
/*        
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println("crc2");
*/  
        crc8=getCRC(buf,*len);
        
/*        
        xx++;
  Serial.print(xx);
  Serial.print(" ");
  Serial.print(c1);
  Serial.print(" ");
  Serial.println(crc8);
*/  
        if(crc8 != c1) {
          *state=TSCR_PST_INIT;
          break;
        }
//  Serial.println("crc3");
        *state=TSCR_PST_READY;
        return(TSCR_PST_READY);
      default:
        *state=TSCR_PST_INIT;
        break;  
    }
  }
  
  return(rval);
}

int comm_unpack1(unsigned char *d, unsigned int l, unsigned char *buf, unsigned int *len)
{
  memcpy((void*)d,(void*)(buf+*len),l);
  (*len)+=l;
  return(0);
}

int comm_unpackuccb(unsigned char *buf, unsigned long len, 
                    unsigned long *commpkt_counter,
                    int *battV,
                    int *tsX,
                    int *tsY,
                    int *fsX,
                    int *fsY,
                    int *fsZ,
                    int *fsBS,
                    int *fsBE,
                    int *stb,
                    int *b6pBS,
                    int *b6pBE,
                    int *m1s,
                    int *m2s,
                    int *rdd,
                    int *tsx,
                    int *tsy,
                    uint16_t *m1rpm,
                    uint16_t *m2rpm,
                    char *tscrtakeover)
{
  unsigned int l;
  
  l=1;

  comm_unpack1((unsigned char *)commpkt_counter,sizeof(unsigned long),buf,&l);
  comm_unpack1((unsigned char *)battV,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)tsX,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)tsY,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)fsX,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)fsY,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)fsZ,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)fsBS,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)fsBE,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)stb,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)b6pBS,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)b6pBE,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)m1s,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)m2s,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)rdd,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)tsx,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)tsy,sizeof(int),buf,&l);
  comm_unpack1((unsigned char *)m1rpm,sizeof(uint16_t),buf,&l);
  comm_unpack1((unsigned char *)m2rpm,sizeof(uint16_t),buf,&l);
  comm_unpack1((unsigned char *)tscrtakeover,sizeof(char),buf,&l);

  return(0);
}

int comm_recv(void)
{
  int ret,stb,pwr0,poslight;

  ret=comm_read(&g_rts_state,g_rts_commbuf,&g_rts_len);
  
  if(ret == TSCR_PST_READY) {
    comm_unpackuccb(g_rts_commbuf,g_rts_len,
                    &g_cb_w_commpkt_counter,
                    &g_cb_battV,
                    &g_cb_tsX,
                    &g_cb_tsY,
                    &g_cb_fsX,
                    &g_cb_fsY,
                    &g_cb_fsZ,
                    &g_cb_fsBS,
                    &g_cb_fsBE,
                    &stb,
                    &g_cb_b6pBS,
                    &g_cb_b6pBE,
                    &g_cb_mLs,
                    &g_cb_mRs,
                    &g_cb_rdd,
                    &g_cb_tsxp,
                    &g_cb_tsyp,
                    &g_sh1_mLrpm,
                    &g_sh1_mRrpm,
                    &g_tscr_takeover);

    g_cb_sw10p=stb&UCCB_ST_SW10P;
    if((int)(stb&UCCB_ST_M1) == 0) g_cb_mLs=0;
    if((int)(stb&UCCB_ST_M2) == 0) g_cb_mRs=0;
    poslight=(int)((stb&UCCB_ST_POSLIGHT)>>UCCB_PL_STPOS);

    if(((g_cb_mLs != 0) || (g_cb_mRs != 0) || (g_cb_rdd != 0)) &&
       ((g_tscr_power != 0) || (g_tscr_rudder != 0)) &&
       (g_tscr_takeover == 0)){
      g_tscr_power=0;
      g_tscr_rudder=0;
      drawWheel(WHEEL_X0,WHEEL_Y0,90,WHEEL_R1,WHEEL_R2,WHEEL_A1,WHEEL_B,g_dsp_background,VGA_GREEN);
      pwr0=POWER_Y0-POWER_B/3;
      drawPower(POWER_X0,POWER_Y0,POWER_A,POWER_B,pwr0,g_dsp_background,g_tscr_power_color);
    }

    if(g_cb_poslight == go_cb_poslight) {
      if(poslight != g_cb_poslight) {
        g_cb_poslight=poslight;
        go_cb_poslight=poslight;
        draw_poslight(g_cb_poslight,0);
      }
    } else {
      if(g_cb_poslight == poslight) {
        go_cb_poslight=poslight;
      }
    }
  
    return(1);
  }

  return(0);
}

int tscr_comm(void)
{
  comm_send();
  comm_recv();

  return(0);
}
