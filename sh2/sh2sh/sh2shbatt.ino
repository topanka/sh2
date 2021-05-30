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
  long a;

  if((g_millis-l_brtv) >= 500) {
    l_bv=analogRead(UCCB_BATTV_PORT);
    ra_bv.addValue(l_bv);
    l_bv=round(ra_bv.getAverage()+0.5);
    l_brtv=g_millis;
  }
  if((g_millis-l_brta) >= g_batt_read_tmo) {
    l_ba=analogRead(UCCB_BATTA_PORT);
    if(l_ba >= 100) {
      a=ra_ba.getAverage();
      if(l_ba > a+10) {
        l_ba=a+10;
      } else if(l_ba < a-10) {
        if(a > 10) {
          l_ba=a-10;
        }
      }
      ra_ba.addValue(l_ba);
    } else {
      ra_ba.addValue(100);
    }
    l_ba=round(ra_ba.getAverage()+0.5);
    l_brta=g_millis;
    batt_curr_cutoff(l_ba);
  }
  
  *battV=l_bv;
  *battA=l_ba;
  
  return(0);
}


void batt_curr_cutoff(int ba)
{
  float voltage;
  static int l_limit=0;

  voltage=(5.0/1023.0)*ba;
  voltage=voltage-g_ACS770_QOV+0.015;

  if(voltage > g_ACS770_cutoff) {
    l_limit++;
    g_batt_read_tmo=10;
  } else {
    l_limit=0;
    g_batt_read_tmo=25;
    g_batt_cutoff_reached=0;
  }

  if(l_limit > 3) {
    g_batt_cutoff_reached=1;
    Serial.print("cutOff: ");
    Serial.print(g_ACS770_cutoff);
    Serial.println(" reached");
  }
}

void totcurr(void)
{
  float voltage;
  float current;
  int l_limit=0;

  voltage=(5.0/1023.0)*g_battA;
  voltage=voltage-g_ACS770_QOV+0.015;
  current=voltage/g_ACS770_FACTOR;

  Serial.print("totA:");
  Serial.print(g_battA);
  Serial.print(" ");
  Serial.print(voltage);
  Serial.print(" ");
  Serial.print(current);
  Serial.print(" ");
  Serial.print(g_ACS770_cutoff);
  Serial.print(" ");
  Serial.println();
  
}
