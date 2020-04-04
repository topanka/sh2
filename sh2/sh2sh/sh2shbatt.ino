RunningAverage ra_bv(5);
RunningAverage ra_ba(7);

int batt_setup(void)
{
  int i,v,avn;
  
  avn=7;
  
//fill the buffer  
  for(i=0;i < avn;i++) {
    v=analogRead(UCCB_BATTV_PORT);
    ra_bv.addValue(v);
  }
  
  avn=11;
  
//fill the buffer  
  for(i=0;i < avn;i++) {
    v=analogRead(UCCB_BATTA_PORT);
    ra_ba.addValue(v);
  }
  
  return(0);
}

int batt_read(int *battV, int *battA)
{
  static unsigned long l_brtv=0,l_brta=0;
  static int l_bv=0;
  static int l_ba=0;

  if((g_millis-l_brtv) >= 500) {
    l_bv=analogRead(UCCB_BATTV_PORT);
    ra_bv.addValue(l_bv);
    l_bv=round(ra_bv.getAverage()+0.5);
    l_brtv=g_millis;
  }
  if((g_millis-l_brta) >= 30) {
    l_ba=analogRead(UCCB_BATTA_PORT);
    ra_ba.addValue(l_ba);
    l_ba=round(ra_ba.getAverage()+0.5);
    l_brta=g_millis;
  }
  
  *battV=l_bv;
  *battA=l_ba;
  
  return(0);
}

void totcurr(void)
{
  float FACTOR=40.0/1000.0;
  float QOV=0.5;
  float cutOffLimit=1.0;    //1.0 is 1A
  float cutOff=FACTOR/cutOffLimit;
  float voltage;
  float current;

//return(0);
  
  voltage=(5.0/1023.0)*g_battA;
  voltage=voltage-QOV+0.015;
  current=voltage/FACTOR;
  Serial.print("totA:");
  Serial.print(g_battA);
  Serial.print(" ");
  Serial.print(voltage);
  Serial.print(" ");
  Serial.print(current);
  Serial.print(" ");
  Serial.println();
}
