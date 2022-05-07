
class area {

      private:
      bald ainx, ainy, afinx, afiny;

      public:
      area(bald,bald,bald,bald);
      bool es_dentro(bald,bald);
      bald inx();
      bald iny();
      bald finx();
      bald finy();
      void salva(FILE*);
      void carga(FILE*);
      };
