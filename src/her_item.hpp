
#define ITREAPAR 1200

enum t_item {NING=0,RAND,
             KEY1,KEY2,KEY3,KEY4,BOTI,
             // Solo soldado
             PIST,AMET,LANZ,ANIQ,DRAG,INFE,SHOC,BALA,COHE,NAPA,
             // Solo guerrero
             DAGA,EXCA,FALC,MAZA,MART,AGUI,EDGE,EMAD,ECOB,EACE,EBRO,EPLA,EORO,EDIV,
             // Solo hechicero
             MFUE,MHIE,MELE,APOC,
             // Especial items
             WALL,BERS,INVI,SHAD,REPU,HEAL,BOLT,MAPA,VENE,DIOS,
             // Gemas
             GEM1,GEM2,GEM3};

class item {

      private:
      bald ix, iy;
      t_item itipo;
      word c_cont;

      public:
      item(bald,bald,t_item);
      void actualiza();
      void recoge();
      t_item tipo();
      bool esta(bald,bald);
      bool es_cogible();
      bald x_pos();
      bald y_pos();
      void salva(FILE*);
      void carga(FILE*);
      };

