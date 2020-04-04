void loop_counter(void)
{
  unsigned int m1c,m2c; 
  
  g_loop_cnt++;
  if((g_millis-g_loop_ct) > 1000) {
    g_loop_cps=g_loop_cnt;
    g_loop_cnt=0;
    g_loop_ct=g_millis;
  Serial.print("*** loop CPS: ");
  Serial.println(g_loop_cps);
  
//  md_getmc(&m1c,&m2c);
//md_calibrate_vzcr();
//  md_getmc(&m1c,&m2c);
md_get_state();

  }
}

void setup()
{
  Serial.begin(115200);

  delay(1000);
  batt_setup();
  md_setup();
  eval_setup();
  comm_setup();
  servo_setup();
  light_setup();
  qe_setup();
  temp_setup();
  acs709_setup();
  
//  Serial.begin(115200);
  
  delay(2000);
  g_loop_ct=millis();
}

void loop()
{
  g_millis=millis();
  loop_counter();
  
  batt_read(&g_battV,&g_battA);
//  acs709_read();
  
  md_checkmc();
  temp_read();
  comm_send();
  comm_recv();
  eval_doit();
  qe_doit();

}
