#include <UTouch.h>

#include "cpmdn.h"

int tch_setup(void)
{
  med_init(&g_xmdn,35);
  med_init(&g_ymdn,35);
  tmr_init(&g_tmr_touch,70);
  return(0);
}

int tch_touch(int *xx, int *yy)
{
  static int l_x=0,l_y=0;
  int x, y;

  if(myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();

/*
 Serial.print(x);
 Serial.print(" ");
 Serial.println(y);
*/

    med_addvalue(&g_xmdn,x);
    med_addvalue(&g_ymdn,y);
    if(med_getmedianex(&g_xmdn,&x) != 0) return(0);
    med_getmedianex(&g_ymdn,&y);

    if((abs(l_x-x) > 5) || abs(l_y-y) > 5) {
      l_x=x;
      l_y=y;
      tmr_reset(&g_tmr_touch);
      return(0);  
    }
    if(tmr_do(&g_tmr_touch) != 1) {
      return(0);
    }
   
/*
  Serial.print(x);
  Serial.print(" ");
  Serial.println(y);
*/
    *xx=x;
    *yy=y;
    med_reset(&g_xmdn);
    med_reset(&g_ymdn);
    return(1);
  }
  return(0);
}

