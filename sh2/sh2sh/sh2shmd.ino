//left-1, right-2

#define SH2_MOTORCURRENT_MIN      300
//#define SH2_MOTORSPEED_LIMIT      60

struct MD18V25 g_mdl={0};
struct MD18V25 g_mdr={0};

RunningAverage g_mcral(11);
RunningAverage g_mcrar=(11);

void MD18V25_calibrate_vzcr(struct MD18V25 *md)
{
  unsigned int i,vzcr;

  vzcr=0;
  for(i=0;i < 11;i++) {
    vzcr+=analogRead(md->acs709_vzcr_pin);
  }
  md->acs709_vzcr=vzcr/11;
//  md->acs709_vzcr+=6;  

Serial.print("VZCR: ");
Serial.println(md->acs709_vzcr);

  return;
}

void MD18V25_calibrate_vzcr_offset(struct MD18V25 *md)
{
  unsigned int i;
  int viout,vzcr;

  viout=0;
  vzcr=0;
  for(i=0;i < 11;i++) {
    viout+=analogRead(md->acs709_viout_pin);
    vzcr+=analogRead(md->acs709_vzcr_pin);
  }
  md->acs709_vzcr_offset=(viout-vzcr)/11;

Serial.print("VZCR offset: ");
Serial.println(md->acs709_vzcr_offset);

  md->acs709_vzcr+=(md->acs709_vzcr_offset+2);

  return;
}

int MD18V25_reset(struct MD18V25 *mdl, struct MD18V25 *mdr)
{
  digitalWrite(mdl->reset_pin,LOW);      //reset left motor driver
  digitalWrite(mdr->reset_pin,LOW);      //reset right motor driver
  delay(100);
  digitalWrite(mdl->reset_pin,HIGH);      //enable left motor driver
  digitalWrite(mdr->reset_pin,HIGH);      //enable right motor driver

  return(0);
}

int MD18V25_init(struct MD18V25 *mdl, struct MD18V25 *mdr)
{
  mdl->dir_pin=10;
  mdl->pwm_pin=7;
  mdl->_OCR=&OCR4B;
  mdl->acs709_viout_pin=SH2SH_ACS709_VIOUTL_PORT;
  mdl->acs709_vzcr_pin=SH2SH_ACS709_VZCRL_PORT;
  mdl->ff2_pin=30;
  mdl->ff1_pin=31;
  mdl->reset_pin=33;
  mdl->mcra=&g_mcral;
  mdl->curr_cutoff_reached=0;
  mdl->overloaded=0;
  
  mdr->dir_pin=9;
  mdr->pwm_pin=6;
  mdr->_OCR=&OCR4A;
  mdr->acs709_viout_pin=SH2SH_ACS709_VIOUTR_PORT;
  mdr->acs709_vzcr_pin=SH2SH_ACS709_VZCRR_PORT;
  mdr->ff2_pin=42;
  mdr->ff1_pin=43;
  mdr->reset_pin=47;   //right pislog
  mdr->mcra=&g_mcrar;
  mdr->curr_cutoff_reached=0;
  mdr->overloaded=0;
  
  pinMode(mdl->dir_pin,OUTPUT);
  pinMode(mdl->pwm_pin,OUTPUT);
  pinMode(mdl->reset_pin,OUTPUT);
  pinMode(mdl->ff1_pin,INPUT);
  pinMode(mdl->ff2_pin,INPUT);
  pinMode(mdr->dir_pin,OUTPUT);
  pinMode(mdr->pwm_pin,OUTPUT);
  pinMode(mdr->reset_pin,OUTPUT);
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

  MD18V25_calibrate_vzcr_offset(mdl);
  MD18V25_calibrate_vzcr_offset(mdr);

/*
  Serial.print(mdl->acs709_vzcr);
  Serial.print(" ");
  Serial.print(mdr->acs709_vzcr);
  Serial.println();
*/

  MD18V25_reset(mdl,mdr);

  return(0);  
}

int8_t MD18V25_getstate(struct MD18V25 *md)
{
  int ff1,ff2;
  uint8_t s=0;
  
  ff1=digitalRead(md->ff1_pin);
  ff2=digitalRead(md->ff2_pin);
  if(ff1 == HIGH) s+=1;
  if(ff2 == HIGH) s+=2;

  return(s);
}

int MD18V25_setpwr(struct MD18V25 *md, int pwr)
{
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

/*
Serial.print(g_mdl.acs709_vzcr);
Serial.print(" ");
Serial.print(g_mdr.acs709_vzcr);
Serial.println();
*/
  
}

void md_calibrate_vzcr_offset(void)
{
  MD18V25_calibrate_vzcr_offset(&g_mdl);
  MD18V25_calibrate_vzcr_offset(&g_mdr);

/*
Serial.print(g_mdl.acs709_vzcr);
Serial.print(" ");
Serial.print(g_mdr.acs709_vzcr);
Serial.println();
*/
  
}

