
// Requiere los TADïs siguientes
#include "her_nive.hpp"
#include "her_cont.hpp"
#include "her_bala.hpp"
#include "her_expl.hpp"
#include <allegro5/allegro.h>
//#include <jgmod.h>
#include "her_soun.hpp"
#include "CMessageQueue.hpp"

#define MAXPL 8
#define MAXBA 50
#define MAXEX 150
#define MAXGO 30
#define DCOL 0.75
#define MAXMS 80
#define MSGSIZE int(1920/15)
#define MSGTIME 300
#define MPTWAIT 10
#define LLSTARTZ 10.0
#define TRCICLO 60*6

#define PRIO1 0x01
#define PRIO2 0x02
#define PRIO3 0x04
#define PRIO4 0x08
#define PRIO5 0x10
#define PRIO6 0x20
#define PRIO7 0x40
#define PRIO8 0x80
#define ALLPRIO 0xff

#define COOP 0
#define DMCH 1

#define MROFFLINE	0
#define MRSERVER	1
#define MRCLIENTE	2

#define OPPLAYER	1
#define OPENEMY		2
#define OPBARR		3
#define OPTRAP		4


typedef struct {

      float x,y,z;
      int cont;

      } gota;

class batalla {

      private:
      warrior *player[MAXPL];
      bala *bbala[MAXBA];
      explos *bexplos[MAXEX];
      nivel *niv;
      byte nplayers, nbalas, nexplos, nmens, ncmens, bmenscolor[MAXMS], cmenscolor[MAXMS], bmensprio[MAXMS], mzoom,
           tipoj,prncar, jug_local, modo_red, chat_msg_length, chat_espera,
           *matr_enem[MSIZEY];
      t_control contr[MAXPL];
      char bmensajes[MAXMS][MSGSIZE],cmensajes[MAXMS][MSGSIZE],cprompt[MSGSIZE],lastprompt[MSGSIZE],direc[20],chat_msg[MSGSIZE];
      word bmenstime[MAXMS], bcont, map_op_cont, con_op_cont, dat_op_cont;
      waves *bwav;
      bool modo_mapa, modo_consola, modo_datos, matr_mapa[MSIZEX/5][MSIZEY/5],
           con_input, enemy_map, gema, mens_fin_recibido, ventana_chat;
      long tcont;
      int maxmsgsize, interr_red_espera, puerta_red_espera, tiempo_max;
      float last_received_x[MAXPL][2], last_received_y[MAXPL][2], last_received_enem_x[L_NENEM][2], last_received_enem_y[L_NENEM][2];
      FILE *logf;

      void actualiza_ia_control(warrior*,controlm*,int,int, CMessageQueue* aRecQueue, CMessageQueue* aSendQueue);
      w_dir direccion_movim(float,float,float,float,warrior*,bool);
      w_dir dir_alinearse(float,float,float,float,warrior*,bool);
      w_dir mejor_direccion(w_dir,w_dir,w_dir,w_dir,bool,bool,bool,bool,bool);
      void actualiza_player(byte,warrior*,controlm*,bool);
	  void actualiza_player_red_local(byte,warrior*,controlm*,bool, CMessageQueue*, CMessageQueue*);
      void actualiza_caminar_player_red_remoto(byte c, warrior* w, bool pl);
	  void actualiza_player_red_remoto(byte,warrior*,controlm*,bool);
	  void envia_danyo_red(byte c, cdanyo tipo, word cantidad, byte tipo_oponente, byte id_oponente, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue);
	  void incrementa_muertes_red(byte, CMessageQueue*, CMessageQueue*);
      void actualiza_bala(byte, CMessageQueue *aRecQueue = 0, CMessageQueue *aSendQueue = 0);
      void desencola_mensaje(int);


