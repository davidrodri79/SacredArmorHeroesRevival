#include<stdlib.h>
#include"her_tipo.hpp"
#include"her_puen.hpp"

// Crea el puente que empieza en (xin,yin) y acaba en (xfin,yfin). No puede
// crearse un puente diagonal, debe ser horizontal o vertical. En principio,
// est  replegado (desactivado). Si se especifica que el puente es tempori-
// zado, al pasar el tiempo [espera] se desactivar  solo.
puente::puente(bald xin, bald yin, bald xfin, bald yfin,bool tempor,word espera)
{
         pxin=xin; pyin=yin; pxfin=xfin; pyfin=yfin;
         if(yin==yfin) phoriz=TRUE;
         else{
              if(xin==xfin) phoriz=FALSE;
              else          her_error("PUENTE",ERR_IMP);};

         ultima_orden=FALSE; ptempo=tempor; pespera=espera; cont=0; paso_cont=0;
         progreso=0;
         if(phoriz) size=abs(pxfin-pxin);
         else       size=abs(pyfin-pyin);
}
// Actualiza la estructura: Si se est  cerrando a abriendo y a£n no ha termi-
// nado de realizar la acci¢n, lo avanza en una unidad (baldosa)
void puente::actualiza()
{
         if(paso_cont==0){
            if(ultima_orden==TRUE){
                if(progreso<size+1) progreso++;
            }else{
                if(progreso>0) progreso--;
            };
            paso_cont=P_PASO;
         } else paso_cont--;

         if((ptempo) && (cont>0)){
                    cont--;
                    if(cont==0) ultima_orden=FALSE;
                    };
}
// Cambia la £ltima orden del puente, si est  abierto o abri‚ndose le ordena
// que se cierre y viceversa
void puente::activa()
{
      if(ultima_orden==TRUE) ultima_orden=FALSE;
      else {
           ultima_orden=TRUE;
           cont=pespera;
           paso_cont=P_PASO;};
}
bool puente::horiz()
{
      return phoriz;
}
bald puente::inx()
{
      return pxin;
}
bald puente::iny()
{
      return pyin;
}
bald puente::finx()
{
      return pxfin;
}
bald puente::finy()
{
      return pyfin;
}
// Devuelve cierto o falso dependiendo si en el momento actual, el puente
// cubre la baldosa (x,y), dicho de otro modo, si el puente est  extendido
// en esa posici¢n.
bool puente::suelo(bald x, bald y)
{
      bool b=FALSE;
      if((phoriz) && (y==pyin)){
                if((pxfin>=pxin) && (x>=pxin) && (x<pxin+progreso)) b=TRUE;
                if((pxfin<=pxin) && (x<=pxin) && (x>pxin-progreso)) b=TRUE;
                };
      if((!phoriz) && (x==pxin)){
                if((pyfin>=pyin) && (y>=pyin) && (y<pyin+progreso)) b=TRUE;
                if((pyfin<=pyin) && (y<=pyin) && (y>pyin-progreso)) b=TRUE;
                };
      return b;
}
// Devuelve cierto si el puente existe en esta posici¢n, este activo o
// inactivo
bool puente::dentro(bald x, bald y)
{
      bool b=FALSE;
      if((phoriz) && (y==pyin)){
                if((pxfin>=pxin) && (x>=pxin) && (x<=pxfin)) b=TRUE;
                if((pxfin<=pxin) && (x<=pxin) && (x>=pxfin)) b=TRUE;
                };
      if((!phoriz) && (x==pxin)){
                if((pyfin>=pyin) && (y>=pyin) && (y<=pyfin)) b=TRUE;
                if((pyfin<=pyin) && (y<=pyin) && (y>=pyfin)) b=TRUE;
                };
      return b;
}
void puente::salva(FILE *fp)
{
      if(fp==NULL) {her_error("PUENTE",ERR_FIL); return;};
      fwrite(&pxin,sizeof(pxin),1,fp);
      fwrite(&pyin,sizeof(pyin),1,fp);
      fwrite(&pxfin,sizeof(pxfin),1,fp);
      fwrite(&pyfin,sizeof(pyfin),1,fp);
      fwrite(&ptempo,sizeof(ptempo),1,fp);
      fwrite(&pespera,sizeof(pespera),1,fp);
}
void puente::carga(FILE *fp)
{
      if(fp==NULL) {her_error("PUENTE",ERR_FIL); return;};
      fread(&pxin,sizeof(pxin),1,fp);
      fread(&pyin,sizeof(pyin),1,fp);
      fread(&pxfin,sizeof(pxfin),1,fp);
      fread(&pyfin,sizeof(pyfin),1,fp);
      fread(&ptempo,sizeof(ptempo),1,fp);
      fread(&pespera,sizeof(pespera),1,fp);

      if(pyin==pyfin) phoriz=TRUE; else phoriz=FALSE;
      ultima_orden=FALSE; cont=0;
      progreso=0;
      if(phoriz) size=abs(pxfin-pxin);
      else       size=abs(pyfin-pyin);
}

