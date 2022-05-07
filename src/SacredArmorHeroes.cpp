/*========================================================================
  =   Sacred Armor Heroes. English full version                          =
  =   Created by Active Minds 1999,2000,2018. A game from NHSP & MTX     =
  =   Code written by NHSP                                               =
  ========================================================================*/

#define CRUSADE_ONLINE
#define DEATHMATCH_ONLINE


//#include <stdlib.h>

#include"her_tipo.hpp"
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<dpmi.h>
#include<time.h>
#include<allegro5/allegro.h>
#include<allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
//#include<jgmod.h>
#include"her_graf.hpp"
#include"her_jueg.hpp"
#include"her_kti.hpp"

#include "profiler.h"

#ifdef DEATHMATCH_ONLINE
#if defined SAHWINDOWS
#include <winsock2.h>   // the networking library
#elif defined SAHLINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif
#include "CNetworkInterface.hpp"
#endif



volatile int speed_counter = 0;

char GDIR[12]="DATA";


char NOMBRE_MODULO[]="Sacred Armor Heroes Revival - Version 1.1.1";
char VERSION_MODULO[] = "v1.1.1R";

#if defined SAHWINDOWS
char SISTEMA[] = "Windows";
#elif defined SAHLINUX
char SISTEMA[] = "Ubuntu";
#else
char SISTEMA[] = "Unknown";
#endif

int lmd=1, lmm=7, lma=2020;

#define NUM_WIN_RESOLUTIONS 4
#define NUM_FUL_RESOLUTIONS 2

int WINDOW_RESOLUTIONS[NUM_WIN_RESOLUTIONS][2]={{640,400},{720,400},{800,600},{1040,600}};
int FULLSCR_RESOLUTIONS[NUM_FUL_RESOLUTIONS][2]={{1280,720},{1920,1080}};

#define TICKS_PER_SECOND	60
#define TICK_DURATION		(1.f/TICKS_PER_SECOND)

ALLEGRO_DISPLAY *game_display;
ALLEGRO_SAMPLE  *selec, *accept_snd, *cancel;
ALLEGRO_BITMAP *cursor, *cuadro_negro;
extern ALLEGRO_SAMPLE_ID sample_ret_id;
ALLEGRO_AUDIO_STREAM* musica;

extern ALLEGRO_BITMAP *crear_bitmap(int w, int h);
extern ALLEGRO_BITMAP *cargar_bitmap(const char* f);
extern void destruir_bitmap(ALLEGRO_BITMAP *bmp);

bool fullscr_op = FALSE, music_op=TRUE, sound_op=TRUE, gore_op=TRUE, split_op=FALSE, muerte_gore, console_op=FALSE, intro_am=TRUE, show_fps=FALSE;
int res_op=1, fullscr_res_op=1, cursor_a=0, sshots=0, hell_op=0;
t_control cont_op[4]={TEC1,NOTC,NOTC,NOTC};
game_info *partida;
char epi_descr[4][40]={"The castle invaded by the orcs","The hi-tech fortress","The profanated temple","The Invaders lare"},
     epi_files[4][8]={"mediev","futuro","antigu","infier"},
     lev_names[4][7][MENSZ];

double ultimo_inicio_tick, tiempo_por_consumir, fps;

extern void rect(ALLEGRO_BITMAP *bmp,int x1, int y1, int x2, int y2, unsigned char color);
extern void rectfill(ALLEGRO_BITMAP *bmp,int x1, int y1, int x2, int y2, unsigned char color);
extern void circlefill(ALLEGRO_BITMAP *bmp,int x1, int y1, float r, unsigned char color);

void inicia_tick()
{
	tiempo_por_consumir = 0;
	ultimo_inicio_tick = al_get_time();
}

bool tick_terminado()
{
	return al_get_time() >= ultimo_inicio_tick + (TICK_DURATION);
}

int consume_ciclos_transcurridos()
{
	float elapsed =  al_get_time() - ultimo_inicio_tick;
	ultimo_inicio_tick = al_get_time();
	tiempo_por_consumir += elapsed;
	int ciclos_a_consumir = int (tiempo_por_consumir / TICK_DURATION);
	fps = (double)60.f/(tiempo_por_consumir / TICK_DURATION);
	tiempo_por_consumir -= ciclos_a_consumir * TICK_DURATION;
	return ciclos_a_consumir;
}

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

void mensaje_inicio(batalla *b,ALLEGRO_BITMAP *pant,charset *ch,const char *msg, int sx, int sy, byte c)
{
      b->encola_mensaje_consola(msg,c);
      b->actualiza(1);
      ch->muestra_pantalla(pant,b);
      //blit(pant,screen,0,0,0,0,sx,sy);
	  ___START_PROFILER()
	  update_display_and_flip(pant);
}
void arranque_sistema(batalla *b,ALLEGRO_BITMAP *pant,charset *ch,waves *s,int SCRX, int SCRY /*,int SCRDRV*/)
{
      long memoria;
      char frase[60];

	   b->abre_cierra_consola();
     fprintf(stderr,"Loading standard sprites...\n");
       al_init_primitives_addon();

	 ch->carga_sprites(b);

	 sprintf(frase, "Allegro %s succesfully initialized.\n", ALLEGRO_VERSION_STR);
	 mensaje_inicio(b, pant, ch, frase, SCRX, SCRY, BMAR1);

//      if(set_gfx_mode(SCRDRV, SCRX, SCRY, 0, 0)==0){
         sprintf(frase,"%dx%dx%d display succesfully initialized.",SCRX,SCRY,al_get_pixel_format_bits(al_get_display_format(game_display)));
         mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//      }else{
//         mensaje_inicio(b,pant,ch,"Error at initializing graphic mode!",SCRX,SCRY,BCO1);
//         exit(1);
//      };
      sprintf(frase,"Last update : %d %s %dth",lma,smes(lmm),lmd);
      mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);

	  sprintf(frase, "Platform : %s", SISTEMA);
	  mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//      check_cpu();
//      if(cpu_family==3) mensaje_inicio(b,pant,ch,"CPU : 386",SCRX,SCRY,BMAR1);
//      if(cpu_family==4) mensaje_inicio(b,pant,ch,"CPU : 486",SCRX,SCRY,BMAR1);
//      if(cpu_family>=5)
//         mensaje_inicio(b,pant,ch,"CPU : Pentium(R) or higher",SCRX,SCRY,BMAR1);
//      if(cpu_mmx) mensaje_inicio(b,pant,ch,"MMX instructions available",SCRX,SCRY,BMAR1);
//
      //memoria=_go32_dpmi_remaining_physical_memory();
//
      //sprintf(frase,"System's free RAM memory : %ld Mb",(unsigned long)(memoria/1000000));
      //mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//
//      switch(os_type){
//         case OSTYPE_UNKNOWN : mensaje_inicio(b,pant,ch,"Operating system : MS-DOS",SCRX,SCRY,BMAR1); break;
//         case OSTYPE_WIN3 : mensaje_inicio(b,pant,ch,"Operating system : Windows 3.1",SCRX,SCRY,BMAR1); break;
//         case OSTYPE_WIN95 : mensaje_inicio(b,pant,ch,"Operating system : Windows 95-98",SCRX,SCRY,BMAR1); break;
//         case OSTYPE_WINNT : mensaje_inicio(b,pant,ch,"Operating system : Windows NT",SCRX,SCRY,BMAR1); break;};
//
//      if(install_timer()==0) mensaje_inicio(b,pant,ch,"60 Hz timer installed.",SCRX,SCRY,BMAR1);
//      else                   mensaje_inicio(b,pant,ch,"Error installing timer!",SCRX,SCRY,BCO1);
      if(al_install_keyboard())
	  {
		  mensaje_inicio(b,pant,ch,"Keyboard control installed.",SCRX,SCRY,BMAR1);
		  controlm::install_keyboard_listener();
	  }
      else                   mensaje_inicio(b,pant,ch,"Error installig keyboard control!",SCRX,SCRY,BCO1);

      if(al_install_mouse()) mensaje_inicio(b,pant,ch,"Mouse control installed.",SCRX,SCRY,BMAR1);
	  else					 mensaje_inicio(b,pant,ch,"Error installig mouse control!",SCRX,SCRY,BCO1);

      if(al_install_joystick()){
         mensaje_inicio(b,pant,ch,"Joystick control installed.",SCRX,SCRY,BMAR1);
         sprintf(frase,"%d joystick(s) detected.",al_get_num_joysticks());
         mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
		 for(int i = 0; i < al_get_num_joysticks(); i++)
		 {
			 sprintf(frase,"Joystick %d: %s",i+1,al_get_joystick_name(al_get_joystick(i)));
			 mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
		 }
      }else mensaje_inicio(b,pant,ch,"Error installig joystick control!",SCRX,SCRY,BMAR1);
//      //if(install_sound(DIGI_SB16, MIDI_NONE, "")==0){
//      //          sprintf(frase,"Sound card : Sound Blaster 16 or higher");
//      //          mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//      //}else{
//      //      if(install_sound(DIGI_SBPRO, MIDI_NONE, "")==0){
//      //          sprintf(frase,"Sound card : Sound Blaster Pro",digi_driver->name);
//      //          mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//      //      }else{
//                if(install_sound(DIGI_AUTODETECT, MIDI_NONE, "")==0){
//                        sprintf(frase,"Sound card : Sound Blaster",digi_driver->name);
//                        mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//                }else mensaje_inicio(b,pant,ch,"Error initializing sound card!",SCRX,SCRY,BCO1);
////	        };
//      //};
//      reserve_voices(32,-1);
//      if(install_mod(8)>=0){
//         sprintf(frase,"JGMOD player installed succesfully.");
//         mensaje_inicio(b,pant,ch,frase,SCRX,SCRY,BMAR1);
//      } else mensaje_inicio(b,pant,ch,"Error initializing JGMOD!",SCRX,SCRY,BCO1);

	   if(al_install_audio())
	   {
		   mensaje_inicio(b,pant,ch,"Audio system installed",SCRX,SCRY,BMAR1);
		   al_reserve_samples(32);
		   if(al_init_acodec_addon())
		   {
			    mensaje_inicio(b,pant,ch,"Audio codecs initialized",SCRX,SCRY,BMAR1);
		   }

	   }

	    mensaje_inicio(b,pant,ch,"Loading data. Please wait...",SCRX,SCRY,BMAR1);

//      mensaje_inicio(b,pant,ch,"Loading battle sprites and paletteï",SCRX,SCRY,BMAR1);
//      ch->carga_bitmaps_batalla(b);
//      mensaje_inicio(b,pant,ch,"Creating color tables. Please waitï",SCRX,SCRY,BMAR1);
//      ch->crea_tablas_color(b->dev_nivel());



}

void carga_fullscr_setting()
{
      FILE *ptr;
	  char aux[20];

      ptr=fopen("heroes.cfg","rb");
      if(ptr!=NULL){
		 fread(&fullscr_op,sizeof(fullscr_op),1,ptr);
		 fread(&aux,sizeof(music_op),1,ptr);
         fread(&aux,sizeof(sound_op),1,ptr);
         fread(&aux,sizeof(gore_op),1,ptr);
         fread(&aux,sizeof(split_op),1,ptr);
         fread(&res_op,sizeof(res_op),1,ptr);
		 fread(&fullscr_res_op,sizeof(fullscr_res_op),1,ptr);
		 fclose(ptr);
	  }
}

void carga_config()
{
      FILE *ptr;
      char s[20];
	  int i,j;
      nivel *ni;

      ptr=fopen("heroes.cfg","rb");
      if(ptr!=NULL){
		 fread(&fullscr_op,sizeof(fullscr_op),1,ptr);
         fread(&music_op,sizeof(music_op),1,ptr);
         fread(&sound_op,sizeof(sound_op),1,ptr);
         fread(&gore_op,sizeof(gore_op),1,ptr);
         fread(&split_op,sizeof(split_op),1,ptr);
         fread(&res_op,sizeof(res_op),1,ptr);
		 fread(&fullscr_res_op,sizeof(fullscr_res_op),1,ptr);
         fread(&cont_op,sizeof(t_control),4,ptr);
         fread(&sshots,sizeof(sshots),1,ptr);
         fread(&hell_op,sizeof(hell_op),1,ptr);
         fclose(ptr);
      };

      for(i=0; i<MAX_GAME; i++){
         sprintf(s,"saved%d.sav",i);
         ptr=fopen(s,"rb");
         if(ptr!=NULL) {fread(&partida[i],sizeof(game_info),1,ptr);
						fclose(ptr);};
      }
      // Guarda los nombres de los niveles
      for(i=0; i<4; i++)
      for(j=0; j<7; j++){
         sprintf(s,"%s/%s%d.niv",GDIR,epi_files[i],j+1);
         ni=DBG_NEW nivel();
         ni->carga(s);
         sprintf(lev_names[i][j],"%s",ni->nombre());
         delete ni;
      };
      muerte_gore=gore_op;
}
void guarda_config()
{
      FILE *ptr;
      char s[20];
	  int i;

      ptr=fopen("heroes.cfg","w+b");
	  fwrite(&fullscr_op,sizeof(fullscr_op),1,ptr);
      fwrite(&music_op,sizeof(music_op),1,ptr);
      fwrite(&sound_op,sizeof(sound_op),1,ptr);
      fwrite(&gore_op,sizeof(gore_op),1,ptr);
      fwrite(&split_op,sizeof(split_op),1,ptr);
      fwrite(&res_op,sizeof(res_op),1,ptr);
	  fwrite(&fullscr_res_op,sizeof(fullscr_res_op),1,ptr);
      fwrite(&cont_op,sizeof(t_control),4,ptr);
      fwrite(&sshots,sizeof(sshots),1,ptr);
      fwrite(&hell_op,sizeof(hell_op),1,ptr);
      fclose(ptr);

      for(i=0; i<MAX_GAME; i++)
         if(partida[i].en_uso){
            sprintf(s,"saved%d.sav",i);
            ptr=fopen(s,"w+b");
            fwrite(&partida[i],sizeof(game_info),1,ptr);
            fclose(ptr);
         }
}

void intro(ALLEGRO_DISPLAY *)
{
        ALLEGRO_BITMAP *aux, *letras[12], *logo, *presents, *pant;
        //RGB *paleta;
        ALLEGRO_AUDIO_STREAM *musica;
        int angulo, ang2, i,j,k,p;
        float escala, esc2, amp;

        //paleta=new RGB[256];

		//al_resize_display(game_display,640,480);
        pant=crear_bitmap(640,400);
        aux=cargar_bitmap("letras.pcx");
			al_convert_mask_to_alpha(aux,al_map_rgb(0,0,0));
        logo=cargar_bitmap("logo.pcx");
			al_convert_mask_to_alpha(logo,al_map_rgb(0,0,0));
        presents=cargar_bitmap("presents.pcx");
			al_convert_mask_to_alpha(presents,al_map_rgb(0,0,0));
        //musica=load_mod("am.jgm");
		musica = al_load_audio_stream("am.xm", 4, 2048);

        //set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
        //set_pallete(paleta);

        for(i=0; i<12; i++){
                letras[i]=crear_bitmap(50,50);
				al_set_target_bitmap(letras[i]);
				al_draw_bitmap_region(aux,0,50*i,50,50,0,0,0);
                //blit(aux,letras[i],0,50*i,0,0,50,50);
		};

        //clear_to_color(pant,0);
        //draw_sprite(pant,logo,0,0);
        //rotate_sprite(pant,logo,0,100,angulo);
        //blit(pant,screen,0,0,0,0,640,480);

		al_set_target_bitmap(pant);
		al_clear_to_color(al_map_rgb(0,0,0));
		al_draw_bitmap(logo,0,0,0);

        //set_mod_volume(120);
        //play_mod(musica,FALSE);
		if(music_op)
			al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());

        // Primera parte: 6 segundos. Logo rotando y acerc ndose
        angulo=360*2;
        escala=0.5+(360*0.01);

        speed_counter=0;
        do{
			inicia_tick();
            /*while(speed_counter > 0)*/{
                        angulo-=2;
                        escala-=0.01;
                        speed_counter--;
            };
            //clear_to_color(pant,0);
			al_set_target_bitmap(pant);
			al_clear_to_color(al_map_rgb(0,0,0));
            //rotate_scaled_sprite(pant,logo,320-int(200*escala),220-int(150*escala),itofix(angulo),ftofix(escala));
			al_draw_scaled_rotated_bitmap(logo,al_get_bitmap_width(logo)/2,al_get_bitmap_height(logo)/2,320,220,escala,escala,2*ALLEGRO_PI*angulo/360.f,0);
           update_display_and_flip(pant);

			while(!tick_terminado());

        }while(angulo>0);

        //Segunda parte: Caen las 12 letras y forman el t¡tulo

        for(j=0; j<12; j++){

                speed_counter=0; esc2=6;
                do{
					inicia_tick();
                /*while(speed_counter > 0)*/{

                        esc2-=0.2;
                        speed_counter--;
                };
                //clear_to_color(pant,0);
				al_set_target_bitmap(pant);
				al_clear_to_color(al_map_rgb(0,0,0));
                //rotate_scaled_sprite(pant,logo,320-int(200*escala),220-int(150*escala),itofix(0),ftofix(escala));
				al_draw_scaled_rotated_bitmap(logo,al_get_bitmap_width(logo)/2,al_get_bitmap_height(logo)/2,320,220,escala,escala,0,0);

                for(k=0; k<=5; k++)
                        if(k<j) al_draw_bitmap(letras[k],80+(40*k)-25,50-25,0); //draw_sprite(pant,letras[k],80+(40*k)-25,50-25);
                for(k=11; k>5; k--)
                        if(k<j) al_draw_bitmap(letras[k],80+(40*k)-25,50-25,0); //draw_sprite(pant,letras[k],80+(40*k)-25,50-25);

				//rotate_scaled_sprite(pant,letras[j],80+(40*j)-int(25*esc2),50-int(25*esc2),itofix(0),ftofix(esc2));
				al_draw_scaled_rotated_bitmap(letras[j],25,25,80+(40*j),50,esc2,esc2,0,0);

                update_display_and_flip(pant);

				while(!tick_terminado());

                }while(esc2>1.2);
        };

        // Tercera parte: letras de "presents" que se encogen
        speed_counter=0; amp=6;
        do{
				inicia_tick();
                /*while(speed_counter > 0)*/{

                        amp-=0.2;
                        speed_counter--;
                };
       al_set_target_bitmap(pant);
	   al_clear_to_color(al_map_rgb(0,0,0));
        //rotate_scaled_sprite(pant,logo,320-int(200*escala),220-int(150*escala),itofix(0),ftofix(escala));
	   al_draw_scaled_rotated_bitmap(logo,al_get_bitmap_width(logo)/2,al_get_bitmap_height(logo)/2,320,220,escala,escala,0,0);

       for(k=0; k<=5; k++)
                if(k<j) al_draw_bitmap(letras[k],80+(40*k)-25,50-25,0); //draw_sprite(pant,letras[k],80+(40*k)-25,50-25);
        for(k=11; k>5; k--)
                 if(k<j) al_draw_bitmap(letras[k],80+(40*k)-25,50-25,0); //draw_sprite(pant,letras[k],80+(40*k)-25,50-25);
         //stretch_sprite(pant,presents,320-int(amp*223/2),400,int(223*amp),38);
		al_draw_scaled_bitmap(presents,0,0,224,43,320-int(amp*223/2),340,int(223*amp),38,0);
        update_display_and_flip(pant);

		while(!tick_terminado());

        }while(amp>1.2);

        al_rest(2);

        // Cuarta parte: fade out imagen y volumen
        //fade_out(1);

        for(i=50; i>0; i--){
        //        set_mod_volume(i);
                //delay(50);
			al_set_audio_stream_gain(musica,(i*0.02));
			al_rest(0.05);
                };

		 al_set_target_bitmap(pant);
		 al_clear_to_color(al_map_rgb(0,0,0));
		 update_display_and_flip(pant);

        //stop_mod();
        //delete[] paleta;
        //destruir_bitmap(pant);
        destruir_bitmap(aux);
        destruir_bitmap(logo);
		destruir_bitmap(pant);
        for(i=0; i<12; i++) destruir_bitmap(letras[i]);
        destruir_bitmap(presents);
		//al_resize_display(game_display, 640, 400 );

        //destroy_mod(musica);
		al_detach_audio_stream(musica);
		al_destroy_audio_stream(musica);
}

bool evento_control(c_eventos e)
{
      controlm c;
      t_control tipos[6]={TEC1,TEC2,JOY1,JOY2,JOY3,JOY4};
      bool b=FALSE;
      for(int i=0; i<6; i++){

         c.actualiza(tipos[i]);
         switch(e){
                   case CARR : if(c.arr()) b=TRUE; break;
                   case CABA : if(c.aba()) b=TRUE; break;
                   case CDER : if(c.der()) b=TRUE; break;
                   case CIZQ : if(c.izq()) b=TRUE; break;
                   case CBU1 : if(c.boton(0)) b=TRUE; break;
                   case CBU2 : if(c.boton(1)) b=TRUE; break;
                   case CBU3 : if(c.boton(2)) b=TRUE; break;
                   case CBU4 : if(c.boton(3)) b=TRUE; break;
         };
      };
      return b;
}
void esperar_evento()
{
      while((!evento_control(CARR)) && (!evento_control(CABA)) && (!evento_control(CDER)) &&
            (!evento_control(CIZQ)) && (!evento_control(CBU1)) && (!evento_control(CBU2)) &&
            (!evento_control(CBU3)) && (!evento_control(CBU4))){};
}

void galeria( charset *ch)
{
		int espera=0, current_image = 0;
		bool image_changed = true;
		ALLEGRO_KEYBOARD_STATE keyboard;
		ALLEGRO_BITMAP *img = 0, *pant;
		const int NUM_IMAGES = 71;
		char s[60],
			image_files[NUM_IMAGES][60]={

				"Heroes inicial",
				 "Guerrero Omega",
				 "Omega musculoso",
				 "Guerrero sagrado",
				 "Hechicero Beta",
				 "Hechicero sagrado",
				 "Sigma y Omega",
				 "Soldado sagrado",

				"Tropa gamma",
				"Dragon y orco",
				"Esqueleto",
				 "Gamma y esqueleto",
				"Enemigos medieval 1",
				 "Enemigos medieval 2",
				 "Mago",
				 "Secundarios",

				 "Enemigos antiguo 1",
				 "Enemigos antiguo 2",
				 "Enemigos futuro 1",
				 "Enemigos futuro 2",
				 "Cyborg por Luke",
				 "Cyborg",
				 "Enemigos infierno 1",
				 "Enemigos infierno 2",
				 "Adan completo",
				 "Adan partes",

				 "Diagrama del programa",
				 "Batalla hipotetica",
				 "Charset medieval",
				 "Charset antiguo",
				 "Charset Infierno",

				 "Torre de Waraihan",

				 "Entrada al castillo",
				 "Sistema de cloacas",
				 "Cripta",
				 "Mazmorras",
				 "Patio Principal",
				 "Guarida de los hechiceros",
				 "Fortaleza subterranea",
				 "Mapa medieval",

				 "Aldea nomada",
				 "Ruinas en el bosque",
				 "Entrada en las cavernas",
				 "Nucleo del volcan",
				 "Laberinto maldito",
				 "Templo del Sol",
				 "Altar de Koppalha",
				 "Mapa antiguo",

				 "Laboratorio de calculo",
				 "Dormitorios de los empleados",
				 "Habitacion del ordenador principal",
				 "Mapa futuro",

				 "Tierras muertas de Harashisan",
				 "Castillo maldito",
				 "Altar satanico",
				 "Entrada al infierno",
				 "Hoyo de los gusanos",
				 "Ciudadela de carne",
				 "Cuatro puertas al nucleo",
				 "Mapa infierno",

				 "Ilustracion 3 heroes",
				 "Ilustracion beta",
				 "Ilustracion final",
				 "Ilustracion mundo",
				 "Ilustracion omega",
				 "Ilustracion sigma",

				 "IM1FULL",
				 "IM2FULL",
				 "IM3FULL",
				 "IM4FULL",
				 "IM5FULL"
				 };

		pant = crear_bitmap(1920,1080);

		inicia_tick();
        speed_counter=1;
        do{

            while(speed_counter > 0)
			{
				al_get_keyboard_state(&keyboard);

                if((evento_control(CDER)) && (espera==0)){
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
					current_image = (current_image + 1) % NUM_IMAGES;
					image_changed = true;
					espera = 30;
				};
				if((evento_control(CIZQ)) && (espera==0)){
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
					current_image = (current_image + NUM_IMAGES - 1) % NUM_IMAGES;
					image_changed = true;
					espera = 30;
				};

				if(((al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE)) || (evento_control(CBU2))) && (espera==0)){
                    if(sound_op) al_play_sample(cancel,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
					destruir_bitmap(pant);
					if(img)
						destruir_bitmap(img);
                    return;
				};

				if(image_changed)
				{
					if (img)
						destruir_bitmap(img);
					sprintf(s,"ARTE/%s.%s",image_files[current_image],"jpg");
					img = cargar_bitmap(s);
					image_changed = false;
				}

                speed_counter--;
                if (espera>0) espera--;

            };

			// Escala y muestra la imagen
			int bmpw = img ? al_get_bitmap_width(img) : 0;
			int bmph = img ? al_get_bitmap_height(img) : 0;
			int scrw = al_get_bitmap_width(pant);
			int scrh = al_get_bitmap_height(pant);

			al_set_target_bitmap(pant);
			al_clear_to_color(al_map_rgb(0,0,0));
			if(img)
			{
				if(bmpw != scrw || bmph != scrh)
				{
					if((float)scrw / (float)scrh > (float)bmpw / (float)bmph)
					{
						int vwporth = scrh;
						int vwportw = bmpw * scrh/bmph;
						al_draw_scaled_bitmap(img,0,0,bmpw,bmph,(scrw - vwportw) / 2,0,vwportw,vwporth,0);
					}
					else
					{
						int vwportw = scrw;
						int vwporth = bmph * scrw/bmpw;
						al_draw_scaled_bitmap(img,0,0,bmpw,bmph,0,(scrh - vwporth) / 2,vwportw,vwporth,0);
					}
				}
				else
				{
					al_draw_bitmap(img,0,0,0);
				}
			}

			ch->texto(pant,image_files[current_image],30,20,BGRIS,true);

			ch->texto(pant,"Left/Right: Change image - ESC: Exit gallery",30,al_get_bitmap_height(pant)-40,BGRIS,true);

			update_display_and_flip(pant);

			speed_counter = consume_ciclos_transcurridos();



         }while(1);

}


