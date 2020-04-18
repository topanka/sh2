int acs709_setup(void)
{
  tmr_init(&g_tmr_acs709_s1,30);
  return(0);
}

int acs709_read(void)
{
  int sol,vzcrl;
  int sor,vzcrr;
  int mal,mar;

  if(tmr_do(&g_tmr_acs709_s1) != 1) return(0);

//  Serial.print("*** loop CPS: ");
//  Serial.println(g_loop_cps);
  
  sol=analogRead(SH2SH_ACS709_VIOUTL_PORT);
  vzcrl=analogRead(SH2SH_ACS709_VZCRL_PORT);
  sor=analogRead(SH2SH_ACS709_VIOUTR_PORT);
  vzcrr=analogRead(SH2SH_ACS709_VZCRR_PORT);
  
//  if(tmr_do(&g_tmr_acs709_s1) != 1) return(0);

//  Serial.print("*** loop CPS: ");
//  Serial.println(g_loop_cps);

  Serial.print("VIOUT: ");
  Serial.print(sol);
  Serial.print(" ");
  Serial.println(sor);
  
  Serial.print("VZCR: ");
  Serial.print(vzcrl);
  Serial.print(" ");
  Serial.println(vzcrr);

  mal=5000.0*(sol-vzcrl)/1023.0/0.028;
  mar=5000.0*(sor-vzcrr)/1023.0/0.028;

  Serial.print("mA: ");
  Serial.print(mal);
  Serial.print(" ");
  Serial.println(mar);
  
 // Serial.print("Battery voltage: ");
 // Serial.println(g_battV);
  
  return(1);  
}

int acs709_get_mA(unsigned char viout_pin, unsigned int vzcr, unsigned int *mA)
{
  unsigned int viout;
 
  viout=analogRead(viout_pin);
//  vzcr=analogRead(vzcr_pin);

  if(viout >= vzcr) {
    *mA=5000.0*(viout-vzcr)/1023.0/0.028;
  } else {
    *mA=0;
  }

//  Serial.print(viout);
//  Serial.print(" ");

//if(g_cb_m2s > 0) {
if(viout_pin == SH2SH_ACS709_VIOUTR_PORT) {
//  Serial.println();
  /*
  Serial.print("VIOUT: ");
  Serial.print(v1);
  Serial.print(" ");
  Serial.print(v2);
  Serial.print(" ");
  Serial.print(v3);
  Serial.print(" ");
  Serial.print(viout);
  Serial.print(" ");
  Serial.println(vzcr);
  Serial.print("mA=");
  */
//  Serial.println(*mA);
}
//}


//  Serial.print("mA=");
//  Serial.println(*mA);
}
