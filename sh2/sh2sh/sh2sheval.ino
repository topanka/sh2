void eval_setup(void)
{
  pinMode(13,OUTPUT);
  
  tmr_init(&g_tmr_lightpos,100);
}

int eval_doit(void)
{
  static uint8_t sw=0;
  static int l_lastlightpos=UCCB_PL_OFF;
  
//  if(g_recv_ready != 1) return(0);
  
  if((g_cb_fsBE == 11) && (g_recv_ready == 1)) {
    sw++;
    sw%=2;
    if(sw == 1) {
      light_pos_on();
    } else {
      light_pos_off();
    }
//    md_go();
  }      
  
#if 0  
  if(g_cb_fsBE != 0) {
      digitalWrite(13,HIGH);
//    md_go();
  }  
#endif  

  if(g_cb_b6pBE != 0) {
    digitalWrite(13,LOW);
//    md_stop();
  }      
  
  md_setspeed();
  servo_rudder();
  servo_holder();
  
/*  
  if(g_cb_b6pBE == 11) {
    light_pos_on();
  }      
  if(g_cb_b6pBE == 41) {
    light_pos_off();
  }      
*/  

  if(g_cb_lightpos != l_lastlightpos) {
    if(g_cb_lightpos == UCCB_PL_OFF) {
      light_pos_off();
    } else if(g_cb_lightpos == UCCB_PL_ON) {
      light_pos_on();
    }
    l_lastlightpos=g_cb_lightpos;
  }
  if(g_cb_lightpos == UCCB_PL_BLINK) {
    if(tmr_do(&g_tmr_lightpos) == 1) {
      if(g_tmr_lightpos.cnt%2 == 0) {
        light_pos_on();
      } else {
        light_pos_off();
      }
    }
  }
  
  return(1);
}

