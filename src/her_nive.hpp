// Requiere los TAD's siguientes

#include"her_mapa.hpp"
#include"her_item.hpp"
#include"her_warr.hpp"

#define L_NITEM 300
#define L_NENEM 100

class nivel {

      private:
      mapa *mp;
      item *it[L_NITEM];
      warrior *enem[L_NENEM];

      byte nitem, nenem, csombra, cluz, maxsecr;
      char nnombre[MENSZ], nfdecor[FNAME], nfspr[NSPR][FNAME], nfmusic[FNAME];
      float ncoefr, ncoefg, ncoefb;

      public:
      nivel();
      ~nivel();
      void cambia_nombre(const char*);
      void cambia_fdecor(const char*);
      void cambia_fspr(const char*,int);
      void cambia_fmusic(const char*);
      void cambia_color_sombra(byte);
      void cambia_color_luz(byte);
      void cambia_coef_paleta(float,float,float);
      void ingresa_enem(warrior*);
      void cambia_enem(warrior*, int);
      void ingresa_item(bald,bald,t_item);
      void retira_enem(int);
      void retira_item(int);
      int hay_enem(bald,bald,int);
      bool hay_item(bald,bald);
      int primer_item(bald,bald);
      int num_enemigos();
      int num_enemigos_muertos();
      int num_secrets();
      int num_total_secrets();
      int num_aliados();
      int num_aliados_muertos();
      mapa *dev_mapa();
      warrior *dev_enem(int);
      item *dev_item(int);
      char *nombre();
      char *fdecor();
      char *fspr(int);
      char *fmusic();
      byte color_sombra();
      byte color_luz();
      float coefr();
      float coefg();
      float coefb();
      void info(int&,int&);
      void salva(char*);
      void carga(char*);
      void importar(char*,int,char*,char*,char*,byte,byte,byte,byte);
      void corrige_esquinas();
      void elimina_items_inutiles(bool,bool,bool,byte);
      };

bool pared_h(telem);
bool pared_v(telem);

struct O_ENEMY {
	char e_xy[4];
	char e_t;
	unsigned char e_l;
	char e_d;
	char e_w;
	};

struct O_TELEPORT {
	char origen_xy[4];
	char destino_xy[4];
	};

struct O_SWITCH {
	char activador_xy[4];
	char activable_xy[4];
	char estado;
	};

struct O_LEVEL {
	char t_n;
	char e_n;
	char s_n;
	char start_xy[4];
	char map_name[25];
	char map[10][10][8][8];
	struct O_ENEMY enemies[100];
	struct O_TELEPORT teleport[20];
	struct O_SWITCH switches[20];
	};

struct O_EPISODE {
	char epi_name[25];
	char charset_file[12];
	char enemy_file[12];
	struct O_LEVEL nivel[8];
	};


