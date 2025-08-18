#define WHEEL_PART          1

int dsp_setup(void)
{
  int pwr0;
  
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myGLCD.setFont(BigFont);

  myGLCD.setColor(VGA_RED);
  myGLCD.fillCircle(WHEEL_X0,WHEEL_Y0,WHEEL_R0);

//return(0);

  myGLCD.setColor(VGA_GREEN);
  myGLCD.drawCircle(WHEEL_X0,WHEEL_Y0,WHEEL_R1);
  myGLCD.drawCircle(WHEEL_X0,WHEEL_Y0,WHEEL_R2);
  myGLCD.drawLine(WHEEL_X0-WHEEL_R1,WHEEL_Y0,WHEEL_X0-WHEEL_R0,WHEEL_Y0);
  myGLCD.drawLine(WHEEL_X0+WHEEL_R0,WHEEL_Y0,WHEEL_X0+WHEEL_R1,WHEEL_Y0);
  myGLCD.drawLine(WHEEL_X0,WHEEL_Y0-WHEEL_R0,WHEEL_X0,WHEEL_Y0-WHEEL_R1);

  myGLCD.drawLine(POWER_X0,POWER_Y0,POWER_X0+POWER_A,POWER_Y0);
  myGLCD.drawLine(POWER_X0+POWER_A,POWER_Y0,POWER_X0+POWER_A,POWER_Y0-POWER_B);
  myGLCD.drawLine(POWER_X0+POWER_A,POWER_Y0-POWER_B,POWER_X0,POWER_Y0-POWER_B);
  myGLCD.drawLine(POWER_X0,POWER_Y0-POWER_B,POWER_X0,POWER_Y0);

/*
  myGLCD.drawLine(x0,y0,x0+a,y0);
  myGLCD.drawLine(x0+a,y0,x0+a,y0-b);
  myGLCD.drawLine(x0+a,y0-b,x0,y0-b);
  myGLCD.drawLine(x0,y0-b,x0,y0);
*/
  
  myTouch.InitTouch(LANDSCAPE);
//  myTouch.setPrecision(PREC_MEDIUM);
  myTouch.setPrecision(PREC_LOW);

  drawWheel(WHEEL_X0,WHEEL_Y0,90,WHEEL_R1,WHEEL_R2,WHEEL_A1,WHEEL_B,g_dsp_background,VGA_GREEN);
  pwr0=POWER_Y0-POWER_B/3;
  drawPower(POWER_X0,POWER_Y0,POWER_A,POWER_B,pwr0,g_dsp_background,g_tscr_power_color);
  myGLCD.setColor(VGA_GREEN);
  myGLCD.printNumI((long)0,POWER_X0+POWER_A+3,pwr0-myGLCD.getFontYsize()/2);
  myGLCD.print((char*)"%",POWER_X0+POWER_A+3+myGLCD.getFontXsize(),pwr0-myGLCD.getFontYsize()/2);
  myGLCD.printNumI((long)100,POWER_X0+POWER_A+3,POWER_Y0-myGLCD.getFontYsize()/2);
  myGLCD.print((char*)"%",POWER_X0+POWER_A+3+3*myGLCD.getFontXsize(),POWER_Y0-myGLCD.getFontYsize()/2);
  myGLCD.printNumI((long)100,POWER_X0+POWER_A+3,POWER_Y0-POWER_B-myGLCD.getFontYsize()/2);
  myGLCD.print((char*)"%",POWER_X0+POWER_A+3+3*myGLCD.getFontXsize(),POWER_Y0-POWER_B-myGLCD.getFontYsize()/2);
 
  myGLCD.setColor(VGA_YELLOW);
  myGLCD.printNumI((long)go_cb_fsX,50,370);
  myGLCD.printNumI((long)go_cb_fsY,170,370);
  myGLCD.printNumI((long)go_cb_fsZ,290,370);
  myGLCD.printNumI((long)go_cb_mLs,50,395);
  myGLCD.printNumI((long)go_cb_mRs,170,395);
  myGLCD.printNumI((long)go_cb_rdd,290,395);
  
  myGLCD.print((char*)"RPM:",10,420);
  myGLCD.printNumI((long)go_cb_mLs,100,420);
  myGLCD.printNumI((long)go_cb_mRs,200,420);

  draw_poslight(UCCB_PL_OFF,1);
  
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print((char*)"Ver.:1.3/sh2",600,450);
  
  myGLCD.setColor(VGA_SILVER);
  myGLCD.fillRect(REDRAW_X0,REDRAW_Y0,REDRAW_X0+REDRAW_LEN,REDRAW_Y0+REDRAW_LEN);

  return(0);
}

