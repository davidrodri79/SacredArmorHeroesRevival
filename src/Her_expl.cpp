#include"her_tipo.hpp"
#include"her_item.hpp"
#include"her_warr.hpp"
#include"her_expl.hpp"


// Crea una explosi¢n con las caracter¡sticas indicadas
explos::explos(float x, float y, e_tipo t, word f, warrior *creador)
{
      px=x; py=y; etipo=t; efuerza=f;
      eacabada=FALSE; eanim=0.0; cont=0; ecreador=creador;
}
// Actualiza la explosi¢n en su ciclo l¢gico, e indicando si ha terminado.
// Notar que la de fuego, cuando muere genera una llama en el suelo que dura
// FLAMETIME ciclos.
void explos::actualiza()
{
      int maxdur;

      switch(etipo){
        case ELLA : maxdur=FLAMETIME; break;
        case ELL3 :
        case ELL2 : maxdur=FLAM2TIME; break;
        default   : maxdur=EXPTIME; break;
      };
      if(cont<maxdur) {cont++; eanim+=ANIMSP; if(eanim>=4.0) eanim-=4.0;}
      else {
            if(etipo==EFUE){

                  etipo=ELLA; cont=0; eanim=0; efuerza=2;

            }else eacabada=TRUE;
      };
}
// Consultoras
float explos::posx()
{
      return px;
}
float explos::posy()
{
      return py;
}
e_tipo explos::tipo()
{
      return etipo;
}
word explos::fuerza()
{
      return efuerza;
}
byte explos::anim()
{
      if((etipo==ELL2) || (etipo==ELL3)) return (byte)((cont/5)%3);
      if(etipo==ELLA) return (byte)eanim;

      if(cont<EXPTIME/3) return 0;
      if((cont>=EXPTIME/3) && (cont<2*EXPTIME/3)) return 1;
      if(cont>=2*EXPTIME/3) return 2;
}
bool explos::acabada()
{
      return eacabada;
}
warrior *explos::creador()
{
        return ecreador;
}

