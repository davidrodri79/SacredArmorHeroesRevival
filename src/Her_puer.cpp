#include"her_tipo.hpp"
#include"her_puer.hpp"

// Crea una puerta que va de (inx,iny) a (finx,finy), del tipo [tipo]. Esta
// puerta debe ser definida de modo que su inicio no sea de baldosa mayor en
// x o y a su fin (coherente), y debe representar una linea hor. o vert. en
// el mapeado.
puerta::puerta(bald inx, bald iny, bald finx, bald finy, tpuerta tipo)
{
         if((finx<inx) || (finy<iny)) her_error("PUERTA",ERR_IMP);
         else{
              if((inx!=finx) && (iny!=finy)) her_error("PUERTA",ERR_IMP);
              else{
                  pinx=inx; piny=iny; pfinx=finx; pfiny=finy;
                  if(piny==pfiny) phoriz=TRUE;
                  else            phoriz=FALSE;
                  ptipo=tipo;
                  ultima_orden=FALSE;
                  espera=0; pabertura=0;
                  };
              };
}
// Actualiza la puerta, de modo que dependiendo de su £ltima orden (abrir o
// cerrar) se abrir  o cerrar  en una unidad. A menos que sea una puerta de
// interruptor y fija, al pasar el tiempo PTESPERA se cerrar  sola.
void puerta::actualiza()
{
         if(ultima_orden==TRUE){
               if(pabertura<PMAXABERT) pabertura++;}
         else{
               if(pabertura>0) pabertura--;};

         if(ptipo!=INTER_FIJA){
               if(espera>0) espera--;
               if((espera==0) && (ultima_orden==TRUE)) activa(); };
}
// Modifica el estado de la puerta, eso es, la abre o cierra
void puerta::activa()
{
         if(ultima_orden==TRUE) ultima_orden=FALSE;
         else {ultima_orden=TRUE; espera=PTESPERA;};
}
// Fuerza la apertura de la puerta
void puerta::abre(word tiempo_espera)
{
         ultima_orden=TRUE; espera=tiempo_espera;
}
// Devuelve cierto si en la baldosa (x,y) la puerta en cuesti¢n existe
bool puerta::pared(bald x, bald y)
{
      if((phoriz) && (y==piny) && (x>=pinx) && (x<=pfinx)) return TRUE;
      if((!phoriz) && (x==pinx) && (y>=piny) && (y<=pfiny)) return TRUE;
      return FALSE;
}
bool puerta::permite()
{
      if(pabertura>=PMAXABERT-PMARGEN) return TRUE;
      else return FALSE;
}
// Consultoras de los datos de la puerta
bald puerta::inx()
{
      return pinx;
}
bald puerta::iny()
{
      return piny;
}
bald puerta::finx()
{
      return pfinx;
}
bald puerta::finy()
{
      return pfiny;
}
tpuerta puerta::tipo()
{
      return ptipo;
}
bool puerta::horiz()
{
      return phoriz;
}
bool puerta::abriendo()
{
      return ultima_orden;
}
byte puerta::abertura()
{
      return pabertura;
}
void puerta::salva(FILE *fp)
{
      if(fp==NULL) {her_error("PUERTA",ERR_FIL); return;};
      fwrite(&pinx,sizeof(pinx),1,fp);
      fwrite(&piny,sizeof(piny),1,fp);
      fwrite(&pfinx,sizeof(pfinx),1,fp);
      fwrite(&pfiny,sizeof(pfiny),1,fp);
      fwrite(&ptipo,sizeof(ptipo),1,fp);
}
void puerta::carga(FILE *fp)
{
      if(fp==NULL) {her_error("PUERTA",ERR_FIL); return;};
      fread(&pinx,sizeof(pinx),1,fp);
      fread(&piny,sizeof(piny),1,fp);
      fread(&pfinx,sizeof(pfinx),1,fp);
      fread(&pfiny,sizeof(pfiny),1,fp);
      fread(&ptipo,sizeof(ptipo),1,fp);

      if(piny==pfiny) phoriz=TRUE; else phoriz=FALSE;
      ultima_orden=FALSE; espera=0; pabertura=0;
}

