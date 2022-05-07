
#include <allegro5/allegro_audio.h>

#define S_DISTCOEF 16
#define FILE_WAV_EXT "wav"
#define FILE_MOD_EXT "s3m"

enum s_sample {S_NULO=0, S_INTER, S_TELE, S_PUE1, S_PUE2, S_ESPA, S_PUNY, S_PIST, S_AMET, S_LANZ, S_ANIQ, S_DRAG, S_INFE, S_HIEL, S_RAYO, S_BOLT,
                S_EXBA, S_EXCO, S_EXFU, S_EXHI, S_EXEL, S_ITBO, S_ITES, S_ITMU, S_ITLL, S_ITEN, S_ITSP, S_TRAM, S_LLAM, S_ESCU, S_CORT, S_APOC, S_TRUE,
                S_LEVE, S_SECR};

class waves {

         private:
         // Personajes
         ALLEGRO_SAMPLE *s_warr[NSPR+1][3],
         // Ambiente
            *s_inter, *s_teleport, *s_puerta1, *s_puerta2, *s_espada, *s_puny, *s_a_sigma[6],
            *s_hielo, *s_rayo, *s_bolt,
            *s_explos[5], *s_itespada, *s_itbotiquin, *s_itmunicion, *s_itllave, *s_itenergia,
            *s_itspecial, *s_trampa, *s_apocal,
            *s_llama, *s_escudo, *s_corte, *s_trueno, *s_ambient[4], *s_secret, *s_level;
		 ALLEGRO_SAMPLE_INSTANCE *s_ambient_inst[4], *s_llama_inst;
         //JGMOD *bgm;
		  ALLEGRO_MIXER* mixer_1;
		  ALLEGRO_AUDIO_STREAM* bgm;
         bool load_ok, activo,llama, ambient[4];

         public:
         waves(char *);
         ~waves();
         ALLEGRO_SAMPLE *carga_sample(const char*, const char*);
         void stop_ambient_samples();
         void set_activo(bool);
         void load_wav_especificas(const char*, const char*);
         void load_bgm(const char*, const char*);
         void load_wav_warrior(const char*, const char*,byte);
         void play_wav_ambient(s_sample,int,float,bool);
         void play_wav_warrior(int,int,int,float,bool);
         void activa_wav_llama();
         void activa_wav_ambient(byte);
         void apaga_wav_llama();
         void apaga_wav_ambient(byte);
         void actualiza_wav_llama(int,float,bool);
         void actualiza_wav_ambient(byte,int,float,bool);
         bool wav_llama_activa();
         bool wav_ambient_activo(byte);
         void play_bgm();
         void stop_bgm();
         };

ALLEGRO_SAMPLE* cargar_sample(const char *file);
ALLEGRO_AUDIO_STREAM* cargar_musica(const char *file);

