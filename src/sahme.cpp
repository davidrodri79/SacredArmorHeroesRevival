/*
########################################################################
###  SAHME Sacred Armor Heroes Map Editor                            ###
###  Genera niveles compatibles con el engine de Sacred Armor Heroes ###
###  Versi¢n 1.3 - Por NHSP                                          ###
########################################################################
*/

#include<math.h>
#include<stdlib.h>
//#include<dpmi.h>
#include"her_tipo.hpp"
#include<allegro5/allegro.h>
#include"her_graf.hpp"
#include"profiler.h"

#define C_BKG BMAR3
#define C_FRG BMAR1
#define C_SEL BROJO

volatile int speed_counter = 0;

char GDIR[12]="DATA", FILENAME[12]="security";

char NOMBRE_MODULO[]="Sacred Armor Heroes Map Editor 1.3 - Por NHSP";

int lmd=11, lmm=7, lma=2000;

int SCRX, SCRY, SCRDRV, MODO=3;

BITMAP *tile, *button[13];

enum {BNEW=0,BINF,BOPE,BSIG,BDIS,BDEC,BENE,BTOO,BNO,BYES,BCL1,BCL2,BCL3};

BITMAP *pant, *back;
charset *ch;
waves *s;
batalla *b;
nivel *niv, *buffer;
mapa *mp;
//RGB *pal;

int buffersx=0, buffersy=0;

void update_display_and_flip(ALLEGRO_BITMAP *bmp)
{
	int bmpw = al_get_bitmap_width(bmp);
	int bmph = al_get_bitmap_height(bmp);
	int scrw = al_get_display_width(game_display);
	int scrh = al_get_display_height(game_display);

	al_set_target_backbuffer(game_display);
	___ENTER_PROFILER_BLOCK("Clear display")
	al_clear_to_color(al_map_rgb(0,0,0));
	___LEAVE_PROFILER_BLOCK("Clear display")
	if(bmpw != scrw || bmph != scrh)
	{
		if((float)scrw / (float)scrh > (float)bmpw / (float)bmph)
		{
			int vwporth = scrh;
			int vwportw = bmpw * scrh/bmph;
			___ENTER_PROFILER_BLOCK("Draw scaled bmp")
			al_draw_scaled_bitmap(bmp,0,0,bmpw,bmph,(scrw - vwportw) / 2,0,vwportw,vwporth,0);
			___LEAVE_PROFILER_BLOCK("Draw scaled bmp")
		}
		else
		{
			int vwportw = scrw;
			int vwporth = bmph * scrw/bmpw;
			___ENTER_PROFILER_BLOCK("Draw scaled bmp")
			al_draw_scaled_bitmap(bmp,0,0,bmpw,bmph,0,(scrh - vwporth) / 2,vwportw,vwporth,0);
			___LEAVE_PROFILER_BLOCK("Draw scaled bmp")
		}
	}
	else
	{
		___ENTER_PROFILER_BLOCK("Draw bmp")
		al_draw_bitmap(bmp,0,0,0);
		___LEAVE_PROFILER_BLOCK("Draw bmp")
	}
	___ENTER_PROFILER_BLOCK("Allegro flip")
	al_flip_display();
	___LEAVE_PROFILER_BLOCK("Allegro flip")
}