int elige_opcion(ALLEGRO_BITMAP *pant,int x1, int x2, int sy, int iy, int nop, int sop)
{
         ALLEGRO_BITMAP *sec[2], *sec_scr;
		 ALLEGRO_KEYBOARD_STATE keyboard;
         int o=sop, espera=15;

         sec[0]=crear_bitmap(32,32);
         sec[1]=crear_bitmap(32,32);
		// sec_scr = crear_bitmap(al_get_bitmap_width(pant),al_get_bitmap_height(pant));
		/* al_set_target_bitmap(sec_scr);
		 al_draw_bitmap( al_get_backbuffer(pant), 0, 0, 0);*/

		 inicia_tick();
         speed_counter=1;
         do{

            while(speed_counter > 0)
			{
				al_get_keyboard_state(&keyboard);

                if((evento_control(CARR)) && (espera==0)){
                                 o--; espera=10;
                                 if(o<0) o=nop-1;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								  if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                if((evento_control(CABA)) && (espera==0)){
                                 o++; espera=10;
                                 if(o==nop) o=0;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								  if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                //if(((key[KEY_ENTER]) || (evento_control(CBU1)))&& (espera==0)) {
				if(((al_key_down(&keyboard,ALLEGRO_KEY_ENTER)) || (evento_control(CBU1)))&& (espera==0)) {
                                 //if(sound_op)play_sample(accept,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(accept_snd,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
                                 destruir_bitmap(sec[0]);
                                 destruir_bitmap(sec[1]);
								 //destruir_bitmap(sec_scr);
                                 return o;};
                //if(((key[KEY_ESC]) || (evento_control(CBU2))) && (espera==0)){
				if(((al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE)) || (evento_control(CBU2))) && (espera==0)){
                                 //if(sound_op)play_sample(cancel,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(cancel,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
                                 destruir_bitmap(sec[0]);
                                 destruir_bitmap(sec[1]);
								 //destruir_bitmap(sec_scr);
                                 return -1;};
                speed_counter--;
                if (espera>0) espera--;
                cursor_a=(cursor_a+1)%40;
            };
            // Guarda cuadraditos
            //blit(pant,sec[0],x1,sy+(iy*o),0,0,30,30);
            //blit(pant,sec[1],x2,sy+(iy*o),0,0,30,30);
            //// Pinta sprites
            //masked_blit(cursor,pant,30*int(cursor_a/2),0,x1,sy+(iy*o),30,30);
            //masked_blit(cursor,pant,30*int(cursor_a/2),0,x2,sy+(iy*o),30,30);
            //blit(pant,screen,0,0,0,0,640,400);
            //// Restaura cuadraditos
            //blit(sec[0],pant,0,0,x1,sy+(iy*o),30,30);
            //blit(sec[1],pant,0,0,x2,sy+(iy*o),30,30);


			 // Guarda cuadraditos
			al_set_target_bitmap(sec[0]);
			al_draw_bitmap_region(pant,x1,sy+(iy*o),32,32,0,0,0);
			al_set_target_bitmap(sec[1]);
			al_draw_bitmap_region(pant,x2,sy+(iy*o),32,32,0,0,0);

			// Pinta sprites

			al_set_target_bitmap(pant);
			//al_draw_bitmap(sec_scr,0,0,0);
			al_draw_bitmap_region(cursor,30*int(cursor_a/2),0,30,30,x1,sy+(iy*o),0);
			al_draw_bitmap_region(cursor,30*int(cursor_a/2),0,30,30,x2,sy+(iy*o),0);

			update_display_and_flip(pant);

			speed_counter = consume_ciclos_transcurridos();

			// Restaura cuadraditos
			al_set_target_bitmap(pant);
			al_draw_bitmap(sec[0],x1,sy+(iy*o),0);
			al_draw_bitmap(sec[1],x2,sy+(iy*o),0);

         }while(1);
}
int menu_principal(charset *ch, ALLEGRO_BITMAP *pant)
{
        ALLEGRO_BITMAP *sah;

        //sah=load_pcx("sah.pcx",ch->dev_paleta());
		sah=cargar_bitmap("sah.pcx");
		al_convert_mask_to_alpha(sah,al_get_pixel(sah,0,0));

		 al_set_target_bitmap(pant);

        ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
        //draw_sprite(pant,sah,20,30);
		al_draw_bitmap(sah,20,30,0);
		ch->texto(pant, "REVIVAL", 500, 130, BAMA2, true);
        ch->texto(pant,"Heroes Crusade",240,180,BBLAN,true);
        ch->texto(pant,"Deathmatch",240,210,BBLAN,true);
        ch->texto(pant,"Options",240,240,BBLAN,true);
		ch->texto(pant,"Art Gallery",240,270,BBLAN,true);
        ch->texto(pant,"Quit Game",240,300,BBLAN,true);
        ch->texto(pant,"Active Minds 1999,2000,2001,2020",140,360,BBLAN,true);
        ch->texto(pant,VERSION_MODULO,20,370,BAMA1+2,true);
        //blit(pant,screen,0,0,0,0,640,400);
		al_flip_display();

        destruir_bitmap(sah);

        return elige_opcion(pant,170,440,170,30,5,0);
}
int nearest40(int n)
{
        if(n%40==0) return n;
        else        return n+(40-(n%40));
}
int menu(ALLEGRO_BITMAP *pant, charset *ch, const char *tit, char *ops[], int nops, int sop)
{

        int sx=strlen(tit)*13+80, sy=(nops+2)*30, scrx=al_get_bitmap_width(pant), scry=al_get_bitmap_height(pant);
        sx=nearest40(sx); sy=nearest40(sy);
        int ix=(scrx-sx)/2, iy=(scry-sy)/2,i,j;

		al_set_target_bitmap(pant);
        /*ch->set_tipo_transp(2);
        for(i=ix; i<ix+sx; i+=40)
        for(j=iy; j<iy+sy; j+=40)
                draw_trans_sprite(pant,cuadro_negro,i+15,j+15);*/
		al_draw_filled_rectangle(ix+15,iy+15,ix+sx+15,iy+sy+15,al_map_rgba(0,0,0,128));

        ch->ventana(pant,ix,iy,ix+sx,iy+sy,BMAR3,BMAR2,FALSE);
        ch->texto(pant,tit,(scrx-(strlen(tit)*13))/2,iy+15,BAMA1+2,true);
        for(i=0; i<nops; i++)
                ch->texto(pant,ops[i],(scrx-(strlen(ops[i])*13))/2,iy+50+(30*i),BMAR1,true);

        return elige_opcion(pant,ix+15,ix+sx-45,iy+43,30,nops,sop);
}
int menu_si_no(ALLEGRO_BITMAP *pant, charset *ch, const char *tit)
{
      char *ops[2],op;
      for(int i=0; i<2; i++) ops[i]=DBG_NEW char[5];
      sprintf(ops[0],"YES");
      sprintf(ops[1],"NO");
      op=menu(pant,ch,tit,ops,2,0);
	  for (int i = 0; i < 2; i++) delete[] ops[i];
	  return op;
}
void dialog_accept(ALLEGRO_BITMAP *pant, charset *ch, const char *msg, const char *option)
{
	char *ops[1];
	ops[0] = DBG_NEW char[60];
	sprintf(ops[0], option);
	menu(pant,ch,msg,ops,1,0);
	delete[] ops[0];
}
void ventana_texto(ALLEGRO_BITMAP *pant, charset *ch, const char *tit)
{
        int sx=strlen(tit)*13+80, sy=60, scrx=al_get_bitmap_width(pant), scry=al_get_bitmap_height(pant);
        sx=nearest40(sx); sy=nearest40(sy);
        int ix=(scrx-sx)/2, iy=(scry-sy)/2,i,j;

		al_set_target_bitmap(pant);
        /*ch->set_tipo_transp(2);
        for(i=ix; i<ix+sx; i+=40)
        for(j=iy; j<iy+sy; j+=40)
                draw_trans_sprite(pant,cuadro_negro,i+15,j+15);*/
		al_draw_filled_rectangle(ix+15,iy+15,ix+sx+15,iy+sy+15,al_map_rgba(0,0,0,128));

        ch->ventana(pant,ix,iy,ix+sx,iy+sy,BMAR3,BMAR2,FALSE);
        ch->texto(pant,tit,(scrx-(strlen(tit)*13))/2,iy+15,BAMA1+2,true);

		update_display_and_flip(pant);
}
void opciones(charset *ch,ALLEGRO_BITMAP *pant)
{
      int op=0,i,j;
      char *res_str[NUM_WIN_RESOLUTIONS], *fullscr_res_str[NUM_FUL_RESOLUTIONS], *cont_str[8];

      for(i=0; i<NUM_WIN_RESOLUTIONS; i++) {
		  res_str[i]=DBG_NEW char[10];
		  sprintf(res_str[i],"%dx%d",WINDOW_RESOLUTIONS[i][0],WINDOW_RESOLUTIONS[i][1]);
	  }

	  for(i=0; i<NUM_FUL_RESOLUTIONS; i++) {
		  fullscr_res_str[i]=DBG_NEW char[10];
		  sprintf(fullscr_res_str[i],"%dx%d",FULLSCR_RESOLUTIONS[i][0],FULLSCR_RESOLUTIONS[i][1]);
	  }

      for(i=0; i<8; i++) cont_str[i]=DBG_NEW char[15];
      sprintf(cont_str[0],"None");
      sprintf(cont_str[1],"Cursors");
      sprintf(cont_str[2],"WASDTYUI keys");
      sprintf(cont_str[3],"Joystick/Pad 1");
      sprintf(cont_str[4],"Joystick/Pad 2");
	  sprintf(cont_str[5],"Joystick/Pad 3");
	  sprintf(cont_str[6],"Joystick/Pad 4");
      sprintf(cont_str[7],"Mouse");

      do{
        ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);

        ch->texto(pant,"GAME OPTIONS CUSTOMIZE",200,40,BAMA1+2,true);

        ch->texto(pant,"Player 1 Controls",60,80,15,true);
        ch->texto(pant,"Player 2 Controls",60,105,15,true);
        ch->texto(pant,"Player 3 Controls",60,130,15,true);
        ch->texto(pant,"Player 4 Controls",60,155,15,true);
        ch->texto(pant,"Window Resolution",60,180,BMAR1,true);
		ch->texto(pant,"Full screen Resolution",60,205,BMAR1,true);
		ch->texto(pant,"Full screen",60,230,BMAR1,true);
        ch->texto(pant,"Sound",60,255,BMAR1,true);
        ch->texto(pant,"Music",60,280,BMAR1,true);
        ch->texto(pant,"Blood and Gore",60,305,BMAR1,true);
        ch->texto(pant,"Split Screen in Multiplayer",60,330,BMAR1,true);
        ch->texto(pant,"Exit",60,355,BMAR1,true);

        for(i=0; i<4; i++)
                ch->texto(pant,cont_str[(int)cont_op[i]],400,80+(25*i),15,true);
        ch->texto(pant,res_str[res_op],470,180,BMAR1,true);
		 ch->texto(pant,fullscr_res_str[fullscr_res_op],470,205,BMAR1,true);
		if(fullscr_op) ch->texto(pant,"YES",500,230,BMAR1,true);
        else         ch->texto(pant,"NO",500,230,BNEGR);
        if(sound_op) ch->texto(pant,"YES",500,255,BMAR1,true);
        else         ch->texto(pant,"NO",500,255,BNEGR);
        if(music_op) ch->texto(pant,"YES",500,280,BMAR1,true);
        else         ch->texto(pant,"NO",500,280,BNEGR);
        if(gore_op) ch->texto(pant,"YES",500,305,BMAR1,true);
        else         ch->texto(pant,"NO",500,305,BNEGR);
        if(split_op) ch->texto(pant,"Always",480,330,BMAR1,true);
        else         ch->texto(pant,"When Far",480,330,BMAR1,true);

		al_flip_display();
        //blit(pant,screen,0,0,0,0,640,400);

        op=elige_opcion(pant,20,590,73,25,12,op);
        switch(op){

            case 0 : case 1 : case 2 :
            case 3 : j=menu(pant,ch,"Control Method",cont_str,8,(int)cont_op[op]);
                     if(j!=-1) cont_op[op]=(t_control)j; break;
			case 4 : j=menu(pant,ch,"Window Resolution",res_str,NUM_WIN_RESOLUTIONS,res_op);
                     if(j!=-1)
						 {
							 res_op=j;
							 if(fullscr_op == false) al_resize_display(game_display,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);
						  }
					 break;
			case 5 : j=menu(pant,ch,"Full Screen Resolution",fullscr_res_str,NUM_FUL_RESOLUTIONS,fullscr_res_op);
                     if(j!=-1)
						 {
							 fullscr_res_op=j;
							 dialog_accept(pant,ch,"Exit game to apply changes","Accept");
						  }
					 break;
			case 6 : if(fullscr_op==TRUE) fullscr_op=FALSE; else fullscr_op=TRUE;
					 dialog_accept(pant,ch,"Exit game to apply changes","Accept");
					 //al_rest(0.5f);
					 /*al_destroy_display(game_display);
					 al_set_new_display_flags(fullscr_op ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED);
					 game_display = al_create_display(640, 400);*/
					 //pant = game_display;
				break;
            case 7 : if(sound_op==TRUE) sound_op=FALSE; else sound_op=TRUE; break;
            case 8 : if(music_op==TRUE) {
											music_op=FALSE;/* stop_mod();*/
											al_detach_audio_stream(musica);
										}
										else
										{
											music_op=TRUE; /*play_mod(musica,TRUE);*/
											al_attach_audio_stream_to_mixer(musica,al_get_default_mixer());

										}; break;
            case 9 : if(gore_op==TRUE) gore_op=FALSE; else gore_op=TRUE; break;
            case 10 : if(split_op==TRUE) split_op=FALSE; else split_op=TRUE; break;
            case -1: op=11; break;
        };

      }while(op!=11);

      for(i=0; i<NUM_WIN_RESOLUTIONS; i++) delete[] res_str[i];
	   for(i=0; i<NUM_FUL_RESOLUTIONS; i++) delete[] fullscr_res_str[i];
      for(i=0; i<8; i++) delete[] cont_str[i];

	   guarda_config();
}
void str_hora(unsigned long int i,char *str)
{
      int h,m,s;

      h=(int)(i/3600);
      m=(int)((i-(h*3600))/60);
      s=(int)(i%60);
      sprintf(str,"%d:%02d:%02d",h,m,s);
}
int elige_partida(charset *ch, ALLEGRO_BITMAP *pant, bool vacias)
{
       char s[20], dif_str[5][15]={"EASY","NORMAL","HARD","VERY HARD","NIGHTMARE"};
       int op;

       ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
       ch->texto(pant,"CHOOSE A GAME FILE",200,20,BAMA1+2);

       for(int i=0; i<MAX_GAME; i++){
           rect(pant,59,59+45*i,581,101+45*i,BMAR3);
            rect(pant,60,60+45*i,580,100+45*i,BMAR3+3);
            rect(pant,61,61+45*i,579,99+45*i,BMAR3+6);
            if(partida[i].en_uso==FALSE)
               ch->texto(pant,"Empty File",100,75+45*i,BMAR1,true);
            else{
               rectfill(pant,62,62+45*i,578,98+45*i,BMAR3+9);
               for(int j=0; j<partida[i].n_players; j++){
                  rectfill(pant,70+(160*(j%2)),65+(int)(20*(j/2))+45*i,70+(160*(j%2))+13*6,77+(int)(20*(j/2))+45*i,partida[i].pl[j].c1+4);
                  ch->texto(pant,partida[i].pl[j].nombre,70+(160*(j%2)),65+(int)(20*(j/2))+45*i,BAMA1+2);
                  rectfill(pant,160+(160*(j%2)),65+(int)(20*(j/2))+45*i,13*3+160+(160*(j%2)),12+65+(int)(20*(j/2))+45*i,partida[i].pl[j].c2+4);
                  sprintf(s,"L%d",partida[i].pl[j].level);
                  ch->texto(pant,s,160+(160*(j%2)),65+(int)(20*(j/2))+45*i,BAMA1+2);
               };

               ch->texto(pant,dif_str[partida[i].dif-1],390,65+45*i,BMAR1);
               str_hora(partida[i].tiempo,s);
               ch->texto(pant,s,390,85+45*i,BMAR1);
               sprintf(s,"%d%c",partida[i].porcent,'%');
               ch->texto(pant,s,520,85+45*i,BMAR1);
            };
       };
       if(vacias) return elige_opcion(pant,20,590,65,45,MAX_GAME,0);
       else{
              do{
                   op=elige_opcion(pant,20,590,65,45,MAX_GAME,0);
              }while(partida[op].en_uso==FALSE);
              return op;
       };
}
int elige_clase(charset *ch, ALLEGRO_BITMAP *pant, int p)
{
         char s[50];
         int o=1,espera=10,i;
         float osc[3]={24.0,24.0,24.0};
         ALLEGRO_BITMAP *fotos[3], *oscur[25];
		 ALLEGRO_KEYBOARD_STATE keyboard;

         for(i=1; i<=3; i++){
                  sprintf(s,"%s/clase%d.%s",GDIR,i,FILE_IMG_EXT);
                  fotos[i-1]=cargar_bitmap(s);
         };
         for(i=0; i<25; i++){
                  oscur[i]=crear_bitmap(186,226);
                  //clear_to_color(oscur[i],55+i*8);
         };
         ch->set_tipo_transp(2);

		 inicia_tick();

         speed_counter=1;
         do{

            while(speed_counter > 0)
			{

				al_get_keyboard_state(&keyboard);

                if((evento_control(CARR)) && (espera==0) && (o==3)){
                                 o=1; espera=10;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								  if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                if((evento_control(CABA)) && (espera==0) && (o<3)){
                                 o=3; espera=10;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                if((evento_control(CDER)) && (espera==0) && (o<2)){
                                 o++; espera=10;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                if((evento_control(CIZQ)) && (espera==0) && (o>0)){
                                 o--; espera=10;
                                 //if(sound_op)play_sample(selec,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
                if(((al_key_down(&keyboard,ALLEGRO_KEY_ENTER)) || (evento_control(CBU1)))
                && (espera==0)) {
                                 //if(sound_op)play_sample(accept,255,128,1000,FALSE);
								  if(sound_op) al_play_sample(accept_snd,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
                                 for(i=0; i<3; i++) destruir_bitmap(fotos[i]);
                                 for(i=0; i<25; i++) destruir_bitmap(oscur[i]);
                                 return o;};
                if(((al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE)) || (evento_control(CBU2))) && (espera==0)){
                                 //if(sound_op)play_sample(cancel,255,128,1000,FALSE);
								 if(sound_op) al_play_sample(cancel,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
                                 for(i=0; i<3; i++) destruir_bitmap(fotos[i]);
                                 for(i=0; i<25; i++) destruir_bitmap(oscur[i]);
                                 return -1;};
                speed_counter--;
                if (espera>0) espera--;
                for(i=0; i<3; i++){
                        if((i==o) && (osc[i]<24.0)) osc[i]+=0.5;
                        if((i!=o) && (osc[i]>0.0)) osc[i]-=0.5;
                }
            };

			al_set_target_bitmap(pant);
            ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);

            sprintf(s,"PLAYER %d PLEASE CHOOSE A CLASS",p);
            ch->texto(pant,s,130,20,BAMA1+2,true);
            ch->ventana(pant,0,320,640,400,BMAR3,BMAR2,FALSE);
            ch->ventana(pant,10,50,210,290,BMAR3,BMAR2,FALSE);
            //draw_sprite(pant,fotos[0],17,57);
			al_draw_bitmap(fotos[0],17,57,0);
            //draw_trans_sprite(pant,oscur[int(osc[0])],17,57);
			al_draw_filled_rectangle(17,57,17+186,57+226,al_map_rgba_f(0,0,0,0.8f-(0.8f*osc[0]/24.f)));
            ch->ventana(pant,220,50,420,290,BMAR3,BMAR2,FALSE);
            //draw_sprite(pant,fotos[1],227,57);
			al_draw_bitmap(fotos[1],227,57,0);
            //draw_trans_sprite(pant,oscur[int(osc[1])],227,57);
			al_draw_filled_rectangle(227,57,227+186,57+226,al_map_rgba_f(0,0,0,0.8f-(0.8f*osc[1]/24.f)));
            ch->ventana(pant,430,50,630,290,BMAR3,BMAR2,FALSE);
            //draw_sprite(pant,fotos[2],437,57);
			al_draw_bitmap(fotos[2],437,57,0);
            //draw_trans_sprite(pant,oscur[int(osc[2])],437,57);
			al_draw_filled_rectangle(437,57,437+186,57+226,al_map_rgba_f(0,0,0,0.8f-(0.8f*osc[2]/24.f)));
            ch->texto(pant,"Import Character",225,300,BMAR1,true);
            if(o<3) rect(pant,10+(210*o),50,210+(210*o),290,BROJO);
            else    rect(pant,150,295,490,315,BROJO);
            switch(o){

               case 0 : ch->texto(pant,"Omega Warrior",15,335,BAMA1+2,true);
                        ch->texto(pant,"Attacks with swords and defends with shields.",15,352,BMAR1,true);
                        ch->texto(pant,"Has to collect each sword or shield.",15,369,BMAR1,true); break;
               case 1 : ch->texto(pant,"Sigma Soldier",15,335,BAMA1+2,true);
                        ch->texto(pant,"Shots with guns and fire weapons",15,352,BMAR1,true);
                        ch->texto(pant,"Has to collect every weapon and appropiate ammo.",15,369,BMAR1,true); break;
               case 2 : ch->texto(pant,"Beta Sorcerer",15,335,BAMA1+2,true);
                        ch->texto(pant,"Attacks and defends by casting magic spells",15,352,BMAR1,true);
                        ch->texto(pant,"Has to collect energy to cast the spells",15,369,BMAR1,true); break;
               case 3 : ch->texto(pant,"Import a character from a previous saved game",15,335,BMAR1,true); break;
            };
            update_display_and_flip(pant);

			speed_counter = consume_ciclos_transcurridos();

         }while(1);


}

void input_string(charset *ch, ALLEGRO_BITMAP *pant, const char *tit, char *s, int maxs)
{
   char k;
   int tec, c=strlen(s), espera=10;
   ALLEGRO_BITMAP *aux=crear_bitmap(280,80);
   ALLEGRO_EVENT key;
   bool finished = false;

   speed_counter=0;
   controlm::flushallkeys();
   //clear_keybuf();
   //blit(pant,aux,180,160,0,0,280,80);
   al_set_target_bitmap(aux);
   al_draw_bitmap_region( pant , 180,160,280,80,0,0,0);
   do{
         //while(speed_counter > 0)
		 {
				inicia_tick();

                if((controlm::keypressed()) && (espera==0)){

                    /* tec=readkey();
                     k=tec>>8;
                     if(k==KEY_BACKSPACE){
                           if(c>0){
                              c--; s[c]='\0';
                              espera=10;
                              if(sound_op)al_play_sample(cancel,255,128,1000,FALSE);};
                     }else if(k==KEY_ENTER){
                           espera=10;
                           if(sound_op)al_play_sample(accept,255,128,1000,FALSE);
                     }else if(c<6){
                           s[c]=(char)tec; s[c+1]='\0'; c++;
                           espera=10;
                           if(sound_op)al_play_sample(selec,255,128,1000,FALSE);
                     };*/

					if(controlm::readkey(&key))
					{
						if(key.keyboard.type == ALLEGRO_EVENT_KEY_CHAR)
						{
							if(key.keyboard.keycode == ALLEGRO_KEY_BACKSPACE )
							{
								if(c>0){
									c--; s[c]='\0';
									espera=10;
									if(sound_op)al_play_sample(cancel,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
								};
							}
							else if (key.keyboard.keycode == ALLEGRO_KEY_ENTER)
							{
								 espera=10;
								if(sound_op)al_play_sample(accept_snd,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
								finished = true;
							}
							else if(c<maxs)
							{
								s[c]=(char)key.keyboard.unichar; s[c+1]='\0'; c++;
								espera=10;
								if(sound_op)al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
							}
						}
					}
                };

                speed_counter--;
                if (espera>0) espera--;

				 al_set_target_bitmap( pant );
				ch->ventana(pant,180,160,460,240,BMAR3,BMAR2,FALSE);
				ch->texto(pant,tit,(640-(strlen(tit)*13))/2,185,BMAR1,true);
				ch->texto(pant,s,(640-(strlen(s)*13))/2,210,BAMA1+2,true);
				if((int)al_get_time() % 2 == 0)
					ch->texto(pant,"|",(640-(strlen(s)*13))/2 + (strlen(s)*13),210,BAMA1+2,true);
				update_display_and_flip(pant);

				while(!tick_terminado());
         };


   }while(!finished);

   //blit(aux,pant,0,0,180,160,280,80);
   al_set_target_bitmap( pant );
   al_draw_bitmap( aux, 180, 160, 0);
   destruir_bitmap(aux);
}
int elige_color(charset *ch, ALLEGRO_BITMAP *pant, int op)
{
	 byte color[20]={BROJO,BGRIS,BNEGR,BROSA,BNARA,BAMA1,BAMA2,BVER1,
            BVER2+8,BVER3,BAZUC+4,BAZUM,BAZUN,BAZUO,BLILA,BVIOL+4,BMAR1+2,BMAR2,
            BMAR3,BIRIS};


	  ALLEGRO_KEYBOARD_STATE keyboard;
	  int sel = op, espera = 10;

	  bool salir = false;

	  al_set_target_bitmap(pant);
	  int x = 180;
	  int y = 80;
	  al_draw_filled_rectangle(x+15,y+15,x+280+15,y+240+15,al_map_rgba(0,0,0,128));

	  inicia_tick();
      speed_counter=1;
      do{

        while(speed_counter > 0){
			// Logic
			al_get_keyboard_state(&keyboard);

            if((evento_control(CDER)) && (espera==0)){
                sel = (sel + 1) % 20; espera=10;
				if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
			};
			if((evento_control(CIZQ)) && (espera==0)){
                sel = (sel - 1 + 20) % 20; espera=10;
				if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
			};
			if((evento_control(CABA)) && (espera==0)){
                sel = (sel + 5) % 20; espera=10;
				if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
			};
			if((evento_control(CARR)) && (espera==0)){
                sel = (sel - 5 + 20) % 20; espera=10;
				if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
			};
			if(((al_key_down(&keyboard,ALLEGRO_KEY_ENTER)) || (evento_control(CBU1)))&& (espera==0)) {
                salir = true;
				if(sound_op) al_play_sample(accept_snd,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
            };

			if(espera > 0) espera--;

            speed_counter--;
        };

		al_set_target_bitmap(pant);
		ch->ventana(pant,x,y,x+280,y+240,BMAR3,BMAR2,false);
		for(int j = 0; j < 20; j++)
		{
			int xx = x + 15 + 50*(j%5);
			int yy = y + 25 + 50*(int)(j/5);
			for(int i = 0; i < 8; i++)
				rectfill(pant,xx+(5*i),yy,xx+(5*i)+5,yy+40,color[j]+i);
		}

		int sx = x + 15 + 50*(sel%5);
		int sy = y + 25 + 50*(int)(sel/5);
		rect(pant, sx-1, sy-1, sx+40+1, sy+40+1, BROJO);
		rect(pant, sx-2, sy-2, sx+40+2, sy+40+2, BROJO);

		update_display_and_flip(pant);

		speed_counter = consume_ciclos_transcurridos();

	  } while (!salir);

	return sel;
}
void personalizar(charset *ch, ALLEGRO_BITMAP *pant, batalla *bt, player_info *pl, bool quitar_baston)
{
       int op=0,i;
       warrior *w;
       char s[20], img_sprites[50], clase[3][10]={"Soldier",
                                 "Sorcerer",
                                 "Warrior"};
       byte color[20]={BROJO,BGRIS,BNEGR,BROSA,BNARA,BAMA1,BAMA2,BVER1,
            BVER2+8,BVER3,BAZUC+4,BAZUM,BAZUN,BAZUO,BLILA,BVIOL+4,BMAR1+2,BMAR2,
            BMAR3,BIRIS}, a1=0, a2=0;

	   for(i = 0; i < 20; i++)
	   {
		   if (pl->c1 == color[i]) a1 = i;
		   if (pl->c2 == color[i]) a2 = i;
	   }

       ch->limpia_matriz_luz(MEDLUZ);



	    w=DBG_NEW warrior("",0,0,DABA,pl->spr,0,pl->clase,pl->c1,pl->c2,1,0);
		if(quitar_baston) w->set_baston_mago(false);
		sprintf(img_sprites,"%s/%s.%s",bt->directorio(),bt->dev_nivel()->fspr(w->cualspr()),FILE_IMG_EXT);
		int id_skin = ch->crea_skin_personaje(img_sprites,w->cualspr(),pl->c1,pl->c2);
		w->set_skin(id_skin);
		bool refresh_skin = false;

       do{

			if(refresh_skin)
			{
				 ch->destruye_skin(id_skin);
				 id_skin = ch->crea_skin_personaje(img_sprites,w->cualspr(),pl->c1,pl->c2);
				 w->set_skin(id_skin);
				 refresh_skin = false;
			}

			al_set_target_bitmap( pant );
			ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
			ch->texto(pant,"CUSTOMIZE THE CHARACTER",150,20,BAMA1+2,true);
            ch->ventana(pant,460,70,580,190,BMAR3,BMAR2,FALSE);
            rectfill(pant,470,80,570,180,0);
            ch->muestra_guerrero(pant,w,-14,3,0,0,TRUE);
            ch->texto(pant,clase[(int)pl->clase],400,220,BMAR1,true);
            sprintf(s,"Level:     %d",pl->level);
            ch->texto(pant,s,400,250,BMAR1,true);
            sprintf(s,"Experience:%d",pl->exp);
            ch->texto(pant,s,400,280,BMAR1,true);

            ch->texto(pant,"Character Name",60,80,BMAR1,true);
            rectfill(pant,60,97,148,116,0);
            ch->texto(pant,pl->nombre,70,100,BAMA1+2,true);

            for(i=0; i<8; i++)
                    rectfill(pant,60+(30*i),140,90+(30*i),180,pl->c1+i);
            rect(pant,60,140,300,180,BROJO+6);
            rect(pant,61,141,299,179,BROJO+3);
            rect(pant,62,142,298,178,BROJO);
            ch->texto(pant,"Primary Color",80,155,BNEGR);
            for(i=0; i<8; i++)
                    rectfill(pant,60+(30*i),220,90+(30*i),260,pl->c2+i);
            ch->texto(pant,"Secondary Color",80,235,BNEGR);
            rect(pant,60,220,300,260,BROJO+6);
            rect(pant,61,221,299,259,BROJO+3);
            rect(pant,62,222,298,258,BROJO);
            ch->texto(pant,"Finish",60,320,BMAR1,true);

            op=elige_opcion(pant,20,320,70,80,4,op);
            switch(op){
               case -1: op=0; break;
               case 0 : input_string(ch,pant,"Type in your name",pl->nombre,6); break;
               case 1 :  a1 = elige_color(ch,pant,a1); pl->c1=color[a1]; refresh_skin = true; break;
               case 2 :  a2 = elige_color(ch,pant,a2); pl->c2=color[a2]; refresh_skin = true; break;
            };


       }while(op!=3);

	   delete w;
}

void calcula_porcent(int c)
{
      float s=0;
      for(int i=0; i<4; i++)
      for(int j=1; j<8; j++)
         s+=partida[c].exitos[i][j];
      s+=(partida[c].train+partida[c].sacred+
          partida[c].kti+partida[c].completo);
      partida[c].porcent=(byte)((s/115)*100);
}
void prologo(ALLEGRO_BITMAP *pant, charset *ch, int epi, int c)
{
      int num_lines=12,j;
      float i=420;
      ALLEGRO_BITMAP *pic;
      char s[60];
      char hist[2][4][12][50]=
         {{{"For centuries, the orcs have been humans's",
            "enemies. When the Invaders came, the orcs",
            "immediately allied with them. After being",
            "expeled of their fortress by the legendary",
            "Sigma Soldier, they decided to take a huge",
            "castle near the village of Green River, in",
            "the realm of Valiant Land. The Orc King has",
            "got one of the Three Gems, so you must find",
            "a way to reconquer the castle and defeat",
            "both the orcs, their mercenaries and the",
            "other criatures that reside in some zones",
            "of the castle."},
           {"The Gheon realm has got the most advanced",
            "civilization in our world. The Silver Moon",
            "fortress, located in the capital, works as",
            "the cybernetic brain of the whole region.",
            "When the Invaders hacked the computer sys-",
            "tem, actually got the control over all the",
            "zone. It's not easy to enter this concrete",
            "fortress, beacuse all the vigilance droids",
            "are armed and with orders of destroy any",
            "intruder, but is the only way to recover",
            "the Gem which has got the cyborg who",
            "conquered Silver Moon."},
           {"Since the ancient times, in the region of",
            "Gookan-Da, the nomadic tribes have adored",
            "their God of the Sun, Koppalha. His sanc-",
            "tuary, the Temple of Sun, placed in the",
            "slope of a volcano, has been taken by the",
            "Invaders. They have introduced a false Ko-",
            "ppalha that now leads all the priests, who",
            "believe that he is their God. The only way",
            "to restore the order is enter the temple",
            "and unmask the false Koppalha, then take",
            "the Gem that he has. But the zone is home",
            "of many beasts and tribes of barbarians..."},
           {"With the Three Gems in our hands, we have",
            "been able to create the Sacred Armors that",
            "will allow you to enter the Invaders home",
            "without dying. It's an organic fortress",
            "located on the Dead Lands of Harashisan,",
            "made of the own caparison of the Invaders",
            "and of the rests of the victims that they",
            "killed for feeding. There's nothing in the",
            "world more similar to Hell. We don't know",
            "anymore about what you will find there,",
            "but prepare yourself for the worst of the",
            "horrors..."}},
          {{"Finally, the miserable Orc King has been",
            "cut down and all his minions run away back",
            "to the dark regions of the swamps.And also",
            "the Invaders have lost one of their main",
            "headquarters. Now, the realm of Valiant",
            "Land belongs again to their legitimate ow-",
            "ners, the humans.",
            "And so, with the Earth Gem in your hands,",
            "you get out from the underground fortress",
            "and breathe again the pure air of these",
            "beautiful valleys.",
            ""},
           {"At the moment that the death scream of the",
            "cyborg monster deafens you, you see how all",
            "the computers of the fortress work again",
            "without the control of the hacker. All the",
            "robot units cease to attack you, and all",
            "the traps became inactive.",
            "While the operators take care of the reco-",
            "vered systems, you grab the Fire Gem from",
            "the viscera of the monster and make your",
            "way out of the building. The time of this",
            "cybernetic nitghmare has reached its end.",
            ""},
            {"Even the mastering of the apocalypse spell",
             "cannot save this impostor from the hand of",
             "justice. The false Koppalha was only a po-",
             "werfull sorceror priest that used his magic",
             "to deceive the priests of the temple.",
             "By sacrificing the village inhabitants, he",
             "wanted to start a reign based in the terror",
             "and constraint. Now the temple becomes aga-",
             "in the holy place that it was and you have",
             "in your hand the sacred Gem of Water.",
             "",
             ""},
             {"\"Hahaha...pathetic humans!...",
              "So you think that slaying the Motherbrain",
              "you can put an end to our plan...You're so",
              "wrong!",
              "During this time, we have observed you. We",
              "learnt a lot about your nature. We have se-",
              "lected your best qualities and we created",
              "the Ultimate Being, that will be the first",
              "of a new race of invaders...",
              "We call him 'Adan',in honor of our beloved",
              "guests,the humans. So, now you'll have the",
              "great privilege to meet him!...\""
             }}
         };
      char sacred[12][50]=
             {"You succeeded in your quest. Finally, the",
              "Mystical Gems are back in our hands. Those",
              "arcane relics are imbued with the power of",
              "the ancient magicians, so them cannot be",
              "corrupted by the evil power of the Inva-",
              "ders. By creating a special clothes with",
              "this magic, we can provide you the sacred",
              "protection of the light.",
              "But now,the true battle begins. Wear this",
              "Sacred Armor and go to face the pure evil",
              "in his own domains: The dead lands of",
              "Harashisan..."};
      char adan[12][50]=
             {"The body of the Adan lies in the ground of",
              "the Mother Brain chamber. You escape trough",
              "the tunnels and come back to the coast of",
              "the dead continent, while the fortress begin",
              "to collapse.Yes,it's a victory,but you know",
              "that the menace is not over. The Invaders",
              "still desire to take control over your Earth",
              "and sure they'll come back. May be this time",
              "they will create an army of monsters like",
              "the Adan, so the battle will be more bloody",
              "than ever...",
              ""};

      switch(c){
            case 1 :
            case 0 : j=5; break;
            case 2 : j=4; break;
            case 3 : j=6; break;};

      sprintf(s,"%s/imagen%d.%s",GDIR,j,FILE_IMG_EXT);
      pic=cargar_bitmap(s);

      speed_counter=0;
      while((i>-300) && (!al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))){

		    inicia_tick();
		    al_get_keyboard_state(&key_state);
           /* while(speed_counter > 0){

                i-=0.3;
                speed_counter--;
            };*/
			i-=0.3;
            //blit(pic,pant,0,0,0,0,640,400);
			al_set_target_bitmap(pant);
			al_draw_bitmap(pic,0,0,0);
			al_set_clipping_rectangle(0,80,640,320);
            for(j=0; j<num_lines; j++)
               switch(c){
                         case 0:
                         case 1: ch->texto(pant,hist[c][epi][j],35,i+(30*j),BAMA1+2,true); break;
                         case 2: ch->texto(pant,sacred[j],35,i+(30*j,true),BAMA1+2); break;
                         case 3: ch->texto(pant,adan[j],35,i+(30*j),BAMA1+2,true); break;
               };
            //blit(pic,pant,0,0,0,0,640,80);
            //blit(pic,pant,0,320,0,320,640,400);
			al_reset_clipping_rectangle();
            if((c==0) || (c==1))
            ch->texto(pant,epi_descr[epi],(640-(strlen(epi_descr[epi])*13))/2,33,BMAR1,true);
            update_display_and_flip(pant);

			while(!tick_terminado());
      };
      destruir_bitmap(pic);
}
void escena_final(ALLEGRO_BITMAP *pant, charset *ch, int par)
{
        char frases[4][2][50]=
                {{"\"So, you're still not surrending...",
                  "You still want our lives, isn't it?..."
                  },
                 {"Don't matter how many times you try",
                    "to crush us, it will be useless..."
                  },
                 {"Today we finally are calm. Nothing will",
                  "fear us anymore. At last we realize..."
                  },
                 {"The Legend of the Sacred Armor Heroes",
                  "will never die!!!...\""}
                 };
        char credits[35][50]=
             {"Active Minds - STAFF","","Original concept and creation",
              "NHSP & MTX","Main supervisor and programmer","NHSP",
              "History and character design","NHSP","Graphics design",
              "NHSP & MTX","Level design","NHSP & MTX","Sound Effects & MOD Music",
              "MTX","SAHME and auxiliar tools","NHSP","Character Voices : Cast",
              "NHSP","MTX","Victor Rodr¡guez","Concepcion Pellicer","Special Thanks:",
              "Shawn Hargreaves (Allegro Library)","Matthew Leverton (Allegro.CC web site)",
              "LUKE (will you play?)","Jordi Poch (sure you'll play!)",
              "All the Allegro.CC testers","And of course, to YOU!","",
              "Active Minds 1999,2000,2001,2018","","And remember:",
              "\"The best is always still to come...\""
        };
        char s[30];
        float i=0;
        int k,j,l;
        ALLEGRO_BITMAP *pic=NULL;

        // Muerte de Ad n
         /*if(musica!=NULL) destroy_mod(musica);
         sprintf(s,"%s\\warofire.%s",GDIR,FILE_MOD_EXT);
         musica=load_mod(s);
         if(music_op){
             set_mod_volume(120);
             play_mod(musica,TRUE);
         }*/
		 if(musica != NULL) { al_destroy_audio_stream(musica); musica = NULL; }
		 sprintf(s,"%s/warofire.%s",GDIR,FILE_MOD_EXT);
		 musica =  cargar_musica(s);
		 if(music_op)
			al_attach_audio_stream_to_mixer(musica,al_get_default_mixer());

        prologo(pant,ch,0,3);
        //stop_mod();

        // Charla de Waraihan
        sprintf(s,"%s/imagen7.%s",GDIR,FILE_IMG_EXT);
        pic=cargar_bitmap(s);
        speed_counter=0;
        while((i<1200) && (!al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))){

			inicia_tick();
			al_get_keyboard_state(&key_state);
            /*while(speed_counter > 0)*/{

                i++;
                speed_counter--;
            };

			al_set_target_bitmap(pant);
            ch->ventana(pant,0,0,640,400,0,0,FALSE);
            //blit(pic,pant,0,0,40,40,560,260);
			al_draw_bitmap(pic,40,40,0);
            if((i>240) && (i<1200)){

                ch->texto(pant,frases[int((i/240)-1)][0],(640-(strlen(frases[int((i/240)-1)][0])*13))/2,330,BAMA1+2,true);
                ch->texto(pant,frases[int((i/240)-1)][1],(640-(strlen(frases[int((i/240)-1)][1])*13))/2,360,BAMA1+2,true);
            }
            update_display_and_flip(pant);

			while(!tick_terminado());
        };
        // Cr‚ditos
        speed_counter=0; i=600; k=0; l=1;
        /* if(musica!=NULL) destroy_mod(musica);
         sprintf(s,"%s\\ironpowe.%s",GDIR,FILE_MOD_EXT);
         musica=load_mod(s);
         if(music_op){
             set_mod_volume(120);
             play_mod(musica,TRUE);
         }*/
		 if(musica != NULL) { al_destroy_audio_stream(musica); musica = NULL; }
		 sprintf(s,"%s/ironpowe.%s",GDIR,FILE_MOD_EXT);
		 musica =  cargar_musica(s);
		 if(music_op)
			al_attach_audio_stream_to_mixer(musica,al_get_default_mixer());

        while((i>-2000) && (!al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))){

			inicia_tick();
			al_get_keyboard_state(&key_state);
            /*while(speed_counter > 0)*/{

                if(k%1800==0){
                      if(pic!=NULL) destruir_bitmap(pic);
                      sprintf(s,"%s/imagen%d.%s",GDIR,l,FILE_IMG_EXT);
                      pic=cargar_bitmap(s);
                      l++;
                };
                i-=0.3; k++;
                speed_counter--;
            };
            //blit(pic,pant,0,0,0,0,640,400);
			al_set_target_bitmap(pant);
			al_draw_bitmap(pic,0,0,0);
            for(j=0; j<34; j++)
                     ch->texto(pant,credits[j],(640-strlen(credits[j])*13)/2,i+(j*60),BAMA1+2,true);
            update_display_and_flip(pant);
			while(!tick_terminado());
        };
        //stop_mod();
		al_detach_audio_stream(musica);

        partida[par].completo=1;
        calcula_porcent(par);
}
void jugar_kti(ALLEGRO_BITMAP *pant, charset *ch, int par, int scrx, int scry)
{
      kti *gal;

      gal=DBG_NEW kti(&partida[par],pant,ch);

	  inicia_tick();
      speed_counter=1;
      while(!gal->acabado()){

				___START_PROFILER()
				___ENTER_PROFILER_BLOCK("TICK")
				___ENTER_PROFILER_BLOCK("kti::actualiza")
				while(speed_counter>0){
                        gal->actualiza(cont_op);
                        speed_counter--;
                }
				___LEAVE_PROFILER_BLOCK("kti::actualiza")

				___ENTER_PROFILER_BLOCK("kti::render")
                gal->muestra(pant);
				update_display_and_flip(pant);
				___LEAVE_PROFILER_BLOCK("kti::render")

				speed_counter = consume_ciclos_transcurridos();
				___LEAVE_PROFILER_BLOCK("TICK")

				al_get_keyboard_state(&key_state);
				if(al_key_down(&key_state,ALLEGRO_KEY_F8))
				{
					___SHOW_PROFILER()
				}
      };
      if(gal->completo()){ partida[par].kti=1; calcula_porcent(par);};
      delete gal;
}
void rank(int par, int c, char *s)
{
        char c1[20], c2[20], c3[20];

        switch(int(partida[par].porcent/20)){
                case 0 : sprintf(c1,"Inept"); break;
                case 1 : sprintf(c1,"Clumsy"); break;
                case 2 : sprintf(c1,"Normal"); break;
                case 3 : sprintf(c1,"Average"); break;
                case 4 : sprintf(c1,"Genial"); break;
                case 5 : sprintf(c1,"Supreme"); break;
        }
        switch(partida[par].dif-1){
                case 0 : sprintf(c2,"Coward"); break;
                case 1 : sprintf(c2,"Common"); break;
                case 2 : sprintf(c2,"Daring"); break;
                case 3 : sprintf(c2,"Brave"); break;
                case 4 : sprintf(c2,"Fearless"); break;
        }
        switch(int(partida[par].pl[c].level/6)){
                case 0 : sprintf(c3,"Apprentice"); break;
                case 1 : sprintf(c3,"Cadet"); break;
                case 2 : sprintf(c3,"Warrior"); break;
                case 3 : sprintf(c3,"Knight"); break;
                case 4 : sprintf(c3,"Hero"); break;
                default : sprintf(c3,"Master"); break;
        }
        sprintf(s,"%s & %s %s",c1,c2,c3);
}
void status(charset *ch, ALLEGRO_BITMAP *pant, batalla *b, int par, int pl)
{
        char s[60], a[30], names[4][10]={"Mediev."," Future"," Ancient","  Hell"};
        int SX=400, SY=280;
        int ox=int((al_get_bitmap_width(pant)-SX)/2), oy=int((al_get_bitmap_height(pant)-SY)/2),i,j;

		al_set_target_bitmap(pant);
        ch->ventana(pant,ox,oy,ox+SX,oy+SY,0,0,FALSE);
        ch->texto(pant,"GAME STATUS",ox+130,oy+10,BAMA1+2,true);

        for(i=0; i<4; i++){
                ch->texto(pant,names[i],ox+20+90*i,oy+40,BMAR1,true);
                for(j=0; j<7; j++){
                        switch(partida[par].exitos[i][j+1]){
                                case 0 : sprintf(a," "); break;
                                case 1 : sprintf(a,"¶"); break;
                                case 2 : sprintf(a,"·"); break;
                                case 3 : sprintf(a,"¸"); break;
                                case 4 : sprintf(a,"¹"); break;
                        };
                        rect(pant,ox+40+90*i,oy+60+20*j,ox+100+90*i,oy+80+20*j,BROSA);
                        ch->texto(pant,a,ox+66+90*i,oy+64+20*j,BCO2,true);
                };
        }
        rank(par,pl,s);
        ch->texto(pant,s,ox+((SX-(strlen(s)*13))/2),oy+210,BAMA1+2,true);
        str_hora(partida[par].tiempo+b->tiempo_batalla(),a);
        sprintf(s,"Total time:%s",a);
        ch->texto(pant,s,ox+((SX-(strlen(s)*13))/2),oy+230,BMAR1,true);
        sprintf(s,"Game percent:%d%c",partida[par].porcent,'%');
        ch->texto(pant,s,ox+((SX-(strlen(s)*13))/2),oy+250,BMAR1,true);

        update_display_and_flip(pant);

        al_rest(0.5f);
        esperar_evento();
}

void how_to_play(charset *ch, ALLEGRO_BITMAP *pant, batalla *b, int par)
{
       int SX=640, SY=400;
       int ox=int((al_get_bitmap_width(pant)-SX)/2), oy=int((al_get_bitmap_height(pant)-SY)/2),i,j;
       char s[60];
       char descrip[6][8][20]={
         {"None","None","None","None","None","None","None","None"},
         {"Cur.Up","Cur.Down","Cur.Left","Cur.Right","R.Control","Alt Gr","R.Shift","Space"},
         {"W","S","A","D","T","Y","U","I"},
         {"Joy1 Up","Joy1 Down","Joy1 Left","Joy1 Right","Joy1 Button 1","Joy1 Button 2","Joy1 Button 3","Joy1 Button 4"},
         {"Joy2 Up","Joy2 Down","Joy2 Left","Joy2 Right","Joy2 Button 1","Joy2 Button 2","Joy2 Button 3","Joy2 Button 4"},
         {"Mouse Up+Ri","Mouse Do+Le","Mouse Up+Le","Mouse Do+Ri","Mou.Button 1","None","Mou.Button 3","Mou.Button 2"}};

       for(i=0; i<b->num_players(); i++){

		al_set_target_bitmap(pant);
        ch->ventana(pant,ox,oy,ox+SX,oy+SY,0,0,FALSE);
        sprintf(s,"GAME CONTROLS - Player %d:%s",i+1,partida[par].pl[i].nombre);
        ch->texto(pant,s,ox+((SX-(strlen(s)*13))/2),oy+30,BAMA1+2,true);

        if((i==0) && (partida[par].pl[i].clase!=OMEGA))
        ch->texto(pant,"Weapons   :1...9,0,-",ox+20,oy+75,BMAR1,true);
        else
        ch->texto(pant,"[No weapon hotkeys]",ox+20,oy+75,BMAR1,true);
        ch->texto(pant,"Map On/Of :Tab",ox+20,oy+115,BMAR1,true);
        ch->texto(pant,"Move Up   :",ox+20,oy+155,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][0],ox+160,oy+155,BMAR1,true);
        ch->texto(pant,"Move Down :",ox+20,oy+195,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][1],ox+160,oy+195,BMAR1,true);
        ch->texto(pant,"Move Left :",ox+20,oy+235,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][2],ox+160,oy+235,BMAR1,true);
        ch->texto(pant,"Move Right:",ox+20,oy+275,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][3],ox+160,oy+275,BMAR1,true);

        ch->texto(pant,"Console   :F1",ox+320,oy+75,BMAR1,true);
        ch->texto(pant,"Status    :F2",ox+320,oy+115,BMAR1,true);
        ch->texto(pant,"Attack    :",ox+320,oy+155,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][4],ox+460,oy+155,BMAR1,true);
        ch->texto(pant,"Strafee   :",ox+320,oy+195,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][5],ox+460,oy+195,BMAR1,true);
        switch(partida[par].pl[i].clase){
                case SIGMA : sprintf(s,"Next Weap.:"); break;
                case OMEGA : sprintf(s,"Defense   :"); break;
                case BETA  : sprintf(s,"Next Spell:"); break;
        }
        ch->texto(pant,s,ox+320,oy+235,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][6],ox+460,oy+235,BMAR1,true);
        ch->texto(pant,"Action    :",ox+320,oy+275,BMAR1,true);
        ch->texto(pant,descrip[cont_op[i]][7],ox+460,oy+275,BMAR1,true);

        ch->texto(pant,"Press F3 to display this screen in the game",ox+35,oy+330,15,true);
        ch->texto(pant,"Press any button to continue",ox+115,oy+360,15,true);

        update_display_and_flip(pant);

		al_rest(0.5f);
        esperar_evento();
        //delay(500);

        };

}
void muestra_fps(ALLEGRO_BITMAP *pant, charset *ch)
{
	char s[20];
	if(show_fps)
	{
		sprintf(s,"FPS:%.02f",fps);
		ch->texto(pant,s,al_get_bitmap_width(pant)-130,0,BBLAN);
	}
}
void nivel_central(charset *ch, ALLEGRO_BITMAP *pant, int par, int &epi, int &lev)
{
         ALLEGRO_BITMAP *pic;
		 ALLEGRO_BITMAP *pant2;
         waves *so;
         charset *ch2;
         batalla *b;
         //int SCRX[5]={320,360,400,640,800},SCRY[5]={200,240,300,400,600},
             //SCRDRV[5]={GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT},
		 int i,j,maxops;
         //RGB *pal;
         player_info *pl;
         char s[60];
         bool salir=FALSE;
         warrior *w,*sec;
         mapa *ma;
         char award[5][10]={" ","¶","·","¸","¹"};
         area to_epi[4]={area(15,15,35,24),area(100,16,117,24),area(50,15,69,24),area(150,15,192,24)},
              train_fin(124,52,129,58);

         //pal=new RGB[256];

		 //pant2 = pant;
         so=DBG_NEW waves(GDIR);
         so->set_activo(sound_op);
         b=DBG_NEW batalla(GDIR,"central.niv",so,COOP,WINDOW_RESOLUTIONS[res_op][0],split_op);
         ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);
         controlm cont;

         // Ya armaduras sagradas
         if(partida[par].sacred==1){
                b->dev_nivel()->cambia_fspr("ssigma",0);
                b->dev_nivel()->cambia_fspr("somega",1);
                b->dev_nivel()->cambia_fspr("sbeta",2);};

         //Convierte los secundarios en inmortales
         i=0;
         while((w=b->dev_enem(i))!=NULL){
                w->set_modo_dios();
                i++;
         };
         // Ingresa los jugadores
         for(i=0; i<partida[par].n_players; i++){
            pl=&partida[par].pl[i];
            b->ingresa_player(pl->nombre,pl->spr,pl->clase,pl->c1,pl->c2,cont_op[i],pl->level,pl->exp);
         };
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            w->nueva_salud(partida[par].pl[i].salud);
            for(j=0; j<10; j++){
               if(partida[par].pl[i].armas[j]==TRUE) w->nueva_arma(j+1);
               w->nuevo_escudo(partida[par].pl[i].escudo);
            };
            for(j=0; j<3; j++)
               w->nueva_municion(j,partida[par].pl[i].ammo[j]);
         };
         if(music_op) b->activa_musica();
         ch2->carga_sprites(b);
         if(console_op) b->activa_input_consola();
         b->elimina_items_inutiles();
         b->desvela_mapa();

         //Secundarios paseando rand
         for(i=0; i<=4; i++)
         if(rand()%4==0) b->dev_enem(i)->set_sclase(CATENT);

         //A jugar!
         if(partida[par].n_players==1)
         switch(partida[par].pl[0].clase){
            case SIGMA : j=3; break;
            case BETA  : j=2; break;
            case OMEGA : j=1; break;}
         else j=4;
         sprintf(s,"%s/imagen%d.%s",GDIR,j,FILE_IMG_EXT);
         pic=cargar_bitmap(s);

		 al_set_target_bitmap(pant);
         //blit(pic,pant,0,0,0,0,640,400);
		 al_draw_bitmap(pic,0,0,0);
         destruir_bitmap(pic);
         ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
         ch->texto(pant,b->dev_nivel()->nombre(),(640-(strlen(b->dev_nivel()->nombre())*13))/2,220,BAMA1+2,true);
         ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
         update_display_and_flip(pant);
         ch2->carga_bitmaps_batalla(b);
         ch2->crea_tablas_color(b->dev_nivel());
		 al_set_target_bitmap(pant);
         ch->texto(pant,"Press any button to start",155,360,BAMA1+2,true);
         update_display_and_flip(pant);


         esperar_evento();

    //     if(res_op!=3)
			 ////set_gfx_mode(SCRDRV[res_op], SCRX[res_op], SCRY[res_op], 0, 0);
			 //al_resize_display(game_display,WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);
         //ch->carga_bitmaps_batalla(b);
		 pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

         //clear_to_color(pant2,0);
		 if(partida[par].porcent==0) how_to_play(ch,pant2,b,par);

		 inicia_tick();
         speed_counter=1;
         do{

			___START_PROFILER()
			___ENTER_PROFILER_BLOCK("TICK")

			while(speed_counter > 0)
			{

				___ENTER_PROFILER_BLOCK("Update")
				___ENTER_PROFILER_BLOCK("batalla::actualiza")
                b->actualiza(partida[par].dif);
                speed_counter--;
				___LEAVE_PROFILER_BLOCK("batalla::actualiza")

                //INTERACCION JUGADORES CON NIVEL CENTRAL
                ma=b->dev_mapa();
                for(i=0; i<b->num_players(); i++)
				{

                    w=b->dev_player(i);

                    // Salida hacia los episodios
                    for(j=0; j<4; j++)
                    if(to_epi[j].es_dentro(w->x_pos(),w->y_pos()))
                            {salir=TRUE; epi=j;};

                    // Puertas que se deben abrir o cerrar
                    // Puerta al training o a la zona de secundarios
                    /*if(partida[par].train==0)
                            ma->dev_puerta(5)->abre();
                    else    ma->dev_puerta(6)->abre();*/
					ma->dev_puerta(5)->abre();
					ma->dev_puerta(6)->abre();

                    // Completar el entrenamiento
                    if(train_fin.es_dentro(w->x_pos(),w->y_pos())){
                            partida[par].train=1; calcula_porcent(par);
					};

                    // Puerta al infierno
                    if(partida[par].sacred==1)
                            ma->dev_puerta(7)->abre();

                    // KTI superado
                    if(partida[par].kti==1)
					    b->dev_enem(9)->cambia_frase("Oh,you're awesome! I suppose that I need more practice...");

                    //Jugar KTI
                    cont.actualiza(cont_op[i]);
                    if(((bald)w->front_x(1.0)==40) && ((bald)w->front_y(1.0)==123) && (cont.boton(3)))
                    if(menu_si_no(pant2,ch2,"Play the KTI arcade?")==0)
					{
                        so->stop_bgm();
                        jugar_kti(pant2,ch2,par,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);
                        so->play_bgm();
                    }

                    //Interacci¢n secundarios
                    if((j=b->hay_enem((bald)w->front_x(1.0),(bald)w->front_y(1.0),0))>=0)
					{
                          sec=b->dev_enem(j);
                          switch(j)
						  {
                                case 0 ://Waraihan
                                if(partida[par].train==0) sec->cambia_frase("Welcome to my tower,young hero.First of all,you must complete your training.Go to your right.");
                                else if(partida[par].sacred==0)
								{
                                    switch(rand()%12)
									{
                                        case 0 : sec->cambia_frase("There are vellums that contain spells,and you can use them even thought you're not a sorcerer!"); break;
                                        case 1 : sec->cambia_frase("If you pick up a full zone map,you will be able to see the complete area map."); break;
                                        case 2 : sec->cambia_frase("When I was young,I was a paladin,but this battle's too much for my old body!"); break;
                                        case 3 : sec->cambia_frase("The legendary Sigma Soldier won the first battle,noe it's time for us to complete the work!"); break;
                                        case 4 : sec->cambia_frase("The Invaders are clever and skillfull,they master any war technique they see!"); break;
                                        case 5 : sec->cambia_frase("In each area,you'll find a medium boss.They usually protect an important item!"); break;
                                        case 6 : sec->cambia_frase("Wherever you fight,please don't kill any innocent being,or you'll be punished!"); break;
                                        case 7 : sec->cambia_frase("In order to recover the trhee gems,you'll have to defeat three great bosses."); break;
                                        case 8 : sec->cambia_frase("The Ork King is hidding in the castle.He's a coward,and protects itself with dozens of orcs!"); break;
                                        case 9 : sec->cambia_frase("Unmask the false Koppalha,in order to free the ancient Temple of the Sun."); break;
                                        case 10 : sec->cambia_frase("It seems that the one who took Silver Moon is a half-cyborg creature."); break;
                                        case 11 : sec->cambia_frase("We don't know much about the organic fortress of the Invaders..."); break;
                                    }
                                }
                                else if(partida[par].completo==0) sec->cambia_frase("Well,wearing this sacred cloth you must not fear the power of evil.");
                                else if(partida[par].porcent<100) sec->cambia_frase("Well,you think you finished your quest? Find more! There are lots of secrets!");
                                else sec->cambia_frase("Oh, you're really a legendary hero! I'm so glad of being your partner...");
                                break;

                                case 1 ://Artillero
                                if(w->clase()!=SIGMA) sec->cambia_frase("Hey, you're not a Sigma Soldier! I can do nothing for you.");
                                else if(w->arma()==0) sec->cambia_frase("Are you going to beat those foes with your bare hands?! You're amazing!!!");
                                else if(w->arma()==1) {sec->cambia_frase("This pistol uses bullets of 9mm. It's fast and handy, but only useful with weak targets.");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(BALA);}
                                else if(w->arma()==2) {sec->cambia_frase("Ah,a chaingun! It shots three bullets of 9mm simultaneously,and can do pretty damage!");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(BALA);}
                                else if(w->arma()==3) {sec->cambia_frase("This rocket launcher uses these explosive rockets.The rockets are self-propelled.");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(COHE);}
                                else if(w->arma()==4) {sec->cambia_frase("The Annihilator! Each shot throws two rockets.Very powerfull,but be carefull with the explosion!");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(COHE);}
                                else if(w->arma()==5) {sec->cambia_frase("Haha,the  \"Dragon\".With a load of napalm,throws a huge fireball.When exploding,creates a flame.");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(NAPA);}
                                else if(w->arma()==6) {sec->cambia_frase("Hey boy,you found a \"Inferno\"! Even thought it wastes lots of napalm,its flame is awesome...");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(NAPA);}
                                else if(w->arma()==7) {sec->cambia_frase("What we have here? Uh,this is a incredible weapon! But it needs both the three kinds of ammo.");
                                                      if(sec->item_asociado()!=NING) sec->nuevo_item_asociado(NAPA);}
                                break;

                                case 2 ://Bruja
                                if(w->clase()!=BETA) sec->cambia_frase("Sorry, but I only can help the Beta Sorcerers.");
                                else{
									switch(rand()%12)
									{
                                     case 0 : sec->cambia_frase("If you're out of energy,just use the staff to hit the enemies.It's better than stay quiet!"); break;
                                     case 1 : sec->cambia_frase("Use the fireball wisely. Wherever it explodes,it creates a flame that burns for a while."); break;
                                     case 2 : sec->cambia_frase("The repulsion aura is a good method to avoid a fight when you're in trouble!"); break;
                                     case 3 : sec->cambia_frase("The fire wall is a terrible spell.Not only burns,when casting it causes a great damage."); break;
                                     case 4 : sec->cambia_frase("Freeze your enemies with the ice ball,then attack them with another method."); break;
                                     case 5 : sec->cambia_frase("The invulnerability spell acts like a shield,but lasts less time."); break;
                                     case 6 : sec->cambia_frase("By creating an ice barrier,you can block a doorway and secure your escape!"); break;
                                     case 7 : sec->cambia_frase("The lighting attacks decreases your opponent's health even after of the impact."); break;
                                     case 8 : sec->cambia_frase("Being temporally invisible is very useful to enter a room without being discovered."); break;
                                     case 9 : sec->cambia_frase("The bolt spell affects every enemy inside your action radius."); break;
                                     case 10: sec->cambia_frase("I've heard about the terrible apocalypse spell,but we'll need a book to learn how to cast it."); break;
									};

									if(sec->item_asociado()!=NING)
                                      switch(rand()%3){
										case 0 : sec->nuevo_item_asociado(MFUE); break;
										case 1 : sec->nuevo_item_asociado(MHIE); break;
										case 2 : sec->nuevo_item_asociado(MELE); break;
                                      }
                                }
                                break;

                                case 3 : //Herrero
                                if(w->clase()!=OMEGA) sec->cambia_frase("You're wrong,man. I'm here to attend Omega Warriors!");
                                else if(rand()%2==0){
                                      switch(w->arma()){
                                      case 0 : sec->cambia_frase("Yes,I know,you're strong.But...better find a good sword to slain those foes!"); break;
                                      case 1 : sec->cambia_frase("Ah,a steel dagger! Small and swift.It's a good start,but...nothing else."); break;
                                      case 2 : sec->cambia_frase("Yes,a falchion. It's curved edge makes it fast and effective.Use it well."); break;
                                      case 3 : sec->cambia_frase("So,you're getting better,are'nt? This great sword seems to be pretty useful."); break;
                                      case 4 : sec->cambia_frase("I like this one! You should break some Invader's heads with this! But watch over yours!!!"); break;
                                      case 5 : sec->cambia_frase("Wow,this mace looks heavy.But if you can handle it,sure you'll smash the foes easily."); break;
                                      case 6 : sec->cambia_frase("At last,a \"Golden Eagle\"!.Its gold is really outstanding.You're a lucky man,warrior!"); break;
                                      case 7 : sec->cambia_frase("I can't believe,this is the legendary\"Justice Edge\"!.Now you're really deadful..."); break;
                                      }
                                }else{
                                      switch(w->escudo()){
                                      case 0 : sec->cambia_frase("If I where you,I would find a good shield to protect that beatiful skin!"); break;
                                      case 1 : sec->cambia_frase("Well,it's a leather shield.It can protect you from the punches,but no from the steel!"); break;
                                      case 2 : sec->cambia_frase("Ha,a wooden one.Should protect your face from the daggers of your opponents."); break;
                                      case 3 : sec->cambia_frase("Yes,this is a steel tower shield.You can block several sword attacks with this."); break;
                                      case 4 : sec->cambia_frase("You're handling a bronze shield? Well,know that no sword can break this one!"); break;
                                      case 5 : sec->cambia_frase("Hey,this is a silver shield of the knights! Oh,yes,it's a very good protection."); break;
                                      case 6 : sec->cambia_frase("Uh-oh,you got a golden one! Well man,from now you have the best protection I know."); break;
                                      case 7 : sec->cambia_frase("Eh? Let me see this one...The legendary \"Protector\"!.If the legend's true,now you're invincible!"); break;
                                      }
                                };
                                break;

                                case 4 : //M‚dico
                                if(w->salud()<30) sec->cambia_frase("Oh,god! Those wounds are terrible! Afortunately,you came here in time.");
                                else if(w->salud()<60) sec->cambia_frase("Be careful! You're rather injuried.You need to repose.");
                                else if(w->salud()<100) sec->cambia_frase("Don't worry,you're not in danger.I'll cure those scratches.");
                                else switch(rand()%8){
                                     case 0 : sec->cambia_frase("Exists a silver shield that gives you temporal invulnerability.If you find one,use it wisely."); break;
                                     case 1 : sec->cambia_frase("I know about a potion of mystical strenght that triplicates your attack power! Try to find one."); break;
                                     case 2 : sec->cambia_frase("There is a magical ghost skull that make the enemies fear you.Be sure to take it if you find one!"); break;
                                     case 3 : sec->cambia_frase("After drinking an elixir of partial invisibility,the enemies must be very close to you to see you."); break;
                                     case 4 : sec->cambia_frase("The dark fluid of shadows creates a shadow aura around you,and no one can see you then!"); break;
                                     case 5 : sec->cambia_frase("By taking a heart of maximum health,your vitality will be increased to the double of his maximum."); break;
                                     case 6 : sec->cambia_frase("A very strange item,the God's protection.Gives you both strenght,repulsion,inmunity and health!"); break;
                                     case 7 : sec->cambia_frase("Be careful with the poison flasks! They seem to be a potion,but drain your health."); break;
                                     }
                                break;

                                case 26 : //Monje del status
                                cont.actualiza(cont_op[i]);
                                if(((bald)w->front_x(1.0)==sec->x_pos()) && ((bald)w->front_y(1.0)==sec->y_pos()) && (cont.boton(3))){
                                        status(ch2,pant2,b,par,i);
										al_rest(0.5f);
                                        consume_ciclos_transcurridos();};
                                break;
							}
                        };
                }
			___LEAVE_PROFILER_BLOCK("Update")
            };


			___ENTER_PROFILER_BLOCK("Render")
			al_set_target_bitmap(pant2);
			___ENTER_PROFILER_BLOCK("charset::muestra_pantalla")
            ch2->muestra_pantalla(pant2,b);
			___LEAVE_PROFILER_BLOCK("charset::muestra_pantalla")
			muestra_fps(pant2,ch2);
			___ENTER_PROFILER_BLOCK("Flip display")
			update_display_and_flip(pant2);
			___LEAVE_PROFILER_BLOCK("Flip display")
			___LEAVE_PROFILER_BLOCK("Render")

			speed_counter = consume_ciclos_transcurridos();
			//fprintf(stderr,"Elapsed:%g - Llama update %d veces\n",al_get_time() - ultimo_inicio_tick, speed_counter);

			___LEAVE_PROFILER_BLOCK("TICK")

			if(al_key_down(&key_state,ALLEGRO_KEY_F8))
			{
				___SHOW_PROFILER()
			}

			al_get_keyboard_state(&key_state);

            if(al_key_down(&key_state,ALLEGRO_KEY_F3)) {how_to_play(ch,pant2,b,par); consume_ciclos_transcurridos(); speed_counter=1;};
            if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
                         //get_palette(pal);
                         sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
                         al_save_bitmap(s,pant2);
                         b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
      //                   //delay(500);
						 al_rest(0.5f);
						};

            if(al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))
            if(menu_si_no(pant2,ch2,"Quit the game?")==0)
               {salir=TRUE; epi=-1;};

         }while(!salir);

         // Escoge subnivel
         maxops=1;
         if(epi>=0){
			   al_set_target_bitmap(pant2);
               ch->texto(pant2,"Choose one zone",40,40,BAMA1+2);
               for(j=0; j<9; j++) rect(pant2,35,55+(20*j),390,75+20*j,BROJO);
               //vline(pant2,365,75,215,BROJO);
			   rect(pant2,365,75,365,215,BROJO);
               if(partida[par].exitos[epi][0]>0) ch->texto(pant2,"Prologue",40,60,BMAR1);
               for(j=0; j<7; j++){
                       if(partida[par].exitos[epi][j]>0) {ch->texto(pant2,lev_names[epi][j],40,80+(20*j),BMAR1);
                                                          ch->texto(pant2,award[partida[par].exitos[epi][j+1]],370,80+(20*j),BMAR1);
                                                          maxops++;}
                       else                            ch->texto(pant2,"???",40,80+(20*j),BMAR1);
               };
               if(partida[par].exitos[epi][7]>0){
                if(epi!=3) ch->texto(pant2,"Epilogue",40,220,BMAR1);
                else ch->texto(pant2,"Final Battle",40,220,BMAR1);
                maxops++;
               };
               do{
                  lev=elige_opcion(pant2,0,400,50,20,maxops,0);
               }while(lev<0);
         };
         //clear_to_color(screen,0);

         if(music_op) b->desactiva_musica();
         b->stop_ambient_samples();
         partida[par].tiempo+=b->tiempo_batalla();
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            partida[par].pl[i].salud=w->salud();
            partida[par].pl[i].exp=w->exp();
            partida[par].pl[i].level=w->level();
            for(j=1; j<=10; j++){
               if(w->tiene_arma(j)) partida[par].pl[i].armas[j-1]=TRUE;
               partida[par].pl[i].escudo=w->escudo();
            };
            for(j=0; j<3; j++)
               partida[par].pl[i].ammo[j]=w->municion(j);
         };

         // Regresa al modo 640x400
    //     if(res_op!=3)
			 ////set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
			 //al_resize_display(game_display, 640, 400);
         /*set_pallete(pal);*/
         destruir_bitmap(pant2);
         delete ch2;
         //delete[] pal;
         delete so;
         delete b;
}