int numPlaces(unsigned long n)
{
  if (n < 10) return 1;
  if (n < 100) return 2;
  if (n < 1000) return 3;
  if (n < 10000) return 4;
  if (n < 100000) return 5;
  if (n < 1000000) return 6;
  if (n < 10000000) return 7;
  if (n < 100000000) return 8;
  if (n < 1000000000) return 9;
  return 10;
}

void drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness)
{
  int rDelta = -(thickness/2);
  int px, py, cx, cy;

  startAngle -= 90;
  endAngle   -= 90;
  
  if (startAngle!=endAngle)
  {
    for (int i=0; i<thickness; i++)
    {
      px = x + SpeedTrig.cos(startAngle) * (r+rDelta+i);
      py = y + SpeedTrig.sin(startAngle) * (r+rDelta+i);
      for (int d=startAngle+1; d<endAngle+1; d++)
      {
        cx = x + SpeedTrig.cos(d) * (r+rDelta+i);
        cy = y + SpeedTrig.sin(d) * (r+rDelta+i);
        myGLCD.drawLine(px, py, cx, cy);
        px = cx;
        py = cy;
      }
    }
  }
  else
  {
    px = x + SpeedTrig.cos(startAngle) * (r+rDelta);
    py = y + SpeedTrig.sin(startAngle) * (r+rDelta);
    cx = x + SpeedTrig.cos(startAngle) * (r-rDelta);
    cy = y + SpeedTrig.sin(startAngle) * (r-rDelta);
    myGLCD.drawLine(px, py, cx, cy);
  }
}
void drawPie(int x, int y, int r, int startAngle, int endAngle)
{
  int px, py, cx, cy;

  startAngle -= 90;
  endAngle   -= 90;
  if (startAngle>endAngle)
    startAngle -= 360;
  
  px = x + SpeedTrig.cos(startAngle) * r;
  py = y + SpeedTrig.sin(startAngle) * r;
  myGLCD.drawLine(x, y, px, py);
  for (int d=startAngle+1; d<endAngle+1; d++)
  {
    cx = x + SpeedTrig.cos(d) * r;
    cy = y + SpeedTrig.sin(d) * r;
    myGLCD.drawLine(px, py, cx, cy);
    px = cx;
    py = cy;
  }
  myGLCD.drawLine(x, y, px, py);
}

int drawPointer(int x0, int y0, int a, int r, int R)
{
  int x1,y1,x2,y2,x3,y3;

  x1=x0+r*SpeedTrig.cos(a+90);
  y1=y0-r*SpeedTrig.sin(a+90);
  x2=x0+r*SpeedTrig.cos(a+270);
  y2=y0-r*SpeedTrig.sin(a+270);
  x3=x0+R*SpeedTrig.cos(a);
  y3=y0-R*SpeedTrig.sin(a);
  
  myGLCD.drawLine(x1,y1,x3,y3);
  drawArc(x0,y0,r,180-a,360-a,1);
  myGLCD.drawLine(x2,y2,x3,y3);
  
  return(0);
}

int animatePointer(int x0, int y0, int a, int r, int R, word bclr, word fclr)
{
  static int l_x0=0,l_y0=0,l_a=0,l_r=0,l_R=0,f=1;

  if((l_x0 == x0) && (l_y0 == y0) && (l_a == a)) return(0);

  if(f == 0) {
    myGLCD.setColor(bclr);
    drawPointer(l_x0,l_y0,l_a,l_r,l_R);
  }
  myGLCD.setColor(fclr);
  drawPointer(x0,y0,a,r,R);
  l_x0=x0;
  l_y0=y0;
  l_a=a;
  l_r=r;
  l_R=R;
  f=0;

  return(0);
}


