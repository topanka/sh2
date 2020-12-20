//barna sda

int i2c_setup(void)
{
  Wire.begin(17);
  Wire.onReceive(i2c_receivefrommaster);
  g_i2cbuf.state=0;
  g_i2cbuf.len=0;
  
  return(0);
}

void i2c_receivefrommaster(int n)
{
  int i;
  char c;

  Serial.print("receive frommaster ");
  Serial.println(n);
  
  if(g_i2cbuf.state != 0) return;
  for(i=0;i < n;i++) {
    c=Wire.read();      /* receive byte as a character */
    if(g_i2cbuf.state == 1) continue;
    if(g_i2cbuf.len >= I2CGEN_BUFSIZE) continue;
    g_i2cbuf.buf[g_i2cbuf.len]=c;
    g_i2cbuf.len++;
    if(c == '\0') {
      g_i2cbuf.state=1;
    }
//    Serial.print(c);           /* print the character */
  }
//  Serial.println();             /* to newline */
}

void i2c_processcommand(void)
{
//  static unsigned long xx=0;
  
  if(g_i2cbuf.state != 1) {
/*    
    if(xx%5000 == 0) {
      Serial.print("No sms command ");
      Serial.println(xx);
    }
    xx++;
*/    
    return;
  }
  Serial.println(g_i2cbuf.buf);
  g_i2cbuf.len=0;
  g_i2cbuf.state=0;
}
