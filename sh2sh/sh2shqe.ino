extern volatile unsigned long timer0_millis;

static int8_t lookup_table[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};

//tbi - time between interrupts
 
volatile uint8_t g_qe_num_cnt=4;
volatile uint8_t g_qe_num_tbi=10;

volatile uint8_t g_qe_rpm1=0;
volatile int8_t g_qe_dir1=0;
volatile uint8_t g_qe_rpm2=0;
volatile int8_t g_qe_dir2=0;

volatile unsigned long g_qe_tbi1=1;
volatile unsigned long g_qe_tbi2=1;

 void qe_setup()
 {
//   Serial.begin(115200);
//   attachInterrupt(2, rpm_fun, RISING);

//   attachInterrupt(2, rpm_fun1, CHANGE);
//   attachInterrupt(3, rpm_fun2, CHANGE);
   
   attachInterrupt(2, encoder_isr1, CHANGE);
   attachInterrupt(3, encoder_isr1, CHANGE);

   attachInterrupt(4, encoder_isr2, CHANGE);
   attachInterrupt(5, encoder_isr2, CHANGE);

 }
 
uint16_t qe_rpm1_cnt(void)
{
  static uint8_t l_idx=0;
  static uint16_t l_rpm=0;
  static unsigned long t0=0;
  static unsigned long l_tms[100]={0};
  static uint8_t l_cnt[100]={0};
  static uint16_t l_sum=0;
  unsigned long t1,dt;
  uint8_t r,in;

  t1=g_millis;
  if((t1-t0) < 50) return(l_rpm);
  t0=t1;
  
  noInterrupts();
  r=g_qe_rpm1;
  interrupts();
  g_qe_rpm1=0;    //elvileg az int engedelyezese utan itt kapok meg egy utasitast
  
  l_sum+=r;
  
  if(r == 0) {
    l_sum=0;
    l_cnt[0]=0;
    l_tms[0]=t1;
    l_idx=0;
    l_rpm=0;
    return(l_rpm);
  }
  
  l_idx++;
  l_idx%=g_qe_num_cnt;
  l_tms[l_idx]=t1;
  l_cnt[l_idx]=r;
  if(l_cnt[0] == 0) {
    dt=l_tms[l_idx]-l_tms[0];
  } else {
    in=(l_idx+1)%g_qe_num_cnt;
    l_sum-=l_cnt[in];
    dt=l_tms[l_idx]-l_tms[in];
  }
  
/*  
  Serial.print("1b ");
  Serial.print(r,DEC);
  Serial.print(" ");
  Serial.print(l_tms[0],DEC);
  Serial.print(" ");
  Serial.print(dt,DEC);
  Serial.print(" ");
  Serial.print(tbi1,DEC);
  Serial.print(" ");
  Serial.println(l_sum,DEC);
 */ 
  
  if(dt == 0) return(l_rpm);
  l_rpm=(l_sum*1000UL)/dt;
  return(l_rpm);
}

uint16_t qe_rpm1_tbi(void)
{
  static uint8_t l_idx=0,l_r0=0;
  static uint16_t l_rpm=0;
  static uint16_t l_tbi[100]={0};
//  static uint16_t l_sum=0;
  static unsigned long l_sum=0;
  static unsigned long l_t0=0;
  unsigned long dt;
  uint16_t dtx,atbi;

/*
  Serial.print("xx ");
  Serial.print(l_r0,DEC);
  Serial.print(" ");
  Serial.print(g_qe_rpm1,DEC);
  Serial.print(" ");
  Serial.print(l_t0,DEC);
  Serial.print(" ");
  Serial.println(g_millis,DEC);
*/

  if(l_r0 == g_qe_rpm1) {
    if(g_millis > l_t0+70) {
      l_sum=0;
      l_tbi[0]=0;
      l_idx=0;
      l_rpm=0;
      return(l_rpm);
    } else if(g_millis > l_t0+50) {
      dtx=8000;
    } else {
      return(l_rpm);
    }
  } else {
    l_r0=g_qe_rpm1;
    l_t0=g_millis;
    
    noInterrupts();
    dt=g_qe_tbi1;
    interrupts();
    
    dtx=dt>>3;
  }

  if(dtx > 10000) {
     l_sum=0;
     l_tbi[0]=0;
     l_idx=0;
     l_rpm=0;
     return(l_rpm);
  }
  l_sum+=dtx;
  
  l_idx++;
  l_idx%=g_qe_num_tbi;
  if(l_tbi[0] == 0) {
    if(l_idx == 0) {
      atbi=l_sum/g_qe_num_tbi;
    } else {
      atbi=l_sum/l_idx;
    }
  } else {
    l_sum-=l_tbi[l_idx];
    atbi=l_sum/g_qe_num_tbi;
  }
  l_tbi[l_idx]=dtx;
  
/*  
  Serial.print("1b ");
  Serial.print(dt,DEC);
  Serial.print(" ");
  Serial.print(dtx,DEC);
  Serial.print(" ");
  Serial.print(l_sum,DEC);
  Serial.print(" ");
  Serial.println(atbi,DEC);
*/  

  l_rpm=125000UL/atbi;
  return(l_rpm);
}


