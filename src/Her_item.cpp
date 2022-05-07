#include"her_tipo.hpp"
#include"her_item.hpp"

item::item(bald x, bald y, t_item tipo)
{
      ix=x; iy=y; itipo=tipo; c_cont=0;
}
void item::actualiza()
{
    if(c_cont>0) c_cont--;
}
void item::recoge()
{
    c_cont=ITREAPAR;
}
t_item item::tipo()
{
      return itipo;
}
bool item::esta(bald x, bald y)
{
      if((x==ix) && (y==iy)) return TRUE;
                 else        return FALSE;
}
bool item::es_cogible()
{
      if(c_cont==0) return TRUE;
      else          return FALSE;
}
bald item::x_pos()
{
      return ix;
}
bald item::y_pos()
{
      return iy;
}
void item::salva(FILE *fp)
{
      if (fp==NULL) {her_error("ITEM",ERR_FIL); return;};

      fwrite(&ix,sizeof(ix),1,fp);
      fwrite(&iy,sizeof(iy),1,fp);
      fwrite(&itipo,sizeof(itipo),1,fp);
}
void item::carga(FILE *fp)
{
      if (fp==NULL) {her_error("ITEM",ERR_FIL); return;};

      fread(&ix,sizeof(ix),1,fp);
      fread(&iy,sizeof(iy),1,fp);
      fread(&itipo,sizeof(itipo),1,fp);
}
