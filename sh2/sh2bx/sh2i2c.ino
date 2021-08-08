//barna sda

int g_i2cmaster_state=0;
unsigned char g_i2cmaster_len=0;

int i2c_setup(void)
{
  Wire.begin(17);
  Wire.onReceive(i2c_receivefrommaster);
  Wire.onRequest(i2c_requestfrommaster);
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

void i2c_requestfrommaster(void)
{
  char buf[24];
  int i;

  if(g_i2cmaster_state == 0) {
    g_i2cmaster_len=2+micros()%20;
    Wire.write((char*)&g_i2cmaster_len,1);
    g_i2cmaster_state=1;
    return;
  }
  if(g_i2cmaster_state == 1) {
    for(i=0;i < g_i2cmaster_len-1;i++) {
      buf[i]=g_i2cmaster_len+'a';
    }
    buf[g_i2cmaster_len]='\0';
    Wire.write(buf,(int)g_i2cmaster_len);
    g_i2cmaster_state=0;
    return;
  }
  return;
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
  Serial.print("Running command: ");
  Serial.println(g_i2cbuf.buf);
  g_i2cbuf.len=0;
  g_i2cbuf.state=0;
}
