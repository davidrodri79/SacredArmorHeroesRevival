#include"her_tipo.hpp"
#include"her_item.hpp"
#include"her_warr.hpp"
#include"her_bala.hpp"

// Crea una bala con coordenadas (x,y) direcci¢n d, velocidad v, tipo t y
// fuerza f. El creador es un puntero a personaje.
bala::bala(float x, float y, w_dir d, float v, float a, b_tipo t, word f, warrior *creador)
{
      px=x; py=y; bdir=d; btipo=t; bfuerza=f; bvel=v;
      banim=0; bacc=a; bcreador=creador;
}
// Actualiza la bala, haci‚ndola avanzar en la direcci¢n a que mira
void bala::avanza()
{
      banim+=ANIMSP; if (banim>=4.0) banim-=4.0;
      switch(bdir){
         case DDER : px+=bvel; break;
         case DIZQ : px-=bvel; break;
         case DABA : py+=bvel; break;
         case DARR : py-=bvel; break;
      };
      bvel+=bacc;
}
// Consultoras
float bala::posx()
{
      return px;
}
float bala::posy()
{
      return py;
}
float bala::vel()
{
      return bvel;
}
float bala::anim()
{
      if(banim<3.0) return (byte)banim;
      else          return (byte)1;
}
b_tipo bala::tipo()
{
      return btipo;
}
w_dir bala::dir()
{
      return bdir;
}
word bala::fuerza()
{
      return bfuerza;
}
warrior *bala::creador()
{
      return bcreador;
}

