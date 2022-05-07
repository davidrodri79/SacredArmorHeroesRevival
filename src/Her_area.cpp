#include "her_tipo.hpp"
#include "her_area.hpp"

area::area(bald inx, bald iny, bald finx, bald finy)
{
        if((inx>finx) || (iny>finy))
                her_error("AREA",ERR_COO);
        else{
             ainx=inx; ainy=iny; afinx=finx; afiny=finy;
        }
}
bool area::es_dentro(bald x, bald y)
{
        if((x>=ainx) && (x<=afinx) && (y>=ainy) && (y<=afiny))
                return TRUE;
        else    return FALSE;
}
bald area::inx()
{
        return ainx;
}
bald area::iny()
{
        return ainy;
}
bald area::finx()
{
        return afinx;
}
bald area::finy()
{
        return afiny;
}
void area::salva(FILE *fp)
{
      fwrite(&ainx,sizeof(ainx),1,fp);
      fwrite(&ainy,sizeof(ainy),1,fp);
      fwrite(&afinx,sizeof(afinx),1,fp);
      fwrite(&afiny,sizeof(afiny),1,fp);
}
void area::carga(FILE *fp)
{
      fread(&ainx,sizeof(ainx),1,fp);
      fread(&ainy,sizeof(ainy),1,fp);
      fread(&afinx,sizeof(afinx),1,fp);
      fread(&afiny,sizeof(afiny),1,fp);
}
