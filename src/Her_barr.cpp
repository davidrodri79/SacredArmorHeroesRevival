#include"her_tipo.hpp"
#include"her_barr.hpp"

//Crea un objeto destruible (barrera) en las coordenadas (x,y). Esta barrera
//soporta un da¤o m ximo de BMAXD
barrera::barrera(bald x, bald y, byte t)
{
      bx=x; by=y; destruc=0; btipo=t;
}
//Da¤a la barrera en d unidades. El danyo m ximo aceptado es BMAXD
void barrera::danyar(word d)
{
      if(destruc+d<BMAXD) destruc+=d;
      else                destruc=BMAXD;
}
// Consultoras de posici¢n
bald barrera::posx()
{
      return bx;
}
bald barrera::posy()
{
      return by;
}
// Devuelve un byte indicando la "magnitud" del da¤o recibido. 0 inidica da¤o
// nulo o insignificante, 1 moderado, 2 grave y 3 destrucci¢n completa (debe
// eliminarse del mapa)
byte barrera::danyo()
{
      if(destruc<BMAXD/3) return 0;
      if((destruc>=BMAXD/3) && (destruc<2*BMAXD/3)) return 1;
      if((destruc>=2*BMAXD/3) && (destruc<BMAXD)) return 2;
      if(destruc==BMAXD) return 3;
}
// Devuelve el tipo. El tipo indica si es barrera de decorado, o creada
// durante la batalla
byte barrera::tipo()
{
      return btipo;
}
void barrera::salva(FILE *fp)
{
      if(fp==NULL) {her_error("BARRERA",ERR_FIL); return;};

      fwrite(&bx,sizeof(bx),1,fp);
      fwrite(&by,sizeof(by),1,fp);
}
void barrera::carga(FILE *fp)
{
      if(fp==NULL) {her_error("BARRERA",ERR_FIL); return;};

      fread(&bx,sizeof(bx),1,fp);
      fread(&by,sizeof(by),1,fp);

      destruc=0; btipo=0;
}