int juega_nivel_batalla(charset *ch, ALLEGRO_BITMAP *pant, int par, int epi, int lev)
{
         ALLEGRO_BITMAP *pic;
		 ALLEGRO_BITMAP *pant2;
         waves *so;
         charset *ch2;
         batalla *b;
         //int SCRX[5]={320,360,400,640,800},SCRY[5]={200,240,300,400,600},
         //    SCRDRV[5]={/*GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT*/},
		 int result=1,i,j;
         //RGB *pal;
         player_info *pl;
         char s[50], exterior[4][7]={{1,0,0,0,0,0,0},{1,0,0,0,0,0,0},{1,1,0,0,0,0,0},{1,0,0,0,0,0,0}};
         bool salir=FALSE;
         warrior *w;
         char award[5][10]={"Nothing","Finished","Bronze","Silver","Gold"};

         //pal=DBG_NEW RGB[256];

         so=DBG_NEW waves(GDIR);
         so->set_activo(sound_op);
         sprintf(s,"%s%d.niv",epi_files[epi],lev);
         b=DBG_NEW batalla(GDIR,s,so,COOP,WINDOW_RESOLUTIONS[res_op][0],split_op);
         ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

         // Ya armaduras sagradas
         if(partida[par].sacred==1){
                b->dev_nivel()->cambia_fspr("ssigma",0);
                b->dev_nivel()->cambia_fspr("somega",1);
                b->dev_nivel()->cambia_fspr("sbeta",2);};

         // Ingresa los jugadores
         for(i=0; i<partida[par].n_players; i++){
            pl=&partida[par].pl[i];
            b->ingresa_player(pl->nombre,pl->spr,pl->clase,pl->c1,pl->c2,cont_op[i],pl->level,pl->exp);
         };
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            w->nueva_salud(partida[par].pl[i].salud);
            for(j=0; j<10; j++){
               if(partida[par].pl[i].armas[j]==TRUE) w->nueva_arma(j+1);
               w->nuevo_escudo(partida[par].pl[i].escudo);
            };
            for(j=0; j<3; j++)
               w->nueva_municion(j,partida[par].pl[i].ammo[j]);
         };
         if(music_op) b->activa_musica();
         ch2->carga_sprites(b);
         if(console_op) b->activa_input_consola();
         b->elimina_items_inutiles();
         b->ajusta_enemigos(partida[par].dif,lev);
         // Lluvia en niveles exteriores una de cada 4 partidas
         if((exterior[epi][lev-1]==1) && (rand()%4==0)) {
                b->activa_lluvia();
                b->dev_nivel()->cambia_coef_paleta(b->dev_nivel()->coefr()*0.75,
                                                   b->dev_nivel()->coefg()*0.8,
                                                   b->dev_nivel()->coefb());
         };

         //A jugar!
         if(partida[par].n_players==1)
         switch(partida[par].pl[0].clase){
            case SIGMA : j=3; break;
            case BETA  : j=2; break;
            case OMEGA : j=1; break;}
         else j=4;
         sprintf(s,"%s/imagen%d.%s",GDIR,j,FILE_IMG_EXT);
         pic=cargar_bitmap(s);
		  al_set_target_bitmap(pant);
         //blit(pic,pant,0,0,0,0,640,400);
		 al_draw_bitmap(pic,0,0,0);
         destruir_bitmap(pic);
         ch->texto(pant,epi_descr[epi],(640-(strlen(epi_descr[epi])*13))/2,60,BAMA1+2,true);
         ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
         ch->texto(pant,b->dev_nivel()->nombre(),(640-(strlen(b->dev_nivel()->nombre())*13))/2,220,BAMA1+2,true);
         ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
         update_display_and_flip(pant);
         ch2->carga_bitmaps_batalla(b);
         ch2->crea_tablas_color(b->dev_nivel());
		 al_set_target_bitmap(pant);
         ch->texto(pant,"Press any button to start",155,360,BAMA1+2,true);
         update_display_and_flip(pant);

         esperar_evento();

    //     if(res_op!=3)
			 ////set_gfx_mode(SCRDRV[res_op], SCRX[res_op], SCRY[res_op], 0, 0);
			 //al_resize_display(game_display, WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1] );

		 pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);
         //ch->carga_bitmaps_batalla(b);

		 inicia_tick();
         speed_counter=1;
         do{

			___START_PROFILER()
			___ENTER_PROFILER_BLOCK("TICK")

			___ENTER_PROFILER_BLOCK("Update")
            while(speed_counter > 0){

				___ENTER_PROFILER_BLOCK("batalla::actualiza")
                b->actualiza(partida[par].dif);
				___LEAVE_PROFILER_BLOCK("batalla::actualiza")
                speed_counter--;
            };
			___LEAVE_PROFILER_BLOCK("Update")

			___ENTER_PROFILER_BLOCK("Render")
			___ENTER_PROFILER_BLOCK("charset::muestra_pantalla")
            ch2->muestra_pantalla(pant2,b);
			___LEAVE_PROFILER_BLOCK("charset::muestra_pantalla")
			muestra_fps(pant2,ch2);
			___ENTER_PROFILER_BLOCK("Flip display")
			update_display_and_flip(pant2);
			___LEAVE_PROFILER_BLOCK("Flip display")
			___LEAVE_PROFILER_BLOCK("Render")

			al_get_keyboard_state(&key_state);

			if(al_key_down(&key_state,ALLEGRO_KEY_F8))
			{
				___SHOW_PROFILER()
			}

			if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
                         //get_palette(pal);
                         sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
                         al_save_bitmap(s,pant2);
                         b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
      //                   //delay(500);
						 al_rest(0.5f);
						};
            if(al_key_down(&key_state,ALLEGRO_KEY_F3)) {how_to_play(ch,pant2,b,par); speed_counter=0;};

            if(al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))
            if(menu_si_no(pant2,ch2,"Back to Wairaihan's tower?")==0)
               salir=TRUE;
            // Completar nivel si motherbrain muerto
            if((epi==3) && (lev==7) && (b->dev_enem(44)->estado()>=DIE))
                        for(i=0; i<b->num_players(); i++)
                                 if(b->dev_player(i)->estado()!=OUTL)
                                 b->dev_player(i)->nuevo_estado(OUTL);

            if(b->coop_nivel_completado()) salir=TRUE;

			speed_counter = consume_ciclos_transcurridos();

         }while(!salir);

         //clear_to_color(screen,0);
         if(music_op) b->desactiva_musica();
         b->stop_ambient_samples();
         partida[par].tiempo+=b->tiempo_batalla();
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            partida[par].pl[i].salud=w->salud();
            partida[par].pl[i].exp=w->exp();
            partida[par].pl[i].level=w->level();
            for(j=1; j<=10; j++){
               if(w->tiene_arma(j)) partida[par].pl[i].armas[j-1]=TRUE;
               partida[par].pl[i].escudo=w->escudo();
            };
            for(j=0; j<3; j++)
               partida[par].pl[i].ammo[j]=w->municion(j);
         };
         // Regresa al modo 640x400
    //     if(res_op!=3)
			 ////set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
			 //al_resize_display(game_display, 640, 400);
         /*set_pallete(pal);*/
         destruir_bitmap(pant2);
         delete ch2;
         //delete[] pal;
         delete so;
         if(b->coop_nivel_completado()){
            //clear_to_color(pant,0);
			 al_set_target_bitmap(pant);
			 al_clear_to_color(al_map_rgb(0,0,0));
            ch->texto(pant,b->dev_nivel()->nombre(),50,40,BMAR1);
            ch->texto(pant,award[partida[par].exitos[epi][lev]],500,40,BMAR1);
            ch->texto(pant,"Results",50,100,BMAR1);
            rectfill(pant,50,120,590,121,BROJO);
            sprintf(s,"Elapsed time               %d:%02d",b->tiempo_batalla()/60,b->tiempo_batalla()%60);
            ch->texto(pant,s,50,140,BMAR1);
            sprintf(s,"Enemies killed             %d/%d",b->dev_nivel()->num_enemigos_muertos(),b->dev_nivel()->num_enemigos());
            ch->texto(pant,s,50,170,BMAR1);
            if(b->dev_nivel()->num_enemigos_muertos()==b->dev_nivel()->num_enemigos()){
               ch->texto(pant,"OK",520,170,BVER1); result++;
            }else ch->texto(pant,"MISS",520,170,BROJO);
            sprintf(s,"Allieds alive              %d/%d",b->dev_nivel()->num_aliados()-b->dev_nivel()->num_aliados_muertos(),b->dev_nivel()->num_aliados());
            ch->texto(pant,s,50,200,BMAR1);
            if(b->dev_nivel()->num_aliados_muertos()==0){
               ch->texto(pant,"OK",520,200,BVER1); result++;
            }else ch->texto(pant,"MISS",520,200,BROJO);
            sprintf(s,"Secrets found              %d/%d",b->dev_nivel()->num_total_secrets()-b->dev_nivel()->num_secrets(),b->dev_nivel()->num_total_secrets());
            ch->texto(pant,s,50,230,BMAR1);
            if(b->dev_nivel()->num_secrets()==0){
               ch->texto(pant,"OK",520,230,BVER1); result++;
            }else ch->texto(pant,"MISS",520,230,BROJO);
            rectfill(pant,50,250,590,251,BROJO);
            sprintf(s,"You got the %s Award in this level!",award[result]);
            if(result>1) ch->texto(pant,s,(640-(strlen(s)*13))/2,280,BMAR1);
            if(partida[par].exitos[epi][lev]<result) partida[par].exitos[epi][lev]=result;
            calcula_porcent(par);
            sprintf(s,"Total game percent : %d%c",partida[par].porcent,'%');
            ch->texto(pant,s,(640-(strlen(s)*13))/2,310,BMAR1);
            ch->texto(pant,"Press any button to continue",155,360,BMAR1);
           update_display_and_flip(pant);


            //delay(1000);
            esperar_evento();

            delete b;
   	    guarda_config();
            return 1;
         }else {delete b; return 0;};
}

