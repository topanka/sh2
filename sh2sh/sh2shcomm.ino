byte g_w_commbuf[100]={0};
unsigned long g_w_sendtime=0;
unsigned long g_w_commpkt_counter=0;        //total counter of sent packets

byte g_r_commbuf[100]={0};
int g_r_state=UCCB_PST_INIT;
unsigned int g_r_len=0;
byte g_commmode=1;
unsigned long g_commmode_t=0;

int comm_setup(void)
{
//  Serial2.begin(9600);
  Serial2.begin(19200);
  buildCRCTable();

  return(0);
}

int comm_pack1(byte *d, uint16_t l, byte *buf, uint16_t *len)
{
  if((*len+l) > sizeof(g_w_commbuf)) return(-1);
  memcpy((void*)(buf+*len),(void*)d,l);
  (*len)+=l;
  return(0);
}

int comm_packsh1(uint16_t *len)
{
  byte lead=UCCB_SHIP_LEAD;
  byte crc8;
  unsigned int m1c=0,m2c=0;
  int16_t rpm1,rpm2;
  uint8_t mstate;
  
  *len=0;
  
  md_getmc(&m1c,&m2c);
  if(g_dir_m1 >= 0) rpm1=(int16_t)g_rpm_m1;
  else rpm1=-(int16_t)g_rpm_m1;
  if(g_dir_m2 >= 0) rpm2=(int16_t)g_rpm_m2;
  else rpm2=-(int16_t)g_rpm_m2;
  mstate=g_state_ml+(g_state_mr<<4);

/*
  Serial.print(rpm1);
  Serial.print(" ");
  Serial.println(rpm2);
*/  
  
  comm_pack1((byte*)&lead,sizeof(lead),g_w_commbuf,len);      //1:1
//  comm_pack1((byte*)&g_w_commpkt_counter,sizeof(g_w_commpkt_counter),g_w_commbuf,len);    //4:5
  comm_pack1((byte*)&g_loop_cps,sizeof(g_loop_cps),g_w_commbuf,len);    //4:5
  comm_pack1((byte*)&g_battV,sizeof(g_battV),g_w_commbuf,len);    //2:7
  comm_pack1((byte*)&g_battA,sizeof(g_battA),g_w_commbuf,len);    //2:9
  comm_pack1((byte*)&m1c,sizeof(m1c),g_w_commbuf,len);    //2:11
  comm_pack1((byte*)&m2c,sizeof(m2c),g_w_commbuf,len);    //2:13
  comm_pack1((byte*)&rpm1,sizeof(rpm1),g_w_commbuf,len);    //2:15
  comm_pack1((byte*)&rpm2,sizeof(rpm2),g_w_commbuf,len);    //2:17
  comm_pack1((byte*)&g_temperature,sizeof(g_temperature),g_w_commbuf,len);    //2:19
  comm_pack1((byte*)&mstate,sizeof(mstate),g_w_commbuf,len);    //2:20
  crc8=getCRC(g_w_commbuf,*len);
  comm_pack1((byte*)&crc8,sizeof(crc8),g_w_commbuf,len);    //1:21
  
//21 byte long  
  
  return(0);
}

int comm_send(void)
{
  uint16_t len;

//  g_w_commpkt_counter++;
  
//csakhogy ne kuggyon soha  
//g_commmode=1;

  if(g_commmode != 0) return(0);
  
/*  
  Serial.print("commmode ");
  Serial.println(g_commmode);
*/

  g_w_commpkt_counter++;
  
  comm_packsh1(&len);
  
//  delay(50);
  
  Serial2.write((byte*)&g_w_commbuf[0],len);
  
//  g_w_commpkt_counter=0;
  g_commmode=1;

  return(1);
}

int comm_read(int *state, unsigned char *buf, unsigned int *len)
{
  int rval=-1,ret,nr=0;
  unsigned char c1;
  unsigned char crc8;
//  static unsigned long xx=0;

  while(Serial2.available()) {
    c1=(unsigned char)Serial2.read();
    
    if(++nr >= 100) break;
    switch(*state) {
      case UCCB_PST_INIT:
      case UCCB_PST_READY:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" init/ready");
*/  
        if(c1 == UCCB_CBOX_LEAD) {
          *len=0;
          buf[*len]=c1;
          *state=UCCB_PST_DATA;
    	  } else {
          *state=UCCB_PST_INIT;
        }
	      break;
      case UCCB_PST_DATA:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" data");
*/  
        (*len)++;
        buf[*len]=c1;
        if(*len == UCCB_CBOX_PKTLAST) {
          *state=UCCB_PST_CRC;
        }
        break;
      case UCCB_PST_CRC:
/*      
  Serial.print(*len);
  Serial.print(" ");
  Serial.print(c1);
  Serial.println(" crc1");
*/  
        if(*len != UCCB_CBOX_PKTLAST) {
          *state=UCCB_PST_INIT;
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
          *state=UCCB_PST_INIT;
          break;
        }
//  Serial.println("crc3");
        *state=UCCB_PST_READY;
        return(UCCB_PST_READY);
      default:
        *state=UCCB_PST_INIT;
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
                    byte *commmode)
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
  comm_unpack1((unsigned char *)commmode,sizeof(byte),buf,&l);

  return(0);
}

int comm_recv(void)
{
  int ret,stb;

  g_recv_ready=0;
  if(g_commmode == 0) return(0);
  
  ret=comm_read(&g_r_state,g_r_commbuf,&g_r_len);
  
  if(ret == UCCB_PST_READY) {
    comm_unpackuccb(g_r_commbuf,g_r_len,
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
                    &g_cb_mls,
                    &g_cb_m2s,
                    &g_cb_rdd,
                    &g_cb_tsxp,
                    &g_cb_tsyp,
                    &g_commmode);

    g_cb_sw10p=stb&UCCB_ST_SW10P;
    if((int)(stb&UCCB_ST_M1) == 0) g_cb_mls=0;
    if((int)(stb&UCCB_ST_M2) == 0) g_cb_m2s=0;
    g_cb_lightpos=(int)((stb&UCCB_ST_POSLIGHT)>>UCCB_PL_STPOS);
    
    g_recv_ready=1;
//    Serial.println("packet read");
    return(1);
  }

  return(0);
}
