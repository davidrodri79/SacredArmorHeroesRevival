#include"her_tipo.hpp"
#include<math.h>
#include<stdlib.h>
#include"her_item.hpp"
#include"her_warr.hpp"


//Devuelve la experiencia necesaria para ascender a nivel [l]
word exp_level(byte l)
{
        if(l==1) return 0;
        else     return (WLEVPASS*(l-1))+exp_level(l-1);
}
// Crea un guerrero de cracter¡sticas indicadas
// cualspr : N£mero de sprite. El charset del nivel dispone de varios
// level : Nivel del personaje, reputaci¢n del guerrero
// wdir : direcci¢n inicial , 0-3
// NCO1, NCO2 : colores para el sprite (color 1 de gamas con 8 col. como m ximo)
// Constructor para player
warrior::warrior(const char *nombre, float x, float y, w_dir dir, byte cualspr, byte bando, w_clase clase, byte NCO1, byte NCO2, byte lev, word exp)
{
      int i;
      esta_activo = true;
      sprintf(wnombre,"%s",nombre);
      sprintf(wfrase,"");
      wlev=lev; wexp=exp;
      wx=x; wy=y; wsalud=salud_maxima(); wdir=dir; oridir=dir; wclase=clase;
      wcualspr=cualspr; wcualskin = 255; wcol[0]=NCO1; wcol[1]=NCO2; max_arma=1; wdios=FALSE;
      wanim=0; west=WALK; cont=0; wbando=bando; die_cont=0;
      item_as=NING; baston_mago = true;

      for(i=0; i<11; i++) armas_disp[i]=FALSE;
      for(i=0; i<11; i++) escud_disp[i]=FALSE;
      for(i=0; i<3; i++) ammo[i]=0;
      for(i=0; i<4; i++) llaves_disp[i]=FALSE;
      for(i=0; i<WNSPECIALS; i++) cspecial[i]=0;
      armas_disp[0]=TRUE;
      switch(wclase){
            case SIGMA : max_arma=8; break;
            case OMEGA : max_arma=8; break;
            case BETA  : max_arma=11;
                         for(int i=0; i<10; i++) armas_disp[i]=TRUE; break;

            };
      warma=0; wesc=0; tesp_ca=0; telec=0; brelease=TRUE;
      wiaest=PASEO; orispr=wcualspr;
	  woponente=NULL; wagresor=NULL; wcong=0;
}
// Constructor para enemigo/secundario
warrior::warrior(const char *nombre, float x, float y, w_dir dir, byte cualspr, byte salud,
                byte arma, byte escudo, w_clase clase, w_sclase subclase, byte bando,
                byte NCO1, byte NCO2, t_item it, const char *frase)
{
      int i;
      esta_activo = true;
      sprintf(wnombre,"%s",nombre);
      sprintf(wfrase,"%s",frase);
      wx=x; wy=y; wsalud=salud; wdir=dir; oridir=dir; wclase=clase; wsubclase=subclase;
      wcualspr=cualspr; wcol[0]=NCO1; wcol[1]=NCO2;
      wanim=0; west=WALK; cont=0; wbando=bando; wia_cont=0; wia_dir=wdir;
      item_as=it; wlev=1; wexp=0; max_arma=11; wdios=FALSE; baston_mago = true;
      for(i=0; i<11; i++) armas_disp[i]=FALSE;
      for(i=0; i<4; i++) llaves_disp[i]=FALSE;
      for(i=0; i<WNSPECIALS; i++) cspecial[i]=0;
      warma=arma; wesc=escudo; tesp_ca=0; telec=0; talk_cont=0;
      if(wsubclase==EBOSS) wiaest=ESPER;
      else                 wiaest=PASEO;
	  woponente=NULL; wagresor=NULL; wcong=0; orispr=wcualspr;
}
void warrior::actualiza(bool mover)
{
      if(tesp_ca>0) tesp_ca--;
      if(telec>0) telec--;
      if(talk_cont>0) talk_cont--;
      wia_cont++;

      for(int i=0; i<WNSPECIALS; i++)
         if(cspecial[i]>0) cspecial[i]--;
      switch(west){
            // Si esta andando o no, en pie
            case WALK : if(mover){
                           if(wanim==-1) wanim=0;
                           else wanim+=ANIMSP;
                           if (wanim>=4.0) wanim-=4.0; }
                        else wanim=-1;
                        // Si da¤o, recuperamos
                        if(cont>0) cont--;
                        break;
            // Pegando un pu¤etazo o hechizo
            case HECH :
            case PUNY : if(mover){
                           if(wanim==-1) wanim=0;
                           else wanim+=ANIMSP;
                           if (wanim>=4.0) wanim-=4.0; }
                        else wanim=-1;
                        if(cont<WPUNWAIT) cont++; else nuevo_estado(WALK); break;
            // Disparando con arma (SIGMA)
            case SHOT : if(mover){
                           if(wanim==-1) wanim=0;
                           else wanim+=ANIMSP;
                           if (wanim>=4.0) wanim-=4.0; }
                        else wanim=-1;
                        if(cont<WTSHOT) cont++; else nuevo_estado(WALK); break;
            case SLAS : if(mover){
                           if(wanim==-1) wanim=0;
                           else wanim+=ANIMSP;
                           if (wanim>=4.0) wanim-=4.0; }
                        else wanim=-1;
                        if(cont<WTSLAS) cont++; else nuevo_estado(WALK); break;
            case DEFE : if(mover){
                           if(wanim==-1) wanim=0;
                           else wanim+=ANIMSP;
                           if (wanim>=4.0) wanim-=4.0; }
                        else wanim=-1;
                        break;
            // Congelado
            case CONG : if(wcong>0) wcong--; else nuevo_estado(WALK); break;
            // En un teleport
            case TELP : if(cont<WTTEL) cont++; else nuevo_estado(WALK); break;
            // Fuera del nivel
            case OUTL : if(cont<WTTEL) cont++; break;
            // Muerte o destrucci¢n
            case DIE  :
            case DEST : if(cont<WTCAE) cont++; break;
            };
}
// D  un paso hacia la direcci¢n indicada modificando la direccion del perso-
// naje si es necesario
void warrior::avanza(w_dir d, bool permite, bool nocdir)
{
      float co=0.985+(0.015*wlev);
      // Si no se pulsa el "mantener direcci¢n", pasamos a mirar hacia all¡
      if(!nocdir) nueva_dir(d);
      //Avanzar
      if(permite){
            switch(d){
                case DDER : wx+=WPASO*co; break;
                case DABA : wy+=WPASO*co; break;
                case DIZQ : wx-=WPASO*co; break;
                case DARR : wy-=WPASO*co; break;};
            };
}
// Modificadoras directas
void warrior::nueva_pos(float x, float y)
{
      wx=x; wy=y;
}
void warrior::nueva_dir(w_dir d)
{
      if(west<TELP) wdir=d;
}
void warrior::nuevo_estado(w_est e)
{
      cont=0;
      if((e==DEST) && (muerte_gore==FALSE)) e=DIE;
      if((e==DIE) || (e==DEST)) {wsalud=0; wx=int(wx)+0.5; wy=int(wy)+0.5;
								 if(west==CONG) wcualspr=NSPR;}
	  if(e==CONG) wcong=WTCONG;
	  west=e;
}
void warrior::nueva_arma(byte c)
{
      armas_disp[c]=TRUE;
      if((wclase!=BETA) && (c>7)) her_error("WARRIOR",ERR_IMP);
      if(c>warma && (wclase != BETA || c == 10)) warma=c;
}
bool warrior::set_arma(byte c, bool forzar)
{
        if((tesp_ca==0) && (forzar || ((wclase==BETA) && (c<=11)) ||
                            ((wclase==SIGMA) && (c<=7))))
        {
                tesp_ca=15;
                if(armas_disp[c]==TRUE){
                        warma=c;
                        return TRUE;
                }else return FALSE;
        }else return TRUE;
}
void warrior::nuevo_escudo(byte c, bool forzar)
{
      if(wclase!=OMEGA) her_error("WARRIOR",ERR_IMP);
      escud_disp[c]=TRUE;
      if(c>wesc || forzar) wesc=c;
}
void warrior::nuevo_item_asociado(t_item it)
{
      item_as=it;
}
void warrior::set_skin(byte s)
{
	wcualskin = s;
}
void warrior::cambia_nombre(const char *s)
{
      sprintf(wnombre,"%s",s);
}
void warrior::cambia_frase(const char *s)
{
      sprintf(wfrase,"%s",s);
}
void warrior::nueva_salud(word s)
{
      wsalud=s;
}
void warrior::nueva_municion(byte c, word a)
{
      ammo[c]=a;
}
void warrior::set_modo_dios()
{
      wdios=TRUE;
}
void warrior::danyar(word c, cdanyo caus, warrior* agres)
{
      wagresor=agres; causa=caus;
      if((west!=DIE) && (west!=DEST) && (wdios==FALSE))
      if(wsalud<=c){
         wsalud=0;
         if(c>(word)(salud_maxima()*0.2)) nuevo_estado(DEST);
         else     nuevo_estado(DIE);
      } else{
         wsalud-=c;
         cont=WDOLWAIT;
         if(west!=CONG) west=WALK;
      };
}
void warrior::obtiene_special(w_special s, word dur)
{
      cspecial[s]=dur;
}
void warrior::obtiene_llave(byte c)
{
      llaves_disp[c]=TRUE;
}
void warrior::electrocutar(bool activar)
{
      telec= activar ? WTELEC : 0;
}
// Cambia el arma actual del warrior hacia la siguiente que est‚ disponible.
// Si empu¤amos el m ximo arma disponible, pasa de nuevo al 0 (pu¤o/bast¢n)
void warrior::cambia_arma()
{
      if(tesp_ca!=0) return;
      do{
            warma=(warma+1)%max_arma;

      }while(armas_disp[warma]==FALSE);
      tesp_ca=15;
}
void warrior::nueva_dir_ia(w_dir d)
{
        wia_dir=d;
}
void warrior::reset_contador_ia()
{
        wia_cont=0;
}
void warrior::set_contador_ia(byte c)
{
        wia_cont=c;
}
void warrior::set_sclase(w_sclase c)
{
        wsubclase=c;
}
bool warrior::interactuar()
{
//        if(wclase!=CASUAL) return FALSE;
        if(talk_cont>0) return FALSE;

        switch(wsubclase){
                case CPRISI : wsubclase=CESCAP; break;
                case CCURIO : wsubclase=CPASEA; break;
                case CATENT : wsubclase=CINDIC; break;
                };
        item_as=NING; talk_cont=WTALKWAIT;
        return TRUE;
}
void warrior::nuevo_estado_ia(w_iaest e)
{
      wiaest=e;
      wia_cont=0;
}
void warrior::nuevo_oponente(warrior *op)
{
      woponente=op;
}
void warrior::reset(float x, float y, w_dir d,bool dmch)
{
      wx=x; wy=y; wsalud=salud_maxima(); wdir=d;
      wanim=0; west=WALK; cont=0;
	  int i;
      if(dmch){
        for(i=0; i<11; i++) armas_disp[i]=FALSE;
        for(i=0; i<11; i++) escud_disp[i]=FALSE;
        warma=0; wesc=0;
      };
      for(i=0; i<3; i++) ammo[i]=10;
//      for(int i=0; i<4; i++) llaves_disp[i]=FALSE;
      for(i=0; i<WNSPECIALS; i++) cspecial[i]=0;
      armas_disp[0]=TRUE;
      switch(wclase){
            case SIGMA : max_arma=8; break;
            case OMEGA : max_arma=8; break;
            case BETA  : max_arma=11;
                         for(i=0; i<10; i++) armas_disp[i]=TRUE;
                         break;
      };
      tesp_ca=0; telec=0; wcualspr=orispr;
//      die_cont++;
}
void warrior::gana_exp(byte e)
{
        wexp+=e;
}
void warrior::sube_level()
{
        //if(wlev<WMAXLEV)
        wlev++;
}
void warrior::incr_muertes()
{
        die_cont++;
}
void warrior::set_release(bool b)
{
      brelease=b;
}
// Mejora en un grado el arma del personaje
void warrior::mejora_arma()
{
        switch(wclase){
                case SIGMA : if((warma<7) && (warma>0)) warma++; break;
                case OMEGA : if((warma<7) && (warma>0)) warma++;
                             if((wesc<7) && (wesc>0)) wesc++; break;
                case BETA :
                     switch(warma){
                        case 0 : warma++; break;
                        case 1 : warma=3; break;
                        case 2 : break;
                        case 3 : warma=10; break;
                        case 4 : break;
                        case 5 : break;
                        case 6 : break;
                        case 7 : warma=9; break;
                        case 8 : break;
                        case 9 : warma=10; break;
                        case 10: break; }; break;
        };
}
// Consultoras
w_clase warrior::clase()
{
      return wclase;
}
w_sclase warrior::subclase()
{
      return wsubclase;
}
bald warrior::x_pos()
{
      return (bald)wx;
}
bald warrior::y_pos()
{
      return (bald)wy;
}
float warrior::fx_pos()
{
      return wx;
}
float warrior::fy_pos()
{
      return wy;
}
float warrior::front_x(float dist)
{
      switch(wdir){
            case DDER : return (wx+dist);
            case DIZQ : return (wx-dist);
            case DARR : return (wx);
            case DABA : return (wx);};
}
float warrior::front_x_2(int i,float dist)
{
      int op;
      if(i==1) op=-1; if(i==2) op=1;
      switch(wdir){
            case DDER : return (wx+dist);
            case DIZQ : return (wx-dist);
            case DARR : return (wx+(WFSEP*op));
            case DABA : return (wx-(WFSEP*op));};
}
float warrior::front_y(float dist)
{
      switch(wdir){
            case DDER : return (wy);
            case DIZQ : return (wy);
            case DARR : return (wy-dist);
            case DABA : return (wy+dist);};
}
float warrior::front_y_2(int i,float dist)
{
      int op;
      if(i==1) op=-1; if(i==2) op=1;
      switch(wdir){
            case DDER : return (wy+(WFSEP*op));
            case DIZQ : return (wy-(WFSEP*op));
            case DARR : return (wy-dist);
            case DABA : return (wy+dist);};
}
word warrior::salud()
{
      return wsalud;
}
w_dir warrior::dir()
{
      return wdir;
}
w_dir warrior::dir_original()
{
      return oridir;
}
w_dir warrior::dir_ia()
{
      return wia_dir;
}
byte warrior::cualspr()
{
      return wcualspr;
}
byte warrior::cualskin()
{
      return wcualskin;
}
byte warrior::anim()
{
      if(wanim<3.0) return (byte)wanim;
      else          return (byte)1;
}
byte warrior::arma()
{
      return warma;
}
byte warrior::escudo()
{
      return wesc;
}
byte warrior::municion(byte c)
{
      return ammo[c];
}
bool warrior::tiene_arma(byte c)
{
      return armas_disp[c];
}
bool warrior::en_movim()
{
      return (wanim>=0);
}
bool warrior::dios()
{
      return wdios;
}
w_est warrior::estado()
{
      return west;
}
w_iaest warrior::estado_ia()
{
      return wiaest;
}
byte warrior::contador()
{
      if(west==TELP){
         if(cont<WTTEL/2) return cont;
         else return (WTTEL-cont);
      }else return cont;
}
void warrior::set_contador(byte c)
{
        cont=c;
}
byte warrior::contador_ia()
{
      return wia_cont;
}
byte warrior::color(int i)
{
      if(i==1) return wcol[0];
      if(i==2) return wcol[1];
      else return 0;
}
byte warrior::muertes()
{
        return die_cont;
}
byte warrior::level()
{
        return wlev;
}
word warrior::exp()
{
        return wexp;
}
word warrior::salud_maxima()
{
        return 100+(5*(wlev-1));
}
word warrior::municion_maxima(byte c)
{
        switch(wclase){
                case SIGMA : switch(c){
                                case 0 : return 95+(5*wlev);
                                case 1 : return 45+(3*wlev);
                                case 2 : return 195+(5*wlev);
                                }; break;
                case BETA  : return 95+(5*wlev); break;
                case OMEGA : return 0; break;
        };
}
float warrior::level_coef()
{
        return 0.9+(0.05*wlev);
}
bool warrior::special(w_special s)
{
      if(cspecial[s]>0){
        if(cspecial[s]>150) return TRUE;
        else{
            if(((cspecial[s]/10)%2)==0) return TRUE;
            else                       return FALSE;
            };
      }
      else return FALSE;
}
bool warrior::posee_llave(byte c)
{
      return llaves_disp[c];
}
bool warrior::electrocutado()
{
      if(telec>0) return TRUE;
      else        return FALSE;
}
bool warrior::puede_danyar(word d)
{
      if(west!=DEFE) return TRUE;
      else{
           if(d>(word)(5*wesc*level_coef())) return TRUE;
           else         return FALSE;
      }
}
bool warrior::release_boton()
{
      return brelease;
}
char *warrior::nombre()
{
      return wnombre;
}
char *warrior::frase()
{
      return wfrase;
}
t_item warrior::item_asociado()
{
      if(item_as==RAND){
         switch(rand()%4){
               case 0 : return BOTI; break;
               case 1 : switch(wclase){
                           case SIGMA : return BALA; break;
                           case OMEGA : return NING; break;
                           case BETA  : return MFUE; break;
                           case CASUAL: return BOTI; break;}; break;
               case 2 : switch(wclase){
                           case SIGMA : return COHE; break;
                           case OMEGA : return NING; break;
                           case BETA  : return MHIE; break;
                           case CASUAL: return VENE; break;}; break;
               case 3 : switch(wclase){
                           case SIGMA : return NAPA; break;
                           case OMEGA : return NING; break;
                           case BETA  : return MELE; break;
                           case CASUAL: return NING; break;}; break;
         };
      } else return item_as;
}
byte warrior::bando()
{
        return wbando;
}
warrior *warrior::oponente()
{
        return woponente;
}
warrior *warrior::agresor()
{
        return wagresor;
}
cdanyo warrior::causa_danyo()
{
        return causa;
}
void warrior::restaura_sprite_original()
{
	wcualspr = orispr;
}
bool warrior::tiene_baston_mago()
{
	return baston_mago;
}
void warrior::set_baston_mago(bool b)
{
	baston_mago = b;
}
void warrior::salva(FILE *fp)
{
      int i;
      word s;
      if (fp==NULL) {her_error("WARRIOR",ERR_FIL); return;};

      fwrite(&wnombre,sizeof(char),16,fp);
      fwrite(&wfrase,sizeof(char),100,fp);
      fwrite(&wx,sizeof(wx),1,fp);
      fwrite(&wy,sizeof(wy),1,fp);
      s=(byte)wsalud;
      fwrite(&s,sizeof(byte),1,fp);
      fwrite(&wdir,sizeof(wdir),1,fp);
      fwrite(&warma,sizeof(warma),1,fp);
      fwrite(&wesc,sizeof(wesc),1,fp);
      fwrite(&wclase,sizeof(wclase),1,fp);
      fwrite(&wsubclase,sizeof(wsubclase),1,fp);
      fwrite(&wbando,sizeof(wbando),1,fp);
      fwrite(&wcualspr,sizeof(wcualspr),1,fp);
      fwrite(&wcol,sizeof(byte),2,fp);
      fwrite(&item_as,sizeof(item_as),1,fp);
}
void warrior::carga(FILE *fp)
{
      int i;
      byte s;
      if (fp==NULL) {her_error("WARRIOR",ERR_FIL); return;};

      fread(&wnombre,sizeof(char),16,fp);
      fread(&wfrase,sizeof(char),100,fp);
      fread(&wx,sizeof(wx),1,fp);
      fread(&wy,sizeof(wy),1,fp);
      fread(&s,sizeof(byte),1,fp);
      wsalud=(word)s;
      fread(&wdir,sizeof(wdir),1,fp);
      fread(&warma,sizeof(warma),1,fp);
      fread(&wesc,sizeof(wesc),1,fp);
      fread(&wclase,sizeof(wclase),1,fp);
      fread(&wsubclase,sizeof(wsubclase),1,fp);
      fread(&wbando,sizeof(wbando),1,fp);
      fread(&wcualspr,sizeof(wcualspr),1,fp);
      fread(&wcol,sizeof(byte),2,fp);
      fread(&item_as,sizeof(item_as),1,fp);

      wanim=0; west=WALK; tesp_ca=0; telec=0; wia_cont=0; wia_dir=wdir;
      wlev=1; wexp=0; oridir=wdir;
      for(i=0; i<WNSPECIALS; i++) cspecial[i]=0; woponente=NULL;
      if(wsubclase==EBOSS) wiaest=ESPER;
      else                 wiaest=PASEO;
}


