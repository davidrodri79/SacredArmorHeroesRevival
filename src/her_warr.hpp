
#define WPASO 0.06
#define WDOLWAIT 20
#define WPUNWAIT 20
#define WTSHOT 20
#define WTSLAS 26
#define WTCAE 39
#define WTTEL 100
#define WTCONG 500
#define WTELEC 120
#define WTSPECIAL 1800
#define WTALKWAIT 60
#define WNSPECIALS 5
#define WFSEP 0.15

#define WMAXLEV 50
#define WLEVPASS 30

enum w_est {WALK=0, PUNY, SHOT, SLAS, HECH, DEFE, TELP, CONG, DIE, DEST, OUTL};

enum w_iaest {PASEO=0, HUIDA, PERSE, APUNT, ROTAR, ATACA, ESPER};

enum w_clase {SIGMA=0, BETA, OMEGA, CASUAL};

enum w_sclase {CPASEA=0,CESCAP,CINDIC,CPRISI,CCURIO,ENORM,EBOSS,CATENT};

enum w_special {INVIS=0, INVUL=1, BERSERK=2, FEAR=3, SHADOW=4};

enum cdanyo {DPUNY=0, DESPA, DBALA, DCOHE, DFUEG, DELEC, DHIEL, DLASE, DSUEL,
             DVENE, DTELE, DAPOC};

word exp_level(byte);

extern bool muerte_gore;

class warrior {

      private:
      float wx, wy, wanim;
      char wnombre[16], wfrase[100];
      w_clase wclase;
      w_sclase wsubclase;
      byte wcualspr, wcualskin, orispr, wcol[2], warma, wesc, wlev,
           max_arma, tesp_ca, telec, wbando, wia_cont, talk_cont, die_cont;
      word ammo[3], wsalud, wexp, wcong;
      w_dir wdir, oridir, wia_dir;
      word cont, cspecial[WNSPECIALS];
      w_est west;
      w_iaest wiaest;
      t_item item_as;
      bool esta_activo, armas_disp[11], escud_disp[11], llaves_disp[4], brelease, wdios, baston_mago;
      warrior *woponente, *wagresor;
      cdanyo causa;

      public:
      warrior(const char*,float,float,w_dir,byte,byte,w_clase,byte,byte,byte,word);
      warrior(const char *, float, float, w_dir, byte, byte, byte, byte, w_clase, w_sclase, byte, byte, byte, t_item, const char *);
      bool activo() { return esta_activo; }
      void set_activo(bool b) { esta_activo = b; }
      void actualiza(bool);
      void avanza(w_dir,bool,bool);
      void nueva_pos(float,float);
      void nueva_dir(w_dir);
      void nuevo_estado(w_est);
      void nueva_arma(byte);
      bool set_arma(byte, bool forzar = false);
      void nuevo_escudo(byte c, bool forzar = false);
      void nuevo_item_asociado(t_item);
      void cambia_nombre(const char*);
      void cambia_frase(const char*);
      void danyar(word,cdanyo,warrior*);
      void cambia_arma();
      void nueva_salud(word);
      void nueva_municion(byte,word);
      void obtiene_special(w_special,word);
      void obtiene_llave(byte);
      void set_modo_dios();
      void electrocutar(bool activar = true);
      void nueva_dir_ia(w_dir);
      void reset_contador_ia();
      void set_contador_ia(byte);
      void set_sclase(w_sclase);
	  void set_skin(byte s);
      void gana_exp(byte);
      void sube_level();
      void incr_muertes();
      bool interactuar();
      void nuevo_estado_ia(w_iaest);
      void nuevo_oponente(warrior*);
      void reset(float,float,w_dir,bool);
      void set_release(bool);
      void mejora_arma();
      w_clase clase();
      w_sclase subclase();
      bald x_pos();
      bald y_pos();
      float fx_pos();
      float fy_pos();
      float front_x(float);
      float front_x_2(int,float);
      float front_y(float);
      float front_y_2(int,float);
      word salud();
      w_dir dir();
      byte cualspr();
	  byte cualskin();
      byte anim();
      byte arma();
      byte escudo();
      w_est estado();
      w_iaest estado_ia();
      byte contador();
	  void set_contador(byte);
      byte color(int);
      byte municion(byte);
      t_item item_asociado();
      byte contador_ia();
      w_dir dir_ia();
      w_dir dir_original();
      char *nombre();
      char *frase();
      bool tiene_arma(byte);
      bool en_movim();
      bool special(w_special);
      bool posee_llave(byte);
      bool electrocutado();
      bool puede_danyar(word);
      bool release_boton();
      bool dios();
      byte bando();
      byte muertes();
      byte level();
      word exp();
      word salud_maxima();
      word municion_maxima(byte);
      float level_coef();
      warrior* oponente();
      warrior* agresor();
      cdanyo causa_danyo();
	  void restaura_sprite_original();
	  bool tiene_baston_mago();
	  void set_baston_mago(bool);
      void salva(FILE*);
      void carga(FILE*);

      };