int compWheel(int x0, int y0, int ax, int R1, int R2, int a1, int b,
              int *x1, int *y1, int *x2, int *y2, int *x3, int *y3,
              int *x4, int *y4, int *xa, int *ya)
{
  int n,i;

  n=6;

  for(i=0;i < n;i++) {
    *x1=x0+R2*SpeedTrig.cos(ax-a1);
    *y1=y0-R2*SpeedTrig.sin(ax-a1);
    *x2=*x1+b*SpeedTrig.cos(ax);
    *y2=*y1-b*SpeedTrig.sin(ax);
  
    *x3=x0+R2*SpeedTrig.cos(ax+a1);
    *y3=y0-R2*SpeedTrig.sin(ax+a1);
    *x4=*x3+b*SpeedTrig.cos(ax);
    *y4=*y3-b*SpeedTrig.sin(ax);

    *xa=x0+(R2+b)*SpeedTrig.cos(ax);
    *ya=y0-(R2+b)*SpeedTrig.sin(ax);
  }
  
  return(0);
}

int angle_write(int a)
{
  int n,x,y,xd,yd;

  a=-(a-90);
  g_tscr_rudder=a;
  myGLCD.setFont(SevenSegNumFont);
  n=numPlaces(abs(a));
  x=WHEEL_X0-n*myGLCD.getFontXsize()/2;
  y=WHEEL_Y0-100;
  xd=WHEEL_X0+n*myGLCD.getFontXsize()/2;
  yd=y;
  myGLCD.printNumI(abs(a),x,y);
  if(a < 0) {
    y+=myGLCD.getFontYsize()/2;
    myGLCD.drawLine(x-25,y-1,x-5,y-1);
    myGLCD.drawLine(x-25,y,x-5,y);
    myGLCD.drawLine(x-25,y+1,x-5,y+1);
  }
  myGLCD.setFont(BigFont);
  yd=yd-myGLCD.getFontYsize()/4;
  myGLCD.print((char*)"o",xd,yd);

  return(0);
}

int drawWheel(int x0, int y0, int a, int R1, int R2, int a1, int b, 
              word bclr, word fclr)
{
  static int x1[WHEEL_PART],y1[WHEEL_PART],x2[WHEEL_PART],y2[WHEEL_PART],x3[WHEEL_PART],f=1;
  static int y3[WHEEL_PART],x4[WHEEL_PART],y4[WHEEL_PART],xa[WHEEL_PART],ya[WHEEL_PART],pa;
  uint8_t *font;
  int an,i,ax;

  font=myGLCD.getFont();
  an=360/WHEEL_PART;

//  if(a > pa) an=-an;

//  for(i=0;i < WHEEL_PART;i++) {
  for(i=WHEEL_PART-1;i >= 0;i--) {

    if(f == 0) {
      ax=pa+i*an;    
      myGLCD.setColor(bclr);
      myGLCD.drawLine(x1[i],y1[i],x2[i],y2[i]);
      drawArc(xa[i],ya[i],b/2,-ax,180-ax,1);
      myGLCD.drawLine(x3[i],y3[i],x4[i],y4[i]);
      if(i == 0) {
        angle_write(pa);
      }
    }

    ax=a+i*an;    
    compWheel(x0,y0,ax,R1,R2,a1,b,
              &x1[i],&y1[i],&x2[i],&y2[i],&x3[i],&y3[i],&x4[i],&y4[i],&xa[i],&ya[i]);
              
    if(i == 0) {
      myGLCD.setColor(VGA_BLUE);
      angle_write(a);
    } else {
      myGLCD.setColor(fclr);
    }
    myGLCD.drawLine(x1[i],y1[i],x2[i],y2[i]);
    drawArc(xa[i],ya[i],b/2,-ax,180-ax,1);
    myGLCD.drawLine(x3[i],y3[i],x4[i],y4[i]);
   
  }
 
  pa=a;
  f=0;
  myGLCD.setFont(font);
  
  return(0);
}

