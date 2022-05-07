#define BMAXD 200

class barrera {

      private:
      bald bx, by;
      word destruc;
      byte btipo;

      public:
      barrera(bald,bald,byte);
      void danyar(word);
      bald posx();
      bald posy();
      byte danyo();
      byte tipo();
      void salva(FILE*);
      void carga(FILE*);

      };








