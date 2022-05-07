
#define PMAXABERT 55
#define PTESPERA  200
#define PMARGEN   10

enum tpuerta {NORM,LLA1,LLA2,LLA3,LLA4,INTER,INTER_FIJA,GEMA};

class puerta {

         private:
         bald pinx,piny,pfinx,pfiny;
         tpuerta ptipo;
         word espera;
         byte pabertura;
         bool ultima_orden,phoriz;

         public:
         puerta(bald,bald,bald,bald,tpuerta);
         void actualiza();
         void activa();
         void abre(word tiempo_espera = 0);
         bald inx();
         bald iny();
         bald finx();
         bald finy();
         bool pared(bald,bald);
         bool permite();
         tpuerta tipo();
         bool horiz();
         bool abriendo();
         byte abertura();
         void salva(FILE*);
         void carga(FILE*);
         };

