void setup()
{
  comm_setup();
  tch_setup();
  dsp_setup();
  
  Serial.begin(115200);
}

void loop()
{
  int t,a,r;
  long x,y;
  
  g_millis=millis();
/*  
    myGLCD.print("Text rotation", 0, 0);
    myGLCD.setColor(0, 0, 255);
    myGLCD.print("0 degrees", 0, 16, 0);
    myGLCD.print("90 degrees", 319, 0, 90);
    myGLCD.print("180 degrees", 319, 239, 180);
    myGLCD.print("270 degrees", 0, 239, 270);

    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("45", 90, 100, 45);
    myGLCD.print("90", 200, 50, 90);
    myGLCD.print("180", 300, 200, 180);

    myGLCD.setColor(255,0,0);
    myGLCD.printNumI((long)g_oval,200,300);
        
    myGLCD.printNumI((long)g_oval,550,300);
    
  while (true) {
    if(Serial2.available() > 0) {
      g_puff[g_idx]=Serial2.read();
      if(g_puff[g_idx] == '#') {
        g_idx=0;
        continue;
      }
      if(g_puff[g_idx] == '/') {
        g_puff[g_idx]='\0';
        g_idx=0;
        val=atol((char*)&g_puff[0]);
        if(val != g_oval) {
          myGLCD.setColor(0,0,0);
          myGLCD.printNumI(g_oval,200,300);
          myGLCD.setColor(255,0,0);
          myGLCD.printNumI(val,200,300);
          g_oval=val;
        }
      }
      g_idx++;
      if(g_idx >= sizeof(g_puff)) {
        g_idx=0;
        continue;
      }
    }
  }
*/  

/*
  for(i=0;i < 180;i++) {
    
    x1=x0+(int)(90.0*SpeedTrig.cos(i*2)+0.5);
    y1=y0+(int)(90.0*SpeedTrig.sin(i*2)+0.5);
    myGLCD.drawLine(x0,y0,x1,y1);

    delay(300);
  }
*/
/*
  for(i=0;i < 90;i+=10) {
    Serial.print(SpeedTrig.sin(i));
    Serial.print(" ");
    Serial.print(SpeedTrig.sin(i+90));
    Serial.print(" ");
    Serial.print(SpeedTrig.sin(i+180));
    Serial.print(" ");
    Serial.print(SpeedTrig.sin(i+270));
    Serial.println();
  }
*/

  t=tch_touch(&g_touchx,&g_touchy);

/*
  if(t == 1) {
    if((g_touchx < 200) && (g_touchy < 200)){
      a=180.0*SpeedTrig.atan2(y0-g_touchy,g_touchx-x0)/PI;
      animatePointer(100,100,a,16,90,g_dsp_background,VGA_GREEN);
    }
  }
*/  

/*
Serial.print(t);
Serial.print(" ");
Serial.print(g_touchx);
Serial.print(" ");
Serial.println(g_touchy);
*/

  if(t == 1) {
    x=g_touchx-WHEEL_X0;
    y=g_touchy-WHEEL_Y0;
    r=sqrt(x*x+y*y);
    if(r < WHEEL_R0) {
      if(g_tscr_power != 0) {
        g_tscr_power=0;
//        animatePower(POWER_X0,POWER_Y0,POWER_A,POWER_B,POWER_Y0-POWER_B/3,g_dsp_background,g_tscr_power_color);
        animatePower(POWER_X0,POWER_Y0,POWER_A,POWER_B,pwr_0(),g_dsp_background,g_tscr_power_color);
      }
      if(g_tscr_rudder != 0) {
        g_tscr_rudder=0;
        animateWheel(WHEEL_X0,WHEEL_Y0,90,WHEEL_R1,WHEEL_R2,WHEEL_A1,WHEEL_B,g_dsp_background,VGA_GREEN);
      }
    } else if((g_touchy <= WHEEL_Y0) && (g_touchx <= (WHEEL_X0+WHEEL_R2+WHEEL_B))) {
      a=180.0*SpeedTrig.atan2(WHEEL_Y0-g_touchy,g_touchx-WHEEL_X0)/PI;
      animateWheel(WHEEL_X0,WHEEL_Y0,a,WHEEL_R1,WHEEL_R2,WHEEL_A1,WHEEL_B,g_dsp_background,VGA_GREEN);
    } else if((g_touchy <= POWER_Y0) && (g_touchx >= POWER_X0) && (g_touchx <= POWER_X0+POWER_A)) {
      animatePower(POWER_X0,POWER_Y0,POWER_A,POWER_B,g_touchy,g_dsp_background,g_tscr_power_color);
    } else if((g_touchx >= POSLIGHT_X0) && (g_touchy >= POSLIGHT_Y0) && (g_touchx <= (POSLIGHT_X0+POSLIGHT_A)) && (g_touchy <= (POSLIGHT_Y0+POSLIGHT_B))) {
      draw_poslight(UCCB_PL_OFF);
    } else if((g_touchx >= (POSLIGHT_X0+1*(POSLIGHT_A+POSLIGHT_X_SHIFT))) && (g_touchy >= POSLIGHT_Y0) && (g_touchx <= (POSLIGHT_X0+1*(POSLIGHT_A+POSLIGHT_X_SHIFT)+POSLIGHT_A)) && (g_touchy <= (POSLIGHT_Y0+POSLIGHT_B))) {
      draw_poslight(UCCB_PL_ON);
    } else if((g_touchx >= (POSLIGHT_X0+2*(POSLIGHT_A+POSLIGHT_X_SHIFT))) && (g_touchy >= POSLIGHT_Y0) && (g_touchx <= (POSLIGHT_X0+2*(POSLIGHT_A+POSLIGHT_X_SHIFT)+POSLIGHT_A)) && (g_touchy <= (POSLIGHT_Y0+POSLIGHT_B))) {
      draw_poslight(UCCB_PL_BLINK);
    }
  }

/*
  for(i=0;i <= 36;i++) {
    animatePointer(100,100,i*10,16,90,g_dsp_background,VGA_GREEN);
    delay(50);
  }
*/  
  
//  drawPointer(100,100,0,16,90);
//  drawPointer(100,100,90,6,90);
//  drawPointer(100,100,180,6,90);

//  drawArc(200,200,100,0,180,2);
//  drawPie(300,200,100,0,180);
  
  tscr_comm();
  tscr_display();
}
