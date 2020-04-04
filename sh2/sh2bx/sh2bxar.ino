#define SMAR_AVCH_MIN          1
#define SMAR_VCH_TMO           300


/*
#define SMAR_VCH_MIN           4
#define SMAR_VCH_TMO           300
*/

int smar_setup(void)
{
  return(0);
}

int smar_init(SMAR *smar, uint8_t loc, int port, uint8_t avn, unsigned int eqn, uint8_t vcmin)
{
  int rval=-1,i;

  if(smar == NULL) goto end;
  smar->port=port;
  smar->loc=loc;
  if(avn > SMAR_TOT_NUM) goto end;
  if(eqn > 20) goto end;
  for(i=0;i < SMAR_TOT_NUM;i++) {
    smar->tbl[i]=0;
  }
  smar->avn=avn;
  smar->eqn=eqn;
  smar->vcmin=vcmin;
  
  rval=0;
  
end:

  return(rval);
}

int smar_analogRead(SMAR *smar)
{
  int rval=-1,v,lvch=0;
  int sv;

  if(smar->loc == SMAR_ADCLOC_ARDUINO) {
    sv=analogRead(smar->port);
  } else if(smar->loc == SMAR_ADCLOC_MCP3008) {
    sv=adc_single_channel_read(smar->port);
  } else {
    goto end;
  }
  v=smar->sum/smar->avn;
  if(abs(sv-v) > smar->vcmin) {
    smar->sum-=smar->tbl[smar->idx];
    smar->sum+=sv;
    smar->tbl[smar->idx]=sv;
    smar->idx=(smar->idx+1)%smar->avn;
    v=smar->sum/smar->avn;
  }
  if(smar->lvv < 0) {
    smar->lvv=v;
    smar->lvc=0;
    rval=v;
  } else {
    if(smar->lvv == v) {
      smar->lvc=0;
      smar->lvt=g_millis;
      rval=v;
    } else {
      if(smar->lvc++ > smar->eqn) {
        if(abs(smar->lvv-v) <= SMAR_AVCH_MIN) {
          if((g_millis-smar->lvt) > SMAR_VCH_TMO) {
            lvch=1;
          }
        } else {
          lvch=1;
        }
        if(lvch == 1) {
          smar->lvv=v;
          smar->lvc=0;
          rval=v;
        } else {
          rval=smar->lvv;
        }
      } else {
        rval=smar->lvv;
      }
    }
  }

end:

  return(rval);
}

int smar_reset(SMAR *smar)
{
  int i;
  
  smar->idx=0;
  smar->sum=0;
  smar->lvv=0;
  smar->lvc=0;
  smar->lvt=0;
  for(i=0;i < SMAR_TOT_NUM;i++) {
    smar->tbl[i]=0;
  }
  return(0);
}