      public:
      gota bgota[MAXGO];
      bool lluvia;
      int trueno;
      batalla(const char*,const char*,waves*,byte,int,bool);
      batalla(const char*,const char*,byte,char*,char*,char*,byte,byte,byte,byte,waves*,int,bool);
      ~batalla();
      void ingresa_player(char *,byte,w_clase,byte,byte,t_control,byte,word);
      void ingresa_bala(float,float,w_dir,float,float,b_tipo,byte,warrior*);
	  void crea_bala_red(float, float, w_dir, float, float, b_tipo, byte, byte, byte, s_sample, CMessageQueue*, CMessageQueue*);
      void ingresa_explos(float,float,e_tipo,byte,warrior*);
	  void crea_explos_red(float,float,e_tipo,byte,byte,byte, CMessageQueue*, CMessageQueue*);
      void ajusta_enemigos(int,byte);
      void elimina_items_inutiles();
      void ingresa_item_util(bald,bald,t_item);
      void ingresa_item_util_red(bald, bald, t_item, CMessageQueue* recQueue, CMessageQueue* sendQueue);
      void pared_de_fuego(float,float,byte,byte,bool,warrior*);
	  void pared_de_fuego_red(float x, float y, byte l, byte f, bool horiz, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue);
      void bolt(float,float,float,byte,word,warrior*);
	  void bolt_red(float x, float y, float dist, byte bando, word danyo, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue);
      void apocalipsis(float,float,float,word,warrior*);
	  void apocalipsis_red(float x, float y, float dist, word danyo, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue);
      void encola_mensaje(const char *, byte, byte);
      void encola_mensaje_consola(const char *,byte);
	  void encola_mensaje_red(const char *, byte, byte, CMessageQueue*, CMessageQueue*);
	  bool identifica_guerrero(warrior *w, byte &tipo, byte &id);
      void actualiza(int);
	  void actualiza_modo_red(CMessageQueue*,CMessageQueue*);
      void actualiza_consola();
      void comando_consola(char*);
      void desvela_mapa();
      void reset_warrior(warrior*);
	  void reset_warrior_red(byte, byte, CMessageQueue*, CMessageQueue*);
      void abre_cierra_consola();
      void activa_input_consola();
      void cierra_input_consola();
      int hay_player(bald,bald,int);
      int hay_enem(bald,bald,int);
      void ingreso_o_retira_enem(bald, bald, int);
      int hay_bala(bald,bald,int);
      int hay_explos(bald,bald,int);
      int dist_explos(float,float,float);
      int primera_llama();
      int danyo_total_explos(float,float,float);
      warrior *oponente_cercano(warrior*,float,float,float,byte,bool,byte *categoria = 0, byte *id = 0);
      bool obstaculo(bald,bald,bald,bald);
      float centroplx();
      float centroply();
      bool single_screen();
      bool coop_nivel_completado();
	  bool partida_red_acabada();
      int tiempo_batalla();
      long tiempo_batalla_ticks();
      int maxim_frags();
      int diferencia_x(int,int,int,int);
      int diferencia_y(int,int,int,int);
      float dist_centro(float,float);
      float dist_players(int,int);
      byte num_players();
      mapa *dev_mapa();
      nivel *dev_nivel();
      warrior *dev_enem(int);
      warrior *dev_player(int);
      bala *dev_bala(int);
      explos *dev_explos(int);
      char *directorio();
      char *dev_mens(int);
      char *dev_mens_consola(int);
      char *dev_prompt_consola();
      byte mens_color(int);
      byte mens_consola_color(int);
      byte mens_prio(int);
      warrior* colision_con_guerrero(float x, float y, warrior *w);
      bool permite(float,float,warrior*);
      bool permite2(float,float,warrior*);
      bool mapa_activo();
      bool consola_activa();
      bool datos_activo();
      bool consola_input_activo();
      bool mapa_conocido(bald,bald);
      bool enemigos_en_mapa();
      byte zoom_mapa();
      void retira_bala(int);
      void retira_explos(int);
      void sonido_ambiente(s_sample,float,float);
      void sonido_warrior(warrior*,int);
      void activa_musica();
      void desactiva_musica();
      bool force_split;
      void activa_lluvia();
      void actualiza_lluvia();
      void stop_ambient_samples();

	  bool es_deathmatch() { return tipoj == DMCH; }
	  byte dev_modo_red();
	  void asigna_modo_red(byte);
	  byte dev_jug_local();
	  void asigna_jug_local(byte);
	  bool ventana_chat_abierta();
	  char* dev_mensaje_chat();
	  void asigna_tiempo_max_batalla(int t) { tiempo_max = t; }
	  int  tiempo_max_batalla() { return tiempo_max; }
	  void deshacer_solapamientos_players();

      };

void argumento(char*,int,char*);
void partir(char*,int,int,char*);
const char *smes(int);




