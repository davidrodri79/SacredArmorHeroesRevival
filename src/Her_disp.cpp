#include "her_tipo.hpp"
#include "her_disp.hpp"

// Crea un disparador en la posici¢n indicada, apuntando en direcci¢n d y
// con tipo de bala [bala]. Puede ser autom tico a intervalos de tiempo, o
// activarse por interruptor.
disparador::disparador(bald x, bald y, w_dir d, byte bala, d_tipo t, byte f, word time)
{
      dx=x; dy=y; ddir=d; tbala=bala; dtipo=t; cont=0; tiempo=time;
      dfuerza=f;
}
void disparador::actualiza()
{
      if(dtipo==DTEMPOR)
         if(cont>0) cont--; else cont=tiempo;
}
bool disparador::disparando()
{
      if((cont==0) && (dtipo==DTEMPOR)) return TRUE; else return FALSE;
}
bald disparador::x_pos()
{
      return dx;
}
bald disparador::y_pos()
{
      return dy;
}
w_dir disparador::dir()
{
      return ddir;
}
byte disparador::t_bala()
{
      return tbala;
}
byte disparador::fuerza()
{
      return dfuerza;
}
void disparador::salva(FILE *fp)
{
      fwrite(&dx,sizeof(dx),1,fp);
      fwrite(&dy,sizeof(dy),1,fp);
      fwrite(&ddir,sizeof(ddir),1,fp);
      fwrite(&tbala,sizeof(tbala),1,fp);
      fwrite(&dtipo,sizeof(dtipo),1,fp);
      fwrite(&dfuerza,sizeof(dfuerza),1,fp);
      fwrite(&tiempo,sizeof(tiempo),1,fp);
}
void disparador::carga(FILE *fp)
{
      fread(&dx,sizeof(dx),1,fp);
      fread(&dy,sizeof(dy),1,fp);
      fread(&ddir,sizeof(ddir),1,fp);
      fread(&tbala,sizeof(tbala),1,fp);
      fread(&dtipo,sizeof(dtipo),1,fp);
      fread(&dfuerza,sizeof(dfuerza),1,fp);
      fread(&tiempo,sizeof(tiempo),1,fp);
      cont=0;
}

