

enum b_tipo {BULL=0, ROCK, FUEG, HIEL, ELEC, FLAM};

class bala {
           private:
           float px, py, bvel, bacc, banim;
           b_tipo btipo;
           w_dir bdir;
           byte bfuerza;
           warrior *bcreador;

           public:
           bala(float,float,w_dir,float,float,b_tipo,word,warrior*);
           void avanza();
           float posx();
           float posy();
           float vel();
           float anim();
           b_tipo tipo();
           w_dir dir();
           word fuerza();
           warrior* creador();
           };