int juega_nivel_final(charset *ch, ALLEGRO_BITMAP *pant, int par)
{
         ALLEGRO_BITMAP *pant2,*pic;
         waves *so;
         charset *ch2;
         batalla *b;
         //int SCRX[5]={320,360,400,640,800},SCRY[5]={200,240,300,400,600},
		 int i,j,tim=3*60*60;
         //RGB *pal;
         player_info *pl;
         char s[50];
         bool salir=FALSE;
         warrior *w;

         //pal=DBG_NEW RGB[256];

         so=DBG_NEW waves(GDIR);
         so->set_activo(sound_op);
         sprintf(s,"infier8.niv");
         b=DBG_NEW batalla(GDIR,s,so,COOP,WINDOW_RESOLUTIONS[res_op][0],split_op);
         ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

         // Ya armaduras sagradas
         if(partida[par].sacred==1){
                b->dev_nivel()->cambia_fspr("ssigma",0);
                b->dev_nivel()->cambia_fspr("somega",1);
                b->dev_nivel()->cambia_fspr("sbeta",2);};

         // Ingresa los jugadores
         for(i=0; i<partida[par].n_players; i++){
            pl=&partida[par].pl[i];
            b->ingresa_player(pl->nombre,pl->spr,pl->clase,pl->c1,pl->c2,cont_op[i],pl->level,pl->exp);
         };
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            w->nueva_salud(partida[par].pl[i].salud);
            for(j=0; j<10; j++){
               if(partida[par].pl[i].armas[j]==TRUE) w->nueva_arma(j+1);
               w->nuevo_escudo(partida[par].pl[i].escudo);
            };
            for(j=0; j<3; j++)
               w->nueva_municion(j,partida[par].pl[i].ammo[j]);
         };
         if(music_op) b->activa_musica();
         ch2->carga_sprites(b);
         if(console_op) b->activa_input_consola();
         b->elimina_items_inutiles();
         b->ajusta_enemigos(partida[par].dif,12);

         //A jugar!
         if(partida[par].n_players==1)
         switch(partida[par].pl[0].clase){
            case SIGMA : j=3; break;
            case BETA  : j=2; break;
            case OMEGA : j=1; break;}
         else j=4;
         sprintf(s,"%s/imagen%d.%s",GDIR,j,FILE_IMG_EXT);
         pic=cargar_bitmap(s);
         //blit(pic,pant,0,0,0,0,640,400);
		 al_set_target_bitmap(pant);
		 al_draw_bitmap(pic,0,0,0);
         destruir_bitmap(pic);
         ch->texto(pant,epi_descr[3],(640-(strlen(epi_descr[3])*13))/2,60,BAMA1+2,true);
         ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
         ch->texto(pant,b->dev_nivel()->nombre(),(640-(strlen(b->dev_nivel()->nombre())*13))/2,220,BAMA1+2);
         ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
         update_display_and_flip(pant);
         ch2->carga_bitmaps_batalla(b);
         ch2->crea_tablas_color(b->dev_nivel());
         ch->texto(pant,"Press any button to start",155,360,BAMA1+2,true);
		 update_display_and_flip(pant);
         //blit(pant,screen,0,0,0,0,640,400);

         esperar_evento();

    //     if(res_op!=3)
			 ////set_gfx_mode(GFX_AUTODETECT, SCRX[res_op], SCRY[res_op], 0, 0);
			 //al_resize_display(game_display, WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

		 pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

         b->dev_enem(0)->nueva_arma(3);
         b->dev_enem(0)->nueva_arma(4);
         b->dev_enem(0)->nueva_arma(9);
         b->dev_enem(0)->nueva_arma(10);

		 inicia_tick();
         speed_counter=1;
         do{

			 while(speed_counter > 0){

                tim--;
                b->actualiza(partida[par].dif);
                // Abre la escapada si Ad n est  muerto
                if(b->dev_enem(0)->estado()>=DIE) b->dev_mapa()->dev_puerta(0)->abre();
                // Cambia el hechizo de Ad n
                if(rand()%120==0){
                        b->dev_enem(0)->cambia_arma();
                };
                if(b->dev_enem(0)->arma()==0) b->dev_enem(0)->cambia_arma();
                // Pone items de recarga si no los hay
                if(tim%800==0){
                        b->ingresa_item_util(35,31,BALA);
                        b->ingresa_item_util(35,37,COHE);
                        b->ingresa_item_util(35,41,NAPA);
                        b->ingresa_item_util(45,31,MFUE);
                        b->ingresa_item_util(43,37,MHIE);
                        b->ingresa_item_util(43,41,MELE);
                        b->ingresa_item_util(40,22,BOTI);
                }
                speed_counter--;
            };
            ch2->muestra_pantalla(pant2,b);
            // Reloj
            sprintf(s,"%02d:%02d",int(tim/3600),int((tim%3600)/60));
            ch->texto(pant2,s,WINDOW_RESOLUTIONS[res_op][0]-10-(13*strlen(s)),10,15);
			update_display_and_flip(pant2);

			if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
                         //get_palette(pal);
                         sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
                         al_save_bitmap(s,pant2);
                         b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
      //                   //delay(500);
						 al_rest(0.5f);
						};

            if(al_key_down(&key_state,ALLEGRO_KEY_ESCAPE))
            if(menu_si_no(pant,ch2,"This is the final battle! Are you sure?")==0)
               salir=TRUE;
            if(b->coop_nivel_completado()) salir=TRUE;
            if(tim<=30) salir=TRUE;

			speed_counter = consume_ciclos_transcurridos();

         }while(!salir);

         if(music_op) b->desactiva_musica();
         b->stop_ambient_samples();
         partida[par].tiempo+=b->tiempo_batalla();
         for(i=0; i<partida[par].n_players; i++){
            w=b->dev_player(i);
            partida[par].pl[i].salud=w->salud();
            partida[par].pl[i].exp=w->exp();
            partida[par].pl[i].level=w->level();
            for(j=1; j<=10; j++){
               if(w->tiene_arma(j)) partida[par].pl[i].armas[j-1]=TRUE;
               partida[par].pl[i].escudo=w->escudo();
            };
            for(j=0; j<3; j++)
               partida[par].pl[i].ammo[j]=w->municion(j);
         };

         // Regresa al modo 640x400
    //     if(res_op!=3)
			 ////set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
			 //al_resize_display(game_display, 640, 400);
         //set_pallete(pal);
         destruir_bitmap(pant2);
         delete ch2;
         //delete pal;
         delete so;
         if(b->coop_nivel_completado()) {delete b; return 1;}
         else                           {delete b; return 0;};
}

