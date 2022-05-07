#include"her_tipo.hpp"
#include<math.h>
#include<allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
//#include<jgmod.h>
#include"her_graf.hpp"
#include"her_jueg.hpp"
#include"her_kti.hpp"

#ifndef PI
#define PI 3.1416
#endif

extern ALLEGRO_BITMAP *crear_bitmap(int w, int h);
extern ALLEGRO_BITMAP *cargar_bitmap(const char* f);
extern void destruir_bitmap(ALLEGRO_BITMAP *bmp);

static byte colors[KTI_NLEVELS][2]={{BAZUC,BAZUM},{BAZUO,BAZUM},{BROJO,BROSA},{BVER1,BVER3},{BAMA1,BNARA},{BNARA,BAMA2},{BMAR1,BMAR2},{BVIOL,BLILA},{BGRIS,BNEGR},{BIRIS,BROJO}};

static unsigned int times[KTI_NLEVELS]={1000,1000,1100,1100,1200,1200,1100,1300,1100,1100};

static ALLEGRO_BITMAP *invaders_colored[KTI_NLEVELS], *players_colored[4][5];

kti::kti(game_info *gi2, ALLEGRO_BITMAP *pant,charset *ch2)
{
        ALLEGRO_BITMAP *aux;
        int i,j,k,l;

        //display=create_sub_bitmap(pant,(pant->w-320)/2,(pant->h-200)/2,320,200);
		display = crear_bitmap(320,200);
        ch=ch2;

        gi=gi2; scroll[0]=0; scroll[1]=0; scroll[2]=0;
        planetx=50+(rand()%200); planety=-100; planetc=0;
        for(i=0; i<4; i++) {shipx[i]=120+(20*i); sdelay[i]=0;};
        for(i=0; i<KTI_MAXSHOTS; i++) shots[i].use=FALSE;

        for(i=0; i<5; i++)
        for(j=0; j<5; j++)
        ship[i][j]=crear_bitmap(12,12);
        for(i=0; i<3; i++)
        explos[i]=crear_bitmap(12,12);
        for(i=0; i<4; i++)
        planet[i]=crear_bitmap(30,30);
        for(i=0; i<3; i++)
        sky[i]=crear_bitmap(100,100);
        for(i=0; i<8; i++)
        border[i]=crear_bitmap(20,20);
        temp=crear_bitmap(12,12);


        aux=cargar_bitmap("KTI/kti.img");
		al_convert_mask_to_alpha(aux,al_map_rgb(0,0,0));

        for(i=0; i<5; i++)
        for(j=0; j<5; j++){
			//blit(aux,ship[i][j],12*i,j*12,0,0,12,12);
			al_set_target_bitmap(ship[i][j]);
			al_draw_bitmap_region(aux,12*i,12*j,12,12,0,0,0);
		}
        for(i=0; i<4; i++){
			//blit(aux,planet[i],75+(30*i),0,0,0,30,30);
			al_set_target_bitmap(planet[i]);
			al_draw_bitmap_region(aux,75+30*i,0,30,30,0,0,0);
		}
        for(i=0; i<3; i++){
			//blit(aux,explos[i],12*i,60,0,0,12,12);
			al_set_target_bitmap(explos[i]);
			al_draw_bitmap_region(aux,12*i,60,12,12,0,0,0);
		}
        for(i=0; i<3; i++){
			//blit(aux,sky[i],100*i,72,0,0,100,100);
			al_set_target_bitmap(sky[i]);
			al_draw_bitmap_region(aux,100*i,72,100,100,0,0,0);
		}
        for(i=0; i<8; i++)
		{
			//blit(aux,border[i],75+(20*i),30,0,0,20,20);
			al_set_target_bitmap(border[i]);
			al_draw_bitmap_region(aux,75+(20*i),30,20,20,0,0,0);
		}

		// Colorea invasores
		for(i=0; i < KTI_NLEVELS; i++)
		{
			invaders_colored[i] = ch->crea_copia_bitmap_coloreado(ship[4][0],colors[i][0],colors[i][1]);
		}
		// Colorea players
		for(i=0; i < 4; i++)
		for(j=0; j < 5; j++)
		{
			int sh = 0;
			 switch(gi->pl[i].clase){
                case SIGMA : sh=0; break;
                case OMEGA : sh=1; break;
                case BETA : sh=2; break;
             }
			 players_colored[i][j] = ch->crea_copia_bitmap_coloreado(ship[sh][j],gi->pl[i].c1,gi->pl[i].c2);
		}


        destruir_bitmap(aux);

        sshot=cargar_sample("KTI/shot.smp");
        smotor=cargar_sample("KTI/motor.smp");
        sexplos=cargar_sample("KTI/explos.smp");
        //bgm=load_mod("kti\\kti.jgm");
		bgm = cargar_musica("KTI/kti.mus");

        //play_sample(smotor,255,128,1000,TRUE);
		ALLEGRO_SAMPLE_ID retid;
		al_play_sample(smotor,1.0f,0.f,1.f,ALLEGRO_PLAYMODE_LOOP,&retid);
        /*set_mod_volume(80);
        play_mod(bgm,TRUE);*/
		al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());

        counter=0; lev=0; acab=FALSE; comple=FALSE;
        for(i=0; i<KTI_NLEVELS; i++) sequence[i]=i;
        for(i=0; i<100; i++){
                j=rand()%KTI_NLEVELS;
                k=rand()%KTI_NLEVELS;
                l=sequence[j];
                sequence[j]=sequence[k];
                sequence[k]=l;
        }
}
kti::~kti()
{
        int i,j;
        destruir_bitmap(display);
        for(i=0; i<5; i++)
        for(j=0; j<5; j++)
        destruir_bitmap(ship[i][j]);
        for(i=0; i<3; i++)
        destruir_bitmap(explos[i]);
        for(i=0; i<4; i++)
        destruir_bitmap(planet[i]);
        for(i=0; i<3; i++)
        destruir_bitmap(sky[i]);
        for(i=0; i<8; i++)
        destruir_bitmap(border[i]);
        destruir_bitmap(temp);
		for(i=0; i < KTI_NLEVELS; i++)
		{
			destruir_bitmap(invaders_colored[i]);
		}
		for(i=0; i < 4; i++)
		for(j=0; j < 5; j++)
		{
			destruir_bitmap(players_colored[i][j]);
		}

        al_destroy_sample(sshot);
        al_destroy_sample(smotor);
        al_destroy_sample(sexplos);
		al_detach_audio_stream(bgm);
		al_destroy_audio_stream(bgm);
        /*stop_mod();
        destroy_mod(bgm);*/
}
void kti::muestra(ALLEGRO_BITMAP *pant)
{
        int i,j,k;
        char s[20];
		//ALLEGRO_DISPLAY *pant = al_get_current_display();

		al_set_target_bitmap(display);
        //clear_to_color(display,0);
		al_clear_to_color(al_map_rgb(0,0,0));

        // Galaxia
        al_draw_bitmap(planet[planetc],planetx,int(planety),0);
        for(i=0; i<3; i++)
        for(j=0; j<3; j++){
        al_draw_bitmap(sky[2],100*i,(100*j)-100+int(scroll[2]),0);
        al_draw_bitmap(sky[1],100*i,(100*j)-100+int(scroll[1]),0);
        al_draw_bitmap(sky[0],100*i,(100*j)-100+int(scroll[0]),0);
        };

        // Disparos
        for(i=0; i<KTI_MAXSHOTS; i++)
                if(shots[i].use==TRUE)
                al_draw_bitmap(ship[shots[i].c][4],shots[i].x,shots[i].y,0);

        // Naves
        for(i=0; i<gi->n_players; i++){
                 /*switch(gi->pl[i].clase){
                        case SIGMA : j=0; break;
                        case OMEGA : j=1; break;
                        case BETA : j=2; break;
                 }
                 draw_sprite_colored(display,ship[j][0],shipx[i],KTI_SHIPY,
                        gi->pl[i].c2,gi->pl[i].c1,MEDLUZ,NULL,NULL,NULL);*/
				 al_draw_bitmap(players_colored[i][0],shipx[i],KTI_SHIPY,0);
                 al_draw_bitmap(players_colored[i][1+(int(scroll[2])%3)],shipx[i],KTI_SHIPY+12,0);
        }

        //Invasores
		//al_set_target_bitmap(temp);
        //clear_to_color(temp,0);
		//al_clear_to_color(al_map_rgb(0,0,0));
       /* draw_sprite_colored(temp,ship[4][0],0,0,colors[sequence[lev]][1],
                        colors[sequence[lev]][0],MEDLUZ,NULL,NULL,NULL);*/
		//al_draw_bitmap(ship[4][0],0,0,0);
		al_set_target_bitmap(display);
        for(i=0; i<KTI_NINVADERS; i++)
                 if(inv[i].state==0.0)
					 al_draw_rotated_bitmap(invaders_colored[lev],6,6,int(inv[i].x),int(inv[i].y),inv[i].angle+(PI/2),0);
					 //rotate_sprite(display,temp,int(inv[i].x),int(inv[i].y),ftofix((40.75*inv[i].angle)-64));
                 else if(inv[i].state<3.0)
                      al_draw_bitmap(explos[int(inv[i].state)],int(inv[i].x),int(inv[i].y),0);

        //Marco
        al_draw_bitmap(border[0],0,0,0);
        al_draw_bitmap(border[2],300,0,0);
        for(i=20; i<320-20; i+=20){
                al_draw_bitmap(border[1],i,0,0);
                al_draw_bitmap(border[6],i,180,0);
        }
        al_draw_bitmap(border[5],0,180,0);
        al_draw_bitmap(border[7],300,180,0);
        for(i=20; i<200-20; i+=20){
                al_draw_bitmap(border[3],0,i,0);
                al_draw_bitmap(border[4],300,i,0);
        }
        if(counter<KTI_TITLETIME){
                ch->texto(display," Kill The Invaders",40,80,32);
                sprintf(s," Wave %d-Get Ready!",lev+1);
                ch->texto(display,s,40,100,69);
        }
        if(counter>times[sequence[lev]]){
                sprintf(s,"     Kills:%d",kills());
                ch->texto(display,s,45,80,69);
                if(kills()==KTI_NINVADERS)
                        ch->texto(display,"     Perfect!",45,100,15);
                else ch->texto(display,"    GAME OVER!",45,100,32);
        }

		al_set_target_bitmap( pant );
		al_draw_bitmap(display,(al_get_bitmap_width(pant) - 320)/2,(al_get_bitmap_height(pant) - 200)/2,0);
}
void kti::actualiza(t_control *tc)
{
        controlm c;
        int i,j,k;
		ALLEGRO_SAMPLE_ID retid;

        // Galaxia
        scroll[0]+=1; if(scroll[0]>100) scroll[0]-=100;
        scroll[1]+=0.5; if(scroll[1]>100) scroll[1]-=100;
        scroll[2]+=0.25; if(scroll[2]>100) scroll[2]-=100;

        if(counter>times[sequence[lev]]+KTI_TITLETIME)
                if(kills()==KTI_NINVADERS){
                        lev++; counter=0;
                        if(lev==KTI_NLEVELS) {acab=TRUE; comple=TRUE;};
                }else acab=TRUE;

        if(counter==0) nuevo_nivel();
        counter++;

        planety+=0.25;
        if(planety>200){
                planety=-100; planetc=(planetc+1)%4; planetx=50+(rand()%200);
        };

        // Disparos
        for(i=0; i<KTI_MAXSHOTS; i++){
                if(shots[i].use==TRUE)
                if(shots[i].y>0){

                        shots[i].y-=3;
                        for(j=0; j<KTI_NINVADERS; j++){

                                if((inv[j].state==0.0) &&
                                   (abs(shots[i].x-int(inv[j].x))<12) &&
                                   (abs(shots[i].y-int(inv[j].y))<12)){
                                        inv[j].state+=KTI_EXPLVEL;
                                        shots[i].use=FALSE;
                                        al_play_sample(sexplos,1.0f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&retid);
                                };
                        };
                }
                else shots[i].use=FALSE;
        }
        // Jugadores
        for(i=0; i<gi->n_players; i++){

                 switch(gi->pl[i].clase){
                        case SIGMA : k=0; break;
                        case OMEGA : k=1; break;
                        case BETA  : k=2; break;
                 }

                if(sdelay[i]>0) sdelay[i]--;
                c.actualiza(tc[i]);
                if((c.izq()) && (shipx[i]>40)) shipx[i]--;
                if((c.der()) && (shipx[i]<268)) shipx[i]++;
                if((c.boton(0)) && (sdelay[i]==0)){

                        j=0;
                        while(shots[j].use==TRUE) j++;
                        shots[j].x=shipx[i]; shots[j].y=KTI_SHIPY-6; shots[j].c=k;
                        shots[j].use=TRUE; sdelay[i]=KTI_SDELAY;
                        al_play_sample(sshot,1.0f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE,&retid);
                }
        }
        // Aliens
        for(i=0; i<KTI_NINVADERS; i++)
                if(counter>=KTI_TITLETIME+(i*KTI_INVDELAY)) update_alien(i);
}
void kti::nuevo_nivel()
{
        int i;
        float xx,yy,aa;

        counter=0;
        switch(sequence[lev]){
                case 0 : xx=-20; yy=120; aa=0.0; break;
                case 1 : xx=330; yy=120; aa=PI; break;
                case 2 : xx=-20; yy=120; aa=0.0; break;
                case 3 : xx=-20; yy=100; aa=0.0; break;
                case 4 : xx=330; yy=140; aa=PI; break;
                case 5 : xx=-20; yy=140; aa=0.0; break;
                case 6 : xx=-20; yy=40; aa=0.0; break;
                case 7 : xx=330; yy=140; aa=PI; break;
                case 8 : xx=-20; yy=140; aa=0.0; break;
                case 9 : xx=330; yy=140; aa=PI; break;
        }
        for(i=0; i<KTI_NINVADERS; i++){
                inv[i].x=xx; inv[i].y=yy; inv[i].angle=aa; inv[i].state=0.0; inv[i].ct=0;
        }
}
void kti::update_alien(int i)
{
        float speed=2.0;
        // Dependiendeo de la wave, orientar los aliens
        switch(sequence[lev]){
                case 0 : inv[i].angle-=0.003; break;
                case 1 : inv[i].angle+=0.003; break;
                case 2 : if(inv[i].ct<50) inv[i].angle=0;
                         else if(inv[i].ct<90) inv[i].angle=3*PI/2;
                         else if(inv[i].ct<140) inv[i].angle=0;
                         else if(inv[i].ct<180) inv[i].angle=PI/2;
                         else inv[i].angle=0; break;
                case 3 : if(inv[i].ct<60) inv[i].angle=0;
                         else if(inv[i].ct<100) inv[i].angle=5*PI/4;
                         else if(inv[i].ct<200) inv[i].angle=0;
                         else if(inv[i].ct<240) inv[i].angle=3*PI/4;
                         else inv[i].angle=0;
                         break;
                case 4 : if((inv[i].ct>80) && (inv[i].ct<290)) inv[i].angle+=0.03;
                         break;
                case 5 : if((inv[i].ct>80) && (inv[i].ct<290)) inv[i].angle-=0.03;
                         break;
                case 6 : inv[i].angle=PI/2*sin(float(inv[i].ct)/20);
                         break;
                case 7 : if(inv[i].ct<270)
                         inv[i].angle=PI+(PI/2*sin(float(inv[i].ct)/20));
                         else inv[i].angle=PI/2*sin(float(inv[i].ct)/10);
                         break;
                case 8 : if(inv[i].ct<100) inv[i].angle-=0.01;
                         else if(inv[i].ct<140) inv[i].angle=PI/2;
                         else if(inv[i].ct==141) inv[i].angle=0.0;
                         else if(inv[i].ct>=300) inv[i].angle=0.0;
                         else inv[i].angle-=0.01;
                         break;
                case 9 : if(inv[i].ct<140) inv[i].angle+=0.005;
                         else if(inv[i].ct<220) inv[i].angle=0.0;
                         else if(inv[i].ct==221) inv[i].angle=PI/2;
                         else if(inv[i].ct>=300) inv[i].angle=PI;
                         else inv[i].angle+=0.005;
                         break;
        }
        if(inv[i].state==0.0){
                inv[i].x+=speed*cos(inv[i].angle);
                inv[i].y+=speed*sin(inv[i].angle);
                inv[i].ct++;
        }else if(inv[i].state<3.0) inv[i].state+=KTI_EXPLVEL;
}
int kti::kills()
{
        int i,s=0;
        for(i=0; i<KTI_NINVADERS; i++)
                 if(inv[i].state>=3.0) s++;
        return s;
}
bool kti::acabado()
{
        return acab;
}
bool kti::completo()
{
        return comple;
}
