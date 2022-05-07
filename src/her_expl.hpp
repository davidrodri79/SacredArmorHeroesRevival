
#define EXPTIME 14
#define FLAMETIME 500
#define FLAM2TIME 120

enum e_tipo {EPUN=0, EBAL, ECOH, EFUE, EHIE, EELE, ELLA, ELL2, ELL3};

class explos {

      private:
      float px,py,eanim;
      e_tipo etipo;
      word efuerza;
      word cont;
      bool eacabada;
      warrior *ecreador;

      public:
      explos(float,float,e_tipo,word,warrior*);
      void actualiza();
      float posx();
      float posy();
      e_tipo tipo();
      word fuerza();
      byte anim();
      bool acabada();
      warrior *creador();
      };

