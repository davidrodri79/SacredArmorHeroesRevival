#define P_PASO 15

class puente {

         private:
         bald pxin, pyin, pxfin, pyfin;
         word pespera,cont,paso_cont;
         bool ultima_orden,phoriz,ptempo;
         byte progreso,size;

         public:
         puente(bald,bald,bald,bald,bool,word);
         void actualiza();
         void activa();
         bool suelo(bald,bald);
         bool dentro(bald,bald);
         bool horiz();
         bald inx();
         bald iny();
         bald finx();
         bald finy();
         void salva(FILE*);
         void carga(FILE*);
         };
