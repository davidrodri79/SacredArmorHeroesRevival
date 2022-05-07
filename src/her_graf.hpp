//Requiere los tads siguientes
#include"her_bata.hpp"

#define FILE_IMG_EXT "pcx"

#define BALX 60
#define BALY 35
#define PARX 60
#define PARY 114
#define CUBX 60
#define CUBY 59
#define ESBX 60
#define ESBY 110
#define ESLX 22
#define ESLY 100
#define PUEX 30
#define PUEY 105
#define COLX 60
#define COLY 119
#define BARX 60
#define BARY 89
#define TUMX 90
#define TUMY 74
#define BULX 60
#define BULY 40
#define ESCX 30
#define ESCY 45
#define ITEX 36
#define ITEY 39

#define MARCADOR1X 86
#define MARCADOR1Y 54
#define MARCADOR2X 57
#define MARCADOR2Y 54
#define CONTILEX 40
#define CONTILEY 40

#define BBLAN 15
#define BROJO 32
#define BGRIS 19
#define BNEGR 31-7
#define BROSA 48
#define BNARA 56
#define BAMA1 64
#define BAMA2 72
#define BVER1 80
#define BVER2 90
#define BVER3 104
#define BAZUC 112
#define BAZUM 120
#define BAZUN 128
#define BAZUO 144
#define BLILA 164
#define BVIOL 180
#define BMAR1 192
#define BMAR2 200
#define BMAR3 210
#define BIRIS 232

#define BCO1 BNARA
#define BCO2 BGRIS

#define RAIZ2 1.4142136

#define MAXLUZ 13
#define MINLUZ 1
#define MEDLUZ int((MAXLUZ-MINLUZ)/2)+MINLUZ

#define FONT_SIGMA_AMMO 18
#define FONT_BETA_AMMO 21
#define FONT_SIGMA_WEAPON 240
#define FONT_BETA_WEAPON 198
#define FONT_OMEGA_WEAPON 146
#define FONT_OMEGA_SHIELD FONT_OMEGA_WEAPON+7

#define MAXSKINS 50


enum light_t {LSUELO, LCUBOI, LCUBOD, LCOLUI, LCOLUD};

class skin {

	public:
	  ALLEGRO_BITMAP *c_normal[2], *c_guardia[2], *c_punyo[2],
               *c_danyo[2], *c_magia[2], *c_ataque[2][3],
               *p_andar[2][3], *p_normal[2], *t_muerte1[3],
               *t_muerte2[3];

	  byte id_pers, color1, color2;

	  skin();
	  ~skin();

	  void carga_sprites(ALLEGRO_BITMAP *im);
};

class charset {

        private:
        ALLEGRO_BITMAP *half[2], *quart[4],
               //DECORADO
               *baldnorm[5][MAXLUZ], *balddan[2][3][MAXLUZ], *baldmort[2][3][MAXLUZ],
               *parednorm[6][2][MAXLUZ], *paredanim[3][3][2][MAXLUZ],
               *esqbase[MAXLUZ], *esqtras[MAXLUZ], *esqdel[MAXLUZ],
               *marclat1[MAXLUZ], *marclat2[MAXLUZ], *marcarc[MAXLUZ], *marcsup[MAXLUZ],
               *interr[2][MAXLUZ], *telep[3][MAXLUZ], *decornorm[6][MAXLUZ], *decoranim[3][3][MAXLUZ],
               *extra[MAXLUZ], *columna[3][MAXLUZ],
               *puertanorm[2][MAXLUZ], *puertallav[2][4][MAXLUZ], *barrera[3][MAXLUZ], *laserpost[2][MAXLUZ],
               *tirador[2][MAXLUZ],
               //WARRIORS
               *c_normal[NSPR+1][2], *c_guardia[NSPR+1][2], *c_punyo[NSPR+1][2],
               *c_danyo[NSPR+1][2], *c_magia[NSPR+1][2], *c_ataque[NSPR+1][2][3],
               *p_andar[NSPR+1][2][3], *p_normal[NSPR+1][2], *t_muerte1[NSPR+1][3],
               *t_muerte2[NSPR+1][3],
               //EXPLOSIONES
               *explosi[5][3], *llamas[4], *llama2[3],
               //BALAS
               *balas[6][6],
               //ARMAS SIGMA
               *a_sigma[7][2],
               //ARMAS OMEGA
               *a_omega[7][6], *escudos[7][2],
               //BAST‡N
               *baston[6],
               //ITEMS RECOGIBLES
               *botiquin, *llaves[4][3], *cbalas, *cohetes, *napalm[3], *mana[3][3],
               *invul[3], *berserk[3], *invis[3], *repuls[3], *health[3], *bolt, *mapacomp[3],
               *veneno[3], *shadows[3], *divino[3], *apocalip, *gema[3][3],
               //SPRITES
               /**luces[2][3][2],*/ *aura[3], *sombra, *hielo[3][MAXLUZ], *capsulas[3], *laser[2][3], *gota[4],
               //FUENTE Y OTROS
               *fuente[256], *marcador[2], *consola[12], *minicara[3], *minicaras_coloreadas[MAXPL];

