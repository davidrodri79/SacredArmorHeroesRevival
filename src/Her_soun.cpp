#include"her_tipo.hpp"
#include<allegro5/allegro.h>
//#include<jgmod.h>
#include"her_soun.hpp"

#include <allegro5/allegro_acodec.h>

ALLEGRO_SAMPLE_ID sample_ret_id;

ALLEGRO_SAMPLE* cargar_sample(const char *file)
{
    ALLEGRO_FILE *fp = al_fopen(file, "rb");
    ALLEGRO_SAMPLE *ptr = 0;
    if(fp != NULL)
    {
        ptr = al_load_sample_f(fp, ".wav");
        al_fclose(fp);
    }
	return ptr;
}

ALLEGRO_AUDIO_STREAM* cargar_musica(const char *file)
{
    ALLEGRO_FILE *fp = al_fopen(file, "rb");
    ALLEGRO_AUDIO_STREAM *ptr = 0;
    if(fp != NULL)
    {
        ptr = al_load_audio_stream_f(fp, ".s3m", 4, 2048);
        al_fclose(fp);
    }
	return ptr;
}

// Inicializa la estructura con los samples del directorio "dir", sin cargar
// los de los personajes espec¡ficos
waves::waves(char *dir)
{
   char s[60];
   int i,j;
   activo=TRUE;
   llama=FALSE;

   for(i = 0; i < 4; i++)
	   s_ambient_inst[i] = 0;
   s_llama_inst = 0;

	if(!al_is_audio_installed()) { load_ok = false; return; }
	load_ok = true;


   for(i=0; i<N_SMP_AMBI; i++) ambient[i]=FALSE;
   for(i=0; i<NSPR+1; i++)
   for(j=0; j<3; j++)
           s_warr[i][j]=NULL;
   for(j=0; j<6; j++)
           s_a_sigma[j]=NULL;
   for(j=0; j<5; j++)
           s_explos[j]=NULL;
   sprintf(s,"%s/teleport.%s",dir,FILE_WAV_EXT);
   s_teleport=cargar_sample(s);
   sprintf(s,"%s/espada.%s",dir,FILE_WAV_EXT);
   s_espada=cargar_sample(s);
   sprintf(s,"%s/punch.%s",dir,FILE_WAV_EXT);
   s_puny=cargar_sample(s);
   sprintf(s,"%s/llama.%s",dir,FILE_WAV_EXT);
   s_llama=cargar_sample(s);
   if(s_llama) {
		s_llama_inst = al_create_sample_instance( s_llama );
		al_attach_sample_instance_to_mixer(s_llama_inst, al_get_default_mixer());
		al_set_sample_instance_playmode(s_llama_inst, ALLEGRO_PLAYMODE_LOOP);
   }
   sprintf(s,"%s/itespada.%s",dir,FILE_WAV_EXT);
   s_itespada=cargar_sample(s);
   sprintf(s,"%s/itbotiqu.%s",dir,FILE_WAV_EXT);
   s_itbotiquin=cargar_sample(s);
   sprintf(s,"%s/itmunici.%s",dir,FILE_WAV_EXT);
   s_itmunicion=cargar_sample(s);
   sprintf(s,"%s/itllave.%s",dir,FILE_WAV_EXT);
   s_itllave=cargar_sample(s);
   sprintf(s,"%s/itenergi.%s",dir,FILE_WAV_EXT);
   s_itenergia=cargar_sample(s);
   sprintf(s,"%s/itspecia.%s",dir,FILE_WAV_EXT);
   s_itspecial=cargar_sample(s);
   sprintf(s,"%s/escudo.%s",dir,FILE_WAV_EXT);
   s_escudo=cargar_sample(s);
   sprintf(s,"%s/corte.%s",dir,FILE_WAV_EXT);
   s_corte=cargar_sample(s);
   sprintf(s,"%s/apocalip.%s",dir,FILE_WAV_EXT);
   s_apocal=cargar_sample(s);
   sprintf(s,"%s/trueno.%s",dir,FILE_WAV_EXT);
   s_trueno=cargar_sample(s);
   sprintf(s,"%s/pistola.%s",dir,FILE_WAV_EXT);
   s_a_sigma[0]=cargar_sample(s);
   sprintf(s,"%s/ametrall.%s",dir,FILE_WAV_EXT);
   s_a_sigma[1]=cargar_sample(s);
   sprintf(s,"%s/lanzacoh.%s",dir,FILE_WAV_EXT);
   s_a_sigma[2]=cargar_sample(s);
   sprintf(s,"%s/aniquila.%s",dir,FILE_WAV_EXT);
   s_a_sigma[3]=cargar_sample(s);
   sprintf(s,"%s/dragon.%s",dir,FILE_WAV_EXT);
   s_a_sigma[4]=cargar_sample(s);
   sprintf(s,"%s/inferno.%s",dir,FILE_WAV_EXT);
   s_a_sigma[5]=cargar_sample(s);
   sprintf(s,"%s/hielo.%s",dir,FILE_WAV_EXT);
   s_hielo=cargar_sample(s);
   sprintf(s,"%s/rayo.%s",dir,FILE_WAV_EXT);
   s_rayo=cargar_sample(s);
   sprintf(s,"%s/bolt.%s",dir,FILE_WAV_EXT);
   s_bolt=cargar_sample(s);
   sprintf(s,"%s/secret.%s",dir,FILE_WAV_EXT);
   s_secret=cargar_sample(s);
   sprintf(s,"%s/level.%s",dir,FILE_WAV_EXT);
   s_level=cargar_sample(s);
   sprintf(s,"%s/exp_bala.%s",dir,FILE_WAV_EXT);
   s_explos[0]=cargar_sample(s);
   sprintf(s,"%s/exp_cohe.%s",dir,FILE_WAV_EXT);
   s_explos[1]=cargar_sample(s);
   sprintf(s,"%s/exp_fueg.%s",dir,FILE_WAV_EXT);
   s_explos[2]=cargar_sample(s);
   sprintf(s,"%s/exp_hiel.%s",dir,FILE_WAV_EXT);
   s_explos[3]=cargar_sample(s);
   sprintf(s,"%s/exp_elec.%s",dir,FILE_WAV_EXT);
   s_explos[4]=cargar_sample(s);

}
waves::~waves()
{

   if(!load_ok) return;

   int i,j;
   al_destroy_sample(s_inter);
   al_destroy_sample(s_teleport);
   al_destroy_sample(s_puerta1);
   al_destroy_sample(s_puerta2);
   al_destroy_sample(s_espada);
   al_destroy_sample(s_puny);
   al_destroy_sample(s_itespada);
   al_destroy_sample(s_itbotiquin);
   al_destroy_sample(s_itmunicion);
   al_destroy_sample(s_itenergia);
   al_destroy_sample(s_itllave);
   al_destroy_sample(s_itspecial);
   al_destroy_sample(s_trampa);
   al_stop_sample_instance(s_llama_inst);
   al_destroy_sample_instance(s_llama_inst);
   al_destroy_sample(s_llama);
   al_destroy_sample(s_escudo);
   al_destroy_sample(s_corte);
   al_destroy_sample(s_apocal);
   al_destroy_sample(s_trueno);
   al_destroy_sample(s_level);
   al_destroy_sample(s_secret);
   for(i=0; i<N_SMP_AMBI; i++)
           if(s_ambient[i]!=NULL) {al_stop_sample_instance(s_ambient_inst[i]);
								   al_destroy_sample_instance(s_ambient_inst[i]);
                                   al_destroy_sample(s_ambient[i]);

									};
   for(i=0; i<6; i++)
           if(s_a_sigma[i]!=NULL) al_destroy_sample(s_a_sigma[i]);
   al_destroy_sample(s_hielo);
   al_destroy_sample(s_rayo);
   al_destroy_sample(s_bolt);
   for(i=0; i<5; i++)
           if(s_explos[i]!=NULL) al_destroy_sample(s_explos[i]);
   for(i=0; i<NSPR+1; i++)
   for(j=0; j<3; j++)
           if(s_warr[i][j]!=NULL) al_destroy_sample(s_warr[i][j]);
   //destroy_mod(bgm);
   al_destroy_audio_stream(bgm);

}
void waves::stop_ambient_samples()
{

   for(int i=0; i<N_SMP_AMBI; i++)
           if(ambient[i]) al_stop_sample_instance(s_ambient_inst[i]);

}
ALLEGRO_SAMPLE *waves::carga_sample(const char *dir, const char *file)
{

   FILE *fp;
   char s[60];

   sprintf(s,"%s/nulo.%s",dir,FILE_WAV_EXT);

   fp=fopen(file,"r");
   if(fp==NULL) return cargar_sample(s);
   else { fclose(fp); sprintf(s, "%s", file);  return cargar_sample(s); };

}
// Activa o desactiva los sonidos
void waves::set_activo(bool a)
{
      activo=a;
}
// Carga waves asociadas al decorado en cuesti¢n
void waves::load_wav_especificas(const char *dir, const char *name)
{
   if(!load_ok) return;

   char s[60];
   sprintf(s,"%s/%s0.%s",dir,name,FILE_WAV_EXT);
   s_inter=carga_sample(dir,s);
   sprintf(s,"%s/%s1.%s",dir,name,FILE_WAV_EXT);
   s_puerta1=carga_sample(dir,s);
   sprintf(s,"%s/%s2.%s",dir,name,FILE_WAV_EXT);
   s_puerta2=carga_sample(dir,s);
   sprintf(s,"%s/%s3.%s",dir,name,FILE_WAV_EXT);
   s_trampa=carga_sample(dir,s);
   for(int i=0; i<N_SMP_AMBI; i++){
        sprintf(s,"%s/%s%d.%s",dir,name,i+4,FILE_WAV_EXT);
        s_ambient[i]=carga_sample(dir,s);
		if(s_ambient[i]){
			s_ambient_inst[i] = al_create_sample_instance( s_ambient[i] );
			if(s_ambient_inst[i])
			{
				if(al_get_default_mixer()){
					al_attach_sample_instance_to_mixer(s_ambient_inst[i], al_get_default_mixer());
					al_set_sample_instance_playmode(s_ambient_inst[i], ALLEGRO_PLAYMODE_LOOP);
				}
			}
		}
   };

}
// Carga la m£sica de fondo
void waves::load_bgm(const char *dir,const char *file)
{
    char s[60];
    sprintf(s,"%s/%s.%s",dir,file,FILE_MOD_EXT);
    //bgm=load_mod(s);
	bgm = cargar_musica(s);
}
// Carga los samples del warrior de nombre name en el directorio dir y los
// guarda en el personaje c de los NSPR disponibles.
// Los wavs cargados ser n name.wa0, name.wa1 y name.wa2 (queja, muerte, y
// destrucci¢n)
void waves::load_wav_warrior(const char *dir, const char *name, byte c)
{

   char s[60];

   for(int i=0; i<3; i++){
      sprintf(s,"%s/%s%d.%s",dir,name,i,FILE_WAV_EXT);
      s_warr[c][i]=carga_sample(dir,s);};

}
// Reproduce un sample de los sonidos ambiente est ndar, aplicando est‚reo y
// volumen en funci¢n de su posici¢n en la batalla
void waves::play_wav_ambient(s_sample i,int posx,float dist,bool single)
{

   int pan=int((640/2)-posx)+128;
   int vol=int(255-(dist*S_DISTCOEF));
   ALLEGRO_SAMPLE *smp=NULL;

   if(pan<0) pan=0;
   if(pan>255) pan=255;
   if((vol<=0) && (single)) return;
   if(vol>255) vol=255;
   switch(i){
         case S_INTER : smp=s_inter; break;
         case S_TELE  : smp=s_teleport; break;
         case S_PUE1  : smp=s_puerta1; break;
         case S_PUE2  : smp=s_puerta2; break;
         case S_ESPA  : smp=s_espada; break;
         case S_PUNY  : smp=s_puny; break;
         case S_TRAM  : smp=s_trampa; break;
         case S_ITES  : smp=s_itespada; break;
         case S_ITMU  : smp=s_itmunicion; break;
         case S_ITBO  : smp=s_itbotiquin; break;
         case S_ITLL  : smp=s_itllave; break;
         case S_ITEN  : smp=s_itenergia; break;
         case S_ITSP  : smp=s_itspecial; break;
         case S_ESCU  : smp=s_escudo; break;
         case S_CORT  : smp=s_corte; break;
         case S_APOC  : smp=s_apocal; break;
         case S_TRUE  : smp=s_trueno; break;
         case S_PIST  : smp=s_a_sigma[0]; break;
         case S_AMET  : smp=s_a_sigma[1]; break;
         case S_LANZ  : smp=s_a_sigma[2]; break;
         case S_ANIQ  : smp=s_a_sigma[3]; break;
         case S_DRAG  : smp=s_a_sigma[4]; break;
         case S_INFE  : smp=s_a_sigma[5]; break;
         case S_HIEL  : smp=s_hielo; break;
         case S_RAYO  : smp=s_rayo; break;
         case S_BOLT  : smp=s_bolt; break;
         case S_LEVE  : smp=s_level; break;
         case S_SECR  : smp=s_secret; break;
         case S_EXBA  : smp=s_explos[0]; break;
         case S_EXCO  : smp=s_explos[1]; break;
         case S_EXFU  : smp=s_explos[2]; break;
         case S_EXHI  : smp=s_explos[3]; break;
         case S_EXEL  : smp=s_explos[4]; break;

   }
   if((smp!=NULL) && (activo)){
      if(single==TRUE) al_play_sample(smp,vol/255.f,-(pan-128)/128.f,1.f,ALLEGRO_PLAYMODE_ONCE, &sample_ret_id);//play_sample(smp,int(vol),pan,1000,FALSE);
	  else al_play_sample(smp,1.f,0.f,1.f,ALLEGRO_PLAYMODE_ONCE, &sample_ret_id);}//play_sample(smp,255,128,1000,FALSE);};

}
// Reproduce un sample de uno de los personajes, aplicando est‚reo y
// volumen en funci¢n de su posici¢n en la batalla
void waves::play_wav_warrior(int p,int s,int posx,float dist,bool single)
{

   int pan=int((640/2)-posx)+128;
   int vol=int(255-(dist*S_DISTCOEF));

   if(pan<0) pan=0;
   if(pan>255) pan=255;
   if((vol<=0) && (single)) return;
   if(vol>255) vol=255;
   if(activo)
   if(single==TRUE) al_play_sample(s_warr[p][s],vol/255.f,-(pan-128)/128.f,(900+(50*(rand()%3)))/1000.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);//play_sample(s_warr[p][s],int(vol),pan,900+(50*(rand()%3)),FALSE);
   else  al_play_sample(s_warr[p][s],1.f,0.f,(900+(50*(rand()%3)))/1000.f,ALLEGRO_PLAYMODE_ONCE,&sample_ret_id);//play_sample(s_warr[p][s],255,128,900+(50*(rand()%3)),FALSE);

}
// Rutinas para el sonido del crepitar del fuego de las llamas
void waves::activa_wav_llama()
{

    if(activo)
    //play_sample(s_llama,255,128,1000,TRUE);
		al_play_sample_instance(s_llama_inst);
    llama=TRUE;

}
void waves::activa_wav_ambient(byte c)
{

    if(c<N_SMP_AMBI && s_ambient_inst[c]){
        if(activo)al_play_sample_instance(s_ambient_inst[c]);//play_sample(s_ambient[c],255,128,1000,TRUE);
        ambient[c]=TRUE;};

}
void waves::apaga_wav_llama()
{
	 llama=FALSE;
    if(load_ok && activo)
    al_stop_sample_instance(s_llama_inst);


}
void waves::apaga_wav_ambient(byte c)
{
	if(c<N_SMP_AMBI) ambient[c]=FALSE;

    if(load_ok && s_ambient_inst[c]){
        if(activo)al_stop_sample_instance(s_ambient_inst[c]);
    };

}
void waves::actualiza_wav_llama(int posx,float dist,bool single)
{

   int pan=int((640/2)-posx)+128;
   int vol=int(255-(dist*S_DISTCOEF));

   if(!load_ok) return;

   if(pan<0) pan=0;
   if(pan>255) pan=255;
   if(vol<0) vol=0;
   if(vol>255) vol=255;
    if((single==TRUE) && (activo))
   {
	   al_set_sample_instance_gain(s_llama_inst,vol/255.f);
	   al_set_sample_instance_pan(s_llama_inst,-(pan-128)/128.f);
	   //adjust_sample(s_ambient[c],int(vol),pan,1000,TRUE);
   }
   else
   {
	   al_set_sample_instance_gain(s_llama_inst,1.f);
	   al_set_sample_instance_pan(s_llama_inst,0.f);
   }

}
void waves::actualiza_wav_ambient(byte c,int posx,float dist,bool single)
{
	if(!load_ok) return;
	if (!s_ambient_inst[c]) return;

   int pan=int((640/2)-posx)+128;
   int vol=int(255-(dist*S_DISTCOEF));

   if(pan<0) pan=0;
   if(pan>255) pan=255;
   if(vol<0) vol=0;
   if(vol>255) vol=255;
   if((single==TRUE) && (activo))
   {
	   al_set_sample_instance_gain(s_ambient_inst[c],vol/255.f);
	   al_set_sample_instance_pan(s_ambient_inst[c],-(pan-128)/128.f);
	   //adjust_sample(s_ambient[c],int(vol),pan,1000,TRUE);
   }
   else
   {
	   al_set_sample_instance_gain(s_ambient_inst[c],1.f);
	   al_set_sample_instance_pan(s_ambient_inst[c],0.f);
   }

}
bool waves::wav_llama_activa()
{
    return llama;
}
bool waves::wav_ambient_activo(byte c)
{
    return ambient[c];
}
// Comienza a tocar la m£sica de fondo
void waves::play_bgm()
{
    //set_mod_volume(40);
    //play_mod(bgm,TRUE);
	al_set_audio_stream_playmode(bgm, ALLEGRO_PLAYMODE_LOOP);
	al_attach_audio_stream_to_mixer(bgm, al_get_default_mixer());
}
// Detiene la m£sica de fondo
void waves::stop_bgm()
{
    //stop_mod();
	al_detach_audio_stream(bgm);
}