void md_get_state(void)
{
  g_state_ml=MD18V25_getstate(&g_mdl);
  g_state_mr=MD18V25_getstate(&g_mdr);

/*
  if(g_millis%30000 < 4000) g_state_mr=1;
  else if(g_millis%30000 < 8000) g_state_mr=2;
  else if(g_millis%30000 < 12000) g_state_mr=3;
  else if(g_millis%30000 < 16000) g_state_ml=1;
  else if(g_millis%30000 < 20000) {
    g_state_ml=2;
    g_state_mr=3;
  }
*/
/*
Serial.print("state: ");
Serial.print(s1);
Serial.print(" ");
Serial.print(s2);
Serial.println();
*/
  
}

int md_getmc(unsigned int *mcl, unsigned int *mcr)
{
  if(mcl != NULL) {
    *mcl=g_mdl.mcra->getAverage();
    if((*mcl < SH2_MOTORCURRENT_MIN) && (g_rpm_ml == 0)) {
      *mcl=0;
    }
  }
  if(mcr != NULL) {
    *mcr=g_mdr.mcra->getAverage();
    if((*mcr < SH2_MOTORCURRENT_MIN) && (g_rpm_mr == 0)) {
      *mcr=0;
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

int md_checkmc1(struct MD18V25 *md, uint16_t rpm)
{
  unsigned int mc;
  long a;

//  md_get_state();
  
  acs709_get_mA(md->acs709_viout_pin,md->acs709_vzcr,&mc);
//  acs709_get_mA(md->acs709_viout_pin,md->acs709_vzcr_pin,&mc);
//  Serial.print(mc);
//  Serial.print(" ");
  if(mc >= g_md_curr_cutoff) {
    if(md->curr_cutoff_reached < 3) {
//Serial.print("=== motor cutoff: ");
//Serial.println(mc);
      md->curr_cutoff_reached++;
    } else {
Serial.print("=== motor cutoff: ");
Serial.println(mc);
      md->overloaded=1;
    }
  } else {
//    if(g_cb_md_stop == 1) {
      md->curr_cutoff_reached=0;
//    }
  }
  
  a=md->mcra->getAverage();
  if(mc > a+500) {
    mc=a+500;
  } else if(mc < a-500) {
    if(a > 500) {
      mc=a-500;
    }
  }
  if((rpm == 0) || (mc > 0)) {
    md->mcra->addValue(mc);
  }
  
  return(0);
}

int md_checkmc(void)
{
  if(tmr_do(&g_tmr_checkmc) != 1) return(0);
  
  md_get_state();
  
  md_checkmc1(&g_mdl,g_rpm_ml);
  md_checkmc1(&g_mdr,g_rpm_mr);

  return(1);
}

int md_setspeed(void)
{

//g_cb_m1s=48;  
//g_cb_m1s=0;  
//g_cb_m2s=45;  
//g_cb_m2s=0;  
  
/*
if((g_cb_msl != 0) || (g_cb_msr != 0)) {  
Serial.print("md setspeed ");
Serial.print(g_cb_msl);
Serial.print(" ");
Serial.println(g_cb_msr);
}
*/


  if((g_batt_cutoff_reached == 0) && (g_mdl.overloaded == 0) && (g_mdr.overloaded == 0)) {
    if(g_recv_ready != 1) {
      if((g_millis-g_recv_ready_time) > 1000) {
        g_cb_msl=0;
        g_cb_msr=0;
      } else {
        return(0);
      }
    }
  } else {
    if(g_cb_md_stop == 1) {
      g_mdl.overloaded=0;
      g_mdr.overloaded=0;
    } else {
      g_cb_msl=0;
      g_cb_msr=0;
    }

/*    
    if(g_cb_msl > SH2_MOTORSPEED_LIMIT) {
      g_cb_msl=SH2_MOTORSPEED_LIMIT;
    } else if(g_cb_msl < -SH2_MOTORSPEED_LIMIT) {
      g_cb_msl=-SH2_MOTORSPEED_LIMIT;
    }
    if(g_cb_msr > SH2_MOTORSPEED_LIMIT) {
      g_cb_msr=SH2_MOTORSPEED_LIMIT;
    } else if(g_cb_msr < -SH2_MOTORSPEED_LIMIT) {
      g_cb_msr=-SH2_MOTORSPEED_LIMIT;
    }
*/    
  }

  MD18V25_setpwr(&g_mdl,-g_cb_msl);
  MD18V25_setpwr(&g_mdr,g_cb_msr);

  return(0);
}

int md_reset(void)
{
  if(g_cb_mdreset != 1) return(0);

  MD18V25_reset(&g_mdl,&g_mdr);
  g_cb_mdreset=0;
  
  Serial.println("md reset completed");

  return(0);
}