        //COLOR_MAP
		char  *ttrans, *tluz, *tsombra;
        //RGB *paleta;
        byte *matrizluz[MSIZEY], antorchas[200][2], ilum_trans[3], nantor;
        int tamx, tamy;

		//Skins de personaje
		skin *skins[MAXSKINS];
		int congelado_skin;


        public:
        charset(ALLEGRO_BITMAP*,int,int);
        void nuevo_display(int,int);
        ~charset();
        //RGB *dev_paleta();
        void carga_bitmaps_batalla(batalla*);
        void crea_variantes_ilum(ALLEGRO_BITMAP *b[], int alpha_pixel_x = 0, int alpha_pixel_y = 0);
        void carga_escenario(char*,batalla*);
		ALLEGRO_BITMAP* crea_copia_bitmap_coloreado(ALLEGRO_BITMAP *im, byte col1, byte col2);
		int crea_skin_personaje(char *file, byte id_pers, byte col1, byte col2);
		void destruye_skin(int);
		void destruye_todas_skins();
        void carga_guerrero(char*,int);
        void carga_sprites(batalla*);
        void carga_explosiones(char*);
        void carga_balas(char*);
        void carga_armas_sigma(char*);
        void carga_armas_omega(char*);
        void carga_escudos(char*);
        void carga_items(char*);
        void carga_fuente(char*);
        void crea_tablas_color(nivel*);
        void muestra_pantalla(ALLEGRO_BITMAP*,batalla*);
        void muestra(ALLEGRO_BITMAP*,batalla*,int,int,float,float,int target_player);
        void muestra_baldosa(ALLEGRO_BITMAP*,mapa*,bald,bald,int,int);
        void muestra_decorado(ALLEGRO_BITMAP*,mapa*,bald,bald,float,float,int,int);
        void muestra_guerrero(ALLEGRO_BITMAP*,warrior*,float,float,int,int,bool);
        void muestra_hud_guerrero(ALLEGRO_BITMAP*,warrior*,float,float,int,int);
        void muestra_bala(ALLEGRO_BITMAP*,bala*,byte,float,float,int,int);
        void muestra_explos(ALLEGRO_BITMAP*,explos*,float,float,int,int);
        void muestra_gota(ALLEGRO_BITMAP*,float,float,int,int,float,float,float,int);
        void muestra_item(ALLEGRO_BITMAP*,item*,float,float,int,int,bool,byte);
        void muestra_luces(ALLEGRO_BITMAP*,mapa*,bald,bald,int,int,int,int);
        void texto(ALLEGRO_BITMAP*,const char*,int,int,byte,bool sombra = false);
        void muestra_mensajes(ALLEGRO_BITMAP*,batalla*,byte);
        void muestra_marcador(ALLEGRO_BITMAP*,warrior*,int,int);
		void muestra_ranking_deathmatch(ALLEGRO_BITMAP *pant, batalla *b);
        void muestra_datos_personaje(ALLEGRO_BITMAP*,int,warrior*,int,int);
        void muestra_consola(ALLEGRO_BITMAP*,batalla*);
        void ventana(ALLEGRO_BITMAP *, int, int, int, int, byte, byte, bool);
        void limpia_matriz_luz(byte);
        void inserta_aura_luz(bald,bald,byte);
        void inserta_aura_oscu(bald,bald,byte);
        void inserta_luz(bald,bald,byte,bool);
        void inserta_oscu(bald,bald,byte,bool);
        byte consulta_luz(bald,bald);
        void aura_estrella(bald,bald);
        void aplica_luz_batalla(batalla*);
        void muestra_mapa(ALLEGRO_BITMAP*,batalla*);
        void muestra_casilla_mapa(int,int,ALLEGRO_BITMAP*,batalla*,int,int,int);
        //void muestra_puntero(BITMAP*,int,int);
        void set_tipo_transp(int i);

        };


void rectfill(ALLEGRO_BITMAP *bmp,int x1, int y1, int x2, int y2, unsigned char color);
void circlefill(ALLEGRO_BITMAP *bmp,int x1, int y1, float r, unsigned char color);
//void draw_sprite_colored(BITMAP *bmp, BITMAP *sprite, int x, int y, char, char, byte, byte[], COLOR_MAP*, COLOR_MAP*);
//void draw_sprite_colored_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, char, char, byte, byte[], COLOR_MAP*, COLOR_MAP*);
//void draw_trans_sprite2(BITMAP *bmp, BITMAP *sprite, int x, int y, COLOR_MAP*);
void draw_sprite_ilum(ALLEGRO_BITMAP *bmp, ALLEGRO_BITMAP *sprite, int x, int y, byte, byte[], char*, char*,bool flip = false);
void draw_sprite_ilum_h_flip(ALLEGRO_BITMAP *bmp, ALLEGRO_BITMAP *sprite, int x, int y, byte, byte[], char*, char*);
//void draw_sprite_colored_rand(BITMAP *bmp, BITMAP *sprite, int x, int y, char, char, int);
//void draw_sprite_colored_rand_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, char, char, int);
