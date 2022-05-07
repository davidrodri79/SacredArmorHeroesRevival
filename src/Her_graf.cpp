
#include"her_tipo.hpp"
#include<math.h>
#include<string.h>
#include<allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include"her_graf.hpp"
#include"profiler.h"

#include <map>


ALLEGRO_BITMAP *silueta = 0;

unsigned char dp2palette[256][3] = {
	{0, 0, 0},			//0
	{255, 255, 255},
	{0, 170, 0},
	{0, 170, 170},
	{170, 0, 0},
	{170, 0, 170},
	{170, 85, 0},
	{170, 170, 170},
	{85, 85, 85},
	{85, 85, 255},
	{85, 255, 85},		//10
	{85, 255, 255},
	{255, 85, 85},
	{255, 85, 255},
	{255, 255, 85},
	{255, 255, 255},
	{239, 239, 239},
	{223, 223, 223},
	{211, 211, 211},
	{195, 195, 195},
	{183, 183, 183},	//20
	{171, 171, 171},
	{155, 155, 155},
	{143, 143, 143},
	{127, 127, 127},
	{115, 115, 115},
	{103, 103, 103},
	{87, 87, 87},
	{75, 75, 75},
	{59, 59, 59},
	{47, 47, 47},		//30
	{35, 35, 35},
	{255, 0, 0},
	{239, 0, 0},
	{227, 0, 0},
	{215, 0, 0},
	{203, 0, 0},
	{191, 0, 0},
	{179, 0, 0},
	{167, 0, 0},
	{155, 0, 0},		//40
	{139, 0, 0},
	{127, 0, 0},
	{115, 0, 0},
	{103, 0, 0},
	{91, 0, 0},
	{79, 0, 0},
	{64, 0, 0},
	{255, 218, 218},
	{255, 186, 186},
	{255, 159, 159},	//50
	{255, 127, 127},
	{255, 95, 95},
	{255, 64, 64},
	{255, 32, 32},
	{255, 0, 0},
	{252, 168, 92},
	{252, 152, 64},
	{252, 136, 32},
	{252, 120, 0},
	{228, 108, 0},
	{204, 96, 0},
	{180, 84, 0},
	{156, 76, 0},
	{252, 252, 216},
	{252, 252, 184},
	{252, 252, 156},
	{252, 252, 124},
	{252, 248, 92},
	{252, 244, 64},
	{252, 244, 32},
	{252, 244, 0},
	{228, 216, 0},
	{204, 196, 0},
	{180, 172, 0},
	{156, 156, 0},
	{132, 132, 0},
	{112, 108, 0},
	{88, 84, 0},
	{64, 64, 0},
	{208, 252, 92},
	{196, 252, 64},
	{180, 252, 32},
	{160, 252, 0},
	{144, 228, 0},
	{128, 204, 0},
	{116, 180, 0},
	{96, 156, 0},
	{216, 252, 216},
	{188, 252, 184},
	{156, 252, 156},
	{128, 252, 124},
	{96, 252, 92},
	{64, 252, 64},
	{32, 252, 32},
	{0, 252, 0},
	{0, 255, 0},
	{0, 239, 0},
	{0, 227, 0},
	{0, 215, 0},
	{7, 203, 0},
	{7, 191, 0},
	{7, 179, 0},
	{7, 167, 0},
	{7, 155, 0},
	{7, 139, 0},
	{7, 127, 0},
	{7, 115, 0},
	{7, 103, 0},
	{7, 91, 0},
	{7, 79, 0},
	{4, 64, 0},
	{218, 255, 255},
	{184, 252, 252},
	{156, 252, 252},
	{124, 252, 248},
	{92, 252, 252},
	{64, 252, 252},
	{32, 252, 252},
	{0, 252, 252},
	{0, 228, 228},
	{0, 204, 204},
	{0, 180, 180},
	{0, 156, 156},
	{0, 132, 132},
	{0, 112, 112},
	{0, 88, 88},
	{0, 64, 64},
	{92, 188, 252},
	{64, 176, 252},
	{32, 168, 252},
	{0, 156, 252},
	{0, 140, 228},
	{0, 124, 204},
	{0, 108, 180},
	{0, 92, 156},
	{218, 218, 255},
	{186, 191, 255},
	{159, 159, 255},
	{127, 128, 255},
	{95, 96, 255},
	{64, 64, 255},
	{32, 37, 255},
	{0, 5, 255},
	{0, 0, 255},
	{0, 0, 239},
	{0, 0, 227},
	{0, 0, 215},
	{0, 0, 203},
	{0, 0, 191},
	{0, 0, 179},
	{0, 0, 167},
	{0, 0, 155},
	{0, 0, 139},
	{0, 0, 127},
	{0, 0, 115},
	{0, 0, 103},
	{0, 0, 91},
	{0, 0, 79},
	{0, 0, 64},
	{240, 218, 255},
	{229, 186, 255},
	{218, 159, 255},
	{208, 127, 255},
	{202, 95, 255},
	{191, 64, 255},
	{181, 32, 255},
	{170, 0, 255},
	{154, 0, 229},
	{128, 0, 207},
	{117, 0, 181},
	{96, 0, 159},
	{80, 0, 133},
	{69, 0, 112},
	{53, 0, 90},
	{42, 0, 64},
	{255, 218, 255},
	{255, 186, 255},
	{255, 159, 255},
	{255, 127, 255},
	{255, 95, 255},
	{255, 64, 255},
	{255, 32, 255},
	{255, 0, 255},
	{224, 0, 229},
	{202, 0, 207},
	{181, 0, 181},
	{159, 0, 159},
	{133, 0, 133},
	{111, 0, 112},
	{90, 0, 90},
	{64, 0, 64},
	{255, 234, 223},
	{255, 224, 208},
	{255, 218, 197},
	{255, 213, 191},
	{255, 207, 176},
	{255, 197, 165},
	{255, 191, 159},
	{255, 186, 144},
	{255, 176, 128},
	{255, 165, 112},
	{255, 159, 96},
	{240, 149, 95},
	{234, 143, 90},
	{223, 138, 85},
	{208, 128, 80},
	{202, 127, 79},
	{191, 122, 74},
	{181, 112, 69},
	{170, 106, 64},
	{160, 101, 63},
	{159, 96, 58},
	{144, 95, 53},
	{138, 90, 48},
	{128, 80, 47},
	{117, 79, 42},
	{111, 74, 37},
	{95, 64, 32},
	{85, 63, 31},
	{74, 58, 26},
	{64, 48, 26},
	{58, 47, 21},
	{42, 32, 15},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{0, 0, 0},
	{199, 43, 43},
	{199, 79, 43},
	{199, 119, 43},
	{199, 159, 43},
	{199, 199, 43},
	{159, 199, 43},
	{119, 199, 43},
	{79, 199, 43},
	{43, 199, 51},
	{43, 199, 95},
	{43, 199, 139},
	{43, 199, 183},
	{43, 171, 199},
	{43, 127, 199},
	{43, 83, 199},
	{47, 43, 199},
	{91, 43, 199},
	{135, 43, 199},
	{179, 43, 199},
	{199, 43, 175},
	{199, 43, 131},
	{199, 43, 87},
	{199, 43, 43},
	{255, 255, 255}
};

ALLEGRO_COLOR get_dp2_color(unsigned char idx)
{
	return al_map_rgb(dp2palette[idx][0],dp2palette[idx][1],dp2palette[idx][2]);
}

inline void set_blender_for_transparent()
{
	al_set_blender(ALLEGRO_ADD,ALLEGRO_ALPHA,ALLEGRO_INVERSE_ALPHA);
}

inline void restore_default_blender()
{
	al_set_blender(ALLEGRO_ADD,ALLEGRO_ONE,ALLEGRO_INVERSE_ALPHA);
}

//#define DEBUG_BITMAPS_MEMORY

#ifdef DEBUG_BITMAPS_MEMORY
struct TReservaInfo
{
	int w;
	int h;
	int id;
};

long total_reservas = 0;
std::map<int, TReservaInfo> reservas_bitmaps;
#endif

ALLEGRO_BITMAP *crear_bitmap(int w, int h)
{
	ALLEGRO_BITMAP *ptr = al_create_bitmap(w, h);
	al_set_target_bitmap(ptr);
    al_clear_to_color(al_map_rgba(0,0,0,0));
#ifdef DEBUG_BITMAPS_MEMORY
	TReservaInfo r;
	r.w = w; r.h = h; r.id = total_reservas;
	reservas_bitmaps[(int)ptr] = r;
	total_reservas++;
#endif
	return ptr;
}

ALLEGRO_BITMAP *cargar_bitmap(const char* f)
{
    ALLEGRO_FILE *fp = al_fopen(f, "rb");
    ALLEGRO_BITMAP *ptr = 0;
    if(fp != NULL)
    {
        ptr = al_load_bitmap_f(fp, NULL);
		//ptr = al_load_bitmap(f);
#ifdef DEBUG_BITMAPS_MEMORY
        TReservaInfo r;
        r.w = -1; r.h = -1; r.id = total_reservas;
        reservas_bitmaps[(int)ptr] = r;
        total_reservas++;
#endif
        al_fclose(fp);
    }
	return ptr;
}

void destruir_bitmap(ALLEGRO_BITMAP *bmp)
{
#ifdef DEBUG_BITMAPS_MEMORY
	reservas_bitmaps.erase((int)bmp);
#endif
	al_destroy_bitmap(bmp);
}

void muestra_reservas()
{
#ifdef DEBUG_BITMAPS_MEMORY
	int num = 0;
	fprintf(stderr," RESERVAS ===================================================\n");
	for (std::map<int, TReservaInfo>::iterator it = reservas_bitmaps.begin(); it != reservas_bitmaps.end(); it++)
	{
		fprintf(stderr, "%d: Bitmap %dx%d: Pointer %d\n", it->second.id, it->second.w, it->second.h, it->first);
		num++;
	}
#endif
}

skin::skin()
{
	int i,j,k;
     for(i=0; i<2; i++){ c_normal[i]=crear_bitmap(CUBX,CUBY);
                          c_guardia[i]=crear_bitmap(CUBX,CUBY);
                          c_punyo[i]=crear_bitmap(CUBX,CUBY);
                          c_danyo[i]=crear_bitmap(CUBX,CUBY);
                          c_magia[i]=crear_bitmap(CUBX,CUBY); };

      for(i=0; i<2; i++)
      for(k=0; k<3; k++){  c_ataque[i][k]=crear_bitmap(CUBX,CUBY);
                           p_andar[i][k]=crear_bitmap(CUBX,CUBY); };

      for(i=0; i<2; i++) p_normal[i]=crear_bitmap(CUBX,CUBY);

      for(i=0; i<2; i++){ t_muerte1[i]=crear_bitmap(BARX,BARY);
                          t_muerte2[i]=crear_bitmap(BARX,BARY); };
      t_muerte1[2]=crear_bitmap(TUMX,TUMY);
      t_muerte2[2]=crear_bitmap(TUMX,TUMY);
}

