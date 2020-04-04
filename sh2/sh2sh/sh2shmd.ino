//left-1, right-2

#define SH1_MOTORCURRENT_MIN      300

struct MD18V25 g_mdl={0};
struct MD18V25 g_mdr={0};

RunningAverage m1cra(11);
RunningAverage m2cra=(11);

void MD18V25_calibrate_vzcr(struct MD18V25 *md)
{
  unsigned int i,vzcr;

  vzcr=0;
  for(i=0;i < 11;i++) {
    vzcr+=analogRead(md->acs709_vzcr_pin);
  }
  md->acs709_vzcr=vzcr/11;
  md->acs709_vzcr+=6;  

  return;
}

int MD18V25_init(struct MD18V25 *mdl, struct MD18V25 *mdr)
{
  mdl->dir_pin=10;
  mdl->pwm_pin=7;
  mdl->_OCR=&OCR4B;
  mdl->acs709_viout_pin=SH2SH_ACS709_VIOUTL_PORT;
  mdl->acs709_vzcr_pin=SH2SH_ACS709_VZCRL_PORT;
  mdl->ff2_pin=42;
  mdl->ff1_pin=43;
  mdl->reset_pin=47;
  
  mdr->dir_pin=9;
  mdr->pwm_pin=6;
  mdr->_OCR=&OCR4A;
  mdr->acs709_viout_pin=SH2SH_ACS709_VIOUTR_PORT;
  mdr->acs709_vzcr_pin=SH2SH_ACS709_VZCRR_PORT;
  mdr->ff2_pin=30;
  mdr->ff1_pin=31;
  mdr->reset_pin=32;
  
  pinMode(mdl->dir_pin,OUTPUT);
  pinMode(mdl->pwm_pin,OUTPUT);
  pinMode(mdl->ff1_pin,INPUT);
  pinMode(mdl->ff2_pin,INPUT);
  pinMode(mdr->dir_pin,OUTPUT);
  pinMode(mdr->pwm_pin,OUTPUT);
  pinMode(mdr->ff1_pin,INPUT);
  pinMode(mdr->ff2_pin,INPUT);

/*  
  TCCR1A=0b10100000;
  TCCR1B=0b00010001;
  ICR1=400;
*/
  
  TCCR4A = 0b10101000;
  TCCR4B = 0b00010001;
  ICR4 = 400;

  MD18V25_calibrate_vzcr(mdl);
  MD18V25_calibrate_vzcr(mdr);

Serial.print(mdl->acs709_vzcr);
Serial.print(" ");
Serial.print(mdr->acs709_vzcr);
Serial.println();

  return(0);  
}

int MD18V25_getstate(struct MD18V25 *md)
{
  int s=0,ff1,ff2;
  
  ff1=digitalRead(md->ff1_pin);
  ff2=digitalRead(md->ff2_pin);
  if(ff1 == HIGH) s+=1;
  if(ff2 == HIGH) s+=2;

  return(s);
}

int MD18V25_setpwr(struct MD18V25 *md, int pwr)
{
  uint16_t pwr0;

  if(pwr >= 0) {
    digitalWrite(md->dir_pin,HIGH);
    *md->_OCR=(uint16_t)pwr;
  } else {
    digitalWrite(md->dir_pin,LOW);
    *md->_OCR=(uint16_t)(-pwr);
  }
  return(0);
}

void md_setup(void)
{
  MD18V25_init(&g_mdl,&g_mdr);
  
  tmr_init(&g_tmr_checkmc,10);
}

void md_calibrate_vzcr(void)
{
  MD18V25_calibrate_vzcr(&g_mdl);
  MD18V25_calibrate_vzcr(&g_mdr);

Serial.print(g_mdl.acs709_vzcr);
Serial.print(" ");
Serial.print(g_mdr.acs709_vzcr);
Serial.println();
  
}

void md_get_state(void)
{
  int s1,s2;
  
  s1=MD18V25_getstate(&g_mdl);
  s2=MD18V25_getstate(&g_mdr);

Serial.print("state: ");
Serial.print(s1);
Serial.print(" ");
Serial.print(s2);
Serial.println();
  
}

int md_getmc(unsigned int *m1c, unsigned int *m2c)
{
  if(m1c != NULL) {
    *m1c=m1cra.getAverage();
    if((*m1c < SH1_MOTORCURRENT_MIN) && (g_rpm_m1 == 0)) {
      *m1c=0;
    }
  }
  if(m2c != NULL) {
    *m2c=m2cra.getAverage();
    if((*m2c < SH1_MOTORCURRENT_MIN) && (g_rpm_m2 == 0)) {
      *m2c=0;
    }
  }

/*
  Serial.print("mc: ");
  Serial.print(*m1c);
  Serial.print(" ");
  Serial.print(*m2c);
  Serial.println();
*/  
  return(0);
}

int md_checkmc(void)
{
  unsigned int mc;
  long a;

  if(tmr_do(&g_tmr_checkmc) != 1) return(0);
  
  acs709_get_mA(g_mdl.acs709_viout_pin,g_mdl.acs709_vzcr,&mc);
//  Serial.print(mc);
//  Serial.print(" ");
  
  a=m1cra.getAverage();
  if(mc > a+500) {
    mc=a+500;
  } else if(mc < a-500) {
    if(a > 500) {
      mc=a-500;
    } else {
  //    mc=0;
    }
  }
  if((g_rpm_m1 == 0) || (mc > 0)) {
    m1cra.addValue(mc);
  }
  
  acs709_get_mA(g_mdr.acs709_viout_pin,g_mdr.acs709_vzcr,&mc);
//  Serial.print(mc);
//  Serial.println();
  if((g_rpm_m2 == 0) || (mc > 0)) {
    m2cra.addValue(mc);
  }
  return(1);
}

int md_setspeed(void)
{

   if(g_recv_ready != 1) return(0);
  
//g_cb_m1s=48;  
//g_cb_m1s=0;  
//g_cb_m2s=45;  
//g_cb_m2s=0;  
  
/*
if((g_cb_mls != 0) || (g_cb_m2s != 0)) {  
Serial.print(g_cb_mls);
Serial.print(" ");
Serial.println(g_cb_m2s);
}
*/
  MD18V25_setpwr(&g_mdl,g_cb_mls);
  MD18V25_setpwr(&g_mdr,g_cb_m2s);

  return(0);
}
