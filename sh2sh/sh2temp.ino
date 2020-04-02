RunningAverage ra_temp(7);

int temp_setup(void)
{
  tmr_init(&g_tmr_temperature,1000);
  return(0);
}

int temp_read(void)
{
  int temp;

  if(tmr_do(&g_tmr_temperature) != 1) return(0);

  temp=analogRead(UCCB_TEMPERATURE_PORT);
  temp=analogRead(UCCB_TEMPERATURE_PORT);
  ra_temp.addValue(temp);
  temp=ra_temp.getAverage()+0.5;
  g_temperature=(int)(ra_temp.getAverage()*4.8828125);

/*  
  Serial.print(temp/10);
  Serial.print(".");
  Serial.print(temp%10);
  Serial.println("C");
*/
//totcurr();  

  return(1);  
}