void skin::carga_sprites(ALLEGRO_BITMAP *im)
{
	 int i,j,k;
	 for(i=0; i<2; i++)
	  {
			al_set_target_bitmap(c_normal[i]);
			al_draw_bitmap_region(im,0+70*i,0,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
		    al_set_target_bitmap(c_guardia[i]);
			al_draw_bitmap_region(im,0+70*i,70,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
		    al_set_target_bitmap(c_punyo[i]);
			al_draw_bitmap_region(im,0+70*i,140,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
		    al_set_target_bitmap(c_danyo[i]);
			al_draw_bitmap_region(im,0+70*i,210,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
		    al_set_target_bitmap(c_magia[i]);
			al_draw_bitmap_region(im,0+70*i,280,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
            for(j=0; j<3; j++)
			{
				  al_set_target_bitmap(c_ataque[i][j]);
				  al_draw_bitmap_region(im,0+70*j,350+70*i,CUBX,CUBY,0,0,0);
			}
      for(i=0; i<2; i++)
            for(j=0; j<3; j++)
			{
				al_set_target_bitmap(p_andar[i][j]);
				al_draw_bitmap_region(im,210+70*j,0+70*i,CUBX,CUBY,0,0,0);
			}
      for(i=0; i<2; i++)
	  {
			al_set_target_bitmap(p_normal[i]);
			al_draw_bitmap_region(im,210+70*i,140,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
			al_set_target_bitmap(t_muerte1[i]);
			al_draw_bitmap_region(im,210+70*i,210,BARX,BARY,0,0,0);
	  }

	  al_set_target_bitmap(t_muerte1[2]);
	  al_draw_bitmap_region(im,350,210,TUMX,TUMY,0,0,0);
      for(i=0; i<2; i++)
	  {
			al_set_target_bitmap(t_muerte2[i]);
			al_draw_bitmap_region(im,210+70*i,310,BARX,BARY,0,0,0);
	  }

	  al_set_target_bitmap(t_muerte2[2]);
	  al_draw_bitmap_region(im,350,310,TUMX,TUMY,0,0,0);
}
skin::~skin()
{
	 int i,j,k;
      for(i=0; i<2; i++){ destruir_bitmap(c_normal[i]);
                          destruir_bitmap(c_guardia[i]);
                          destruir_bitmap(c_punyo[i]);
                          destruir_bitmap(c_danyo[i]);
                          destruir_bitmap(c_magia[i]); };
      for(i=0; i<2; i++)
      for(k=0; k<3; k++){  destruir_bitmap(c_ataque[i][k]);
                           destruir_bitmap(p_andar[i][k]); };

      for(i=0; i<2; i++) destruir_bitmap(p_normal[i]);

      for(i=0; i<3; i++){ destruir_bitmap(t_muerte1[i]);
                          destruir_bitmap(t_muerte2[i]); };
}

charset::charset(ALLEGRO_BITMAP *pant,int scrx, int scry)
{
      int i,j,k,l;

      tamx=scrx; tamy=scry;
/*      for(i=0; i<2; i++) half[i]=create_sub_bitmap(pant,0,i*(scry/2),scrx,scry/2);
      for(i=0; i<4; i++) quart[i]=create_sub_bitmap(pant,(scrx/2)*(i%2),(scry/2)*(i/2),scrx/2,scry/2);*/
      //Decorado
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<5; i++) baldnorm[i][k]=crear_bitmap(BALX,BALY);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<3; i++) balddan[j][i][k]=crear_bitmap(BALX,BALY);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<3; i++) baldmort[j][i][k]=crear_bitmap(BALX,BALY);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<6; j++)
      for(i=0; i<2; i++) parednorm[j][i][k]=crear_bitmap(PARX,PARY);
      for(k=0; k<MAXLUZ; k++)
      for(l=0; l<2; l++)
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) paredanim[j][i][l][k]=crear_bitmap(PARX,PARY);
      for(k=0; k<MAXLUZ; k++){
      esqbase[k]=crear_bitmap(ESBX,ESBY);
      esqtras[k]=crear_bitmap(ESLX,ESLY);
      esqdel[k]=crear_bitmap(ESLX,ESLY);
      marclat1[k]=crear_bitmap(PARX,PARY);
      marclat2[k]=crear_bitmap(PARX,PARY);
      marcarc[k]=crear_bitmap(PARX,PARY);
      marcsup[k]=crear_bitmap(PARX,PARY);};
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) interr[i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) telep[i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<6; i++) decornorm[i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) decoranim[j][i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      extra[k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) columna[i][k]=crear_bitmap(COLX,COLY);

      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) puertanorm[i][k]=crear_bitmap(PUEX,PUEY);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<4; i++) puertallav[j][i][k]=crear_bitmap(PUEX,PUEY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) barrera[i][k]=crear_bitmap(BARX,BARY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) laserpost[i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) tirador[i][k]=crear_bitmap(CUBX,CUBY);
      //Warriors

      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++){ c_normal[j][i]=crear_bitmap(CUBX,CUBY);
                          c_guardia[j][i]=crear_bitmap(CUBX,CUBY);
                          c_punyo[j][i]=crear_bitmap(CUBX,CUBY);
                          c_danyo[j][i]=crear_bitmap(CUBX,CUBY);
                          c_magia[j][i]=crear_bitmap(CUBX,CUBY); };
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++)
      for(k=0; k<3; k++){  c_ataque[j][i][k]=crear_bitmap(CUBX,CUBY);
                           p_andar[j][i][k]=crear_bitmap(CUBX,CUBY); };
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++) p_normal[j][i]=crear_bitmap(CUBX,CUBY);
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++){ t_muerte1[j][i]=crear_bitmap(BARX,BARY);
                          t_muerte2[j][i]=crear_bitmap(BARX,BARY); };
      for(j=0; j<NSPR+1; j++){ t_muerte1[j][2]=crear_bitmap(TUMX,TUMY);
                             t_muerte2[j][2]=crear_bitmap(TUMX,TUMY); };
      //Explosiones
      for(i=0; i<5; i++)
      for(k=0; k<3; k++) explosi[i][k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<4; k++) llamas[k]=crear_bitmap(CUBX,CUBY);
      for(k=0; k<3; k++) llama2[k]=crear_bitmap(CUBX,CUBY);

      //Balas
      for(i=0; i<6; i++)
      for(k=0; k<6; k++) balas[i][k]=crear_bitmap(BULX,BULY);

      //Armas
      for(i=0; i<7; i++)
      for(k=0; k<2; k++) a_sigma[i][k]=crear_bitmap(BULX,BULY);
      for(i=0; i<7; i++)
      for(k=0; k<6; k++) a_omega[i][k]=crear_bitmap(CUBX,CUBY);
      for(i=0; i<7; i++)
      for(k=0; k<2; k++) escudos[i][k]=crear_bitmap(ESCX,ESCY);
      for(k=0; k<6; k++) baston[k]=crear_bitmap(CUBX,CUBY);
      //Items
      botiquin=crear_bitmap(ITEX,ITEY);
      for(i=0; i<4; i++)
      for(j=0; j<3; j++) llaves[i][j]=crear_bitmap(ITEX,ITEY);
      cbalas=crear_bitmap(ITEX,ITEY);
      cohetes=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++) napalm[j]=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) mana[i][j]=crear_bitmap(ITEX,ITEY);
      for(i=0; i<3; i++) invul[i]=crear_bitmap(ITEX,ITEY);
      for(i=0; i<3; i++) berserk[i]=crear_bitmap(ITEX,ITEY);
      for(i=0; i<3; i++) invis[i]=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++) repuls[j]=crear_bitmap(ITEX,ITEY);
      for(i=0; i<3; i++) health[i]=crear_bitmap(ITEX,ITEY);
      bolt=crear_bitmap(ITEX,ITEY);
      for(i=0; i<3; i++) mapacomp[i]=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++) veneno[j]=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++) shadows[j]=crear_bitmap(ITEX,ITEY);
      for(j=0; j<3; j++) divino[j]=crear_bitmap(ITEX,ITEY);
      apocalip=crear_bitmap(ITEX,ITEY);
	 /* for(i=0; i<3; i++)
      for(j=0; j<3; j++) gema[i][j]=crear_bitmap(ITEX,ITEY);*/

      //Sprites
      /*for(j=0; j<2; j++)
      for(i=0; i<3; i++){
      luces[0][i][j]=create_bitmap(BALX,BALY);
      luces[1][i][j]=create_bitmap(PARX,PARY);
      };*/
	  if(!silueta) silueta = crear_bitmap(100,100);

      for(i=0; i<3; i++) aura[i]=crear_bitmap(70,70);
      sombra=crear_bitmap(BALX,BALY);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) hielo[i][k]=crear_bitmap(BARX,BARY);
      for(i=0; i<3; i++) capsulas[i]=crear_bitmap(BARX,BARY);
      for(i=0; i<3; i++)
      for(j=0; j<2; j++) laser[j][i]=crear_bitmap(CUBX,CUBY);
      for(j=0; j<4; j++) gota[j]=crear_bitmap(5,10);
      /*ttrans=new COLOR_MAP;
      tluz=new COLOR_MAP;
      tsombra=new COLOR_MAP;
      paleta=new RGB[256];*/
      for(i=0; i<256; i++) fuente[i]=crear_bitmap(12,12);
      marcador[0]=crear_bitmap(MARCADOR1X,MARCADOR1Y);
      marcador[1]=crear_bitmap(MARCADOR2X,MARCADOR2Y);
      for(i=0; i<12; i++) consola[i]=crear_bitmap(CONTILEX,CONTILEY);


      //Matriz de iluminaci¢n
      for(i=0; i<MSIZEY; i++)
         matrizluz[i]=DBG_NEW byte[MSIZEX];

	  //Skins
	  for(i=0; i<MAXSKINS; i++)
		  skins[i] = 0;

}
void charset::nuevo_display(int x, int y)
{
      tamx=x; tamy=y;
}
charset::~charset()
{
      int i,j,k,l;
/*
      for(i=0; i<2; i++) destroy_bitmap(half[i]);
      for(i=0; i<4; i++) destroy_bitmap(quart[i]);*/
      //Decorado
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<5; i++) destruir_bitmap(baldnorm[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<3; i++) destruir_bitmap(balddan[j][i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<3; i++) destruir_bitmap(baldmort[j][i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<6; j++)
      for(i=0; i<2; i++) destruir_bitmap(parednorm[j][i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(l=0; l<2; l++)
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) destruir_bitmap(paredanim[j][i][l][k]);
      for(k=0; k<MAXLUZ; k++){
      destruir_bitmap(esqbase[k]);
      destruir_bitmap(esqtras[k]);
      destruir_bitmap(esqdel[k]);
      destruir_bitmap(marclat1[k]);
      destruir_bitmap(marclat2[k]);
      destruir_bitmap(marcarc[k]);
      destruir_bitmap(marcsup[k]);};

      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) destruir_bitmap(interr[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) destruir_bitmap(telep[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<6; i++) destruir_bitmap(decornorm[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) destruir_bitmap(decoranim[j][i][k]);
      for(k=0; k<MAXLUZ; k++)
      destruir_bitmap(extra[k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) destruir_bitmap(columna[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) destruir_bitmap(puertanorm[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(j=0; j<2; j++)
      for(i=0; i<4; i++) destruir_bitmap(puertallav[j][i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) destruir_bitmap(barrera[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) destruir_bitmap(laserpost[i][k]);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<2; i++) destruir_bitmap(tirador[i][k]);
      for(j=0; j<4; j++) destruir_bitmap(gota[j]);
      //Warriors
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++){ destruir_bitmap(c_normal[j][i]);
                          destruir_bitmap(c_guardia[j][i]);
                          destruir_bitmap(c_punyo[j][i]);
                          destruir_bitmap(c_danyo[j][i]);
                          destruir_bitmap(c_magia[j][i]); };
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++)
      for(k=0; k<3; k++){  destruir_bitmap(c_ataque[j][i][k]);
                           destruir_bitmap(p_andar[j][i][k]); };
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<2; i++) destruir_bitmap(p_normal[j][i]);
      for(j=0; j<NSPR+1; j++)
      for(i=0; i<3; i++){ destruir_bitmap(t_muerte1[j][i]);
                          destruir_bitmap(t_muerte2[j][i]); };
      //Explosiones
      for(i=0; i<5; i++)
      for(k=0; k<3; k++) destruir_bitmap(explosi[i][k]);
      for(k=0; k<4; k++) destruir_bitmap(llamas[k]);
      for(k=0; k<3; k++) destruir_bitmap(llama2[k]);

      //Balas
      for(i=0; i<6; i++)
      for(k=0; k<6; k++) destruir_bitmap(balas[i][k]);

      //Armas
      for(i=0; i<7; i++)
      for(k=0; k<2; k++) destruir_bitmap(a_sigma[i][k]);
      for(i=0; i<7; i++)
      for(k=0; k<6; k++) destruir_bitmap(a_omega[i][k]);
      for(i=0; i<7; i++)
      for(k=0; k<2; k++) destruir_bitmap(escudos[i][k]);
      for(k=0; k<6; k++) destruir_bitmap(baston[k]);
      //Items
      destruir_bitmap(botiquin);
      for(i=0; i<4; i++)
      for(j=0; j<3; j++) destruir_bitmap(llaves[i][j]);
      destruir_bitmap(cbalas);
      destruir_bitmap(cohetes);
      for(j=0; j<3; j++) destruir_bitmap(napalm[j]);
      for(j=0; j<3; j++)
      for(i=0; i<3; i++) destruir_bitmap(mana[i][j]);
      for(i=0; i<3; i++) destruir_bitmap(invul[i]);
      for(i=0; i<3; i++) destruir_bitmap(berserk[i]);
      for(j=0; j<3; j++) destruir_bitmap(invis[j]);
      for(j=0; j<3; j++) destruir_bitmap(repuls[j]);
      for(j=0; j<3; j++) destruir_bitmap(health[j]);
      destruir_bitmap(bolt);
      for(j=0; j<3; j++) destruir_bitmap(mapacomp[j]);
      for(j=0; j<3; j++) destruir_bitmap(veneno[j]);
      for(j=0; j<3; j++) destruir_bitmap(shadows[j]);
      for(j=0; j<3; j++) destruir_bitmap(divino[j]);
      destruir_bitmap(apocalip);
	  for(i=0; i<3; i++)
      for(j=0; j<3; j++) destruir_bitmap(gema[i][j]);

      //Sprites
      /*for(j=0; j<2; j++)
      for(i=0; i<3; i++){
      destroy_bitmap(luces[0][i][j]);
      destroy_bitmap(luces[1][i][j]);
      };*/
	  if(silueta) { destruir_bitmap(silueta); silueta = 0; }

      for(i=0; i<3; i++) destruir_bitmap(aura[i]);
      destruir_bitmap(sombra);
      for(k=0; k<MAXLUZ; k++)
      for(i=0; i<3; i++) destruir_bitmap(hielo[i][k]);
      for(i=0; i<3; i++) destruir_bitmap(capsulas[i]);
      for(i=0; i<3; i++)
      for(j=0; j<2; j++) destruir_bitmap(laser[j][i]);
      /*delete ttrans;
      delete tluz;
      delete tsombra;
      delete[] paleta;*/
      for(i=0; i<256; i++) destruir_bitmap(fuente[i]);
      destruir_bitmap(marcador[0]);
      destruir_bitmap(marcador[1]);
      for(i=0; i<3; i++) destruir_bitmap(minicara[i]);
	  for(i=0; i < MAXPL; i++)
			if(minicaras_coloreadas[i]) destruir_bitmap(minicaras_coloreadas[i]);
      for(i=0; i<12; i++) destruir_bitmap(consola[i]);

      //Matriz de iluminaci¢n
      for(i=0; i<MSIZEY; i++)
         delete[] matrizluz[i];

	 destruye_todas_skins();

	 muestra_reservas();
}
/*RGB *charset::dev_paleta()
{
      return paleta;
}*/
// Carga todos los bitmaps que va a usar una batalla bt, correspondientes
// a los ficheros indicados en la misma estructura
void charset::carga_bitmaps_batalla(batalla *bt)
{

      char s[60];
	  int i,j;

      nantor=0;
      for(i=0; i<MSIZEY; i++)
         for(j=0; j<MSIZEX; j++){
            if(bt->dev_mapa()->conse(j,i)==PHO7) {antorchas[nantor][0]=j; antorchas[nantor][1]=i+1; nantor++;};
            if(bt->dev_mapa()->conse(j,i)==PVE7) {antorchas[nantor][0]=j+1; antorchas[nantor][1]=i; nantor++;};
         };

      sprintf(s,"%s/%s.%s",bt->directorio(),bt->dev_nivel()->fdecor(),FILE_IMG_EXT);
      carga_escenario(s,bt);
      // Transforma la paleta para los coeficientes del nivel
      /*for(i=0; i<256; i++){
                paleta[i].r=(unsigned char)(paleta[i].r*bt->dev_nivel()->coefr());
                paleta[i].g=(unsigned char)(paleta[i].g*bt->dev_nivel()->coefg());
                paleta[i].b=(unsigned char)(paleta[i].b*bt->dev_nivel()->coefb());
                };
      set_pallete(paleta);
	  */
      for(i=0; i<NSPR; i++){
            sprintf(s,"%s/%s.%s",bt->directorio(),bt->dev_nivel()->fspr(i),FILE_IMG_EXT);
            carga_guerrero(s,i);};

	  // Carga las skins de los personajes activos
	  i = 0;
	  while(bt->dev_player(i))
	  {
		  warrior *w = bt->dev_player(i);
		  sprintf(s,"%s/%s.%s",bt->directorio(),bt->dev_nivel()->fspr(w->cualspr()),FILE_IMG_EXT);
		  int id_skin = crea_skin_personaje(s,w->cualspr(),w->color(1),w->color(2));
		  if(id_skin >= 0)
			  bt->dev_player(i)->set_skin(id_skin);
		  i++;
	  }

	  i = 0;
	  while(bt->dev_enem(i))
	  {
		  warrior *w = bt->dev_enem(i);
		  sprintf(s,"%s/%s.%s",bt->directorio(),bt->dev_nivel()->fspr(w->cualspr()),FILE_IMG_EXT);
		  int id_skin = crea_skin_personaje(s,w->cualspr(),w->color(1),w->color(2));
		  if(id_skin >= 0)
			  bt->dev_enem(i)->set_skin(id_skin);
		  i++;
	  }

	  // Skin de congelado
	  sprintf(s,"%s/%s.%s",bt->directorio(),"congela",FILE_IMG_EXT);
	  congelado_skin = crea_skin_personaje(s,NSPR,BCO1,BCO2);

}
void charset::crea_variantes_ilum(ALLEGRO_BITMAP *b[], int alpha_pixel_x, int alpha_pixel_y)
{

		int i;
   //   for(i=0; i<MAXLUZ; i++)
   //   if(i!=MEDLUZ-1){
   //         //clear_to_color(b[i],0);
   //         //draw_sprite_ilum(b[i],b[MEDLUZ-1],0,0,i+1,ilum_trans,tluz,tsombra);

			//al_set_target_bitmap(b[i]);
			//al_draw_tinted_bitmap(b[MEDLUZ-1],al_map_rgba_f(0.5,0.5,0.5,1.0),0,0,0);
	  //};

		/*al_set_target_bitmap(b[0]);
		al_draw_tinted_bitmap(b[MEDLUZ-1],al_map_rgba_f(0.1,0.1,0.1,1.0),0,0,0);
		al_set_target_bitmap(b[1]);
		al_draw_tinted_bitmap(b[MEDLUZ-1],al_map_rgba_f(0.4,0.4,0.4,1.0),0,0,0);
		al_set_target_bitmap(b[2]);
		al_draw_tinted_bitmap(b[MEDLUZ-1],al_map_rgba_f(0.7,0.7,0.7,1.0),0,0,0);
*/
		float step = 0.9 / (MEDLUZ-1);
		for(i=0; i <= MEDLUZ-2; i++)
		{
			float light = 0.1 + (step * i);
			al_set_target_bitmap(b[i]);
			al_draw_tinted_bitmap(b[MEDLUZ-1],al_map_rgba_f(light,light,light,1.0),0,0,0);
		}

		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);

		step = 0.6 / (MEDLUZ);
		for(i=MEDLUZ; i<=MAXLUZ-1; i++)
		{
			float light = 0.0f + step * (i - MEDLUZ - 1);
			al_set_target_bitmap(b[i]);
			al_draw_bitmap(b[MEDLUZ-1],0,0,0);
			al_draw_filled_rectangle(0,0,al_get_bitmap_width(b[i]),al_get_bitmap_height(b[i]),al_map_rgba_f(1.0,1.0,1.0,light));
			al_convert_mask_to_alpha(b[i],al_get_pixel(b[i],alpha_pixel_x,alpha_pixel_y));
		}
		/*al_set_target_bitmap(b[4]);
		al_draw_bitmap(b[MEDLUZ-1],0,0,0);
		al_draw_filled_rectangle(0,0,al_get_bitmap_width(b[4]),al_get_bitmap_height(b[4]),al_map_rgba_f(1.0,1.0,1.0,0.2));
		al_convert_mask_to_alpha(b[4],al_get_pixel(b[4],0,0));
		al_set_target_bitmap(b[5]);
		al_draw_bitmap(b[MEDLUZ-1],0,0,0);
		al_draw_filled_rectangle(0,0,al_get_bitmap_width(b[4]),al_get_bitmap_height(b[4]),al_map_rgba_f(1.0,1.0,1.0,0.4));
		al_convert_mask_to_alpha(b[5],al_get_pixel(b[5],0,0));
		al_set_target_bitmap(b[6]);
		al_draw_bitmap(b[MEDLUZ-1],0,0,0);
		al_draw_filled_rectangle(0,0,al_get_bitmap_width(b[4]),al_get_bitmap_height(b[4]),al_map_rgba_f(1.0,1.0,1.0,0.6));
		al_convert_mask_to_alpha(b[6],al_get_pixel(b[6],0,0));
*/
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);//Default blender

}
// Obtiene los sprites del decorado de una imagen [file] pcx del tipo adecuado
void charset::carga_escenario(char *file, batalla *bt)
{

      ALLEGRO_BITMAP *im, *im2;
      int i,j;
      char s[60];

      im2=cargar_bitmap(file);
	  al_convert_mask_to_alpha(im2,al_get_pixel(im2,0,0));

	  im = crear_bitmap(al_get_bitmap_width(im2),al_get_bitmap_height(im2));

	  al_set_target_bitmap(im);
	  al_draw_tinted_bitmap(im2, al_map_rgba_f(bt->dev_nivel()->coefr(),bt->dev_nivel()->coefg(),bt->dev_nivel()->coefb(),1.f),0,0,0);

	  destruir_bitmap(im2);

      // M¢dulos para oscurecer e iluminar habitaciones
      /*for(j=0; j<6; j++)
      clear_to_color(luces[int(j/3)][j%3][0],255);
      for(i=0; i<3; i++){
      blit(im,luces[0][i][0],700+70*i,0,0,0,BALX,BALY);
      blit(im,luces[1][i][0],700+70*i,40,0,0,PARX,PARY);
      draw_sprite_h_flip(luces[1][i][1],luces[1][i][0],0,0);};
      for(i=0; i<3; i++)
      ilum_trans[i]=208-48*i;*/

      for(j=0; j<2; j++)
            for(i=0; i<3; i++)
                  //blit(im,balddan[j][i][MEDLUZ-1],0+70*j,0+40*i,0,0,BALX,BALY);
					{
						al_set_target_bitmap(balddan[j][i][MEDLUZ-1]);
						al_draw_bitmap_region(im,0+70*j,0+40*i,BALX,BALY,0,0,0);
					}
      for(j=0; j<2; j++)
            for(i=0; i<3; i++)
                  //blit(im,baldmort[j][i][MEDLUZ-1],140+70*j,0+40*i,0,0,BALX,BALY);
					{
						al_set_target_bitmap(baldmort[j][i][MEDLUZ-1]);
						al_draw_bitmap_region(im,140+70*j,0+40*i,BALX,BALY,0,0,0);
					}
      for(i=0; i<5; i++)
            //blit(im,baldnorm[i][MEDLUZ-1],280+70*i,0,0,0,BALX,BALY);
			{
				al_set_target_bitmap(baldnorm[i][MEDLUZ-1]);
				al_draw_bitmap_region(im,280+70*i,0,BALX,BALY,0,0,0);
			}
      for(j=0; j<3; j++)
            for(i=0; i<3; i++){
                  /*blit(im,paredanim[j][i][0][MEDLUZ-1],0+70*j,120+120*i,0,0,PARX,PARY);
                  clear_to_color(paredanim[j][i][1][MEDLUZ-1],0);
                  draw_sprite_h_flip(paredanim[j][i][1][MEDLUZ-1],paredanim[j][i][0][MEDLUZ-1],0,0);
                  */
					al_set_target_bitmap(paredanim[j][i][0][MEDLUZ-1]);
					al_draw_bitmap_region(im,0+70*j,120+120*i,PARX,PARY,0,0,0);
					al_set_target_bitmap(paredanim[j][i][1][MEDLUZ-1]);
					al_draw_bitmap_region(im,0+70*j,120+120*i,PARX,PARY,0,0,ALLEGRO_FLIP_HORIZONTAL );
				};

      for(i=0; i<6; i++){
            /*blit(im,parednorm[i][0][MEDLUZ-1],210+70*i,120,0,0,PARX,PARY);
            clear_to_color(parednorm[i][1][MEDLUZ-1],0);
            draw_sprite_h_flip(parednorm[i][1][MEDLUZ-1],parednorm[i][0][MEDLUZ-1],0,0);
*/
			al_set_target_bitmap(parednorm[i][0][MEDLUZ-1]);
			al_draw_bitmap_region(im,210+70*i,120,PARX,PARY,0,0,0);
			al_set_target_bitmap(parednorm[i][1][MEDLUZ-1]);
			al_draw_bitmap_region(im,210+70*i,120,PARX,PARY,0,0,ALLEGRO_FLIP_HORIZONTAL );
            };
	  /*
      blit(im,esqbase[MEDLUZ-1],250,480,0,0,ESBX,ESBY);
      blit(im,esqtras[MEDLUZ-1],220,480,0,0,ESLX,ESLY);
      blit(im,esqdel[MEDLUZ-1],320,480,0,0,ESLX,ESLY);
      blit(im,marclat1[MEDLUZ-1],420,380,0,0,PARX,PARY);
      blit(im,marclat2[MEDLUZ-1],490,380,0,0,PARX,PARY);
      blit(im,marcsup[MEDLUZ-1],560,380,0,0,PARX,PARY);
      blit(im,marcarc[MEDLUZ-1],630,380,0,0,PARX,PARY);*/

	  al_set_target_bitmap(esqbase[MEDLUZ-1]);
	  al_draw_bitmap_region(im,250,480,ESBX,ESBY,0,0,0);
	  al_set_target_bitmap(esqtras[MEDLUZ-1]);
	  al_draw_bitmap_region(im,220,480,ESBX,ESBY,0,0,0);
	  al_set_target_bitmap(esqdel[MEDLUZ-1]);
	  al_draw_bitmap_region(im,320,480,ESBX,ESBY,0,0,0);
	  al_set_target_bitmap(marclat1[MEDLUZ-1]);
	  al_draw_bitmap_region(im,420,380,PARX,PARY,0,0,0);
	  al_set_target_bitmap(marclat2[MEDLUZ-1]);
	  al_draw_bitmap_region(im,490,380,PARX,PARY,0,0,0);
	  al_set_target_bitmap(marcsup[MEDLUZ-1]);
	  al_draw_bitmap_region(im,560,380,PARX,PARY,0,0,0);
	  al_set_target_bitmap(marcarc[MEDLUZ-1]);
	  al_draw_bitmap_region(im,630,380,PARX,PARY,0,0,0);




      for(i=0; i<2; i++)
	  {
            //blit(im,interr[i][MEDLUZ-1],0+70*i,600,0,0,CUBX,CUBY);
			al_set_target_bitmap(interr[i][MEDLUZ-1]);
			al_draw_bitmap_region(im,0+70*i,600,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<3; i++)
	  {
		    //blit(im,telep[i][MEDLUZ-1],140+70*i,600,0,0,CUBX,CUBY);
			al_set_target_bitmap(telep[i][MEDLUZ-1]);
			al_draw_bitmap_region(im,140+70*i,600,CUBX,CUBY,0,0,0);

	  }
	  al_set_target_bitmap(extra[MEDLUZ-1]);
	  //blit(im,extra[MEDLUZ-1],350,600,0,0,CUBX,CUBY);
	  al_draw_bitmap_region(im,350,600,CUBX,CUBY,0,0,0);
      for(i=0; i<6; i++)
	  {
           //blit(im,decornorm[i][MEDLUZ-1],280+70*i,40,0,0,CUBX,CUBY);
		  al_set_target_bitmap(decornorm[i][MEDLUZ-1]);
		  al_draw_bitmap_region(im,280+70*i,40,CUBX,CUBY,0,0,0);
	  }
      for(j=0; j<3; j++)
            for(i=0; i<3; i++)
			{
                  //blit(im,decoranim[j][i][MEDLUZ-1],210+70*j,240+70*i,0,0,CUBX,CUBY);
					al_set_target_bitmap(decoranim[j][i][MEDLUZ-1]);
					al_draw_bitmap_region(im,210+70*j,240+70*i,CUBX,CUBY,0,0,0);
			}
      for(i=0; i<3; i++)
	  {
            //blit(im,columna[i][MEDLUZ-1],420+70*i,240,0,0,COLX,COLY);
			al_set_target_bitmap(columna[i][MEDLUZ-1]);
			al_draw_bitmap_region(im,420+70*i,240,COLX,COLY,0,0,0);
	  }

      //blit(im,puertanorm[0][MEDLUZ-1],0,480,0,0,PUEX,PUEY);
      //clear_to_color(puertanorm[1][MEDLUZ-1],0);
      //draw_sprite_h_flip(puertanorm[1][MEDLUZ-1],puertanorm[0][MEDLUZ-1],0,0);
	  al_set_target_bitmap(puertanorm[0][MEDLUZ-1]);
	  al_draw_bitmap_region(im,0,480,PUEX,PUEY,0,0,0);
	  al_set_target_bitmap(puertanorm[1][MEDLUZ-1]);
	  al_draw_bitmap_region(im,0,480,PUEX,PUEY,0,0,ALLEGRO_FLIP_HORIZONTAL);

      for(i=0; i<4; i++){
            //blit(im,puertallav[0][i][MEDLUZ-1],40+40*i,480,0,0,COLX,COLY);
            //clear_to_color(puertallav[1][i][MEDLUZ-1],0);
            //draw_sprite_h_flip(puertallav[1][i][MEDLUZ-1],puertallav[0][i][MEDLUZ-1],0,0);
			al_set_target_bitmap(puertallav[0][i][MEDLUZ-1]);
			al_draw_bitmap_region(im,40+40*i,480,PUEX,PUEY,0,0,0);
			al_set_target_bitmap(puertallav[1][i][MEDLUZ-1]);
			al_draw_bitmap_region(im,40+40*i,480,PUEX,PUEY,0,0,ALLEGRO_FLIP_HORIZONTAL);
            };
      for(i=0; i<3; i++)
	  {
			//blit(im,barrera[i][MEDLUZ-1],420+70*i,500,0,0,BARX,BARY);
		  al_set_target_bitmap(barrera[i][MEDLUZ-1]);
		  al_draw_bitmap_region(im,420+70*i,500,BARX,BARY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
               //blit(im,laserpost[i][MEDLUZ-1],420+70*i,600,0,0,CUBX,CUBY);
		  al_set_target_bitmap(laserpost[i][MEDLUZ-1]);
		  al_draw_bitmap_region(im,420+70*i,600,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
               //blit(im,tirador[i][MEDLUZ-1],560+70*i,600,0,0,CUBX,CUBY);
		  al_set_target_bitmap(tirador[i][MEDLUZ-1]);
		  al_draw_bitmap_region(im,560+70*i,600,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<3; i++){
		  al_set_target_bitmap(llaves[0][i]);
		  al_draw_bitmap_region(im,700+ITEX*i,200,ITEX,ITEY,0,0,0);
		  al_set_target_bitmap(llaves[1][i]);
		  al_draw_bitmap_region(im,700+ITEX*i,250,ITEX,ITEY,0,0,0);
		  al_set_target_bitmap(llaves[2][i]);
		  al_draw_bitmap_region(im,700+ITEX*i,300,ITEX,ITEY,0,0,0);
		  al_set_target_bitmap(llaves[3][i]);
		  al_draw_bitmap_region(im,700+ITEX*i,350,ITEX,ITEY,0,0,0);
     /* blit(im,llaves[0][i],700+ITEX*i,200,0,0,ITEX,ITEY);
      blit(im,llaves[1][i],700+ITEX*i,250,0,0,ITEX,ITEY);
      blit(im,llaves[2][i],700+ITEX*i,300,0,0,ITEX,ITEY);
      blit(im,llaves[3][i],700+ITEX*i,350,0,0,ITEX,ITEY);*/
	  };
      destruir_bitmap(im);

}

ALLEGRO_BITMAP* charset::crea_copia_bitmap_coloreado(ALLEGRO_BITMAP *im, byte col1, byte col2)
{
	int width = al_get_bitmap_width(im);
	int height = al_get_bitmap_height(im);

	ALLEGRO_BITMAP *coloreada = crear_bitmap(width, height);

	al_lock_bitmap(im,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
	al_lock_bitmap(coloreada,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);

	al_set_target_bitmap(coloreada);

	for(int j = 0; j < height; j++)
	for(int i = 0; i < width; i++)
	{
		unsigned char sr, sg, sb, sa;
		ALLEGRO_COLOR source, dest;

		source = al_get_pixel(im,i,j);

		al_unmap_rgba(source,&sr,&sg,&sb,&sa);

		dest = al_map_rgba(sr,sg,sb,sa);

		for(int k = 0; k < 8; k++)
		{
			if(sr == dp2palette[BNARA+k][0] && sg == dp2palette[BNARA+k][1] && sb == dp2palette[BNARA+k][2])
				dest = al_map_rgba(dp2palette[col1+k][0],dp2palette[col1+k][1],dp2palette[col1+k][2],sa);

			if(sr == dp2palette[BGRIS+k][0] && sg == dp2palette[BGRIS+k][1] && sb == dp2palette[BGRIS+k][2])
				dest = al_map_rgba(dp2palette[col2+k][0],dp2palette[col2+k][1],dp2palette[col2+k][2],sa);
		}

		al_put_pixel(i,j,dest);
	}

	al_unlock_bitmap(im);
	al_unlock_bitmap(coloreada);

	return coloreada;
}

int charset::crea_skin_personaje(char *file, byte id_pers, byte col1, byte col2)
{
	int id_skin = 0;

	while(id_skin < MAXSKINS &&
			(skins[id_skin] == 0 || skins[id_skin]->id_pers != id_pers || skins[id_skin]->color1 != col1 || skins[id_skin]->color2 != col2))
	{
		id_skin++;
	}

	// Ya existe una skin adecuada!
	if(id_skin < MAXSKINS)
		return id_skin;

	//Nueva combinacion personaje/colores. Hay que crearla
	ALLEGRO_BITMAP *im = cargar_bitmap(file);
	al_convert_mask_to_alpha(im,al_get_pixel(im,0,0));
	int width = al_get_bitmap_width(im);
	int height = al_get_bitmap_height(im);
	ALLEGRO_BITMAP *coloreada = crea_copia_bitmap_coloreado(im,col1,col2);

	/*al_set_target_backbuffer(al_get_current_display());
	al_draw_bitmap(coloreada,0,0,0);
	al_flip_display();*/

	skin *nueva_skin = DBG_NEW skin();
	nueva_skin->id_pers = id_pers;
	nueva_skin->color1 = col1;
	nueva_skin->color2 = col2;
	nueva_skin->carga_sprites(coloreada);

	destruir_bitmap(im);
	destruir_bitmap(coloreada);

	// BUsca un slot para colocarla
	id_skin = 0;
	while(id_skin < MAXSKINS && skins[id_skin])
		id_skin++;

	// No caben más skins!!!
	if(id_skin == MAXSKINS)
	{
		delete nueva_skin;
		return -1;
	}

	skins[id_skin] = nueva_skin;
	return id_skin;

}

void charset::destruye_skin(int i)
{
	if(i < MAXSKINS)
	{
		if(skins[i])
		{
			delete skins[i];
			skins[i] = 0;
		}
	}
}

void charset::destruye_todas_skins()
{
	 //Skins
	  for(int i = 0; i < MAXSKINS; i++)
		  if(skins[i])
		  {
			delete skins[i];
			skins[i] = 0;
		  }
}

// Obtiene los sprites de un guerrero de una imagen [file] pcx y lo carga en el
// guerrero c
void charset::carga_guerrero(char *file,int c)
{

      ALLEGRO_BITMAP *im;
      int i,j;
      if(c>NSPR) {her_error("CHARSET",ERR_NOC); return;};

      im=cargar_bitmap(file);
	  al_convert_mask_to_alpha(im,al_get_pixel(im,0,0));

      for(i=0; i<2; i++)
	  {
            //blit(im,c_normal[c][i],0+70*i,0,0,0,CUBX,CUBY);
			al_set_target_bitmap(c_normal[c][i]);
			al_draw_bitmap_region(im,0+70*i,0,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
            //blit(im,c_guardia[c][i],0+70*i,70,0,0,CUBX,CUBY);
		    al_set_target_bitmap(c_guardia[c][i]);
			al_draw_bitmap_region(im,0+70*i,70,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
            //blit(im,c_punyo[c][i],0+70*i,140,0,0,CUBX,CUBY);
		    al_set_target_bitmap(c_punyo[c][i]);
			al_draw_bitmap_region(im,0+70*i,140,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
            //blit(im,c_danyo[c][i],0+70*i,210,0,0,CUBX,CUBY);
		    al_set_target_bitmap(c_danyo[c][i]);
			al_draw_bitmap_region(im,0+70*i,210,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
            //blit(im,c_magia[c][i],0+70*i,280,0,0,CUBX,CUBY);
		    al_set_target_bitmap(c_magia[c][i]);
			al_draw_bitmap_region(im,0+70*i,280,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
            for(j=0; j<3; j++)
			{
                  //blit(im,c_ataque[c][i][j],0+70*j,350+70*i,0,0,CUBX,CUBY);
				  al_set_target_bitmap(c_ataque[c][i][j]);
				  al_draw_bitmap_region(im,0+70*j,350+70*i,CUBX,CUBY,0,0,0);
			}
      for(i=0; i<2; i++)
            for(j=0; j<3; j++)
			{
				//blit(im,p_andar[c][i][j],210+70*j,0+70*i,0,0,CUBX,CUBY);
				al_set_target_bitmap(p_andar[c][i][j]);
				al_draw_bitmap_region(im,210+70*j,0+70*i,CUBX,CUBY,0,0,0);
			}
      for(i=0; i<2; i++)
	  {
            //blit(im,p_normal[c][i],210+70*i,140,0,0,CUBX,CUBY);
			al_set_target_bitmap(p_normal[c][i]);
			al_draw_bitmap_region(im,210+70*i,140,CUBX,CUBY,0,0,0);
	  }
      for(i=0; i<2; i++)
	  {
		   //blit(im,t_muerte1[c][i],210+70*i,210,0,0,BARX,BARY);
			al_set_target_bitmap(t_muerte1[c][i]);
			al_draw_bitmap_region(im,210+70*i,210,BARX,BARY,0,0,0);
	  }
      //blit(im,t_muerte1[c][2],350,210,0,0,TUMX,TUMY);
	  al_set_target_bitmap(t_muerte1[c][2]);
	  al_draw_bitmap_region(im,350,210,TUMX,TUMY,0,0,0);
      for(i=0; i<2; i++)
	  {
            //blit(im,t_muerte2[c][i],210+70*i,310,0,0,BARX,BARY);
			al_set_target_bitmap(t_muerte2[c][i]);
			al_draw_bitmap_region(im,210+70*i,310,BARX,BARY,0,0,0);
	  }
	  //blit(im,t_muerte2[c][2],350,310,0,0,TUMX,TUMY);
	  al_set_target_bitmap(t_muerte2[c][2]);
	  al_draw_bitmap_region(im,350,310,TUMX,TUMY,0,0,0);

      destruir_bitmap(im);

}
void charset::carga_sprites(batalla *b)
{

        ALLEGRO_BITMAP *bmp, *sombra2;
        int i;
        char s[60];

		sombra2 = crear_bitmap(70,70);

        // Auras de luz de antorchas, etc.
        for(i=0; i<3; i++)
		{
			al_set_target_bitmap(aura[i]);
            //al_clear_to_color(aura[i],255);
		}
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
        for(i=25; i>=0; i--)
		{
			al_set_target_bitmap(aura[0]);
			al_draw_filled_circle(35,35,i,al_map_rgba(255,255,255,8));

		}
        for(i=22; i>=0; i--)
		{
			al_set_target_bitmap(aura[1]);
            al_draw_filled_circle(35,35,i,al_map_rgba(255,255,255,8));
		}
        for(i=19; i>=0; i--)
		{
			al_set_target_bitmap(aura[2]);
            al_draw_filled_circle(35,35,i,al_map_rgba(255,255,255,8));
		}

		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
		for(i=19; i>=0; i--)
		{
			al_set_target_bitmap(sombra2);
            al_draw_filled_circle(35,35,i,al_map_rgba(0,0,0,16));
		}

        // C¡rculo de sombra en el suelo
		al_set_target_bitmap(sombra);
        //al_draw_bitmap(sombra,aura[2],-10,0,BALX+20,BALY);
		al_draw_scaled_bitmap(sombra2,0,0,70,70,0,0,BALX,BALY,0);
		//al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
		destruir_bitmap(sombra2);

        // Fuente de letras
#if defined SAHWINDOWS
        sprintf(s,"%s\\%s.%s",b->directorio(),"fuente",FILE_IMG_EXT);
#elif defined SAHLINUX
		sprintf(s, "%s/%s.%s", b->directorio(), "fuente", FILE_IMG_EXT);
#endif
        carga_fuente(s);

        // Marcador
        sprintf(s,"%s/%s.%s",b->directorio(),"marcador",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
		al_set_target_bitmap(marcador[0]);
        al_draw_bitmap_region(bmp,0,0,MARCADOR1X,MARCADOR1Y,0,0,0);
		al_set_target_bitmap(marcador[1]);
        al_draw_bitmap_region(bmp,MARCADOR1X,0,MARCADOR2X,MARCADOR2Y,0,0,0);
        destruir_bitmap(bmp);

        // Mini caras
        sprintf(s,"%s/sig_mini.%s",b->directorio(),FILE_IMG_EXT);
        minicara[0]=cargar_bitmap(s);
        sprintf(s,"%s/bet_mini.%s",b->directorio(),FILE_IMG_EXT);
        minicara[1]=cargar_bitmap(s);
        sprintf(s,"%s/ome_mini.%s",b->directorio(),FILE_IMG_EXT);
        minicara[2]=cargar_bitmap(s);

		for(i=0; i < MAXPL; i++)
			minicaras_coloreadas[i] = 0;
		for(i=0; i < b->num_players(); i++)
		{
			warrior *w = b->dev_player(i);
			minicaras_coloreadas[i] = crea_copia_bitmap_coloreado(minicara[(int)w->clase()], w->color(1), w->color(2));
		}

        // Tiles consola
        sprintf(s,"%s/%s.%s",b->directorio(),"consola",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_get_pixel(bmp,0,0));
        for(i=0; i<12; i++)
		{
			//blit(bmp,consola[i],CONTILEX*int(i%3),CONTILEY*int(i/3),0,0,CONTILEX,CONTILEY);
			al_set_target_bitmap(consola[i]);
			al_draw_bitmap_region(bmp,CONTILEX*int(i%3),CONTILEY*int(i/3),CONTILEX,CONTILEY,0,0,0);
		}
		destruir_bitmap(bmp);

        // Explosiones, armas, etc
        sprintf(s,"%s/%s.%s",b->directorio(),"hielo",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
        for(i=0; i<3; i++)
		{
			//blit(bmp,hielo[i][MEDLUZ-1],BARX*i,0,0,0,BARX,BARY);
			al_set_target_bitmap(hielo[i][MEDLUZ-1]);
			al_draw_bitmap_region(bmp,BARX*i,0,BARX,BARY,0,0,0);
		}
		destruir_bitmap(bmp);

        sprintf(s,"%s/%s.%s",b->directorio(),"capsulas",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
        for(i=0; i<3; i++)
		{
			//blit(bmp,capsulas[i],BARX*i,0,0,0,BARX,BARY);
			al_set_target_bitmap(capsulas[i]);
		  al_draw_bitmap_region(bmp,BARX*i,0,BARX,BARY,0,0,0);
		}
		destruir_bitmap(bmp);

        sprintf(s,"%s/%s.%s",b->directorio(),"explos",FILE_IMG_EXT);
        carga_explosiones(s);
        sprintf(s,"%s/%s.%s",b->directorio(),"balas",FILE_IMG_EXT);
        carga_balas(s);
        sprintf(s,"%s/%s.%s",b->directorio(),"arma_sig",FILE_IMG_EXT);
        carga_armas_sigma(s);
        sprintf(s,"%s/%s.%s",b->directorio(),"arma_ome",FILE_IMG_EXT);
        carga_armas_omega(s);
        sprintf(s,"%s/%s.%s",b->directorio(),"escudos",FILE_IMG_EXT);
        carga_escudos(s);
        sprintf(s,"%s/%s.%s",b->directorio(),"baston",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
        for(int j=0; j<6; j++)
		{
			  al_set_target_bitmap(baston[j]);
              al_draw_bitmap_region(bmp,70*j,0,CUBX,CUBY,0,0,0);
		}
        destruir_bitmap(bmp);

        sprintf(s,"%s/%s.%s",b->directorio(),"items",FILE_IMG_EXT);
        carga_items(s);

        sprintf(s,"%s/%s.%s",b->directorio(),"laser",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
        for(i=0; i<3; i++){
				al_set_target_bitmap(laser[0][i]);
				al_draw_bitmap_region(bmp,CUBX*i,0,CUBX,CUBY,0,0,0);
				al_set_target_bitmap(laser[1][i]);
				al_draw_bitmap_region(bmp,CUBX*i,0,CUBX,CUBY,0,0,ALLEGRO_FLIP_HORIZONTAL);
                //blit(bmp,laser[0][i],CUBX*i,0,0,0,CUBX,CUBY);
                //clear_to_color(laser[1][i],0);
                //draw_sprite_h_flip(laser[1][i],laser[0][i],0,0);
		};
        destruir_bitmap(bmp);

        sprintf(s,"%s/%s.%s",b->directorio(),"gota",FILE_IMG_EXT);
        bmp=cargar_bitmap(s);
		al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
        for(i=0; i<4; i++) {
			//blit(bmp,gota[i],10*i,0,0,0,5,10);
			al_set_target_bitmap(gota[i]);
			al_draw_bitmap_region(bmp,10*i,0,5,10,0,0,0);
		}
        destruir_bitmap(bmp);

	    sprintf(s,"%s/%s.%s",b->directorio(),"congela",FILE_IMG_EXT);
        carga_guerrero(s,NSPR);


		// Carga las skins

}
void charset::carga_explosiones(char *file)
{

      ALLEGRO_BITMAP *bmp;
	  int i,j;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
      for(i=0; i<5; i++)
      for(j=0; j<3; j++)
	  {
              //blit(bmp,explosi[i][j],70*j,70*i,0,0,CUBX,CUBY);
			al_set_target_bitmap(explosi[i][j]);
			al_draw_bitmap_region(bmp,70*j,70*i,CUBX,CUBY,0,0,0);
	  }
      for(j=0; j<4; j++)
	  {
		//blit(bmp,llamas[j],70*j,350,0,0,CUBX,CUBY);
		  al_set_target_bitmap(llamas[j]);
		  al_draw_bitmap_region(bmp,70*j,350,CUBX,CUBY,0,0,0);
	  }
      for(j=0; j<3; j++)
	  {
		//blit(bmp,llama2[j],70*j,420,0,0,CUBX,CUBY);
		  al_set_target_bitmap(llama2[j]);
		  al_draw_bitmap_region(bmp,70*j,420,CUBX,CUBY,0,0,0);
	  }
      destruir_bitmap(bmp);

}
void charset::carga_balas(char *file)
{

      ALLEGRO_BITMAP *bmp;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
      for(int i=0; i<6; i++)
      for(int j=0; j<6; j++)
	  {
              //blit(bmp,balas[i][j],70*j,50*i,0,0,BULX,BULY);
			al_set_target_bitmap(balas[i][j]);
			al_draw_bitmap_region(bmp,70*j,50*i,BULX,BULY,0,0,0);
	  }
      destruir_bitmap(bmp);

}
void charset::carga_armas_sigma(char *file)
{

      ALLEGRO_BITMAP *bmp;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_get_pixel(bmp,0,0));
      for(int i=0; i<7; i++)
      for(int j=0; j<2; j++)
	  {
		//blit(bmp,a_sigma[i][j],70*i,50*j,0,0,BULX,BULY);
		al_set_target_bitmap(a_sigma[i][j]);
		al_draw_bitmap_region(bmp,70*i,50*j,BULX,BULY,0,0,0);
	  }
      destruir_bitmap(bmp);

}

void charset::carga_armas_omega(char *file)
{

      ALLEGRO_BITMAP *bmp;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
      for(int i=0; i<7; i++)
      for(int j=0; j<6; j++)
	  {
              //blit(bmp,a_omega[i][j],70*j,70*i,0,0,CUBX,CUBY);
			al_set_target_bitmap(a_omega[i][j]);
			al_draw_bitmap_region(bmp,70*j,70*i,CUBX,CUBY,0,0,0);
	  }
      destruir_bitmap(bmp);
}
void charset::carga_escudos(char *file)
{

      ALLEGRO_BITMAP *bmp;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));
      for(int i=0; i<2; i++)
      for(int j=0; j<7; j++)
	  {
             //blit(bmp,escudos[j][i],40*j,50*i,0,0,ESCX,ESCY);
			al_set_target_bitmap(escudos[j][i]);
			al_draw_bitmap_region(bmp,40*j,50*i,ESCX,ESCY,0,0,0);
	  }
      destruir_bitmap(bmp);
}
void charset::carga_items(char *file)
{

      int i,j;
      ALLEGRO_BITMAP *bmp, *temp;
      bmp=cargar_bitmap(file);
	  al_convert_mask_to_alpha(bmp,al_map_rgb(0,0,0));

	  al_set_target_bitmap(botiquin);
	  al_draw_bitmap_region(bmp,0,0,ITEX,ITEY,0,0,0);
	  al_set_target_bitmap(apocalip);
	  al_draw_bitmap_region(bmp,0,50,ITEX,ITEY,0,0,0);

	  temp = crear_bitmap(ITEX,ITEY);
	  for(i=0; i<3; i++)
	  for(j=0; j < 3; j++)
	  {
			byte color;
			if(i == 0) color = BROJO;
			if(i == 1) color = BNARA;
			if(i == 2) color = BAZUN;
			al_set_target_bitmap(temp);
			al_clear_to_color(al_map_rgba(0,0,0,0));
			al_draw_bitmap_region(bmp,ITEX*j,100,ITEX,ITEY,0,0,0);
			gema[i][j] = crea_copia_bitmap_coloreado(temp,color,BCO2);
	  }
	  destruir_bitmap(temp);

	  al_set_target_bitmap(cbalas);
	  al_draw_bitmap_region(bmp,0,250,ITEX,ITEY,0,0,0);
	  al_set_target_bitmap(cohetes);
	  al_draw_bitmap_region(bmp,0,300,ITEX,ITEY,0,0,0);
	  for(i=0; i < 3; i++)
	  {
		al_set_target_bitmap(napalm[i]);
		al_draw_bitmap_region(bmp,120+ITEX*i,0,ITEX,ITEY,0,0,0);
	  }
	  for(i=0; i < 3; i++)
	  {
		al_set_target_bitmap(mana[0][i]);
		al_draw_bitmap_region(bmp,120+ITEX*i,50,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(mana[1][i]);
		al_draw_tinted_bitmap_region(bmp,al_map_rgba(255,255,255,128),120+ITEX*i,100,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(mana[2][i]);
		al_draw_bitmap_region(bmp,120+ITEX*i,150,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(invul[i]);
		al_draw_bitmap_region(bmp,120+ITEX*i,200,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(berserk[i]);
		al_draw_bitmap_region(bmp,120+ITEX*i,250,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(invis[i]);
		al_draw_tinted_bitmap_region(bmp,al_map_rgba(255,255,255,128),120+ITEX*i,300,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(health[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,0,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(repuls[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,50,ITEX,ITEY,0,0,0);
	  }
	  al_set_target_bitmap(bolt);
	  al_draw_bitmap_region(bmp,240,150,ITEX,ITEY,0,0,0);
	  for(i=0; i < 3; i++)
	  {
		al_set_target_bitmap(mapacomp[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,100,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(veneno[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,200,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(shadows[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,250,ITEX,ITEY,0,0,0);
		al_set_target_bitmap(divino[i]);
		al_draw_bitmap_region(bmp,240+ITEX*i,300,ITEX,ITEY,0,0,0);
	  }

	  destruir_bitmap(bmp);

     /* blit(bmp,botiquin,0,0,0,0,ITEX,ITEY);
      blit(bmp,apocalip,0,50,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,gema[i],ITEX*i,100,0,0,ITEX,ITEY);
      blit(bmp,cbalas,0,250,0,0,ITEX,ITEY);
      blit(bmp,cohetes,0,300,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,napalm[i],120+ITEX*i,0,0,0,ITEX,ITEY);
      for(i=0; i<3; i++)  blit(bmp,mana[0][i],120+ITEX*i,50,0,0,ITEX,ITEY);
      for(i=0; i<3; i++)  blit(bmp,mana[1][i],120+ITEX*i,100,0,0,ITEX,ITEY);
      for(i=0; i<3; i++)  blit(bmp,mana[2][i],120+ITEX*i,150,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,invul[i],120+ITEX*i,200,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,berserk[i],120+ITEX*i,250,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,invis[i],120+ITEX*i,300,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,health[i],240+ITEX*i,0,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,repuls[i],240+ITEX*i,50,0,0,ITEX,ITEY);
      blit(bmp,bolt,240,150,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,mapacomp[i],240+ITEX*i,100,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,veneno[i],240+ITEX*i,200,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,shadows[i],240+ITEX*i,250,0,0,ITEX,ITEY);
      for(i=0; i<3; i++) blit(bmp,divino[i],240+ITEX*i,300,0,0,ITEX,ITEY);*/

}
void charset::carga_fuente(char *file)
{
      ALLEGRO_BITMAP *bmp;
      bmp=cargar_bitmap(file);

	  al_set_target_bitmap(bmp);
	  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
	  al_draw_filled_rectangle(0,0,al_get_bitmap_width(bmp),al_get_bitmap_height(bmp),al_map_rgba_f(1.0,1.0,1.0,0.2f));
	  restore_default_blender();

	  al_convert_mask_to_alpha(bmp,al_get_pixel(bmp,0,0));

      for(int i=0; i<256; i++)
	  {
             //blit(bmp,fuente[i],16*int(i%16),15*int(i/16),0,0,12,12);
			al_set_target_bitmap(fuente[i]);
			al_draw_bitmap_region(bmp,16*int(i%16),15*int(i/16),12,12,0,0,0);
	  }
      destruir_bitmap(bmp);

}
void charset::crea_tablas_color(nivel *ni)
{

        byte c;
		int i,j,k;
        /*create_trans_table(ttrans,paleta,128,128,128,NULL);
        c=ni->color_luz();
        create_light_table(tluz,paleta,paleta[c].r,paleta[c].g,paleta[c].b,NULL);
        c=ni->color_sombra();
        create_light_table(tsombra,paleta,paleta[c].r,paleta[c].g,paleta[c].b,NULL);
        // Crea variantes de iluminaci¢n para los decorados*/
        for(i=0; i<5; i++)
            crea_variantes_ilum(baldnorm[i]);
        for(i=0; i<2; i++)
        for(j=0; j<3; j++){
            crea_variantes_ilum(balddan[i][j]);
            crea_variantes_ilum(baldmort[i][j]);};
        for(i=0; i<6; i++)
        for(j=0; j<2; j++)
            crea_variantes_ilum(parednorm[i][j]);
        for(i=0; i<3; i++)
        for(j=0; j<3; j++)
        for(k=0; k<2; k++)
            crea_variantes_ilum(paredanim[i][j][k]);
        crea_variantes_ilum(esqtras);
        crea_variantes_ilum(esqbase);
        crea_variantes_ilum(esqdel);
        crea_variantes_ilum(marclat1);
        crea_variantes_ilum(marclat2);
        crea_variantes_ilum(marcarc);
        crea_variantes_ilum(marcsup);
        for(i=0; i<2; i++)
        crea_variantes_ilum(interr[i]);
        for(i=0; i<3; i++)
        crea_variantes_ilum(telep[i]);
        for(i=0; i<6; i++)
        crea_variantes_ilum(decornorm[i]);
        for(j=0; j<3; j++)
        for(i=0; i<3; i++)
        crea_variantes_ilum(decoranim[i][j]);
        crea_variantes_ilum(extra);
        for(i=0; i<3; i++)
        crea_variantes_ilum(columna[i]);

        crea_variantes_ilum(puertanorm[0]);
		crea_variantes_ilum(puertanorm[1],PUEX-1,0);
        for(j=0; j<4; j++)
		{
			crea_variantes_ilum(puertallav[0][j]);
			crea_variantes_ilum(puertallav[1][j],PUEX-1,0);
		}
        for(i=0; i<3; i++)
        crea_variantes_ilum(barrera[i]);
        for(i=0; i<3; i++)
        crea_variantes_ilum(hielo[i]);
        for(i=0; i<2; i++)
        crea_variantes_ilum(laserpost[i]);
        for(i=0; i<2; i++)
        crea_variantes_ilum(tirador[i]);

}
void charset::set_tipo_transp(int i)
{
	/*
        switch(i){
                case 0 : color_map=ttrans; break;
                case 1 : color_map=tluz; break;
                case 2 : color_map=tsombra; break;
        };
		*/
}
void charset::muestra_pantalla(ALLEGRO_BITMAP *pant, batalla *bt)
{

      float rx;
      char s[40];

      //Rango de baldosas a dibujar
      rx=2*tamx/(BALX*RAIZ2);

      //clear_to_color(pant,0);

	  al_set_target_bitmap(pant);
	  al_reset_clipping_rectangle();
	  al_clear_to_color(al_map_rgb(0,0,0));

      if(bt->consola_activa()){
                muestra_consola(pant,bt);
      }else
      if(bt->datos_activo()){
                muestra_datos_personaje(pant,0,bt->dev_player(0),20,0);
                if(bt->num_players()>1) muestra_datos_personaje(pant,1,bt->dev_player(1),tamx-300,0);
                if(bt->num_players()>2) muestra_datos_personaje(pant,2,bt->dev_player(2),20,tamy-215);
                if(bt->num_players()>3) muestra_datos_personaje(pant,3,bt->dev_player(3),tamx-300,tamy-215);
                sprintf(s,"Enemies:%d/%d",bt->dev_nivel()->num_enemigos_muertos(),
                                         bt->dev_nivel()->num_enemigos());
                texto(pant,s,0,tamy-36,BMAR1);
                sprintf(s,"Allieds:%d/%d",bt->dev_nivel()->num_aliados()-bt->dev_nivel()->num_aliados_muertos(),
                                         bt->dev_nivel()->num_aliados());
                texto(pant,s,(tamx-(strlen(s)*13))/2,tamy-16,BMAR1);
                sprintf(s,"Secrets:%d/%d",bt->dev_nivel()->num_total_secrets()-bt->dev_nivel()->num_secrets(),
                                         bt->dev_nivel()->num_total_secrets());
                texto(pant,s,(tamx-(strlen(s)*13)),tamy-36,BMAR1);

      }else
      if(bt->mapa_activo()){
        muestra_mapa(pant,bt);
        muestra_mensajes(pant,bt,PRIO1|PRIO2|PRIO3|PRIO4);
        texto(pant,bt->dev_nivel()->nombre(),5,tamy-16-5,BCO2);
      }else

	  if(bt->dev_modo_red() == MRSERVER || bt->dev_modo_red() == MRCLIENTE)
	  {
		int local_player = bt->dev_jug_local();
		muestra(pant,bt,tamx,tamy,bt->dev_player(local_player)->fx_pos()-1,bt->dev_player(local_player)->fy_pos()-1,local_player);
		muestra_marcador(pant,bt->dev_player(local_player),5,tamy-MARCADOR1Y-5);
		if(bt->es_deathmatch()) muestra_ranking_deathmatch(pant,bt);
		muestra_mensajes(pant,bt,getMaskFromPlayerId(local_player));
		if(bt->ventana_chat_abierta())
		{
			al_set_target_bitmap(pant);
			ventana(pant, 0, tamy-80, tamx, tamy, BMAR3, BMAR2, false);
			texto(pant, bt->dev_mensaje_chat(), 30, tamy-45, BBLAN, true);
			if((int)al_get_time() % 2 == 0)
					texto(pant,"|",30+(strlen(bt->dev_mensaje_chat())*13),tamy-45,BBLAN,true);
		}
	  }
	  else
      switch(bt->num_players()){
            // Un jugador
            case 1 : muestra(pant,bt,tamx,tamy,bt->centroplx()-1,bt->centroply()-1,0);
                     muestra_marcador(pant,bt->dev_player(0),5,tamy-MARCADOR1Y-5);
                     muestra_mensajes(pant,bt,PRIO1); break;
            // Dos jugadores
            case 2 : if(!bt->single_screen()){
                        // Ambos muy separados
						al_set_clipping_rectangle(0,0,tamx,tamy/2);
                        muestra(pant,bt,tamx,tamy/2,bt->dev_player(0)->fx_pos()-1,bt->dev_player(0)->fy_pos()-1,0);
                        muestra_marcador(pant,bt->dev_player(0),5,(tamy/2)-MARCADOR1Y-5);
                        muestra_mensajes(pant,bt,PRIO1);
						al_set_clipping_rectangle(0,tamy/2,tamx,tamy/2);
                        muestra(pant,bt,tamx,tamy/2,bt->dev_player(1)->fx_pos()-1,bt->dev_player(1)->fy_pos()-1,1);
                        muestra_marcador(pant,bt->dev_player(1),5,(tamy/2)-MARCADOR1Y-5);
                        muestra_mensajes(pant,bt,PRIO2);
                        }else{
                        // Ambos cerca
                        muestra(pant,bt,tamx,tamy,bt->centroplx()-1,bt->centroply()-1,-1);
                        muestra_marcador(pant,bt->dev_player(0),5,tamy-MARCADOR1Y-5);
                        muestra_marcador(pant,bt->dev_player(1),tamx-MARCADOR1X-MARCADOR2X-5,tamy-MARCADOR1Y-5);
                        muestra_mensajes(pant,bt,PRIO1|PRIO2);};
                     break;
            //// Tres jugadores
            case 3 : if(!bt->single_screen()){
                        // 1 y 2 lejos de 3
                        if((bt->dist_players(0,1)>(rx*rx/4)) || (bt->force_split)){
                           // 1 lejos de 2
							al_set_clipping_rectangle(0,0,tamx/2,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(0)->fx_pos()-1,bt->dev_player(0)->fy_pos()-1,0);
                           muestra_marcador(pant,bt->dev_player(0),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO1);
						   al_set_clipping_rectangle(tamx/2,0,tamx,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(1)->fx_pos()-1,bt->dev_player(1)->fy_pos()-1,1);
                           muestra_marcador(pant,bt->dev_player(1),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO2);
                        }else{
                           // 1 cerca de 2
						   al_set_clipping_rectangle(0,0,tamx,tamy/2);
                           muestra(pant,bt,tamx,tamy/2,((bt->dev_player(0)->fx_pos()+bt->dev_player(1)->fx_pos())/2)-1,
                                                       ((bt->dev_player(0)->fy_pos()+bt->dev_player(1)->fy_pos())/2)-1,-1);
                           muestra_marcador(pant,bt->dev_player(0),5,tamy/2-MARCADOR1Y-5);
                           muestra_marcador(pant,bt->dev_player(1),tamx-MARCADOR1X-MARCADOR2X-5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO1|PRIO2);};

					 al_set_clipping_rectangle(0,tamy/2,tamx,tamy/2);
                     muestra(pant,bt,tamx,tamy/2,bt->dev_player(2)->fx_pos()-1,bt->dev_player(2)->fy_pos()-1,2);
                     muestra_marcador(pant,bt->dev_player(2),5,tamy/2-MARCADOR1Y-5);
                     muestra_mensajes(pant,bt,PRIO3);
                     }else{
                         // Los 3 cerca
                         muestra(pant,bt,tamx,tamy,bt->centroplx()-1,bt->centroply()-1,-1);
                         muestra_marcador(pant,bt->dev_player(0),5,tamy-MARCADOR1Y-5);
                         muestra_marcador(pant,bt->dev_player(1),tamx/2-(MARCADOR1X+MARCADOR2X)/2,tamy-MARCADOR1Y-5);
                         muestra_marcador(pant,bt->dev_player(2),tamx-MARCADOR1X-MARCADOR2X-5,tamy-MARCADOR1Y-5);
                         muestra_mensajes(pant,bt,PRIO1|PRIO2|PRIO3);};
                     break;
            //// Cuatro jugadores !!!
            case 4 : if(!bt->single_screen()){
                        // 1 y 2 lejos de 3 y 4
                        if((bt->dist_players(0,1)>(rx*rx/4)) || (bt->force_split)){
                           // 1 lejos de 2
						   al_set_clipping_rectangle(0,0,tamx/2,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(0)->fx_pos()-1,bt->dev_player(0)->fy_pos()-1,0);
                           muestra_marcador(pant,bt->dev_player(0),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO1);
						   al_set_clipping_rectangle(tamx/2,0,tamx/2,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(1)->fx_pos()-1,bt->dev_player(1)->fy_pos()-1,1);
                           muestra_marcador(pant,bt->dev_player(1),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO2);

                        }else{
                           // 1 cerca de 2
						   al_set_clipping_rectangle(0,0,tamx,tamy/2);
                           muestra(pant,bt,tamx,tamy/2,((bt->dev_player(0)->fx_pos()+bt->dev_player(1)->fx_pos())/2)-1,
                                                       ((bt->dev_player(0)->fy_pos()+bt->dev_player(1)->fy_pos())/2)-1,-1);
                           muestra_marcador(pant,bt->dev_player(0),5,tamy/2-MARCADOR1Y-5);
                           muestra_marcador(pant,bt->dev_player(1),tamx-MARCADOR1X-MARCADOR2X-5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO1|PRIO2);};
                        if((bt->dist_players(2,3)>(rx*rx/4)) || (bt->force_split)){
                           // 3 lejos de 4
						   al_set_clipping_rectangle(0,tamy/2,tamx/2,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(2)->fx_pos()-1,bt->dev_player(2)->fy_pos()-1,2);
                           muestra_marcador(pant,bt->dev_player(2),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO3);
						   al_set_clipping_rectangle(tamx/2,tamy/2,tamx/2,tamy/2);
                           muestra(pant,bt,tamx/2,tamy/2,bt->dev_player(3)->fx_pos()-1,bt->dev_player(3)->fy_pos()-1,3);
                           muestra_marcador(pant,bt->dev_player(3),5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO4);
                        }else{
                           // 3 cerca de 4
						   al_set_clipping_rectangle(0,tamy/2,tamx,tamy/2);
                           muestra(pant,bt,tamx,tamy/2,((bt->dev_player(2)->fx_pos()+bt->dev_player(3)->fx_pos())/2)-1,
                                                            ((bt->dev_player(2)->fy_pos()+bt->dev_player(3)->fy_pos())/2)-1,-1);
                           muestra_marcador(pant,bt->dev_player(2),5,tamy/2-MARCADOR1Y-5);
                           muestra_marcador(pant,bt->dev_player(3),tamx-MARCADOR1X-MARCADOR2X-5,tamy/2-MARCADOR1Y-5);
                           muestra_mensajes(pant,bt,PRIO3|PRIO4);
                             };
                     }else{
                         // Los 4 cerca
                         muestra(pant,bt,tamx,tamy,bt->centroplx()-1,bt->centroply()-1,-1);
                         muestra_marcador(pant,bt->dev_player(0),5,tamy-MARCADOR1Y-5);
                         muestra_marcador(pant,bt->dev_player(1),tamx/2-(MARCADOR1X+MARCADOR2X)-5,tamy-MARCADOR1Y-5);
                         muestra_marcador(pant,bt->dev_player(2),tamx/2+5,tamy-MARCADOR1Y-5);
                         muestra_marcador(pant,bt->dev_player(3),tamx-MARCADOR1X-MARCADOR2X-5,tamy-MARCADOR1Y-5);
                         muestra_mensajes(pant,bt,PRIO1|PRIO2|PRIO3|PRIO4);};
            };
			al_reset_clipping_rectangle();

}
void charset::muestra(ALLEGRO_BITMAP *pant,batalla *bt,int tamx, int tamy, float px, float py, int target_player)
{

        int ix,iy;
        float cx=tamx/2, cy=tamy/2, aux, ranx, rany;
        int x, y, i, j, k;
        mapa *mp=bt->dev_mapa();
        item *it;
        byte a=mp->anim();

		// Obtiene el clipping actual
		int clipx, clipy, clipw, cliph;
		al_get_clipping_rectangle(&clipx,&clipy,&clipw,&cliph);
		cx += clipx;
		cy += clipy;

        //Rango de baldosas a dibujar
        ranx=2*tamx/(BALX*RAIZ2)+2; rany=ranx;

        //Iluminaci¢n
        if((bt->lluvia) && (bt->trueno>TRCICLO-60))
             limpia_matriz_luz((byte)(MAXLUZ-(rand()%3)));
        else{
             limpia_matriz_luz(MEDLUZ);
             aplica_luz_batalla(bt);
        };

        for(i=0; i<3; i++)
        for(iy=int(py-rany); iy<=py+rany; iy+=1)
        for(ix=int(px-ranx); ix<=px+ranx; ix+=1){

            x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
            y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0));

            if((x>=clipx-2*BALX) && (x<=clipx+tamx+2*BALX) && (y>clipy-BALY) && (y<clipy+tamy+90))
            if((iy>=0) && (ix>=0) && (iy<MSIZEY) && (ix<MSIZEX)){

                  switch(i){
                  case 0 : muestra_baldosa(pant,mp,(bald)ix,(bald)iy,x,y); break;
                  case 1 : j=0;
                           // Muestra los jugadores en pantalla
                           while(bt->hay_player((bald)ix,(bald)iy,j)>=0){
                              k=bt->hay_player((bald)ix,(bald)iy,j);
                              if(bt->dev_player(k)->activo())
                                muestra_guerrero(pant,bt->dev_player(k),px,py,cx,cy,bt->dev_mapa()->suelo(bt->dev_player(k)->x_pos(),bt->dev_player(k)->y_pos()));
                              j=k+1;
                           };
                           j=0;
                           // Muestra los enemigos/secundarios
                           while(bt->hay_enem((bald)ix,(bald)iy,j)>=0){
                              k=bt->hay_enem((bald)ix,(bald)iy,j);
                              muestra_guerrero(pant,bt->dev_enem(k),px,py,cx,cy,bt->dev_mapa()->suelo(bt->dev_enem(k)->x_pos(),bt->dev_enem(k)->y_pos()));
                              j=k+1;
                           };

                           // Items recogibles
                           if(bt->dev_nivel()->hay_item(ix,iy)){
                              it=bt->dev_nivel()->dev_item(bt->dev_nivel()->primer_item(ix,iy));
                              muestra_item(pant,it,px,py,cx,cy,bt->dev_mapa()->suelo(it->x_pos(),it->y_pos()),a);
                                };

                           // Decorados, paredes, etc.
                           muestra_decorado(pant,mp,(bald)ix,(bald)iy,px,py,x,y);


                           // Balas en movimiento
                           j=0;
                           while(bt->hay_bala((bald)ix,(bald)iy,j)>=0){
                              k=bt->hay_bala((bald)ix,(bald)iy,j);
                              muestra_bala(pant,bt->dev_bala(k),bt->dev_nivel()->dev_mapa()->anim(),px,py,cx,cy);
                              j=k+1;
                           };

                          // Explosiones
                          j=0;
                          while(bt->hay_explos((bald)ix,(bald)iy,j)>=0){
                             k=bt->hay_explos((bald)ix,(bald)iy,j);
                             muestra_explos(pant,bt->dev_explos(k),px,py,cx,cy);
                             j=k+1;
                          };

                           break;
                  case 2 : muestra_luces(pant,mp,(bald)ix,(bald)iy,px,py,x,y); break;
                  };
            };
        };

		//Gotas de lluvia
		if(bt->lluvia)
		{
			___ENTER_PROFILER_BLOCK("charset::muestra_gota")
			for(j=0; j<MAXGO; j++)
					muestra_gota(pant,px,py,cx,cy,bt->bgota[j].x,bt->bgota[j].y,bt->bgota[j].z,bt->bgota[j].cont);
			___LEAVE_PROFILER_BLOCK("charset::muestra_gota")
		}

		// Fade a rojo al morir
		if(target_player >= 0)
		{
            warrior *local_pl = bt->dev_player(target_player);
            if(local_pl->estado() == DIE || local_pl->estado() == DEST)
            {
                byte alpha = local_pl->contador() * 3;
                if (alpha > 192) alpha = 192;
                //fprintf(stderr,"Contador: %d, Alpha: %d\n", local_pl->contador(), alpha);
                al_set_target_bitmap(pant);
                set_blender_for_transparent();
                al_draw_filled_rectangle(0,0,al_get_bitmap_width(pant),al_get_bitmap_height(pant),al_map_rgba(255,0,0,alpha));
            }
        }

        //Heads up display de los jugadores
        for(k = 0; k < MAXPL; k++)
        if(k != target_player && bt->dev_player(k) && bt->dev_player(k)->activo())
            muestra_hud_guerrero(pant,bt->dev_player(k),px,py,cx,cy);
}
// Muestra en la pantalla pant en (sx,sy) la baldosa correspondiente a la posici¢n
// (px,py) del mapa mp.
void charset::muestra_baldosa(ALLEGRO_BITMAP *pant, mapa *mp, bald px, bald py, int sx, int sy)
{

      ALLEGRO_BITMAP *spr=NULL;
      byte a,l=consulta_luz(px,py)-1;

      a=mp->anim();
      switch(mp->consb(px,py)){

            case BSU1 : spr=baldnorm[0][l]; break;
            case BSU2 : spr=baldnorm[1][l]; break;
            case BSU3 : spr=baldnorm[2][l]; break;
            case BSU4 : spr=baldnorm[3][l]; break;
            case BSU5 : spr=baldnorm[4][l]; break;
            case BDA1 : spr=balddan[0][a][l]; break;
            case BDA2 : spr=balddan[1][a][l]; break;
            case BMO1 : spr=baldmort[0][a][l]; break;
            case BMO2 : spr=baldmort[1][a][l]; break;};

      if(mp->hay_puente(px,py)>=0) spr=baldnorm[0][l];

      if(spr!=NULL){
         //draw_sprite(pant,spr,sx,sy);
		 al_draw_bitmap(spr,sx,sy,0);
      };

}
// Muestra en la pantalla pant el decorado de la baldosa (px,py) en las coordenadas (sx,sy).
// Efectua transparencias para aquellas paredes que oculten la visi¢n del centro (cx,cy).
void charset::muestra_decorado(ALLEGRO_BITMAP *pant, mapa *mp, bald px, bald py, float cx, float cy, int sx, int sy)
{

      byte a,r,l;
      int i=-1;
      bool ec=FALSE,el=FALSE,er=FALSE,eu=FALSE,ed=FALSE, dib=FALSE;
      ALLEGRO_BITMAP *spr=0;

      a=mp->anim();
      // Contenido de las puertas
      l=consulta_luz(px,py)-1;
      i=mp->hay_puerta(px,py);
      if(i>=0){
               r=mp->dev_puerta(i)->abertura();
               if(mp->dev_puerta(i)->horiz()){
               switch(mp->dev_puerta(i)->tipo()){
                        /*case INTER_FIJA :
                        case INTER :
                        case GEMA :
                        case NORM : masked_blit(puertanorm[1][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA1 : masked_blit(puertallav[1][0][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA2 : masked_blit(puertallav[1][1][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA3 : masked_blit(puertallav[1][2][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA4 : masked_blit(puertallav[1][3][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;*/
						case INTER_FIJA :
                        case INTER :
                        case GEMA :
                        case NORM : al_draw_bitmap_region(puertanorm[1][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA1 : al_draw_bitmap_region(puertallav[1][0][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA2 : al_draw_bitmap_region(puertallav[1][1][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA3 : al_draw_bitmap_region(puertallav[1][2][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA4 : al_draw_bitmap_region(puertallav[1][3][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
                        };
               }else{
               switch(mp->dev_puerta(i)->tipo()){
                        /*case INTER_FIJA :
                        case INTER :
                        case GEMA :
                        case NORM : masked_blit(puertanorm[0][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA1 : masked_blit(puertallav[0][0][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA2 : masked_blit(puertallav[0][1][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA3 : masked_blit(puertallav[0][2][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;
                        case LLA4 : masked_blit(puertallav[0][3][l],pant,0,r+10,sx+13,sy-82+10,PUEX,PUEY-r-10); break;*/
						case INTER_FIJA :
                        case INTER :
                        case GEMA :
                        case NORM : al_draw_bitmap_region(puertanorm[0][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA1 : al_draw_bitmap_region(puertallav[0][0][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA2 : al_draw_bitmap_region(puertallav[0][1][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA3 : al_draw_bitmap_region(puertallav[0][2][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
						case LLA4 : al_draw_bitmap_region(puertallav[0][3][l],0,r+10,PUEX,PUEY-r-10,sx+13,sy-82+10,0); break;
                        };
               };
      };
      // Paredes horizontales y puertas
      dib=FALSE;
      l=consulta_luz(px,py+1)-1;
      if(cy>=py){
      switch(mp->conse(px,py)){
            case PHAT :
            case PHO1 : al_draw_bitmap(parednorm[0][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO2 : al_draw_bitmap(parednorm[1][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO3 : al_draw_bitmap(parednorm[2][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO4 : al_draw_bitmap(parednorm[3][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO5 : al_draw_bitmap(parednorm[4][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO6 : al_draw_bitmap(parednorm[5][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO7 : al_draw_bitmap(paredanim[0][a][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO8 : al_draw_bitmap(paredanim[1][a][1][l],sx,sy-85,0); dib=TRUE; break;
            case PHO9 : al_draw_bitmap(paredanim[2][a][1][l],sx,sy-85,0); dib=TRUE; break;
      };
      }else{
      //color_map=ttrans;
	  float dist = py - cy;
	  byte alpha = dist > 1.f ? 128 : 255 - (127 * dist);
	  ALLEGRO_COLOR trans = al_map_rgba(alpha,alpha,alpha,alpha);
      switch(mp->conse(px,py)){
            case PHAT :
            case PHO1 : al_draw_tinted_bitmap(parednorm[0][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO2 : al_draw_tinted_bitmap(parednorm[1][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO3 : al_draw_tinted_bitmap(parednorm[2][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO4 : al_draw_tinted_bitmap(parednorm[3][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO5 : al_draw_tinted_bitmap(parednorm[4][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO6 : al_draw_tinted_bitmap(parednorm[5][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO7 : al_draw_tinted_bitmap(paredanim[0][a][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO8 : al_draw_tinted_bitmap(paredanim[1][a][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PHO9 : al_draw_tinted_bitmap(paredanim[2][a][1][l],trans,sx,sy-85,0); dib=TRUE; break;
            };
      };

      // Paredes verticales
      dib=FALSE;
      l=consulta_luz(px+1,py)-1;
      if(cx>=px){
      switch(mp->conse(px,py)){
            case PVAT :
            case PVE1 : al_draw_bitmap(parednorm[0][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE2 : al_draw_bitmap(parednorm[1][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE3 : al_draw_bitmap(parednorm[2][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE4 : al_draw_bitmap(parednorm[3][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE5 : al_draw_bitmap(parednorm[4][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE6 : al_draw_bitmap(parednorm[5][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE7 : al_draw_bitmap(paredanim[0][a][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE8 : al_draw_bitmap(paredanim[1][a][0][l],sx,sy-85,0); dib=TRUE; break;
            case PVE9 : al_draw_bitmap(paredanim[2][a][0][l],sx,sy-85,0); dib=TRUE; break;
      };
      }else{
		float dist = px - cx;
		byte alpha = dist > 1.f ? 128 : 255 - (127 * dist);
		ALLEGRO_COLOR trans = al_map_rgba(alpha,alpha,alpha,alpha);
		switch(mp->conse(px,py)){
            case PVAT :
            case PVE1 : al_draw_tinted_bitmap(parednorm[0][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE2 : al_draw_tinted_bitmap(parednorm[1][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE3 : al_draw_tinted_bitmap(parednorm[2][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE4 : al_draw_tinted_bitmap(parednorm[3][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE5 : al_draw_tinted_bitmap(parednorm[4][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE6 : al_draw_tinted_bitmap(parednorm[5][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE7 : al_draw_tinted_bitmap(paredanim[0][a][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE8 : al_draw_tinted_bitmap(paredanim[1][a][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            case PVE9 : al_draw_tinted_bitmap(paredanim[2][a][0][l],trans,sx,sy-85,0); dib=TRUE; break;
            };
      };

      // Marcos de puertas
      l=consulta_luz(px,py)-1;
      switch(mp->conse(px,py)){
           /* case DM1H : draw_sprite_ilum_h_flip(pant,marclat1[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DM2H : draw_sprite_ilum_h_flip(pant,marclat2[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DTRH : draw_sprite_ilum_h_flip(pant,marcsup[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DARH : draw_sprite_ilum_h_flip(pant,marcarc[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;*/
			case DM1H : al_draw_bitmap(marclat1[l],sx,sy-85, ALLEGRO_FLIP_HORIZONTAL); break;
			case DM2H : al_draw_bitmap(marclat2[l],sx,sy-85, ALLEGRO_FLIP_HORIZONTAL); break;
			case DTRH : al_draw_bitmap(marcsup[l],sx,sy-85, ALLEGRO_FLIP_HORIZONTAL); break;
			case DARH : al_draw_bitmap(marcarc[l],sx,sy-85, ALLEGRO_FLIP_HORIZONTAL); break;

            };

      switch(mp->conse(px,py)){
           /* case DM1V : draw_sprite_ilum(pant,marclat1[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DM2V : draw_sprite_ilum(pant,marclat2[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DTRV : draw_sprite_ilum(pant,marcsup[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;
            case DARV : draw_sprite_ilum(pant,marcarc[l],sx,sy-85,consulta_luz(px,py),ilum_trans,tluz,tsombra); break;*/
			case DM1V : al_draw_bitmap(marclat1[l],sx,sy-85, 0); break;
			case DM2V : al_draw_bitmap(marclat2[l],sx,sy-85, 0); break;
			case DTRV : al_draw_bitmap(marcsup[l],sx,sy-85, 0); break;
			case DARV : al_draw_bitmap(marcarc[l],sx,sy-85, 0); break;
      };

      //Decorados
      l=consulta_luz(px,py)-1;
      dib=FALSE;
      switch(mp->conse(px,py)){
            case INTE : i=mp->hay_interr(px,py);
                        if(i>=0){
                        if((mp->dev_interr(i))->on()) spr=interr[1][l];
                        else spr=interr[0][l]; dib=TRUE; }; break;
            case TELE : spr=telep[a][l]; dib=TRUE; break;
            case DEC1 : spr=decornorm[0][l]; dib=TRUE; break;
            case DEC2 : spr=decornorm[1][l]; dib=TRUE; break;
            case DEC3 : spr=decornorm[2][l]; dib=TRUE; break;
            case DEC4 : spr=decornorm[3][l]; dib=TRUE; break;
            case DEC5 : spr=decornorm[4][l]; dib=TRUE; break;
            case DEC6 : spr=decornorm[5][l]; dib=TRUE; break;
            case DEC7 : spr=decoranim[0][a][l]; dib=TRUE; break;
            case DEC8 : spr=decoranim[1][a][l]; dib=TRUE; break;
            case DEC9 : spr=decoranim[2][a][l]; dib=TRUE; break;
            case EXIT : spr=extra[l]; dib=TRUE; break;
            };
      if(dib==TRUE) al_draw_bitmap(spr,sx,sy-30,0);

      // Columnas
      dib=FALSE;
      switch(mp->conse(px,py)){
            case COL1 : spr=columna[0][l]; dib=TRUE; break;
            case COL2 : spr=columna[1][l]; dib=TRUE; break;
            case COL3 : spr=columna[2][l]; dib=TRUE; break;
            };
      if(dib==TRUE) al_draw_bitmap(spr,sx,sy-90,0);

      // Esquinas
      l=consulta_luz(px,py)-1;
      switch(mp->conse(px,py)){
            case ECRU : ec=TRUE; el=TRUE; er=TRUE; eu=TRUE; ed=TRUE; break;
            case EL_D : ec=TRUE; el=TRUE; ed=TRUE; break;
            case ER_D : ec=TRUE; er=TRUE; ed=TRUE; break;
            case EL_U : ec=TRUE; el=TRUE; eu=TRUE; break;
            case ER_U : ec=TRUE; er=TRUE; eu=TRUE; break;
            case EULD : ec=TRUE; eu=TRUE; el=TRUE; ed=TRUE; break;
            case ERUL : ec=TRUE; er=TRUE; eu=TRUE; el=TRUE; break;
            case EURD : ec=TRUE; eu=TRUE; er=TRUE; ed=TRUE; break;
            case ELDR : ec=TRUE; el=TRUE; ed=TRUE; er=TRUE; break;
      };
      //Caso de esquina : Construimos el sprite de la esquina
      if(el) al_draw_bitmap(esqtras[l],sx+8,sy-85,0);
      if(eu) al_draw_bitmap(esqtras[l],sx+30,sy-85, ALLEGRO_FLIP_HORIZONTAL );
      if(ec) al_draw_bitmap(esqbase[l],sx,sy-80, 0);
      if(er) al_draw_bitmap(esqdel[l],sx+30,sy-74, 0);
      if(ed) al_draw_bitmap(esqdel[l],sx+8,sy-74, ALLEGRO_FLIP_HORIZONTAL);

      // Barreras

      l=consulta_luz(px,py)-1;
      i=mp->hay_barr(px,py);
      if((i>=0) && (mp->dev_barr(i)->danyo()<3)){
            //draw_trans_sprite2(pant,sombra,sx,sy+5,tsombra);
			al_draw_bitmap(sombra,sx,sy+5,0);
            if(mp->dev_barr(i)->tipo()==0)
            al_draw_bitmap(barrera[mp->dev_barr(i)->danyo()][l],sx,sy-60,0);
            else
            al_draw_bitmap(hielo[mp->dev_barr(i)->danyo()][l],sx,sy-60,0);
      };


      //Poste de l ser
      switch(mp->conse(px,py)){
            case LPIZ : al_draw_bitmap(laserpost[0][l],sx,sy-30,0); break;
            case LPDE : al_draw_bitmap(laserpost[1][l],sx,sy-30,0); break;};
      // L ser
      //color_map=ttrans;
      i=mp->hay_laser(px,py);
      if(i>=0){
            if(mp->dev_laser(i)->horiz()) al_draw_tinted_bitmap(laser[0][a],al_map_rgba(255,255,255,128),sx,sy-30,0);
            else                          al_draw_tinted_bitmap(laser[1][a],al_map_rgba(255,255,255,128),sx,sy-30,0);
      };
      //Poste de L ser
      switch(mp->conse(px,py)){
            case LPAR : al_draw_bitmap(laserpost[0][l],sx,sy-30,ALLEGRO_FLIP_HORIZONTAL); break;
            case LPAB : al_draw_bitmap(laserpost[1][l],sx,sy-30,ALLEGRO_FLIP_HORIZONTAL); break;
      // Tirador
            case TIAR : al_draw_bitmap(tirador[1][l],sx,sy-70,ALLEGRO_FLIP_HORIZONTAL); break;
            case TIAB : al_draw_bitmap(tirador[0][l],sx,sy-70,ALLEGRO_FLIP_HORIZONTAL); break;
            case TIIZ : al_draw_bitmap(tirador[1][l],sx,sy-70,0); break;
            case TIDE : al_draw_bitmap(tirador[0][l],sx,sy-70,0); break;
      };

}
void charset::muestra_luces(ALLEGRO_BITMAP *pant, mapa *mp, bald px, bald py, int cx, int cy, int sx, int sy)
{

      byte a;

      a=mp->anim();
      // Antorchas en paredes
      switch(mp->conse(px,py)){
            case PVE7 : al_draw_bitmap(aura[a],sx+5,sy-85,0); break; //draw_trans_sprite2(pant,aura[a],sx+5,sy-85,tluz); break;
            case PHO7 : al_draw_bitmap(aura[a],sx-15,sy-85,0); break; //draw_trans_sprite2(pant,aura[a],sx-15,sy-85,tluz); break;
      };
}
void charset::muestra_guerrero(ALLEGRO_BITMAP *pant, warrior *w, float px, float py, int cx, int cy, bool suelo)
{
      int x,y,ax,ay,c=w->cualspr();
      float ix=w->fx_pos()-0.5, iy=w->fy_pos()-0.5,
            fix=w->front_x(0.6)-0.5, fiy=w->front_y(0.6)-0.5;
      ALLEGRO_BITMAP *cuerpo = 0, *piernas = 0;
      bool inv;
      byte c1=w->color(1), c2=w->color(2), luz=consulta_luz(w->x_pos(),w->y_pos());
      int p;
//      if(w->special(SHADOW)) return;
      if(w->cualspr()>NSPR) return;

	  skin *mi_skin = skins[w->cualskin()];

	  if(w->cualspr() == NSPR) mi_skin = skins[congelado_skin];

      x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
      y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0));
      if(!suelo) y-=8;

      ax=(int)floor(cx-15.0+((fix-px)*30.0)-((fiy-py)*30.0));
      ay=(int)floor(cy-7.5+((fix-px)*15.0)+((fiy-py)*15.0));
      if(!suelo) ay-=8;

      if((w->estado()==OUTL) && (w->contador()>=WTTEL)) return;

      //Sombra
      //draw_trans_sprite2(pant,sombra,x,y,tsombra);
	  al_draw_bitmap(sombra,x,y,0);

      //Completo
      switch(w->estado()){
      case DIE : if((w->dir()==DDER) || (w->dir()==DIZQ)){
            if(w->contador()<WTCAE/2)
				//draw_sprite_colored(pant,t_muerte1[c][0],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
				draw_sprite_ilum(pant,mi_skin->t_muerte1[0],x,y-60,luz,ilum_trans,tluz,tsombra);
            else{
                 if(w->contador()<WTCAE)
					 //draw_sprite_colored(pant,t_muerte1[c][1],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum(pant,mi_skin->t_muerte1[1],x,y-60,luz,ilum_trans,tluz,tsombra);
                 else{
                     al_draw_bitmap(sombra,x+30,y+15,0);
                      //draw_sprite_colored(pant,t_muerte1[c][2],x,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum(pant,mi_skin->t_muerte1[2],x,y-30,luz,ilum_trans,tluz,tsombra);
				 };
            };
         }else{
            if(w->contador()<WTCAE/2)
				//draw_sprite_colored_h_flip(pant,t_muerte1[c][0],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
				draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte1[0],x,y-60,luz,ilum_trans,tluz,tsombra);
            else{
                 if(w->contador()<WTCAE)
					 //draw_sprite_colored_h_flip(pant,t_muerte1[c][1],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte1[1],x,y-60,luz,ilum_trans,tluz,tsombra);
                 else{
                      al_draw_bitmap(sombra,x-30,y+15,0);
                      //draw_sprite_colored_h_flip(pant,t_muerte1[c][2],x-30,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);
					  draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte1[2],x-30,y-30,luz,ilum_trans,tluz,tsombra);
				 };
            };
         }; break;
      case DEST : if((w->dir()==DDER) || (w->dir()==DIZQ)){
            if(w->contador()<WTCAE/2)
				//draw_sprite_colored(pant,t_muerte2[c][0],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
				draw_sprite_ilum(pant,mi_skin->t_muerte2[0],x,y-60,luz,ilum_trans,tluz,tsombra);
            else{
                 if(w->contador()<WTCAE)
					 //draw_sprite_colored(pant,t_muerte2[c][1],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum(pant,mi_skin->t_muerte2[1],x,y-60,luz,ilum_trans,tluz,tsombra);
                 else{
                      //draw_trans_sprite2(pant,sombra,x+30,y+15,tsombra);
					 al_draw_bitmap(sombra,x+30,y+15,0);
                      //draw_sprite_colored(pant,t_muerte2[c][2],x,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum(pant,mi_skin->t_muerte2[2],x,y-30,luz,ilum_trans,tluz,tsombra);
				 };
            };
         }else{
            if(w->contador()<WTCAE/2)
				//draw_sprite_colored_h_flip(pant,t_muerte2[c][0],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
				draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte2[0],x,y-60,luz,ilum_trans,tluz,tsombra);
            else{
                 if(w->contador()<WTCAE)
					 //draw_sprite_colored_h_flip(pant,t_muerte2[c][1],x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte2[1],x,y-60,luz,ilum_trans,tluz,tsombra);
                 else{
                      //draw_trans_sprite2(pant,sombra,x-30,y+15,tsombra);
					 al_draw_bitmap(sombra,x-30,y+15,0);
                      //draw_sprite_colored_h_flip(pant,t_muerte2[c][2],x-30,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);
					 draw_sprite_ilum_h_flip(pant,mi_skin->t_muerte2[2],x-30,y-30,luz,ilum_trans,tluz,tsombra);
				 };
            };
         }; break;
      };

      if((w->estado()==WALK) || (w->estado()==OUTL) || (w->estado()==CONG) || (w->estado()==SLAS)){
                     //Escudos Omega
                     if((w->dir()==DIZQ) && (w->clase()==OMEGA) && (w->escudo()>0) && (w->estado()!=TELP) && ((w->contador()==0) || (w->estado()==SLAS)) && (w->estado()!=DEFE))
                           draw_sprite_ilum(pant,escudos[w->escudo()-1][1],x+15,y-50,luz,ilum_trans,tluz,tsombra);
                     if((w->dir()==DARR) && (w->clase()==OMEGA) && (w->escudo()>0) && (w->estado()!=TELP) && ((w->contador()==0) || (w->estado()==SLAS)) && (w->estado()!=DEFE))
                           draw_sprite_ilum_h_flip(pant,escudos[w->escudo()-1][1],x+15,y-50,luz,ilum_trans,tluz,tsombra);
      };
      //Arma y escudo en posici¢n de defensa
      if((w->clase()==OMEGA) && ((w->estado()==DEFE) || ((w->estado()==WALK) && (w->contador()>0)))){
                        if(w->dir()==DIZQ){
                           if(w->escudo()>0) draw_sprite_ilum(pant,escudos[w->escudo()-1][1],x+11,y-48,luz,ilum_trans,tluz,tsombra);
                           if(w->arma()>0)   draw_sprite_ilum(pant,a_omega[w->arma()-1][4],x-20,y-70,luz,ilum_trans,tluz,tsombra);};
                        if(w->dir()==DARR){
                           if(w->escudo()>0) draw_sprite_ilum_h_flip(pant,escudos[w->escudo()-1][1],x+21,y-48,luz,ilum_trans,tluz,tsombra);
                           if(w->arma()>0)   draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][4],x+20,y-70,luz,ilum_trans,tluz,tsombra);};
      };

      //Piernas
      switch(w->estado()){

         case OUTL :
         case CONG :
         case HECH :
         case DEFE :
         case SLAS :
         case TELP :
         case PUNY :
         case SHOT :
         case WALK : switch(w->dir()){
                        case DABA : piernas=mi_skin->p_normal[0]; inv=TRUE; break;
                        case DDER : piernas=mi_skin->p_normal[0]; inv=FALSE; break;
                        case DIZQ : piernas=mi_skin->p_normal[1]; inv=TRUE; break;
                        case DARR : piernas=mi_skin->p_normal[1]; inv=FALSE; break; };
                     if(w->en_movim())
                     switch(w->dir()){
                        case DDER :
                        case DABA : piernas=mi_skin->p_andar[0][w->anim()]; break;
                        case DIZQ :
                        case DARR : piernas=mi_skin->p_andar[1][w->anim()]; break;};
                     // mostrar piernas
                     switch(w->estado()){
                        case TELP :
                        case OUTL :
									set_blender_for_transparent();
									al_draw_tinted_bitmap(piernas,al_map_rgba_f(1.f,1.f,1.f,1.f-(w->contador()/50.f)),x,y-30,inv ? 0 : ALLEGRO_FLIP_HORIZONTAL);
									restore_default_blender();
									//if(inv) draw_sprite_colored_rand(pant,piernas,x,y-30,c1,c2,w->contador()/2);
                                    //else draw_sprite_colored_rand_h_flip(pant,piernas,x,y-30,c1,c2,w->contador()/2);
									break;
                        default   : if(w->special(INVIS)){
                                        //if(inv) draw_sprite_colored_rand(pant,piernas,x,y-30,c1,c2,10);
                                        //else draw_sprite_colored_rand_h_flip(pant,piernas,x,y-30,c1,c2,10);
										set_blender_for_transparent();
										al_draw_tinted_bitmap(piernas,al_map_rgba_f(1.f,1.f,1.f,0.5f),x,y-30,inv ? 0 : ALLEGRO_FLIP_HORIZONTAL);
										restore_default_blender();
                                    }else{
                                        /*if(inv) draw_sprite_colored(pant,piernas,x,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);
                                        else draw_sprite_colored_h_flip(pant,piernas,x,y-30,c1,c2,luz,ilum_trans,tluz,tsombra);*/
										if(inv) draw_sprite_ilum(pant,piernas,x,y-30,luz,ilum_trans,tluz,tsombra);
										else draw_sprite_ilum_h_flip(pant,piernas,x,y-30,luz,ilum_trans,tluz,tsombra);
                                    }; break;
                     };
                     break;
      };

      //Cuerpo

      switch(w->estado()){

         case OUTL :
         case CONG :
         case DEFE :
         case SHOT :
         case TELP :
         case WALK :
                     // Normal en guardia
			 if((w->clase()==SIGMA) || (w->clase()==CASUAL) || ((w->clase()==OMEGA) && (w->arma()==0))
					|| (w->estado()==DEFE) || ((w->clase()==BETA) && (w->arma()>0))
					|| ((w->clase()==BETA) && (w->arma() == 0) && (!w->tiene_baston_mago())))
                     switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_normal[0]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_normal[0]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_normal[1]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_normal[1]; inv=FALSE; break; };

                     // Omega con arma blanca o Beta con bast¢n
                     if(((w->clase()==OMEGA) && (w->arma()>0) && (w->estado()!=DEFE)) ||
						 ((w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago())))
                     switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_guardia[0]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_guardia[0]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_guardia[1]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_guardia[1]; inv=FALSE; break; };

                     // En da¤o
                     if((w->contador()>0) && (w->estado()==WALK))
                     switch(w->dir()){
                        case DDER :
                        case DABA : cuerpo=mi_skin->c_danyo[0]; break;
                        case DIZQ :
                        case DARR : cuerpo=mi_skin->c_danyo[1]; break; };


                     // Bast¢n beta si en da¤o
                     if((w->estado()==WALK) && (w->contador()>0) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago())){
                     if(w->dir()==DIZQ)
                     draw_sprite_ilum(pant,baston[4],x-20,y-70,luz,ilum_trans,tluz,tsombra);
                     if(w->dir()==DARR)
                     draw_sprite_ilum_h_flip(pant,baston[4],x+20,y-70,luz,ilum_trans,tluz,tsombra);
                     };

                     // Armas sigma
                     if((w->dir()==DIZQ) && (w->clase()==SIGMA) && (w->arma()>0) && (w->estado()!=TELP))
                           draw_sprite_ilum(pant,a_sigma[w->arma()-1][0],ax,ay-40,consulta_luz(ix-1,iy),ilum_trans,tluz,tsombra);
                     if((w->dir()==DARR) && (w->clase()==SIGMA) && (w->arma()>0) && (w->estado()!=TELP))
                           draw_sprite_ilum_h_flip(pant,a_sigma[w->arma()-1][0],ax,ay-40,consulta_luz(ix,iy-1),ilum_trans,tluz,tsombra);

                     //Armas Omega
                     if((w->dir()==DIZQ) && (w->clase()==OMEGA) && (w->arma()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum(pant,a_omega[w->arma()-1][3],x-34,y-50,luz,ilum_trans,tluz,tsombra);
                     if((w->dir()==DARR) && (w->clase()==OMEGA) && (w->arma()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][3],x+35,y-50,luz,ilum_trans,tluz,tsombra);

                     //Bast¢n Beta
					 if((w->dir()==DIZQ) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago()) && (w->estado()!=TELP) && (w->contador()==0))
                           draw_sprite_ilum(pant,baston[3],x-34,y-50,luz,ilum_trans,tluz,tsombra);
					 if((w->dir()==DARR) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago()) && (w->estado()!=TELP) && (w->contador()==0))
                           draw_sprite_ilum_h_flip(pant,baston[3],x+35,y-50,luz,ilum_trans,tluz,tsombra);


                     // mostrar cuerpo
					 if(cuerpo)
                     switch(w->estado()){
                        case TELP :
                        case OUTL : //if(inv) draw_sprite_colored_rand(pant,cuerpo,x,y-60,c1,c2,w->contador()/2);
                                    //else draw_sprite_colored_rand_h_flip(pant,cuerpo,x,y-60,c1,c2,w->contador()/2);
									set_blender_for_transparent();
									al_draw_tinted_bitmap(cuerpo,al_map_rgba_f(1.f,1.f,1.f,1.f-(w->contador()/50.f)),x,y-60,inv ? 0 : ALLEGRO_FLIP_HORIZONTAL);
									restore_default_blender();
									break;
                        default   : if(w->special(INVIS)){
                                        //if(inv) draw_sprite_colored_rand(pant,cuerpo,x,y-60,c1,c2,10);
                                        //else draw_sprite_colored_rand_h_flip(pant,cuerpo,x,y-60,c1,c2,10);
										set_blender_for_transparent();
										al_draw_tinted_bitmap(cuerpo,al_map_rgba_f(1.f,1.f,1.f,0.5f),x,y-60,inv ? 0 : ALLEGRO_FLIP_HORIZONTAL);
										restore_default_blender();
                                    }else{
                                        //if(inv) draw_sprite_colored(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
                                        //else draw_sprite_colored_h_flip(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
										if (inv) draw_sprite_ilum(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
										else draw_sprite_ilum_h_flip(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);

                                    }; break;
                     };

                     // C psulas de poder especial
                     //color_map=ttrans;
					 set_blender_for_transparent();
                     if(w->special(INVUL)) al_draw_tinted_bitmap(capsulas[0],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(BERSERK)) al_draw_tinted_bitmap(capsulas[1],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(FEAR)) al_draw_tinted_bitmap(capsulas[2],al_map_rgba(255,255,255,128),x,y-60,0);
					 restore_default_blender();

                     // Electricidad
                     if(w->electrocutado()) al_draw_bitmap(explosi[4][rand()%3],x,y-60,0);

                     // Baston beta en da¤o
					 if((w->estado()==WALK) && (w->contador()>0) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago())){
                     if(w->dir()==DABA)
                     draw_sprite_ilum(pant,baston[1],x+2,y-65,luz,ilum_trans,tluz,tsombra);
                     if(w->dir()==DDER)
                     draw_sprite_ilum_h_flip(pant,baston[1],x-2,y-65,luz,ilum_trans,tluz,tsombra);
                     };

                     //Armas Sigma
                     if((w->dir()==DABA) && (w->clase()==SIGMA) && (w->arma()>0) && (w->estado()!=TELP))
                           draw_sprite_ilum(pant,a_sigma[w->arma()-1][1],ax,ay-40,consulta_luz(ix,iy+1),ilum_trans,tluz,tsombra);
					if((w->dir()==DDER) && (w->clase()==SIGMA) && (w->arma()>0) && (w->estado()!=TELP))
                           draw_sprite_ilum_h_flip(pant,a_sigma[w->arma()-1][1],ax,ay-40,consulta_luz(ix+1,iy),ilum_trans,tluz,tsombra);


                     //Escudos Omega
                     if((w->dir()==DABA) && (w->clase()==OMEGA) && (w->escudo()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum(pant,escudos[w->escudo()-1][0],x-5,y-45,luz,ilum_trans,tluz,tsombra);
                     if((w->dir()==DDER) && (w->clase()==OMEGA) && (w->escudo()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum_h_flip(pant,escudos[w->escudo()-1][0],x+35,y-45,consulta_luz(ix-1,iy),ilum_trans,tluz,tsombra);

                     //Armas Omega
                     if((w->dir()==DABA) && (w->clase()==OMEGA) && (w->arma()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum(pant,a_omega[w->arma()-1][0],x+7,y-34,luz,ilum_trans,tluz,tsombra);
                     if((w->dir()==DDER) && (w->clase()==OMEGA) && (w->arma()>0) && (w->estado()!=TELP) && (w->contador()==0) && (w->estado()!=DEFE))
                           draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][0],x-7,y-34,luz,ilum_trans,tluz,tsombra);

                     //Bast¢n Beta
					 if((w->dir()==DABA) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago()) && (w->estado()!=TELP) && (w->contador()==0))
                           draw_sprite_ilum(pant,baston[0],x+7,y-34,luz,ilum_trans,tluz,tsombra);
					 if((w->dir()==DDER) && (w->clase()==BETA) && (w->arma()==0) && (w->tiene_baston_mago()) && (w->estado()!=TELP) && (w->contador()==0))
                           draw_sprite_ilum_h_flip(pant,baston[0],x-7,y-34,luz,ilum_trans,tluz,tsombra);


                     //Arma y escudo en posici¢n de defensa
                     if((w->clase()==OMEGA) && ((w->estado()==DEFE) || ((w->estado()==WALK) && (w->contador()>0)))){
                        if(w->dir()==DABA){
                           if(w->arma()>0)   draw_sprite_ilum(pant,a_omega[w->arma()-1][1],x+2,y-65,luz,ilum_trans,tluz,tsombra);
                           if(w->escudo()>0) draw_sprite_ilum(pant,escudos[w->escudo()-1][0],x+5,y-40,luz,ilum_trans,tluz,tsombra);};
                        if(w->dir()==DDER){
                           if(w->arma()>0)   draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][1],x-2,y-65,luz,ilum_trans,tluz,tsombra);
                           if(w->escudo()>0) draw_sprite_ilum_h_flip(pant,escudos[w->escudo()-1][0],x+25,y-40,luz,ilum_trans,tluz,tsombra);};
                     };

                     break;


         case PUNY : if(w->contador()<WPUNWAIT-4)
                     switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_punyo[0]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_punyo[0]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_punyo[1]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_punyo[1]; inv=FALSE; break; }
                     else
                     switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_normal[0]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_normal[0]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_normal[1]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_normal[1]; inv=FALSE; break; };

                       /*if(w->special(INVIS)){
                            if(inv) draw_sprite_colored_rand(pant,cuerpo,x,y-60,c1,c2,10);
                            else draw_sprite_colored_rand_h_flip(pant,cuerpo,x,y-60,c1,c2,10);
                        }else*/{
                            //if(inv) draw_sprite_colored(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
                            //else draw_sprite_colored_h_flip(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
							if (inv) draw_sprite_ilum(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
							else draw_sprite_ilum_h_flip(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
                        };
                     // C psulas de poder especial
                     //color_map=ttrans;
                      set_blender_for_transparent();
                     if(w->special(INVUL)) al_draw_tinted_bitmap(capsulas[0],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(BERSERK)) al_draw_tinted_bitmap(capsulas[1],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(FEAR)) al_draw_tinted_bitmap(capsulas[2],al_map_rgba(255,255,255,128),x,y-60,0);
					 restore_default_blender();
					 break;

         case HECH : switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_magia[0]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_magia[0]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_magia[1]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_magia[1]; inv=FALSE; break; };
                   /*if(w->special(INVIS)){
                        if(inv) draw_sprite_colored_rand(pant,cuerpo,x,y-60,c1,c2,10);
                        else draw_sprite_colored_rand_h_flip(pant,cuerpo,x,y-60,c1,c2,10);
                    }else*/{
                        //if(inv) draw_sprite_colored(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
                        //else draw_sprite_colored_h_flip(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
						 //al_draw_bitmap(cuerpo,x,y-60, inv ? 0 : ALLEGRO_FLIP_HORIZONTAL );
						 if (inv) draw_sprite_ilum(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
						else draw_sprite_ilum_h_flip(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
                    };
                     // C psulas de poder especial
                     //color_map=ttrans;
                      set_blender_for_transparent();
                     if(w->special(INVUL)) al_draw_tinted_bitmap(capsulas[0],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(BERSERK)) al_draw_tinted_bitmap(capsulas[1],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(FEAR)) al_draw_tinted_bitmap(capsulas[2],al_map_rgba(255,255,255,128),x,y-60,0);
					 restore_default_blender();
					 break;

         case SLAS : if(w->contador()<WTSLAS/3) p=0;
                     if((w->contador()>=WTSLAS/3) && (w->contador()<2*WTSLAS/3)) p=1;
                     if(w->contador()>=2*WTSLAS/3) p=2;

                     // Armas Omega
                     if((w->dir()==DIZQ) && (w->clase()==OMEGA))
                     switch(p){
                        case 0 : draw_sprite_ilum(pant,a_omega[w->arma()-1][4],x-15,y-104,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum(pant,a_omega[w->arma()-1][5],x-36,y-96,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum(pant,a_omega[w->arma()-1][3],x-43,y-60,luz,ilum_trans,tluz,tsombra); break;
                        };
                     if((w->dir()==DARR) && (w->clase()==OMEGA))
                     switch(p){
                        case 0 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][4],x+15,y-104,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][5],x+36,y-96,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][3],x+43,y-60,luz,ilum_trans,tluz,tsombra); break;
                        };
                     // Armas Omega
                     if((w->dir()==DIZQ) && (w->clase()==BETA))
                     switch(p){
                        case 0 : draw_sprite_ilum(pant,baston[4],x-15,y-104,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum(pant,baston[5],x-36,y-96,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum(pant,baston[3],x-43,y-60,luz,ilum_trans,tluz,tsombra); break;
                        };
                     //Bast¢n
                     if((w->dir()==DARR) && (w->clase()==BETA))
                     switch(p){
                        case 0 : draw_sprite_ilum_h_flip(pant,baston[4],x+15,y-104,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum_h_flip(pant,baston[5],x+36,y-96,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum_h_flip(pant,baston[3],x+43,y-60,luz,ilum_trans,tluz,tsombra); break;
                        };
                     switch(w->dir()){
                        case DABA : cuerpo=mi_skin->c_ataque[0][p]; inv=TRUE; break;
                        case DDER : cuerpo=mi_skin->c_ataque[0][p]; inv=FALSE; break;
                        case DIZQ : cuerpo=mi_skin->c_ataque[1][p]; inv=TRUE; break;
                        case DARR : cuerpo=mi_skin->c_ataque[1][p]; inv=FALSE; break; };
                     /*if(w->special(INVIS)){
                        if(inv) draw_sprite_colored_rand(pant,cuerpo,x,y-60,c1,c2,10);
                        else draw_sprite_colored_rand_h_flip(pant,cuerpo,x,y-60,c1,c2,10);
                     }else*/{
                        //if(inv) draw_sprite_colored(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
                        //else draw_sprite_colored_h_flip(pant,cuerpo,x,y-60,c1,c2,luz,ilum_trans,tluz,tsombra);
						if (inv) draw_sprite_ilum(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
						else draw_sprite_ilum_h_flip(pant,cuerpo,x,y-60,luz,ilum_trans,tluz,tsombra);
                     };
                     // C psulas de poder especial
                     //color_map=ttrans;
                      set_blender_for_transparent();
                     if(w->special(INVUL)) al_draw_tinted_bitmap(capsulas[0],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(BERSERK)) al_draw_tinted_bitmap(capsulas[1],al_map_rgba(255,255,255,128),x,y-60,0);
                     if(w->special(FEAR)) al_draw_tinted_bitmap(capsulas[2],al_map_rgba(255,255,255,128),x,y-60,0);
					 restore_default_blender();

                     //Escudos Omega
                     if((w->dir()==DABA) && (w->clase()==OMEGA) && (w->escudo()>0))
                           draw_sprite_ilum(pant,escudos[w->escudo()-1][0],x-5,y-45,luz,ilum_trans,tluz,tsombra);
                     if((w->dir()==DDER) && (w->clase()==OMEGA) && (w->escudo()>0))
                           draw_sprite_ilum_h_flip(pant,escudos[w->escudo()-1][0],x+35,y-45,luz,ilum_trans,tluz,tsombra);

                     // Armas Omega
                     if((w->dir()==DABA) && (w->clase()==OMEGA))
                     switch(p){
                        case 0 : draw_sprite_ilum(pant,a_omega[w->arma()-1][1],x+22,y-108,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum(pant,a_omega[w->arma()-1][2],x-18,y-101,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum(pant,a_omega[w->arma()-1][0],x-24,y-33,luz,ilum_trans,tluz,tsombra); break;
                        };
                     if((w->dir()==DDER) && (w->clase()==OMEGA))
                     switch(p){
                        case 0 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][1],x-22,y-108,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][2],x+18,y-101,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum_h_flip(pant,a_omega[w->arma()-1][0],x+24,y-33,luz,ilum_trans,tluz,tsombra); break;
                     };
                     // Bast¢n Beta
                     if((w->dir()==DABA) && (w->clase()==BETA))
                     switch(p){
                        case 0 : draw_sprite_ilum(pant,baston[1],x+22,y-108,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum(pant,baston[2],x-18,y-101,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum(pant,baston[0],x-24,y-33,luz,ilum_trans,tluz,tsombra); break;
                        };
                     if((w->dir()==DDER) && (w->clase()==BETA))
                     switch(p){
                        case 0 : draw_sprite_ilum_h_flip(pant,baston[1],x-22,y-108,luz,ilum_trans,tluz,tsombra); break;
                        case 1 : draw_sprite_ilum_h_flip(pant,baston[2],x+18,y-101,luz,ilum_trans,tluz,tsombra); break;
                        case 2 : draw_sprite_ilum_h_flip(pant,baston[0],x+24,y-33,luz,ilum_trans,tluz,tsombra); break;
                     };
                     break;
      };

      if(w->estado()==CONG){ //color_map=ttrans;
                             //draw_trans_sprite(pant,hielo[0][luz-1],x,y-60);
							 set_blender_for_transparent();
							 al_draw_tinted_bitmap(hielo[0][luz-1],al_map_rgba(255,255,255,64),x,y-60,0);
							 restore_default_blender();
							};


}

void charset::muestra_hud_guerrero(ALLEGRO_BITMAP *pant, warrior *w, float px, float py, int cx, int cy)
{
      int x,y;
      float ix=w->fx_pos()-0.5, iy=w->fy_pos()-0.5;
      byte c1=w->color(1), c2=w->color(2);

      x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
      y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0));
      //if(!suelo) y-=8;

      x += CUBX /2;
      y -= 100;

      if((w->estado()==OUTL) && (w->contador()>=WTTEL)) return;

      int bar_width = 13*6 + 2;
      int bar_half  = bar_width / 2;
      int bar_height = 13 + 2;
      float health = (float)w->salud() / (float)w->salud_maxima();
      if(health > 1.f) health = 1.f;

      al_set_target_bitmap(pant);
      rectfill(pant,x - bar_half - 1, y-2, x + bar_half + 1, y + bar_height + 2, BBLAN);
      rectfill(pant,x - bar_half, y-1, x + bar_half, y + bar_height, 0);
      rectfill(pant,x - bar_half, y-1, x - bar_half + int(bar_width * health), y + bar_height, c2);

      texto(pant, w->nombre(), x - (strlen(w->nombre()) * 13)/2, y, c1, true );

      char s[5];
      sprintf(s,"%c",FONT_BETA_AMMO+1);
      texto(pant,s,x - 6, y + bar_height + 4,BBLAN);
}

void charset::muestra_bala(ALLEGRO_BITMAP *pant, bala *b, byte a,float px, float py, int cx, int cy)
{

      int x,y;
      float ix=b->posx()-0.5, iy=b->posy()-0.5;
      byte l=consulta_luz(ix,iy);
      x=(int)floor(cx-15.0+(int)((ix-px)*30.0)-(int)((iy-py)*30.0));
      y=(int)floor(cy-7.5+(int)((ix-px)*15.0)+(int)((iy-py)*15.0));

      if(b->tipo()!=BULL) al_draw_bitmap(sombra,x,y,0);//draw_trans_sprite2(pant,sombra,x,y,tsombra);
      if((b->tipo()==BULL) || (b->tipo()==ROCK) || (b->tipo()==HIEL))
      /*switch(b->dir()){
         case DABA : draw_sprite_ilum(pant,balas[b->tipo()][0+a],x,y-45,l,ilum_trans,tluz,tsombra); break;
         case DDER : draw_sprite_ilum_h_flip(pant,balas[b->tipo()][0+a],x,y-45,l,ilum_trans,tluz,tsombra); break;
         case DIZQ : draw_sprite_ilum(pant,balas[b->tipo()][3+a],x,y-45,l,ilum_trans,tluz,tsombra); break;
         case DARR : draw_sprite_ilum_h_flip(pant,balas[b->tipo()][3+a],x,y-45,l,ilum_trans,tluz,tsombra); break;
         }
      else switch(b->dir()){
         case DABA : draw_sprite(pant,balas[b->tipo()][0+a],x,y-45); break;
         case DDER : draw_sprite_h_flip(pant,balas[b->tipo()][0+a],x,y-45); break;
         case DIZQ : draw_sprite(pant,balas[b->tipo()][3+a],x,y-45); break;
         case DARR : draw_sprite_h_flip(pant,balas[b->tipo()][3+a],x,y-45); break;
         };*/
	   switch(b->dir()){
         case DABA : al_draw_bitmap(balas[b->tipo()][0+a],x,y-45,0); break;
         case DDER : al_draw_bitmap(balas[b->tipo()][0+a],x,y-45,ALLEGRO_FLIP_HORIZONTAL); break;
         case DIZQ : al_draw_bitmap(balas[b->tipo()][3+a],x,y-45,0); break;
         case DARR : al_draw_bitmap(balas[b->tipo()][3+a],x,y-45,ALLEGRO_FLIP_HORIZONTAL); break;
         }
      else switch(b->dir()){
         case DABA : al_draw_bitmap(balas[b->tipo()][0+a],x,y-45,0); break;
         case DDER : al_draw_bitmap(balas[b->tipo()][0+a],x,y-45,ALLEGRO_FLIP_HORIZONTAL); break;
         case DIZQ : al_draw_bitmap(balas[b->tipo()][3+a],x,y-45,0); break;
         case DARR : al_draw_bitmap(balas[b->tipo()][3+a],x,y-45,ALLEGRO_FLIP_HORIZONTAL); break;
         };

}
void charset::muestra_explos(ALLEGRO_BITMAP *pant, explos *e,float px, float py, int cx, int cy)
{
      int x,y;
      float ix=e->posx()-0.5, iy=e->posy()-0.5;
      x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
      y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0));

      if((e->tipo()!=EPUN) && (e->tipo()<ELLA)){
/*            if((e->tipo()==EBAL) || (e->tipo==ECOH) || (e->tipo()==EHIE))
            draw_sprite_ilum(pant,explosi[int(e->tipo()-1)][e->anim()],x,y-45,
               consulta_luz(e->posx(),e->posy()),ilum_trans,tluz,tsombra);
            else*/
            al_draw_bitmap(explosi[int(e->tipo()-1)][e->anim()],x,y-45,0);
      };
      if(e->tipo()==ELLA){
            /*color_map=tsombra;
            draw_trans_sprite2(pant,sombra,x,y,tsombra);
            color_map=ttrans;*/
		   al_draw_bitmap(sombra,x,y,0);
//            draw_trans_sprite(pant,llamas[e->anim()],x,y-30);
            al_draw_tinted_bitmap(llamas[e->anim()],al_map_rgba(255,255,255,128),x,y-30,0);
      };
      //color_map=ttrans;
      if(e->tipo()==ELL2) al_draw_bitmap(llama2[e->anim()],x,y-60,0);
      if(e->tipo()==ELL3) al_draw_bitmap(llama2[e->anim()],x,y-60,ALLEGRO_FLIP_HORIZONTAL);
}
void charset::muestra_gota(ALLEGRO_BITMAP *pant, float px, float py, int cx, int cy, float ix, float iy, float iz, int cont)
{
	 int x,y,a;

      x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
      y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0)-(iz*30.0));

      //color_map=ttrans;
      if(cont==0) a=0;
      if((cont>=1) && (cont<=3)) a=1;
      if((cont>=4) && (cont<=6)) a=2;
      if((cont>=7) && (cont<=9)) a=3;
      //draw_trans_sprite(pant,gota[a],x,y);
	  al_draw_tinted_bitmap(gota[a],al_map_rgba(255,255,255,128),x,y,0);
}
void charset::muestra_item(ALLEGRO_BITMAP *pant, item *i,float px, float py, int cx, int cy, bool suelo, byte a)
{

      int x,y;
      bald ix=i->x_pos(), iy=i->y_pos();
      x=(int)floor(cx-15.0+((ix-px)*30.0)-((iy-py)*30.0));
      y=(int)floor(cy-7.5+((ix-px)*15.0)+((iy-py)*15.0));
      byte l=consulta_luz(ix,iy);

      if(!suelo) y-=8;

      if(i->es_cogible()){

        //draw_trans_sprite2(pant,sombra,x,y+5,tsombra);
		  al_draw_bitmap(sombra,x,y+5,0);

        switch(i->tipo()){

         case KEY1 : draw_sprite_ilum(pant,llaves[0][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case KEY2 : draw_sprite_ilum(pant,llaves[1][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case KEY3 : draw_sprite_ilum(pant,llaves[2][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case KEY4 : draw_sprite_ilum(pant,llaves[3][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case BOTI : draw_sprite_ilum(pant,botiquin,x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case BALA : draw_sprite_ilum(pant,cbalas,x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case COHE : draw_sprite_ilum(pant,cohetes,x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case NAPA : draw_sprite_ilum(pant,napalm[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case MFUE : draw_sprite_ilum(pant,mana[0][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case MHIE :
			 set_blender_for_transparent();
			 al_draw_tinted_bitmap(mana[1][a],al_map_rgba(255,255,255,128),x+12,y-11,0);
			 restore_default_blender();
			 /*color_map=ttrans; draw_trans_sprite(pant,mana[1][a],x+12,y-11);*/
			 break;
         case MELE : draw_sprite_ilum(pant,mana[2][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case APOC : draw_sprite_ilum(pant,apocalip,x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case WALL : draw_sprite_ilum(pant,invul[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case BERS : draw_sprite_ilum(pant,berserk[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case INVI :
			 set_blender_for_transparent();
			 al_draw_tinted_bitmap(invis[a],al_map_rgba(255,255,255,128),x+12,y-11,0);
			 restore_default_blender();
			 /*color_map=ttrans; draw_trans_sprite(pant,invis[a],x+12,y-11);*/
			 break;
         case REPU : draw_sprite_ilum(pant,repuls[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case HEAL : draw_sprite_ilum(pant,health[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case BOLT : draw_sprite_ilum(pant,bolt,x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case MAPA : draw_sprite_ilum(pant,mapacomp[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case VENE : draw_sprite_ilum(pant,veneno[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case SHAD : draw_sprite_ilum(pant,shadows[a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case DIOS : draw_sprite_ilum(pant,divino[a],x+12,y-11,l,ilum_trans,tluz,tsombra);/*color_map=ttrans; draw_trans_sprite(pant,divino[a],x+12,y-11);*/ break;
         case GEM1 : draw_sprite_ilum(pant,gema[0][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case GEM2 : draw_sprite_ilum(pant,gema[1][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case GEM3 : draw_sprite_ilum(pant,gema[2][a],x+12,y-11,l,ilum_trans,tluz,tsombra); break;
         case AMET :
         case LANZ :
         case ANIQ :
         case DRAG :
         case INFE :
         case SHOC :
         case PIST : al_draw_bitmap(a_sigma[i->tipo()-PIST][1],x,y-5,0); break; //draw_sprite_ilum(pant,a_sigma[i->tipo()-PIST][1],x,y-5,l,ilum_trans,tluz,tsombra); break;
         case EXCA :
         case FALC :
         case MAZA :
         case MART :
         case AGUI :
         case EDGE :
         case DAGA : draw_sprite_ilum(pant,a_omega[i->tipo()-DAGA][1],x,y-30,l,ilum_trans,tluz,tsombra); break;
         case ECOB :
         case EACE :
         case EBRO :
         case EPLA :
         case EORO :
         case EDIV :
         case EMAD : draw_sprite_ilum(pant,escudos[i->tipo()-EMAD][0],x+15,y-20,l,ilum_trans,tluz,tsombra); break;
         default : //draw_sprite(pant,extra[l],x,y-30);
					break;
         }
      };

}
void charset::texto(ALLEGRO_BITMAP *bmp, const char *frase, int x, int y, byte color, bool sombra)
{
	  al_set_target_bitmap(bmp);
      int xx=x, yy=y, i=0;
      while(frase[i]!='\0'){
			   yy=y;
               switch(frase[i]){
                  case 'j':
                  case 'p':
                  case 'q':
                  case 'g':
                  case 'y': yy += 2; break;
                  default : break;
			   };
			   if (sombra)
				   al_draw_tinted_bitmap(fuente[(unsigned char)frase[i]],al_map_rgba(0,0,0,128),xx+2,yy+2,0);

			   al_draw_tinted_bitmap(fuente[(unsigned char)frase[i]],al_map_rgb(dp2palette[color][0],dp2palette[color][1],dp2palette[color][2]),xx,yy,0);
               xx+=13; i++;
               };

}
void charset::muestra_mensajes(ALLEGRO_BITMAP *bmp, batalla *b, byte prio)
{
	 int clipx, clipy, clipw, cliph;
	 al_get_clipping_rectangle(&clipx,&clipy,&clipw,&cliph);
      int x=clipx+5, y=clipy+5, i=0;
      while(b->dev_mens(i)!=NULL){
            if((b->mens_prio(i) & prio)>0){
               texto(bmp,b->dev_mens(i),x,y,b->mens_color(i));
               y+=15;
            };
            i++;
      };

}
void charset::muestra_marcador(ALLEGRO_BITMAP *bmp,warrior *w,int x, int y)
{

      char s[30];
	  int clipx, clipy, clipw, cliph;
	  al_get_clipping_rectangle(&clipx,&clipy,&clipw,&cliph);
	  x += clipx;
	  y += clipy;

      if(w->cualspr()>=NSPR) return;
      sprintf(s,"%d",w->muertes());
      if(w->muertes()>0)
      texto(bmp,s,x,y-16,BAMA1+2);
      al_draw_bitmap(marcador[0],x,y,0);
      texto(bmp,w->nombre(),x+3,y+3,w->color(1));
      sprintf(s,"%c",3);
      texto(bmp,s,x+3,y+21,BBLAN);
	  sprintf(s,"%3d",w->salud());
      texto(bmp,s,x+16,y+21,w->color(1));
      sprintf(s,"%02d",w->level());
      texto(bmp,s,x+57,y+39,w->color(2));
      sprintf(s,"%c",4);
      if(w->posee_llave(0)) texto(bmp,s,x+3,y+39,BMAR3);
      else                  texto(bmp,s,x+3,y+39,223);
      if(w->posee_llave(1)) texto(bmp,s,x+15,y+39,BMAR2);
      else                  texto(bmp,s,x+15,y+39,223);
      if(w->posee_llave(2)) texto(bmp,s,x+27,y+39,BGRIS);
      else                  texto(bmp,s,x+27,y+39,223);
      if(w->posee_llave(3)) texto(bmp,s,x+39,y+39,BAMA2);
      else                  texto(bmp,s,x+39,y+39,223);
      if(w->clase()==OMEGA){
        sprintf(s,"%c",FONT_OMEGA_WEAPON+(w->arma()-1));
        if(w->arma()>0) texto(bmp,s,x+58,y+21,BBLAN);
        sprintf(s,"%c",FONT_OMEGA_SHIELD+(w->escudo()-1));
        if(w->escudo()>0) texto(bmp,s,x+70,y+21,BBLAN);
      }else{
         al_draw_bitmap(marcador[1],x+MARCADOR1X,y,0);
         if(w->clase()==SIGMA)
            for(int i=0; i<3; i++){
               sprintf(s,"%c",FONT_SIGMA_AMMO+i);
               texto(bmp,s,x+89,y+3+(18*i),BBLAN);
			   sprintf(s,"%3d",w->municion(i));
               texto(bmp,s,x+102,y+3+(18*i),w->color(2));
               if(w->arma()>0){
                        sprintf(s,"%c",FONT_SIGMA_WEAPON+2*(w->arma()-1));
                        texto(bmp,s,x+59,y+21,BBLAN);
                        sprintf(s,"%c",FONT_SIGMA_WEAPON+2*(w->arma()-1)+1);
                        texto(bmp,s,x+69,y+21,BBLAN);
               };
            };
         if(w->clase()==BETA){
            for(int i=0; i<3; i++){
                sprintf(s,"%c",FONT_BETA_AMMO+i);
                texto(bmp,s,x+89,y+3+(18*i),BBLAN);
				sprintf(s,"%3d",w->municion(i));
                texto(bmp,s,x+102,y+3+(18*i),w->color(2));
			};
            sprintf(s,"%c",FONT_BETA_WEAPON+w->arma());
            texto(bmp,s,x+59+6,y+21,BBLAN);
         };
      };

}
void charset::muestra_ranking_deathmatch(ALLEGRO_BITMAP *pant, batalla *b)
{
	int x = al_get_bitmap_width(pant) - MARCADOR1X - MARCADOR2X - 5,
		y = al_get_bitmap_height(pant) - b->num_players() * 18 - 5;
	char s[10];

	byte rank[MAXPL] = {0,1,2,3,4,5,6,7}, aux;

	// Ordena
	for(int j=0; j<b->num_players()-1; j++)
	for(int i=0; i<b->num_players()-1; i++)
         if(b->dev_player(rank[i+1])->muertes()>b->dev_player(rank[i])->muertes()){
            aux=rank[i];
            rank[i]=rank[i+1];
            rank[i+1]=aux;
		 };

	// Muestra
	al_set_target_bitmap(pant);
	for(int i = 0; i < b->num_players(); i++)
	{
		warrior *pl = b->dev_player(rank[i]);
		al_draw_bitmap_region(marcador[0],0,0,MARCADOR1X,18,x,y+18*i,0);
		al_draw_bitmap_region(marcador[1],0,0,MARCADOR2X,18,x+MARCADOR1X,y+18*i,0);
		texto(pant,pl->nombre(),x+3,y+18*i+3,pl->activo() ? pl->color(1) : BNEGR);
		sprintf(s,"%03d",pl->muertes());
		texto(pant,s,x+MARCADOR1X+3,y+18*i+3,pl->activo() ? BBLAN : BNEGR);
	}

	long tim = b->tiempo_max_batalla() * 60 - b->tiempo_batalla_ticks();
    sprintf(s,"%02d:%02d",int(tim/3600),int((tim%3600)/60));
    if(tim > 30 * 60 || (tim % 30 < 15)) texto(pant,s,x,y-13,tim > 30 * 60 ? BAMA1+2 : BROJO);
}
void charset::muestra_datos_personaje(ALLEGRO_BITMAP *p,int player, warrior *w,int x, int y)
{

        char s[20], s2[5];
		int i;
        ventana(p,x,y,x+280,y+160,BMAR3,BMAR2,FALSE);
        rectfill(p,x+20,y+15,x+100,y+70,w->color(1));
        //draw_sprite_colored(p,minicara[(int)w->clase()],x+20,y+15,w->color(1),w->color(2),MEDLUZ,NULL,NULL,NULL);
		al_draw_bitmap(minicaras_coloreadas[player],x+20,y+15,0);
        texto(p,w->nombre(),x+20,y+75,BAMA1+2,true);
        sprintf(s,"Level   %2d",w->level());
        texto(p,s,x+135,y+15,BAMA1+2,true);
        sprintf(s,"Exp %6d",w->exp());
        texto(p,s,x+135,y+35,BMAR1,true);
        if(w->level()<WMAXLEV) sprintf(s,"Next%6d",exp_level(w->level()+1));
        else              sprintf(s,"Next  NONE");
        texto(p,s,x+135,y+55,BMAR1,true);
        sprintf(s,"%c%3d/%3d",3,w->salud(),w->salud_maxima());
        texto(p,s,x+20,y+95,BMAR1,true);
        sprintf(s,"%c",4);
        if(w->posee_llave(0)) texto(p,s,x+20,y+115,BMAR3);
        else                  texto(p,s,x+20,y+115,BNEGR);
        if(w->posee_llave(1)) texto(p,s,x+40,y+115,BMAR2);
        else                  texto(p,s,x+40,y+115,BNEGR);
        if(w->posee_llave(2)) texto(p,s,x+60,y+115,BGRIS);
        else                  texto(p,s,x+60,y+115,BNEGR);
        if(w->posee_llave(3)) texto(p,s,x+80,y+115,BAMA2);
        else                  texto(p,s,x+80,y+115,BNEGR);
        if(w->clase()==SIGMA){
        for(i=0; i<3; i++){
                sprintf(s,"%c%3d/%3d",FONT_SIGMA_AMMO+i,w->municion(i),w->municion_maxima(i));
                texto(p,s,x+160,y+75+(20*i),BMAR1,true);};
        for(i=0; i<7; i++){
                sprintf(s,"%c",FONT_SIGMA_WEAPON+(2*i));
                sprintf(s2,"%c",FONT_SIGMA_WEAPON+(2*i)+1);
                if(w->tiene_arma(i+1)){
                        texto(p,s,x+20+34*i,y+135,BGRIS,true);
                        texto(p,s2,x+32+34*i,y+135,BGRIS,true);};
                };
        };
        if(w->clase()==BETA){
        for(i=0; i<3; i++){
                sprintf(s,"%c%3d/%3d",FONT_BETA_AMMO+i,w->municion(i),w->municion_maxima(i));
                texto(p,s,x+160,y+75+(20*i),BMAR1,true);};
        for(i=0; i<10; i++){
                sprintf(s,"%c",FONT_BETA_WEAPON+i+1);
                if(w->tiene_arma(i+1)){
                        texto(p,s,x+20+25*i,y+135,BGRIS,true);};
                };
        };
        if(w->clase()==OMEGA){
                sprintf(s,"%c",FONT_OMEGA_WEAPON+w->arma()-1);
                if(w->arma()>0) texto(p,s,x+20,y+135,BGRIS,true);
                sprintf(s,"%c",FONT_OMEGA_SHIELD+w->escudo()-1);
                if(w->escudo()>0) texto(p,s,x+40,y+135,BGRIS,true);
        };

}
void charset::muestra_consola(ALLEGRO_BITMAP *p, batalla *b)
{
        int i=MAXMS, j;
        //time t; date d;
		char s[15];

        if(b->consola_input_activo()){
                ventana(p,0,0,tamx,tamy,BMAR3,BMAR2,TRUE);
                texto(p,">",30,tamy-20,BAMA1+2,true);
                texto(p,b->dev_prompt_consola(),46,tamy-20,BAMA1+2,true);
                texto(p,"|",46+(13*strlen(b->dev_prompt_consola())),tamy-20,BAMA1+2,true);
                j=tamy-60;
        }else{
                 ventana(p,0,0,tamx,tamy,BMAR3,BMAR2,FALSE);
                j=tamy-40;
        };
        while(b->dev_mens_consola(i)==NULL)
                i--;
        while((i>=0) && (j>=20)){
                texto(p,b->dev_mens_consola(i),30,j,b->mens_consola_color(i),true);
                i--; j-=20;
        };
        /*gettime(&t); getdate(&d);
        sprintf(s,"%2d:%02d:%02d",t.ti_hour,t.ti_min,t.ti_sec);
        texto(p,s,tamx-10-(13*strlen(s)),2,15);*/
}
void charset::ventana(ALLEGRO_BITMAP *p, int x1, int y1, int x2, int y2, byte c1, byte c2, bool con)
{
          ALLEGRO_BITMAP *tile;
          int op=0;
          if(con) op=3;
          for(int j=y1; j<=y2-CONTILEY; j+=CONTILEY)
          for(int i=x1; i<=x2-CONTILEX; i+=CONTILEX){
                tile=consola[4];
                if(i==x1) tile=consola[3];
                if(j==y1) tile=consola[1];
                if(i==x2-CONTILEX) tile=consola[5];
                if(j==y2-CONTILEY) tile=consola[7+op];
                if((i==x1) && (j==y1)) tile=consola[0];
                if((i==x2-CONTILEX) && (j==y1)) tile=consola[2];
                if((i==x1) && (j==y2-CONTILEY)) tile=consola[6+op];
                if((i==x2-CONTILEX) && (j==y2-CONTILEY)) tile=consola[8+op];
                //draw_sprite(p,tile,i,j);
				al_draw_bitmap(tile, i, j, 0);
          };
}
void charset::limpia_matriz_luz(byte l)
{
      for(int i=0; i<MSIZEY; i++)
         for(int j=0; j<MSIZEX; j++)
            matrizluz[i][j]=l;
}
void charset::inserta_aura_luz(bald x, bald y,byte l)
{
      int i,j,a;
      j=0; i=l;
 /*     while(i>MINLUZ){
            for(a=x-j; a<x+j+1; a++){
               inserta_luz(y-j,a,i,TRUE);
               inserta_luz(y+j,a,i,TRUE);};

            for(a=y-j; a<y+j+1; a++){
               inserta_luz(a,x-j,i,TRUE);
               inserta_luz(a,x+j,i,TRUE);};
            j++; i--;
      }*/
	  for(i=x-MAXLUZ; i<=x+MAXLUZ; i++)
		  for(j=y-MAXLUZ; j<=y+MAXLUZ; j++)
		  {
			  int dx = i-x;
			  int dy = j-y;
			  int dist = (int)sqrt(float(dx*dx+dy*dy));
			  inserta_luz(j,i,dist <= l ? l - dist : 0,TRUE);
			  //inserta_luz(j,i,l,TRUE);
		  }

}
void charset::inserta_aura_oscu(bald x, bald y,byte l)
{
      int i,j,a;
      j=0; i=l;
      while(i<MAXLUZ){
            for(a=x-j; a<x+j+1; a++){
               inserta_oscu(y-j,a,i,TRUE);
               inserta_oscu(y+j,a,i,TRUE);};

            for(a=y-j; a<y+j+1; a++){
               inserta_oscu(a,x-j,i,TRUE);
               inserta_oscu(a,x+j,i,TRUE);};
            j++; i++;
      }
}
void charset::inserta_luz(bald x, bald y, byte l, bool control)
{
      if((x<MSIZEX) && (y<MSIZEY))
      if((matrizluz[y][x]<l) || (!control)) matrizluz[y][x]=l;
}
void charset::inserta_oscu(bald x, bald y, byte l, bool control)
{
      if((x<MSIZEX) && (y<MSIZEY))
      if((matrizluz[y][x]>l) || (!control)) matrizluz[y][x]=l;
}
byte charset::consulta_luz(bald x, bald y)
{
      if((x>MSIZEX) || (y>MSIZEY)) return MEDLUZ;
      else                         return matrizluz[x][y];
}
void charset::aplica_luz_batalla(batalla *bt)
{
      int i=0;
      bala *bal;
      area *ar;
      explos *ex;
      warrior *w;
      byte lla_an[4]={0,1,2,1};

      // Areas oscuras
      i=0;
      while((ar=bt->dev_mapa()->dev_oscu(i))!=NULL){

         for(int k=ar->iny(); k<=ar->finy(); k++)
            for(int j=ar->inx(); j<=ar->finx(); j++)
               if(bt->dev_mapa()->oscu_on[i])
                if((k==ar->iny()) || (k==ar->finy()) || (j==ar->inx()) || (j==ar->finx()))
                        inserta_luz(k,j,MINLUZ+2,FALSE);
                else if((k==ar->iny()+1) || (k==ar->finy()-1) || (j==ar->inx()+1) || (j==ar->finx()-1))
                        inserta_luz(k,j,MINLUZ+1,FALSE);
                else
                        inserta_luz(k,j,MINLUZ,FALSE);
         i++;
      };

      // Balas, bolas de fuego, etc
      i=0;
      while((bal=bt->dev_bala(i))!=NULL){
         if(bal->tipo()==FUEG) inserta_aura_luz((bald)bal->posx(),(bald)bal->posy(),MAXLUZ);
         i++;
      };

      // Explosiones y llamas
      i=0;
      while((ex=bt->dev_explos(i))!=NULL){
         if(ex->tipo()==EFUE) inserta_aura_luz((bald)ex->posx(),(bald)ex->posy(),MAXLUZ);
         if(ex->tipo()==ELLA) inserta_aura_luz((bald)ex->posx(),(bald)ex->posy(),MAXLUZ-lla_an[ex->anim()]);
         if((ex->tipo()==ELL2) || (ex->tipo()==ELL3) || (ex->tipo()==EELE) || (ex->tipo()==ECOH) || (ex->tipo()==EHIE))
                inserta_aura_luz((bald)ex->posx(),(bald)ex->posy(),MAXLUZ-ex->anim());
         i++;
      };
      // Antorchas
      for(i=0; i<nantor; i++)
            inserta_aura_luz(antorchas[i][0],antorchas[i][1],MEDLUZ-bt->dev_mapa()->anim());
      // Shadow Special
      i=0;
      while((w=bt->dev_player(i))!=NULL){
        if(w->special(SHADOW)) inserta_aura_oscu(w->x_pos(),w->y_pos(),MINLUZ);
        i++;
      };
      // Estrella del apocalipsis
      i=0;
      while((w=bt->dev_player(i))!=NULL){
        if((w->estado()==HECH) && (w->arma()==10)) aura_estrella(w->x_pos(),w->y_pos());
        i++;
      };
      i=0;
      while((w=bt->dev_enem(i))!=NULL){
        if((w->estado()==HECH) && (w->arma()==10)) aura_estrella(w->x_pos(),w->y_pos());
        i++;
      };


}
void charset::aura_estrella(bald x, bald y)
{
        byte mat[14][14]={{0,0,0,0,0,1,1,1,1,0,0,0,0,0},
                          {0,0,0,1,1,0,1,1,0,1,1,0,0,0},
                          {0,0,1,0,0,0,1,1,0,0,0,1,0,0},
                          {0,1,0,0,0,1,0,0,1,0,0,0,1,0},
                          {0,1,1,1,1,1,1,1,1,1,1,1,1,0},
                          {1,0,1,1,1,0,0,0,0,1,1,1,0,1},
                          {1,0,0,0,1,0,0,0,0,1,0,0,0,1},
                          {1,0,0,0,1,1,0,0,1,1,0,0,0,1},
                          {1,0,0,1,0,0,1,1,0,0,1,0,0,1},
                          {0,1,0,1,0,1,0,0,1,0,1,0,1,0},
                          {0,1,1,1,1,0,0,0,0,1,1,1,1,0},
                          {0,0,1,0,0,0,0,0,0,0,0,1,0,0},
                          {0,0,0,1,1,0,0,0,0,1,1,0,0,0},
                          {0,0,0,0,0,1,1,1,1,0,0,0,0,0}}, l;
        int i,j;
        l=1+rand()%7;
        for(i=0; i<14; i++)
        for(j=0; j<14; j++)
                 if(mat[i][j]==1)
                 inserta_luz(y-7+i,x-7+j,l,FALSE);
}
/*
void draw_sprite_colored(BITMAP *bmp, BITMAP *sprite, int x, int y, char NCO1, char NCO2, byte l, byte ilum_trans[], COLOR_MAP *tluz, COLOR_MAP *tsombra)
{
        int i,j;
        byte *ori, *des;
        byte c;

        for(i=0; i<sprite->h; i++)
        if((y+i>=0) && (y+i<bmp->h)){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x];
                 for(j=0; j<sprite->w; j++){
                          if((x+j>=0) && (x+j<bmp->w) && (*ori!=0)){
                                c=*ori;
                                switch(c){
                                    case BCO1  : c=NCO1; break;
                                    case BCO1+1: c=NCO1+1; break;
                                    case BCO1+2: c=NCO1+2; break;
                                    case BCO1+3: c=NCO1+3; break;
                                    case BCO1+4: c=NCO1+4; break;
                                    case BCO1+5: c=NCO1+5; break;
                                    case BCO1+6: c=NCO1+6; break;
                                    case BCO1+7: c=NCO1+7; break;
                                    case BCO2  : c=NCO2; break;
                                    case BCO2+1: c=NCO2+1; break;
                                    case BCO2+2: c=NCO2+2; break;
                                    case BCO2+3: c=NCO2+3; break;
                                    case BCO2+4: c=NCO2+4; break;
                                    case BCO2+5: c=NCO2+5; break;
                                    case BCO2+6: c=NCO2+6; break;
                                    case BCO2+7: c=NCO2+7; break;
                                    default : break;
                                };
                                if(l==MEDLUZ) *des=c;
                                else{
                                     if(l<MEDLUZ){color_map=tsombra;
                                                  *des=color_map->data[ilum_trans[MEDLUZ-(1+l)]][c];}
                                     else{
                                                  color_map=tluz;
                                                  *des=color_map->data[ilum_trans[l-(MEDLUZ+1)]][c];};
                                };};
                          ori++; des++;
                          };
                 };
}*/
void draw_sprite_ilum(ALLEGRO_BITMAP *bmp, ALLEGRO_BITMAP *sprite, int x, int y, byte l, byte ilum_trans[], char *tluz, char *tsombra, bool flip)
{
	if( l < MEDLUZ )
	{
		float step = 0.9f / (MEDLUZ-1);
		float light = 0.1f + step * l;
		al_draw_tinted_bitmap(sprite,al_map_rgba_f(light,light,light,1.f),x,y, flip ? ALLEGRO_FLIP_HORIZONTAL : 0);
	}
	else
	{
		al_draw_bitmap(sprite,x,y,flip ? ALLEGRO_FLIP_HORIZONTAL : 0);

		if( l > MEDLUZ )
		{
			// FUNCIONA; PERO ES MUUUUUY LENTO
			al_set_target_bitmap(silueta);
			al_clear_to_color(al_map_rgba(0,0,0,0));

			// Crea una silueta del sprite con color blanco
			al_draw_bitmap(sprite,0,0,0);
			al_draw_filled_rectangle(0,0,al_get_bitmap_width(sprite),al_get_bitmap_height(sprite),al_map_rgba_f(1.0,1.0,1.0,0.95f));
			al_convert_mask_to_alpha(silueta,al_get_pixel(silueta,0,0));

			al_set_target_bitmap(bmp);
			//float step = 0.9 / (MEDLUZ-1);
			//float light = 0.0f + step * (l - MEDLUZ - 1);
			float step = 0.6 / (MEDLUZ-1);
			float light = 0.0f + step * (l - MEDLUZ - 1);
			al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ONE);
			al_draw_tinted_bitmap_region(silueta,al_map_rgba_f(1.f,1.f,1.f,light),0,0,al_get_bitmap_width(sprite),al_get_bitmap_height(sprite),x,y,flip ? ALLEGRO_FLIP_HORIZONTAL : 0);
			restore_default_blender();

		}
	}
        /*int i,j;
        byte *ori, *des;
        byte c;

        if(l==MEDLUZ) {draw_sprite(bmp,sprite,x,y); return;};

        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x];
                 for(j=0; j<sprite->w; j++){
                          if((x+j>=0) && (x+j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=0)){
                                c=*ori;
                                if(l<MEDLUZ){color_map=tsombra;
                                                  *des=color_map->data[ilum_trans[MEDLUZ-(1+l)]][c];}
                                else{
                                                  color_map=tluz;
                                                  *des=color_map->data[ilum_trans[l-(MEDLUZ+1)]][c];};
                                };
                          ori++; des++;
                          };
                 };*/
}
void draw_sprite_ilum_h_flip(ALLEGRO_BITMAP *bmp, ALLEGRO_BITMAP *sprite, int x, int y, byte l, byte ilum_trans[], char *tluz, char *tsombra)
{
	draw_sprite_ilum(bmp,sprite,x,y,l,ilum_trans,tluz,tsombra,true);
	/*if( l < MEDLUZ )
	{
			float step = 0.9f / (MEDLUZ-1);
			float light = 0.1f + step * l;
			al_draw_tinted_bitmap(sprite,al_map_rgba_f(light,light,light,1.f),x,y,ALLEGRO_FLIP_HORIZONTAL);
		}
		else
			al_draw_bitmap(sprite,x,y,ALLEGRO_FLIP_HORIZONTAL);*/
        /*int i,j;
        byte *ori, *des;
        byte c;

        if(l==MEDLUZ) {draw_sprite_h_flip(bmp,sprite,x,y); return;};

        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x+(sprite->w)]-1;
                 for(j=0; j<sprite->w; j++){
                          if((x+(sprite->w)-j>0) && (x+(sprite->w)-j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=0)){
                                c=*ori;
                                if(l<MEDLUZ){color_map=tsombra;
                                                  *des=color_map->data[ilum_trans[MEDLUZ-(1+l)]][c];}
                                else{
                                                  color_map=tluz;
                                                  *des=color_map->data[ilum_trans[l-(MEDLUZ+1)]][c];};
                                };
                          ori++; des--;
                          };
                 };*/
}

/*
void draw_sprite_colored_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, char NCO1, char NCO2, byte l, byte ilum_trans[], COLOR_MAP *tluz, COLOR_MAP *tsombra)
{
        int i,j;
        byte *ori, *des;
        byte c;

        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x+(sprite->w)];
                 for(j=0; j<sprite->w; j++){
                          if((x+(sprite->w)-j>0) && (x+(sprite->w)-j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=0)){
                                c=*ori;
                                switch(c){
                                    case BCO1  : c=NCO1; break;
                                    case BCO1+1: c=NCO1+1; break;
                                    case BCO1+2: c=NCO1+2; break;
                                    case BCO1+3: c=NCO1+3; break;
                                    case BCO1+4: c=NCO1+4; break;
                                    case BCO1+5: c=NCO1+5; break;
                                    case BCO1+6: c=NCO1+6; break;
                                    case BCO1+7: c=NCO1+7; break;
                                    case BCO2  : c=NCO2; break;
                                    case BCO2+1: c=NCO2+1; break;
                                    case BCO2+2: c=NCO2+2; break;
                                    case BCO2+3: c=NCO2+3; break;
                                    case BCO2+4: c=NCO2+4; break;
                                    case BCO2+5: c=NCO2+5; break;
                                    case BCO2+6: c=NCO2+6; break;
                                    case BCO2+7: c=NCO2+7; break;
                                    default : break;
                                };
                                if(l==MEDLUZ) *des=c;
                                else{
                                     if(l<MEDLUZ){color_map=tsombra;
                                                  *des=color_map->data[ilum_trans[MEDLUZ-(1+l)]][c];}
                                     else{
                                                  color_map=tluz;
                                                  *des=color_map->data[ilum_trans[l-(MEDLUZ+1)]][c];};
                                };};
                          ori++; des--;
                          };
                 };
}
void draw_sprite_colored_rand(BITMAP *bmp, BITMAP *sprite, int x, int y, char NCO1, char NCO2, int r)
{
        int i,j;
        byte *ori, *des;
        byte c;

        if(r==0) r=1;
        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x];
                 for(j=0; j<sprite->w; j++){
                          if((x+j>=0) && (x+j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=0) && (rand()%r==0)){
                                c=*ori;
                                switch(c){
                                    case BCO1  : c=NCO1; break;
                                    case BCO1+1: c=NCO1+1; break;
                                    case BCO1+2: c=NCO1+2; break;
                                    case BCO1+3: c=NCO1+3; break;
                                    case BCO1+4: c=NCO1+4; break;
                                    case BCO1+5: c=NCO1+5; break;
                                    case BCO1+6: c=NCO1+6; break;
                                    case BCO1+7: c=NCO1+7; break;
                                    case BCO2  : c=NCO2; break;
                                    case BCO2+1: c=NCO2+1; break;
                                    case BCO2+2: c=NCO2+2; break;
                                    case BCO2+3: c=NCO2+3; break;
                                    case BCO2+4: c=NCO2+4; break;
                                    case BCO2+5: c=NCO2+5; break;
                                    case BCO2+6: c=NCO2+6; break;
                                    case BCO2+7: c=NCO2+7; break;
                                    default : break;
                                };
                                *des=c;};
                          ori++; des++;
                          };
                 };
}
void draw_sprite_colored_rand_h_flip(BITMAP *bmp, BITMAP *sprite, int x, int y, char NCO1, char NCO2, int r)
{
        int i,j;
        byte *ori, *des;
        byte c;

        if(r==0) r=1;
        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x+(sprite->w)];
                 for(j=0; j<sprite->w; j++){
                          if((x+(sprite->w)-j>0) && (x+(sprite->w)-j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=0) && (rand()%r==0)){
                                c=*ori;
                                switch(c){
                                    case BCO1  : c=NCO1; break;
                                    case BCO1+1: c=NCO1+1; break;
                                    case BCO1+2: c=NCO1+2; break;
                                    case BCO1+3: c=NCO1+3; break;
                                    case BCO1+4: c=NCO1+4; break;
                                    case BCO1+5: c=NCO1+5; break;
                                    case BCO1+6: c=NCO1+6; break;
                                    case BCO1+7: c=NCO1+7; break;
                                    case BCO2  : c=NCO2; break;
                                    case BCO2+1: c=NCO2+1; break;
                                    case BCO2+2: c=NCO2+2; break;
                                    case BCO2+3: c=NCO2+3; break;
                                    case BCO2+4: c=NCO2+4; break;
                                    case BCO2+5: c=NCO2+5; break;
                                    case BCO2+6: c=NCO2+6; break;
                                    case BCO2+7: c=NCO2+7; break;
                                    default : break;
                                };
                                *des=c;};
                          ori++; des--;
                          };
                 };
}
void draw_trans_sprite2(BITMAP *bmp, BITMAP *sprite, int x, int y, COLOR_MAP *cm)
{
        int i,j;
        byte *ori, *des;
        byte c,d;

        for(i=0; i<sprite->h; i++){
                 ori=&sprite->line[i][0];
                 des=&bmp->line[y+i][x+(sprite->w)];
                 for(j=0; j<sprite->w; j++){
                          if((x+(sprite->w)-j>0) && (x+(sprite->w)-j<bmp->w) && (y+i>=0) && (y+i<bmp->h) && (*ori!=255)){
                                c=*ori; d=*des;
                                *des=cm->data[c][d];};
                          ori++; des--;
                          };
                 };
}
*/
void charset::muestra_mapa(ALLEGRO_BITMAP *bmp,batalla *b)
{
        float cx=b->centroplx(), cy=b->centroply();
        int zoom=b->zoom_mapa(), inx=int(tamx/2-(cx*zoom)), iny=int(tamy/2-(cy*zoom));
        int minx,miny,maxx,maxy;

        b->dev_mapa()->caja_contenedora(minx,miny,maxx,maxy);
        for(int i=miny; i<=maxy; i++)
        for(int j=minx; j<=maxx; j++)
                muestra_casilla_mapa(inx+zoom*j,iny+zoom*i,bmp,b,j,i,zoom);
}

void rect(ALLEGRO_BITMAP *bmp,int x1, int y1, int x2, int y2, unsigned char color)
{
	al_draw_rectangle(x1,y1,x2+1,y2+1,al_map_rgb(dp2palette[color][0],dp2palette[color][1],dp2palette[color][2]),1.0f);
}

void rectfill(ALLEGRO_BITMAP *bmp,int x1, int y1, int x2, int y2, unsigned char color)
{
	al_draw_filled_rectangle(x1,y1,x2+1,y2+1,al_map_rgb(dp2palette[color][0],dp2palette[color][1],dp2palette[color][2]));
}

void circlefill(ALLEGRO_BITMAP *bmp,int x1, int y1, float r, unsigned char color)
{
	al_draw_filled_circle(x1,y1,r,al_map_rgb(dp2palette[color][0],dp2palette[color][1],dp2palette[color][2]));
}

void charset::muestra_casilla_mapa(int x, int y,ALLEGRO_BITMAP *bmp, batalla *b,int mx, int my, int zoom)
{

        mapa *mp=b->dev_mapa();
        nivel *ni=b->dev_nivel();
        int i; byte c;

        if(!b->mapa_conocido(mx,my)) return;
        if(b->dev_mapa()->no_elementos(mx,my)) return;

        if(mp->consb(mx,my)!=NADA){
                if(!mp->suelo(mx,my)) rectfill(bmp,x,y,x+(zoom-1),y+(zoom-1),BMAR2+4);
                else                 rectfill(bmp,x,y,x+(zoom-1),y+(zoom-1),BMAR1);

        }
        if((mp->conse(mx,my)!=VACIO) && (mp->conse(mx,my)!=EXIT)){
                switch(mp->conse(mx,my)){
                        case TELE : c=BVER2+4; break;
                        case INTE : c=BMAR3+8; break;
                        case ITEM : c=BMAR2+4; break;
                        case DTRH :
                        case DTRV : c=BMAR2+4; break;
                        default : c=BMAR3+4; break;
                };
                rectfill(bmp,x,y,x+(zoom-1),y+(zoom-1),c);
        };
        if(((i=mp->hay_puerta(mx,my))>=0) && ((mp->conse(mx,my)==DTRH) || (mp->conse(mx,my)==DTRV))){

                switch(mp->dev_puerta(i)->tipo()){
                        case LLA1 : c=BMAR3; break;
                        case LLA2 : c=BMAR2; break;
                        case LLA3 : c=BGRIS; break;
                        case LLA4 : c=BAMA2; break;
                        default   : c=BMAR3; break;};

                if(!mp->dev_puerta(i)->permite())
                if(mp->dev_puerta(i)->horiz())
                        rectfill(bmp,x,y+(zoom/2),x+(zoom-1),y+(zoom/2)+1,c);
                else
                        rectfill(bmp,x+(zoom/2),y,x+(zoom/2)+1,y+(zoom-1),c);
        };

        if((i=mp->hay_laser(mx,my))>=0)
                if(mp->dev_laser(i)->horiz())
                        rectfill(bmp,x,y+(zoom/2),x+(zoom-1),y+(zoom/2)+1,10+mp->anim());
                else
                        rectfill(bmp,x+(zoom/2),y,x+(zoom/2)+1,y+(zoom-1),10+mp->anim());

        if(mp->hay_barr(mx,my)>=0) rectfill(bmp,x,y,x+(zoom-1),y+(zoom-1),BMAR2+6);

        if(ni->hay_item(mx,my))
               circlefill(bmp,x+(zoom/2),y+(zoom/2),zoom/3,mp->anim()+1);

        if((i=b->hay_player(mx,my,0))>=0)
               circlefill(bmp,x+(zoom/2),y+(zoom/2),zoom/3,b->dev_player(i)->color(1));

        if(b->enemigos_en_mapa()==TRUE)
        if((i=b->hay_enem(mx,my,0))>=0)
               circlefill(bmp,x+(zoom/2),y+(zoom/2),zoom/3,b->dev_enem(i)->color(1));

        if(((i=b->hay_enem(mx,my,0))>=0) && (b->dev_enem(i)->bando()==0))
               circlefill(bmp,x+(zoom/2),y+(zoom/2),zoom/3,b->dev_nivel()->dev_enem(i)->color(1));

}
//void charset::muestra_puntero(BITMAP *bmp, int x, int y)
//{
//
//    //  draw_sprite(bmp,fuente['^'],x,y);
//}