/*
int drawWheel(int x0, int y0, int a, int R1, int R2, int a1, int b)
{
  int x1,y1,x2,y2,x3,y3,x4,y4,an,n,i,ax;
  int xa,ya;

  n=6;

  an=360/n;

  for(i=0;i < n;i++) {
    ax=a+i*an;    
    x1=x0+R2*SpeedTrig.cos(ax-a1);
    y1=y0-R2*SpeedTrig.sin(ax-a1);
    x2=x1+b*SpeedTrig.cos(ax);
    y2=y1-b*SpeedTrig.sin(ax);
  
    x3=x0+R2*SpeedTrig.cos(ax+a1);
    y3=y0-R2*SpeedTrig.sin(ax+a1);
    x4=x3+b*SpeedTrig.cos(ax);
    y4=y3-b*SpeedTrig.sin(ax);

    xa=x0+(R2+b)*SpeedTrig.cos(ax);
    ya=y0-(R2+b)*SpeedTrig.sin(ax);

    if(i == 0) {
      myGLCD.drawLine(x0,y0,x1,y1);
      myGLCD.drawLine(x0,y0,x3,y3);
//      myGLCD.drawCircle(x0,y0,R1);
//      myGLCD.drawCircle(x0,y0,R2);
    }
    
    myGLCD.drawLine(x1,y1,x2,y2);
    drawArc(xa,ya,b/2,-ax,180-ax,1);
    myGLCD.drawLine(x3,y3,x4,y4);
  }
  
  return(0);
}

int animateWheel(int x0, int y0, int a, int R1, int R2, int a1, int b, word bclr, word fclr)
{
  static int l_x0=0,l_y0=0,l_a=0,l_R1=0,l_R2=0,f=1;
  int x1,y1,x2,y2,x3,y3;

  if((l_x0 == x0) && (l_y0 == y0) && (l_a == a)) return(0);

  if(f == 0) {
    myGLCD.setColor(bclr);
    drawWheel(l_x0,l_y0,l_a,l_R1,l_R2,a1,b);
  }
  myGLCD.setColor(fclr);
  drawWheel(x0,y0,a,R1,R2,a1,b);
  l_x0=x0;
  l_y0=y0;
  l_a=a;
  l_R1=R1;
  l_R2=R2;
  f=0;
}
*/

int animateWheel(int x0, int y0, int a, int R1, int R2, int a1, int b, word bclr, word fclr)
{
  static int l_a=0;

  if(l_a == a) return(0);

  drawWheel(x0,y0,a,R1,R2,a1,b,bclr,fclr);
  l_a=a;

  return(0);
}

int drawPower1(int x0, int y0, int a, int b, int pwr)
{
/*  
  myGLCD.drawLine(x0,y0,x0+a,y0);
  myGLCD.drawLine(x0+a,y0,x0+a,y0-b);
  myGLCD.drawLine(x0+a,y0-b,x0,y0-b);
  myGLCD.drawLine(x0,y0-b,x0,y0);
*/  

  if(pwr > (POWER_Y0-2)) pwr=POWER_Y0-2;
  else if(pwr < (POWER_Y0-POWER_B+2)) pwr=POWER_Y0-POWER_B+2;
  
  myGLCD.drawLine(x0+1,pwr-1,x0+a-1,pwr-1);
  myGLCD.drawLine(x0+1,pwr,x0+a-1,pwr);
  myGLCD.drawLine(x0+1,pwr+1,x0+a-1,pwr+1);
  
  return(0);
}

int pwr_0(void)
{
  return(POWER_Y0-POWER_B/3);
}

int pwr_percent(int pwr, int *spwr)
{
  if(pwr <= pwr_0()) {
    *spwr=(100UL*(pwr_0()-pwr))/((2*POWER_B)/3);
    if(*spwr > 100) *spwr=100;
  } else {
    *spwr=(100UL*(pwr-pwr_0()))/(POWER_B/3);
    if(*spwr > 100) *spwr=100;
    *spwr=-*spwr;
  }

  return(0);
}

