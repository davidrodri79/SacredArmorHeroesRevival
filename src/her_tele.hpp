
class telep {

      private:
      bald txin, txout, tyin, tyout;

      public:
      telep(bald, bald, bald, bald);
      bald x_in();
      bald x_out();
      bald y_in();
      bald y_out();
      void salva(FILE*);
      void carga(FILE*);
};

