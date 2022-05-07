
#define MAX_GAME 7

typedef struct {

   char nombre[10];
   w_clase clase;
   byte c1,c2,level,spr,escudo, ammo[3];
   word exp,salud;
   bool armas[10];

} player_info;


typedef struct {

   player_info pl[4];
   byte porcent,dif,n_players;
   unsigned long int tiempo;
   bool en_uso;
   char exitos[4][8], train, sacred, kti, completo;

} game_info;