int pwr_write(int pwr)
{
  int spwr,n,x,y,xp,yp,s;
  
  myGLCD.setFont(SevenSegNumFont);
  pwr_percent(pwr,&spwr);
  g_tscr_power=spwr;
  n=numPlaces(abs(spwr));
  s=myGLCD.getFontYsize();
  x=WHEEL_X0-n*myGLCD.getFontXsize()/2;
  y=WHEEL_Y0+50;
  xp=WHEEL_X0+n*myGLCD.getFontXsize()/2;
  yp=y;
  myGLCD.printNumI(abs(spwr),x,y);
  if(spwr < 0) {
    y+=myGLCD.getFontYsize()/2;
    myGLCD.drawLine(x-25,y-1,x-5,y-1);
    myGLCD.drawLine(x-25,y,x-5,y);
    myGLCD.drawLine(x-25,y+1,x-5,y+1);
  }
  myGLCD.setFont(BigFont);
  yp=yp-myGLCD.getFontYsize()/4;
  myGLCD.print((char*)"o",xp,yp);
  x=xp;
  y=yp;
  xp=xp+myGLCD.getFontXsize();
  yp=yp+s-myGLCD.getFontYsize();
  myGLCD.print((char*)"o",xp,yp);
  x=x+myGLCD.getFontYsize()/2;
  y=y+myGLCD.getFontYsize()/2;
  xp=xp+myGLCD.getFontYsize()/2;
  yp=yp+myGLCD.getFontYsize()/2;
  myGLCD.drawLine(x,yp,xp,y);

  return(0);
}

int drawPower(int x0, int y0, int a, int b, int pwr, word bclr, word fclr)
{
  int pwr0,spwr;
  uint8_t *font;
  static int f=1,ppwr=0;

  font=myGLCD.getFont();
  if(f == 0) {
    myGLCD.setColor(bclr);
    drawPower1(x0,y0,a,b,ppwr);
    pwr_write(ppwr);
  }
  
  myGLCD.setColor(VGA_GREEN);
  pwr0=pwr_0();
  myGLCD.drawLine(x0+1,pwr0,x0+a-1,pwr0);
  
  myGLCD.setColor(fclr);
  drawPower1(x0,y0,a,b,pwr);
   
  ppwr=pwr;
  f=0;
  
  myGLCD.setFont(SevenSegNumFont);
  pwr_percent(pwr,&spwr);
  pwr_write(pwr);
  myGLCD.setFont(font);
  
  return(0);
}


int animatePower(int x0, int y0, int a, int b, int pwr, word bclr, word fclr)
{
  static int l_pwr=0;

  if(l_pwr == pwr) return(0);

  drawPower(x0,y0,a,b,pwr,bclr,fclr);
  l_pwr=pwr;

  return(0);
}

int tscr_display(void)
{
  if(go_cb_fsX != g_cb_fsX) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_fsX,50,370);
    go_cb_fsX=g_cb_fsX;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_fsX,50,370);
  }
  if(go_cb_fsY != g_cb_fsY) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_fsY,170,370);
    go_cb_fsY=g_cb_fsY;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_fsY,170,370);
  }
  if(go_cb_fsZ != g_cb_fsZ) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_fsZ,290,370);
    go_cb_fsZ=g_cb_fsZ;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_fsZ,290,370);
  }
  if(go_cb_mLs != g_cb_mLs) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_mLs,50,395);
    go_cb_mLs=g_cb_mLs;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_mLs,50,395);
  }
  if(go_cb_mRs != g_cb_mRs) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_mRs,170,395);
    go_cb_mRs=g_cb_mRs;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_mRs,170,395);
  }
  if(go_cb_rdd != g_cb_rdd) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_cb_rdd,290,395);
    go_cb_rdd=g_cb_rdd;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_cb_rdd,290,395);
  }
  if(go_sh1_mLrpm != g_sh1_mLrpm) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_sh1_mLrpm,100,420);
    go_sh1_mLrpm=g_sh1_mLrpm;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_sh1_mLrpm,100,420);
  }
  if(go_sh1_mRrpm != g_sh1_mRrpm) {
    myGLCD.setColor(g_dsp_background);
    myGLCD.printNumI((long)go_sh1_mRrpm,200,420);
    go_sh1_mRrpm=g_sh1_mRrpm;
    myGLCD.setColor(VGA_YELLOW);
    myGLCD.printNumI((long)go_sh1_mRrpm,200,420);
  }

  return(0);
}

