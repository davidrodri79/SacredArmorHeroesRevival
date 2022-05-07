#include"her_tipo.hpp"
#include"her_inte.hpp"


// Crea un interruptor en (xpos,ypos) que activa el objeto n£mero cobj
// del tipo tobj
interr::interr(bald xpos, bald ypos, i_target tobj, bald cobjx, bald cobjy)
{
        ixpos=xpos; iypos=ypos; itobj=tobj; icobjx=cobjx; icobjy=cobjy;
        ion=FALSE; espera=0;
}
// Actualizar estructura en tiempo de juego
void interr::actualiza()
{
        if(espera>0) espera--;
}
// Devuelve si el interruptor permite ser modificado en el momento actual
bool interr::permite()
{
        if(espera==0) return TRUE;
        else return FALSE;
}
// Modifica el estado del interruptor ON<->OFF. Empieza un tiempo de espera,
// tras el cual el interruptor podr  ser modificado de nuevo
void interr::activa()
{
        if(ion==TRUE) ion=FALSE;
                else ion=TRUE;
        espera=IESPERA;
}
// Consultoras de datos
bald interr::x_pos()
{
        return ixpos;
}
bald interr::y_pos()
{
        return iypos;
}
i_target interr::tobj()
{
        return itobj;
}
bald interr::cobjx()
{
        return icobjx;
}
bald interr::cobjy()
{
        return icobjy;
}
bool interr::on()
{
        return ion;
}
void interr::salva(FILE *fp)
{
      if(fp==NULL) {her_error("INTERRUPTOR",ERR_FIL); return;};
      fwrite(&ixpos,sizeof(ixpos),1,fp);
      fwrite(&iypos,sizeof(iypos),1,fp);
      fwrite(&itobj,sizeof(itobj),1,fp);
      fwrite(&icobjx,sizeof(icobjx),1,fp);
      fwrite(&icobjy,sizeof(icobjy),1,fp);
}
void interr::carga(FILE *fp)
{
      if(fp==NULL) {her_error("INTERRUPTOR",ERR_FIL); return;};
      fread(&ixpos,sizeof(ixpos),1,fp);
      fread(&iypos,sizeof(iypos),1,fp);
      fread(&itobj,sizeof(itobj),1,fp);
      fread(&icobjx,sizeof(icobjx),1,fp);
      fread(&icobjy,sizeof(icobjy),1,fp);

      ion=FALSE; espera=0;
}


