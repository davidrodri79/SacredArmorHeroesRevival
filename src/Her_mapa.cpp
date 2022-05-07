#include"her_tipo.hpp"
#include"her_mapa.hpp"

// Crea un mapa de nivel vac¡o con una matriz de baldosas de MSIZEX*MSIZEY
// baldosas vacias
mapa::mapa()
{
      int y,x;
      for(y=0; y<MSIZEY; y++){
               matrizb[y]=DBG_NEW tbald[MSIZEX];
               matrize[y]=DBG_NEW telem[MSIZEX];
               interes[y]=DBG_NEW byte[MSIZEX];
               if ((matrizb[y]==NULL) || (matrize[y]==NULL) || (interes[y]==NULL)){
                           her_error("MAPA",ERR_MEM);
                           break;
               }else{
                     for(x=0; x<MSIZEX; x++){
                              matrizb[y][x]=NADA;
                              matrize[y][x]=VACIO;};
               };
      };
      manim=0.0;
      ntele=0; npuerta=0; ninterr=0; npuente=0; noscu=0; nsecr=0; nbarr=0; ndisp=0;
      nlaser=0; nambi=0;
      inix=0; iniy=0; inidir=DARR;
      fin=DBG_NEW area((bald)1,(bald)0,(bald)1,(bald)0);
      for(x=0; x<M_NTELE; x++) mtele[x]=NULL;
      for(x=0; x<M_NPUER; x++) mpuerta[x]=NULL;
      for(x=0; x<M_NINTE; x++) minterr[x]=NULL;
      for(x=0; x<M_NPUEN; x++) mpuente[x]=NULL;
      for(x=0; x<M_NOSCU; x++) {moscu[x]=NULL; oscu_on[x]=TRUE;};
      for(x=0; x<M_NSECR; x++) msecr[x]=NULL;
      for(x=0; x<M_NBARR; x++) mbarr[x]=NULL;
      for(x=0; x<M_NDISP; x++) mdisp[x]=NULL;
      for(x=0; x<M_NLASE; x++) mlaser[x]=NULL;
      minx=MSIZEX-1; miny=MSIZEY-1; maxx=0; maxy=0;
}
mapa::~mapa()
{
     int y;
     for(y=0; y<MSIZEY; y++){
              if(matrizb[y]!=NULL) delete[] matrizb[y];
              if(matrize[y]!=NULL) delete[] matrize[y];
              if(interes[y]!=NULL) delete[] interes[y];};

	 delete fin;
     for(y=0; y<ntele; y++) if(mtele[y]!=NULL) delete mtele[y];
     for(y=0; y<npuerta; y++) if(mpuerta[y]!=NULL) delete mpuerta[y];
     for(y=0; y<ninterr; y++) if(minterr[y]!=NULL) delete minterr[y];
     for(y=0; y<npuente; y++) if(mpuente[y]!=NULL) delete mpuente[y];
     for(y=0; y<noscu; y++) if(moscu[y]!=NULL) delete moscu[y];
	 for(y=0; y<nsecr; y++) if (msecr[y]!=NULL) delete msecr[y];
     for(y=0; y<nbarr; y++) if(mbarr[y]!=NULL) delete mbarr[y];
     for(y=0; y<ndisp; y++) if(mdisp[y]!=NULL) delete mdisp[y];
     for(y=0; y<nlaser; y++) if(mlaser[y]!=NULL) delete mlaser[y];
}
// Instala una baldosa de tipo [c] en la posici¢n (x,y) de la matriz
void mapa::insb(bald x, bald y, tbald c)
{
      if((y>=MSIZEY) || (x>=MSIZEX))
                     her_error("MAPA",ERR_COO);
      else{
            if(matrizb[y]!=NULL){
                  matrizb[y][x]=c;
            if(x<minx) minx=x; if(x>maxx) maxx=x;
            if(y<miny) miny=y; if(y>maxy) maxy=y;

            }else
                  her_error("MAPA",ERR_NOI);
            };
}
// Instala un elemento [c] en la posici¢n (x,y) de la matriz
void mapa::inse(bald x, bald y, telem c)
{
      if((y>=MSIZEY) || (x>=MSIZEX))
                     her_error("MAPA",ERR_COO);
      else{
            if(matrize[y]!=NULL){
                  matrize[y][x]=c;
            if(x<minx) minx=x; if(x>maxx) maxx=x;
            if(y<miny) miny=y; if(y>maxy) maxy=y;

            }else
                  her_error("MAPA",ERR_NOI);
            };
}
// Consulta el contenido de la baldosa (x,y)
tbald mapa::consb(bald x, bald y)
{
      if((y>=MSIZEY) || (x>=MSIZEX))
         her_error("MAPA_ITEM-1",ERR_COO);
      else{
            if(matrizb[y]!=NULL)
                  return matrizb[y][x];
            else
                  her_error("MAPA",ERR_NOI);
      };
}
// Consulta el elemento en la baldosa (x,y)
telem mapa::conse(bald x, bald y)
{
      if((y>=MSIZEY) || (x>=MSIZEX))
         her_error("MAPA",ERR_COO);
      else{
            if(matrize[y]!=NULL)
                  return matrize[y][x];
            else
                  her_error("MAPA",ERR_NOI);
      };
}
// Devuelve el estado de la animaci¢n de sprites, un frame de 0,1 o 2
byte mapa::anim()
{
      if(manim<3.0) return (byte)manim;
      else          return (byte)1;
}
// Establece el inicio del nivel en (x,y)
void mapa::nueva_pos_inicio(bald x, bald y, w_dir d)
{
      if((y>=MSIZEY) || (x>=MSIZEX))
            her_error("MAPA",ERR_COO);
      else{ inix=x; iniy=y; inidir=d;};
}
// Consultoras
bald mapa::x_inicio()
{
      return inix;
}
bald mapa::y_inicio()
{
      return iniy;
}
w_dir mapa::dir_inicio()
{
      return inidir;
}
// Devuelve CIERTO si la baldosa (px,py) permite caminar, osea, no existe
// ning£n obst culo solido que impida el paso
bool mapa::permite(bald px, bald py)
{
      //Ver si existe mapa
      if(consb(px,py)==NADA) return FALSE;
      //Ver si no hay decorado
      telem p=conse(px,py);
      if((p!=VACIO) && (p!=TELE) && (p!=DTRH) && (p!=DTRV) && (p!=ITEM) && (p!=PHAT) && (p!=PVAT) && (p!=EXIT)
                    && (p!=TIAR) && (p!=TIAB) && (p!=TIIZ) && (p!=TIDE))
         return FALSE;
      //Ver si hay alguna barrera h bil
      int i=hay_barr(px,py);
      if(i>=0)
               if(dev_barr(i)->danyo()<3) return FALSE;
      //Ver si hay alguna puerta cerrada
      i=hay_puerta(px,py);
      if(i>=0)
               if(dev_puerta(i)->permite()) return TRUE;
               else                         return FALSE;
      return TRUE;
}
// Devuelve cierto si el suelo en (px,py) est  elevado o no (No estar ele-
// vado quiere decir que hay baldosas da¤inas o mortales)
bool mapa::suelo(bald px, bald py)
{
      bool b=TRUE;
      if(hay_puente(px,py)>=0) b=FALSE;

      tbald p=consb(px,py);
      if((p==BSU1) || (p==BSU2) || (p==BSU3) || (p==BSU4) || (p==BSU5)) b=FALSE;

      return b;
}
// Establece el area final, que comprende el cuadrilatero de baldosas
// con esquina superior izquierda (izq,arr) y inferior derecha (der,aba)
void mapa::area_final(bald izq, bald arr, bald der, bald aba)
{
      if((arr>=MSIZEY) || (izq>=MSIZEX) || (aba>=MSIZEY) || (der>=MSIZEX)
                          || (arr>aba)|| (izq>der))
            her_error("MAPA",ERR_COO);
      else{
           if(fin!=NULL) delete(fin);
           fin=DBG_NEW area(izq,arr,der,aba);
      };
}
// Decide si una baldosa (x,y) pertenece al area de final de nivel o no
bool mapa::es_final(bald x, bald y)
{
      return fin->es_dentro(x,y);
}
// Devuelve CIERTO si esta fuera del area de inter‚s (Caja contenedora)
bool mapa::no_elementos(bald x, bald y)
{
      if((x<minx) || (x>maxx) || (y<miny) || (y>maxy)) return TRUE;
      else return FALSE;
}
// Crea un teleport en el mapa con las coordenadas indicadas, si aun queda
// espacio.
void mapa::anyade_telep(bald inx, bald iny, bald finx, bald finy)
{
      if(ntele>=M_NTELE) her_error("MAPA",ERR_LLE);
      else{
         mtele[ntele]=DBG_NEW telep(inx,iny,finx,finy);
         if(mtele[ntele]==NULL) her_error("MAPA",ERR_MEM);
         else ntele++;
      }
}
// Crea una puerta normal, con apertura libre y cierre por tiempo
void mapa::anyade_puerta_normal(bald inx, bald iny, bald finx, bald finy)
{
      if(npuerta>=M_NPUER) her_error("MAPA",ERR_LLE);
      else{
         mpuerta[npuerta]=DBG_NEW puerta(inx,iny,finx,finy,NORM);
         if(mpuerta[npuerta]==NULL) her_error("MAPA",ERR_MEM);
         else npuerta++;
      }
}
//Crea una puerta que requiere llave, se cierra por tiempo
void mapa::anyade_puerta_llave(bald inx, bald iny, bald finx, bald finy, tpuerta tipo)
{
      int i=0;
      if((tipo!=LLA1) && (tipo!=LLA2) && (tipo!=LLA3) && (tipo!=LLA4) && (tipo!=GEMA)){
            her_error("MAPA",ERR_IMP); return;};

      if(npuerta>=M_NPUER) her_error("MAPA",ERR_LLE);
      else{
         mpuerta[npuerta]=DBG_NEW puerta(inx,iny,finx,finy,tipo);
         if(mpuerta[npuerta]==NULL) her_error("MAPA",ERR_MEM);
         else npuerta++;
      }
}
//A¤ade una puerta de (inx,iny) a (finx,finy) que se activa con el
//interruptor que colocamos en (iix,iiy)
void mapa::anyade_puerta_interr(bald inx, bald iny, bald finx, bald finy,
                                 bald iix, bald iiy, bool fija)
{
      tpuerta tipo;
      if(npuerta>=M_NPUER) her_error("MAPA",ERR_LLE);
      else{
         if(fija==FALSE) tipo=INTER; else tipo=INTER_FIJA;
         mpuerta[npuerta]=DBG_NEW puerta(inx,iny,finx,finy,tipo);
         if(mpuerta[npuerta]==NULL) her_error("MAPA",ERR_MEM);
         else{
              npuerta++;
              if(ninterr>=M_NINTE) her_error("MAPA",ERR_LLE);
              else{
                   minterr[ninterr]=DBG_NEW interr(iix,iiy,PUER,inx,iny);
                   if(minterr[ninterr]==NULL) her_error("MAPA",ERR_MEM);
                   else ninterr++;
              };
         };
      };
}
//A¤ade una puente de (inx,iny) a (finx,finy) que se activa con el
//interruptor que colocamos en (iix,iiy)
void mapa::anyade_puente_interr(bald inx, bald iny, bald finx, bald finy,
                                 bald iix, bald iiy, bool tempor, word espera)
{
      bool tipo;
      if(npuente>=M_NPUEN) her_error("MAPA",ERR_LLE);
      else{
         mpuente[npuente]=DBG_NEW puente(inx,iny,finx,finy,tempor,espera);
         if(mpuente[npuente]==NULL) her_error("MAPA",ERR_MEM);
         else{
              npuente++;
              if(ninterr>=M_NINTE) her_error("MAPA",ERR_LLE);
              else{
                   minterr[ninterr]=DBG_NEW interr(iix,iiy,PUEN,inx,iny);
                   if(minterr[ninterr]==NULL) her_error("MAPA",ERR_MEM);
                   ninterr++;
              };
         };
      };
}
// Crea un area oscura en el mapa con las coordenadas indicadas, si aun queda
// espacio.
void mapa::anyade_area_oscura(bald inx, bald iny, bald finx, bald finy)
{
      if(noscu>=M_NOSCU) her_error("MAPA",ERR_LLE);
      else{
         moscu[noscu]=DBG_NEW area(inx,iny,finx,finy);
         if(moscu[noscu]==NULL) her_error("MAPA",ERR_MEM);
         else noscu++;
      };
}
// Crea un area oscura en el mapa con las coordenadas indicadas, si aun queda
// espacio. El interruptor indicado enciende la luz
void mapa::anyade_area_oscura_interr(bald inx, bald iny, bald finx, bald finy, bald ix, bald iy)
{
      if(noscu>=M_NOSCU) her_error("MAPA",ERR_LLE);
      else{
         moscu[noscu]=DBG_NEW area(inx,iny,finx,finy);
         if(moscu[noscu]==NULL) her_error("MAPA",ERR_MEM);
         else {
              noscu++;
              if(ninterr>=M_NINTE) her_error("MAPA",ERR_LLE);
              else{
                   minterr[ninterr]=DBG_NEW interr(ix,iy,OSCU,inx,iny);
                   if(minterr[ninterr]==NULL) her_error("MAPA",ERR_MEM);
                   ninterr++;
              };
         };
      };
}
// Crea un area que el mapa interpretar  como secreta en el mapa con las
// coordenadas indicadas, si aun queda espacio.
void mapa::anyade_area_secreta(bald inx, bald iny, bald finx, bald finy)
{
      if(nsecr>=M_NSECR) her_error("MAPA",ERR_LLE);
      else{
         msecr[nsecr]=DBG_NEW area(inx,iny,finx,finy);
         if(msecr[nsecr]==NULL) her_error("MAPA",ERR_MEM);
         else nsecr++;
      };
}
// Crea un objeto destruible (barrera) en (x,y)
void mapa::anyade_barrera(bald x, bald y, byte t)
{
      if(nbarr>=M_NBARR) her_error("MAPA",ERR_LLE);
      else{
         mbarr[nbarr]=DBG_NEW barrera(x,y,t);
         activa_interes(x,y,BIT_BAR);
         if(mbarr[nbarr]==NULL) her_error("MAPA",ERR_MEM);
         else nbarr++;
      };
}
// A¤ade un disparador en (x,y), que dispara temporizadamente
void mapa::anyade_disp_auto(bald x, bald y, w_dir d, byte b, byte f, word ti)
{
      if(ndisp>=M_NDISP) her_error("MAPA",ERR_LLE);
      else{
         mdisp[ndisp]=DBG_NEW disparador(x,y,d,b,DTEMPOR,f,ti);
         if(mdisp[ndisp]==NULL) her_error("MAPA",ERR_MEM);
         else ndisp++;
      };
}
// A¤ade un disparador que se activa por interruptor
void mapa::anyade_disp_interr(bald x, bald y, bald ix, bald iy, w_dir d, byte b,byte f)
{
      if(ndisp>=M_NDISP) her_error("MAPA",ERR_LLE);
      else{
         mdisp[ndisp]=DBG_NEW disparador(x,y,d,b,DINTER,f,0);
         if(mdisp[ndisp]==NULL) her_error("MAPA",ERR_MEM);
         else {
              ndisp++;
              if(ninterr>=M_NINTE) her_error("MAPA",ERR_LLE);
              else{
                   minterr[ninterr]=DBG_NEW interr(ix,iy,DISP,x,y);
                   if(minterr[ninterr]==NULL) her_error("MAPA",ERR_MEM);
                   ninterr++;
              };
         };
      };
}
//A¤ade una  barrera laser de (inx,iny) a (finx,finy) que se desactiva con el
//interruptor que colocamos en (iix,iiy)
void mapa::anyade_laser(bald inx, bald iny, bald finx, bald finy,
                                 bald iix, bald iiy)
{
      bool tipo;
      if(nlaser>=M_NLASE) her_error("MAPA",ERR_LLE);
      else{
         mlaser[nlaser]=DBG_NEW puente(inx,iny,finx,finy,FALSE,0);
         if(mlaser[nlaser]==NULL) her_error("MAPA",ERR_MEM);
         else{
              mlaser[nlaser]->activa();
              nlaser++;
              if(ninterr>=M_NINTE) her_error("MAPA",ERR_LLE);
              else{
                   minterr[ninterr]=DBG_NEW interr(iix,iiy,LASE,inx,iny);
                   if(minterr[ninterr]==NULL) her_error("MAPA",ERR_MEM);
                   ninterr++;
              };
         };
      };
}
// A¤ade una fuente de sonido ambiental, en la posici¢n (x,y) y con el sample
// asociado al nivel n£mero "s"
void mapa::anyade_sonido_amb(bald x, bald y, byte s)
{
        if(nambi>=M_NAMBI) her_error("MAPA",ERR_LLE);
        else{
             if(s>=N_SMP_AMBI) her_error("MAPA",ERR_IMP);
             else{
                ambientx[nambi]=x; ambienty[nambi]=y;
                ambients[nambi]=s;
                nambi++;
             };
        };
}
// Elimina el teleport de n£mero identif. c.
void mapa::retira_telep(int c)
{
      int i;
      if(c<ntele){
            delete mtele[c];
            for(i=c; i<M_NTELE-1; i++)
                     mtele[i]=mtele[i+1];
            mtele[M_NTELE-1]=NULL;
            ntele--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina la puerta de n£mero identif. c. El interruptor asociado (si lo hay)
// se convertir  en fantasma, no apunta a nada, y habr  que eliminarlo
// manualmente
void mapa::retira_puerta(int c)
{
      int i;
      if(c<npuerta){
            delete mpuerta[c];
            for(i=c; i<M_NPUER-1; i++)
                     mpuerta[i]=mpuerta[i+1];
            mpuerta[M_NPUER-1]=NULL;
            npuerta--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina el interruptor de n£mero identif. c.
void mapa::retira_interr(int c)
{
      int i;
      if(c<ninterr){
            delete minterr[c];
            for(i=c; i<M_NINTE-1; i++)
                     minterr[i]=minterr[i+1];
            minterr[M_NINTE-1]=NULL;
            ninterr--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina el puente de n£mero identif. c. El interruptor asociado (si lo hay)
// se convertir  en fantasma, no apunta a nada, y habr  que eliminarlo
// manualmente
void mapa::retira_puente(int c)
{
      int i;
      if(c<npuente){
            delete mpuente[c];
            for(i=c; i<M_NPUEN-1; i++)
                     mpuente[i]=mpuente[i+1];
            mpuente[M_NPUEN-1]=NULL;
            npuente--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina el area oscura de n£mero identif. c.
void mapa::retira_oscu(int c)
{
      int i;
      if(c<noscu){
            delete moscu[c];
            for(i=c; i<M_NOSCU-1; i++)
                     moscu[i]=moscu[i+1];
            moscu[M_NOSCU-1]=NULL;
            noscu--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina el area oscura de n£mero identif. c.
void mapa::retira_secr(int c)
{
      int i;
      if(c<nsecr){
            delete msecr[c];
            for(i=c; i<M_NSECR-1; i++)
                     msecr[i]=msecr[i+1];
            msecr[M_NSECR-1]=NULL;
            nsecr--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina la barrera de n£mero identif. c.
void mapa::retira_barrera(int c)
{
      int i;
      if(c<nbarr){
            interes[mbarr[c]->posy()][mbarr[c]->posx()]&=(!BIT_BAR);
            delete mbarr[c];
            for(i=c; i<M_NBARR-1; i++)
                     mbarr[i]=mbarr[i+1];
            mbarr[M_NBARR-1]=NULL;
            nbarr--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina un disparador
void mapa::retira_disp(int c)
{
      int i;
      if(c<ndisp){
            delete mdisp[c];
            for(i=c; i<M_NDISP-1; i++)
                     mdisp[i]=mdisp[i+1];
            mdisp[M_NDISP-1]=NULL;
            ndisp--;
      }else her_error("MAPA",ERR_NOC);
}
// Elimina una barrera laser
void mapa::retira_laser(int c)
{
      int i;
      if(c<nlaser){
            delete mlaser[c];
            for(i=c; i<M_NLASE-1; i++)
                     mlaser[i]=mlaser[i+1];
            mlaser[M_NLASE-1]=NULL;
            nlaser--;
      }else her_error("MAPA",ERR_NOC);
}
void mapa::retira_sonido_amb(int c)
{
      int i;
      if(c<nambi){
            for(i=c; i<M_NAMBI-1; i++){
                     ambientx[i]=ambientx[i+1];
                     ambienty[i]=ambienty[i+1];
                     ambients[i]=ambients[i+1];};
            nambi--;
      }else her_error("MAPA",ERR_NOC);
}
// Mantiene la estructura, actualizando todos los elementos independientes
// y las animaciones
void mapa::actualiza()
{
      int i;
      manim+=ANIMSP; if (manim>=4.0) manim-=4.0;
      for(i=0; i<npuerta; i++) mpuerta[i]->actualiza();
      for(i=0; i<npuente; i++) mpuente[i]->actualiza();
      for(i=0; i<ninterr; i++) minterr[i]->actualiza();
      for(i=0; i<nbarr; i++) if (mbarr[i]->danyo()==3) retira_barrera(i);
      for(i=0; i<ndisp; i++) mdisp[i]->actualiza();
      for(i=0; i<nlaser; i++) mlaser[i]->actualiza();
}
// Busca en el mapa un teleport cuyo origen est‚ en (x,y). Si lo encuentra,
// devuelve el numero identificativo. Si no, un -1
int mapa::hay_telep(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_TEL)==BIT_EMPTY) return -1;
      while(i<ntele){
         if((mtele[i]->x_in()==x) && (mtele[i]->y_in()==y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. de la puerta que existe en (x,y), si la hay; y -1
// en caso negativo
int mapa::hay_puerta(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_PUER)==BIT_EMPTY) return -1;
      while(i<npuerta){
         if(mpuerta[i]->pared(x,y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. del puente que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_puente_on_o_off(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_PUE_LAS)==BIT_EMPTY) return -1;
      while(i<npuente){
         if(mpuente[i]->dentro(x,y)) return i;
         i++;
      };
      return -1;
}
int mapa::hay_puente(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_PUE_LAS)==BIT_EMPTY) return -1;
      while(i<npuente){
         if(mpuente[i]->suelo(x,y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. del interruptor que existe en (x,y), si lo hay;
// y -1 en caso negativo
int mapa::hay_interr(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_INT_DIS)==BIT_EMPTY) return -1;
      while(i<ninterr){
         if((minterr[i]->x_pos()==x) && (minterr[i]->y_pos()==y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. del area oscura que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_oscu(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_OSC)==BIT_EMPTY) return -1;
      while(i<noscu){
         if(moscu[i]->es_dentro(x,y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. del area secreta que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_secr(bald x, bald y)
{
      int i=0;
      while(i<nsecr){
         if(msecr[i]->es_dentro(x,y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. de la barrera que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_barr(bald x, bald y)
{
      if((interes[y][x]&BIT_BAR)==BIT_EMPTY) return -1;
      int i=0;
      while(i<nbarr){
         if((mbarr[i]->posx()==x) && (mbarr[i]->posy()==y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. del disparador que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_disp(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_INT_DIS)==BIT_EMPTY) return -1;
      while(i<ndisp){
         if((mdisp[i]->x_pos()==x) && (mdisp[i]->y_pos()==y)) return i;
         i++;
      };
      return -1;
}
// Devuelve el n£mero id. de la barrera que existe en (x,y), si lo hay; y -1
// en caso negativo
int mapa::hay_laser(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_PUE_LAS)==BIT_EMPTY) return -1;
      while(i<nlaser){
         if(mlaser[i]->suelo(x,y)) return i;
         i++;
      };
      return -1;
}
int mapa::hay_laser_on_o_off(bald x, bald y)
{
      int i=0;
      if((interes[y][x]&BIT_PUE_LAS)==BIT_EMPTY) return -1;
      while(i<nlaser){
         if(mlaser[i]->dentro(x,y)) return i;
         i++;
      };
      return -1;
}
int mapa::hay_sonido_amb(bald x, bald y)
{
        int i=0;
        while(i<nambi){
                if((ambientx[i]==x) && (ambienty[i]==y)) return (int)ambients[i];
                i++;
        };
        return -1;
}
void mapa::info(int &tel,int &puen,int &puer,int &inte,int &osc,
                int &sec, int &bar,int &dis,int &las, int &amb)
{
        tel=ntele; puen=npuente; puer=npuerta; inte=ninterr; osc=noscu;
        sec=nsecr; bar=nbarr; dis=ndisp; las=nlaser; amb=nambi;
}
void mapa::caja_contenedora(int &nx, int &ny, int &mx, int &my)
{
        nx=minx; ny=miny; mx=maxx; my=maxy;
}
// Activa el interruptor i y su elemento asociado (puerta,puente);
void mapa::activa_interr(int i)
{
      if(i<ninterr){
            if(minterr[i]->permite()){
                  minterr[i]->activa();
                  switch(minterr[i]->tobj()){
                        case PUER : mpuerta[hay_puerta(minterr[i]->cobjx(),minterr[i]->cobjy())]->activa(); break;
                        case PUEN : mpuente[hay_puente_on_o_off(minterr[i]->cobjx(),minterr[i]->cobjy())]->activa(); break;
                        case OSCU : if(oscu_on[hay_oscu(minterr[i]->cobjx(),minterr[i]->cobjy())]==TRUE)
                                        oscu_on[hay_oscu(minterr[i]->cobjx(),minterr[i]->cobjy())]=FALSE;
                                    else oscu_on[hay_oscu(minterr[i]->cobjx(),minterr[i]->cobjy())]=TRUE; break;
                        case LASE : mlaser[hay_laser_on_o_off(minterr[i]->cobjx(),minterr[i]->cobjy())]->activa(); break;
                        break;
                  };
            };
      }else her_error("MAPA",ERR_NOC);
}
// Fuerza la apertura de la puerta i
void mapa::abre_puerta(int i)
{
      if(i<npuerta) mpuerta[i]->abre();
      else her_error("MAPA",ERR_NOC);
}
// devuelve un puntero al teleport de n£mero [cual]
telep* mapa::dev_telep(int cual)
{
      if(cual<ntele) return mtele[cual];
      else return NULL;
}
// devuelve un puntero al teleport de n£mero [cual]
puerta* mapa::dev_puerta(int cual)
{
      if(cual<npuerta) return mpuerta[cual];
      else return NULL;
}
// devuelve un puntero al interruptor de n£mero [cual]
interr* mapa::dev_interr(int cual)
{
      if(cual<ninterr) return minterr[cual];
      else return NULL;
}
// devuelve un puntero al puente de n£mero [cual]
puente* mapa::dev_puente(int cual)
{
      if(cual<npuente) return mpuente[cual];
      else return NULL;
}
area* mapa::dev_oscu(int cual)
{
      if(cual<noscu) return moscu[cual];
      else return NULL;
}
area* mapa::dev_secr(int cual)
{
      if(cual<nsecr) return msecr[cual];
      else return NULL;
}
area* mapa::dev_final()
{
      return fin;
}
barrera* mapa::dev_barr(int cual)
{
      if(cual<nbarr) return mbarr[cual];
      else return NULL;
}
disparador* mapa::dev_disp(int cual)
{
      if(cual<ndisp) return mdisp[cual];
      else return NULL;
}
puente* mapa::dev_laser(int cual)
{
      if(cual<nlaser) return mlaser[cual];
      else return NULL;
}
int mapa::dev_sonido_amb(int cual,bald &x, bald &y)
{
      if(cual<nambi){ x=ambientx[cual]; y=ambienty[cual]; return ambients[cual];}
      else return -1;
}
// Crea una matriz de booleanos que indica, por baldosa, si hay algun elemento
// especial (telep,puerta,etc). Primero, la limpia (caja contenedora)
void mapa::crea_matriz_interes()
{
        int x,y,xfin,yfin,i,j;
        for(i=0; i<MSIZEY; i++)
        for(j=0; j<MSIZEX; j++)
                interes[i][j]=BIT_EMPTY;

        for(i=0; i<ntele; i++)
                interes[mtele[i]->y_in()][mtele[i]->x_in()]|=BIT_TEL;
        for(i=0; i<ndisp; i++)
                interes[mdisp[i]->y_pos()][mdisp[i]->x_pos()]|=BIT_INT_DIS;
        for(i=0; i<npuerta; i++){
            x=mpuerta[i]->inx(); xfin=mpuerta[i]->finx();
            y=mpuerta[i]->iny(); yfin=mpuerta[i]->finy();
            while((x<xfin) || (y<yfin)){
                interes[y][x]|=BIT_PUER;
                if(x<xfin) x++; if(y<yfin) y++;
            };
            interes[y][x]|=BIT_PUER;
        };
        for(i=0; i<npuente; i++){
            x=mpuente[i]->inx(); xfin=mpuente[i]->finx();
            y=mpuente[i]->iny(); yfin=mpuente[i]->finy();
            while((x!=xfin) || (y!=yfin)){
                interes[y][x]|=BIT_PUE_LAS;
                if(x<xfin) x++; if(y<yfin) y++;
                if(x>xfin) x--; if(y>yfin) y--;
            };
            interes[y][x]|=BIT_PUE_LAS;
        };
        for(i=0; i<nlaser; i++){
            x=mlaser[i]->inx(); xfin=mlaser[i]->finx();
            y=mlaser[i]->iny(); yfin=mlaser[i]->finy();
            while((x!=xfin) || (y!=yfin)){
                interes[y][x]|=BIT_PUE_LAS;
                if(x<xfin) x++; if(y<yfin) y++;
                if(x>xfin) x--; if(y>yfin) y--;
            };
            interes[y][x]|=BIT_PUE_LAS;
        };
        for(i=0; i<ninterr; i++)
                interes[minterr[i]->y_pos()][minterr[i]->x_pos()]|=BIT_INT_DIS;
        for(i=0; i<nbarr; i++)
                interes[mbarr[i]->posy()][mbarr[i]->posx()]|=BIT_BAR;

        for(i=0; i<noscu; i++){
            for(x=moscu[i]->inx(); x<=moscu[i]->finx(); x++)
            for(y=moscu[i]->iny(); y<=moscu[i]->finy(); y++)
                interes[y][x]|=BIT_OSC;
        };
/*        for(int i=0; i<nsecr; i++){
            for(x=msecr[i]->inx(); x<=msecr[i]->finx(); x++)
            for(y=msecr[i]->iny(); y<=msecr[i]->finy(); y++)
                interes[y][x]=TRUE;
        };*/
}
// Manejo de la matriz de interes
bool mapa::es_de_interes(bald x, bald y, byte b)
{
        if ((interes[y][x]&b)==BIT_EMPTY) return FALSE;
        else                            return TRUE;
}
void mapa::activa_interes(bald x, bald y, byte b)
{
        interes[y][x]|=b;
}
void mapa::desactiva_interes(bald x, bald y, byte b)
{
        interes[y][x]&=(~b);
}
void mapa::vacia_matriz_interes(byte b)
{
        int i,j;
        for(i=0; i<MSIZEY; i++)
        for(j=0; j<MSIZEY; j++)
                 interes[i][j]&=(!b);
}
void mapa::salva(FILE *fp)
{
      int i,j;
      if (fp==NULL) {her_error("MAPA",ERR_FIL); return;};

      fwrite(&inix,sizeof(inix),1,fp);
      fwrite(&iniy,sizeof(iniy),1,fp);
      fwrite(&inidir,sizeof(w_dir),1,fp);
      fin->salva(fp);

      fwrite(&minx,sizeof(minx),1,fp);
      fwrite(&miny,sizeof(miny),1,fp);
      fwrite(&maxx,sizeof(maxx),1,fp);
      fwrite(&maxy,sizeof(maxy),1,fp);
      for(i=miny; i<=maxy; i++)
      for(j=minx; j<=maxx; j++)
               fwrite(&matrizb[i][j],sizeof(tbald),1,fp);
      for(i=miny; i<=maxy; i++)
      for(j=minx; j<=maxx; j++)
               fwrite(&matrize[i][j],sizeof(telem),1,fp);
      fwrite(&ntele,sizeof(ntele),1,fp);
      for(i=0; i<ntele; i++)
               mtele[i]->salva(fp);
      fwrite(&npuerta,sizeof(npuerta),1,fp);
      for(i=0; i<npuerta; i++)
               mpuerta[i]->salva(fp);
      fwrite(&ninterr,sizeof(ninterr),1,fp);
      for(i=0; i<ninterr; i++)
               minterr[i]->salva(fp);
      fwrite(&npuente,sizeof(npuente),1,fp);
      for(i=0; i<npuente; i++)
               mpuente[i]->salva(fp);
      fwrite(&noscu,sizeof(noscu),1,fp);
      for(i=0; i<noscu; i++)
               moscu[i]->salva(fp);
      fwrite(&nsecr,sizeof(nsecr),1,fp);
      for(i=0; i<nsecr; i++)
               msecr[i]->salva(fp);
      fwrite(&nbarr,sizeof(nbarr),1,fp);
      for(i=0; i<nbarr; i++)
               mbarr[i]->salva(fp);
      fwrite(&ndisp,sizeof(ndisp),1,fp);
      for(i=0; i<ndisp; i++)
               mdisp[i]->salva(fp);
      fwrite(&nlaser,sizeof(nlaser),1,fp);
      for(i=0; i<nlaser; i++)
               mlaser[i]->salva(fp);
      fwrite(&nambi,sizeof(nambi),1,fp);
      for(i=0; i<nambi; i++){
               fwrite(&ambientx[i],sizeof(ambientx[i]),1,fp);
               fwrite(&ambienty[i],sizeof(ambienty[i]),1,fp);
               fwrite(&ambients[i],sizeof(ambients[i]),1,fp);};
}
void mapa::carga(FILE *fp)
{
      int i,j;
      if (fp==NULL) {her_error("MAPA",ERR_FIL); return;};

      fread(&inix,sizeof(inix),1,fp);
      fread(&iniy,sizeof(iniy),1,fp);
      fread(&inidir,sizeof(w_dir),1,fp);
      fin->carga(fp);
      for(i=0; i<MSIZEY; i++)
      for(j=0; j<MSIZEX; j++){
                matrize[i][j]=VACIO; matrizb[i][j]=NADA;
      };
      fread(&minx,sizeof(minx),1,fp);
      fread(&miny,sizeof(miny),1,fp);
      fread(&maxx,sizeof(maxx),1,fp);
      fread(&maxy,sizeof(maxy),1,fp);
      for(i=miny; i<=maxy; i++)
      for(j=minx; j<=maxx; j++)
               fread(&matrizb[i][j],sizeof(tbald),1,fp);
      for(i=miny; i<=maxy; i++)
      for(j=minx; j<=maxx; j++)
               fread(&matrize[i][j],sizeof(telem),1,fp);
      fread(&ntele,sizeof(ntele),1,fp);
      for(i=0; i<ntele; i++){
               mtele[i]=DBG_NEW telep(0,0,0,0);
               mtele[i]->carga(fp);};
      fread(&npuerta,sizeof(npuerta),1,fp);
      for(i=0; i<npuerta; i++){
               mpuerta[i]=DBG_NEW puerta(0,0,0,0,NORM);
               mpuerta[i]->carga(fp);};
      fread(&ninterr,sizeof(ninterr),1,fp);
      for(i=0; i<ninterr; i++){
               minterr[i]=DBG_NEW interr(0,0,PUER,0,0);
               minterr[i]->carga(fp);};
      fread(&npuente,sizeof(npuente),1,fp);
      for(i=0; i<npuente; i++){
               mpuente[i]=DBG_NEW puente(0,0,0,0,TRUE,0);
               mpuente[i]->carga(fp);};
      fread(&noscu,sizeof(noscu),1,fp);
      for(i=0; i<noscu; i++){
               moscu[i]=DBG_NEW area(0,0,0,0);
               moscu[i]->carga(fp);};
      fread(&nsecr,sizeof(nsecr),1,fp);
      for(i=0; i<nsecr; i++){
               msecr[i]=DBG_NEW area(0,0,0,0);
               msecr[i]->carga(fp);};
      fread(&nbarr,sizeof(nbarr),1,fp);
      for(i=0; i<nbarr; i++){
               mbarr[i]=DBG_NEW barrera(0,0,0);
               mbarr[i]->carga(fp);}
      fread(&ndisp,sizeof(ndisp),1,fp);
      for(i=0; i<ndisp; i++){
               mdisp[i]=DBG_NEW disparador(0,0,DABA,0,DTEMPOR,0,0);
               mdisp[i]->carga(fp);};
      fread(&nlaser,sizeof(nlaser),1,fp);
      for(i=0; i<nlaser; i++){
               mlaser[i]=DBG_NEW puente(0,0,0,0,FALSE,0);
               mlaser[i]->carga(fp);
               mlaser[i]->activa();};

      fread(&nambi,sizeof(nambi),1,fp);
      for(i=0; i<nambi; i++){
               fread(&ambientx[i],sizeof(ambientx[i]),1,fp);
               fread(&ambienty[i],sizeof(ambienty[i]),1,fp);
               fread(&ambients[i],sizeof(ambients[i]),1,fp);};
      manim=0.0;
}