void juega_partida(charset *ch, ALLEGRO_BITMAP *pant,int par)
{
      int epi=0,lev,res=1;

      muerte_gore=gore_op;


      while(epi!=-1)
      {
         nivel_central(ch,pant,par,epi,lev);
         res=1;
         if(epi!=-1)
         while((lev<9) && (res>0))
         {
            // Jugar un nivel
            if((lev>0) && (lev<8))

               res=juega_nivel_batalla(ch,pant,par,epi,lev);
            // Escena de Armadura Sagrada
            if((res>0) && (partida[par].exitos[0][7]>0) &&
                             (partida[par].exitos[1][7]>0) &&
                             (partida[par].exitos[2][7]>0) &&
                             (partida[par].sacred==0))
            {

                partida[par].sacred=1;
                hell_op=1;
                calcula_porcent(par);
                prologo(pant,ch,0,2);
            }
            // Pr¢logo
            else if (lev==0)
            {
               prologo(pant,ch,epi,0);
               res=1;
            // pilogo
            }
            else if(lev==8)
            {
               prologo(pant,ch,epi,1);
               res=1;
               // Nivel final contra Ad n
               if(epi==3)
               { 
                   res=juega_nivel_final(ch,pant,par);
                   // Final del juego
                   if(res>0) escena_final(pant,ch,par);
                };
            };
            lev++;
         };
      };
}
void juego_normal(charset *ch, ALLEGRO_BITMAP *pant, batalla *bt)
{
      int par,i,j,k,gi,pi;
      char *ops[5];
      player_info *pl;

      par=elige_partida(ch,pant,TRUE);

      if(par>=0){
         if(partida[par].en_uso==FALSE){
            // Crear nuevo juego
            for(j=0; j<4; j++)
            for(k=1; k<8; k++)
                    partida[par].exitos[j][k]=0;
            for(j=0; j<4; j++)
                    partida[par].exitos[j][0]=1;
            partida[par].train=1; //OJO
            partida[par].sacred=0; //OJO
            partida[par].kti=0;
            partida[par].completo=0;

            for(j=0; j<4; j++) ops[j]=DBG_NEW char[10];
            sprintf(ops[0],"One");
            sprintf(ops[1],"Two");
            sprintf(ops[2],"Three");
            sprintf(ops[3],"Four");
            i=menu(pant,ch,"Number of players",ops,4,0);
            for(j=0; j<4; j++) delete[] ops[j];

            if(i<0) return;

            ch->destruye_todas_skins();

            if(i>=0){

               partida[par].n_players=i+1;
               // Crea cada jugador
               for(j=0; j<partida[par].n_players; j++){
                  // Elige la clase
				  al_rest(0.5f);
                  i=elige_clase(ch,pant,j+1);
                  pl=&partida[par].pl[j];
                  if(i<0) return;
                  if(i>=0){
                     // Nuevo caracter
                     for(k=0; k<10; k++)
                             pl->armas[k]=FALSE;
                     pl->escudo=0;
                     for(k=0; k<3; k++)
                             pl->ammo[k]=0;
                     pl->level=1; pl->exp=0; pl->salud=100;
                     if(i==0) {pl->clase=OMEGA; pl->spr=1; pl->c1=BNARA; pl->c2=BGRIS; sprintf(pl->nombre,"Omega");};
                     if(i==1) {pl->clase=SIGMA; pl->spr=0; pl->c1=BROJO; pl->c2=BGRIS; sprintf(pl->nombre,"Sigma");};
                     if(i==2) {pl->clase=BETA;  pl->spr=2; pl->c1=BAZUN; pl->c2=BAZUM; sprintf(pl->nombre,"Beta");};
                     // Importar car cter!
                     if(i==3){
                              gi=elige_partida(ch,pant,FALSE);
                              if(gi<0) return;
                              for(k=0; k<4; k++) ops[k]=DBG_NEW char[10];
                              for(k=0; k<partida[gi].n_players; k++)
                                sprintf(ops[k],"%s",partida[gi].pl[k].nombre);
                              pi=menu(pant,ch,"Character to import",ops,partida[gi].n_players,0);
                              for(k=0; k<4; k++) delete[] ops[k];
                              if(pi<0) return;
                              *pl=partida[gi].pl[pi];

                     }
					 else personalizar(ch,pant,bt,pl,false);
                  };
               };

               for(j=0; j<5; j++) ops[j]=DBG_NEW char[10];
               sprintf(ops[0],"Easy");
               sprintf(ops[1],"Normal");
               sprintf(ops[2],"Hard");
               sprintf(ops[3],"Very Hard");
               sprintf(ops[4],"Nightmare");
               i=menu(pant,ch,"Choose a difficulty level",ops,5,0);
               for(j=0; j<5; j++) delete[] ops[j];

               if(i<0) return;
               if(i>=0){
                  partida[par].porcent=0;
                  partida[par].tiempo=0;
                  partida[par].dif=i+1;
                  partida[par].en_uso=TRUE;
               };
            };
         };
         //stop_mod();
		 al_detach_audio_stream(musica);

         juega_partida(ch,pant,par);
      };
}

#ifdef CRUSADE_ONLINE

struct TLobbyEntry
{
    player_info pl;
    char character[8];
    bool remove_staff;
    tbyte lobbyId;
    tbyte playerId;
    ALLEGRO_BITMAP* mPicture;
};

byte sprite_por_clase_heroe(w_clase clase)
{
    switch (clase)
    {
    case SIGMA: return 0;
    case OMEGA: return 1;
    case BETA: return 2;
    default: return 0;
    }
}

ALLEGRO_BITMAP* crea_miniatura(charset* ch, char* personaje, byte c1, byte c2)
{
    char s[60];
    sprintf(s, "%s/%s.%s", GDIR, personaje, FILE_IMG_EXT);
    ALLEGRO_BITMAP* temp = cargar_bitmap(s);
    ALLEGRO_BITMAP* temp2, * imagen;

    temp2 = crear_bitmap(50, 40);
    al_set_target_bitmap(temp2);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    al_convert_mask_to_alpha(temp, al_get_pixel(temp, 0, 0));
    al_draw_bitmap_region(temp, 5, 5, 50, 40, 0, 0, 0);

    destruir_bitmap(temp);

    imagen = ch->crea_copia_bitmap_coloreado(temp2, c1, c2);

    destruir_bitmap(temp2);

    return imagen;
}

ALLEGRO_CONFIG* get_online_config()
{
    ALLEGRO_CONFIG* cfg = al_load_config_file("network.cfg");
    if (cfg == NULL)
    {
        cfg = al_create_config();

        al_add_config_comment(cfg, "", "# IP adress of the server");
        al_set_config_value(cfg, "", "default server", "127.0.0.1");
        al_add_config_comment(cfg, "", "# TCP/IP port used by the game (must be the same on server)");
        al_set_config_value(cfg, "", "port", "1337");
        al_add_config_comment(cfg, "", "# Name of local character");
        al_set_config_value(cfg, "", "nickname", "Player");

        al_save_config_file("network.cfg", cfg);
    }
    return cfg;
}

void save_online_config(ALLEGRO_CONFIG* cfg)
{
    al_save_config_file("network.cfg", cfg);
}
void nivel_central_server(charset* ch, ALLEGRO_BITMAP* pant, int par, int num_players, int& epi, int& lev, player_info *pl_info, CNetworkServer *server, std::vector<TLobbyEntry> *lLobby, CMessageQueue *recQueue, CMessageQueue *sendQueue)
{

    // Load screen
    waves* so;
    batalla* b;
    charset* ch2;
    mapa* ma;
    warrior* w;
    ALLEGRO_BITMAP* pic, * pant2;
    ALLEGRO_KEYBOARD_STATE keyboard;
    int local_player = 0;
    char nombre[8] = "Dummy", s[60];
    bool salir = FALSE;
    area to_epi[4] = { area(15,15,35,24),area(100,16,117,24),area(50,15,69,24),area(150,15,192,24) },
        train_fin(124, 52, 129, 58);

    so = DBG_NEW waves(GDIR);
    so->set_activo(sound_op);
    b = DBG_NEW batalla(GDIR, "central.niv", so, COOP, WINDOW_RESOLUTIONS[res_op][0], split_op);
    b->asigna_jug_local(local_player);
    b->asigna_modo_red(MRSERVER);
    //b->asigna_tiempo_max_batalla(60 * time_op[tim]);
    ch2 = DBG_NEW charset(pant, WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    // Send start info
    for (int j = 1; j < num_players; j++)
    {
        TMsgStartMatch* lMsgStart = DBG_NEW TMsgStartMatch;
        lMsgStart->mAssignedPlayerId = (tbyte)j;
       /* lMsgStart->mStartX = start_pos[j][0];
        lMsgStart->mStartY = start_pos[j][1];*/
        lMsgStart->mType = eMsgStartMatch;
        lMsgStart->mSender = MASK_SERVER;
        lMsgStart->mReceiver = getMaskFromPlayerId(j);
        lMsgStart->mNumPlayers = num_players;
        lMsgStart->mEpisode = 10;
        lMsgStart->mLevel = 0;
        //sprintf(lMsgStart->mArena, "central");
        //lMsgStart->mTimeMax = time_op[tim] * 60;

        for (int i = 0; i < MAXPL; i++)
        {
            lMsgStart->mProfiles[i].mClass = pl_info[i].clase;
            lMsgStart->mProfiles[i].mColor1 = pl_info[i].c1;
            lMsgStart->mProfiles[i].mColor2 = pl_info[i].c2;
            lMsgStart->mProfiles[i].mRemoveStaff = false;// quitar_baston[i];
            memcpy(lMsgStart->mProfiles[i].mName, pl_info[i].nombre, 8);
            //memcpy(lMsgStart->mProfiles[i].mCharacter, pers[i], 8);
        }

        sendQueue->queueMessage(lMsgStart);
    }

    // Ingresa los jugadores
    for (int i = 0; i < num_players; i++) {
        //b->dev_nivel()->cambia_fspr(pers[i], i);
        b->ingresa_player(pl_info[i].nombre, pl_info[i].spr, pl_info[i].clase, pl_info[i].c1, pl_info[i].c2, i == local_player ? cont_op[0] : NOTC, 1, 0);
        /* if (quitar_baston[i])
             b->dev_player(i)->set_baston_mago(false);*/
        if (i != 0)
            b->dev_player(i)->set_activo(false);
    };

    // Posición inicial
    //b->dev_player(0)->nueva_pos((float)start_pos[0][0], (float)start_pos[0][1]);

    //stop_mod();
    al_detach_audio_stream(musica);

    if (music_op) b->activa_musica();
    ch2->carga_sprites(b);
    if (console_op) b->activa_input_consola();
    b->elimina_items_inutiles();
    for (int i = 0; i < NSPR; i++)
        so->load_wav_warrior(GDIR, b->dev_nivel()->fspr(i), i);

    //A jugar!
    al_set_target_bitmap(pant);
    sprintf(s, "%s/imagen4.%s", GDIR, FILE_IMG_EXT);
    pic = cargar_bitmap(s);
    al_draw_bitmap(pic, 0, 0, 0);

    //ch->texto(pant, "Deathmatch battle", 220, 60, BAMA1 + 2, true);
    ch->texto(pant, "Now entering:", 250, 180, BAMA1 + 2, true);
    ch->texto(pant, b->dev_nivel()->nombre(), (640 - (strlen(b->dev_nivel()->nombre()) * 13)) / 2, 220, BAMA1 + 2, true);
    ch->texto(pant, "Please wait while loading level data", 90, 320, BAMA1 + 2, true);
    update_display_and_flip(pant);
    ch2->carga_bitmaps_batalla(b);
    ch2->crea_tablas_color(b->dev_nivel());
    /*for(int i=0; i<num_players; i++)
            so->load_wav_warrior(GDIR,pers[i],i);*/
            //ch->texto(pant,"Press any button to start",155,360,BAMA1,true);
    update_display_and_flip(pant);

    //esperar_evento();

    pant2 = crear_bitmap(WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    inicia_tick();
    speed_counter = 1;
    do {

        server->receiveMessages(recQueue);

        // Remove disconnected people
        for (int i = 0; i < lLobby->size(); i++)
        {
            if (!server->isClientConnected((*lLobby)[i].lobbyId))
            {
                TMsgPlDisconnect* lMsgDisc = DBG_NEW TMsgPlDisconnect;

                lMsgDisc->mType = eMsgPlDisconnect;
                lMsgDisc->mSender = MASK_SERVER;
                lMsgDisc->mReceiver = MASK_SERVER;
                lMsgDisc->mPlayerId = (*lLobby)[i].playerId;

                recQueue->queueMessage(lMsgDisc);

                destruir_bitmap((*lLobby)[i].mPicture);
                lLobby->erase(lLobby->begin() + i);
                i--;
            }
        }

        while (speed_counter > 0) {

            // Interacciones con el nivel central
            w = b->dev_player(0);

            // Salida hacia los episodios
            for (int j = 0; j < 4; j++)
                if (to_epi[j].es_dentro(w->x_pos(), w->y_pos()))
                {
                    salir = TRUE; epi = j;
                };

            ma = b->dev_mapa();

            // Puertas que se deben abrir o cerrar
                   // Puerta al training o a la zona de secundarios
                   /*if(partida[par].train==0)
                           ma->dev_puerta(5)->abre();
                   else    ma->dev_puerta(6)->abre();*/
            //ma->dev_puerta(5)->abre();
            //ma->dev_puerta(6)->abre();

            TMsgOpenDoor* lMsg1 = DBG_NEW TMsgOpenDoor;
            lMsg1->mType = eMsgOpenDoor;
            lMsg1->mDoorId = (tbyte)5;
            lMsg1->mReceiver = MASK_SERVER;
            lMsg1->mSender = MASK_SERVER;
            recQueue->queueMessage(lMsg1);

            TMsgOpenDoor* lMsg2 = DBG_NEW TMsgOpenDoor;
            lMsg2->mType = eMsgOpenDoor;
            lMsg2->mDoorId = (tbyte)6;
            lMsg2->mReceiver = MASK_SERVER;
            lMsg2->mSender = MASK_SERVER;
            recQueue->queueMessage(lMsg2);
            
            b->actualiza_modo_red(recQueue, sendQueue);
            speed_counter--;
        };

        server->sendMessages(sendQueue);

        ch2->muestra_pantalla(pant2, b);
        muestra_fps(pant2, ch2);
        update_display_and_flip(pant2);

        speed_counter = consume_ciclos_transcurridos();

        if (al_key_down(&key_state, ALLEGRO_KEY_F12)) {
            //get_palette(pal);
            sprintf(s, "image%03d.png", sshots); sshots = (sshots + 1) % 1000;
            al_save_bitmap(s, pant2);
            b->encola_mensaje("Wrote screenshot to disk", 15, PRIO1);
        };

        if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE))
            if (menu_si_no(pant2, ch2, "Quit the game?") == 0)
            {
                salir = TRUE; epi = -1;
            };

        while (!tick_terminado());

        al_get_keyboard_state(&keyboard);

    } while (!salir);


    int maxops = 1;
    if (epi >= 0) {
        al_set_target_bitmap(pant2);
        ch->texto(pant2, "Choose one zone", 40, 40, BAMA1 + 2);
        for (int j = 0; j < 9; j++) rect(pant2, 35, 55 + (20 * j), 390, 75 + 20 * j, BROJO);
        //vline(pant2,365,75,215,BROJO);
        rect(pant2, 365, 75, 365, 215, BROJO);
        if (partida[par].exitos[epi][0] > 0) ch->texto(pant2, "Prologue", 40, 60, BMAR1);
        for (int j = 0; j < 7; j++) {
            if (partida[par].exitos[epi][j] > 0) {
                ch->texto(pant2, lev_names[epi][j], 40, 80 + (20 * j), BMAR1);
                //ch->texto(pant2, award[partida[par].exitos[epi][j + 1]], 370, 80 + (20 * j), BMAR1);
                maxops++;
            }
            else                            ch->texto(pant2, "???", 40, 80 + (20 * j), BMAR1);
        };
        if (partida[par].exitos[epi][7] > 0) {
            if (epi != 3) ch->texto(pant2, "Epilogue", 40, 220, BMAR1);
            else ch->texto(pant2, "Final Battle", 40, 220, BMAR1);
            maxops++;
        };
        do {
            lev = elige_opcion(pant2, 0, 400, 50, 20, maxops, 0);
        } while (lev < 0);
    };

    if (music_op) b->desactiva_musica();
    b->stop_ambient_samples();
   /* partida[par].tiempo += b->tiempo_batalla();
    for (i = 0; i < partida[par].n_players; i++) {
        w = b->dev_player(i);
        partida[par].pl[i].salud = w->salud();
        partida[par].pl[i].exp = w->exp();
        partida[par].pl[i].level = w->level();
        for (j = 1; j <= 10; j++) {
            if (w->tiene_arma(j)) partida[par].pl[i].armas[j - 1] = TRUE;
            partida[par].pl[i].escudo = w->escudo();
        };
        for (j = 0; j < 3; j++)
            partida[par].pl[i].ammo[j] = w->municion(j);
    };*/

    // Regresa al modo 640x400
//     if(res_op!=3)
         ////set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
         //al_resize_display(game_display, 640, 400);
     /*set_pallete(pal);*/
    destruir_bitmap(pant2);
    delete ch2;
    //delete[] pal;
    delete so;
    delete b;
}