uint16_t qe_rpm2_tbi(void)
{
  static uint8_t l_idx=0,l_r0=0;
  static uint16_t l_rpm=0;
  static uint16_t l_tbi[100]={0};
//  static uint16_t l_sum=0;
  static unsigned long l_sum=0;
  static unsigned long l_t0=0;
  unsigned long dt;
  uint16_t dtx,atbi;

/*
  Serial.print("xx ");
  Serial.print(l_r0,DEC);
  Serial.print(" ");
  Serial.print(g_qe_rpm1,DEC);
  Serial.print(" ");
  Serial.print(l_t0,DEC);
  Serial.print(" ");
  Serial.println(g_millis,DEC);
*/

  if(l_r0 == g_qe_rpm2) {
    if(g_millis > l_t0+70) {
      l_sum=0;
      l_tbi[0]=0;
      l_idx=0;
      l_rpm=0;
      return(l_rpm);
    } else if(g_millis > l_t0+50) {
      dtx=8000;
    } else {
      return(l_rpm);
    }
  } else {
    l_r0=g_qe_rpm2;
    l_t0=g_millis;
    
    noInterrupts();
    dt=g_qe_tbi2;
    interrupts();
    
    dtx=dt>>3;
  }

  if(dtx > 10000) {
     l_sum=0;
     l_tbi[0]=0;
     l_idx=0;
     l_rpm=0;
     return(l_rpm);
  }
  l_sum+=dtx;
  
  l_idx++;
  l_idx%=g_qe_num_tbi;
  if(l_tbi[0] == 0) {
    if(l_idx == 0) {
      atbi=l_sum/g_qe_num_tbi;
    } else {
      atbi=l_sum/l_idx;
    }
  } else {
    l_sum-=l_tbi[l_idx];
    atbi=l_sum/g_qe_num_tbi;
  }
  l_tbi[l_idx]=dtx;
  
/*  
  Serial.print("1b ");
  Serial.print(dt,DEC);
  Serial.print(" ");
  Serial.print(dtx,DEC);
  Serial.print(" ");
  Serial.print(l_sum,DEC);
  Serial.print(" ");
  Serial.println(atbi,DEC);
*/  

  l_rpm=125000UL/atbi;
  return(l_rpm);
}

//#if 0
 void qe_doit(void)
{
   g_rpm_m2=qe_rpm1_tbi();
   g_rpm_m1=qe_rpm2_tbi();
   g_dir_m2=g_qe_dir1;
   g_dir_m1=g_qe_dir2;
}
//#endif

#if 0
 void qe_doit(void)
{
  uint16_t rpm1=0,rpm2=0;
  static uint16_t rpm1o=0,rpm2o=0;

   rpm1=qe_rpm1_tbi();
   rpm2=qe_rpm2_tbi();
   
   if((rpm1 != rpm1o) || (rpm2 != rpm2o)) {
     Serial.print("1a ");
     Serial.print(micros(),DEC);
     Serial.print(" ");
     Serial.print(rpm1,DEC);
     Serial.print(" ");
     Serial.println(rpm2,DEC);
     rpm1o=rpm1;
     rpm2o=rpm2;
   }
   
/*  
   rpm1=qe_rpm1();
   rpm2=qe_rpm2();
   
   if((rpm1 != rpm1o) || (rpm2 != rpm2o)) {
     Serial.print("1a ");
     Serial.print(rpm1,DEC);
     Serial.print(" ");
     Serial.println(rpm2,DEC);
     rpm1o=rpm1;
     rpm2o=rpm2;
   }
*/   
   
/*   
   if(xx != g_qe_rpm1) {
     Serial.print("1c ");
     Serial.println(g_qe_rpm1,DEC);
     xx=g_qe_rpm1;
   }
*/   
   
}
#endif

void encoder_isr1()
{
  static uint8_t enc_val=0;
  static unsigned long l_t0=0;
  unsigned long mm;
  uint8_t ei;
  
  enc_val = enc_val << 2;
  enc_val = enc_val | (~PIND & 0b0011);
  ei=enc_val & 0b1111;
  g_qe_dir1=lookup_table[ei];
  if((ei != 0b0001) && (ei != 0b0010)) return;
  g_qe_rpm1++;

/*  
  g_qe_tbi1=g_millis-l_t0;
  l_t0=g_millis;
*/  

  mm=micros();
  g_qe_tbi1=mm-l_t0;
  l_t0=mm;
  
}
    
void encoder_isr2()
{
  static uint8_t enc_val=0;
  static unsigned long l_t0=0;
  uint8_t ei;
  unsigned long mm;
  
  enc_val = enc_val << 2;
  enc_val = enc_val | ((~PIND & 0b1100) >> 2);
  ei=enc_val & 0b1111;
  g_qe_dir2=lookup_table[ei];
  if((ei != 0b0100) && (ei != 0b1000)) return;
  g_qe_rpm2++;
  mm=micros();
  g_qe_tbi2=mm-l_t0;
  l_t0=mm;
}