#define POSLIGHT_X0        520
#define POSLIGHT_Y0        10
#define POSLIGHT_A         82
#define POSLIGHT_B         35
#define POSLIGHT_X_SHIFT   15

int draw_colrect(int x0, int y0, int a, int b, word c1, word c2)
{
  myGLCD.setColor(c1);
  myGLCD.drawLine(x0,y0,x0+a,y0);
  myGLCD.drawLine(x0,y0,x0,y0+b);
  myGLCD.setColor(c2);
  myGLCD.drawLine(x0+a,y0+b,x0+a,y0);
  myGLCD.drawLine(x0+a,y0+b,x0,y0+b);

  return(0);
}

int draw_poslight(int poslight, int force)
{
  int i,x0,y0;
  static int l_poslight=-1;

  if((l_poslight == poslight) && (force == 0)) return(0);
  l_poslight=poslight;
  g_cb_poslight=poslight;
  for(i=0;i < 3;i++) {
    myGLCD.setColor(VGA_WHITE);
    x0=POSLIGHT_X0+i*(POSLIGHT_A+POSLIGHT_X_SHIFT);
//    myGLCD.drawRect(x0,POSLIGHT_Y0,x0+POSLIGHT_A,POSLIGHT_Y0+POSLIGHT_B);
    if(((g_cb_poslight == UCCB_PL_OFF) && (i == 0)) ||
       ((g_cb_poslight == UCCB_PL_ON) && (i == 1)) ||
       ((g_cb_poslight == UCCB_PL_BLINK) && (i == 2))) {
      draw_colrect(x0,POSLIGHT_Y0,POSLIGHT_A,POSLIGHT_B,VGA_GRAY,VGA_WHITE);
    } else {
      draw_colrect(x0,POSLIGHT_Y0,POSLIGHT_A,POSLIGHT_B,VGA_WHITE,VGA_GRAY);
    }
    myGLCD.setColor(VGA_GREEN);
    myGLCD.fillRect(x0+1,POSLIGHT_Y0+1,x0+POSLIGHT_A-1,POSLIGHT_Y0+POSLIGHT_B-1);
  }
  myGLCD.setBackColor(VGA_GREEN);
  y0=POSLIGHT_Y0+POSLIGHT_B/2-myGLCD.getFontYsize()/2;
  x0=POSLIGHT_X0+0*(POSLIGHT_A+POSLIGHT_X_SHIFT)+POSLIGHT_A/2-(3*myGLCD.getFontXsize())/2;
  if(g_cb_poslight == UCCB_PL_OFF) {
    myGLCD.setColor(VGA_WHITE);
} else {
    myGLCD.setColor(VGA_LIME);
  }
//  myGLCD.print("On",POSLIGHT_X0+0*(POSLIGHT_A+POSLIGHT_X_SHIFT),y0);
  myGLCD.print((char*)"Off",x0,y0);
  x0=POSLIGHT_X0+1*(POSLIGHT_A+POSLIGHT_X_SHIFT)+POSLIGHT_A/2-(2*myGLCD.getFontXsize())/2;
  if(g_cb_poslight == UCCB_PL_ON) {
    myGLCD.setColor(VGA_WHITE);
  } else {
    myGLCD.setColor(VGA_LIME);
  }
//  myGLCD.print("Off",POSLIGHT_X0+1*(POSLIGHT_A+POSLIGHT_X_SHIFT),y0);
  myGLCD.print((char*)"On",x0,y0);
  x0=POSLIGHT_X0+2*(POSLIGHT_A+POSLIGHT_X_SHIFT)+POSLIGHT_A/2-(5*myGLCD.getFontXsize())/2;
  if(g_cb_poslight == UCCB_PL_BLINK) {
    myGLCD.setColor(VGA_WHITE);
  } else {
    myGLCD.setColor(VGA_LIME);
  }
//  myGLCD.print("Blink",POSLIGHT_X0+2*(POSLIGHT_A+POSLIGHT_X_SHIFT),y0);
  myGLCD.print((char*)"Blink",x0,y0);

  myGLCD.setBackColor(g_dsp_background);

  g_force_send=1;  

  return(0);
}