int nivel_batalla_server(charset* ch, ALLEGRO_BITMAP* pant, int par, int num_players, int& epi, int& lev, player_info* pl_info, CNetworkServer* server, std::vector<TLobbyEntry>* lLobby, CMessageQueue* recQueue, CMessageQueue* sendQueue)
{

    // Load screen
    waves* so;
    batalla* b;
    charset* ch2;
    mapa* ma;
    warrior* w;
    ALLEGRO_BITMAP* pic, * pant2;
    ALLEGRO_KEYBOARD_STATE keyboard;
    int local_player = 0;
    char nombre[8] = "Dummy", s[60];
    bool salir = FALSE;
  

    so = DBG_NEW waves(GDIR);
    so->set_activo(sound_op);
    sprintf(s, "%s%d.niv", epi_files[epi], lev);
    b = DBG_NEW batalla(GDIR, s, so, COOP, WINDOW_RESOLUTIONS[res_op][0], split_op);
    b->asigna_jug_local(local_player);
    b->asigna_modo_red(MRSERVER);
    //b->asigna_tiempo_max_batalla(60 * time_op[tim]);
    ch2 = DBG_NEW charset(pant, WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    // Send start info
    for (int j = 1; j < num_players; j++)
    {
        TMsgStartMatch* lMsgStart = DBG_NEW TMsgStartMatch;
        lMsgStart->mAssignedPlayerId = (tbyte)j;
        /* lMsgStart->mStartX = start_pos[j][0];
         lMsgStart->mStartY = start_pos[j][1];*/
        lMsgStart->mType = eMsgStartMatch;
        lMsgStart->mSender = MASK_SERVER;
        lMsgStart->mReceiver = getMaskFromPlayerId(j);
        lMsgStart->mNumPlayers = num_players;
        //sprintf(lMsgStart->mArena, "central");
        lMsgStart->mEpisode = epi;
        lMsgStart->mLevel = lev;
        //lMsgStart->mTimeMax = time_op[tim] * 60;

        for (int i = 0; i < MAXPL; i++)
        {
            lMsgStart->mProfiles[i].mClass = pl_info[i].clase;
            lMsgStart->mProfiles[i].mColor1 = pl_info[i].c1;
            lMsgStart->mProfiles[i].mColor2 = pl_info[i].c2;
            lMsgStart->mProfiles[i].mRemoveStaff = false;// quitar_baston[i];
            memcpy(lMsgStart->mProfiles[i].mName, pl_info[i].nombre, 8);
            //memcpy(lMsgStart->mProfiles[i].mCharacter, pers[i], 8);
        }

        sendQueue->queueMessage(lMsgStart);
    }

    // Ingresa los jugadores
    for (int i = 0; i < num_players; i++) {
        //b->dev_nivel()->cambia_fspr(pers[i], i);
        b->ingresa_player(pl_info[i].nombre, pl_info[i].spr, pl_info[i].clase, pl_info[i].c1, pl_info[i].c2, i == local_player ? cont_op[0] : NOTC, 1, 0);
        /* if (quitar_baston[i])
             b->dev_player(i)->set_baston_mago(false);*/
        if (i != 0)
            b->dev_player(i)->set_activo(false);
    };

    // Posición inicial
    //b->dev_player(0)->nueva_pos((float)start_pos[0][0], (float)start_pos[0][1]);

    //stop_mod();
    al_detach_audio_stream(musica);

    if (music_op) b->activa_musica();
    ch2->carga_sprites(b);
    if (console_op) b->activa_input_consola();
    b->elimina_items_inutiles();
    for (int i = 0; i < NSPR; i++)
        so->load_wav_warrior(GDIR, b->dev_nivel()->fspr(i), i);

    //A jugar!
    al_set_target_bitmap(pant);
    sprintf(s, "%s/imagen4.%s", GDIR, FILE_IMG_EXT);
    pic = cargar_bitmap(s);
    al_draw_bitmap(pic, 0, 0, 0);

    //ch->texto(pant, "Deathmatch battle", 220, 60, BAMA1 + 2, true);
    ch->texto(pant, "Now entering:", 250, 180, BAMA1 + 2, true);
    ch->texto(pant, b->dev_nivel()->nombre(), (640 - (strlen(b->dev_nivel()->nombre()) * 13)) / 2, 220, BAMA1 + 2, true);
    ch->texto(pant, "Please wait while loading level data", 90, 320, BAMA1 + 2, true);
    update_display_and_flip(pant);
    ch2->carga_bitmaps_batalla(b);
    ch2->crea_tablas_color(b->dev_nivel());
    /*for(int i=0; i<num_players; i++)
            so->load_wav_warrior(GDIR,pers[i],i);*/
            //ch->texto(pant,"Press any button to start",155,360,BAMA1,true);
    update_display_and_flip(pant);

    //esperar_evento();

    pant2 = crear_bitmap(WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    inicia_tick();
    speed_counter = 1;
    do {

        server->receiveMessages(recQueue);

        // Remove disconnected people
        for (int i = 0; i < lLobby->size(); i++)
        {
            if (!server->isClientConnected((*lLobby)[i].lobbyId))
            {
                TMsgPlDisconnect* lMsgDisc = DBG_NEW TMsgPlDisconnect;

                lMsgDisc->mType = eMsgPlDisconnect;
                lMsgDisc->mSender = MASK_SERVER;
                lMsgDisc->mReceiver = MASK_SERVER;
                lMsgDisc->mPlayerId = (*lLobby)[i].playerId;

                recQueue->queueMessage(lMsgDisc);

                destruir_bitmap((*lLobby)[i].mPicture);
                lLobby->erase(lLobby->begin() + i);
                i--;
            }
        }

        while (speed_counter > 0) {

            b->actualiza_modo_red(recQueue, sendQueue);
            speed_counter--;
        };

        server->sendMessages(sendQueue);

        ch2->muestra_pantalla(pant2, b);
        muestra_fps(pant2, ch2);
        update_display_and_flip(pant2);

        speed_counter = consume_ciclos_transcurridos();

        if (al_key_down(&key_state, ALLEGRO_KEY_F12)) {
            //get_palette(pal);
            sprintf(s, "image%03d.png", sshots); sshots = (sshots + 1) % 1000;
            al_save_bitmap(s, pant2);
            b->encola_mensaje("Wrote screenshot to disk", 15, PRIO1);
        };

        if (al_key_down(&key_state, ALLEGRO_KEY_ESCAPE))
            if (menu_si_no(pant2, ch2, "Back to Wairaihan's tower?") == 0)
                salir = TRUE;

        if (b->coop_nivel_completado()) salir = TRUE;

        while (!tick_terminado());

        al_get_keyboard_state(&keyboard);

    } while (!salir);

    if (music_op) b->desactiva_musica();
    b->stop_ambient_samples();
    /* partida[par].tiempo += b->tiempo_batalla();
     for (i = 0; i < partida[par].n_players; i++) {
         w = b->dev_player(i);
         partida[par].pl[i].salud = w->salud();
         partida[par].pl[i].exp = w->exp();
         partida[par].pl[i].level = w->level();
         for (j = 1; j <= 10; j++) {
             if (w->tiene_arma(j)) partida[par].pl[i].armas[j - 1] = TRUE;
             partida[par].pl[i].escudo = w->escudo();
         };
         for (j = 0; j < 3; j++)
             partida[par].pl[i].ammo[j] = w->municion(j);
     };*/

     // Regresa al modo 640x400
 //     if(res_op!=3)
          ////set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
          //al_resize_display(game_display, 640, 400);
      /*set_pallete(pal);*/
    destruir_bitmap(pant2);
    delete ch2;
    //delete[] pal;
    delete so;

    if (b->coop_nivel_completado())
    {
        delete b;
        return 1;

    }
    else
    {
        delete b;
        return 0;
    }
    

}

void nivel_batalla_cliente(charset* ch, ALLEGRO_BITMAP* pant, int num_players, int local_player, int par, int epi, int lev, player_info* pl_info, CNetworkClient* client, CMessageQueue* recQueue, CMessageQueue* sendQueue)
{
    // Start!!
    waves* so;
    batalla* b;
    charset* ch2;
    ALLEGRO_BITMAP* pic, * pant2;
    ALLEGRO_KEYBOARD_STATE keyboard;
    char nombre[8] = "Dummy", s[60];

    so = DBG_NEW waves(GDIR);
    so->set_activo(sound_op);
    if(epi == 10)
        sprintf(s, "central.niv");
    else
        sprintf(s, "%s%d.niv", epi_files[epi], lev);
    b = DBG_NEW batalla(GDIR, s, so, COOP, WINDOW_RESOLUTIONS[res_op][0], split_op);
    b->asigna_jug_local(local_player);
    b->asigna_modo_red(MRCLIENTE);
    //b->asigna_tiempo_max_batalla(tim);
    ch2 = DBG_NEW charset(pant, WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    // Ingresa los jugadores
    for (int i = 0; i < num_players; i++) {
        //b->dev_nivel()->cambia_fspr(pers[i], i);
        b->ingresa_player(pl_info[i].nombre, pl_info[i].spr, pl_info[i].clase, pl_info[i].c1, pl_info[i].c2, i == local_player ? cont_op[0] : NOTC, 1, 0);
        /* if (quitar_baston[i])
             b->dev_player(i)->set_baston_mago(false);*/
        if (i != local_player)
            b->dev_player(i)->set_activo(false);
    };

    // Posición inicial
    //b->dev_player(local_player)->nueva_pos((float)startX, (float)startY);

    //stop_mod();
    al_detach_audio_stream(musica);

    if (music_op) b->activa_musica();
    ch2->carga_sprites(b);
    if (console_op) b->activa_input_consola();
    b->elimina_items_inutiles();
    for (int i = 0; i < NSPR; i++)
        so->load_wav_warrior(GDIR, b->dev_nivel()->fspr(i), i);

    //A jugar!
    al_set_target_bitmap(pant);
    sprintf(s, "%s/imagen4.%s", GDIR, FILE_IMG_EXT);
    pic = cargar_bitmap(s);
    al_draw_bitmap(pic, 0, 0, 0);

    //ch->texto(pant, "Deathmatch battle", 220, 60, BAMA1 + 2, true);
    ch->texto(pant, "Now entering:", 250, 180, BAMA1 + 2, true);
    ch->texto(pant, b->dev_nivel()->nombre(), (640 - (strlen(b->dev_nivel()->nombre()) * 13)) / 2, 220, BAMA1 + 2, true);
    ch->texto(pant, "Please wait while loading level data", 90, 320, BAMA1 + 2, true);
    update_display_and_flip(pant);
    ch2->carga_bitmaps_batalla(b);
    ch2->crea_tablas_color(b->dev_nivel());
    /*for(int i=0; i<num_players; i++)
            so->load_wav_warrior(GDIR,pers[i],i);*/
            //ch->texto(pant,"Press any button to start",155,360,BAMA1,true);
    update_display_and_flip(pant);

    //esperar_evento();

    pant2 = crear_bitmap(WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

    bool lSeguirJugando = true;

    inicia_tick();
    speed_counter = 1;
    do {
        if (!client->isConnected())
        {
            lSeguirJugando = false;
            dialog_accept(pant2, ch, "Connection with server lost", "Accept");
        }

        client->receiveMessages(recQueue);

        while (speed_counter > 0) {

            b->actualiza_modo_red(recQueue, sendQueue);
            speed_counter--;
        };

        client->sendMessages(sendQueue);

        ch2->muestra_pantalla(pant2, b);
        muestra_fps(pant2, ch2);
        update_display_and_flip(pant2);

        speed_counter = consume_ciclos_transcurridos();

        if (al_key_down(&key_state, ALLEGRO_KEY_F12)) {
            //get_palette(pal);
            sprintf(s, "image%03d.png", sshots); sshots = (sshots + 1) % 1000;
            al_save_bitmap(s, pant2);
            b->encola_mensaje("Wrote screenshot to disk", 15, PRIO1);
        };

        while (!tick_terminado());

        al_get_keyboard_state(&keyboard);

        if (b->partida_red_acabada()) lSeguirJugando = false;

    } while ((!al_key_down(&keyboard, ALLEGRO_KEY_ESCAPE) && lSeguirJugando)/* && (b->tiempo_batalla()<60*time_op[tim]) && (b->maxim_frags()<frag_op[frag])*/);

    // Envia mensaje de desconexión SOLO SI SALGO DE LA PARTIDA YO MANUALMENTE!!
    /*TMsgPlDisconnect* lMsgDisc = DBG_NEW TMsgPlDisconnect;

    lMsgDisc->mType = eMsgPlDisconnect;
    lMsgDisc->mSender = getMaskFromPlayerId(local_player);
    lMsgDisc->mReceiver = MASK_SERVER;
    lMsgDisc->mPlayerId = local_player;

    sendQueue->queueMessage(lMsgDisc);*/
    client->sendMessages(sendQueue);


    if (music_op) b->desactiva_musica();

    // Regresa al modo 640x400
   /* if(res_op!=3) al_resize_display(game_display, 640, 400 );*/
    //if(res_op!=3) set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
    //set_pallete(pal);
    destruir_bitmap(pant2);
    delete ch2;
    //delete pal;

    destruir_bitmap(pic);
    delete so;
    delete b;

    //save_online_config(online_cfg);
    //al_destroy_config(online_cfg);
}

void cruzada_online_server(charset* ch, ALLEGRO_BITMAP* pant, batalla* bt)
{
    CNetworkServer server;
    CMessageQueue recQueue;
    CMessageQueue sendQueue;
    ALLEGRO_CONFIG* online_cfg;
    bool lStatus;
    player_info pl[MAXPL];
    char s[60], personajes_heroes[3][8] = { "sigma", "beta", "omega" }, nombre[8] = "Dummy";
    tbyte num_players;


    online_cfg = get_online_config();


    int par = elige_partida(ch, pant, TRUE);

    if (par < 0 || !partida[par].en_uso)
    {
        dialog_accept(pant, ch, "Partida incorrecta", "Accept");
        al_destroy_config(online_cfg);
        return;
    }

    pl[0] = partida[par].pl[0];

    unsigned short conPort = (unsigned short)atoi(al_get_config_value(online_cfg, "", "port"));
    lStatus = server.init(conPort);
    if (!lStatus)
    {
        dialog_accept(pant, ch, "Network error", "Accept");
        al_destroy_config(online_cfg);
        return;
    }

    al_rest(0.5f);

    std::vector<TLobbyEntry> lLobby;
    ALLEGRO_BITMAP* lMyPicture;

    lMyPicture = crea_miniatura(ch, personajes_heroes[partida[par].pl[0].clase], partida[par].pl[0].c1, partida[par].pl[0].c2);

    bool lStayAtLobby = true;
    while (lStayAtLobby)
    {
        inicia_tick();

        server.sendMessages(&sendQueue);

        server.acceptClients();

        server.receiveMessages(&recQueue);

        //Process messages
        TMessage* lMsg;
        while ((lMsg = recQueue.extractFirst()) != 0)
        {
            if (lMsg->mType == eMsgProfile)
            {
                TMsgProfile* lProfMsg = (TMsgProfile*)lMsg;

                TLobbyEntry nuevo;
                nuevo.lobbyId = lProfMsg->mLobbyId;
                sprintf(nuevo.pl.nombre, "%s", lProfMsg->mProfile.mName);
                sprintf(nuevo.character, "%s", lProfMsg->mProfile.mCharacter);
                nuevo.pl.clase = (w_clase)lProfMsg->mProfile.mClass;
                nuevo.pl.c1 = lProfMsg->mProfile.mColor1;
                nuevo.pl.c2 = lProfMsg->mProfile.mColor2;
                nuevo.pl.level = 1; nuevo.pl.exp = 0;
                nuevo.remove_staff = lProfMsg->mProfile.mRemoveStaff;
                nuevo.mPicture = 0;

                lLobby.push_back(nuevo);

                //lStayAtLobby = false;
            }

            delete lMsg;
        }

        // Remove disconnected people
        for (int i = 0; i < lLobby.size(); i++)
        {
            if (!server.isClientConnected(lLobby[i].lobbyId))
            {
                destruir_bitmap(lLobby[i].mPicture);
                lLobby.erase(lLobby.begin() + i);
                i--;
            }
        }

        // Show GUI
        al_set_target_bitmap(pant);
        ch->ventana(pant, 0, 0, 640, 400, BMAR3, BMAR2, FALSE);
        ch->texto(pant, "CRUSADE LOBBY", 180, 20, BAMA1 + 2, true);
        for (int i = 0; i < MAXPL/*lLobby.size()*/; i++)
        {
            int xx = 40 + 280 * (i % 2);
            int yy = 60 + 60 * int(i / 2);
            rect(pant, xx, yy, xx + 270, yy + 55, BBLAN);
            char* lName = 0;
            char* lAdress = 0;
            ALLEGRO_BITMAP* pic = 0;

            if (i == 0)
            {
                lName = (char*)(pl[0].nombre);
                lAdress = server.getServerIPAdress();
                pic = lMyPicture;
            }
            else if (i - 1 < lLobby.size())
            {
                lName = lLobby[i - 1].pl.nombre;
                lAdress = server.getClientIPAdress(lLobby[i - 1].lobbyId);

                if (!lLobby[i - 1].mPicture)
                {
                    lLobby[i - 1].mPicture = crea_miniatura(ch, lLobby[i - 1].character, lLobby[i - 1].pl.c1, lLobby[i - 1].pl.c2);
                }
                pic = lLobby[i - 1].mPicture;


            }
            //sprintf(s,"%s - LobbyId:%d",lLobby[i].pl.nombre, lLobby[i].lobbyId);
            //ch->texto(pant, s, 50, 60 + 40*i, BBLAN, true);
            if (lName)
            {
                al_set_target_bitmap(pant);
                if (pic)
                    al_draw_bitmap(pic, xx + 205, yy + 7, 0);
                ch->texto(pant, lName, xx + 15, yy + 10, BBLAN, true);
                ch->texto(pant, lAdress, xx + 15, yy + 30, BBLAN, true);

            }
            else
            {
                al_draw_filled_rectangle(xx + 1, yy + 1, xx + 269, yy + 55, al_map_rgba(0, 0, 0, 192));
            }
        }
        ch->texto(pant, "Press any button to start game", 150, 350, BAMA1 + 2, true);
        update_display_and_flip(pant);

        if (evento_control(CBU1) || evento_control(CBU2) || evento_control(CBU2) || evento_control(CBU4))
            lStayAtLobby = false;


        while (!tick_terminado());
    };

    server.stopAccepting();

    destruir_bitmap(lMyPicture);

    // Start game!!!

    num_players = lLobby.size() + 1;
    if (num_players > MAXPL) num_players = MAXPL;

    // Assign player mask to final players
    for (int i = 1; i < num_players; i++)
    {
        lLobby[i - 1].playerId = i;
        server.assignPlayerMaskToClient(lLobby[i - 1].lobbyId, getMaskFromPlayerId(i));
    }

    // Register final profiles
    for (int i = 1; i < num_players; i++)
    {
        pl[i] = lLobby[i - 1].pl;
        pl[i].spr = sprite_por_clase_heroe(pl[i].clase);
       /*quitar_baston[i] = lLobby[i - 1].remove_staff;
        sprintf(pers[i], "%s", lLobby[i - 1].character);*/
    }

    server.sendMessages(&sendQueue);

    int epi=0, lev, res = 1;
    while (epi != -1) {

        nivel_central_server(ch, pant, 0, num_players, epi, lev, pl, &server, &lLobby, &recQueue, &sendQueue);
        res = 1;

        if(epi!=-1)
            while ((lev < 9) && (res > 0)) {
                if(lev>0 && lev<8)
                    res = nivel_batalla_server(ch, pant, 0, num_players, epi, lev, pl, &server, &lLobby, &recQueue, &sendQueue);

                lev++;
            }
    }
    
}

TMsgStartMatch* gStartingMessage;

void cruzada_online_cliente(charset* ch, ALLEGRO_BITMAP* pant, batalla* bt)
{
    CNetworkClient client;
    CMessageQueue recQueue;
    CMessageQueue sendQueue;
    ALLEGRO_CONFIG* online_cfg;
    bool lStatus;
    char nombre[8] = "DummyCl", personajes_heroes[3][8] = { "sigma" ,"beta" ,"omega" };
    int num_players, local_player;
    player_info pl[MAXPL];

    online_cfg = get_online_config();


    int par = elige_partida(ch, pant, TRUE);

    if (par < 0 || !partida[par].en_uso)
    {
        dialog_accept(pant, ch, "Partida incorrecta", "Accept");
        al_destroy_config(online_cfg);
        return;
    }


    lStatus = client.init();
    if (!lStatus)
    {
        dialog_accept(pant, ch, "Network error", "Accept");
        al_destroy_config(online_cfg);
        return;
    }

    char lIPAdress[16];
    sprintf(lIPAdress, "%s", al_get_config_value(online_cfg, "", "default server"));
    unsigned short conPort = (unsigned short)atoi(al_get_config_value(online_cfg, "", "port"));

    input_string(ch, pant, "Server IP adress:", lIPAdress, 15);

    al_set_config_value(online_cfg, "", "default server", lIPAdress);

    ventana_texto(pant, ch, "Trying to connect...");
    lStatus = client.connectToServer(lIPAdress, conPort);
    if (!lStatus)
    {
        dialog_accept(pant, ch, "Couldnt connect to server", "Accept");
        al_destroy_config(online_cfg);
        return;
    }


    TMsgProfile* lProf = DBG_NEW TMsgProfile();

    lProf->mType = eMsgProfile;
    lProf->mSender = MASK_NONE;
    lProf->mReceiver = MASK_SERVER;
    sprintf(lProf->mProfile.mName, partida[par].pl[0].nombre);
    sprintf(lProf->mProfile.mCharacter, personajes_heroes[partida[par].pl[0].clase]);
    lProf->mProfile.mClass = partida[par].pl[0].clase;
    lProf->mProfile.mColor1 = partida[par].pl[0].c1;
    lProf->mProfile.mColor2 = partida[par].pl[0].c2;
    lProf->mProfile.mRemoveStaff = false;// quitar_baston[0];

    sendQueue.queueMessage(lProf);

    ventana_texto(pant, ch, "Waiting for other players...");

    // Wait for server signal to start
    bald startX = 0, startY = 0;
    bool lDoStart = false;

    byte lKeepAliveWait = 0;

    TMsgStartMatch *lStartingMessage = 0;

    while (!lDoStart)
    {
        inicia_tick();

        if (lKeepAliveWait == 0)
        {
            lKeepAliveWait = 60;
            TMsgKeepAlive* lMsgAlive = DBG_NEW TMsgKeepAlive;

            lMsgAlive->mType = eMsgKeepAlive;
            lMsgAlive->mSender = 0;
            lMsgAlive->mReceiver = MASK_SERVER;

            sendQueue.queueMessage(lMsgAlive);
        }
        else
            lKeepAliveWait--;


        if (!client.isConnected())
        {
            dialog_accept(pant, ch, "Connection with server lost", "Accept");
            al_destroy_config(online_cfg);
            return;
        }

        client.receiveMessages(&recQueue);

        if (client.isConnected())
            client.sendMessages(&sendQueue);

        TMessage* lMsg;

        while ((lMsg = recQueue.extractFirst()) != 0)
        {
            if (lMsg->mType == eMsgStartMatch)
            {
                gStartingMessage = (TMsgStartMatch*)lMsg;

               //num_players = lStartingMessage->mNumPlayers;
               //local_player = lStartingMessage->mAssignedPlayerId;
               /////* startX = lMsgStart->mStartX;
               //// startY = lMsgStart->mStartY;*/

               //// sprintf(s, "%s.niv", lMsgStart->mArena);

               //// tim = lMsgStart->mTimeMax;

               // for (int i = 0; i < lStartingMessage->mNumPlayers; i++)
               // {
               //     pl[i].clase = (w_clase)lStartingMessage->mProfiles[i].mClass;
               //     memcpy(pl[i].nombre, lStartingMessage->mProfiles[i].mName, 8);
               //     //memcpy(pers[i], lMsgStart->mProfiles[i].mCharacter, 8);
               //     pl[i].c1 = lStartingMessage->mProfiles[i].mColor1;
               //     pl[i].c2 = lStartingMessage->mProfiles[i].mColor2;
               //     pl[i].level = 1; pl[i].exp = 0; pl[i].spr = sprite_por_clase_heroe(pl[i].clase);
               //     //quitar_baston[i] = lStartingMessage->mProfiles[i].mRemoveStaff;
               // }
            }

            //delete lMsg;
            lDoStart = true;

        }

        while (!tick_terminado());
    };

    do {
        int epi, lev;

        num_players = gStartingMessage->mNumPlayers;
        local_player = gStartingMessage->mAssignedPlayerId;
        epi = gStartingMessage->mEpisode;
        lev = gStartingMessage->mLevel;
        ///* startX = lMsgStart->mStartX;
        // startY = lMsgStart->mStartY;*/

        // sprintf(s, "%s.niv", lMsgStart->mArena);

        // tim = lMsgStart->mTimeMax;

        for (int i = 0; i < gStartingMessage->mNumPlayers; i++)
        {
            pl[i].clase = (w_clase)gStartingMessage->mProfiles[i].mClass;
            memcpy(pl[i].nombre, gStartingMessage->mProfiles[i].mName, 8);
            //memcpy(pers[i], lMsgStart->mProfiles[i].mCharacter, 8);
            pl[i].c1 = gStartingMessage->mProfiles[i].mColor1;
            pl[i].c2 = gStartingMessage->mProfiles[i].mColor2;
            pl[i].level = 1; pl[i].exp = 0; pl[i].spr = sprite_por_clase_heroe(pl[i].clase);
            //quitar_baston[i] = lStartingMessage->mProfiles[i].mRemoveStaff;
        }

        delete gStartingMessage; gStartingMessage = 0;

        nivel_batalla_cliente(ch, pant, num_players, local_player, 0, epi, lev, pl, &client, &recQueue, &sendQueue);

    } while (gStartingMessage != 0);


}

#endif

/*
w_clase elige_pers_dmch_old(charset *ch, ALLEGRO_BITMAP *pant, int p, char *pers)
{
      char nombres[30][30]={"Sigma Soldier","Omega Warrior","Beta Sorcerer",
                            "Orc","Skeleton","Knight","Mercenary","Gamma Gladiator",
                            "Sentinel",
                            "Barbarian","Priest","Magician","Gamma Wizard","Android",
                            "Tank","Cyborg","Gamma Trooper","Sacred Sigma Soldier",
                            "Sacred Beta Sorcerer","Sacred Omega Warrior","Gamma Alien"};
      char files[30][8]={"sigma","omega","beta","orco","esquele","caballe","mercena","ggamma",
                         "centine",
                         "barbaro","brujo","hechice","wgamma","android","tank","cyborg","gamma",
                         "ssigma","sbeta","somega","agamma"};
      w_clase clases[30]={SIGMA,OMEGA,BETA,OMEGA,OMEGA,OMEGA,OMEGA,OMEGA,OMEGA,OMEGA,BETA,BETA,BETA,SIGMA,
                        SIGMA,SIGMA,SIGMA,SIGMA,BETA,OMEGA,OMEGA};
      char s[30];
      int np, sel;
      if(hell_op==1) np=21; else np=17;

      ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
      sprintf(s,"PLAYER %d CHOOSE A CHARACTER",p);
      ch->texto(pant,s,150,20,BAMA1,true);

      for(int i=0; i<np; i++)
            ch->texto(pant,nombres[i],(640-(strlen(nombres[i])*13))/2,60+15*i,BMAR1,true);

      do{
      sel=elige_opcion(pant,150,460,50,15,np,0);
      }while(sel<0);

      sprintf(pers,"%s",files[sel]);
      return clases[sel];
}
*/


w_clase elige_pers_dmch(charset *ch, ALLEGRO_BITMAP *pant, int p, char *pers, byte &col1, byte &col2, bool &quitar_baston)
{
	  struct TCharacter
	  {
		  char mName[30];
		  char mFile[8];
		  w_clase mClass;
		  byte mColor1, mColor2;
		  bool mRemoveStaff;
	  };

	  TCharacter personajes[] = {

		  {"Sigma Soldier",				"sigma",	SIGMA,	BROJO, BGRIS, false},
		  {"Omega Warrior",				"omega",	OMEGA,	BNARA, BGRIS, false},
		  {"Beta Sorcerer",				"beta",		BETA,	BAZUM, BAZUN, false},
		  {"Orc",						"orco",		OMEGA,	BVER3, BGRIS, false},
		  {"Skeleton",					"esquele",	OMEGA,	BROJO, BMAR2, false},
		  {"Knight",					"caballe",	OMEGA,	BROJO, BGRIS, false},
		  {"Mercenary (warrior)",		"mercena",	OMEGA,	BROJO, BNEGR, false},
		  {"Gamma Gladiator",			"ggamma",	OMEGA,	BNARA, BAMA1, false},
		  {"Orc King",					"reyorco",	OMEGA,	BAZUM, BAZUC, false},
		  {"Sentinel",					"centine",	OMEGA,	BAZUM, BGRIS, false},
		  {"Dragon",					"dragon",	BETA,	BVER3, BAMA2, true},
		  {"Barbarian",					"barbaro",	OMEGA,	BROJO, BMAR3, false},
		  {"Golem",						"golem",	BETA,	BNARA, BNEGR, true},
		  {"Priest",					"brujo",	BETA,	BAMA2, BAZUM, false},
		  {"Magician",					"hechice",	BETA,	BGRIS, BROJO, false},
		  {"Gamma Wizard",				"wgamma",	BETA,	BNARA, BAMA1, false},
		  {"Koppalha",					"koppalh",	BETA,	BROJO, BAMA1, false},
		  {"Probe",						"sonda",	BETA,	BROJO, BGRIS, true},
		  {"Android",					"android",	SIGMA,	BROJO, BGRIS, false},
		  {"Mercenary (soldier)",		"mercena",	SIGMA,	BAMA2, BVER3, false},
		  {"Tank",						"tank",		SIGMA,	BAMA2, BAZUM, false},
		  {"Cyborg",					"cyborg",	SIGMA,	BMAR2, BGRIS, false},
		  {"Gamma Trooper",				"gamma",	SIGMA,	BNARA, BAMA1, false},
		  {"Cyborg Invader",			"cgamma",	SIGMA,	BNARA, BGRIS, false},
		  {"Sacred Sigma Soldier",		"ssigma",	SIGMA,	BROJO, BGRIS, false},
		  {"Sacred Omega Warrior",		"somega",	OMEGA,	BNARA, BGRIS, false},
		  {"Sacred Beta Sorcerer",		"sbeta",	BETA,	BAZUM, BAZUN, false},
		  {"Zombie",					"zombie",	BETA,	BMAR2, BROJO, true},
		  {"Demon",		  	    		"demonio",  BETA,	BROJO, BNEGR, true},
		  {"Worm",						"gusano",   BETA,	BROSA, BROJO, true},
		  {"Invader (soldier)",			"agamma",	SIGMA,	BROJO, BGRIS, false},
		  {"Invader (warrior)",			"agamma",	OMEGA,	BROJO, BGRIS, false},
		  {"Invader (sorcerer)",		"agamma",	BETA,	BROJO, BGRIS, false},
		  {"Adan",						"adan",		BETA,	BROJO, BGRIS, true},
	  };

	  ALLEGRO_BITMAP *miniaturas[40];
	  char s[30];
	  int num_pers = hell_op ? 34 : 24;

	  // Carga miniaturas
	  for(int i = 0; i < num_pers; i++)
	  {
		  miniaturas[i] = crea_miniatura(ch, personajes[i].mFile, personajes[i].mColor1, personajes[i].mColor2);
	  }

      int np, espera = 0, sel = 0;
      if(hell_op==1) np=21; else np=17;
	  bool salir = false;

	  ALLEGRO_KEYBOARD_STATE keyboard;

	  inicia_tick();
      speed_counter=1;
      do{

            while(speed_counter > 0){
				// Logic
				al_get_keyboard_state(&keyboard);

                if((evento_control(CDER)) && (espera==0)){
                    sel = (sel + 1) % num_pers; espera=10;
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
				if((evento_control(CIZQ)) && (espera==0)){
                    sel = (sel - 1 + num_pers) % num_pers; espera=10;
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
				if((evento_control(CABA)) && (espera==0)){
                    sel = (sel + 9) % num_pers; espera=10;
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
				if((evento_control(CARR)) && (espera==0)){
                    sel = (sel - 9 + num_pers) % num_pers; espera=10;
					if(sound_op) al_play_sample(selec,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
				};
				if(((al_key_down(&keyboard,ALLEGRO_KEY_ENTER)) || (evento_control(CBU1)))&& (espera==0)) {
                    salir = true;
					if(sound_op) al_play_sample(accept_snd,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);
                };

				if(espera > 0) espera--;

                speed_counter--;
            };

			al_set_target_bitmap(pant);
			ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
			if (p >= 0) sprintf(s,"PLAYER %d CHOOSE A CHARACTER",p);
			else		sprintf(s," PLEASE CHOOSE A CHARACTER");
			ch->texto(pant,s,150,20,BAMA1+2,true);

			if(personajes[sel].mClass == SIGMA)
				sprintf(s,"%s %c%c", personajes[sel].mName, FONT_SIGMA_WEAPON, FONT_SIGMA_WEAPON+1);
			else if (personajes[sel].mClass == OMEGA)
				sprintf(s,"%s %c%c", personajes[sel].mName, FONT_OMEGA_WEAPON, FONT_OMEGA_SHIELD);
			else if (personajes[sel].mClass == BETA)
				sprintf(s,"%s %c%c%c", personajes[sel].mName, FONT_BETA_AMMO, FONT_BETA_AMMO+1, FONT_BETA_AMMO+2);

			ch->texto(pant,s,(640-(strlen(s)*13))/2,85,BBLAN,true);

			for(int i = 0; i < num_pers; i++)
			{
				int x = 40 + ((i % 9) * (50 + 10));
				int y = 150 + ((int)(i / 9) * (40 + 10));

				al_set_target_bitmap(pant);
				al_draw_bitmap(miniaturas[i], x, y, 0);
			}

			int xx = 40 + ((sel %9) * (50 + 10));
			int yy = 150 + ((int)(sel / 9) * (40 + 10));

			rect(pant, xx - 1, yy - 1, xx + 52, yy + 42, BROJO);
			rect(pant, xx - 2, yy - 2, xx + 53, yy + 43, BROJO);

			update_display_and_flip(pant);

			speed_counter = consume_ciclos_transcurridos();

	  } while (!salir);

	  sprintf(pers,"%s",personajes[sel].mFile);
	  col1 = personajes[sel].mColor1;
	  col2 = personajes[sel].mColor2;
	  quitar_baston = personajes[sel].mRemoveStaff;

	  for (int i = 0; i < num_pers; i++)
	  {
		  destruir_bitmap(miniaturas[i]);
	  }

      return personajes[sel].mClass;
}

void dmch_pantalla_resultados(charset *ch, ALLEGRO_BITMAP *pant, ALLEGRO_BITMAP *pic, batalla *b, const char *arena_name, int num_players)
{
	 int results[8]={0,1,2,3,4,5,6,7}, aux;
	 char s[60];

      for(int j=0; j<num_players-1; j++)
      for(int i=0; i<num_players-1; i++)
         if(b->dev_player(results[i+1])->muertes()>b->dev_player(results[i])->muertes()){
            aux=results[i];
            results[i]=results[i+1];
            results[i+1]=aux;};

      //blit(pic,pant,0,0,0,0,640,400);
	  al_set_target_bitmap(pant);
	  al_draw_bitmap(pic,0,0,0);
      ch->texto(pant,"DEATHMATCH RESULTS",220,20,BAMA1+2,true);
      sprintf(s,"Battled on %s during %d:%02d",arena_name,b->tiempo_batalla()/60,b->tiempo_batalla()%60);
      ch->texto(pant,s,(640-(strlen(s)*13))/2,60,BMAR1,true);
      for(int i=0; i<num_players; i++){
         sprintf(s,"%6s       %d kills",b->dev_player(results[i])->nombre(),b->dev_player(results[i])->muertes());
         ch->texto(pant,s,(640-(strlen(s)*13))/2,100+20*i,b->dev_player(results[i])->color(1),true);
      };
      if(num_players > 1 && b->dev_player(results[0])->muertes()==b->dev_player(results[1])->muertes())
      sprintf(s,"Drawn match");
      else sprintf(s,"%s wins!",b->dev_player(results[0])->nombre());
      ch->texto(pant,s,(640-(strlen(s)*13))/2,300,BMAR1,true);
      ch->texto(pant,"Press any button to continue",145,360,BMAR1,true);
      update_display_and_flip(pant);
}

void deathmatch(charset *ch, ALLEGRO_BITMAP *pant, batalla *bt)
{
   char *ops[3], pers[4][8], *arenas[4], s[50];
   int nplayers, ar=0,frag=2,tim=1,op=0,frag_op[6]={1,5,10,20,50,100},
       time_op[6]={1,5,10,20,30,60}, results[4]={0,1,2,3};
   bool quitar_baston[4];
   player_info pl[4];
   byte default_colors[4][2]={{BROJO,BGRIS},{BAZUM,BGRIS},{BVER3,BGRIS},{BAMA2,BGRIS}};
   batalla *b;
   charset *ch2;
   ALLEGRO_BITMAP *pic;
   ALLEGRO_BITMAP *pant2;
   nivel *ni;
   waves *so;
   //int SCRX[5]={320,360,400,640,800},SCRY[5]={200,240,300,400,600},
       /*SCRDRV[5]={GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT,GFX_AUTODETECT},*/
   int i,j;
   //RGB *pal;
    ALLEGRO_KEYBOARD_STATE keyboard;

   //pal=DBG_NEW RGB[256];
   for(j=0; j<4; j++){
         arenas[j]=DBG_NEW char[20];
         sprintf(s,"%s/%sa.niv",GDIR,epi_files[j]);
         ni=DBG_NEW nivel();
         ni->carga(s);
         sprintf(arenas[j],"%s",ni->nombre());
         delete ni;
   };

   for(j=0; j<3; j++) ops[j]=DBG_NEW char[10];
   sprintf(ops[0],"Two");
   sprintf(ops[1],"Three");
   sprintf(ops[2],"Four");
   nplayers=menu(pant,ch,"Number of players",ops,3,0)+2;
   for(j=0; j<3; j++) delete[] ops[j];
   if(nplayers>=2){

		 ch->destruye_todas_skins();

         for(i=0; i<nplayers; i++){
				 pl[i].clase=elige_pers_dmch(ch,pant,i+1,pers[i], pl[i].c1, pl[i].c2, quitar_baston[i]);
                 sprintf(pl[i].nombre,"%d Up",i+1);
                 pl[i].level=1; pl[i].exp=0; pl[i].spr=3+i;
                 sprintf(s,"%s/%s.%s",GDIR,pers[i],FILE_IMG_EXT);
                 ch->carga_guerrero(s,3+i);
				 bt->dev_nivel()->cambia_fspr(pers[i],3+i);
				 al_set_target_bitmap(pant);
                 personalizar(ch,pant,bt,&pl[i],quitar_baston[i]);
         };
         // Opciones de Deathmatch
         do{
            ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
            ch->texto(pant,"DEATHMATCH GAMEPLAY OPTIONS",150,20,BAMA1+2,true);
            ch->texto(pant,"Arena",60,140,BMAR1,true);
            ch->texto(pant,arenas[ar],60,160,BAMA1+2,true);
            ch->texto(pant,"Limit of frags",60,200,BMAR1,true);
            sprintf(s,"%d deaths",frag_op[frag]);
            ch->texto(pant,s,420,200,BAMA1+2,true);
            ch->texto(pant,"Time limit",60,260,BMAR1,true);
            sprintf(s,"%d min.",time_op[tim]);
            ch->texto(pant,s,420,260,BAMA1+2,true);
            ch->texto(pant,"Let's go!",60,320,BMAR1,true);

            op=elige_opcion(pant,20,560,135,60,4,op);
            switch(op){

               case 0 : do{
                           ar=menu(pant,ch,"Choose an Arena",arenas,3+hell_op,ar);
                        }while(ar<0); break;
               case 1 : frag++; if(frag==6) frag=0; break;
               case 2 : tim++; if(tim==6) tim=0; break;

            };

         }while(op!=3);

         sprintf(s,"%sa.niv",epi_files[ar]);

         so=DBG_NEW waves(GDIR);
         so->set_activo(sound_op);
         b=DBG_NEW batalla(GDIR,s,so,DMCH,WINDOW_RESOLUTIONS[res_op][0],split_op);
         ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

         // Ingresa los jugadores
         for(i=0; i<nplayers; i++){
            b->dev_nivel()->cambia_fspr(pers[i],3+i);
            b->ingresa_player(pl[i].nombre,pl[i].spr,pl[i].clase,pl[i].c1,pl[i].c2,cont_op[i],1,0);
			b->dev_player(i)->set_baston_mago(!quitar_baston[i]);
         };
         //stop_mod();
		 al_detach_audio_stream(musica);

         if(music_op) b->activa_musica();
         ch2->carga_sprites(b);
         if(console_op) b->activa_input_consola();
         b->elimina_items_inutiles();
         for(i=0; i<NSPR; i++)
            so->load_wav_warrior(GDIR,b->dev_nivel()->fspr(i),i);

         //A jugar!
		 al_set_target_bitmap(pant);
         sprintf(s,"%s/imagen5.%s",GDIR,FILE_IMG_EXT);
         pic=cargar_bitmap(s);
         al_draw_bitmap(pic,0,0,0);

         ch->texto(pant,"Deathmatch battle",220,60,BAMA1+2,true);
         ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
         ch->texto(pant,arenas[ar],(640-(strlen(arenas[ar])*13))/2,220,BAMA1+2,true);
         ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
         update_display_and_flip(pant);
         ch2->carga_bitmaps_batalla(b);
         ch2->crea_tablas_color(b->dev_nivel());
         for(i=0; i<nplayers; i++)
                  so->load_wav_warrior(GDIR,pers[i],3+i);
         ch->texto(pant,"Press any button to start",155,360,BAMA1+2,true);
         update_display_and_flip(pant);

         esperar_evento();

		  //if(res_op!=3)
			 ////set_gfx_mode(SCRDRV[res_op], SCRX[res_op], SCRY[res_op], 0, 0);
			 //al_resize_display(game_display,WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);
         //ch->carga_bitmaps_batalla(b);
		 pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

		 inicia_tick();
         speed_counter=1;
         do{

            while(speed_counter > 0){

                b->actualiza(1);
                speed_counter--;
            };
            ch2->muestra_pantalla(pant2,b);
			muestra_fps(pant2,ch2);
            update_display_and_flip(pant2);

			speed_counter = consume_ciclos_transcurridos();

			if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
                         //get_palette(pal);
                         sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
                         al_save_bitmap(s,pant2);
                         b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
						};

			while(!tick_terminado());

			al_get_keyboard_state(&keyboard);

      }while((!al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE)) && (b->tiempo_batalla()<60*time_op[tim]) && (b->maxim_frags()<frag_op[frag]));

      if(music_op) b->desactiva_musica();

      // Regresa al modo 640x400
	 /* if(res_op!=3) al_resize_display(game_display, 640, 400 );*/
      //if(res_op!=3) set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
      //set_pallete(pal);
      destruir_bitmap(pant2);
      delete ch2;
      //delete pal;


	  dmch_pantalla_resultados(ch, pant, pic, b, arenas[ar], nplayers);

      destruir_bitmap(pic);
      delete so;
      delete b;
      for(j=0; j<4; j++) delete arenas[j];

      esperar_evento();

   };
}

#ifdef DEATHMATCH_ONLINE


int elige_online_o_local(charset *ch, ALLEGRO_BITMAP *pant)
{
	char *ops[2];

	ops[0] = new char[40];
	sprintf(ops[0], "Online (Local network)");
	ops[1] = new char[40];
	sprintf(ops[1], "Split screen");

	int op = menu(pant, ch, "        Start game        ", ops, 2, 0);

	delete[] ops[0];
	delete[] ops[1];

	return op;
}

int elige_server_o_cliente(charset *ch, ALLEGRO_BITMAP *pant)
{
	char *ops[2];

	ops[0] = new char[40];
	sprintf(ops[0], "Create session");
	ops[1] = new char[40];
	sprintf(ops[1], "Join session");

	int op = menu(pant, ch, "   Online Game   ", ops, 2, 0);

	delete[] ops[0];
	delete[] ops[1];

	return op;
}



void deathmatch_online_server(charset *ch, ALLEGRO_BITMAP *pant, batalla *bt)
{

	CNetworkServer server;
	CMessageQueue recQueue;
	CMessageQueue sendQueue;
	ALLEGRO_CONFIG *online_cfg;
	bool lStatus;

	nivel *ni;
	player_info pl[MAXPL];
	char pers[MAXPL][8], *arenas[4], s[60];
	int ar=0,frag=2,tim=1,op=0,frag_op[6]={1,5,10,20,50,100}, time_op[6]={1,5,10,20,30,60}, num_players = 1;
	bool quitar_baston[MAXPL];

	online_cfg = get_online_config();

	for(int j=0; j<4; j++){
         arenas[j]=DBG_NEW char[20];
         sprintf(s,"%s/%sa.niv",GDIR,epi_files[j]);
         ni=DBG_NEW nivel();
         ni->carga(s);
         sprintf(arenas[j],"%s",ni->nombre());
         delete ni;
   };

    ch->destruye_todas_skins();

	pl[0].clase=elige_pers_dmch(ch,pant,-1,pers[0],pl[0].c1, pl[0].c2, quitar_baston[0]);
    sprintf(pl[0].nombre, al_get_config_value(online_cfg, "", "nickname") );
    pl[0].level=1; pl[0].exp=0; pl[0].spr=0;
    sprintf(s,"%s/%s.%s",GDIR,pers[0],FILE_IMG_EXT);
    ch->carga_guerrero(s,3+0);
	bt->dev_nivel()->cambia_fspr(pers[0],0);
	al_set_target_bitmap(pant);
    personalizar(ch,pant,bt,&pl[0],quitar_baston[0]);

	al_set_config_value(online_cfg, "", "nickname", pl[0].nombre);

	unsigned short conPort = (unsigned short)atoi(al_get_config_value(online_cfg, "", "port"));
	lStatus = server.init(conPort);
	if(!lStatus)
	{
		dialog_accept(pant,ch,"Network error","Accept");
		al_destroy_config(online_cfg);
		return;
	}

	al_rest(0.5f);

	// Wait for profiles (lobby)


	std::vector<TLobbyEntry> lLobby;
	ALLEGRO_BITMAP *lMyPicture;

	lMyPicture = crea_miniatura(ch, pers[0], pl[0].c1, pl[0].c2);

	bool lStayAtLobby = true;
	while(lStayAtLobby)
	{
		inicia_tick();

		server.sendMessages(&sendQueue);

		server.acceptClients();

		server.receiveMessages(&recQueue);

		//Process messages
		TMessage *lMsg;
		while((lMsg = recQueue.extractFirst()) != 0)
		{
			if(lMsg->mType == eMsgProfile)
			{
				TMsgProfile *lProfMsg = (TMsgProfile*)lMsg;

				TLobbyEntry nuevo;
				nuevo.lobbyId = lProfMsg->mLobbyId;
				sprintf(nuevo.pl.nombre,"%s",lProfMsg->mProfile.mName);
				sprintf(nuevo.character,"%s",lProfMsg->mProfile.mCharacter);
				nuevo.pl.clase = (w_clase)lProfMsg->mProfile.mClass;
				nuevo.pl.c1=lProfMsg->mProfile.mColor1;
				nuevo.pl.c2=lProfMsg->mProfile.mColor2;
				nuevo.pl.level=1; nuevo.pl.exp=0; nuevo.pl.spr=1;
				nuevo.remove_staff = lProfMsg->mProfile.mRemoveStaff;
				nuevo.mPicture = 0;

				lLobby.push_back(nuevo);

				//lStayAtLobby = false;
			}

			delete lMsg;
		}

		// Remove disconnected people
		for(int i = 0; i < lLobby.size(); i++)
		{
			if(!server.isClientConnected(lLobby[i].lobbyId))
			{
				destruir_bitmap(lLobby[i].mPicture);
				lLobby.erase(lLobby.begin()+i);
				i--;
			}
		}

		// Show GUI
		al_set_target_bitmap(pant);
		ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
		ch->texto(pant,"DEATHMATCH LOBBY",180,20,BAMA1+2,true);
		for(int i = 0; i < MAXPL/*lLobby.size()*/; i++)
		{
			int xx = 40 + 280 * (i%2);
			int yy = 60 + 60 * int(i/2);
			rect(pant,xx,yy,xx+270,yy+55,BBLAN);
			char *lName = 0;
			char *lAdress = 0;
			ALLEGRO_BITMAP *pic = 0;

			if(i == 0)
			{
				lName = pl[0].nombre;
				lAdress = server.getServerIPAdress();
				pic = lMyPicture;
			}
			else if (i-1 < lLobby.size())
			{
				lName = lLobby[i-1].pl.nombre;
				lAdress = server.getClientIPAdress(lLobby[i-1].lobbyId);

				if(!lLobby[i-1].mPicture)
				{
					lLobby[i-1].mPicture = crea_miniatura(ch, lLobby[i-1].character, lLobby[i-1].pl.c1, lLobby[i-1].pl.c2 );
				}
				pic = lLobby[i-1].mPicture;


			}
			//sprintf(s,"%s - LobbyId:%d",lLobby[i].pl.nombre, lLobby[i].lobbyId);
			//ch->texto(pant, s, 50, 60 + 40*i, BBLAN, true);
			if(lName)
			{
				al_set_target_bitmap(pant);
				if(pic)
					al_draw_bitmap(pic,xx + 205,yy+7,0);
				ch->texto(pant, lName, xx + 15, yy + 10, BBLAN, true);
				ch->texto(pant, lAdress, xx + 15, yy + 30, BBLAN, true);

			}
			else
			{
				al_draw_filled_rectangle(xx+1,yy+1,xx+269,yy+55,al_map_rgba(0,0,0,192));
			}
		}
		ch->texto(pant,"Press any button to start game",150,350,BAMA1+2,true);
		update_display_and_flip(pant);

		if(evento_control(CBU1) || evento_control(CBU2) || evento_control(CBU2) || evento_control(CBU4))
			lStayAtLobby = false;


		while(!tick_terminado());
	};

	server.stopAccepting();

	destruir_bitmap(lMyPicture);

	// Opciones de Deathmatchs
    do{
		al_set_target_bitmap(pant);
		ch->ventana(pant,0,0,640,400,BMAR3,BMAR2,FALSE);
		ch->texto(pant,"DEATHMATCH GAMEPLAY OPTIONS",150,20,BAMA1+2,true);
		ch->texto(pant,"Arena",60,140,BMAR1,true);
		ch->texto(pant,arenas[ar],60,160,BAMA1+2,true);
		ch->texto(pant,"Limit of frags",60,200,BMAR1,true);
		sprintf(s,"%d deaths",frag_op[frag]);
		ch->texto(pant,s,420,200,BAMA1+2,true);
		ch->texto(pant,"Time limit",60,260,BMAR1,true);
		sprintf(s,"%d min.",time_op[tim]);
		ch->texto(pant,s,420,260,BAMA1+2,true);
		ch->texto(pant,"Let's go!",60,320,BMAR1,true);

		op=elige_opcion(pant,20,560,135,60,4,op);
		switch(op){

			case 0 : do{
						ar=menu(pant,ch,"Choose an Arena",arenas,3+hell_op,ar);
					}while(ar<0); break;
			case 1 : frag++; if(frag==6) frag=0; break;
			case 2 : tim++; if(tim==6) tim=0; break;

		};

    }while(op!=3);

	// Start game!!!

	num_players = lLobby.size() + 1;
	if(num_players > MAXPL) num_players = MAXPL;

	// Assign player mask to final players
	for(int i = 1; i < num_players; i++)
	{
		lLobby[i-1].playerId = i;
		server.assignPlayerMaskToClient(lLobby[i-1].lobbyId, getMaskFromPlayerId(i));
	}

	// Register final profiles
	for(int i = 1; i < num_players; i++)
	{
		pl[i] = lLobby[i-1].pl;
		pl[i].spr = i;
		quitar_baston[i] = lLobby[i-1].remove_staff;
		sprintf(pers[i],"%s",lLobby[i-1].character);
	}

	server.sendMessages(&sendQueue);

	// Load screen
	waves *so;
	batalla *b;
	charset *ch2;
	ALLEGRO_BITMAP *pic, *pant2;
	ALLEGRO_KEYBOARD_STATE keyboard;
	int local_player = 0;

	sprintf(s,"%sa.niv",epi_files[ar]);
	so=DBG_NEW waves(GDIR);
    so->set_activo(sound_op);
    b=DBG_NEW batalla(GDIR,s,so,DMCH,WINDOW_RESOLUTIONS[res_op][0],split_op);
	b->asigna_jug_local(local_player);
	b->asigna_modo_red(MRSERVER);
	b->asigna_tiempo_max_batalla(60 * time_op[tim]);
    ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

	// Calcula posicion aleatoria (sin colision) para los jugadores
	bald start_pos[MAXPL][2];

	for (int i = 0; i < num_players; i++)
	{
		bald x, y;
		bool alreadyTaken = false;
		do {
			x = rand() % MSIZEX;
			y = rand() % MSIZEY;

			alreadyTaken = false;
			for (int j = 0; j < i; j++)
			{
				if (start_pos[j][0] == x && start_pos[j][1] == y)
					alreadyTaken = true;
			}

		} while (alreadyTaken || !b->permite2(x, y, NULL));

		start_pos[i][0] = x;
		start_pos[i][1] = y;
	}

	// Send start info
	for (int j = 1; j < num_players; j++)
	{
		TMsgStartMatch *lMsgStart = DBG_NEW TMsgStartMatch;
		lMsgStart->mAssignedPlayerId = (tbyte)j;
		lMsgStart->mStartX = start_pos[j][0];
		lMsgStart->mStartY = start_pos[j][1];
		lMsgStart->mType = eMsgStartMatch;
		lMsgStart->mSender = MASK_SERVER;
		lMsgStart->mReceiver = getMaskFromPlayerId(j);
		lMsgStart->mNumPlayers = num_players;
		sprintf(lMsgStart->mArena, "%sa", epi_files[ar]);
		lMsgStart->mTimeMax = time_op[tim] * 60;

		for (int i = 0; i < MAXPL; i++)
		{
			lMsgStart->mProfiles[i].mClass = pl[i].clase;
			lMsgStart->mProfiles[i].mColor1 = pl[i].c1;
			lMsgStart->mProfiles[i].mColor2 = pl[i].c2;
			lMsgStart->mProfiles[i].mRemoveStaff = quitar_baston[i];
			memcpy(lMsgStart->mProfiles[i].mName, pl[i].nombre, 8);
			memcpy(lMsgStart->mProfiles[i].mCharacter, pers[i], 8);
		}

		sendQueue.queueMessage(lMsgStart);
	}

    // Ingresa los jugadores
    for(int i=0; i<num_players; i++){
		b->dev_nivel()->cambia_fspr(pers[i],i);
		b->ingresa_player(pl[i].nombre,pl[i].spr,pl[i].clase,pl[i].c1,pl[i].c2,i == local_player ? cont_op[0] : NOTC, 1,0);
		if(quitar_baston[i])
			b->dev_player(i)->set_baston_mago(false);
        if(i != 0)
            b->dev_player(i)->set_activo(false);
    };

	// Posición inicial
	b->dev_player(0)->nueva_pos((float)start_pos[0][0], (float)start_pos[0][1]);

    //stop_mod();
	al_detach_audio_stream(musica);

    if(music_op) b->activa_musica();
    ch2->carga_sprites(b);
    if(console_op) b->activa_input_consola();
    b->elimina_items_inutiles();
    for(int i=0; i<NSPR; i++)
		so->load_wav_warrior(GDIR,b->dev_nivel()->fspr(i),i);

    //A jugar!
	al_set_target_bitmap(pant);
    sprintf(s,"%s/imagen5.%s",GDIR,FILE_IMG_EXT);
    pic=cargar_bitmap(s);
    al_draw_bitmap(pic,0,0,0);

    ch->texto(pant,"Deathmatch battle",220,60,BAMA1+2,true);
    ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
    ch->texto(pant, b->dev_nivel()->nombre(),(640-(strlen(b->dev_nivel()->nombre())*13))/2,220,BAMA1+2,true);
    ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
    update_display_and_flip(pant);
    ch2->carga_bitmaps_batalla(b);
    ch2->crea_tablas_color(b->dev_nivel());
    /*for(int i=0; i<num_players; i++)
            so->load_wav_warrior(GDIR,pers[i],i);*/
    //ch->texto(pant,"Press any button to start",155,360,BAMA1,true);
    update_display_and_flip(pant);

    //esperar_evento();

	pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

	inicia_tick();
    speed_counter=1;
    do{

		server.receiveMessages(&recQueue);

		// Remove disconnected people
		for(int i = 0; i < lLobby.size(); i++)
		{
			if(!server.isClientConnected(lLobby[i].lobbyId))
			{
				TMsgPlDisconnect *lMsgDisc = DBG_NEW TMsgPlDisconnect;

				lMsgDisc->mType = eMsgPlDisconnect;
				lMsgDisc->mSender = MASK_SERVER;
				lMsgDisc->mReceiver = MASK_SERVER;
				lMsgDisc->mPlayerId = lLobby[i].playerId;

				recQueue.queueMessage(lMsgDisc);

				destruir_bitmap(lLobby[i].mPicture);
				lLobby.erase(lLobby.begin()+i);
				i--;
			}
		}

		while(speed_counter > 0){

			b->actualiza_modo_red(&recQueue,&sendQueue);
			speed_counter--;
		};

		server.sendMessages(&sendQueue);

		ch2->muestra_pantalla(pant2,b);
		muestra_fps(pant2,ch2);
		update_display_and_flip(pant2);

		speed_counter = consume_ciclos_transcurridos();

		if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
						//get_palette(pal);
						sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
						al_save_bitmap(s,pant2);
						b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
					};

		while(!tick_terminado());

		al_get_keyboard_state(&keyboard);

	}while((!al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE)) && (b->tiempo_batalla()<60*time_op[tim]) && (b->maxim_frags()<frag_op[frag]));

	// Notifica el fin de la partida
	TMsgEndMatch *lMsgEnd = DBG_NEW TMsgEndMatch;

	lMsgEnd->mType = eMsgEndMatch;
	lMsgEnd->mSender = MASK_SERVER;
	lMsgEnd->mReceiver = MASK_ALL_CLIENTS;

	sendQueue.queueMessage(lMsgEnd);
	server.sendMessages(&sendQueue);


	 if(music_op) b->desactiva_musica();

      // Regresa al modo 640x400
	 /* if(res_op!=3) al_resize_display(game_display, 640, 400 );*/
      //if(res_op!=3) set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
      //set_pallete(pal);
      destruir_bitmap(pant2);
      delete ch2;
      //delete pal;

	  dmch_pantalla_resultados(ch, pant, pic, b, arenas[ar], num_players);

      destruir_bitmap(pic);
      delete so;
      delete b;
      for(int j=0; j<4; j++) delete arenas[j];

	  for(int j=0; j < lLobby.size(); j++)
		  if(lLobby[j].mPicture) destruir_bitmap(lLobby[j].mPicture);
	  lLobby.clear();

      esperar_evento();

	  save_online_config(online_cfg);
	  al_destroy_config(online_cfg);

}

void deathmatch_online_cliente(charset *ch, ALLEGRO_BITMAP *pant, batalla *bt)
{
	CNetworkClient client;
	CMessageQueue recQueue;
	CMessageQueue sendQueue;
	ALLEGRO_CONFIG *online_cfg;
	bool lStatus;


	player_info pl[MAXPL];
	char pers[MAXPL][8], s[60];
	int local_player = 1;
	int num_players, tim;
	bool quitar_baston[MAXPL];

	online_cfg = get_online_config();

	ch->destruye_todas_skins();

	pl[0].clase=elige_pers_dmch(ch,pant,-1,pers[0],pl[0].c1,pl[0].c2,quitar_baston[0]);
    sprintf(pl[0].nombre, al_get_config_value(online_cfg, "", "nickname") );
    pl[0].level=1; pl[0].exp=0; pl[0].spr=0;
    sprintf(s,"%s/%s.%s",GDIR,pers[0],FILE_IMG_EXT);
    ch->carga_guerrero(s,3+0);
	bt->dev_nivel()->cambia_fspr(pers[0],0);
	al_set_target_bitmap(pant);
    personalizar(ch,pant,bt,&pl[0],quitar_baston[0]);

	al_set_config_value(online_cfg, "", "nickname", pl[0].nombre);

	lStatus = client.init();
	if(!lStatus)
	{
		dialog_accept(pant,ch,"Network error","Accept");
		al_destroy_config(online_cfg);
		return;
	}

	char lIPAdress[16];
	sprintf(lIPAdress, "%s", al_get_config_value(online_cfg, "", "default server"));
	unsigned short conPort = (unsigned short)atoi(al_get_config_value(online_cfg, "", "port"));

	input_string(ch,pant,"Server IP adress:",lIPAdress,15);

	al_set_config_value(online_cfg, "", "default server", lIPAdress);

	ventana_texto(pant, ch, "Trying to connect...");
	lStatus = client.connectToServer(lIPAdress, conPort);
	if(!lStatus)
	{
		dialog_accept(pant,ch,"Couldnt connect to server","Accept");
		al_destroy_config(online_cfg);
		return;
	}


	TMsgProfile *lProf = DBG_NEW TMsgProfile();

	lProf->mType = eMsgProfile;
	lProf->mSender = MASK_NONE;
	lProf->mReceiver = MASK_SERVER;
	sprintf(lProf->mProfile.mName, pl[0].nombre);
	sprintf(lProf->mProfile.mCharacter, pers[0]);
	lProf->mProfile.mClass = (tbyte)pl[0].clase;
	lProf->mProfile.mColor1 = pl[0].c1;
	lProf->mProfile.mColor2 = pl[0].c2;
	lProf->mProfile.mRemoveStaff = quitar_baston[0];

	sendQueue.queueMessage(lProf);

	ventana_texto(pant,ch,"Waiting for other players...");

	// Wait for server signal to start
	bald startX = 0, startY = 0;
	bool lDoStart = false;

	byte lKeepAliveWait = 0;

	while(!lDoStart)
	{
		inicia_tick();

		if(lKeepAliveWait == 0)
		{
			lKeepAliveWait = 60;
			TMsgKeepAlive *lMsgAlive = DBG_NEW TMsgKeepAlive;

			lMsgAlive->mType = eMsgKeepAlive;
			lMsgAlive->mSender = 0;
			lMsgAlive->mReceiver = MASK_SERVER;

			sendQueue.queueMessage(lMsgAlive);
		}
		else
			lKeepAliveWait--;


		if(!client.isConnected())
		{
			dialog_accept(pant,ch,"Connection with server lost","Accept");
			al_destroy_config(online_cfg);
			return;
		}

		client.receiveMessages(&recQueue);

		if(client.isConnected())
            client.sendMessages(&sendQueue);

		TMessage *lMsg;

		while((lMsg = recQueue.extractFirst()) != 0)
		{
			if(lMsg->mType == eMsgStartMatch)
			{
				TMsgStartMatch *lMsgStart = (TMsgStartMatch*)lMsg;

				num_players = lMsgStart->mNumPlayers;
				local_player = lMsgStart->mAssignedPlayerId;
				startX = lMsgStart->mStartX;
				startY = lMsgStart->mStartY;

				sprintf(s,"%s.niv",lMsgStart->mArena);

				tim = lMsgStart->mTimeMax;

				for(int i = 0; i < lMsgStart->mNumPlayers; i++)
				{
					pl[i].clase = (w_clase)lMsgStart->mProfiles[i].mClass;
					memcpy(pl[i].nombre,lMsgStart->mProfiles[i].mName,8);
					memcpy(pers[i],lMsgStart->mProfiles[i].mCharacter,8);
					pl[i].c1=lMsgStart->mProfiles[i].mColor1;
					pl[i].c2=lMsgStart->mProfiles[i].mColor2;
					pl[i].level=1; pl[i].exp=0; pl[i].spr=i;
					quitar_baston[i] = lMsgStart->mProfiles[i].mRemoveStaff;
				}
			}

			delete lMsg;
			lDoStart = true;

		}

		while(!tick_terminado());
	};

	// Start!!
	waves *so;
	batalla *b;
	charset *ch2;
	ALLEGRO_BITMAP *pic, *pant2;
	ALLEGRO_KEYBOARD_STATE keyboard;

	so=DBG_NEW waves(GDIR);
    so->set_activo(sound_op);
    b=DBG_NEW batalla(GDIR,s,so,DMCH,WINDOW_RESOLUTIONS[res_op][0],split_op);
	b->asigna_jug_local(local_player);
	b->asigna_modo_red(MRCLIENTE);
	b->asigna_tiempo_max_batalla(tim);
    ch2=DBG_NEW charset(pant,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

    // Ingresa los jugadores
    for(int i=0; i<num_players; i++){
		b->dev_nivel()->cambia_fspr(pers[i],i);
		b->ingresa_player(pl[i].nombre,pl[i].spr,pl[i].clase,pl[i].c1,pl[i].c2,i == local_player ? cont_op[0] : NOTC,1,0);
		if(quitar_baston[i])
			b->dev_player(i)->set_baston_mago(false);
        if(i != local_player)
            b->dev_player(i)->set_activo(false);
    };

	// Posición inicial
	b->dev_player(local_player)->nueva_pos((float)startX, (float)startY);

    //stop_mod();
	al_detach_audio_stream(musica);

    if(music_op) b->activa_musica();
    ch2->carga_sprites(b);
    if(console_op) b->activa_input_consola();
    b->elimina_items_inutiles();
    for(int i=0; i<NSPR; i++)
		so->load_wav_warrior(GDIR,b->dev_nivel()->fspr(i),i);

    //A jugar!
	al_set_target_bitmap(pant);
    sprintf(s,"%s/imagen5.%s",GDIR,FILE_IMG_EXT);
    pic=cargar_bitmap(s);
    al_draw_bitmap(pic,0,0,0);

    ch->texto(pant,"Deathmatch battle",220,60,BAMA1+2,true);
    ch->texto(pant,"Now entering:",250,180,BAMA1+2,true);
    ch->texto(pant,b->dev_nivel()->nombre(),(640-(strlen(b->dev_nivel()->nombre())*13))/2,220,BAMA1+2,true);
    ch->texto(pant,"Please wait while loading level data",90,320,BAMA1+2,true);
    update_display_and_flip(pant);
    ch2->carga_bitmaps_batalla(b);
    ch2->crea_tablas_color(b->dev_nivel());
    /*for(int i=0; i<num_players; i++)
            so->load_wav_warrior(GDIR,pers[i],i);*/
    //ch->texto(pant,"Press any button to start",155,360,BAMA1,true);
    update_display_and_flip(pant);

    //esperar_evento();

	pant2=crear_bitmap(WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

	bool lSeguirJugando = true;

	inicia_tick();
    speed_counter=1;
    do{
		if(!client.isConnected())
		{
			lSeguirJugando = false;
			dialog_accept(pant2,ch,"Connection with server lost","Accept");
		}

		client.receiveMessages(&recQueue);

		while(speed_counter > 0){

			b->actualiza_modo_red(&recQueue, &sendQueue);
			speed_counter--;
		};

		client.sendMessages(&sendQueue);

		ch2->muestra_pantalla(pant2,b);
		muestra_fps(pant2,ch2);
		update_display_and_flip(pant2);

		speed_counter = consume_ciclos_transcurridos();

		if(al_key_down(&key_state,ALLEGRO_KEY_F12)) {
						//get_palette(pal);
						sprintf(s,"image%03d.png",sshots); sshots=(sshots+1)%1000;
						al_save_bitmap(s,pant2);
						b->encola_mensaje("Wrote screenshot to disk",15,PRIO1);
					};

		while(!tick_terminado());

		al_get_keyboard_state(&keyboard);

		if(b->partida_red_acabada()) lSeguirJugando = false;

	}while((!al_key_down(&keyboard,ALLEGRO_KEY_ESCAPE) && lSeguirJugando)/* && (b->tiempo_batalla()<60*time_op[tim]) && (b->maxim_frags()<frag_op[frag])*/);

	// Envia mensaje de desconexión
	TMsgPlDisconnect *lMsgDisc = DBG_NEW TMsgPlDisconnect;

    lMsgDisc->mType = eMsgPlDisconnect;
    lMsgDisc->mSender = getMaskFromPlayerId(local_player);
    lMsgDisc->mReceiver = MASK_SERVER;
    lMsgDisc->mPlayerId = local_player;

    sendQueue.queueMessage(lMsgDisc);
    client.sendMessages(&sendQueue);


	 if(music_op) b->desactiva_musica();

      // Regresa al modo 640x400
	 /* if(res_op!=3) al_resize_display(game_display, 640, 400 );*/
      //if(res_op!=3) set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
      //set_pallete(pal);
      destruir_bitmap(pant2);
      delete ch2;
      //delete pal;

	  if(b->partida_red_acabada())
	  {
		  dmch_pantalla_resultados(ch, pant, pic, b, b->dev_nivel()->nombre(), num_players);

		  esperar_evento();
	  }

	  destruir_bitmap(pic);
      delete so;
      delete b;

	  save_online_config(online_cfg);
	  al_destroy_config(online_cfg);
}

#endif

void parametros(int n, char *parm[])
{
      int i,j,k;
      for(i=1; i<n; i++){
         if(strcmp(parm[i],"-r")==0){
				for(j=0; j < NUM_WIN_RESOLUTIONS; j++)
					if(atoi(parm[i+1])==WINDOW_RESOLUTIONS[j][0]) res_op=j;
                //if(strcmp(parm[i+1],"360")==0) res_op=1;
                i++;
		 };
		 if(strcmp(parm[i],"-fullr")==0){
				for(j=0; j < NUM_FUL_RESOLUTIONS; j++)
					if(atoi(parm[i+1])==FULLSCR_RESOLUTIONS[j][0]) fullscr_res_op=j;
                //if(strcmp(parm[i+1],"360")==0) res_op=1;
                i++;
		 };
         if(strcmp(parm[i],"-music")==0)
                music_op=TRUE;
         if(strcmp(parm[i],"-nomusic")==0)
                music_op=FALSE;
         if(strcmp(parm[i],"-sound")==0)
                sound_op=TRUE;
         if(strcmp(parm[i],"-nosound")==0)
                sound_op=FALSE;
         if(strcmp(parm[i],"-gore")==0)
                gore_op=TRUE;
         if(strcmp(parm[i],"-nogore")==0)
                gore_op=FALSE;
         if(strcmp(parm[i],"-split")==0)
                split_op=TRUE;
         if(strcmp(parm[i],"-nosplit")==0)
                split_op=FALSE;
		 if(strcmp(parm[i],"-fullscreen")==0)
                fullscr_op=TRUE;
		 if(strcmp(parm[i],"-windowed")==0)
                fullscr_op=FALSE;
         if(strcmp(parm[i],"-reset_sshots")==0)
                sshots=0;
         if(strcmp(parm[i],"-allow_hell")==0)
                hell_op=1;
         if(strcmp(parm[i],"-console")==0)
                console_op=TRUE;
         if(strcmp(parm[i],"-nointro")==0)
                intro_am=FALSE;
         if(strcmp(parm[i],"-control")==0){
                j=atoi(parm[i+1]);
                k=atoi(parm[i+2]);
                if((j>=0) && (j<4) && (k>=0) && (k<=5)) cont_op[j]=(t_control)k;
                i+=2;};
      };
}
/*void increment_speed_counter()
{
        speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter);*/

int main(int argc, char *argv[])
{
      ALLEGRO_BITMAP *pant;
		game_display = NULL;
      charset *ch;
      waves *s;
      batalla *b;
      bool salir=FALSE;
      int op,i;
      char *ops[20], aux[60];

      if(al_init()){
         fprintf(stderr,"Allegro %s succesfully initialized.\n",ALLEGRO_VERSION_STR);
      } else {
         fprintf(stderr,"Could not initialize Allegro!\n");
         return -1;};

//      LOCK_VARIABLE(speed_counter);
//      LOCK_FUNCTION(increment_speed_counter);
//      install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));

//      pant=create_bitmap(640,400);
//		al_set_new_display_flags(ALLEGRO_FULLSCREEN);

		carga_fullscr_setting();

		//al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 0, ALLEGRO_REQUIRE);
		al_set_new_display_flags(fullscr_op ? ALLEGRO_FULLSCREEN : ALLEGRO_WINDOWED);
		al_set_new_bitmap_flags(ALLEGRO_MAG_LINEAR);
		if(fullscr_op)
		{
			game_display = al_create_display(FULLSCR_RESOLUTIONS[fullscr_res_op][0], FULLSCR_RESOLUTIONS[fullscr_res_op][1]);
        }
		else
			game_display = al_create_display(WINDOW_RESOLUTIONS[res_op][0], WINDOW_RESOLUTIONS[res_op][1]);

		if(!game_display) {
			fprintf(stderr, "failed to create display!\n");
			return -1;
		}

		al_set_window_title(game_display,NOMBRE_MODULO);

		pant = crear_bitmap(640,400);



		al_init_image_addon();
		al_set_standard_file_interface();

     s=DBG_NEW waves(GDIR);
     b=DBG_NEW batalla(GDIR,"mediev1.niv",s,COOP,640,TRUE);
     ch=DBG_NEW charset(pant,640,400);


     arranque_sistema(b,pant,ch,s,al_get_display_width(game_display),al_get_display_height(game_display)/*,GFX_AUTODETECT*/);

     cursor=cargar_bitmap("cursor.pcx"/*,ch->dev_paleta()*/);
	 al_convert_mask_to_alpha(cursor,al_map_rgb(0,0,0));

/*      cuadro_negro=create_bitmap(40,40);
      clear_to_color(cuadro_negro,128);*/
      selec=cargar_sample("select.wav");
      accept_snd=cargar_sample("accept.wav");
      cancel=cargar_sample("cancel.wav");
      partida=DBG_NEW game_info[MAX_GAME];
      for(i=0; i<MAX_GAME; i++)
              partida[i].en_uso=FALSE;
      carga_config();
      parametros(argc,argv);

	  if(!fullscr_op) al_resize_display(game_display,WINDOW_RESOLUTIONS[res_op][0],WINDOW_RESOLUTIONS[res_op][1]);

      if(intro_am==TRUE) intro(game_display);

//      srand();

     // set_gfx_mode(GFX_AUTODETECT, 640, 400, 0, 0);
      ch->carga_bitmaps_batalla(b);

	  //escena_final(game_display,ch,0);

      do{
         /*if(musica!=NULL) destroy_mod(musica);
         sprintf(aux,"%s\\menu.%s",GDIR,FILE_MOD_EXT);
         musica=load_mod(aux);
         if(music_op){
             set_mod_volume(120);
             play_mod(musica,TRUE);
         }*/
		 if(musica != NULL) { al_destroy_audio_stream(musica); musica = NULL; }
		 sprintf(aux,"%s/menu.%s",GDIR,FILE_MOD_EXT);
		 musica =  cargar_musica(aux);
		 if(music_op)
			al_attach_audio_stream_to_mixer(musica,al_get_default_mixer());

		 char loc_o_online_ops[2][20]={"LOCAL","ONLINE"};
		 char serv_o_client_ops[2][20]={"CREATE SESSION","JOIN SESSION"};

         op=menu_principal(ch,pant);
         switch(op){

            case 0 : 
#ifdef CRUSADE_ONLINE
                if (elige_online_o_local(ch, pant) != 0)
                    juego_normal(ch, pant, b);
                else
                {
                    if (elige_server_o_cliente(ch, pant) == 0)
                        cruzada_online_server(ch, pant, b);
                    else
                        cruzada_online_cliente(ch, pant, b);
                }
#else
                juego_normal(ch,pant,b); 
#endif
                break;
			case 1 :
#ifdef DEATHMATCH_ONLINE
					if(elige_online_o_local(ch, pant) != 0)
						deathmatch(ch,pant,b);
					 else
					 {
						  if(elige_server_o_cliente(ch, pant) == 0)
							  deathmatch_online_server(ch,pant,b);
						  else
							  deathmatch_online_cliente(ch,pant,b);
					 }
#else
					deathmatch(ch,pant,b);
#endif
					break;
            case 2 : opciones(ch,pant); break;
			case 3 : galeria(ch); break;
            case -1:
            case 4 : for(int i=0; i<2; i++) ops[i]=DBG_NEW char[10];
                     sprintf(ops[0],"YES");
                     sprintf(ops[1],"NO");
                     if(menu(pant,ch,"Are you sure?",ops,2,0)==0) salir=TRUE;
					 for (int i = 0; i < 2; i++) delete[] ops[i];
                     break;
         };
      }while(!salir);

      guarda_config();

      destruir_bitmap(cursor);
      /*destroy_bitmap(cuadro_negro);*/
      al_destroy_sample(selec);
      al_destroy_sample(accept_snd);
      al_destroy_sample(cancel);
      //destroy_mod(musica);
	  destruir_bitmap(pant);
      delete[] partida;
      delete b;
      delete s;
      delete ch;

      al_destroy_display(game_display);
      if(musica != NULL) { al_destroy_audio_stream(musica); musica = NULL; }
      al_uninstall_audio();
      al_uninstall_keyboard();
      al_uninstall_mouse();
      al_uninstall_joystick();
      al_uninstall_system();

#if defined SAHWINDOWS
#ifdef _DEBUG
	  _CrtDumpMemoryLeaks();
#endif
#endif
	  return 0;

}
//END_OF_MAIN();