void mensaje_inicio(batalla *b, charset *ch, ALLEGRO_BITMAP *pant, char *msg, int sx, int sy, byte c)
{
      b->encola_mensaje_consola(msg,c);
      b->actualiza(1);
      ch->muestra_pantalla(pant,b);
	  update_display_and_flip(pant);
}
void arranque_sistema(batalla *b,ALLEGRO_BITMAP *pant,charset *ch,waves *s,int SCRX, int SCRY, int SCRDRV)
{
      long memoria;
      char frase[60];

      if(allegro_init()==0){
         printf("Allegro %s Inicializado correctamente\n",ALLEGRO_VERSION_STR);
      } else {
         printf("­No se pudo inicilizar Allegro!\n");
         exit(1);};

      printf("Cargando sprites est ndar...\n");
      ch->carga_sprites(b);
      b->abre_cierra_consola();
      if(set_gfx_mode(SCRDRV, SCRX, SCRY, 0, 0)==0){
         sprintf(frase,"Modo %dx%dx256 inicializado correctamente.",SCRX,SCRY);
         mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);
      }else{
         mensaje_inicio(b,ch,pant,"­Error al inicializar modo gr fico!",SCRX,SCRY,BCO1);
         exit(1);
      };
      sprintf(frase,"Ultima modificaci¢n : %d %s %dth",lma,smes(lmm),lmd);
      mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);
      check_cpu();
      if(cpu_family==3) mensaje_inicio(b,ch,pant,"CPU : 386",SCRX,SCRY,BMAR1);
      if(cpu_family==4) mensaje_inicio(b,ch,pant,"CPU : 486",SCRX,SCRY,BMAR1);
      if(cpu_family>=5)
         mensaje_inicio(b,ch,pant,"CPU : Pentium o superior",SCRX,SCRY,BMAR1);
      if(cpu_mmx) mensaje_inicio(b,ch,pant,"Instrucciones MMX disponibles",SCRX,SCRY,BMAR1);

      memoria=_go32_dpmi_remaining_physical_memory();
      sprintf(frase,"Memoria RAM del sistema : %ld Mb",(unsigned long)(memoria/1000000));
      mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);

      switch(os_type){
         case OSTYPE_UNKNOWN : mensaje_inicio(b,ch,pant,"Sistema Operativo : MS-DOS",SCRX,SCRY,BMAR1); break;
         case OSTYPE_WIN3 : mensaje_inicio(b,ch,pant,"Sistema Operativo : Windows 3.1",SCRX,SCRY,BMAR1); break;
         case OSTYPE_WIN95 : mensaje_inicio(b,ch,pant,"Sistema Operativo : Windows 95-98",SCRX,SCRY,BMAR1); break;
         case OSTYPE_WINNT : mensaje_inicio(b,ch,pant,"Sistema Operativo : Windows NT",SCRX,SCRY,BMAR1); break;};

      if(install_timer()==0) mensaje_inicio(b,ch,pant,"Temporizador a 60 Hz instalado.",SCRX,SCRY,BMAR1);
      else                   mensaje_inicio(b,ch,pant,"­Error al instalar temporizador!",SCRX,SCRY,BCO1);
      if(install_keyboard()==0) mensaje_inicio(b,ch,pant,"Control de teclado instalado",SCRX,SCRY,BMAR1);
      else                   mensaje_inicio(b,ch,pant,"­Error al instalar teclado!",SCRX,SCRY,BCO1);
      if(install_mouse()>-1) mensaje_inicio(b,ch,pant,"Instalado control de mouse de 2 botones",SCRX,SCRY,BMAR1);
      else                   mensaje_inicio(b,ch,pant,"No se ha encontrado mouse.",SCRX,SCRY,BCO1);
      if(install_joystick(JOY_TYPE_4BUTTON)==0){
         mensaje_inicio(b,ch,pant,"Control de joystick de 4 botones instalado",SCRX,SCRY,BMAR1);
         sprintf(frase,"%d joystick/s detectado/s.",num_joysticks);
         mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);
      }else mensaje_inicio(b,ch,pant,"No se han encontardo joysticks",SCRX,SCRY,BMAR1);
      if(install_sound(DIGI_AUTODETECT, MIDI_NONE, "")==0){
         sprintf(frase,"Tarjeta de sonido:%s",digi_driver->name);
         mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);
      } else mensaje_inicio(b,ch,pant,"­Error al inicializar tarjeta de sonido!",SCRX,SCRY,BCO1);
      reserve_voices(16,0);
      if(install_mod(6)>=0){
         sprintf(frase,"Reproductor JGMOD instalado con ‚xito.");
         mensaje_inicio(b,ch,pant,frase,SCRX,SCRY,BMAR1);
      } else mensaje_inicio(b,ch,pant,"­Error al inicializar JGMOD!",SCRX,SCRY,BCO1);

      mensaje_inicio(b,ch,pant,"Cargando sprites de batalla y paletaï",SCRX,SCRY,BMAR1);
      ch->carga_bitmaps_batalla(b);

      mensaje_inicio(b,ch,pant,"Pulsa una tecla para empezar",SCRX,SCRY,BROJO);
      readkey();
      mensaje_inicio(b,ch,pant,"Creando tablas de color. Esperaï",SCRX,SCRY,BROJO);
}
void parametros(int n, char *parm[], int &MODO, int &dif, byte &gt)
{
      int j;
      for(int i=1; i<n; i++){
         if(strcmp(parm[i],"-r")==0){
                if(strcmp(parm[i+1],"320")==0) MODO=0;
                if(strcmp(parm[i+1],"360")==0) MODO=1;
                if(strcmp(parm[i+1],"400")==0) MODO=2;
                if(strcmp(parm[i+1],"640")==0) MODO=3;
                if(strcmp(parm[i+1],"800")==0) MODO=4;
                if(strcmp(parm[i+1],"1024")==0) MODO=5;
                i++;};
         if(strcmp(parm[i],"-d")==0){
                j=atoi(parm[i+1]);
                if((j>=0) && (j<=5)) dif=j;
                i++;};
         if(strcmp(parm[i],"-coop")==0)
                gt=COOP;
         if(strcmp(parm[i],"-dmch")==0)
                gt=DMCH;
      };
}
void muestra_selec(int cx, int cy, int ix, int iy, int px, int py)
{
        int x,y, maxy, miny, maxx, minx;

        if(ix<px) {minx=ix; maxx=px;}
        else      {maxx=ix; minx=px;};
        if(iy<py) {miny=iy; maxy=py;}
        else      {maxy=iy; miny=py;};

        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++){

                x=(int)((SCRX/2)-15+(30*(j-cx))-(30*(i-cy)));
                y=(int)((SCRY/2)-7.5+(15*(j-cx))+(15*(i-cy)));
                draw_sprite(pant,tile,x,y);
        };
}
void tile_cursor(float cx, float cy, int &px, int &py)
{
        controlm cont;
        float vx=15.0, vy=7.5;
        int mx=SCRX/2, my=SCRY/2;
        float x=cont.x_raton(), y=cont.y_raton();

        px=(int)(((x-mx+vx)/(4*vx))+((y-my+vy)/(4*vy))+cx);
        py=(int)(((y-my+vy)/(2*vy))-px+cx+cy);
}
bool hot_spot(int x1, int y1, int x2, int y2)
{
        controlm c;

        if((c.x_raton()>=x1) && (c.x_raton()<=x2) && (c.y_raton()>=y1)
           && (c.y_raton()<=y2) && (c.b_raton(1))){
                do{}while(c.b_raton(1));
                return TRUE;}
        else  return FALSE;
}
int nearest40(int n)
{
        if(n%40==0) return n;
        else        return n+(40-(n%40));
}
void echo_string(BITMAP *bmp, char *str, int x, int y)
{
        rectfill(bmp,x-1,y-1,x+strlen(str)*13+2,y+13,C_SEL);
        ch->texto(bmp,str,x,y,C_FRG);
}
int menu(char *tit, char *op[], int nop)
{
        bool fin=FALSE, abort=FALSE, scroll=FALSE;
        int o=0, oin=0, ofin=nop, j;
        controlm cont;
        char aux[5];

        // Muchas opciones : Scroll
        if((nop+1)*20>SCRY){

                scroll=TRUE;
                ofin=(SCRY/20)-2; };

        int sx=strlen(tit)*13+40, sy=(ofin+2)*20;
        sx=nearest40(sx); sy=nearest40(sy);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2;

        blit(pant,back,0,0,0,0,SCRX,SCRY);

        do{
                if(keypressed())
                        switch(scroll){
                                case FALSE : switch(readkey()>>8){
                                        case KEY_UP   : if (o>0) o--; break;
                                        case KEY_DOWN : if (o<nop-1) o++; break;
                                        case KEY_ENTER: fin=TRUE; break;
                                        case KEY_ESC  : abort=TRUE; break;
                                        }; break;

                                case TRUE : switch(readkey()>>8){
                                        case KEY_PGUP : for(int k=0; k<ofin-oin; k++)
                                                        if (o>0){o--; if(o<oin){oin--; ofin--;}; }; break;
                                        case KEY_UP   : if (o>0){o--; if(o<oin){oin--; ofin--;}; }; break;
                                        case KEY_PGDN : for(int k=0; k<ofin-oin; k++)
                                                        if (o<nop-1){o++; if(o>=ofin){ofin++; oin++;}; }; break;
                                        case KEY_DOWN : if (o<nop-1){o++; if(o>=ofin){ofin++; oin++;}; }; break;
                                        case KEY_ENTER: fin=TRUE; break;
                                        case KEY_ESC  : abort=TRUE; break;
                                        }; break;
                                };

                if(cont.b_raton(2)) abort=TRUE;

                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+15,iy,C_SEL);

                for(int i=0; i<ofin-oin; i++){

                        j=i+oin;
                        if(j==o) rectfill(pant,ix+13,iy+20*(i+1)+2,ix+sx-14,iy+20*(i+2)-3,C_SEL);
                        ch->texto(pant,op[j],ix+15,iy+20*(i+1)+3,C_FRG);
                        if(hot_spot(ix+5,iy+20*(i+1),ix+sx-10,iy+20*(i+2))){
                                if(o!=j) o=j;
                                else     fin=TRUE;};
                };
                sprintf(aux,"%c",24);
                if(oin>0) ch->texto(pant,aux,ix+sx-30,iy+15,C_FRG);
                sprintf(aux,"%c",25);
                if(ofin<nop) ch->texto(pant,aux,ix+sx-30,iy+sy-30,C_FRG);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while((!fin) && (!abort));

        for(j=0; j<nop; j++) delete[] op[j];

        if(abort) return -1;
        else      return o;
}
void dialog_ok(char *tit)
{
        int sx=strlen(tit)*13+40, sy=80;
        sx=nearest40(sx);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2;
        bool fin=FALSE;
        controlm cont;
        blit(pant,back,0,0,0,0,SCRX,SCRY);
        do{
                if((keypressed()) || (hot_spot((SCRX/2)-10,iy+40,(SCRX/2)+11,iy+62)) || (cont.b_raton(2)))
                                  fin=TRUE;

                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+20,iy+20,C_FRG);
                draw_sprite(pant,button[BYES],(SCRX/2)-10,iy+40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);
}
bool dialog_yes_no(char *tit)
{
        int sx=strlen(tit)*13+40, sy=80;
        sx=nearest40(sx);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2;
        bool fin=FALSE;
        controlm cont;
        blit(pant,back,0,0,0,0,SCRX,SCRY);
        do{
                if((key[KEY_ESC]) || (cont.b_raton(2))) return FALSE;
                if((key[KEY_ENTER]) || (key[KEY_SPACE])) return TRUE;
                if(hot_spot((SCRX/2)-35,iy+40,(SCRX/2)-35+22,iy+40+21))
                        return TRUE;
                if(hot_spot((SCRX/2)+13,iy+40,(SCRX/2)+13+22,iy+40+21))
                        return FALSE;

                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+20,iy+20,C_FRG);
                draw_sprite(pant,button[BYES],(SCRX/2)-35,iy+40);
                draw_sprite(pant,button[BNO],(SCRX/2)+13,iy+40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(1);
}
int dialog_int(char *tit)
{
        int sx=strlen(tit)*13+40, sy=120, np=0;
        sx=nearest40(sx);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2;
        bool fin=FALSE;
        controlm cont;
        char numero[15]="\0",k;

        blit(pant,back,0,0,0,0,SCRX,SCRY);
        do{

                if(keypressed()){

                        k=readkey()>>8;
                        if((k>=KEY_1) && (k<=KEY_0) && (np<15-2)){

                                if(k==KEY_0) numero[np]='0';
                                else numero[np]=k-KEY_1+'1';
                                np++; numero[np]='\0';
                        };
                        if((k==KEY_BACKSPACE) && (np>0)) {np--; numero[np]='\0';};
                        if(k==KEY_ESC) return -999999;
                        if(k==KEY_ENTER) fin=TRUE;
                };
                if(hot_spot((SCRX/2)-35,iy+sy-40,(SCRX/2)-35+22,iy+sy-40+21))
                        fin=TRUE;
                if((hot_spot((SCRX/2)+13,iy+sy-40,(SCRX/2)+13+22,iy+sy-40+21)) || (cont.b_raton(2)))
                        return -999999;
                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+20,iy+20,C_FRG);
                echo_string(pant,numero,(SCRX/2)-(strlen(numero)*13/2),iy+50);
                draw_sprite(pant,button[BYES],(SCRX/2)-35,iy+sy-40);
                draw_sprite(pant,button[BNO],(SCRX/2)+13,iy+sy-40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);

        return atoi(numero);
}
int dialog_string(char *tit, char *dest, int maxs)
{
        int sx=maxs*13+40, maxms=maxs, sy=160, np=0;

        if(sx>SCRX){ sx=SCRX; maxms=(SCRX/13)-5;};
        sx=nearest40(sx);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2, j,c,k;
        bool fin=FALSE;
        controlm cont;
        char f[200];
        sprintf(dest,"");

        blit(pant,back,0,0,0,0,SCRX,SCRY);
        do{

                if(keypressed()){

                        c=readkey();
                        k=c>>8;
                        if((k==KEY_BACKSPACE) && (np>0)) {np--; dest[np]='\0';}
                        else if(k==KEY_ESC) return -1;
                        else if(k==KEY_ENTER) fin=TRUE;
                        else if(np<maxs){
                             dest[np]=c; np++; dest[np]='\0';
                        };
                };
                if(hot_spot((SCRX/2)-35,iy+sy-40,(SCRX/2)-35+22,iy+sy-40+21))
                        fin=TRUE;
                if((hot_spot((SCRX/2)+13,iy+sy-40,(SCRX/2)+13+22,iy+sy-40+21))|| (cont.b_raton(2)))
                        return -1;
                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+20,iy+20,C_FRG);
                j=0;
                do{
                        partir(dest,j,maxms,f);
                        if(strlen(f)>0)
                                echo_string(pant,f,ix+30,iy+50+(15*j));
                        j++;
                }while(strlen(f)>0);

                draw_sprite(pant,button[BYES],(SCRX/2)-35,iy+sy-40);
                draw_sprite(pant,button[BNO],(SCRX/2)+13,iy+sy-40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);

        return 1;
}
void dialog_error()
{
        if(last_error!=NO_ERR) dialog_ok(last_error_message);
        last_error=NO_ERR;
}
int elige_suelo()
{
        char *ops[20];
        int nops=10;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Vac¡o");
        sprintf(ops[1],"Da¤ino 1");
        sprintf(ops[2],"Da¤ino 2");
        sprintf(ops[3],"Mortal 1");
        sprintf(ops[4],"Mortal 2");
        sprintf(ops[5],"Suelo 1");
        sprintf(ops[6],"Suelo 2");
        sprintf(ops[7],"Suelo 3");
        sprintf(ops[8],"Suelo 4");
        sprintf(ops[9],"Suelo 5");

        return menu("Tipo de suelo",ops,nops);
}
int elige_pared()
{
        char *ops[20];
        int nops=20;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Horizontal 1");
        sprintf(ops[1],"Horizontal 2");
        sprintf(ops[2],"Horizontal 3");
        sprintf(ops[3],"Horizontal 4");
        sprintf(ops[4],"Horizontal 5");
        sprintf(ops[5],"Horizontal 6");
        sprintf(ops[6],"Horizontal 7 (A)");
        sprintf(ops[7],"Horizontal 8 (A)");
        sprintf(ops[8],"Horizontal 9 (A)");
        sprintf(ops[9],"Vertical 1");
        sprintf(ops[10],"Vertical 2");
        sprintf(ops[11],"Vertical 3");
        sprintf(ops[12],"Vertical 4");
        sprintf(ops[13],"Vertical 5");
        sprintf(ops[14],"Vertical 6");
        sprintf(ops[15],"Vertical 7 (A)");
        sprintf(ops[16],"Vertical 8 (A)");
        sprintf(ops[17],"Vertical 9 (A)");
        sprintf(ops[18],"Atravesable Hor.");
        sprintf(ops[19],"Atravesable Ver.");

        return PHO1+menu("  Tipos de paredes  ",ops,nops);
}
int elige_esquina()
{
        char *ops[20];
        int nops=9;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Cruz");
        sprintf(ops[1],"Izquierda-Abajo");
        sprintf(ops[2],"Derecha-Abajo");
        sprintf(ops[3],"Izquierda-Arriba");
        sprintf(ops[4],"Derecha-Arriba");
        sprintf(ops[5],"Arr-Izq-Aba");
        sprintf(ops[6],"Der-Arr-Izq");
        sprintf(ops[7],"Arr-Der-Aba");
        sprintf(ops[8],"Izq-Aba-Der");

        return ECRU+menu("  Tipos de esquina  ",ops,nops);
}
int elige_marco()
{
        char *ops[20];
        int nops=8;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Parte 1 Horiz.");
        sprintf(ops[1],"Parte 1 Vert.");
        sprintf(ops[2],"Parte 2 Horiz.");
        sprintf(ops[3],"Parte 2 Vert.");
        sprintf(ops[4],"Arco Horiz.");
        sprintf(ops[5],"Arco Vert.");
        sprintf(ops[6],"Travesa¤o Horiz.");
        sprintf(ops[7],"Travesa¤o Vert.");

        return DM1H+menu(" Marcos para puerta ",ops,nops);
}
int elige_otros()
{
        char *ops[25];
        int nops=24;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Interruptor");
        sprintf(ops[1],"Teleport");
        sprintf(ops[2],"Decoraci¢n 1");
        sprintf(ops[3],"Decoraci¢n 2");
        sprintf(ops[4],"Decoraci¢n 3");
        sprintf(ops[5],"Decoraci¢n 4");
        sprintf(ops[6],"Decoraci¢n 5");
        sprintf(ops[7],"Decoraci¢n 6");
        sprintf(ops[8],"Decoraci¢n 7 (A)");
        sprintf(ops[9],"Decoraci¢n 8 (A)");
        sprintf(ops[10],"Decoraci¢n 9 (A)");
        sprintf(ops[11],"Dec. Extra");
        sprintf(ops[12],"Columna 1");
        sprintf(ops[13],"Columna 2");
        sprintf(ops[14],"Columna 3");
        sprintf(ops[15],"[No Usable]");
        sprintf(ops[16],"Laser Post Arr.");
        sprintf(ops[17],"Laser Post Aba.");
        sprintf(ops[18],"Laser Post Izq.");
        sprintf(ops[19],"Laser Post Der.");
        sprintf(ops[20],"Tirador Arr.");
        sprintf(ops[21],"Tirador Aba.");
        sprintf(ops[22],"Tirador Izq.");
        sprintf(ops[23],"Tirador Der.");

        return INTE+menu("  Decorados y otros  ",ops,nops);
}
int elige_decor()
{
        char *ops[20];
        int nops=5;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Vac¡o");
        sprintf(ops[1],"Pared");
        sprintf(ops[2],"Esquina");
        sprintf(ops[3],"Marco de puerta");
        sprintf(ops[4],"Decor. y otros");

        switch(menu("Tipo de decorado",ops,nops)){

                case 0 : return VACIO; break;
                case 1 : return elige_pared(); break;
                case 2 : return elige_esquina(); break;
                case 3 : return elige_marco(); break;
                case 4 : return elige_otros(); break;
        };
}
int elige_item()
{
        char *ops[45];
        int nops=43;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Llave de Hierro");
        sprintf(ops[1],"Llave de Bronce");
        sprintf(ops[2],"Llave de Plata");
        sprintf(ops[3],"Llave de Oro");
        sprintf(ops[4],"Botiqu¡n");
        sprintf(ops[5],"Pistola");
        sprintf(ops[6],"Ametralladora");
        sprintf(ops[7],"Lanzacohetes");
        sprintf(ops[8],"Aniquilador");
        sprintf(ops[9],"\"Drag¢n\"");
        sprintf(ops[10],"\"Inferno\"");
        sprintf(ops[11],"Rifle de descargas");
        sprintf(ops[12],"Caja de balas");
        sprintf(ops[13],"Cohetes");
        sprintf(ops[14],"Frasco de napalm");
        sprintf(ops[15],"Daga");
        sprintf(ops[16],"Sable");
        sprintf(ops[17],"Espada");
        sprintf(ops[18],"Maza");
        sprintf(ops[19],"Martillo Divino");
        sprintf(ops[20],"\"µguila Dorada\"");
        sprintf(ops[21],"\"Filo Justiciero\"");
        sprintf(ops[22],"Escudo de piel");
        sprintf(ops[23],"Escudo de madera");
        sprintf(ops[24],"Escudo de acero");
        sprintf(ops[25],"Escudo de bronce");
        sprintf(ops[26],"Escudo de plata");
        sprintf(ops[27],"Escudo de oro");
        sprintf(ops[28],"\"Protector\"");
        sprintf(ops[29],"Energ¡a de fuego");
        sprintf(ops[30],"Energ¡a de agua");
        sprintf(ops[31],"Energ¡a de rayo");
        sprintf(ops[32],"Libro del Apocalipsis");
        sprintf(ops[33],"Invulnerabilidad");
        sprintf(ops[34],"Fuerza m¡stica");
        sprintf(ops[35],"Invisibilidad parcial");
        sprintf(ops[36],"Fluido de sombras");
        sprintf(ops[37],"Repulsi¢n");
        sprintf(ops[38],"Salud m xima");
        sprintf(ops[39],"Descarga el‚ctrica");
        sprintf(ops[40],"Mapa completo");
        sprintf(ops[41],"Veneno");
        sprintf(ops[42],"Protecci¢n divina");

        return KEY1+menu("   Ötems recogibles   ",ops,nops);
}
int elige_item_enemigo()
{
        char *ops[20];
        int nops=3;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Ninguno");
        sprintf(ops[1],"Aleatorio");
        sprintf(ops[2],"Otros");

        switch(menu("Item que lleva el personaje",ops,nops)){
                case 0 : return NING; break;
                case 1 : return RAND; break;
                case 2 : return elige_item(); break;};
        return NING;
}
int elige_direc()
{
        char *ops[20];
        int nops=4;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Derecha");
        sprintf(ops[1],"Abajo");
        sprintf(ops[2],"Izquierda");
        sprintf(ops[3],"Arriba");

        return menu("Elige una direcci¢n",ops,nops);
}
int elige_fspr()
{
        char *ops[20];
        int nops=NSPR;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        for(int i=0; i<NSPR; i++) sprintf(ops[i],niv->fspr(i));

        return menu("Elige un personaje (sprites y sonidos)",ops,nops);
}
int elige_clase()
{
        char *ops[20];
        int nops=4;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Soldado");
        sprintf(ops[1],"Hechicero");
        sprintf(ops[2],"Guerrero");
        sprintf(ops[3],"Secundario");

        return menu("Elige una clase",ops,nops);
}
int elige_subclase()
{
        char *ops[20];
        int nops=5;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Paseante");
        sprintf(ops[1],"Huyendo");
        sprintf(ops[2],"Indicador");
        sprintf(ops[3],"Prisionero");
        sprintf(ops[4],"Curioso");

        return menu("Elige una subclase para secundario",ops,nops);
}
int elige_arma_sigma()
{
        char *ops[20];
        int nops=8;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Pu¤os");
        sprintf(ops[1],"Pistola");
        sprintf(ops[2],"Ametralladora");
        sprintf(ops[3],"Lanzacohetes");
        sprintf(ops[4],"Aniquilador");
        sprintf(ops[5],"\"Drag¢n\"");
        sprintf(ops[6],"\"Inferno\"");
        sprintf(ops[7],"Rifle de descargas");

        return menu("Elige una arma de soldado",ops,nops);
}
int elige_arma_beta()
{
        char *ops[20];
        int nops=11;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Bast¢n");
        sprintf(ops[1],"Bola de fuego");
        sprintf(ops[2],"Repulsi¢n");
        sprintf(ops[3],"Pared de fuego");
        sprintf(ops[4],"Bola de hielo");
        sprintf(ops[5],"Invulnerabilidad");
        sprintf(ops[6],"Bloque de hielo");
        sprintf(ops[7],"Rayo");
        sprintf(ops[8],"Invisibilidad");
        sprintf(ops[9],"Descarga");
        sprintf(ops[10],"Apocalipsis");

        return menu("Elige un tipo de hechizo",ops,nops);
}
int elige_arma_omega()
{
        char *ops[20];
        int nops=8;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Pu¤os");
        sprintf(ops[1],"Daga");
        sprintf(ops[2],"Sable");
        sprintf(ops[3],"Espada");
        sprintf(ops[4],"Maza");
        sprintf(ops[5],"\"Martillo Divino\"");
        sprintf(ops[6],"\"µguila dorada\"");
        sprintf(ops[7],"\"Filo Justiciero\"");

        return menu("Elige una arma de guerrero",ops,nops);
}
int elige_escudo()
{
        char *ops[20];
        int nops=8;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Ninguno");
        sprintf(ops[1],"De piel");
        sprintf(ops[2],"De madera");
        sprintf(ops[3],"De acero");
        sprintf(ops[4],"De bronce");
        sprintf(ops[5],"De plata");
        sprintf(ops[6],"De oro");
        sprintf(ops[7],"\"Protector\"");

        return menu("Elige un tipo de escudo",ops,nops);
}
int elige_control()
{
        char *ops[20];
        int nops=6, aux;

        for(int i=0; i<nops; i++) ops[i]=new char[30];

        sprintf(ops[0],"Ninguno");
        sprintf(ops[1],"Cursor+RCTRL+ENTER+RSHIFT");
        sprintf(ops[2],"WASDTYUI");
        sprintf(ops[3],"Joystick/Pad 1");
        sprintf(ops[4],"Joystick/Pad 2");
        sprintf(ops[5],"Mouse");

        aux=menu("Elige un m‚todo de control",ops,nops);
        if(aux<0) aux=0;
        return aux;
}
byte elige_color(char *tit)
{
        int sx=16*16+40, sy=16*16+40, x,y;
        sx=nearest40(sx);
        sy=nearest40(sy);
        int ix=(SCRX-sx)/2, iy=(SCRY-sy)/2;
        bool fin=FALSE;
        controlm cont;
        byte col=0;
        blit(pant,back,0,0,0,0,SCRX,SCRY);
        do{
                if(hot_spot(ix+20,iy+20,ix+20+256,iy+20+256))
                                  fin=TRUE;

                blit(back,pant,0,0,0,0,SCRX,SCRY);
                ch->ventana(pant,ix,iy,ix+sx,iy+sy,C_BKG,C_FRG,FALSE);
                ch->texto(pant,tit,ix+20,iy,C_SEL);
                for(x=0; x<16; x++)
                for(y=0; y<16; y++)
                         rectfill(pant,ix+20+16*x,iy+20+16*y,ix+20+16*x+14,iy+20+16*y+14,(byte)(y*16)+x);

                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);

        col=((cont.x_raton()-ix-20)/16)+(16*((cont.y_raton()-iy-20)/16));
        return col;
}
void edita_suelo(int ix, int iy, int px, int py)
{
        int maxy, miny, maxx, minx;
        int su;

        if(ix<px) {minx=ix; maxx=px;}
        else      {maxx=ix; minx=px;};
        if(iy<py) {miny=iy; maxy=py;}
        else      {maxy=iy; miny=py;};

        su=elige_suelo();

        if(su<0) return;

        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++)
                mp->insb(j,i,(tbald)su);
}
void edita_decor(int ix, int iy, int px, int py)
{
        int maxy, miny, maxx, minx;
        int su;

        if(ix<px) {minx=ix; maxx=px;}
        else      {maxx=ix; minx=px;};
        if(iy<py) {miny=iy; maxy=py;}
        else      {maxy=iy; miny=py;};

        su=elige_decor();

        if(su<0) return;

        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++)
                mp->inse(j,i,(telem)su);
}
void edita_item(int ix, int iy, int px, int py)
{
        int maxy, miny, maxx, minx;
        int su;

        if(ix<px) {minx=ix; maxx=px;}
        else      {maxx=ix; minx=px;};
        if(iy<py) {miny=iy; maxy=py;}
        else      {maxy=iy; miny=py;};

        su=elige_item();

        if(su<0) return;

        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++)
                niv->ingresa_item(j,i,(t_item)su);
        dialog_error();
}
void edita_barrera(int ix, int iy, int px, int py)
{
        int maxy, miny, maxx, minx;
        int su;

        if(ix<px) {minx=ix; maxx=px;}
        else      {maxx=ix; minx=px;};
        if(iy<py) {miny=iy; maxy=py;}
        else      {maxy=iy; miny=py;};

        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++)
                mp->anyade_barrera(j,i,0);
        dialog_error();
}
int elige_coordenadas(char *tit,int &px,int &py)
{
      controlm cont;

      float cx=px, cy=py;
      char frase[20];

      speed_counter=0;

      do{
         tile_cursor(cx,cy,px,py);
         while(speed_counter>0){


                if(cont.b_raton(2)) {do{}while(cont.b_raton(2)); return -1;};
                if(hot_spot(SCRX-40,SCRY-40,SCRX-50+22,SCRY-40+21)) return -1;
                if(cont.b_raton(1)) {do{}while(cont.b_raton(1)); return 1;};

                if(keypressed())
                        switch(readkey()>>8){
                                case KEY_LEFT : if(cx>0) cx--; break;
                                case KEY_RIGHT: if(cx<MSIZEX-1) cx++; break;
                                case KEY_UP   : if(cy>0) cy--; break;
                                case KEY_DOWN : if(cy<MSIZEY-1) cy++; break;
                                case KEY_ESC  : return -1; break;
                                case KEY_ENTER: return  1; break;
                        };

                speed_counter--;
         }
        clear_to_color(pant,0);
        ch->muestra(pant,b,SCRX,SCRY,cx,cy);
        muestra_selec((int)cx,(int)cy,px,py,px,py);
        draw_sprite(pant,button[BNO],SCRX-50,SCRY-40);
        ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
        ch->texto(pant,tit,10,10,C_SEL);
        sprintf(frase,"Cursor:(%d,%d)",px,py);
        ch->texto(pant,frase,10,SCRY-20,C_FRG);
        blit(pant,screen,0,0,0,0,SCRX,SCRY);

      }while(1);
}
int elige_objeto(int x, int y,int tipo, int cual)
{
      controlm cont;

      char frase[40], aux[20];

      speed_counter=0;

      do{
         while(speed_counter>0){

                if(hot_spot(SCRX-400,SCRY-40,SCRX-400+7*13,SCRY-40+11)) return 0;
                if(hot_spot(SCRX-250,SCRY-40,SCRX-250+7*13,SCRY-40+11)) return 1;
                if(hot_spot(SCRX-100,SCRY-40,SCRX-100+22,SCRY-40+21)) return 2;
                if(hot_spot(SCRX-50,SCRY-40,SCRX-50+22,SCRY-40+21)) return 3;

                if(keypressed())
                        switch(readkey()>>8){
                                case KEY_LEFT : return 0; break;
                                case KEY_RIGHT: return 1; break;
                                case KEY_ENTER: return 2; break;
                                case KEY_ESC  : return 3; break;
                        };

                speed_counter--;
         }
        clear_to_color(pant,0);
        ch->muestra(pant,b,SCRX,SCRY,x,y);
        draw_sprite(pant,button[BNO],SCRX-50,SCRY-40);
        draw_sprite(pant,button[BYES],SCRX-100,SCRY-40);
        echo_string(pant,"<Anter.",SCRX-400,SCRY-40);
        echo_string(pant,"Sigui.>",SCRX-250,SCRY-40);
        ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
        switch(tipo){
            case 0 : sprintf(aux,"Teleport"); break;
            case 1 : sprintf(aux,"Puerta"); break;
            case 2 : sprintf(aux,"µrea oscura"); break;
            case 3 : sprintf(aux,"µrea secreta"); break;
            case 4 : sprintf(aux,"Barrera"); break;
            case 5 : sprintf(aux,"Disparador"); break;
            case 6 : sprintf(aux,"Puente"); break;
            case 7 : sprintf(aux,"L ser"); break;
            case 8 : sprintf(aux,"Interruptor"); break;
            case 9 : sprintf(aux,"Item recogible"); break;
            case 10 : sprintf(aux,"Fuente de sonido"); break;
            case 11 : sprintf(aux,"Enemigo"); break;
        };
        sprintf(frase,"Eliminar: %s %d",aux,cual);
        ch->texto(pant,frase,10,10,C_SEL);
        muestra_selec(x,y,x,y,x,y);
        sprintf(frase,"Cursor:(%d,%d)",x,y);
        ch->texto(pant,frase,10,SCRY-20,C_FRG);
        blit(pant,screen,0,0,0,0,SCRX,SCRY);

      }while(1);
}
void edita_tirador(int px, int py)
{
        int b, tim, d, fu, ix=px, iy=py;
        char *ops[20], frase[60];
        int nops=6;
        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Bala");
        sprintf(ops[1],"Cohete");
        sprintf(ops[2],"Bola de fuego");
        sprintf(ops[3],"Hielo");
        sprintf(ops[4],"Electricidad");
        sprintf(ops[5],"Llamarada");

        if((b=menu("Tipo de proyectil",ops,nops))>=0){

                if((d=elige_direc())>=0){

                        if((fu=dialog_int("Fuerza del proyectil"))>=0){

                                nops=2;
                                for(int i=0; i<nops; i++) ops[i]=new char[20];
                                sprintf(ops[0],"Temporizado");
                                sprintf(ops[1],"Con interruptor");

                                switch(menu("Modo de activaci¢n",ops,nops)){

                                        case -1 : break;
                                        case  0 : tim=dialog_int("Tiempo de espera (en 1/60 seg.)");
                                                  if(tim>=0){

                                                        mp->anyade_disp_auto(px,py,(w_dir)d,b,fu,tim);
                                                        dialog_error();
                                                        switch(d){

                                                                case DARR : mp->inse(px,py,TIAR); break;
                                                                case DABA : mp->inse(px,py,TIAB); break;
                                                                case DDER : mp->inse(px,py,TIDE); break;
                                                                case DIZQ : mp->inse(px,py,TIIZ); break;
                                                        };
                                                        dialog_error();
                                                        sprintf(frase,"Nuevo tirador temporizado en (%d,%d)",px,py);
                                                        dialog_ok(frase);
                                                  };
                                                  break;
                                        case  1 : if(elige_coordenadas("Posici¢n del interruptor",ix,iy)>=0){

                                                        mp->anyade_disp_interr(px,py,ix,iy,(w_dir)d,b,fu);
                                                        dialog_error();
                                                        switch(d){

                                                                case DARR : mp->inse(px,py,TIAR); break;
                                                                case DABA : mp->inse(px,py,TIAB); break;
                                                                case DDER : mp->inse(px,py,TIDE); break;
                                                                case DIZQ : mp->inse(px,py,TIIZ); break;
                                                        };
                                                        mp->inse(ix,iy,INTE);
                                                        dialog_error();
                                                        sprintf(frase,"Nuevo tirador activable en (%d,%d)",px,py);
                                                        dialog_ok(frase);
                                                }; break;
                                        };
                                };
                        };
                };
}
void edita_puerta(int ix, int iy, int px, int py)
{
        int t, iix=ix, iiy=iy;
        char *ops[20], frase[60];
        int nops=6;
        for(int i=0; i<nops; i++) ops[i]=new char[20];

        if(ix>px){ t=ix; ix=px; px=t;};
        if(iy>py){ t=iy; iy=py; py=t;};

        sprintf(ops[0],"Normal");
        sprintf(ops[1],"De llave de hierro");
        sprintf(ops[2],"De llave de bronce");
        sprintf(ops[3],"De llave de plata");
        sprintf(ops[4],"De llave de oro");
        sprintf(ops[5],"Con interruptor");

        switch(t=menu("    Tipo de puerta    ",ops,nops)){

                case -1 : break;
                case  0 : mp->anyade_puerta_normal(ix,iy,px,py);
                          sprintf(frase,"Nueva puerta de (%d,%d) a (%d,%d)",ix,iy,px,py);
                          dialog_ok(frase); break;
                case  1 :
                case  2 :
                case  3 :
                case  4 : mp->anyade_puerta_llave(ix,iy,px,py,(tpuerta)t);
                          sprintf(frase,"Nueva puerta con llave de (%d,%d) a (%d,%d)",ix,iy,px,py);
                          dialog_ok(frase); break;
                case  5 : if(elige_coordenadas("Posici¢n del interruptor",iix,iiy)>=0){

                                switch(dialog_yes_no("¨Cierre autom tico?")){

                                        case FALSE : mp->anyade_puerta_interr(ix,iy,px,py,iix,iiy,TRUE); break;
                                        case TRUE  : mp->anyade_puerta_interr(ix,iy,px,py,iix,iiy,FALSE); break;
                                        };

                                mp->inse(iix,iiy,INTE);
                                sprintf(frase,"Nueva puerta con interr. de (%d,%d) a (%d,%d)",ix,iy,px,py);
                                dialog_ok(frase);
                          }; break;
        };
        dialog_error();
}
void edita_puente(int ix, int iy, int px, int py)
{
        int t, iix=ix, iiy=iy;
        char frase[60];

        if(elige_coordenadas("Posici¢n del interruptor",iix,iiy)>=0){

                if(dialog_yes_no("¨Cierre autom tico?")==TRUE){

                        if((t=dialog_int("Tiempo de espera (en 1/60 seg.)"))>=0){

                                mp->anyade_puente_interr(ix,iy,px,py,iix,iiy,TRUE,t);
                                mp->inse(iix,iiy,INTE);
                                sprintf(frase,"Nuevo puente con interr. de (%d,%d) a (%d,%d)",ix,iy,px,py);
                                dialog_ok(frase);
                        };
                }else{
                        mp->anyade_puente_interr(ix,iy,px,py,iix,iiy,FALSE,0);
                        mp->inse(iix,iiy,INTE);
                        sprintf(frase,"Nuevo puente con interr. de (%d,%d) a (%d,%d)",ix,iy,px,py);
                        dialog_ok(frase);
                };
        };
        dialog_error();
}
void edita_fuente_sonido(int px, int py)
{
        int t;
        char frase[60];

        do{
            t=dialog_int("Sample asociado (0-3)");
        }while(t>=N_SMP_AMBI);
        if(t>=0){
                mp->anyade_sonido_amb(px,py,t);
                sprintf(frase,"Nueva fuente de sonido en (%d,%d) de sample %d",px,py,t);
                dialog_ok(frase);
        };
        dialog_error();
}
void edita_laser(int ix, int iy, int px, int py)
{
        char frase[60];
        int iix=ix, iiy=iy;
        if(elige_coordenadas("Posici¢n del interruptor",iix,iiy)>=0){

                mp->anyade_laser(ix,iy,px,py,iix,iiy);
                if(ix<px){
                        mp->inse(ix,iy,LPIZ);
                        mp->inse(px,py,LPDE);
                }else if (ix>px){
                        mp->inse(ix,iy,LPDE);
                        mp->inse(px,py,LPIZ);
                }else if (ix==px){
                        if(iy<py){
                                mp->inse(ix,iy,LPAR);
                                mp->inse(px,py,LPAB);
                        }else{
                                mp->inse(ix,iy,LPAB);
                                mp->inse(px,py,LPAR);
                        };
                };
                mp->inse(iix,iiy,INTE);
                sprintf(frase,"Nuevo l ser de (%d,%d) a (%d,%d)",ix,iy,px,py);
                dialog_ok(frase);
                dialog_error();
        };
}
void edita_oscu(int ix, int iy, int px, int py)
{
        char frase[60];
        int iix=ix, iiy=iy;

        switch(dialog_yes_no("¨Activable con interruptor?")){

                case FALSE :    mp->anyade_area_oscura(ix,iy,px,py);
                                sprintf(frase,"Nueva  rea oscura de (%d,%d) a (%d,%d)",ix,iy,px,py);
                                dialog_ok(frase);
                                break;

                case TRUE  : if(elige_coordenadas("Posici¢n del interruptor",iix,iiy)>=0){
                                mp->anyade_area_oscura_interr(ix,iy,px,py,iix,iiy);
                                mp->inse(iix,iiy,INTE);
                                sprintf(frase,"Nueva  rea oscura activable de (%d,%d) a (%d,%d)",ix,iy,px,py);
                                dialog_ok(frase);
                             }; break;
        };
        dialog_error();
}
void muestra_datos_enem(warrior *en)
{
        float posx=en->fx_pos(), posy=en->fy_pos();
        char
             s_dir[4][10]={"Derecha","Abajo","Izquierda","Arriba"},
             s_clase[4][15]={"Soldado","Hechicero","Guerrero","Secundario"},
             s_casual[7][15]={"Paseante","Huyendo","Indicador","Prisionero","Curioso","#ERROR#","#ERROR#"},
             s_boss[7][10]={"#ERROR#","#ERROR#","#ERROR#","#ERROR#","#ERROR#","Normal","Boss"},
             s_arma[3][12][20]={{"Pu¤os","Pistola","Ametralladora","Lanzacohetes","Aniquilador","\"Drag¢n\"","\"Inferno\"","Rifle de descargas"},
                                {"Bast¢n","Bola de fuego","Repulsi¢n","Pared de Fuego","Bola de hielo","Invisibilidad","Bloque de hielo","Rayo","Invisibilidad","Descarga","Apocalipsis"},
                                {"Pu¤os","Daga","Sable","Espada","Maza","\"Martillo divino\"","\"µguila Dorada\"","\"Filo Justiciero\""}},
             s_escudo[8][15]={"Ninguno","De piel","De madera","De acero","De bronce","De plata","De oro","\"Protector\""},
             s_bando[2][10]={"Aliado","Enemigo"},
             aux[100];
             item *aux_item=NULL;

                aux_item=new item(posx+5,posy+6,en->item_asociado());

                ch->texto(pant,"Nombre:",10,50,C_SEL);
                echo_string(pant,en->nombre(),10,70);
                ch->texto(pant,"Frase:",10,100,C_SEL);
                echo_string(pant,en->frase(),10,120);
                ch->texto(pant,"Direcci¢n:",10,150,C_SEL);
                echo_string(pant,s_dir[en->dir()],10,170);
                ch->texto(pant,"Sprite:",10,200,C_SEL);
                echo_string(pant,niv->fspr(en->cualspr()),10,220);
                ch->texto(pant,"Salud:",10,250,C_SEL);
                sprintf(aux,"%d",en->salud());
                echo_string(pant,aux,10,270);
                ch->texto(pant,"Clase:",10,300,C_SEL);
                echo_string(pant,s_clase[en->clase()],10,320);
                ch->texto(pant,"Subclase:",10,350,C_SEL);
                if(en->clase()==CASUAL) echo_string(pant,s_casual[en->subclase()],10,370);
                else                   echo_string(pant,s_boss[en->subclase()],10,370);
                if(en->clase()!=CASUAL){
                        ch->texto(pant,"Arma:",200,150,C_SEL);
                        echo_string(pant,s_arma[en->clase()][en->arma()],200,170);};
                if(en->clase()==OMEGA){
                        ch->texto(pant,"Escudo:",200,200,C_SEL);
                        echo_string(pant,s_escudo[en->escudo()],200,220);};
                ch->texto(pant,"Bando:",200,250,C_SEL);
                echo_string(pant,s_bando[en->bando()],200,270);
                ch->texto(pant,"Color 1:",200,300,C_SEL);
                rectfill(pant,200,320,300,333,en->color(1));
                ch->texto(pant,"Color 2:",200,350,C_SEL);
                rectfill(pant,200,370,300,383,en->color(2));
                ch->texto(pant,"Item asociado:",400,250,C_SEL);
                rectfill(pant,420,270,520,320,C_SEL);
                ch->muestra_item(pant,aux_item,posx-12,posy+4,posx,posy,TRUE,0);
                ch->muestra_guerrero(pant,en,posx-12,posy+4,posx,posy,TRUE);
}
void edita_enemigo(int px, int py)
{
        warrior *en;
        float posx=px+0.5, posy=py+0.5;
        char nombre[16]="Sin nombre", frase[100]="Sin frase", aux[100];
        w_dir dir=DABA;
        byte cual_spr=0, arma=0, escudo=0, salud=100, bando=1, c1=BCO1, c2=BCO2;
        w_clase clase=SIGMA;
        w_sclase subclase=ENORM;
        t_item it=NING;
        en=NULL;
        bool fin=FALSE, ok=TRUE;
        controlm cont;
        int iaux;

        do{
                if(en!=NULL) delete en;
                en=new warrior(nombre,posx,posy,dir,cual_spr,salud,arma,escudo,clase,subclase,bando,c1,c2,it,frase);
                if(hot_spot(SCRX-50,SCRY-40,SCRX-50+22,SCRY-40+21))
                        {fin=TRUE; ok=FALSE;};
                if(hot_spot(SCRX-100,SCRY-40,SCRX-100+22,SCRY-40+21))
                        fin=TRUE;
                if(hot_spot(10,70,10+100,70+13))
                        if(dialog_string("Nombre del enemigo",aux,16)>=0)
                                sprintf(nombre,"%s",aux);
                if(hot_spot(10,120,10+100,120+13))
                        if(dialog_string("Frase al hablar",aux,100)>=0)
                                sprintf(frase,"%s",aux);
                if(hot_spot(10,170,10+100,170+13)){
                        iaux=elige_direc();
                        if(iaux>=0) dir=(w_dir)iaux;
                };
                if(hot_spot(10,220,10+100,220+13)){
                        iaux=elige_fspr();
                        if(iaux>=0) cual_spr=iaux;
                };
                if(hot_spot(10,270,10+100,270+13)){
                        iaux=dialog_int("Puntos de salud (0-255)");
                        if(iaux>=0) salud=iaux;
                };
                if(hot_spot(10,320,10+100,320+13)){
                        iaux=elige_clase();
                        if(iaux>=0) clase=(w_clase) iaux;
                };
                if(hot_spot(10,370,10+100,370+13)){
                        if(en->clase()==CASUAL)
                            subclase=(w_sclase) elige_subclase();
                        else if(dialog_yes_no("¨El enemigo es un Boss?"))
                             subclase=EBOSS;
                             else
                             subclase=ENORM;
                };
                if((hot_spot(200,170,200+100,170+13)) && (en->clase()!=CASUAL)){
                        switch(en->clase()){
                                case SIGMA : iaux=elige_arma_sigma(); break;
                                case BETA  : iaux=elige_arma_beta(); break;
                                case OMEGA : iaux=elige_arma_omega(); break;};
                        if(iaux>=0) arma=iaux;
                };
                if((hot_spot(200,220,200+100,220+13)) && (en->clase()==OMEGA)){
                        iaux=elige_escudo();
                        if(iaux>=0) escudo=iaux;};
                if(hot_spot(200,270,200+100,270+13)){
                        if(dialog_yes_no("¨Es un aliado?"))
                             bando=0;
                             else
                             bando=1;
                };
                if(hot_spot(200,320,200+100,320+13))
                        c1=elige_color("Color base 1");
                if(hot_spot(200,370,200+100,370+13))
                        c2=elige_color("Color base 2");
                if(hot_spot(420,270,520,320)){
                        iaux=(t_item) elige_item_enemigo();
                        if(iaux>=0) it=(t_item)iaux;
                };
                clear_to_color(pant,0);
                ch->texto(pant,"Creaci¢n de enemigos",10,10,C_SEL);
                muestra_datos_enem(en);
                draw_sprite(pant,button[BNO],SCRX-50,SCRY-40);
                draw_sprite(pant,button[BYES],SCRX-100,SCRY-40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);
        if(ok) niv->ingresa_enem(en);
        b->ingreso_o_retira_enem(en->x_pos(),en->y_pos(),+1);
        dialog_error();
}
void modifica_enemigos()
{
        warrior *en, *ori;
        float posx, posy;
        char nombre[16]="Sin nombre", frase[100]="Sin frase", aux[100];
        w_dir dir=DABA;
        byte cual_spr=0, arma=0, escudo=0, salud=100, bando=1, c1=BCO1, c2=BCO2;
        w_clase clase=SIGMA;
        w_sclase subclase=ENORM;
        t_item it=NING;
        en=NULL;
        bool fin=FALSE, ok=TRUE;
        controlm cont;
        int iaux, e_actual=0, px=10, py=10;

        en=new warrior(nombre,posx,posy,dir,cual_spr,salud,arma,escudo,clase,subclase,bando,c1,c2,it,frase);
        do{
                // Carga el enemigo original

                ori=niv->dev_enem(e_actual);
                if(ori==NULL) return;
                posx=ori->fx_pos();
                posy=ori->fy_pos();
                sprintf(nombre,"%s",ori->nombre());
                sprintf(frase,"%s",ori->frase());
                dir=ori->dir();
                cual_spr=ori->cualspr();
                salud=ori->salud();
                arma=ori->arma();
                escudo=ori->escudo();
                clase=ori->clase();
                subclase=ori->subclase();
                bando=ori->bando();
                c1=ori->color(1);
                c2=ori->color(2);
                it=ori->item_asociado();

                if(hot_spot(SCRX-100,SCRY-40,SCRX-100+22,SCRY-40+21))
                        fin=TRUE;
                if(hot_spot(10,70,10+100,70+13))
                        if(dialog_string("Nombre del enemigo",aux,16)>=0)
                                sprintf(nombre,"%s",aux);
                if(hot_spot(10,120,10+100,120+13))
                        if(dialog_string("Frase al hablar",aux,100)>=0)
                                sprintf(frase,"%s",aux);
                if(hot_spot(10,170,10+100,170+13)){
                        iaux=elige_direc();
                        if(iaux>=0) dir=(w_dir)iaux;
                };
                if(hot_spot(10,220,10+100,220+13)){
                        iaux=elige_fspr();
                        if(iaux>=0) cual_spr=iaux;
                };
                if(hot_spot(10,270,10+100,270+13)){
                        iaux=dialog_int("Puntos de salud (0-255)");
                        if(iaux>=0) salud=iaux;
                };
                if(hot_spot(10,320,10+100,320+13)){
                        iaux=elige_clase();
                        if(iaux>=0) clase=(w_clase) iaux;
                };
                if(hot_spot(10,370,10+100,370+13)){
                        if(en->clase()==CASUAL)
                            subclase=(w_sclase) elige_subclase();
                        else if(dialog_yes_no("¨El enemigo es un Boss?"))
                             subclase=EBOSS;
                             else
                             subclase=ENORM;
                };
                if((hot_spot(200,170,200+100,170+13)) && (en->clase()!=CASUAL)){
                        switch(en->clase()){
                                case SIGMA : iaux=elige_arma_sigma(); break;
                                case BETA  : iaux=elige_arma_beta(); break;
                                case OMEGA : iaux=elige_arma_omega(); break;};
                        if(iaux>=0) arma=iaux;
                };
                if((hot_spot(200,220,200+100,220+13)) && (en->clase()==OMEGA)){
                        iaux=elige_escudo();
                        if(iaux>=0) escudo=iaux;};
                if(hot_spot(200,270,200+100,270+13)){
                        if(dialog_yes_no("¨Es un aliado?"))
                             bando=0;
                             else
                             bando=1;
                };
                if(hot_spot(200,320,200+100,320+13))
                        c1=elige_color("Color base 1");
                if(hot_spot(200,370,200+100,370+13))
                        c2=elige_color("Color base 2");
                if(hot_spot(420,270,520,320)){
                        iaux=(t_item) elige_item_enemigo();
                        if(iaux>=0) it=(t_item)iaux;
                };
                clear_to_color(pant,0);
                sprintf(aux,"Modificaci¢n : Enemigo %d",e_actual);
                ch->texto(pant,aux,10,10,C_SEL);
                muestra_datos_enem(ori);
                echo_string(pant,"<<",SCRX-200,SCRY-40);
                echo_string(pant,">>",SCRX-150,SCRY-40);
                draw_sprite(pant,button[BYES],SCRX-100,SCRY-40);
                echo_string(pant,"Copia",SCRX-70,SCRY-40);
                ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
                blit(pant,screen,0,0,0,0,SCRX,SCRY);

                en=new warrior(nombre,posx,posy,dir,cual_spr,salud,arma,escudo,clase,subclase,bando,c1,c2,it,frase);
                niv->cambia_enem(en,e_actual);
                if((hot_spot(SCRX-200,SCRY-40,SCRX-200+26,SCRY-40+13)) || (key[KEY_LEFT]))
                        if(e_actual>0) e_actual--;
                if((hot_spot(SCRX-150,SCRY-40,SCRX-150+26,SCRY-40+13)) || (key[KEY_RIGHT]))
                        if(niv->dev_enem(e_actual+1)!=NULL) e_actual++;
                if(hot_spot(SCRX-70,SCRY-40,SCRX-70+5*13,SCRY-40+13)){
                        if(elige_coordenadas("Elige el lugar",px,py)>=0){
                                posx=(float)px;
                                posy=(float)py;
                                ori=new warrior(nombre,posx+0.5,posy+0.5,dir,cual_spr,salud,arma,escudo,clase,subclase,bando,c1,c2,it,frase);
                                niv->ingresa_enem(ori);
                                b->ingreso_o_retira_enem(px,py,+1);
                                while(niv->dev_enem(e_actual+1)!=NULL) e_actual++;
                        };
                };
        }while(!fin);
        dialog_error();
}
void edita_misc()
{
       char aux[100];
       bool fin=FALSE;
       controlm cont;
       float cr;

       do{
                if(hot_spot(SCRX-100,SCRY-40,SCRX-100+22,SCRY-40+21))
                        fin=TRUE;
                if(hot_spot(10,70,10+100,70+13))
                        if(dialog_string("T¡tulo del nivel",aux,MENSZ)>=0)
                                niv->cambia_nombre(aux);
                if(hot_spot(10,120,10+100,120+13))
                        if(dialog_string("Fichero",aux,8)>=0)
                                sprintf(FILENAME,"%s",aux);
                if(hot_spot(10,170,110,183)){
                        niv->cambia_color_luz(elige_color("Color de luz"));
                        if(dialog_yes_no("¨Reconstruir tablas de color?"))
                        ch->crea_tablas_color(b->dev_nivel());
                        };
                if(hot_spot(120,170,220,233)){
                        niv->cambia_color_sombra(elige_color("Color de sombra"));
                        if(dialog_yes_no("¨Reconstruir tablas de color?"))
                        ch->crea_tablas_color(b->dev_nivel());
                        };
                if(hot_spot(10,220,10+100,220+13))
                        if(dialog_string("Fichero de m£sica de fondo",aux,MENSZ)>=0){
                                niv->cambia_fmusic(aux);
                                if(dialog_yes_no("¨Cargar la nueva m£sica?"))
                                        s->load_bgm(GDIR,niv->fmusic());
                        };
                if(hot_spot(10+6*13,320,10+6*13+50,320+11)){
                        do{
                                cr=(float)(dialog_int("Coeficiente de rojos (0-200)"))/100.0;
                        }while((cr<0) || (cr>200));
                        niv->cambia_coef_paleta(cr,niv->coefg(),niv->coefb());
                        ch->carga_bitmaps_batalla(b);
                };
                if(hot_spot(10+6*13,340,10+6*13+50,340+11)){
                        do{
                                cr=(float)(dialog_int("Coeficiente de verdes (0-200)"))/100.0;
                        }while((cr<0) || (cr>200));
                        niv->cambia_coef_paleta(niv->coefr(),cr,niv->coefb());
                        ch->carga_bitmaps_batalla(b);
                };
                if(hot_spot(10+6*13,360,10+6*13+50,360+11)){
                        do{
                                cr=(float)(dialog_int("Coeficiente de azules (0-200)"))/100.0;
                        }while((cr<0) || (cr>200));
                        niv->cambia_coef_paleta(niv->coefr(),niv->coefg(),cr);
                        ch->carga_bitmaps_batalla(b);
                };
                if(hot_spot(10,270,10+100,270+13)){
                        if(dialog_string("Fichero",aux,8)>=0){
                                niv->cambia_fdecor(aux);
                                if(dialog_yes_no("¨Cargar el nuevo decorado?"))
                                        ch->carga_bitmaps_batalla(b);
                        };
                };
                for(int i=0; i<NSPR; i++)
                if(hot_spot(350,120+20*i,350+100,120+(20*(i+1)))){
                        if(dialog_string("Fichero",aux,8)>=0){
                                niv->cambia_fspr(aux,i);
                                if(dialog_yes_no("¨Cargar los nuevos sprites?"))
                                        ch->carga_bitmaps_batalla(b);
                        };
                };

               clear_to_color(pant,0);
               ch->texto(pant,"Miscel nea del nivel",10,10,C_SEL);
               ch->texto(pant,"T¡tulo:",10,50,C_SEL);
               echo_string(pant,niv->nombre(),10,70);
               ch->texto(pant,"Nombre del fichero:",10,100,C_SEL);
               echo_string(pant,FILENAME,10,120);
               ch->texto(pant,"Colores de luz/oscuridad:",10,150,C_SEL);
               rectfill(pant,9,169,111,184,C_SEL);
               rectfill(pant,10,170,110,183,niv->color_luz());
               rectfill(pant,119,169,221,184,C_SEL);
               rectfill(pant,120,170,220,183,niv->color_sombra());
               ch->texto(pant,"Fichero de m£sica:",10,200,C_SEL);
               echo_string(pant,niv->fmusic(),10,220);
               ch->texto(pant,"Fichero de decorado:",10,250,C_SEL);
               echo_string(pant,niv->fdecor(),10,270);
               ch->texto(pant,"Coeficientes de paleta:",10,300,C_SEL);
               ch->texto(pant,"Rojo :",10,320,C_SEL);
               sprintf(aux,"%d",(int)(niv->coefr()*100)); echo_string(pant,aux,10+6*13,320);
               ch->texto(pant,"Verde:",10,340,C_SEL);
               sprintf(aux,"%d",(int)(niv->coefg()*100)); echo_string(pant,aux,10+6*13,340);
               ch->texto(pant,"Azul :",10,360,C_SEL);
               sprintf(aux,"%d",(int)(niv->coefb()*100)); echo_string(pant,aux,10+6*13,360);

               ch->texto(pant,"Ficheros de sprites:",350,100,C_SEL);
               for(int i=0; i<NSPR; i++)
                      echo_string(pant,niv->fspr(i),350,120+20*i);
               draw_sprite(pant,button[BYES],SCRX-100,SCRY-40);
               ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
               blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);
}
void informacion()
{
       bool fin=FALSE;
       controlm cont;
       char aux[100];
       int tel, puer, puen, osc, sec, bar, las, dis, inte, ite, enem, ambi;
       do{
               if(hot_spot(SCRX-100,SCRY-40,SCRX-100+22,SCRY-40+21) || (key[KEY_ENTER]))
                        fin=TRUE;
               clear_to_color(pant,0);
               ch->texto(pant,NOMBRE_MODULO,10,10,C_SEL);
               ch->texto(pant,"Informaci¢n del nivel en curso:",10,30,C_SEL);

               niv->dev_mapa()->info(tel,puen,puer,inte,osc,sec,bar,dis,las,ambi);
               niv->info(ite,enem);
               sprintf(aux,"Teleports : %d/%d",tel,M_NTELE); ch->texto(pant,aux,20,70,C_FRG);
               sprintf(aux,"Puentes : %d/%d",puen,M_NPUEN); ch->texto(pant,aux,20,90,C_FRG);
               sprintf(aux,"Puertas : %d/%d",puer,M_NPUER); ch->texto(pant,aux,20,110,C_FRG);
               sprintf(aux,"Interruptores : %d/%d",inte,M_NINTE); ch->texto(pant,aux,20,130,C_FRG);
               sprintf(aux,"µreas oscuras : %d/%d",osc,M_NOSCU); ch->texto(pant,aux,20,150,C_FRG);
               sprintf(aux,"µreas secretas : %d/%d",sec,M_NSECR); ch->texto(pant,aux,20,170,C_FRG);
               sprintf(aux,"Barreras : %d/%d",bar,M_NBARR); ch->texto(pant,aux,20,190,C_FRG);
               sprintf(aux,"Disparadores : %d/%d",dis,M_NDISP); ch->texto(pant,aux,20,210,C_FRG);
               sprintf(aux,"Barreras l ser : %d/%d",las,M_NLASE); ch->texto(pant,aux,20,230,C_FRG);
               sprintf(aux,"Ötems : %d/%d",ite,L_NITEM); ch->texto(pant,aux,20,250,C_FRG);
               sprintf(aux,"Enemigos : %d/%d",enem,L_NENEM); ch->texto(pant,aux,20,270,C_FRG);
               sprintf(aux,"Fuentes de sonido : %d/%d",ambi,M_NAMBI); ch->texto(pant,aux,20,290,C_FRG);
               sprintf(aux,"Inicio : [%d,%d]",niv->dev_mapa()->x_inicio(),niv->dev_mapa()->y_inicio());

               ch->texto(pant,aux,20,310,C_FRG);
               sprintf(aux,"Memoria disponible : %d Mb",(int)((float)(_go32_dpmi_remaining_physical_memory())/1000000));
               ch->texto(pant,aux,20,330,C_FRG);
               draw_sprite(pant,button[BYES],SCRX-100,SCRY-40);
               ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
               blit(pant,screen,0,0,0,0,SCRX,SCRY);

        }while(!fin);
}
void info_tile(int px, int py)
{
        char *ops[20], frase[40];
        int nops=2, op, x=px, y=py;

        ops[0]=new char[20];
        sprintf(ops[0],"Baldosa  :%d",niv->dev_mapa()->consb(x,y));
        ops[1]=new char[20];
        sprintf(ops[1],"Decorado :%d",niv->dev_mapa()->conse(x,y));

        if((op=niv->dev_mapa()->hay_telep(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Teleport n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_puente_on_o_off(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Puente n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_puerta(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Puerta n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_interr(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Interruptor n§%d:->(%d,%d)",op,
                                               niv->dev_mapa()->dev_interr(niv->dev_mapa()->hay_interr(x,y))->cobjx(),
                                               niv->dev_mapa()->dev_interr(niv->dev_mapa()->hay_interr(x,y))->cobjy());
                nops++;
        };
        if((op=niv->dev_mapa()->hay_oscu(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"µrea oscura n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_secr(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"µrea secreta n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_barr(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Barrera n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_disp(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Disparador n§%d",op);
                nops++;
        };
        if((op=niv->dev_mapa()->hay_laser(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"L ser n§%d",op);
                nops++;
        };
        if((op=niv->primer_item(x,y))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Item n§%d",op);
                nops++;
        };
        if((op=niv->hay_enem(x,y,0))>=0){
                ops[nops]=new char[20];
                sprintf(ops[nops],"Enemigo n§%d:%s",op,niv->dev_enem(op)->nombre());
                nops++;
        };

        sprintf(frase,"Datos de baldosa [%d,%d]",x,y);
        menu(frase,ops,nops);
}
void copy_area(int ix, int iy, int px, int py)
{
        int op;
        if(ix>px){op=px; px=ix; ix=op;};
        if(iy>py){op=py; py=iy; iy=op;};

        for(int i=ix; i<=px; i++)
        for(int j=iy; j<=py; j++){
                buffer->dev_mapa()->insb(i-ix,j-iy,niv->dev_mapa()->consb(i,j));
                buffer->dev_mapa()->inse(i-ix,j-iy,niv->dev_mapa()->conse(i,j));
        };
        buffersx=px-ix; buffersy=py-iy;
}
void paste_area(int ix, int iy)
{
        int op;
        for(int i=0; i<=buffersx; i++)
        for(int j=0; j<=buffersy; j++){
                niv->dev_mapa()->insb(ix+i,iy+j,buffer->dev_mapa()->consb(i,j));
                niv->dev_mapa()->inse(ix+i,iy+j,buffer->dev_mapa()->conse(i,j));
        };
}
void cut_area(int ix, int iy, int px, int py)
{
        int op;
        if(ix>px){op=px; px=ix; ix=op;};
        if(iy>py){op=py; py=iy; iy=op;};

        copy_area(ix,iy,px,py);

        for(int i=ix; i<=px; i++)
        for(int j=iy; j<=py; j++){
                niv->dev_mapa()->insb(i,j,NADA);
                niv->dev_mapa()->inse(i,j,VACIO);
        };
}
void habitacion(int ix, int iy, int px, int py)
{
        char *ops[20], frase[40];
        int op, nops=9, su, pa;
        if(ix>px){op=px; px=ix; ix=op;};
        if(iy>py){op=py; py=iy; iy=op;};

        su=elige_suelo();

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Pared 1");
        sprintf(ops[1],"Pared 2");
        sprintf(ops[2],"Pared 3");
        sprintf(ops[3],"Pared 4");
        sprintf(ops[4],"Pared 5");
        sprintf(ops[5],"Pared 6");
        sprintf(ops[6],"Pared 7 [A]");
        sprintf(ops[7],"Pared 8 [A]");
        sprintf(ops[8],"Pared 9 [A]");

        pa=menu("Tipo de pared",ops,nops);

        for(int i=ix; i<=px; i++)
        for(int j=iy; j<=py; j++){
                niv->dev_mapa()->insb(i,j,(tbald)su);
                if((i==ix) || (i==px))
                           niv->dev_mapa()->inse(i,j,PVE1+(telem)pa);
                if((j==iy) || (j==py))
                           niv->dev_mapa()->inse(i,j,PHO1+(telem)pa);
        };
        niv->dev_mapa()->inse(ix,iy,ER_D);
        niv->dev_mapa()->inse(px,iy,EL_D);
        niv->dev_mapa()->inse(ix,py,ER_U);
        niv->dev_mapa()->inse(px,py,EL_U);
}
void marco_puerta(int ix, int iy, int px, int py)
{
        int op,horiz;
        if(ix>px){op=px; px=ix; ix=op;};
        if(iy>py){op=py; py=iy; iy=op;};
        if(iy==py){
                for(op=ix; op<=px; op++)
                        niv->dev_mapa()->inse(op,iy,DTRH);
                niv->dev_mapa()->inse(ix,iy,DM2H);
                niv->dev_mapa()->inse(px,iy,DM1H);
        }else{
                for(op=iy; op<=py; op++)
                        niv->dev_mapa()->inse(ix,op,DTRV);
                niv->dev_mapa()->inse(ix,iy,DM2V);
                niv->dev_mapa()->inse(ix,py,DM1V);
        };
}
void edita_tile(int px, int py)
{
        char *ops[20], frase[40];
        int nops=12, op, x=px, y=py;
        bool ac=FALSE;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Suelo");
        sprintf(ops[1],"Decorado");
        sprintf(ops[2],"Item");
        sprintf(ops[3],"Enemigo");
        sprintf(ops[4],"Teleport");
        sprintf(ops[5],"Barrera");
        sprintf(ops[6],"Tirador");
        sprintf(ops[7],"Fuente de sonido");
        sprintf(ops[8],"Posic. inicio");
        sprintf(ops[9],"*COPIAR");
        sprintf(ops[10],"*CORTAR");
        sprintf(ops[11],"*PEGAR");

        switch(menu("Edici¢n de baldosa",ops,nops)){

                case 0 : edita_suelo(px,py,px,py); break;
                case 1 : edita_decor(px,py,px,py); break;
                case 2 : edita_item(px,py,px,py); ac=TRUE; break;
                case 3 : edita_enemigo(px,py); ac=TRUE; break;
                case 4 : if(elige_coordenadas("Elige el destino del teleport",x,y)>=0){
                                mp->anyade_telep(px,py,x,y);
                                mp->inse(px,py,TELE);
                                mp->inse(x,y,TELE);
                                sprintf(frase,"Nuevo teleport de (%d,%d) a (%d,%d)",px,py,x,y);
                                dialog_ok(frase);
                                ac=TRUE; }; break;
                case 5 : edita_barrera(px,py,px,py); ac=TRUE; break;
                case 6 : edita_tirador(px,py); ac=TRUE; break;
                case 7 : edita_fuente_sonido(px,py); break;
                case 8 : op=elige_direc();
                         if(op>=0){
                                sprintf(frase,"Nueva posici¢n de inicio : (%d,%d)",px,py);
                                dialog_ok(frase);
                                mp->nueva_pos_inicio(px,py,(w_dir)op);
                         }; break;
                case 9 : copy_area(px,py,px,py); break;
                case 10 : cut_area(px,py,px,py); break;
                case 11: paste_area(px,py); break;
                default : break;
                };
        if (ac) niv->dev_mapa()->crea_matriz_interes();
}
void edita_linea(int ix, int iy, int px, int py)
{
        char *ops[20], frase[40];
        int nops=10, op;
        bool ac=FALSE;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Suelo");
        sprintf(ops[1],"Decorado");
        sprintf(ops[2],"Item");
        sprintf(ops[3],"Puerta");
        sprintf(ops[4],"Puente");
        sprintf(ops[5],"Barrera");
        sprintf(ops[6],"L ser");
        sprintf(ops[7],"Marco puerta");
        sprintf(ops[8],"*COPIAR");
        sprintf(ops[9],"*CORTAR");

        switch(menu("Edici¢n de linea",ops,nops)){

                case 0 : edita_suelo(ix,iy,px,py);  break;
                case 1 : edita_decor(ix,iy,px,py);  break;
                case 2 : edita_item(ix,iy,px,py); ac=TRUE; break;
                case 3 : edita_puerta(ix,iy,px,py); ac=TRUE; break;
                case 4 : edita_puente(ix,iy,px,py); ac=TRUE; break;
                case 5 : edita_barrera(ix,iy,px,py); ac=TRUE; break;
                case 6 : edita_laser(ix,iy,px,py); ac=TRUE; break;
                case 7 : marco_puerta(ix,iy,px,py); break;
                case 8 : copy_area(ix,iy,px,py); break;
                case 9 : cut_area(ix,iy,px,py); break;

                default : break;
                };
        if(ac)niv->dev_mapa()->crea_matriz_interes();
}
void edita_area(int ix, int iy, int px, int py)
{
        char *ops[20], frase[40];
        int nops=10, op;
        bool ac=FALSE;

        if(ix>px){op=px; px=ix; ix=op;};
        if(iy>py){op=py; py=iy; iy=op;};

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Suelo");
        sprintf(ops[1],"Decorado");
        sprintf(ops[2],"Item");
        sprintf(ops[3],"Area Oscura");
        sprintf(ops[4],"Area Final");
        sprintf(ops[5],"Area Secreta");
        sprintf(ops[6],"Barreras");
        sprintf(ops[7],"Habitaci¢n rect.");
        sprintf(ops[8],"*COPIAR");
        sprintf(ops[9],"*CORTAR");

        switch(menu("Edici¢n de  rea",ops,nops)){

                case 0 : edita_suelo(ix,iy,px,py); break;
                case 1 : edita_decor(ix,iy,px,py); break;
                case 2 : edita_item(ix,iy,px,py); ac=TRUE; break;
                case 3 : edita_oscu(ix,iy,px,py); ac=TRUE; break;
                case 4 : mp->area_final(ix,iy,px,py);
                         sprintf(frase,"µrea final de (%d,%d) a (%d,%d)",ix,iy,px,py);
                         dialog_ok(frase); break;
                case 5 : mp->anyade_area_secreta(ix,iy,px,py);
                         sprintf(frase,"µrea secreta de (%d,%d) a (%d,%d)",ix,iy,px,py);
                         dialog_ok(frase);
                         ac=TRUE; break;
                case 6 : edita_barrera(ix,iy,px,py); ac=TRUE; break;
                case 7 : habitacion(ix,iy,px,py); break;
                case 8 : copy_area(ix,iy,px,py); break;
                case 9 : cut_area(ix,iy,px,py); break;

                default : break;
                };
        if(ac)niv->dev_mapa()->crea_matriz_interes();
}
void test_nivel()
{
      w_clase clase[4];
      int spr[4], nplayers=0, modo, dif=0;
      batalla *b2;
      char frase[40];
      byte colors[4][2]={{BROJO,BGRIS},{BAZUN,BAZUM},{BVER1,BVER2},{BNARA,BMAR3}};
      t_control controls[4];

      do{

         nplayers=dialog_int("N£mero de jugadores");

      }while((nplayers<1) || (nplayers>4));
      do{

         dif=dialog_int("Dificultad (1-5)");

      }while((dif<1) || (dif>5));

      if(dialog_yes_no("¨Modo Deathmatch?")) modo=DMCH;
      else modo=COOP;

      for(int i=0; i<nplayers; i++){
            clase[i]=(w_clase)elige_clase();
            switch(clase[i]){
                    case SIGMA : spr[i]=0; break;
                    case OMEGA : spr[i]=1; break;
                    case BETA  : spr[i]=2; break; };
            controls[i]=(t_control) elige_control();
            };

      sprintf(frase,"%s\\security.niv",GDIR);
      niv->salva(frase);
      b2=new batalla(GDIR,"security.niv",s,modo,SCRX,FALSE);

      b2->activa_input_consola();

      for(int i=0; i<nplayers; i++)
      b2->ingresa_player("Tester",spr[i],clase[i],colors[i][0],colors[i][1],controls[i]);

      b2->elimina_items_inutiles();

      b->activa_musica();
      speed_counter=0;
      do{

        while(speed_counter > 0){

                b2->actualiza(3);
                speed_counter--;
        };
        ch->muestra_pantalla(pant,b2);
        blit(pant,screen,0,0,0,0,SCRX,SCRY);

      }while((!key[KEY_ESC]) && (!b2->coop_nivel_completado()));
      b->desactiva_musica();

      delete b2;
      clear_keybuf();
}
void elimina_objeto()
{

       char *ops[20], frase[40];
       int nops=12, op, cual=0, op2;
       bool ultim;
       void *punter;
       bald x,y;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Teleport");
        sprintf(ops[1],"Puerta");
        sprintf(ops[2],"µrea oscura");
        sprintf(ops[3],"µrea secreta");
        sprintf(ops[4],"Barrera");
        sprintf(ops[5],"Disparador");
        sprintf(ops[6],"Puente");
        sprintf(ops[7],"L ser");
        sprintf(ops[8],"Interruptor");
        sprintf(ops[9],"Item recogible");
        sprintf(ops[10],"Fuente de sonido");
        sprintf(ops[11],"Enemigo");

        op=menu("Tipo de objeto a eliminar",ops,nops);

        switch(op){
                case -1: return;
                case 0 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_telep(0); break;
                case 1 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_puerta(0); break;
                case 2 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_oscu(0); break;
                case 3 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_secr(0); break;
                case 4 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_barr(0); break;
                case 5 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_disp(0); break;
                case 6 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_puente(0); break;
                case 7 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_laser(0); break;
                case 8 : punter=(void *)b->dev_nivel()->dev_mapa()->dev_interr(0); break;
                case 9 : punter=(void *)b->dev_nivel()->dev_item(0); break;
                case 10: if(b->dev_nivel()->dev_mapa()->dev_sonido_amb(0,x,y)>=0) punter=(void *)1;
                         else                                                     punter=(void *)NULL; break;
                case 11: punter=(void *)b->dev_enem(0); break;
        };
        if((op>=0) && (punter!=NULL)){
                 do{
                        switch(op){
                                case 0 : if(b->dev_nivel()->dev_mapa()->dev_telep(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_telep(cual)->x_in();
                                         y=b->dev_nivel()->dev_mapa()->dev_telep(cual)->y_in(); break;
                                case 1 : if(b->dev_nivel()->dev_mapa()->dev_puerta(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_puerta(cual)->inx();
                                         y=b->dev_nivel()->dev_mapa()->dev_puerta(cual)->iny(); break;
                                case 2 : if(b->dev_nivel()->dev_mapa()->dev_oscu(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_oscu(cual)->inx();
                                         y=b->dev_nivel()->dev_mapa()->dev_oscu(cual)->iny(); break;
                                case 3 : if(b->dev_nivel()->dev_mapa()->dev_secr(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_secr(cual)->inx();
                                         y=b->dev_nivel()->dev_mapa()->dev_secr(cual)->iny(); break;
                                case 4 : if(b->dev_nivel()->dev_mapa()->dev_barr(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_barr(cual)->posx();
                                         y=b->dev_nivel()->dev_mapa()->dev_barr(cual)->posy(); break;
                                case 5 : if(b->dev_nivel()->dev_mapa()->dev_disp(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_disp(cual)->x_pos();
                                         y=b->dev_nivel()->dev_mapa()->dev_disp(cual)->y_pos(); break;
                                case 6 : if(b->dev_nivel()->dev_mapa()->dev_puente(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_puente(cual)->inx();
                                         y=b->dev_nivel()->dev_mapa()->dev_puente(cual)->iny(); break;
                                case 7 : if(b->dev_nivel()->dev_mapa()->dev_laser(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_laser(cual)->inx();
                                         y=b->dev_nivel()->dev_mapa()->dev_laser(cual)->iny(); break;
                                case 8 : if(b->dev_nivel()->dev_mapa()->dev_interr(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_mapa()->dev_interr(cual)->x_pos();
                                         y=b->dev_nivel()->dev_mapa()->dev_interr(cual)->y_pos(); break;
                                case 9 : if(b->dev_nivel()->dev_item(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_nivel()->dev_item(cual)->x_pos();
                                         y=b->dev_nivel()->dev_item(cual)->y_pos(); break;
                                case 10: if(b->dev_nivel()->dev_mapa()->dev_sonido_amb(cual+1,x,y)<0)
                                                ultim=TRUE; else ultim=FALSE;
                                         b->dev_nivel()->dev_mapa()->dev_sonido_amb(cual,x,y); break;
                                case 11: if(b->dev_enem(cual+1)==NULL)
                                                ultim=TRUE; else ultim=FALSE;
                                         x=b->dev_enem(cual)->x_pos();
                                         y=b->dev_enem(cual)->y_pos(); break;

                        };
                        op2=elige_objeto(x,y,op,cual);
                        if((op2==0) && (cual>0)) cual--;
                        if((op2==1) && (!ultim)) cual++;

                }while(op2<2);

                if(op2!=3)
                if(dialog_yes_no("¨Seguro de eliminar?")==TRUE)
                switch(op){
                        case 0 : b->dev_nivel()->dev_mapa()->retira_telep(cual); break;
                        case 1 : b->dev_nivel()->dev_mapa()->retira_puerta(cual); break;
                        case 2 : b->dev_nivel()->dev_mapa()->retira_oscu(cual); break;
                        case 3 : b->dev_nivel()->dev_mapa()->retira_secr(cual); break;
                        case 4 : b->dev_nivel()->dev_mapa()->retira_barrera(cual); break;
                        case 5 : b->dev_nivel()->dev_mapa()->retira_disp(cual); break;
                        case 6 : b->dev_nivel()->dev_mapa()->retira_puente(cual); break;
                        case 7 : b->dev_nivel()->dev_mapa()->retira_laser(cual); break;
                        case 8 : b->dev_nivel()->dev_mapa()->retira_interr(cual); break;
                        case 9 : b->dev_nivel()->retira_item(cual); break;
                        case 10: b->dev_nivel()->dev_mapa()->retira_sonido_amb(cual);
                        case 11: b->dev_nivel()->retira_enem(cual); break;
                };
        };
        if(punter==NULL) dialog_ok("No hay objetos de esta clase");
}
void herramientas()
{
        char *ops[20], frase[40];
        int nops=2, op,lev;

        char fepi[16], fepi2[16], decor[16], fene1[16], fene2[16];
        byte c11,c12,c21,c22;

        FILE *fp;

        for(int i=0; i<nops; i++) ops[i]=new char[20];

        sprintf(ops[0],"Correcci¢n de esquinas");
        sprintf(ops[1],"Importar nivel de S.A.");

        switch(menu("Herramientas de edici¢n",ops,nops)){

                case 0 : niv->corrige_esquinas(); break;
                case 1 : while(dialog_string("Episodio",fepi,8)<0);
                         do{
                            lev=dialog_int("Nivel (1-8)");
                         }while((lev<1) && (lev>8));
                         while(dialog_string("Decorado",decor,8)<0);
                         while(dialog_string("Enemigo 1",fene1,8)<0);
                         while(dialog_string("Enemigo 2",fene2,8)<0);
                         c11=elige_color("En 1. Color base 1");
                         c12=elige_color("En 1. Color base 2");
                         c21=elige_color("En 2. Color base 1");
                         c22=elige_color("En 2. Color base 2");
                         sprintf(fepi2,"%s.epi",fepi);

                         sprintf(frase,"%s\\%s",GDIR,fepi2);
                         if((fp=fopen(frase,"rb"))!=NULL){

                                fclose(fp);
                                delete b;
                                b=new batalla(GDIR,fepi2,(byte)(lev-1),decor,fene1,fene2,c11,c12,c21,c22,s,SCRX,FALSE);
                                b->activa_input_consola();
                                niv=b->dev_nivel();
                                mp=niv->dev_mapa();
                                sprintf(FILENAME,"_%s%d",fepi,lev);
                                ch->carga_bitmaps_batalla(b);
                                ch->crea_tablas_color(b->dev_nivel());
//                                cx=niv->dev_mapa()->x_inicio(); cy=niv->dev_mapa()->y_inicio();

                         }else dialog_ok("­El fichero EPI no existe!");

                         break;
        };
}
void pantalla_principal()
{
      controlm cont;

      float cx=niv->dev_mapa()->x_inicio(), cy=niv->dev_mapa()->y_inicio();
      int ix=15, iy=15, px=15, py=15, sel_state=0;
      char *ops[20], frase[100], frase2[20];
      bool fin=FALSE;
      FILE *fp;

      speed_counter=0;

      do{
         tile_cursor(cx,cy,px,py);
         while(speed_counter>0){

                // Botones de la barra
                if(hot_spot(5,4,5+21,4+22)){

                        // Reiniciar
                        if(dialog_yes_no("¨Seguro de empezar un nivel nuevo?")){
                                delete b;
                                b=new batalla(GDIR,"null.niv",s,COOP,SCRX,TRUE);
                                b->activa_input_consola();
                                niv=b->dev_nivel();
                                mp=niv->dev_mapa();
                                sprintf(FILENAME,"nuevo");
                                ch->carga_bitmaps_batalla(b);
                        };
                }else if(hot_spot(35,4,35+21,4+22)){

                        // Cargar fichero
                        if(dialog_yes_no("¨Seguro de cargar otro fichero?")){
                                if(dialog_string("Nombre:",frase,8)){
                                        sprintf(frase2,"%s\\%s.niv",GDIR,frase);
                                        if((fp=fopen(frase2,"rb"))!=NULL){
                                                fclose(fp);
                                                delete b;
                                                sprintf(frase2,"%s.niv",frase);
                                                b=new batalla(GDIR,frase2,s,COOP,SCRX,TRUE);
                                                b->activa_input_consola();
                                                niv=b->dev_nivel();
                                                mp=niv->dev_mapa();
                                                sprintf(FILENAME,"%s",frase);
                                                ch->carga_bitmaps_batalla(b);
                                                ch->crea_tablas_color(b->dev_nivel());
                                                cx=niv->dev_mapa()->x_inicio(); cy=niv->dev_mapa()->y_inicio();

                                        }else dialog_ok("­El fichero no existe!");
                                };
                        };
                }else if(hot_spot(65,4,65+21,4+22)){

                        sprintf(frase,"¨Salvar %s\\%s.niv?",GDIR,FILENAME);
                        if(dialog_yes_no(frase)){
                                sprintf(frase,"%s\\%s.niv",GDIR,FILENAME);
                                niv->salva(frase);
                        };
                }else if(hot_spot(95,4,95+21,4+22)){
                        edita_misc();
                }else if(hot_spot(125,4,125+21,4+22)){
                        elimina_objeto();
                }else if(hot_spot(155,4,155+21,4+22)){
                        informacion();
                }else if(hot_spot(185,4,185+21,4+22)){
                        modifica_enemigos();
                }else if(hot_spot(215,4,215+21,4+22)){
                        test_nivel();
                }else if(hot_spot(245,4,245+21,4+22)){
                        herramientas();
                }else if(cont.b_raton(2)){
                        info_tile(px,py);
                }else{
                        //Trabajar sobre la selecci¢n
                        if(sel_state==0) {ix=px; iy=py;};
                        if((sel_state==0) && (cont.b_raton(1))) sel_state=1;
                        if((sel_state==1) && (!cont.b_raton(1))) sel_state=2;
                        if(sel_state==2){

                                if((px==ix) && (py==iy)) edita_tile(px,py);
                                else

                                        if((px==ix) || (py==iy)) edita_linea(ix,iy,px,py);
                                        else edita_area(ix,iy,px,py);
                                        sel_state=0;};
                        };
                if(key[KEY_F10])
                        if(dialog_yes_no("¨Deseas salir de SAHME?"))
                                fin=TRUE;
                if(keypressed())
                        switch(readkey()>>8){
                                case KEY_LEFT : if(cx>0) cx--; break;
                                case KEY_RIGHT: if(cx<MSIZEX-1) cx++; break;
                                case KEY_UP   : if(cy>0) cy--; break;
                                case KEY_DOWN : if(cy<MSIZEY-1) cy++; break;
                        };
                speed_counter--;

                if(key[KEY_F12]) {
                         get_palette(pal);
                         save_pcx("captura.pcx",pant,pal);
                         delay(500);};

         }
        clear_to_color(pant,0);
        ch->muestra(pant,b,SCRX,SCRY,cx,cy);
        muestra_selec((int)cx,(int)cy,ix,iy,px,py);

        rectfill(pant,0,0,SCRX,30,20);

        draw_sprite(pant,button[BNEW],5,4);
        draw_sprite(pant,button[BOPE],5+30,4);
        draw_sprite(pant,button[BDIS],5+60,4);
        draw_sprite(pant,button[BDEC],5+90,4);
        draw_sprite(pant,button[BNO],5+120,4);
        draw_sprite(pant,button[BINF],5+150,4);
        draw_sprite(pant,button[BENE],5+180,4);
        draw_sprite(pant,button[BSIG],5+210,4);
        draw_sprite(pant,button[BTOO],5+240,4);

        draw_sprite(pant,button[BCL1+(speed_counter%3)],SCRX-26,4);
        sprintf(frase,"Cursor:(%d,%d)",px,py);
        ch->texto(pant,frase,10,SCRY-20,C_FRG);
        ch->texto(pant,niv->nombre(),10,SCRY-40,C_FRG);
        ch->muestra_puntero(pant,cont.x_raton(),cont.y_raton());
        blit(pant,screen,0,0,0,0,SCRX,SCRY);

      }while(!fin);
}
void carga_bitmaps()
{
        BITMAP *aux;

        tile=load_pcx("data\\tile.pcx",ch->dev_paleta());
        for(int i=0; i<13; i++)
                button[i]=create_bitmap(22,21);
        aux=load_pcx("data\\botones.pcx",ch->dev_paleta());
        for(int i=0; i<13; i++)
                blit(aux,button[i],23*i,0,0,0,22,21);
        destroy_bitmap(aux);
}
void increment_speed_counter()
{
        speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);
int main(int argc, char *argv[])
{
      char frase[60],f[21];
      int j, dif=3, page=0;
      byte gt=COOP;

      LOCK_VARIABLE(speed_counter);
      LOCK_FUNCTION(increment_speed_counter);
      install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));

      parametros(argc,argv,MODO,dif,gt);

      if(MODO==0){ SCRX=320; SCRY=200; SCRDRV=GFX_VGA;};
      if(MODO==1){ SCRX=360; SCRY=240; SCRDRV=GFX_MODEX;};
      if(MODO==2){ SCRX=400; SCRY=300; SCRDRV=GFX_MODEX;};
      if(MODO==3){ SCRX=640; SCRY=400; SCRDRV=GFX_VESA1;};
      if(MODO==4){ SCRX=800; SCRY=600; SCRDRV=GFX_VESA1;};
      if(MODO==5){ SCRX=1024; SCRY=768; SCRDRV=GFX_VESA1;};

      // Buffer para copy, cut y paste
      buffer=new nivel();

      // Genera nivel base, nulo
      niv=new nivel();
      niv->cambia_fspr("sigma",0);
      niv->cambia_fspr("omega",1);
      niv->cambia_fspr("beta",2);
      niv->salva("data\\null.niv");
      delete niv;

      pant=create_bitmap(SCRX,SCRY);
      back=create_bitmap(SCRX,SCRY);
      pal=new RGB[256];
      s=new waves(GDIR);
      b=new batalla(GDIR,"null.niv",s,COOP,SCRX,TRUE);
      ch=new charset(pant,SCRX,SCRY);

      arranque_sistema(b,pant,ch,s,SCRX,SCRY,SCRDRV);
      delete b;

      // Genera nivel base, nulo
      niv=new nivel();
      niv->cambia_fspr("sigma",0);
      niv->cambia_fspr("omega",1);
      niv->cambia_fspr("beta",2);
      niv->salva("data\\null.niv");
      delete niv;

      //Carga £ltimo nivel editado (security) o bien por par metro
      if(argc>1) sprintf(FILENAME,"%s",argv[1]);
      sprintf(frase,"%s.niv",FILENAME);
      b=new batalla(GDIR,frase,s,COOP,SCRX,TRUE);
      b->activa_input_consola();
      ch->carga_bitmaps_batalla(b);

      niv=b->dev_nivel();
      mp=niv->dev_mapa();

      carga_bitmaps();
      ch->crea_tablas_color(b->dev_nivel());

      pantalla_principal();

      set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
      printf("The best is still to come...\n");
      printf("Enjoy!\n");

      niv->salva("data\\security.niv");

      destroy_bitmap(pant);
      delete ch;
      delete b;
      delete s;
      delete buffer;
      delete[] pal;
}









