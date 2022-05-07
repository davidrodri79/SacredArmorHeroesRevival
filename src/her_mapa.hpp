
// Requiere los TAD's siguientes
#include "her_tele.hpp"
#include "her_inte.hpp"
#include "her_puen.hpp"
#include "her_puer.hpp"
#include "her_area.hpp"
#include "her_barr.hpp"
#include "her_disp.hpp"

#define MSIZEX 250
#define MSIZEY 250

#define M_NTELE 30
#define M_NINTE 40
#define M_NPUER 30
#define M_NPUEN 20
#define M_NOSCU 15
#define M_NSECR 20
#define M_NBARR 60
#define M_NDISP 15
#define M_NLASE 15
#define M_NAMBI 30

#define BIT_EMPTY   0x00
#define BIT_TEL     0x01
#define BIT_INT_DIS 0x02
#define BIT_PUER    0x04
#define BIT_BAR     0x08
#define BIT_OSC     0x10
#define BIT_PUE_LAS 0x20

#define BIT_ENE     0x80


typedef unsigned char tbald;
enum {NADA=0,
            // Baldosas y suelos
            BDA1,BDA2,BMO1,BMO2,BSU1,BSU2,BSU3,BSU4,BSU5
            };

typedef unsigned char telem;
enum {VACIO=0,
            // Paredes
            PHO1,PHO2,PHO3,PHO4,PHO5,PHO6,PHO7,PHO8,PHO9,PVE1,PVE2,PVE3,PVE4,
            PVE5,PVE6,PVE7,PVE8,PVE9,PHAT,PVAT,
            // Esquinas
            ECRU,EL_D,ER_D,EL_U,ER_U,EULD,ERUL,EURD,ELDR,
            // puertas (Doors)
            DM1H,DM1V,DM2H,DM2V,DARH,DARV,DTRH,DTRV,
            // Decorados,columnas y otros
            INTE,TELE,DEC1,DEC2,DEC3,DEC4,DEC5,DEC6,DEC7,DEC8,DEC9,EXIT,COL1,
            COL2,COL3,ITEM,LPAR,LPAB,LPIZ,LPDE,TIAR,TIAB,TIIZ,TIDE,
            };

class mapa {

         private:
         tbald *matrizb[MSIZEY];
         telem *matrize[MSIZEY];
         byte *interes[MSIZEY];
         telep *mtele[M_NTELE];
         puerta *mpuerta[M_NPUER];
         interr *minterr[M_NINTE];
         puente *mpuente[M_NPUEN], *mlaser[M_NLASE];
         area *moscu[M_NOSCU], *msecr[M_NSECR], *fin;
         barrera *mbarr[M_NBARR];
         disparador *mdisp[M_NDISP];
         bald ambientx[M_NAMBI], ambienty[M_NAMBI];
         byte ambients[M_NAMBI];
         float manim;
         byte ntele, npuerta, ninterr, npuente, noscu, nsecr, nbarr, ndisp,
              nlaser, nambi;
         bald inix, iniy;
         w_dir inidir;
         int minx, maxx, miny, maxy;

         public:
         bool oscu_on[M_NOSCU];
         mapa();
         ~mapa();
         void insb(bald,bald,tbald);
         void inse(bald,bald,telem);
         void nueva_pos_inicio(bald,bald,w_dir);
         void area_final(bald,bald,bald,bald);
         void anyade_telep(bald,bald,bald,bald);
         void anyade_puerta_normal(bald,bald,bald,bald);
         void anyade_puerta_llave(bald,bald,bald,bald,tpuerta);
         void anyade_puerta_interr(bald,bald,bald,bald,bald,bald,bool);
         void anyade_puente_interr(bald,bald,bald,bald,bald,bald,bool,word);
         void anyade_area_oscura(bald,bald,bald,bald);
         void anyade_area_oscura_interr(bald,bald,bald,bald,bald,bald);
         void anyade_area_secreta(bald,bald,bald,bald);
         void anyade_barrera(bald,bald,byte);
         void anyade_disp_auto(bald,bald,w_dir,byte,byte,word);
         void anyade_disp_interr(bald,bald,bald,bald,w_dir,byte,byte);
         void anyade_laser(bald,bald,bald,bald,bald,bald);
         void anyade_sonido_amb(bald,bald,byte);
         void retira_telep(int);
         void retira_puerta(int);
         void retira_puente(int);
         void retira_interr(int);
         void retira_oscu(int);
         void retira_secr(int);
         void retira_barrera(int);
         void retira_disp(int);
         void retira_laser(int);
         void retira_sonido_amb(int);
         void actualiza();
         void activa_interr(int);
         void abre_puerta(int);
         tbald consb(bald x, bald y);
         telem conse(bald x, bald y);
         byte anim();
         bald x_inicio();
         bald y_inicio();
         w_dir dir_inicio();
         bool permite(bald,bald);
         bool suelo(bald,bald);
         bool es_final(bald,bald);
         bool no_elementos(bald,bald);
         int hay_telep(bald,bald);
         int hay_puerta(bald,bald);
         int hay_puente(bald,bald);
         int hay_puente_on_o_off(bald,bald);
         int hay_interr(bald,bald);
         int hay_oscu(bald,bald);
         int hay_secr(bald,bald);
         int hay_barr(bald,bald);
         int hay_disp(bald,bald);
         int hay_laser(bald,bald);
         int hay_laser_on_o_off(bald,bald);
         int hay_sonido_amb(bald,bald);
         void info(int&,int&,int&,int&,int&,int&,int&,int&,int&,int&);
         void caja_contenedora(int&,int&,int&,int&);
         telep *dev_telep(int);
         puerta *dev_puerta(int);
         puente *dev_puente(int);
         interr *dev_interr(int);
         area *dev_oscu(int);
         area *dev_secr(int);
         area *dev_final();
         barrera *dev_barr(int);
         disparador *dev_disp(int);
         puente *dev_laser(int);
         int dev_sonido_amb(int,bald&,bald&);
         void crea_matriz_interes();
         bool es_de_interes(bald, bald, byte);
         void activa_interes(bald, bald, byte);
         void desactiva_interes(bald, bald, byte);
         void vacia_matriz_interes(byte);
         void salva(FILE*);
         void carga(FILE*);

         };

