#define IESPERA 20

enum i_target {PUEN=0,PUER,OSCU,DISP,LASE};

class interr {

        private:
        bald ixpos, iypos, espera;
        i_target itobj;
        bald icobjx, icobjy;
        bool ion;

        public:
        interr(bald,bald,i_target,bald,bald);
        void actualiza();
        bool permite();
        void activa();
        bald x_pos();
        bald y_pos();
        i_target tobj();
        bald cobjx();
        bald cobjy();
        bool on();
        void salva(FILE*);
        void carga(FILE*);
};
