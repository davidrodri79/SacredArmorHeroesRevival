#include <allegro5/allegro_audio.h>


#define KTI_SHIPY 160
#define KTI_MAXSHOTS 20
#define KTI_SDELAY 30
#define KTI_NLEVELS 10
#define KTI_NINVADERS 20
#define KTI_INVDELAY 30
#define KTI_TITLETIME 180
#define KTI_EXPLVEL 0.1


typedef struct shot{

        short int x,y,c;
        bool use;
};

typedef struct invader{

        float x,y,angle,state;
        unsigned int ct;

};

class kti {

        private:

        ALLEGRO_BITMAP *display, *ship[5][5], *explos[3], *planet[4], *sky[3], *border[8], *temp;
        charset *ch;
        ALLEGRO_SAMPLE *sshot, *smotor, *sexplos;
		ALLEGRO_AUDIO_STREAM* bgm;
        
        game_info *gi;
        float scroll[3], planety;
        int shipx[4], sdelay[4], planetx, planetc, sequence[KTI_NLEVELS], counter, lev;
        shot shots[KTI_MAXSHOTS];
        invader inv[KTI_NINVADERS];
        bool acab, comple;

        public:

        kti(game_info*,ALLEGRO_BITMAP*,charset*);
        void muestra(ALLEGRO_BITMAP*);
        void actualiza(t_control*);
        void nuevo_nivel();
        void update_alien(int);
        int kills();
        bool acabado();
        bool completo();
        ~kti();

};
