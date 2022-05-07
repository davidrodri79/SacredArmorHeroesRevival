#include"her_tipo.hpp"
#include"her_tele.hpp"

// Crea un teleport en la posici¢n (xin,yin) que transporta a (xout,yout)
telep::telep(bald xin, bald yin, bald xout, bald yout)
{
        txin=xin; tyin=yin; txout=xout; tyout=yout;
}
// Consultoras de los diversos campos de un teleport
bald telep::x_in()
{
        return txin;
}
bald telep::x_out()
{
        return txout;
}
bald telep::y_in()
{
        return tyin;
}
bald telep::y_out()
{
        return tyout;
}
void telep::salva(FILE *fp)
{
      if(fp==NULL) {her_error("TELEPORT",ERR_FIL); return;};
      fwrite(&txin,sizeof(txin),1,fp);
      fwrite(&tyin,sizeof(tyin),1,fp);
      fwrite(&txout,sizeof(txout),1,fp);
      fwrite(&tyout,sizeof(tyout),1,fp);
}
void telep::carga(FILE *fp)
{
      if(fp==NULL) {her_error("TELEPORT",ERR_FIL); return;};
      fread(&txin,sizeof(txin),1,fp);
      fread(&tyin,sizeof(tyin),1,fp);
      fread(&txout,sizeof(txout),1,fp);
      fread(&tyout,sizeof(tyout),1,fp);
}



