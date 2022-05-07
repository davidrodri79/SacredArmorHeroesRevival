#include"her_tipo.hpp"
#include<string.h>
#include"her_nive.hpp"

bool pared_v(telem e)
{
        if((e==PVE1) || (e==PVE2) || (e==PVE3) || (e==PVE4) || (e==PVE5) ||
           (e==PVE6) || (e==PVE7) || (e==PVE8) || (e==PVE9) || (e==PVAT) ||
           (e==DM1V) || (e==DM2V) || (e==DARV) || (e==DTRV) || (e==ECRU) ||
           (e==EL_D) || (e==ER_D) || (e==EL_U) || (e==ER_U) || (e==EULD) ||
           (e==ERUL) || (e==EURD) || (e==ELDR))
                     return TRUE;
        else         return FALSE;
}
bool pared_h(telem e)
{
        if((e==PHO1) || (e==PHO2) || (e==PHO3) || (e==PHO4) || (e==PHO5) ||
           (e==PHO6) || (e==PHO7) || (e==PHO8) || (e==PHO9) || (e==PVAT) ||
           (e==DM1H) || (e==DM2H) || (e==DARH) || (e==DTRH) || (e==ECRU) ||
           (e==EL_D) || (e==ER_D) || (e==EL_U) || (e==ER_U) || (e==EULD) ||
           (e==ERUL) || (e==EURD) || (e==ELDR))
                     return TRUE;
        else         return FALSE;
}
// Crea un nivel vacio, con ficheros y nombres por defecto
nivel::nivel()
{
      int i;
      mp=DBG_NEW mapa();
      if(mp==NULL) her_error("NIVEL",ERR_MEM);
      else{
            nenem=0; nitem=0; csombra=0; cluz=255; maxsecr=0;
            ncoefr=1.0; ncoefg=1.0; ncoefb=1.0;
            for(i=0; i<L_NENEM; i++)  enem[i]=NULL;
            for(i=0; i<L_NITEM; i++)  it[i]=NULL;
            sprintf(nnombre,"Sin nombre");
            sprintf(nfdecor,"decorado");
            sprintf(nfmusic,"musica");
            for(i=0; i<NSPR; i++) sprintf(nfspr[i],"warrior");
      };
}
nivel::~nivel()
{
      int i;
      delete mp;
      for(i=0; i<nenem; i++)  delete enem[i];
      for(i=0; i<nitem; i++)  delete it[i];
}
// Especifica el t¡tulo del nivel
void nivel::cambia_nombre(const  char *n)
{
      if(strlen(n)>=MENSZ) her_error("NIVEL",ERR_CAD);
      else sprintf(nnombre,n);
}
// Cambia el fichero imagen con el decorado
void nivel::cambia_fdecor(const char *n)
{
      if(strlen(n)>=FNAME) her_error("NIVEL",ERR_CAD);
      else sprintf(nfdecor,n);
}
// Cambia el fichero de la musica ambiental
void nivel::cambia_fmusic(const char *n)
{
      if(strlen(n)>=FNAME) her_error("NIVEL",ERR_CAD);
      else sprintf(nfmusic,n);
}
// Cambia uno de los ficheros-imagen con sprites de personaje
void nivel::cambia_fspr(const char *n,int c)
{
      if(strlen(n)>=FNAME) her_error("NIVEL",ERR_CAD);
      else if(c>=NSPR) her_error("NIVEL",ERR_NOC);
           else sprintf(nfspr[c],n);
}
// Manipula los colores de las transparencias de luces y sombras
void nivel::cambia_color_sombra(byte c)
{
      csombra=c;
}
void nivel::cambia_color_luz(byte c)
{
      cluz=c;
}
// Cambia los coeficientes que multiplican los colores de la paleta base
void nivel::cambia_coef_paleta(float r,float g,float b)
{
        ncoefr=r; ncoefg=g; ncoefb=b;
}
// A¤ade un enemigo, una estructura del tipo warrior que ser  controlada por
// el ordenador,sea de un bando u otro. Le pasamos un puntero, debe estar
// creado
void nivel::ingresa_enem(warrior *e)
{
      if(e==NULL) her_error("NIVEL",ERR_NOI);
      else{
           if(nenem>=L_NENEM) her_error("NIVEL",ERR_LLE);
           else{
                enem[nenem]=e;
                nenem++;
           };
      };
}
// Cambia un enemigo contenido en la estructura por otro externo
void nivel::cambia_enem(warrior *e,int cual)
{
      if(e==NULL) her_error("NIVEL",ERR_NOI);
      else{
           if(cual>=nenem) her_error("NIVEL",ERR_LLE);
           else{
                delete enem[cual];
                enem[cual]=e;
           };
      };
}
// A¤ade un item o objeto recogible. Intenta colocarlo en alguna de las nueve
// casillas que rodean (x,y), dando prioridad a (x,y)
void nivel::ingresa_item(bald x, bald y, t_item t)
{

   if(nitem>=L_NITEM) her_error("NIVEL",ERR_LLE);
   else if(t>GEM3) her_error("NIVEL",ERR_IMP);
   else{
        if((mp->permite(x,y)) && (!hay_item(x,y))){
                it[nitem]=DBG_NEW item(x,y,t);
                nitem++;
                mp->inse(x,y,ITEM);
        }else{
        for(bald j=y-1; j<=y+1; j++)
        for(bald i=x-1; i<=x+1; i++){
                if((mp->permite(i,j)) && (!hay_item(i,j))){
                        it[nitem]=DBG_NEW item(i,j,t);
                        nitem++;
                        mp->inse(i,j,ITEM);
                        return;};
                };
                her_error("NIVEL",ERR_LLE);
           };
   };
}
// Retira el enemigo de n£mero id c
void nivel::retira_enem(int c)
{
      int i;
      if(c<nenem){
            delete enem[c];
            for(i=c; i<L_NENEM-1; i++)
                     enem[i]=enem[i+1];
            enem[L_NENEM-1]=NULL;
            nenem--;
      }else her_error("NIVEL",ERR_NOC);
}
// Retira el item de n£mero id c
void nivel::retira_item(int c)
{
      int i,x=it[c]->x_pos(),y=it[c]->y_pos();
      if(c<nitem){
            delete it[c];
            for(i=c; i<L_NITEM-1; i++)
                     it[i]=it[i+1];
            it[L_NITEM-1]=NULL;
            nitem--;
            if(primer_item(x,y)<0)
               mp->inse(x,y,VACIO);
      }else her_error("MAPA",ERR_NOC);
}
// Consultoras
int nivel::hay_enem(bald x, bald y, int m)
{
      int i=m;
      while(i<nenem){
        if((enem[i]->x_pos()==x) && (enem[i]->y_pos()==y)) return i;
        i++;
      };
      return -1;
}
bool nivel::hay_item(bald x, bald y)
{
      if (mp->conse(x,y)==ITEM) return TRUE;
      else return FALSE;
}
int nivel::primer_item(bald x, bald y)
{
      int i=0;
      while(i<nitem){
         if((it[i]->x_pos()==x) && (it[i]->y_pos()==y)) return i;
         i++;
      };
      return -1;
}
int nivel::num_enemigos()
{
      int i=0,n=0;
      while(i<nenem){
         if(enem[i]->bando()==1) n++;
         i++;
      };
      return n;
}
int nivel::num_enemigos_muertos()
{
      int i=0,n=0;
      while(i<nenem){
         if((enem[i]->bando()==1) && ((enem[i]->estado()==DIE) || (enem[i]->estado()==DEST))) n++;
         i++;
      };
      return n;
}
int nivel::num_secrets()
{
      int i=0;
      while(mp->dev_secr(i)!=NULL){
         i++;
      };
      return i;
}
int nivel::num_total_secrets()
{
        return maxsecr;
}
int nivel::num_aliados()
{
      int i=0,n=0;
      while(i<nenem){
         if(enem[i]->bando()==0) n++;
         i++;
      };
      return n;
}
int nivel::num_aliados_muertos()
{
      int i=0,n=0;
      while(i<nenem){
         if((enem[i]->bando()==0) && ((enem[i]->estado()==DIE) || (enem[i]->estado()==DEST))) n++;
         i++;
      };
      return n;
}
mapa *nivel::dev_mapa()
{
      return mp;
}
warrior *nivel::dev_enem(int c)
{
      if(c<nenem) return enem[c];
      else return NULL;
}
item *nivel::dev_item(int c)
{
      if(c<L_NITEM) return it[c];
      else her_error("NIVEL",ERR_NOC);
	  return 0;
}
char *nivel::nombre()
{
      return nnombre;
}
char *nivel::fdecor()
{
      return nfdecor;
}
char *nivel::fmusic()
{
      return nfmusic;
}
char *nivel::fspr(int c)
{
      if(c<=NSPR) return nfspr[c];
      else her_error("NIVEL",ERR_NOC);
	  return 0;
}
byte nivel::color_sombra()
{
      return csombra;
}
byte nivel::color_luz()
{
      return cluz;
}
float nivel::coefr()
{
      return ncoefr;
}
float nivel::coefg()
{
      return ncoefg;
}
float nivel::coefb()
{
      return ncoefb;
}
void nivel::info(int &ite,int &enem)
{
        ite=nitem;
        enem=nenem;
}
void nivel::salva(char *file)
{
      int i;
      FILE *fp;

      fp=fopen(file,"w+b");
      if (fp==NULL) {her_error("NIVEL",ERR_FIL); return;};

      fwrite(nnombre,sizeof(char),MENSZ,fp);
      fwrite(nfdecor,sizeof(char),FNAME,fp);
      fwrite(nfmusic,sizeof(char),FNAME,fp);
      for(i=0; i<NSPR; i++)
            fwrite(nfspr[i],sizeof(char),FNAME,fp);
      fwrite(&csombra,sizeof(csombra),1,fp);
      fwrite(&cluz,sizeof(cluz),1,fp);
      fwrite(&ncoefr,sizeof(ncoefr),1,fp);
      fwrite(&ncoefg,sizeof(ncoefg),1,fp);
      fwrite(&ncoefb,sizeof(ncoefb),1,fp);
      mp->salva(fp);
      fwrite(&nenem,sizeof(nenem),1,fp);
      for(i=0; i<nenem; i++)
               enem[i]->salva(fp);
      fwrite(&nitem,sizeof(nitem),1,fp);
      for(i=0; i<nitem; i++)
               it[i]->salva(fp);
      fclose(fp);
}
void nivel::carga(char *file)
{
      int i;
      FILE *fp;

      fp=fopen(file,"rb");
      if (fp==NULL) {her_error("NIVEL",ERR_FIL); return;};

      fread(nnombre,sizeof(char),MENSZ,fp);
      fread(nfdecor,sizeof(char),FNAME,fp);
      fread(nfmusic,sizeof(char),FNAME,fp);
      for(i=0; i<NSPR; i++)
            fread(nfspr[i],sizeof(char),FNAME,fp);
      fread(&csombra,sizeof(csombra),1,fp);
      fread(&cluz,sizeof(cluz),1,fp);
      fread(&ncoefr,sizeof(ncoefr),1,fp);
      fread(&ncoefg,sizeof(ncoefg),1,fp);
      fread(&ncoefb,sizeof(ncoefb),1,fp);
      mp->carga(fp);
      fread(&nenem,sizeof(nenem),1,fp);
      for(i=0; i<nenem; i++){
               enem[i]=DBG_NEW warrior("",0,0,DABA,0,0,0,0,SIGMA,ENORM,1,0,0,NING,"");
               enem[i]->carga(fp);};
      fread(&nitem,sizeof(nitem),1,fp);
      for(i=0; i<nitem; i++){
               it[i]=DBG_NEW item(0,0,NING);
               it[i]->carga(fp);};
      fclose(fp);
      maxsecr=0;
      while(mp->dev_secr(maxsecr)!=NULL){
         maxsecr++;
      };
}
void nivel::importar(char *file,int c,char *dec, char *en1, char *en2,
                          byte c11, byte c12, byte c21, byte c22)
{
        FILE *p;
        O_EPISODE *ep;
        O_LEVEL *l;
        O_ENEMY *en;
        warrior *w;
        bald nx,ny;
        bool sarr, saba, sder, sizq, nof=TRUE;
        int aux, mx, my, rx, ry, i;
        byte c1, c2;
        char saux[20];

        ep=DBG_NEW O_EPISODE;
        p=fopen(file,"rb");
        fread(ep,sizeof(O_EPISODE),1,p);
        fclose(p);
        l=&(ep->nivel[c]);

        cambia_fdecor(dec);
        cambia_fspr("sigma",0);
        cambia_fspr("gamma",1);
        cambia_fspr(en1,2);
        cambia_fspr(en2,3);
        cambia_nombre(l->map_name);
        cambia_color_sombra(222);
        cambia_color_luz(235);
        ncoefr=1.0; ncoefg=1.0; ncoefb=1.0;
        mp->nueva_pos_inicio(l->start_xy[0]*8+l->start_xy[2],l->start_xy[1]*8+l->start_xy[3],DDER);

        // Pre-tratamiento
        for(mx=0; mx<10; mx++)
        for(my=0; my<10; my++)
        for(rx=0; rx<8; rx++)
        for(ry=0; ry<8; ry++){
                // Elimina puertas dobles
                if((l->map[mx][my][rx][7]==8) && (l->map[mx][my][rx+1][7]==9) &&
                   (l->map[mx][my+1][rx][0]==8) && (l->map[mx][my+1][rx+1][0]==9)){
                        l->map[mx][my][rx][7]=1;
                        l->map[mx][my][rx+1][7]=1;};
                if((l->map[mx][my][7][ry]==36) && (l->map[mx][my][7][ry+1]==37) &&
                   (l->map[mx+1][my][0][ry]==36) && (l->map[mx+1][my][0][ry+1]==37)){
                        l->map[mx][my][7][ry]=1;
                        l->map[mx][my][7][ry+1]=1;};
                if((l->map[mx][my][rx][7]==52) && (l->map[mx][my][rx+1][7]==53) &&
                   (l->map[mx][my+1][rx][0]==52) && (l->map[mx][my+1][rx+1][0]==53)){
                        l->map[mx][my][rx][7]=1;
                        l->map[mx][my][rx+1][7]=1;};
                if((l->map[mx][my][7][ry]==54) && (l->map[mx][my][7][ry+1]==55) &&
                   (l->map[mx+1][my][0][ry]==54) && (l->map[mx+1][my][0][ry+1]==55)){
                        l->map[mx][my][7][ry]=1;
                        l->map[mx][my][7][ry+1]=1;};
                if((l->map[mx][my][rx][7]==56) && (l->map[mx][my][rx+1][7]==57) &&
                   (l->map[mx][my+1][rx][0]==56) && (l->map[mx][my+1][rx+1][0]==57)){
                        l->map[mx][my][rx][7]=1;
                        l->map[mx][my][rx+1][7]=1;};
                if((l->map[mx][my][7][ry]==58) && (l->map[mx][my][7][ry+1]==59) &&
                   (l->map[mx+1][my][0][ry]==58) && (l->map[mx+1][my][0][ry+1]==59)){
                        l->map[mx][my][7][ry]=1;
                        l->map[mx][my][7][ry+1]=1;};
                if((l->map[mx][my][rx][7]==60) && (l->map[mx][my][rx+1][7]==61) &&
                   (l->map[mx][my+1][rx][0]==60) && (l->map[mx][my+1][rx+1][0]==61)){
                        l->map[mx][my][rx][7]=1;
                        l->map[mx][my][rx+1][7]=1;};
                if((l->map[mx][my][7][ry]==62) && (l->map[mx][my][7][ry+1]==63) &&
                   (l->map[mx+1][my][0][ry]==62) && (l->map[mx+1][my][0][ry+1]==63)){
                        l->map[mx][my][7][ry]=1;
                        l->map[mx][my][7][ry+1]=1;};
        };

        for(mx=0; mx<10; mx++)
        for(my=0; my<10; my++)
        for(rx=0; rx<8; rx++)
        for(ry=0; ry<8; ry++){
        nx=mx*8+rx; ny=my*8+ry;
        switch(l->map[mx][my][rx][ry]){
                case 70 :
                case 71 :
                case 72 :
                case 73 :
                case 74 :
                case 63 :
                case 62 :
                case 61 :
                case 60 :
                case 59 :
                case 58 :
                case 57 :
                case 56 :
                case 55 :
                case 54 :
                case 53 :
                case 52 :
                case 50 :
                case 49 :
                case 48 :
                case 47 :
                case 40 :
                case 39 :
                case 38 :
                case 37 :
                case 36 :
                case 35 :
                case 33 :
                case 32 :
                case 31 :
                case 30 :
                case 29 :
                case 28 :
                case 27 :
                case 26 :
                case 25 :
                case 24 :
                case 23 :
                case 22 :
                case 21 :
                case 20 :
                case 19 :
                case 18 :
                case 17 :
                case 16 :
                case 15 :
                case 14 :
                case 13 :
                case 12 :
                case 9  :
                case 8  :
                case 7  :
                case 6  :
                case 5  :
                case 4  :
                case 3  :
                case 2  :
                case 1  : mp->insb(nx,ny,BSU1); break;
                case 10 : mp->insb(nx,ny,BDA1); break;
                case 51 :
                case 11 : mp->insb(nx,ny,BMO1); break;
                case 0  :
                default : mp->insb(nx,ny,NADA); break;
        };
        switch(l->map[mx][my][rx][ry]){
                case 2  : mp->inse(nx,ny,PHO1); break;
                case 3  : mp->inse(nx,ny,PVE1); break;
                case 4  : mp->inse(nx,ny,PHO2); break;
                case 5  : mp->inse(nx,ny,PVE2); break;
                case 6  : mp->inse(nx,ny,PHO7); break;
                case 7  : mp->inse(nx,ny,PVE7); break;
                case 9  : mp->inse(nx,ny,DM1H); break;
                case 8  : mp->inse(nx,ny,DTRH); break;
                case 12 : ingresa_item(nx,ny,PIST); break;
                case 13 : ingresa_item(nx,ny,AMET); break;
                case 14 : ingresa_item(nx,ny,LANZ); break;
                case 15 : ingresa_item(nx,ny,ANIQ); break;
                case 16 : ingresa_item(nx,ny,INFE); break;
                case 17 : ingresa_item(nx,ny,DRAG); break;
                case 18 : ingresa_item(nx,ny,KEY1); break;
                case 19 : ingresa_item(nx,ny,KEY2); break;
                case 20 : ingresa_item(nx,ny,KEY3); break;
                case 21 : ingresa_item(nx,ny,BOTI); break;
                case 22 : ingresa_item(nx,ny,BALA); break;
                case 23 : ingresa_item(nx,ny,COHE); break;
                case 24 : ingresa_item(nx,ny,NAPA); break;
                case 25 : ingresa_item(nx,ny,HEAL); break;
                case 26 : ingresa_item(nx,ny,WALL); break;
                case 27 : ingresa_item(nx,ny,BERS); break;
                case 31 :
                case 29 :
                case 28 : if(nof){mp->area_final(nx,ny,nx,ny); mp->insb(nx,ny,BSU5); nof=FALSE;}; break;
                case 30 : mp->inse(nx,ny,TELE); break;
                case 32 : mp->inse(nx,ny,PHAT); break;
                case 33 : mp->inse(nx,ny,INTE); break;
                case 35 : mp->inse(nx,ny,PVAT); break;
                case 36 : mp->inse(nx,ny,DM2V); break;
                case 37 : mp->inse(nx,ny,DTRV); break;
                case 38 : ingresa_item(nx,ny,MAPA); break;
                case 39 : ingresa_item(nx,ny,INVI); break;
                case 40 : mp->anyade_area_secreta(nx,ny,nx,ny); maxsecr++; break;
                case 52 : mp->inse(nx,ny,DTRH);
                          if(l->map[mx][my][rx+1][ry]==53)
                          mp->anyade_puerta_llave(nx-1,ny,nx+1,ny,LLA1); break;
                case 53 : mp->inse(nx,ny,DM1H); break;
                case 54 : mp->inse(nx,ny,DM2V);
                          if(l->map[mx][my][rx][ry+1]==55)
                          mp->anyade_puerta_llave(nx,ny,nx,ny+2,LLA1); break;
                case 55 : mp->inse(nx,ny,DTRV); break;
                case 56 : mp->inse(nx,ny,DTRH);
                          if(l->map[mx][my][rx+1][ry]==57)
                          mp->anyade_puerta_llave(nx-1,ny,nx+1,ny,LLA2); break;
                case 57 : mp->inse(nx,ny,DM1H); break;
                case 58 : mp->inse(nx,ny,DM2V);
                          if(l->map[mx][my][rx][ry+1]==59)
                          mp->anyade_puerta_llave(nx,ny,nx,ny+2,LLA2); break;
                case 59 : mp->inse(nx,ny,DTRV); break;
                case 60 : mp->inse(nx,ny,DTRH);
                          if(l->map[mx][my][rx+1][ry]==61)
                          mp->anyade_puerta_llave(nx-1,ny,nx+1,ny,LLA3); break;
                case 61 : mp->inse(nx,ny,DM1H); break;
                case 62 : mp->inse(nx,ny,DM2V);
                          if(l->map[mx][my][rx][ry+1]==63)
                          mp->anyade_puerta_llave(nx,ny,nx,ny+2,LLA3); break;
                case 63 : mp->inse(nx,ny,DTRV); break;
                case 70 : mp->inse(nx,ny,DEC1); break;
                case 71 : mp->inse(nx,ny,DEC2); break;
                case 72 : mp->inse(nx,ny,DEC7); break;
                case 73 : mp->inse(nx,ny,COL1); break;
                case 74 : mp->inse(nx,ny,EL_U); break;
                default : mp->inse(nx,ny,VACIO); break;
        };};
        // A¤ade los enemigos
        for(i=0; i<l->e_n; i++){
                en=&(l->enemies[i]);
                switch(en->e_t){
                  case 0 : sprintf(saux,"gamma"); aux=1; c1=56; c2=63; break;
                  case 1 : sprintf(saux,"%s",en1); aux=2; c1=c11; c2=c12; break;
                  case 2 : sprintf(saux,"%s",en2); aux=3; c1=c21; c2=c22; break;
                  default: aux=4; break;};
                w=DBG_NEW warrior(saux,float((en->e_xy[0]*8)+en->e_xy[2])+0.5,float((en->e_xy[1]*8)+en->e_xy[3])+0.5,
                                (w_dir)en->e_d,(byte)aux,(byte)(l->enemies[i].e_l),(byte)(en->e_w),0,SIGMA,ENORM,1,c1,c2,NING,"");
                ingresa_enem(w);
        }
        // A¤ade los teleports
        for(i=0; i<l->t_n; i++)
                mp->anyade_telep(l->teleport[i].origen_xy[0]*8+l->teleport[i].origen_xy[2],
                                 l->teleport[i].origen_xy[1]*8+l->teleport[i].origen_xy[3],
                                 l->teleport[i].destino_xy[0]*8+l->teleport[i].destino_xy[2],
                                 l->teleport[i].destino_xy[1]*8+l->teleport[i].destino_xy[3]);
        // A¤ade los interruptores!
        for(i=0; i<l->s_n; i++)
                switch(l->map[l->switches[i].activable_xy[0]][l->switches[i].activable_xy[1]]
                         [l->switches[i].activable_xy[2]][l->switches[i].activable_xy[3]]){

                         case 48 : mp->anyade_puerta_interr(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2]-2,
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],
                                                        l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],
                                                        l->switches[i].activador_xy[0]*8+l->switches[i].activador_xy[2],
                                                        l->switches[i].activador_xy[1]*8+l->switches[i].activador_xy[3],TRUE);
                                                        mp->inse(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                                l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],DM1H);
                                                        mp->inse(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2]-1,
                                                                l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],DTRH);
                                                                break;
                         case 50 : mp->anyade_puerta_interr(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3]-1,
                                                        l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3]+1,
                                                        l->switches[i].activador_xy[0]*8+l->switches[i].activador_xy[2],
                                                        l->switches[i].activador_xy[1]*8+l->switches[i].activador_xy[3],TRUE);
                                                        mp->inse(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                                l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],DTRV);
                                                        mp->inse(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                                l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3]-1,DM2V);
                                                        break;
                         case 51 : mp->anyade_puente_interr(l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2]-1,
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],
                                                        l->switches[i].activable_xy[0]*8+l->switches[i].activable_xy[2],
                                                        l->switches[i].activable_xy[1]*8+l->switches[i].activable_xy[3],
                                                        l->switches[i].activador_xy[0]*8+l->switches[i].activador_xy[2],
                                                        l->switches[i].activador_xy[1]*8+l->switches[i].activador_xy[3],FALSE,0); break;
                };
        delete ep;

        // Tratamiento posterior
        corrige_esquinas();
        for(ny=1; ny<MSIZEY-1; ny++)
        for(nx=1; nx<MSIZEX-1; nx++){
                //Completa puertas
                if((mp->conse(nx,ny)==DTRH) && (mp->conse(nx+1,ny)==DM1H))
                        mp->inse(nx-1,ny,DM2H);
                if((mp->conse(nx,ny)==DTRV) && (mp->conse(nx,ny-1)==DM2V))
                        mp->inse(nx,ny+1,DM1V);

        };
}
void nivel::corrige_esquinas()
{
        bald nx,ny;
        bool sarr, saba, sder, sizq, nof=TRUE;
        int aux;
        for(ny=1; ny<MSIZEY-1; ny++)
        for(nx=1; nx<MSIZEX-1; nx++){
                // A¤ade esquinas que faltan
                sarr=FALSE; saba=FALSE; sder=FALSE; sizq=FALSE;
                if(pared_v(mp->conse(nx,ny-1))) sarr=TRUE;
                if(pared_h(mp->conse(nx-1,ny))) sizq=TRUE;
                if(pared_h(mp->conse(nx+1,ny))) sder=TRUE;
                if(pared_v(mp->conse(nx,ny+1))) saba=TRUE;
                if(1){
                        if((sarr) && (sizq) &&
                        ((mp->conse(nx-1,ny)!=EL_U) || (mp->conse(nx,ny-1)!=EL_U))) mp->inse(nx,ny,EL_U);
                        if((sarr) && (sder)) mp->inse(nx,ny,ER_U);
                        if((saba) && (sizq)) mp->inse(nx,ny,EL_D);
                        if((saba) && (sder)) mp->inse(nx,ny,ER_D);
                        if((sarr) && (sizq) && (saba)) mp->inse(nx,ny,EULD);
                        if((sder) && (sizq) && (saba)) mp->inse(nx,ny,ELDR);
                        if((sarr) && (sder) && (saba)) mp->inse(nx,ny,EURD);
                        if((sarr) && (sizq) && (sder)) mp->inse(nx,ny,ERUL);
                        if((sarr) && (sizq) && (saba) && (sder)) mp->inse(nx,ny,ECRU);
//                        if(mp->consb(nx,ny)==NADA) mp->insb(nx,ny,BSU1);
                };
        };
}
void nivel::elimina_items_inutiles(bool sig, bool bet, bool ome, byte nplayers)
{
        t_item t;
        for(int i=nitem-1; i>=0; i--){
                t=it[i]->tipo();
                if((sig==FALSE) && ((t==PIST) || (t==AMET) || (t==LANZ) ||
                   (t==ANIQ) || (t==DRAG) || (t==INFE) || (t==SHOC) || (t==BALA) ||
                   (t==COHE) || (t==NAPA)))
                   retira_item(i);
                if((bet==FALSE) && ((t==MFUE) || (t==MHIE) || (t==MELE) ||
                   (t==APOC)))
                   retira_item(i);
                if((ome==FALSE) && ((t==DAGA) || (t==EXCA) || (t==FALC) ||
                   (t==MAZA) || (t==MART) || (t==AGUI) || (t==EDGE) || (t==EMAD) ||
                   (t==ECOB) || (t==EACE) || (t==EBRO) || (t==EPLA) || (t==EORO) ||
                   (t==EDIV)))
                   retira_item(i);
                // Duplica llaves para multiplayer
                if((t>=KEY1) && (t<=KEY4))
                for(int j=0; j<nplayers-1; j++)
                        ingresa_item(it[i]->x_pos(),it[i]->y_pos(),t);
        };
}
