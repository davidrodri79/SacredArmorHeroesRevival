#define DTWAIT 40

enum d_tipo {DINTER=0, DTEMPOR};


class disparador {

      private:
      bald dx, dy;
      w_dir ddir;
      byte tbala, cont, dfuerza;
      d_tipo dtipo;
      word tiempo;

      public:
      disparador(bald,bald,w_dir,byte,d_tipo,byte,word);
      void actualiza();
      bool disparando();
      bald x_pos();
      bald y_pos();
      w_dir dir();
      byte t_bala();
      byte fuerza();
      void salva(FILE*);
      void carga(FILE*);
};
