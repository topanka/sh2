void light_setup(void)
{
  pinMode(SH1_POSLIGHT_PORT,OUTPUT);
}

int light_pos_on(void)
{
  digitalWrite(SH1_POSLIGHT_PORT,HIGH);
  return(0);
}

int light_pos_off(void)
{
  digitalWrite(SH1_POSLIGHT_PORT,LOW);
  return(0);
}

