#include"her_tipo.hpp"
#include<math.h>
#include<string.h>
#include<allegro5/allegro.h>
#include"her_bata.hpp"

extern bool show_fps;

#define ENEMY_UPDATE_DIST 25

float posit(float p)
{
      if(p>=0) return p;
      else return -p;
}
float dist(float x1, float y1, float x2, float y2)
{
        float dx=x1-x2, dy=y1-y2;
        return (dx*dx)+(dy*dy);
}
bool dist_menor(float x1, float y1, float x2, float y2, float d)
{
        float dx=x1-x2, dy=y1-y2;
        if((dx*dx)+(dy*dy)<=d*d) return TRUE;
        else return FALSE;
}
bool en_linea(float x1, float y1, float x2, float y2, float d)
{
        float dx=posit(x1-x2), dy=posit(y1-y2);
        if((dx<=d) || (dy<=d)) return TRUE;
        else return FALSE;
}
w_dir dir_mirando(float x1, float y1, float x2, float y2)
{
        float dx=posit(x1-x2), dy=posit(y1-y2);

        if(dx<dy){
                  if(y1<y2) return DABA;
                  else      return DARR;
        }else{
                  if(x1<x2) return DDER;
                  else      return DIZQ;};
}
w_dir dir_inversa(w_dir d)
{
        switch(d){
                case DARR : return DABA;
                case DABA : return DARR;
                case DIZQ : return DDER;
                case DDER : return DIZQ;
				default   : return DABA;};
}
void argumento(char *line,int c,char *arg)
{
        int nsp=c, i=0, j=0;
        // Salta espacios
        while(line[i]==' ') i++;
        // Salta hasta argumento c
        for(int k=0; k<c; k++){
                while(line[i]!=' '){
                        if(line[i]=='\0') {arg[0]='\0'; return;};
                        i++;
                };
                while(line[i]==' ') i++;
                };
        // Copia tramo de line en arg
        while((line[i]!='\0') && (line[i]!=' ')){
                arg[j]=line[i];
                i++; j++;
        };
        arg[j]='\0';
}
void partir(const char *line,int c,int s,char *arg)
{
        int pos=0, i=0;
        while((pos<s*c) && (line[pos]!='\0'))
                        pos++;
        if(line[pos]=='\0') {arg[0]='\0'; return;};
        while((i<s) && (line[pos]!='\0')){
                arg[i]=line[pos];
                i++; pos++;
        }
        if((i==s) && (line[pos-1]!=' ') && (line[pos]!=' ')){arg[i]='-'; arg[i+1]='\0';}
        else    arg[i]='\0';
}
const char *smes(int i)
{
        switch(i){
                case 1 : return "January"; break;
                case 2 : return "February"; break;
                case 3 : return "March"; break;
                case 4 : return "April"; break;
                case 5 : return "May"; break;
                case 6 : return "June"; break;
                case 7 : return "July"; break;
                case 8 : return "August"; break;
                case 9 : return "September"; break;
                case 10 : return "October"; break;
                case 11 : return "November"; break;
                case 12 : return "Desember"; break;
				default : return ""; break;};
}
// Crea una nueva estructura que mantendr  una batalla en el nivel "fniv", que
// esta en el subdirectorio sdirec del actual directorio, con la estructura de
// samples sound . Esta £ltima se cargar  con los samples correspondientes a
// los warriors del nivel
batalla::batalla(const char *sdirec,const char *fniv, waves *sound, byte tipo, int sizex, bool split)
{
      char s[60];
      int i,j;
      //time t;
      //date d;
      sprintf(direc,"%s",sdirec);
      niv=DBG_NEW nivel();
      sprintf(s,"%s/%s",direc,fniv);
      niv->carga(s);
      if(niv==NULL) her_error("BATALLA",ERR_MEM);
      else{
           if(sound==0) her_error("BATALLA",ERR_NOI);
           else{
			   jug_local = 0;
			   modo_red = MROFFLINE;
               for(i=0; i<MAXPL; i++) player[i]=NULL;
               for(i=0; i<MAXBA; i++) bbala[i]=NULL;
               for(i=0; i<MAXEX; i++) bexplos[i]=NULL;
               nplayers=0; nbalas=0; nexplos=0; nmens=0; ncmens=0; prncar=0;
               bwav=sound; bcont=0; tcont=0; maxmsgsize=int((sizex-40)/13); gema=FALSE;
               tipoj=tipo;
               bwav->load_wav_especificas(direc,niv->fdecor());
               bwav->load_bgm(direc,niv->fmusic());
               for(i=0; i<NSPR; i++)
                  bwav->load_wav_warrior(direc,niv->fspr(i),i);
			   bwav->load_wav_warrior(direc,"congela",NSPR);
               modo_mapa=FALSE; modo_consola=FALSE; modo_datos=FALSE; map_op_cont=0; con_op_cont=0; dat_op_cont=0; mzoom=10;
               con_input=FALSE; force_split=split; enemy_map=FALSE; lluvia=FALSE; trueno=0;
               // Mapa 2D
               for(i=0; i<MSIZEX/5; i++)
               for(j=0; j<MSIZEY/5; j++)
               matr_mapa[i][j]=FALSE;
               // Matriz para acceso r pido a enemigos
               for(j=0; j<MSIZEY; j++)
               matr_enem[j]=DBG_NEW byte[MSIZEX];
               for(j=0; j<MSIZEY; j++)
               for(i=0; i<MSIZEX; i++)
               matr_enem[j][i]=0;

			   i=0;
               while(niv->dev_enem(i)!=NULL){
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]++;
                i++;};
               //Construye la matriz de inter‚s para pintado
               dev_mapa()->crea_matriz_interes();
               // Arranque de consola
               sprintf(lastprompt,"%c",'\0');
               logf=fopen("log.dat","w+t");
               //getdate(&d); gettime(&t);
               //sprintf(s,"Console initializedï %d %s %dth %d:%02d",d.da_year,smes(d.da_mon),d.da_day,t.ti_hour,t.ti_min);
			   sprintf(s,"Console initialized... Normal Mode");
               encola_mensaje_consola(s,117);
               encola_mensaje_consola(NOMBRE_MODULO,117);
               encola_mensaje_consola("---------------------------------------------",117);

			   mens_fin_recibido = false; interr_red_espera = 0; puerta_red_espera = 0;
			   ventana_chat = false; sprintf(chat_msg,""); chat_msg_length = 0; chat_espera = 0;
			   for(i = 0; i < MAXPL; i++)
			   {
				   last_received_x[i][0] = 0.0; last_received_x[i][1] = 0.0;
				   last_received_y[i][0] = 0.0; last_received_y[i][1] = 0.0;
			   }

               for (i = 0; i < L_NENEM; i++)
               {
                   last_received_enem_x[i][0] = 0.0; last_received_enem_x[i][1] = 0.0;
                   last_received_enem_y[i][0] = 0.0; last_received_enem_y[i][1] = 0.0;
               }

            }
      }
}
// An loga a la anterior, pero para emular niveles con formato del Sacred Armor original
batalla::batalla(const char *sdirec,const char *fniv, byte l, char *dec, char *en1, char *en2, byte c11,
                      byte c12, byte c21, byte c22, waves *sound, int sizex, bool split)
{
      char s[60];
      int i,j;
      //time t;
      //date d;
      sprintf(direc,"%s",sdirec);
      niv=DBG_NEW nivel();
      sprintf(s,"%s/%s",direc,fniv);
      niv->importar(s,l,dec,en1,en2,c11,c12,c21,c22);
      if(niv==NULL) her_error("BATALLA",ERR_MEM);
      else{
           if(sound==0) her_error("BATALLA",ERR_NOI);
           else{
			   jug_local = 0;
			   modo_red = MROFFLINE;
               for(i=0; i<MAXPL; i++) player[i]=NULL;
               for(i=0; i<MAXBA; i++) bbala[i]=NULL;
               for(i=0; i<MAXEX; i++) bexplos[i]=NULL;
               nplayers=0; nbalas=0; nexplos=0; nmens=0; ncmens=0; prncar=0;
               bwav=sound; bcont=0; tcont=0; maxmsgsize=int((sizex-40)/13);
               tipoj=COOP;
               bwav->load_wav_especificas(direc,niv->fdecor());
               bwav->load_bgm(direc,niv->fmusic());
               for(i=0; i<NSPR; i++)
                  bwav->load_wav_warrior(direc,niv->fspr(i),i);
			   bwav->load_wav_warrior(direc,"congela",NSPR);
               modo_mapa=FALSE; modo_consola=FALSE; modo_datos=FALSE; map_op_cont=0; con_op_cont=0; dat_op_cont=0; mzoom=10;
               con_input=FALSE; force_split=split; enemy_map=FALSE; lluvia=FALSE; trueno=0; gema=FALSE;
               // Mapa 2D
               for(i=0; i<MSIZEX/5; i++)
               for(j=0; j<MSIZEY/5; j++)
               matr_mapa[i][j]=FALSE;
               // Matriz para acceso r pido a enemigos
               for(j=0; j<MSIZEY; j++)
               matr_enem[j]=DBG_NEW byte[MSIZEX];
               for(j=0; j<MSIZEY; j++)
               for(i=0; i<MSIZEX; i++)
               matr_enem[j][i]=0;
               i=0;
               while(niv->dev_enem(i)!=NULL){
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]++;
                i++;};
               //Construye la matriz de inter‚s para pintado
               dev_mapa()->crea_matriz_interes();
               // Arranque de consola
               sprintf(lastprompt,"%c",'\0');
               logf=fopen("log.dat","w+t");
               //getdate(&d); gettime(&t);
               //sprintf(s,"Console initializedï %d %s %dth %d:%02d",d.da_year,smes(d.da_mon),d.da_day,t.ti_hour,t.ti_min);
			   sprintf(s,"Console initialized... Emulation Mode");
               encola_mensaje_consola(s,117);
               encola_mensaje_consola(NOMBRE_MODULO,117);
               encola_mensaje_consola("---------------------------------------------",117);

			   mens_fin_recibido = false; interr_red_espera = 0; puerta_red_espera = 0;
			   ventana_chat = false; sprintf(chat_msg,""); chat_msg_length = 0; chat_espera = 0;
			   for(i = 0; i < MAXPL; i++)
			   {
				   last_received_x[i][0] = 0.0; last_received_x[i][1] = 0.0;
				   last_received_y[i][0] = 0.0; last_received_y[i][1] = 0.0;
			   }

               for (i = 0; i < L_NENEM; i++)
               {
                   last_received_enem_x[i][0] = 0.0; last_received_enem_x[i][1] = 0.0;
                   last_received_enem_y[i][0] = 0.0; last_received_enem_y[i][1] = 0.0;
               }
            }
      }
}
batalla::~batalla()
{
        int i,j;
        delete niv;
        for(i=0; i<MAXPL; i++) delete player[i];
        for(i=0; i<MAXBA; i++) delete bbala[i];
        for(i=0; i<MAXEX; i++) delete bexplos[i];
        for(j=0; j<MSIZEY; j++)
        delete[] matr_enem[j];
        fclose(logf);
}
// A¤ade un jugador al juego, que ser  controlado por el usuario con el metodo
// correspondiente (0-MAXPL-1) al orden en que se ha a¤adido
void batalla::ingresa_player(char *nombre, byte nspr, w_clase clase, byte c1, byte c2, t_control t,byte lev, word exp)
{
      bald x,y;
      w_dir d;
      byte b;

      if(nplayers>=MAXPL) her_error("BATALLA",ERR_LLE);
      else{
        contr[nplayers]=t;
        if(tipoj==COOP){

                x=niv->dev_mapa()->x_inicio()+(nplayers%2);
                y=niv->dev_mapa()->y_inicio()+(nplayers/2);
                d=niv->dev_mapa()->dir_inicio();
                b=0;
        }else{
                do{
                        x=rand()%MSIZEX;
                        y=rand()%MSIZEY;
                        d=(w_dir)(rand()%4);
                        b=nplayers;
                }while(!permite2(x,y,NULL));
        };
      player[nplayers]=DBG_NEW warrior(nombre,x,y,d,nspr,b,clase,c1,c2,lev,exp);
      nplayers++;
      };
}
// A¤ade una bala m¢vil a la estructura
void batalla::ingresa_bala(float x, float y, w_dir d, float v, float a, b_tipo t, byte f, warrior *c)
{
      if(nbalas>=MAXBA) her_error("BATALLA",ERR_LLE);
      else{
           bbala[nbalas]=DBG_NEW bala(x,y,d,v,a,t,f,c);
           nbalas++;
      };
}
void batalla::crea_bala_red(float x, float y, w_dir d, float v, float a, b_tipo t, byte f, byte tipo_creador, byte id_creador, s_sample sonido,
								CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	TMsgNewBullet *lMsg = DBG_NEW TMsgNewBullet;

	lMsg->mType = eMsgNewBullet;
	lMsg->mSender = getMaskFromPlayerId(jug_local);
	lMsg->mReceiver = MASK_SERVER;
	lMsg->mX = x;
	lMsg->mY = y;
	lMsg->mDir = d;
	lMsg->mVel = v;
	lMsg->mAcc = a;
	lMsg->mBulType = t;
	lMsg->mDamage = f;
	lMsg->mSample = sonido;
	lMsg->mCreatorType = tipo_creador;
	lMsg->mCreatorId = id_creador;

	if(modo_red == MRSERVER)
		aRecQueue->queueMessage(lMsg);
	else
		aSendQueue->queueMessage(lMsg);

}
// A¤ade una bala m¢vil a la estructura
void batalla::ingresa_explos(float x, float y, e_tipo t, byte f, warrior *c)
{
      s_sample s=S_NULO;

      if(nexplos>=MAXEX) her_error("BATALLA",ERR_LLE);
      else{
           bexplos[nexplos]=DBG_NEW explos(x,y,t,f,c);
           nexplos++;
           switch(t){
               case EBAL : s=S_EXBA; break;
               case ECOH : s=S_EXCO; break;
               case EFUE : s=S_EXFU; break;
               case EHIE : s=S_EXHI; break;
               case EELE : s=S_EXEL; break;};

           sonido_ambiente(s,x,y);
      };
}
void batalla::crea_explos_red(float x, float y, e_tipo t, byte f, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	TMsgNewExplos *lMsg = DBG_NEW TMsgNewExplos;

	lMsg->mType = eMsgNewExplos;
	lMsg->mSender = modo_red == MRSERVER ? MASK_SERVER : getMaskFromPlayerId(jug_local);
	lMsg->mReceiver = MASK_SERVER;
	lMsg->mCreatorType = tipo_creador;
	lMsg->mCreatorId = id_creador;
	lMsg->mX = x;
	lMsg->mY = y;
	lMsg->mExpType = t;
	lMsg->mDamage = f;

	if(modo_red == MRSERVER)
		aRecQueue->queueMessage(lMsg);
	else
		aSendQueue->queueMessage(lMsg);
}
// Elimina items iniciales si estos no son £tiles para los jugadores de la batalla
void batalla::elimina_items_inutiles()
{
    bool sig=FALSE, bet=FALSE, ome=FALSE;

    for(int i=0; i<nplayers; i++){
            if(player[i]->clase()==SIGMA) sig=TRUE;
            if(player[i]->clase()==BETA) bet=TRUE;
            if(player[i]->clase()==OMEGA) ome=TRUE;};
    niv->elimina_items_inutiles(sig,bet,ome,nplayers);
}
// Ajusta el nivel de los enemigos para que sea proporcional al nivel de dificultad
// y a las proporciones del nivel
void batalla::ajusta_enemigos(int dif, byte l)
{
        byte lmed=0;
        int i;
        warrior *e;
        // dif (1-5), l (1-7)

/*        for(int j=0; j<nplayers; j++) lmed+=player[j]->level();
        lmed=(byte)(lmed/nplayers);*/

        lmed=(byte)(((dif-1)*7+l)-1);
        lmed+=(nplayers-1)*3;

        i=0;
        while(dev_enem(i)!=NULL){
            e=dev_enem(i);
            for(int j=0; j<lmed; j++)
                    if(e->clase()!=CASUAL) e->sube_level();

            if(e->subclase()==EBOSS) {e->sube_level(); e->sube_level();};

            e->nueva_salud((word)((float)e->salud()*e->level_coef()));

            if(lmed>20) e->mejora_arma();
            if(lmed>40) e->mejora_arma();
            i++;
        };

}
//Ingresa un item en tiempo de batalla si es £til para los jugadores existentes
void batalla::ingresa_item_util(bald x, bald y,t_item t)
{
    bool sig=FALSE, bet=FALSE, ome=FALSE;
    int cant=1;

    for(int i=0; i<nplayers; i++){
            if(player[i]->clase()==SIGMA) sig=TRUE;
            if(player[i]->clase()==BETA) bet=TRUE;
            if(player[i]->clase()==OMEGA) ome=TRUE;};
    if((sig==FALSE) && ((t==PIST) || (t==AMET) || (t==LANZ) ||
        (t==ANIQ) || (t==DRAG) || (t==INFE) || (t==SHOC) || (t==BALA) ||
        (t==COHE) || (t==NAPA)))
        return;
    if((bet==FALSE) && ((t==MFUE) || (t==MHIE) || (t==MELE) || (t==APOC)))
        return;
    if((ome==FALSE) && ((t==DAGA) || (t==EXCA) || (t==FALC) ||
        (t==MAZA) || (t==MART) || (t==AGUI) || (t==EDGE) || (t==EMAD) ||
        (t==ECOB) || (t==EACE) || (t==EBRO) || (t==EPLA) || (t==EORO) ||
        (t==EDIV)))
        return;
    if((t>=KEY1) && (t<=KEY4)) cant=nplayers;

    for(int j=0; j<cant; j++)
    niv->ingresa_item(x,y,t);
}
//Ingresa un item en tiempo de batalla si es £til para los jugadores existentes
void batalla::ingresa_item_util_red(bald x, bald y, t_item t, CMessageQueue *recQueue, CMessageQueue *sendQueue)
{
    bool sig = FALSE, bet = FALSE, ome = FALSE;
    int cant = 1;

    for (int i = 0; i < nplayers; i++) {
        if (player[i]->clase() == SIGMA) sig = TRUE;
        if (player[i]->clase() == BETA) bet = TRUE;
        if (player[i]->clase() == OMEGA) ome = TRUE;
    };
    if ((sig == FALSE) && ((t == PIST) || (t == AMET) || (t == LANZ) ||
        (t == ANIQ) || (t == DRAG) || (t == INFE) || (t == SHOC) || (t == BALA) ||
        (t == COHE) || (t == NAPA)))
        return;
    if ((bet == FALSE) && ((t == MFUE) || (t == MHIE) || (t == MELE) || (t == APOC)))
        return;
    if ((ome == FALSE) && ((t == DAGA) || (t == EXCA) || (t == FALC) ||
        (t == MAZA) || (t == MART) || (t == AGUI) || (t == EDGE) || (t == EMAD) ||
        (t == ECOB) || (t == EACE) || (t == EBRO) || (t == EPLA) || (t == EORO) ||
        (t == EDIV)))
        return;
    if ((t >= KEY1) && (t <= KEY4)) cant = nplayers;

    for (int j = 0; j < cant; j++)
    {
        //niv->ingresa_item(x, y, t);

        TMsgNewItem* lMsg = DBG_NEW TMsgNewItem;

        lMsg->mType = eMsgNewItem;
        lMsg->mSender = MRSERVER;
        lMsg->mReceiver = MRSERVER;
        lMsg->mX = x;
        lMsg->mY = y;
        lMsg->mItemType = t;
 
        recQueue->queueMessage(lMsg);      
    }
}
// Crea una pared de llamas de longitud l, en horiz. o vert., con centro en
// (x,y)
void batalla::pared_de_fuego(float x, float y, byte l, byte f, bool horiz, warrior *c)
{
        for(float i=-(l/2); i<(l/2); i+=1.0)
                if(horiz) ingresa_explos(x+i,y,EFUE,f,c);
                else      ingresa_explos(x,y+i,EFUE,f,c);
}
void batalla::pared_de_fuego_red(float x, float y, byte l, byte f, bool horiz, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
        for(float i=-(l/2); i<(l/2); i+=1.0)
                if(horiz) crea_explos_red(x+i,y,EFUE,f,tipo_creador,id_creador,aRecQueue,aSendQueue);
                else      crea_explos_red(x,y+i,EFUE,f,tipo_creador,id_creador,aRecQueue,aSendQueue);
}
// Electrocutar a todos los personajes que no sean del bando [bando] y esten
// En un radio de [dist] de (x,y)
void batalla::bolt(float x, float y, float dist, byte bando, word danyo, warrior *c)
{
        float px, py;
        for(int k=0; k<nplayers; k++){
                px=player[k]->fx_pos()-x;
                py=player[k]->fy_pos()-y;
                if(player[k]->bando()!=bando)
                if((px*px)+(py*py)<=dist*dist)
                        ingresa_explos(player[k]->fx_pos(),player[k]->fy_pos(),EELE,danyo,c);
        };
        int i=0;
        while(dev_enem(i)!=NULL){
                px=dev_enem(i)->fx_pos()-x;
                py=dev_enem(i)->fy_pos()-y;
                if(dev_enem(i)->bando()!=bando)
                if((px*px)+(py*py)<=dist*dist)
                        ingresa_explos(dev_enem(i)->fx_pos(),dev_enem(i)->fy_pos(),EELE,danyo,c);
        i++;
        };
}
void batalla::bolt_red(float x, float y, float dist, byte bando, word danyo, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
        float px, py;
        for(int k=0; k<nplayers; k++){
                px=player[k]->fx_pos()-x;
                py=player[k]->fy_pos()-y;
                if(player[k]->bando()!=bando)
                if((px*px)+(py*py)<=dist*dist)
                        crea_explos_red(player[k]->fx_pos(),player[k]->fy_pos(),EELE,danyo,tipo_creador,id_creador,aRecQueue,aSendQueue);
        };
		/*int i=0;
        while(dev_enem(i)!=NULL){
                px=dev_enem(i)->fx_pos()-x;
                py=dev_enem(i)->fy_pos()-y;
                if(dev_enem(i)->bando()!=bando)
                if((px*px)+(py*py)<=dist*dist)
                        ingresa_explos(dev_enem(i)->fx_pos(),dev_enem(i)->fy_pos(),EELE,danyo,c);
        i++;
        };*/
}
// Da¤o sin explosi¢n a todo personaje que est‚ a dist menor o igual que [dist]
// de (x,y)
void batalla::apocalipsis(float x, float y, float dist, word danyo, warrior *c)
{
        float px,py;
        for(int k=0; k<nplayers; k++){
            px=player[k]->fx_pos()-x;
            py=player[k]->fy_pos()-y;
            if((player[k]!=c) && (player[k]->estado()!=DIE) && (player[k]->estado()!=DEST) && (player[k]->estado()!=OUTL) && (!player[k]->special(INVUL))
                        && (!obstaculo((bald)x,(bald)y,player[k]->x_pos(),player[k]->y_pos())))
            if(((px*px)+(py*py)<=dist*dist) && ((px*px)+(py*py)>1)){
                player[k]->danyar(danyo,DAPOC,c);
                sonido_warrior(player[k],0);};
        };
        int i=0;
        while(dev_enem(i)!=NULL){
                px=dev_enem(i)->fx_pos()-x;
                py=dev_enem(i)->fy_pos()-y;
                if((dev_enem(i)!=c) && (dev_enem(i)->estado()!=DIE) && (dev_enem(i)->estado()!=DEST) && (dev_enem(i)->estado()!=OUTL) && (!dev_enem(i)->special(INVUL))
                        && (!obstaculo((bald)x,(bald)y,dev_enem(i)->x_pos(),dev_enem(i)->y_pos())))
                if((px*px)+(py*py)<=dist*dist){
                    dev_enem(i)->danyar(danyo,DAPOC,c);
                    sonido_warrior(dev_enem(i),0);};
        i++;
        }

}
void batalla::apocalipsis_red(float x, float y, float dist, word danyo, byte tipo_creador, byte id_creador, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
        float px,py;
        for(int k=0; k<nplayers; k++){
            px=player[k]->fx_pos()-x;
            py=player[k]->fy_pos()-y;
            if((k != id_creador) && (player[k]->estado()!=DIE) && (player[k]->estado()!=DEST) && (player[k]->estado()!=OUTL) && (!player[k]->special(INVUL))
                        && (!obstaculo((bald)x,(bald)y,player[k]->x_pos(),player[k]->y_pos())))
            if(((px*px)+(py*py)<=dist*dist) && ((px*px)+(py*py)>1)){
				envia_danyo_red(id_creador,DAPOC,danyo,OPPLAYER,k,aRecQueue,aSendQueue);
                sonido_warrior(player[k],0);
			};
        };
      /*  int i=0;
        while(dev_enem(i)!=NULL){
                px=dev_enem(i)->fx_pos()-x;
                py=dev_enem(i)->fy_pos()-y;
                if((dev_enem(i)!=c) && (dev_enem(i)->estado()!=DIE) && (dev_enem(i)->estado()!=DEST) && (dev_enem(i)->estado()!=OUTL) && (!dev_enem(i)->special(INVUL))
                        && (!obstaculo((bald)x,(bald)y,dev_enem(i)->x_pos(),dev_enem(i)->y_pos())))
                if((px*px)+(py*py)<=dist*dist){
                    dev_enem(i)->danyar(danyo,DAPOC,c);
                    sonido_warrior(dev_enem(i),0);};
        i++;
        }*/

}
void batalla::encola_mensaje(const char *frase, byte color, byte prio)
{
      int j=0; char f[MSGSIZE];
      do{
                partir(frase,j,maxmsgsize,f);
                if((nmens<MAXMS) && (strlen(f)>0)){
                        sprintf(bmensajes[nmens],"%s",f);
                        bmenscolor[nmens]=color;
                        bmenstime[nmens]=MSGTIME;
                        bmensprio[nmens]=prio;
                        nmens++;
                };
                j++;
      }while(strlen(f)>0);
      encola_mensaje_consola(frase,color);
}
void batalla::encola_mensaje_consola(const char *frase, byte color)
{
      int j=0; char f[MSGSIZE];
      fprintf(logf,"%s\n",frase);
      do{
        partir(frase,j,maxmsgsize,f);
        if(strlen(f)>0)
        if(ncmens<MAXMS){
                sprintf(cmensajes[ncmens],"%s",f);
                cmenscolor[ncmens]=color;
                ncmens++;
        }else{
              for(int i=0; i<MAXMS-1; i++){
                sprintf(cmensajes[i],"%s",cmensajes[i+1]);
                cmenscolor[i]=cmenscolor[i+1];
                };
              sprintf(cmensajes[MAXMS-1],"%s",f);
              cmenscolor[MAXMS-1]=color;
        }
        j++;
      }while(strlen(f)>0);
}
void batalla::encola_mensaje_red(const char *txt, byte color, byte prio, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	TMsgConsoleLog *lMsg = DBG_NEW TMsgConsoleLog;

	lMsg->mType = eMsgConsoleLog;
	lMsg->mSender = getMaskFromPlayerId(jug_local);
	lMsg->mReceiver = MASK_SERVER;
	sprintf(lMsg->mText,"%s",txt);
	lMsg->mColor = color;
	lMsg->mPrio = prio;

	if(modo_red == MRSERVER)
	{
		aRecQueue->queueMessage(lMsg);
	}
	else
	{
		aSendQueue->queueMessage(lMsg);
	}
}
bool batalla::identifica_guerrero(warrior *w, byte &tipo, byte &id)
{
	int i;
	for(i=0; i<nplayers; i++){
		if(player[i] == w)
		{
			tipo = OPPLAYER;
			id = i;
			return true;
		}
	}
	i=0;
    while(dev_enem(i)!=NULL){
        if(dev_enem(i) == w)
		{
			tipo = OPENEMY;
			id = i;
			return true;
		}
        i++;
	}
	return false;
}
// Actualiza todos los elementos moviles de la estructura: Jugadores,
// enemigos, balas, explosiones, elementos del mapa.
void batalla::actualiza(int dif)
{
      barrera *ba;
      disparador *di;
      puerta *pu;
      warrior *wa;
      explos *exp_cerc=NULL;
      item *it;
      controlm cont;
      int i,j,k,s;
      float x,y;
      bool si;
      bald cx, cy, xx, yy;

      bcont=(bcont+1)%100; tcont++;

      // Actualiza el mapa
      dev_mapa()->actualiza();
      // Operar con la consola de s.o.
      if(con_op_cont>0) con_op_cont--;
      if((cont.tecla_consola()) && (con_op_cont==0) && (modo_datos==FALSE)){
                con_op_cont=MPTWAIT;
//                clear_keybuf(); prncar=0;
				controlm::flushallkeys();
                if (modo_consola) modo_consola=FALSE;
                else           modo_consola=TRUE;};
      // Operar con el mapa 2D
      if(map_op_cont>0) map_op_cont--;
      if((cont.tecla_mapa()) && (map_op_cont==0) && (tipoj==COOP)){
                map_op_cont=MPTWAIT;
                if (modo_mapa) modo_mapa=FALSE;
                else           modo_mapa=TRUE;};
      // Activa datos players en pantalla
      if(dat_op_cont>0) dat_op_cont--;
      if((cont.tecla_datos()) && (dat_op_cont==0) && (tipoj==COOP) && (modo_consola==FALSE)){
                dat_op_cont=MPTWAIT;
                if (modo_datos) modo_datos=FALSE;
                else           modo_datos=TRUE;};
      if((cont.tecla_mas()) && (map_op_cont==0)){
                map_op_cont=MPTWAIT;
                if(mzoom<20) mzoom++; };
      if((cont.tecla_menos()) && (map_op_cont==0)){
                map_op_cont=MPTWAIT;
                if(mzoom>1) mzoom--; };
      // Mueve los jugadores
      for(i=0; i<nplayers; i++){
              cont.actualiza(contr[i]);
              actualiza_player(i,player[i],&cont,TRUE);
              matr_mapa[player[i]->x_pos()/5][player[i]->y_pos()/5]=TRUE;
              matr_mapa[(player[i]->x_pos()/5)-1][(player[i]->y_pos()/5)]=TRUE;
              matr_mapa[(player[i]->x_pos()/5)+1][(player[i]->y_pos()/5)]=TRUE;
              matr_mapa[(player[i]->x_pos()/5)][(player[i]->y_pos()/5)-1]=TRUE;
              matr_mapa[(player[i]->x_pos()/5)][(player[i]->y_pos()/5)+1]=TRUE;                            };
      // Mueve los enemigos / secundarios
      i=0;
      while(dev_enem(i)!=NULL){
              si=TRUE;
              x=dev_enem(i)->x_pos();
              y=dev_enem(i)->y_pos();
              si=FALSE;
              for(j=0; j<nplayers; j++)
                if(dist_menor(player[j]->x_pos(),player[j]->y_pos(),x,y,ENEMY_UPDATE_DIST))
                    si=TRUE;
              if(si){
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]--;
                cont.reset();
                actualiza_ia_control(dev_enem(i),&cont,i,dif,NULL,NULL);
                actualiza_player(8,dev_enem(i),&cont,FALSE);
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]++;
              };
      i++;};
      // Actualiza los items recogibles
      i=0;
      while(niv->dev_item(i)!=NULL){
              niv->dev_item(i)->actualiza(); i++;};
      // Mueve las balas
      for(i=nbalas-1; i>=0; i--)
              actualiza_bala(i);
      // Maneja explosiones
      i=0;
      while(bexplos[i]!=NULL){
              bexplos[i]->actualiza();
              if(bexplos[i]->acabada()) {retira_explos(i); i--;};
              i++;
              };
      // Da¤a y elimina barreras
      i=0;
      while((ba=dev_mapa()->dev_barr(i))!=NULL){
              if((k=dist_explos((float)ba->posx()+0.5,(float)ba->posy()+0.5,1.0))>=0)
                     ba->danyar(danyo_total_explos((float)ba->posx()+0.5,(float)ba->posy()+0.5,1.0)/3);
              i++;
      };
      // Disparadores
      i=0;
      while((di=dev_mapa()->dev_disp(i))!=NULL){

              if(di->disparando()){

                if(di->t_bala()==(byte)FLAM){
                   if((di->dir()==DABA) || (di->dir()==DARR))
                   ingresa_explos(di->x_pos()+0.5,di->y_pos()+0.5,ELL3,di->fuerza(),NULL);
                   else
                   ingresa_explos(di->x_pos()+0.5,di->y_pos()+0.5,ELL2,di->fuerza(),NULL);
                }else{
                        if(di->disparando())
                                ingresa_bala(di->x_pos()+0.5,di->y_pos()+0.5,di->dir(),0.2,0,(b_tipo)di->t_bala(),di->fuerza(),NULL);
                };
                sonido_ambiente(S_TRAM,di->x_pos(),di->y_pos());
              };
              i++;
      };
      // LLuvia
      if(lluvia) actualiza_lluvia();

      // Subsistema de mensajes
      for(i=0; i<nmens; i++){
              bmenstime[i]--;
              if(bmenstime[i]==0) {desencola_mensaje(i); i--;};
      };
      // Sonidos
      i=0;
      while((pu=dev_mapa()->dev_puerta(i))!=NULL){

                if(pu->abertura()==1){
                        if(pu->abriendo())
                              sonido_ambiente(S_PUE1,pu->inx(),pu->iny());
                        else
                              sonido_ambiente(S_PUE2,pu->inx(),pu->iny());
                };
                if(pu->abertura()==PMAXABERT-1){
                        if(pu->abriendo())
                              sonido_ambiente(S_PUE2,pu->inx(),pu->iny());
                        else
                              sonido_ambiente(S_PUE1,pu->inx(),pu->iny());
                };
              i++;
      };
      // Control del crepitar del fuego
      x=1000;
      for(i=0; i<nplayers; i++)
            for(j=0; j<nexplos; j++)
                     if(bexplos[j]->tipo()>=ELLA){
                                y=dist(player[i]->fx_pos(),player[i]->fy_pos(),bexplos[j]->posx(),bexplos[j]->posy());
                                if(y<x) {x=y; exp_cerc=bexplos[j];};// cuadrado de la distancia
                     };

      if((x<16*16) && (!bwav->wav_llama_activa()))
                bwav->activa_wav_llama();
      if((x>16*16) && (bwav->wav_llama_activa()))
                bwav->apaga_wav_llama();
      if(bwav->wav_llama_activa())
      bwav->actualiza_wav_llama(diferencia_x(640,400,exp_cerc->posx(),exp_cerc->posy()),
                                dist_centro(exp_cerc->posx(),exp_cerc->posy()),TRUE);

      // Control de las fuentes de sonido ambiente
      for(k=0; k<N_SMP_AMBI; k++){

        x=1000;
        for(i=0; i<nplayers; i++){
            j=0;
            while((s=dev_mapa()->dev_sonido_amb(j,xx,yy))>=0){
                     if(s==(int)k){
                                y=dist(player[i]->fx_pos(),player[i]->fy_pos(),xx,yy);
                                if(y<x) {x=y; cx=xx; cy=yy;};// cuadrado de la distancia
                     };
                     j++;
            };
        };
        if((x<16*16) && (!bwav->wav_ambient_activo(k))) bwav->activa_wav_ambient(k);
        if((x>16*16) && (bwav->wav_ambient_activo(k))) bwav->apaga_wav_ambient(k);
        if(bwav->wav_ambient_activo(k))
                bwav->actualiza_wav_ambient(k,diferencia_x(640,400,cx,cy),dist_centro(cx,cy),TRUE);
      };

      if(modo_consola) actualiza_consola();
}

extern TMsgStartMatch* gStartingMessage;

void batalla::actualiza_modo_red(CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	 controlm cont;
	 int i,k;

	 byte prio, allprio = PRIO1 | PRIO2 | PRIO3 | PRIO4 | PRIO5 | PRIO6 | PRIO7 | PRIO8;

       switch(jug_local){
                case 0 : prio=PRIO1; break;
                case 1 : prio=PRIO2; break;
                case 2 : prio=PRIO3; break;
                case 3 : prio=PRIO4; break;
				case 4 : prio=PRIO5; break;
                case 5 : prio=PRIO6; break;
                case 6 : prio=PRIO7; break;
                case 7 : prio=PRIO8; break;};

	 bcont=(bcont+1)%100; tcont++;

	 // Procesa mensajes de red
	 TMessage *lMsg;
	 while((lMsg = aRecQueue->extractFirst()) != 0)
	 {
         if (lMsg->mType == eMsgStartMatch)
         {
             gStartingMessage = (TMsgStartMatch*)lMsg;
             mens_fin_recibido = true;
         }
		 // Actualiza jugador
		 else if(lMsg->mType == eMsgPlUpdate)
		 {
			TMsgPlUpdate *lMsgUpdate = (TMsgPlUpdate*)lMsg;

            if (lMsgUpdate->mCharacterType == 0)
            {
                // Actualización de JUGADOR
                warrior* pl = player[lMsgUpdate->mPlayerId];

                pl->set_activo(true);

                if (fabs(lMsgUpdate->mX - pl->fx_pos()) >= 10.f || fabs(lMsgUpdate->mY - pl->fy_pos()) >= 10.f)
                {
                    pl->nueva_pos(lMsgUpdate->mX, lMsgUpdate->mY);
                }

                if ((w_est)lMsgUpdate->mState != pl->estado())
                {
                    if (pl->estado() == DIE || pl->estado() == DEST)
                    {
                        pl->nueva_pos(lMsgUpdate->mX, lMsgUpdate->mY);
                        pl->restaura_sprite_original();
                    }

                    if (lMsgUpdate->mState == PUNY)
                    {
                        if (!permite(pl->front_x(0.5), pl->front_y(0.5), pl))
                            sonido_ambiente(S_PUNY, pl->fx_pos(), pl->fy_pos());
                    }

                    if (lMsgUpdate->mState == SLAS)
                    {
                        sonido_ambiente(S_ESPA, pl->fx_pos(), pl->fy_pos());
                    }

                    pl->nuevo_estado((w_est)lMsgUpdate->mState);
                }
                if (lMsgUpdate->mState == WALK) pl->set_contador(lMsgUpdate->mCounter);
                if (!pl->tiene_arma(lMsgUpdate->mWeapon)) pl->nueva_arma(lMsgUpdate->mWeapon);
                if (lMsgUpdate->mWeapon != pl->arma()) pl->set_arma(lMsgUpdate->mWeapon, true);
                if (pl->clase() == OMEGA && pl->escudo() != lMsgUpdate->mShield) pl->nuevo_escudo(lMsgUpdate->mShield, true);
                pl->nueva_salud(lMsgUpdate->mHealth);
                if (last_received_y[lMsgUpdate->mPlayerId][1] == 0.f && last_received_y[lMsgUpdate->mPlayerId][1] == 0.f)
                    pl->nueva_pos(lMsgUpdate->mX, lMsgUpdate->mY);
                for (int j = 0; j < 5; j++)
                    pl->obtiene_special((w_special)j, lMsgUpdate->mSpecials[j] != 0 ? 1000 : 0);
                if (lMsgUpdate->mElec)
                    pl->electrocutar();
                else
                    pl->electrocutar(false);
                last_received_x[lMsgUpdate->mPlayerId][0] = last_received_x[lMsgUpdate->mPlayerId][1];
                last_received_y[lMsgUpdate->mPlayerId][0] = last_received_y[lMsgUpdate->mPlayerId][1];
                last_received_x[lMsgUpdate->mPlayerId][1] = lMsgUpdate->mX;
                last_received_y[lMsgUpdate->mPlayerId][1] = lMsgUpdate->mY;
                pl->nueva_dir((w_dir)lMsgUpdate->mDir);

                // Reenvia a los demas jugadores
                if (modo_red == MRSERVER)
                {
                    lMsgUpdate->mReceiver = MASK_ALL_CLIENTS;
                    aSendQueue->queueMessage(lMsgUpdate);
                }
                else
                    delete lMsg;
            }
            else
            {
                // Actualización de ENEMIGO
                warrior* pl =dev_enem(lMsgUpdate->mPlayerId);

                pl->set_activo(true);

                if (fabs(lMsgUpdate->mX - pl->fx_pos()) >= 10.f || fabs(lMsgUpdate->mY - pl->fy_pos()) >= 10.f)
                {
                    matr_enem[pl->y_pos()][pl->x_pos()]--;
                    pl->nueva_pos(lMsgUpdate->mX, lMsgUpdate->mY);
                    matr_enem[pl->y_pos()][pl->x_pos()]++;
                }

                if ((w_est)lMsgUpdate->mState != pl->estado())
                {
                    if (pl->estado() == DIE || pl->estado() == DEST)
                    {
                        pl->nueva_pos(lMsgUpdate->mX, lMsgUpdate->mY);
                        pl->restaura_sprite_original();
                    }

                    if (lMsgUpdate->mState == PUNY)
                    {
                        if (!permite(pl->front_x(0.5), pl->front_y(0.5), pl))
                            sonido_ambiente(S_PUNY, pl->fx_pos(), pl->fy_pos());
                    }

                    if (lMsgUpdate->mState == SLAS)
                    {
                        sonido_ambiente(S_ESPA, pl->fx_pos(), pl->fy_pos());
                    }

                    pl->nuevo_estado((w_est)lMsgUpdate->mState);
                }
                if (lMsgUpdate->mState == WALK) pl->set_contador(lMsgUpdate->mCounter);
              
                pl->nueva_salud(lMsgUpdate->mHealth);
              
                for (int j = 0; j < 5; j++)
                    pl->obtiene_special((w_special)j, lMsgUpdate->mSpecials[j] != 0 ? 1000 : 0);
                if (lMsgUpdate->mElec)
                    pl->electrocutar();
                else
                    pl->electrocutar(false);
                last_received_enem_x[lMsgUpdate->mPlayerId][0] = last_received_enem_x[lMsgUpdate->mPlayerId][1];
                last_received_enem_y[lMsgUpdate->mPlayerId][0] = last_received_enem_y[lMsgUpdate->mPlayerId][1];
                last_received_enem_x[lMsgUpdate->mPlayerId][1] = lMsgUpdate->mX;
                last_received_enem_y[lMsgUpdate->mPlayerId][1] = lMsgUpdate->mY;
                pl->nueva_dir((w_dir)lMsgUpdate->mDir);
            }
		 }
		 // Abre puerta por petición de jugador
		 else if(lMsg->mType == eMsgOpenDoor)
		 {
			 TMsgOpenDoor *lMsgOpen = (TMsgOpenDoor*)lMsg;

			 dev_mapa()->dev_puerta(lMsgOpen->mDoorId)->abre(PTESPERA);

			 // Reenvia a los demas jugadores
			if(modo_red == MRSERVER)
			{
				lMsgOpen->mSender = MASK_SERVER;
				lMsgOpen->mReceiver = MASK_ALL_CLIENTS;
				aSendQueue->queueMessage(lMsgOpen);
			}
			else
				delete lMsg;
		 }
		 // Mensaje para la consola
		 else if(lMsg->mType == eMsgConsoleLog)
		 {
			 TMsgConsoleLog *lMsgLog = (TMsgConsoleLog*)lMsg;

			 encola_mensaje(lMsgLog->mText, lMsgLog->mColor, lMsgLog->mPrio);

			 // Reenvia a los demas jugadores
			if(modo_red == MRSERVER)
			{
				lMsgLog->mSender = MASK_SERVER;
				lMsgLog->mReceiver = MASK_ALL_CLIENTS;
				aSendQueue->queueMessage(lMsgLog);
			}
			else
				delete lMsg;
		 }
		 // Alguien quiere recoger un item
		 else if(lMsg->mType == eMsgRequestItem)
		 {
			 if (modo_red == MRSERVER)
			 {
				 TMsgRequestItem *lMsgItem = (TMsgRequestItem*)lMsg;

				 if(dev_nivel()->dev_item(lMsgItem->mItemId)->es_cogible())
				 {
					 dev_nivel()->dev_item(lMsgItem->mItemId)->recoge();

					 // Notifica que el item ha sido retirado
					 TMsgRemoveItem *lMsgItemTaken = DBG_NEW TMsgRemoveItem;
					 lMsgItemTaken->mType = eMsgRemoveItem;
					 lMsgItemTaken->mSender = MASK_SERVER;
					 lMsgItemTaken->mReceiver = MASK_ALL_CLIENTS;
					 lMsgItemTaken->mItemId = lMsgItem->mItemId;
					 aSendQueue->queueMessage(lMsgItemTaken);

					 // Concede el item al jugador
					 TMsgConcealItem *lMsgGive = DBG_NEW TMsgConcealItem;
					 lMsgGive->mType = eMsgConcealItem;
					 lMsgGive->mSender = MASK_SERVER;
					 lMsgGive->mReceiver = lMsgItem->mSender;
					 lMsgGive->mPlayerId = lMsgItem->mPlayerId;
					 lMsgGive->mItemId = lMsgItem->mItemId;

					 if(lMsgGive->mReceiver == MASK_SERVER)
						aRecQueue->queueMessage(lMsgGive);
					else
						aSendQueue->queueMessage(lMsgGive);
				 }
			 }
			 delete lMsg;
		 }
		 // Un item ha sido recogido
		 else if(lMsg->mType == eMsgRemoveItem)
		 {
			 TMsgRemoveItem *lMsgItem = (TMsgRemoveItem*)lMsg;
			 if(dev_nivel()->dev_item(lMsgItem->mItemId)->es_cogible())
			 {
				 dev_nivel()->dev_item(lMsgItem->mItemId)->recoge();
			 }
			 delete lMsg;
		 }
		 // Me han concedido un item!
		 else if(lMsg->mType == eMsgConcealItem)
		 {
			 TMsgConcealItem *lMsgItem = (TMsgConcealItem*)lMsg;
			 if(lMsgItem->mReceiver == getMaskFromPlayerId(jug_local) && lMsgItem->mPlayerId == jug_local)
			 {
				byte t_so = 0;
				warrior *w = dev_player(jug_local);
				switch(niv->dev_item(lMsgItem->mItemId)->tipo())
				 {
                    case BOTI : if(w->salud()<w->salud_maxima()){
                                    t_so=2;
                                    w->nueva_salud((word)(w->salud()+(w->salud_maxima()*0.3)));
                                    if(w->salud()>w->salud_maxima()) w->nueva_salud(w->salud_maxima());
                                    encola_mensaje("First-aid kit +30%",w->color(1),prio);};
                                break;
                    case KEY1 : if(w->posee_llave(0)==FALSE){
                                    t_so=4;
                                    w->obtiene_llave(0);
                                    encola_mensaje("Iron key",w->color(1),prio);};
                                break;
                    case KEY2 : if(w->posee_llave(1)==FALSE){
                                    t_so=4;
                                    w->obtiene_llave(1);
                                    encola_mensaje("Bronze key",w->color(1),prio);};
                                break;
                    case KEY3 : if(w->posee_llave(2)==FALSE){
                                    t_so=4;
                                    w->obtiene_llave(2);
                                    encola_mensaje("Silver key",w->color(1),prio);};
                                break;
                    case KEY4 : if(w->posee_llave(3)==FALSE){
                                    t_so=4;
                                    w->obtiene_llave(3);
                                    encola_mensaje("Gold key",w->color(1),prio);};
                                break;
                    case PIST : if((w->clase()==SIGMA) && (!w->tiene_arma(1))){
                                    t_so=3;
                                    w->nueva_arma(1);
                                    encola_mensaje("Pistol",w->color(1),prio);};
                                break;
                    case AMET : if((w->clase()==SIGMA) && (!w->tiene_arma(2))){
                                    t_so=3;
                                    w->nueva_arma(2);
                                   encola_mensaje("Machine-gun",w->color(1),prio);};
                                break;
                    case LANZ : if((w->clase()==SIGMA) && (!w->tiene_arma(3))){
                                    t_so=3;
                                    w->nueva_arma(3);
                                    encola_mensaje("Rocket launcher",w->color(1),prio);};
                                break;
                    case ANIQ : if((w->clase()==SIGMA) && (!w->tiene_arma(4))){
                                    t_so=3;
                                    w->nueva_arma(4);
                                    encola_mensaje("\"Annihilator\"",w->color(1),prio);};
                                break;
                    case DRAG : if((w->clase()==SIGMA) && (!w->tiene_arma(5))){
                                    t_so=3;
                                    w->nueva_arma(5);
                                    encola_mensaje("\"Dragon\"",w->color(1),prio);};
                                break;
                    case INFE : if((w->clase()==SIGMA) && (!w->tiene_arma(6))){
                                    t_so=3;
                                    w->nueva_arma(6);
                                    encola_mensaje("\"Inferno\"",w->color(1),prio);};
                                break;
                    case SHOC : if((w->clase()==SIGMA) && (!w->tiene_arma(7))){
                                    t_so=3;
                                    w->nueva_arma(7);
                                    encola_mensaje("Electro-shock gun",w->color(1),prio);};
                                break;
                    case BALA : if((w->clase()==SIGMA) && (w->municion(0)<w->municion_maxima(0))){
                                    t_so=3;
                                    w->nueva_municion(0,w->municion(0)+30);
                                    if(w->municion(0)>w->municion_maxima(0)) w->nueva_municion(0,w->municion_maxima(0));
                                    encola_mensaje("Bullets +30",w->color(1),prio);};
                                break;
                    case COHE : if((w->clase()==SIGMA) && (w->municion(1)<w->municion_maxima(1))){
                                    t_so=3;
                                    w->nueva_municion(1,w->municion(1)+5);
                                    if(w->municion(1)>w->municion_maxima(1)) w->nueva_municion(1,w->municion_maxima(1));
                                    encola_mensaje("Rockets +5",w->color(1),prio);};
                                break;
                    case NAPA : if((w->clase()==SIGMA) && (w->municion(2)<300)){
                                    t_so=3;
                                    w->nueva_municion(2,w->municion(2)+15);
                                    if(w->municion(2)>w->municion_maxima(2)) w->nueva_municion(2,w->municion_maxima(2));
                                    encola_mensaje("Napalm +15",w->color(1),prio);};
                                break;
                    case DAGA : if((w->clase()==OMEGA) && (w->arma()<1)){
                                    w->nueva_arma(1); t_so=1;
                                    encola_mensaje("Dager",w->color(1),prio);};
                                break;
                    case EXCA : if((w->clase()==OMEGA) && (w->arma()<2)){
                                    w->nueva_arma(2); t_so=1;
                                    encola_mensaje("Saber",w->color(1),prio);};
                                break;
                    case FALC : if((w->clase()==OMEGA) && (w->arma()<3)){
                                    w->nueva_arma(3); t_so=1;
                                    encola_mensaje("Sword",w->color(1),prio);};
                                break;
                    case MAZA : if((w->clase()==OMEGA) && (w->arma()<4)){
                                    w->nueva_arma(4); t_so=1;
                                    encola_mensaje("Maul",w->color(1),prio);};
                                break;
                    case MART : if((w->clase()==OMEGA) && (w->arma()<5)){
                                    w->nueva_arma(5); t_so=1;
                                    encola_mensaje("Heavenly Hammer",w->color(1),prio);};
                                break;
                    case AGUI : if((w->clase()==OMEGA) && (w->arma()<6)){
                                    w->nueva_arma(6); t_so=1;
                                    encola_mensaje("\"Golden Eagle\"",w->color(1),prio);};
                                break;
                    case EDGE : if((w->clase()==OMEGA) && (w->arma()<7)){
                                    w->nueva_arma(7); t_so=1;
                                    encola_mensaje("\"Justice Edge\"",w->color(1),prio);};
                                break;
                    case EMAD : if((w->clase()==OMEGA) && (w->escudo()<1)){
                                    w->nuevo_escudo(1); t_so=1;
                                    encola_mensaje("Leather shield",w->color(1),prio);};
                                break;
                    case ECOB : if((w->clase()==OMEGA) && (w->escudo()<2)){
                                    w->nuevo_escudo(2); t_so=1;
                                    encola_mensaje("Wood shield",w->color(1),prio);};
                                break;
                    case EACE : if((w->clase()==OMEGA) && (w->escudo()<3)){
                                    w->nuevo_escudo(3); t_so=1;
                                    encola_mensaje("Steel shield",w->color(1),prio);};
                                break;
                    case EBRO : if((w->clase()==OMEGA) && (w->escudo()<4)){
                                    w->nuevo_escudo(4); t_so=1;
                                    encola_mensaje("Bronze shield",w->color(1),prio);};
                                break;
                    case EPLA : if((w->clase()==OMEGA) && (w->escudo()<5)){
                                    w->nuevo_escudo(5); t_so=1;
                                    encola_mensaje("Silver shield",w->color(1),prio);};
                                break;
                    case EORO : if((w->clase()==OMEGA) && (w->escudo()<6)){
                                    w->nuevo_escudo(6); t_so=1;
                                    encola_mensaje("Gold shield",w->color(1),prio);};
                                break;
                    case EDIV : if((w->clase()==OMEGA) && (w->escudo()<7)){
                                    w->nuevo_escudo(7); t_so=1;
                                    encola_mensaje("\"The Protector\"",w->color(1),prio);};
                                break;
                    case WALL : w->obtiene_special(INVUL,WTSPECIAL);
                                encola_mensaje("INVULNERABILITY",w->color(1),prio);
                                t_so=6;
                                break;
                    case MFUE : if((w->clase()==BETA) && (w->municion(0)<w->municion_maxima(0))){
                                    t_so=5;
                                    w->nueva_municion(0,w->municion(0)+20);
                                    if(w->municion(0)>w->municion_maxima(0)) w->nueva_municion(0,w->municion_maxima(0));
                                    encola_mensaje("Fire energy +20",w->color(1),prio);};
                                break;
                    case MHIE : if((w->clase()==BETA) && (w->municion(1)<w->municion_maxima(1))){
                                    t_so=5;
                                    w->nueva_municion(1,w->municion(1)+20);
                                    if(w->municion(1)>w->municion_maxima(1)) w->nueva_municion(1,w->municion_maxima(1));
                                    encola_mensaje("Water energy +20",w->color(1),prio);};
                                break;
                    case MELE : if((w->clase()==BETA) && (w->municion(2)<w->municion_maxima(2))){
                                    t_so=5;
                                    w->nueva_municion(2,w->municion(2)+20);
                                    if(w->municion(2)>w->municion_maxima(2)) w->nueva_municion(2,w->municion_maxima(2));
                                    encola_mensaje("Lightning energy +20",w->color(1),prio);};
                                break;
                    case APOC : if((w->clase()==BETA) && (!w->tiene_arma(10))){
                                    w->nueva_arma(10);
                                   encola_mensaje("Book of Apocalypse",w->color(1),prio);};
                                break;
                    case BERS : w->obtiene_special(BERSERK,WTSPECIAL);
                                encola_mensaje("MYSTICAL STRENGHT",w->color(1),prio);
                                t_so=6;
                                break;
                    case REPU : w->obtiene_special(FEAR,WTSPECIAL);
                                encola_mensaje("REPULSION",w->color(1),prio);
                                t_so=6;
                                break;
                    case INVI : w->obtiene_special(INVIS,WTSPECIAL);
                                encola_mensaje("INVISIBILITY",w->color(1),prio);
                                t_so=6;
                                break;
                    case SHAD : w->obtiene_special(SHADOW,WTSPECIAL);
                                encola_mensaje("FLUID OF SHADOWS",w->color(1),prio);
                                t_so=6;
                                break;
                    case HEAL : w->nueva_salud(w->salud_maxima()*2);
                                encola_mensaje("MAXIMUM HEALTH",w->color(1),prio);
                                t_so=6;
                                break;
                    case MAPA : desvela_mapa();
                                encola_mensaje("Full zone map",w->color(1),prio);
                                t_so=6;
                                break;
                    case VENE : w->danyar(15,DVENE,NULL);
                                encola_mensaje("Poison!",w->color(1),prio);
                                break;
                    case BOLT : bolt_red(w->fx_pos(),w->fy_pos(),12,w->bando(),50,OPPLAYER,jug_local,aRecQueue,aSendQueue);
                                t_so=7;
                                encola_mensaje("Electric Bolt!",w->color(1),prio);
                                break;
                    case DIOS : w->nueva_salud(w->salud_maxima()*2);
                                w->obtiene_special(INVUL,WTSPECIAL);
                                w->obtiene_special(FEAR,WTSPECIAL);
                                w->obtiene_special(BERSERK,WTSPECIAL);
                                encola_mensaje("GOD'S PROTECTION",w->color(1),prio);
                                t_so=6;
                                break;
                    case GEM1 : if(gema==FALSE){
                                    t_so=4;
                                    gema=TRUE;
                                    encola_mensaje("Gem of Fire",w->color(1),prio);};
                                break;
                    case GEM2 : if(gema==FALSE){
                                    t_so=4;
                                    gema=TRUE;
                                    encola_mensaje("Gem of Earth",w->color(1),prio);};
                                break;
                    case GEM3 : if(gema==FALSE){
                                    t_so=4;
                                    gema=TRUE;
                                    encola_mensaje("Gem of Water",w->color(1),prio);};
                                break;
                };

                switch(t_so){
                    case 1 : sonido_ambiente(S_ITES,w->fx_pos(),w->fy_pos()); break;
                    case 2 : sonido_ambiente(S_ITBO,w->fx_pos(),w->fy_pos()); break;
                    case 3 : sonido_ambiente(S_ITMU,w->fx_pos(),w->fy_pos()); break;
                    case 4 : sonido_ambiente(S_ITLL,w->fx_pos(),w->fy_pos()); break;
                    case 5 : sonido_ambiente(S_ITEN,w->fx_pos(),w->fy_pos()); break;
                    case 6 : sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos()); break;
                    case 7 : sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos()); break;
                };
			 }

			 delete lMsg;
		 }
		 // Petición de dañar a un personaje
		 else if(lMsg->mType == eMsgDamage)
		 {
			 TMsgDamage *lMsgDmg = (TMsgDamage*)lMsg;

			 if(lMsgDmg->mTargetType == OPPLAYER)
			 {
				 if(lMsgDmg->mTargetId == jug_local)
				 {
					 warrior *source = 0;
					 if (lMsgDmg->mSourceType == OPPLAYER)
						 source = dev_player(lMsgDmg->mSourceId);

                     if (lMsgDmg->mSourceType == OPENEMY)
                         source = dev_enem(lMsgDmg->mSourceId);

					 // Dañar al jugador local!
					 warrior *pl = dev_player(jug_local);
                     if((pl->estado()!=DEST) && (pl->estado()!=DIE) && (pl->estado()!=OUTL) && (pl->estado()!=TELP) && (pl->dios()==FALSE))
                     {
                        if((pl->estado()!=DEFE) && ((pl->contador()==0) || (pl->estado()!=WALK)) && (!pl->special(INVUL)))
                        {
                            pl->danyar(lMsgDmg->mAmount,(cdanyo)lMsgDmg->mDamType,source);
                            if(pl->estado()!=CONG)
                            {
                                if(lMsgDmg->mDamType == DHIEL)
                                    pl->nuevo_estado(CONG);
                                if(lMsgDmg->mDamType == DELEC)
                                    pl->electrocutar();
                            }
                        }
                    }

					 delete lMsg;
				 }
				 else if(modo_red == MRSERVER)
				 {
					 // Reenvia al destinatario
					 lMsgDmg->mSender	= MASK_SERVER;
					 lMsgDmg->mReceiver = getMaskFromPlayerId(lMsgDmg->mTargetId);
					 aSendQueue->queueMessage(lMsgDmg);
				 }
			 }
             else if (lMsgDmg->mTargetType == OPENEMY)
             {
                 if (modo_red == MRSERVER)
                 {
                     warrior* source = 0;
                     if (lMsgDmg->mSourceType == OPPLAYER)
                         source = dev_player(lMsgDmg->mSourceId);

                     if (lMsgDmg->mSourceType == OPENEMY)
                         source = dev_enem(lMsgDmg->mSourceId);

                     // Dañar al enemigo (que es local al server)!
                     warrior* pl = dev_enem(lMsgDmg->mTargetId);
                     if ((pl->estado() != DEST) && (pl->estado() != DIE) && (pl->estado() != OUTL) && (pl->estado() != TELP) && (pl->dios() == FALSE))
                     {
                         if ((pl->estado() != DEFE) && ((pl->contador() == 0) || (pl->estado() != WALK)) && (!pl->special(INVUL)))
                         {
                             pl->danyar(lMsgDmg->mAmount, (cdanyo)lMsgDmg->mDamType, source);
                             if (pl->estado() != CONG)
                             {
                                 if (lMsgDmg->mDamType == DHIEL)
                                     pl->nuevo_estado(CONG);
                                 if (lMsgDmg->mDamType == DELEC)
                                     pl->electrocutar();
                             }
                         }
                     }

                     delete lMsg;
                 }

             }
			 else if(lMsgDmg->mTargetType == OPBARR)
			 {
				 // Daña la barrera local
				 dev_mapa()->dev_barr(lMsgDmg->mTargetId)->danyar(lMsgDmg->mAmount);

				 if(modo_red == MRSERVER)
				 {
					 // Reenvia porque soy server
					 lMsgDmg->mSender	= MASK_SERVER;
					 lMsgDmg->mReceiver = MASK_ALL_CLIENTS;
					 aSendQueue->queueMessage(lMsgDmg);
				 }
				 else
					 delete lMsg;

			 }
			 else
			 {
				 delete lMsg;
			 }
		 }
		 // Creación de una bala
		 else if(lMsg->mType == eMsgNewBullet)
		 {
			 TMsgNewBullet *lMsgBull = (TMsgNewBullet*)lMsg;

			 warrior *w = 0;
			 if(lMsgBull->mCreatorType == OPPLAYER)
				 w = dev_player(lMsgBull->mCreatorId);
             if (lMsgBull->mCreatorType == OPENEMY)
                 w = dev_enem(lMsgBull->mCreatorId);
			 ingresa_bala(lMsgBull->mX, lMsgBull->mY, (w_dir)lMsgBull->mDir, lMsgBull->mVel, lMsgBull->mAcc, (b_tipo)lMsgBull->mBulType, lMsgBull->mDamage, w );

			if(w)
				sonido_ambiente((s_sample)lMsgBull->mSample, w->fx_pos(), w->fy_pos());
			else
				sonido_ambiente((s_sample)lMsgBull->mSample, lMsgBull->mX, lMsgBull->mY);

			 // Si soy server, reenvia
			 if(modo_red == MRSERVER)
			 {
				 lMsgBull->mSender = MASK_SERVER;
				 lMsgBull->mReceiver = MASK_ALL_CLIENTS;
				 aSendQueue->queueMessage(lMsgBull);
			 }
			 else
				 delete lMsg;
		 }
		 // Destruccion de una bala
		 else if(lMsg->mType == eMsgEndBullet)
		 {
			 TMsgEndBullet *lMsgBull = (TMsgEndBullet*)lMsg;

			 retira_bala(lMsgBull->mId);
		 }
		 // Crea una explosion
		 else if(lMsg->mType == eMsgNewExplos)
		 {
			 TMsgNewExplos *lMsgExp = (TMsgNewExplos*)lMsg;

			 warrior *w = 0;
			 if(lMsgExp->mCreatorType == OPPLAYER)
				 w = dev_player(lMsgExp->mCreatorId);
             if (lMsgExp->mCreatorType == OPENEMY)
                 w = dev_enem(lMsgExp->mCreatorId);

			 ingresa_explos(lMsgExp->mX, lMsgExp->mY, (e_tipo)lMsgExp->mExpType, lMsgExp->mDamage, w);

			 // Si soy server, reenvia
			 if(modo_red == MRSERVER)
			 {
				 lMsgExp->mSender = MASK_SERVER;
				 lMsgExp->mReceiver = MASK_ALL_CLIENTS;

				 aSendQueue->queueMessage(lMsgExp);
			 }
			 else
				 delete lMsg;
		 }
		 // Crea una barrera
		 else if(lMsg->mType == eMsgNewBarrier)
		 {
			  TMsgNewBarrier *lMsgBarr = (TMsgNewBarrier*)lMsg;

			  dev_mapa()->anyade_barrera((bald)lMsgBarr->mX,(bald)lMsgBarr->mY,lMsgBarr->mType);

			   // Si soy server, reenvia
			 if(modo_red == MRSERVER)
			 {
				 lMsgBarr->mSender = MASK_SERVER;
				 lMsgBarr->mReceiver = MASK_ALL_CLIENTS;

				 aSendQueue->queueMessage(lMsgBarr);
			 }
			 else
				 delete lMsg;

		 }
         // Crea un item
         else if (lMsg->mType == eMsgNewItem)
         {
            TMsgNewItem* lMsgItem = (TMsgNewItem*)lMsg;

            niv->ingresa_item(lMsgItem->mX, lMsgItem->mY, (t_item)(lMsgItem->mItemType));

            // Si soy server, reenvia
            if (modo_red == MRSERVER)
            {
                lMsgItem->mSender = MASK_SERVER;
                lMsgItem->mReceiver = MASK_ALL_CLIENTS;
                aSendQueue->queueMessage(lMsgItem);
            }
            else
                delete lMsg;
         }
		 else if(lMsg->mType == eMsgTurnSwitch)
		 {
			 TMsgTurnSwitch *lMsgSwitch = (TMsgTurnSwitch*)lMsg;

			 interr *in;
			 if(in = dev_mapa()->dev_interr(lMsgSwitch->mSwitchId))
			 {
				dev_mapa()->activa_interr(lMsgSwitch->mSwitchId);
				sonido_ambiente(S_INTER,in->x_pos(),in->y_pos());

				// Si soy server, reenvia
				if(modo_red == MRSERVER)
				{
					lMsgSwitch->mSender = MASK_SERVER;
					lMsgSwitch->mReceiver = MASK_ALL_CLIENTS;

					aSendQueue->queueMessage(lMsgSwitch);
				}

				// Interruptor de disparador
				if (modo_red == MRSERVER)
				if(dev_mapa()->dev_interr(lMsgSwitch->mSwitchId)->tobj() == DISP)
				{
					disparador *dis=dev_mapa()->dev_disp(dev_mapa()->hay_disp(in->cobjx(),in->cobjy()));

					crea_bala_red(dis->x_pos(), dis->y_pos(), dis->dir(), 0.2, 0, (b_tipo)dis->t_bala(), 5, OPTRAP, lMsgSwitch->mSwitchId, S_TRAM, aRecQueue, aSendQueue);
				}

				if (modo_red != MRSERVER)
					delete lMsg;
                                              //// Disparadores
                                              //if(mp->dev_interr(i)->tobj()==DISP){
                                              //     dis=mp->dev_disp(mp->hay_disp(mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy()));
                                              //     ingresa_bala(dis->x_pos(),dis->y_pos(),dis->dir(),0.2,0,(b_tipo)dis->t_bala(),5,NULL);
                                              //     sonido_ambiente(S_TRAM,mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy());};
                                              //sonido_ambiente(S_INTER,mp->dev_interr(i)->x_pos(),mp->dev_interr(i)->y_pos());
			 }
		 }
         else if (lMsg->mType == eMsgTalkToChar)
         {
            TMsgTalkToChar* lMsgTalk = (TMsgTalkToChar*)lMsg;

            warrior* pl = dev_player(lMsgTalk->mPlayerId);
            warrior* sec = dev_enem(lMsgTalk->mEnemyId);
            char s[200];

            // Suelta un ¡tem
            t_item it=sec->item_asociado();
            
            // Se gira hacia el jugador y pronuncia su frase
            sec->nuevo_oponente(pl);
            sprintf(s,"%s:%s",sec->nombre(),sec->frase());
            if((sec->interactuar()==TRUE) && (sec->estado()<TELP)){
                if(sec->dios()==TRUE) encola_mensaje_red(s,64,allprio, aRecQueue, aSendQueue);
                else          encola_mensaje_red(s,15,allprio, aRecQueue, aSendQueue);
                if(it!=NING)
                ingresa_item_util_red(sec->front_x(0.75),sec->front_y(0.75),it, aRecQueue, aSendQueue);
            };

            delete lMsg;
         }
		 else if(lMsg->mType == eMsgIncreaseKills)
		 {
			  TMsgIncreaseKills *lMsgKill = (TMsgIncreaseKills*)lMsg;

			  dev_player(lMsgKill->mPlayerId)->incr_muertes();

			  // Si soy server, reenvia
			  if(modo_red == MRSERVER)
			  {
				lMsgKill->mSender = MASK_SERVER;
				lMsgKill->mReceiver = MASK_ALL_CLIENTS;

				aSendQueue->queueMessage(lMsgKill);
			  }
			  else
				  delete lMsg;
		 }
		 else if(lMsg->mType == eMsgEndMatch)
		 {
			 if(modo_red == MRCLIENTE)
				mens_fin_recibido = true;
		 }
		 else if(lMsg->mType == eMsgPlDisconnect)
		 {
			  TMsgPlDisconnect *lMsgDisc = (TMsgPlDisconnect*)lMsg;

			  if(dev_player(lMsgDisc->mPlayerId)->activo())
			  {
                  dev_player(lMsgDisc->mPlayerId)->set_activo(false); //nuevo_estado(DIE);

                  // Si soy server, reenvia
                  if(modo_red == MRSERVER)
                  {
                        char s[60];
                        sprintf(s,"%s disconnected",dev_player(lMsgDisc->mPlayerId)->nombre());
                        encola_mensaje_red(s,15,0xff,aRecQueue,aSendQueue);

                        lMsgDisc->mSender = MASK_SERVER;
                        lMsgDisc->mReceiver = MASK_ALL_CLIENTS;

                        aSendQueue->queueMessage(lMsgDisc);
                  }
                  else
                    delete lMsg;
                }
                else
                    delete lMsg;
		 }
		 else
		 {
			// Otros mensajes: simplemente descartar
			delete lMsg;
		 }

	 }

     // Operar con la consola de s.o.
     if (con_op_cont > 0) con_op_cont--;
     if ((cont.tecla_consola()) && (con_op_cont == 0) && (modo_datos == FALSE)) {
         con_op_cont = MPTWAIT;
         //                clear_keybuf(); prncar=0;
         controlm::flushallkeys();
         if (modo_consola) modo_consola = FALSE;
         else           modo_consola = TRUE;
     };
	  // Actualiza el mapa
      dev_mapa()->actualiza();
      // Operar con el mapa 2D
      if (map_op_cont > 0) map_op_cont--;
      if ((cont.tecla_mapa()) && (map_op_cont == 0) && (tipoj == COOP)) {
          map_op_cont = MPTWAIT;
          if (modo_mapa) modo_mapa = FALSE;
          else           modo_mapa = TRUE;
      };
      // Activa datos players en pantalla
      if (dat_op_cont > 0) dat_op_cont--;
      if ((cont.tecla_datos()) && (dat_op_cont == 0) && (tipoj == COOP) && (modo_consola == FALSE)) {
          dat_op_cont = MPTWAIT;
          if (modo_datos) modo_datos = FALSE;
          else           modo_datos = TRUE;
      };
      if ((cont.tecla_mas()) && (map_op_cont == 0)) {
          map_op_cont = MPTWAIT;
          if (mzoom < 20) mzoom++;
      };
      if ((cont.tecla_menos()) && (map_op_cont == 0)) {
          map_op_cont = MPTWAIT;
          if (mzoom > 1) mzoom--;
      };
     
	 // Mueve los jugadores
      for(i=0; i<nplayers; i++){
			if(i == jug_local)
			{
				cont.actualiza(contr[i]);
                if(player[i]->activo()) actualiza_player_red_local(i,player[i],&cont,TRUE,aRecQueue,aSendQueue);

				if(tcont % 10 == 0 || (player[i]->estado() == WALK && player[i]->contador() >= WDOLWAIT - 3))
				{
					TMsgPlUpdate *lMsgUpdate = DBG_NEW TMsgPlUpdate;

					lMsgUpdate->mType = eMsgPlUpdate;
					lMsgUpdate->mSender = getMaskFromPlayerId(jug_local);
					lMsgUpdate->mReceiver = modo_red == MRSERVER ? MASK_ALL_CLIENTS : MASK_SERVER;
                    lMsgUpdate->mCharacterType = 0;
					lMsgUpdate->mPlayerId = jug_local;
					lMsgUpdate->mDir = player[i]->dir();
					lMsgUpdate->mState = player[i]->estado();
					lMsgUpdate->mCounter = player[i]->contador();
					lMsgUpdate->mWeapon = player[i]->arma();
					lMsgUpdate->mShield = player[i]->escudo();
					lMsgUpdate->mHealth = player[i]->salud();
					for(int j = 0 ; j < 5; j++)
						lMsgUpdate->mSpecials[j] = player[i]->special((w_special)j) ? 1 : 0;
					lMsgUpdate->mElec = player[i]->electrocutado() ? 1 : 0;
					lMsgUpdate->mX = player[i]->fx_pos();
					lMsgUpdate->mY = player[i]->fy_pos();

					aSendQueue->queueMessage(lMsgUpdate);

				}
			}
			else
				if(player[i]->activo()) actualiza_player_red_remoto(i,player[i],&cont,TRUE);

            matr_mapa[player[i]->x_pos() / 5][player[i]->y_pos() / 5] = TRUE;
            matr_mapa[(player[i]->x_pos() / 5) - 1][(player[i]->y_pos() / 5)] = TRUE;
            matr_mapa[(player[i]->x_pos() / 5) + 1][(player[i]->y_pos() / 5)] = TRUE;
            matr_mapa[(player[i]->x_pos() / 5)][(player[i]->y_pos() / 5) - 1] = TRUE;
            matr_mapa[(player[i]->x_pos() / 5)][(player[i]->y_pos() / 5) + 1] = TRUE;
	  }

      // Mueve los enemigos / secundarios
      if (modo_red == MRSERVER)
      {
          i = 0;
          bool si;
          float x, y;
          while (dev_enem(i) != NULL) {
              si = TRUE;
              x = dev_enem(i)->x_pos();
              y = dev_enem(i)->y_pos();
              si = FALSE;
              for (int j = 0; j < nplayers; j++)
                  if (dist_menor(player[j]->x_pos(), player[j]->y_pos(), x, y, ENEMY_UPDATE_DIST))
                      si = TRUE;
              if (si) {
                  matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]--;
                  cont.reset();
                  actualiza_ia_control(dev_enem(i), &cont, i, 0/*dif*/, aRecQueue, aSendQueue);
                  actualiza_player_red_local(8, dev_enem(i), &cont, FALSE, aRecQueue, aSendQueue);
                  matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]++;

                  // Envia mensaje de actualización con cierta frecuencia
                  if ((tcont + i) % 10 == 0 || (dev_enem(i)->estado() == WALK && dev_enem(i)->contador() >= WDOLWAIT - 3))
                  {
                      TMsgPlUpdate* lMsgUpdate = DBG_NEW TMsgPlUpdate;

                      lMsgUpdate->mType = eMsgPlUpdate;                      
                      lMsgUpdate->mSender = MASK_SERVER;
                      lMsgUpdate->mReceiver = MASK_ALL_CLIENTS;
                      lMsgUpdate->mCharacterType = 1;
                      lMsgUpdate->mPlayerId = i;
                      lMsgUpdate->mDir = dev_enem(i)->dir();
                      lMsgUpdate->mState = dev_enem(i)->estado();
                      lMsgUpdate->mCounter = dev_enem(i)->contador();
                      lMsgUpdate->mWeapon = dev_enem(i)->arma();
                      lMsgUpdate->mShield = dev_enem(i)->escudo();
                      lMsgUpdate->mHealth = dev_enem(i)->salud();
                      for (int j = 0; j < 5; j++)
                          lMsgUpdate->mSpecials[j] = dev_enem(i)->special((w_special)j) ? 1 : 0;
                      lMsgUpdate->mElec = dev_enem(i)->electrocutado() ? 1 : 0;
                      lMsgUpdate->mX = dev_enem(i)->fx_pos();
                      lMsgUpdate->mY = dev_enem(i)->fy_pos();

                      aSendQueue->queueMessage(lMsgUpdate);

                  }
              };
              i++;
          };
      }
      if (modo_red == MRCLIENTE)
      {
          i = 0;
          while (dev_enem(i) != NULL) {     
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]--;
                actualiza_player_red_remoto(i, dev_enem(i), &cont, FALSE);              
                matr_enem[dev_enem(i)->y_pos()][dev_enem(i)->x_pos()]++;
              i++;
          };
      }

	  // Evita solapamientos!!
	  deshacer_solapamientos_players();

	  // Actualiza los items
	  i=0;
      if(tipoj == DMCH)
      while(niv->dev_item(i)!=NULL){
              niv->dev_item(i)->actualiza(); i++;};
      // Mueve las balas
      for(i=nbalas-1; i>=0; i--)
              actualiza_bala(i, aRecQueue, aSendQueue);
	  // Maneja explosiones
      i=0;
      while(bexplos[i]!=NULL){
              bexplos[i]->actualiza();
              if(bexplos[i]->acabada()) {retira_explos(i); i--;};
              i++;
              };
	  // Da¤a y elimina barreras
	  if(modo_red == MRSERVER)
	  {
		  i=0;
		  barrera *ba;
		  while((ba=dev_mapa()->dev_barr(i))!=NULL){
				  if((k=dist_explos((float)ba->posx()+0.5,(float)ba->posy()+0.5,1.0))>=0)
						 envia_danyo_red(0,DPUNY,danyo_total_explos((float)ba->posx()+0.5,(float)ba->posy()+0.5,1.0)/3,OPBARR,i,aRecQueue,aSendQueue);
						 //ba->danyar(danyo_total_explos((float)ba->posx()+0.5,(float)ba->posy()+0.5,1.0)/3);
				  i++;
		  };
	  }
	   // Disparadores
	  if(modo_red == MRSERVER)
	  {
		  i=0;
		  disparador *di;
		  while((di=dev_mapa()->dev_disp(i))!=NULL){

				  if(di->disparando()){

					if(di->t_bala()==(byte)FLAM){
					   if((di->dir()==DABA) || (di->dir()==DARR))
					   //ingresa_explos(di->x_pos()+0.5,di->y_pos()+0.5,ELL3,di->fuerza(),NULL);
					   crea_explos_red(di->x_pos()+0.5f,di->y_pos()+0.5,ELL3,di->fuerza(),0,0,aRecQueue,aSendQueue);
					   else
					   //ingresa_explos(di->x_pos()+0.5,di->y_pos()+0.5,ELL2,di->fuerza(),NULL);
					   crea_explos_red(di->x_pos()+0.5f,di->y_pos()+0.5,ELL2,di->fuerza(),0,0,aRecQueue,aSendQueue);
					   sonido_ambiente(S_TRAM,di->x_pos(),di->y_pos());
					}else{
							if(di->disparando())
									//ingresa_bala(di->x_pos()+0.5,di->y_pos()+0.5,di->dir(),0.2,0,(b_tipo)di->t_bala(),di->fuerza(),NULL);
									crea_bala_red(di->x_pos()+0.5f,di->y_pos()+0.5,di->dir(),0.2,0,(b_tipo)di->t_bala(),di->fuerza(),0,0,S_TRAM,aRecQueue,aSendQueue);
					};
				  };
				  i++;
		  };
	  }

	   // Subsistema de mensajes
      for(i=0; i<nmens; i++){
              bmenstime[i]--;
              if(bmenstime[i]==0) {desencola_mensaje(i); i--;};
      };

	  // Sonidos
      i=0;
	  puerta *pu;
      while((pu=dev_mapa()->dev_puerta(i))!=NULL){

                if(pu->abertura()==1){
                        if(pu->abriendo())
                              sonido_ambiente(S_PUE1,pu->inx(),pu->iny());
                        else
                              sonido_ambiente(S_PUE2,pu->inx(),pu->iny());
                };
                if(pu->abertura()==PMAXABERT-1){
                        if(pu->abriendo())
                              sonido_ambiente(S_PUE2,pu->inx(),pu->iny());
                        else
                              sonido_ambiente(S_PUE1,pu->inx(),pu->iny());
                };
              i++;
      };
      // Control del crepitar del fuego
      int x=1000;
	  int y;
	  explos *exp_cerc=0;
      for(i=0; i<nplayers; i++)
            for(int j=0; j<nexplos; j++)
                     if(bexplos[j]->tipo()>=ELLA){
                                y=dist(player[i]->fx_pos(),player[i]->fy_pos(),bexplos[j]->posx(),bexplos[j]->posy());
                                if(y<x) {x=y; exp_cerc=bexplos[j];};// cuadrado de la distancia
                     };

      if((x<16*16) && (!bwav->wav_llama_activa()))
                bwav->activa_wav_llama();
      if((x>16*16) && (bwav->wav_llama_activa()))
                bwav->apaga_wav_llama();
      if(bwav->wav_llama_activa())
      bwav->actualiza_wav_llama(diferencia_x(640,400,exp_cerc->posx(),exp_cerc->posy()),
                                dist_centro(exp_cerc->posx(),exp_cerc->posy()),TRUE);

      // Control de las fuentes de sonido ambiente
	  bald xx, yy;
	  int s, cx, cy;
      for(k=0; k<N_SMP_AMBI; k++){

        x=1000;
        for(i=0; i<nplayers; i++){
            int j=0;
            while((s=dev_mapa()->dev_sonido_amb(j,xx,yy))>=0){
                     if(s==(int)k){
                                y=dist(player[i]->fx_pos(),player[i]->fy_pos(),xx,yy);
                                if(y<x) {x=y; cx=xx; cy=yy;};// cuadrado de la distancia
                     };
                     j++;
            };
        };
        if((x<16*16) && (!bwav->wav_ambient_activo(k))) bwav->activa_wav_ambient(k);
        if((x>16*16) && (bwav->wav_ambient_activo(k))) bwav->apaga_wav_ambient(k);
        if(bwav->wav_ambient_activo(k))
                bwav->actualiza_wav_ambient(k,diferencia_x(640,400,cx,cy),dist_centro(cx,cy),TRUE);
      };

	  if(modo_consola) actualiza_consola();

	  if(ventana_chat)
	  {
		  if(controlm::keypressed())
		  {

			char ch;
			ALLEGRO_EVENT key;

			if(controlm::readkey(&key))
			{
				if(key.keyboard.type == ALLEGRO_EVENT_KEY_CHAR)
				{
					switch(key.keyboard.keycode)
					{
						case ALLEGRO_KEY_BACKSPACE	: if(chat_msg_length>0) chat_msg_length--; chat_msg[chat_msg_length]='\0'; break;

						default						: if(chat_msg_length<MSGSIZE-10){
														ch = key.keyboard.unichar;
														chat_msg[chat_msg_length]=ch; chat_msg[chat_msg_length+1]='\0';
														chat_msg_length++;}; break;
						case ALLEGRO_KEY_ENTER		: {
														char fullmsg[MSGSIZE];
														if(chat_msg_length > 0)
														{
															sprintf(fullmsg,"%s:%s",dev_player(jug_local)->nombre(),chat_msg);
															encola_mensaje_red(fullmsg, 15, 0xff, aRecQueue, aSendQueue);
														}
														ventana_chat = false;
														chat_espera = 10;
														chat_msg_length=0; chat_msg[0]='\0';
													  }
													  break;
					}
				}
			}
		  }
	  }
	  else
	  {
		   if(cont.tecla_chat() && chat_espera == 0){
			   controlm::flushallkeys();
			   ventana_chat = true;
		   }
	  }
	  if(chat_espera > 0 ) chat_espera--;
}

void batalla::actualiza_consola()
{
        int tec;
        char k,scode,s[70];
        if(last_error!=NO_ERR){
                sprintf(s,"Protection error in module %s:",last_error_module);
                encola_mensaje_consola(s,32);
                encola_mensaje_consola(last_error_message,32);
                last_error=NO_ERR;
        }
       /* if((keypressed()) && (con_input)){

                tec=readkey(); scode=tec>>8; k=(char)tec;
                switch(scode){
                        case KEY_BACKSPACE : if(prncar>0) prncar--; cprompt[prncar]='\0'; break;
                        case KEY_ENTER     : comando_consola(cprompt); sprintf(lastprompt,"%s",cprompt);
                                             prncar=0; cprompt[0]='\0'; break;
                        case KEY_UP        : sprintf(cprompt,"%s",lastprompt); prncar=strlen(lastprompt); break;
                        default : if(prncar<MSGSIZE-1){
                                        cprompt[prncar]=k; cprompt[prncar+1]='\0';
                                        prncar++;}; break;
                };
        };*/
		if(controlm::keypressed() && (con_input)){

			ALLEGRO_EVENT key;

			if(controlm::readkey(&key))
			{
				if(key.keyboard.type == ALLEGRO_EVENT_KEY_CHAR)
				{
					switch(key.keyboard.keycode)
					{
						case ALLEGRO_KEY_BACKSPACE	: if(prncar>0) prncar--; cprompt[prncar]='\0'; break;
						case ALLEGRO_KEY_ENTER		: comando_consola(cprompt); sprintf(lastprompt,"%s",cprompt);
													  prncar=0; cprompt[0]='\0'; break;
						case ALLEGRO_KEY_UP			: sprintf(cprompt,"%s",lastprompt); prncar=strlen(lastprompt); break;
						default						: if(prncar<MSGSIZE-1){
														k = key.keyboard.unichar;
														cprompt[prncar]=k; cprompt[prncar+1]='\0';
														prncar++;}; break;
					}
				}
			}
		}

}
void batalla::comando_consola(char *line)
{
        char args[10][20], s[60];
        int in[10];

        char COM[]="Unknown command",
             NOE[]="Required object do not exists",
             RAN[]="Out of range",
             NIN[]="Information not available",
             PAR[]="The command requires more parametters",
        sitarget[5][8]={"bridge","door","dark","shooter","laser"};

        int i=0;
        mapa *mp=dev_mapa();
        warrior *w=0; puerta *p; disparador *d; telep *t; barrera *b; puente *l;
        interr *ii; area *a; item *it;

        for(i=0; i<10; i++){
                 argumento(line,i,args[i]);
                 in[i]=atoi(args[i]);};

        encola_mensaje_consola(line,64);

        // Cierra consola
        if(strcmp(args[0],"close")==0) modo_consola=FALSE;
        // Vacia pantalla
        else if(strcmp(args[0],"clear")==0) ncmens=0;
        // Mensaje
        else if(strcmp(args[0],"say")==0) encola_mensaje(args[1],192,PRIO1|PRIO2|PRIO3|PRIO4);
        // Version
        else if(strcmp(args[0],"info")==0) encola_mensaje_consola(NOMBRE_MODULO,192);

        else if(strcmp(args[0],"give_map")==0) desvela_mapa();

		else if(strcmp(args[0],"show_fps")==0) show_fps = !show_fps;

        else if(strcmp(args[0],"enemy_map")==0)
            if (enemy_map==FALSE) enemy_map=TRUE; else enemy_map=FALSE;

        else if((strcmp(args[0],"player")==0) || (strcmp(args[0],"enemy")==0)){

                if(strcmp(args[0],"player")==0)
                        w=dev_player(in[1]);
                if(strcmp(args[0],"enemy")==0)
                        w=dev_enem(in[1]);
                if(w!=NULL){
                        if(strcmp(args[2],"kill")==0)
                                w->nuevo_estado(DIE);
                        else if(strcmp(args[2],"freeze")==0)
                                w->nuevo_estado(CONG);
                        else if(strcmp(args[2],"destroy")==0)
                                w->nuevo_estado(DEST);
                        else if(strcmp(args[2],"give_level")==0)
                                w->sube_level();
                        else if(strcmp(args[2],"give_special")==0){
                                if(in[3]<WNSPECIALS)
                                        w->obtiene_special((w_special)in[3],WTSPECIAL);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"give_ammo")==0){
                                if(in[3]<3)
                                        w->nueva_municion(in[3],w->municion_maxima(in[3]));
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"give_shield")==0){
                                if(in[3]<7)
                                        w->nuevo_escudo(in[3]+1);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"give_weapon")==0){
                                if(in[3]<10)
                                        w->nueva_arma(in[3]+1);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"give_key")==0){
                                if(in[3]<4)
                                        w->obtiene_llave(in[3]);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"give_heal")==0){
                                if(in[3]<999)
                                        w->nueva_salud(in[3]);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"set_state")==0){
                                if((w_est)in[3]<=OUTL)
                                        w->nuevo_estado((w_est)in[3]);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"set_pos")==0){
                                if((in[3]<MSIZEX) && (in[4]<MSIZEY))
                                        w->nueva_pos(in[3],in[4]);
                                else encola_mensaje_consola(RAN,192);
                        }else if(strcmp(args[2],"delete")==0){
                                if(strcmp(args[0],"player")==0)
                                encola_mensaje_consola("Players cannot be deleted!",192);
                                else niv->retira_enem(in[1]);
                        }else {
                                sprintf(s,"%s / Level: %d / Experience: %d",w->nombre(),w->level(),w->exp());
                                encola_mensaje_consola(s,192);
                                sprintf(s,"Health: %d / Group: %d / Position: (%d,%d)",w->salud(),w->bando(),w->x_pos(),w->y_pos());
                                encola_mensaje_consola(s,192);
                                sprintf(s,"Weapon: %d / Shield: %d / State: %d",w->arma(),w->escudo(),w->estado());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);

        }else if(strcmp(args[0],"door")==0){
                p=dev_mapa()->dev_puerta(in[1]);
                if(p!=NULL){
                        if(strcmp(args[2],"open")==0)
                                p->abre();
                        else if(strcmp(args[2],"activate")==0)
                                p->activa();
                        else if(strcmp(args[2],"delete")==0)
                                mp->retira_puerta(in[1]);
                        else{
                                sprintf(s,"Door from (%d,%d) to (%d,%d) / Type:%d",p->inx(),p->iny(),p->finx(),p->finy(),p->tipo());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"shooter")==0){
                d=dev_mapa()->dev_disp(in[1]);
                if(d!=NULL){
                        if(strcmp(args[2],"delete")==0)
                                mp->retira_disp(in[1]);
                        else{
                                sprintf(s,"Shooter at (%d,%d) / Type of bullet:%d",d->x_pos(),d->y_pos(),d->t_bala());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"teleport")==0){
                t=dev_mapa()->dev_telep(in[1]);
                if(t!=NULL){
                        if(strcmp(args[2],"delete")==0)
                                mp->retira_telep(in[1]);
                        else{
                                sprintf(s,"Teleport from (%d,%d) to (%d,%d)",t->x_in(),t->y_in(),t->x_out(),t->y_out());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"barrier")==0){
                b=dev_mapa()->dev_barr(in[1]);
                if(b!=NULL){
                        if(strcmp(args[2],"damage")==0)
                                b->danyar(in[3]);
                        else if(strcmp(args[2],"delete")==0)
                                mp->retira_barrera(in[1]);
                        else{
                                sprintf(s,"Barrier at (%d,%d) / Damage:%d",b->posx(),b->posy(),b->danyo());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if((strcmp(args[0],"laser")==0) || (strcmp(args[0],"bridge")==0)){
                if(strcmp(args[0],"laser")==0)
                        l=dev_mapa()->dev_laser(in[1]);
                else
                        l=dev_mapa()->dev_puente(in[1]);
                if(l!=NULL){
                        if(strcmp(args[2],"activate")==0)
                                l->activa();
                        else if(strcmp(args[2],"delete")==0){
                                if(strcmp(args[0],"laser")==0) mp->retira_laser(in[1]);
                                else                           mp->retira_puente(in[1]);
                        }else{
                                sprintf(s,NIN);
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"switch")==0){
                ii=dev_mapa()->dev_interr(in[1]);
                if(ii!=NULL){
                        if(strcmp(args[2],"activate")==0)
                               mp->activa_interr(in[1]);
                        else if(strcmp(args[2],"delete")==0)
                                mp->retira_interr(in[1]);
                        else{
                                sprintf(s,"Switch at (%d,%d) activates %s at (%d,%d)",ii->x_pos(),ii->y_pos(),sitarget[ii->tobj()],ii->cobjx(),ii->cobjy());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"item")==0){
                it=niv->dev_item(in[1]);
                if(it!=NULL){
                        if(strcmp(args[2],"delete")==0)
                                niv->retira_item(in[1]);
                        else{
                                sprintf(s,"Item at (%d,%d) / Type:%d",it->x_pos(),it->y_pos(),it->tipo());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if(strcmp(args[0],"dark_zone")==0){
                        a=dev_mapa()->dev_oscu(in[1]);
                if(a!=NULL){
                        if(strcmp(args[2],"delete")==0)
                                mp->retira_oscu(in[1]);
                        else{
                                sprintf(s,"Dark zone from (%d,%d) to (%d,%d)",a->inx(),a->iny(),a->finx(),a->finy());
                                encola_mensaje_consola(s,192);
                        };
                }else encola_mensaje_consola(NOE,192);
        }else if (strcmp(args[0],"t_matrix")==0){
              if (strcmp(args[3],"set")==0)
                       mp->insb(in[1],in[2],in[4]);
              else{
                        sprintf(s,"Ground matrix at (%d,%d) : %d",in[1],in[2],dev_mapa()->consb(in[1],in[2]));
                        encola_mensaje_consola(s,192);
              };
        }else if (strcmp(args[0],"e_matrix")==0){
              if (strcmp(args[3],"set")==0)
                       mp->inse(in[1],in[2],in[4]);
              else{
                        sprintf(s,"Element matrix at (%d,%d) : %d",in[1],in[2],dev_mapa()->conse(in[1],in[2]));
                        encola_mensaje_consola(s,192);
              };
        }else if (strcmp(args[0],"new")==0){
              if(strcmp(args[1],"enemy")==0){
                w=DBG_NEW warrior("User Enemy",in[2],in[3],DDER,in[4],50,in[5],in[6],(w_clase)in[7],(w_sclase)in[8],in[9],56,192,RAND,"");
                niv->ingresa_enem(w);}
              else if(strcmp(args[1],"item")==0)
                niv->ingresa_item(in[2],in[3],(t_item)in[4]);
              else if(strcmp(args[1],"teleport")==0)
               mp->anyade_telep(in[2],in[3],in[4],in[5]);
              else if(strcmp(args[1],"barrier")==0)
               mp->anyade_barrera(in[2],in[3],in[4]);
              else if(strcmp(args[1],"exit_zone")==0)
               mp->area_final(in[2],in[3],in[4],in[5]);
              else if(strcmp(args[1],"dark_zone")==0)
               mp->anyade_area_oscura(in[2],in[3],in[4],in[5]);
              else if(strcmp(args[1],"start")==0)
               mp->nueva_pos_inicio(in[2],in[3],(w_dir)in[4]);
              else encola_mensaje_consola(PAR,192);
        }else encola_mensaje_consola(COM,192);
}
void batalla::desvela_mapa()
{
        for(int i=0; i<MSIZEX/5; i++)
        for(int j=0; j<MSIZEY/5; j++)
              matr_mapa[i][j]=TRUE;
}
void batalla::reset_warrior(warrior *w)
{
        float x,y;
        w_dir d;
        int i;
        bool t=FALSE;
        warrior *op;
        if(tipoj==COOP){
                x=dev_mapa()->x_inicio(); y=dev_mapa()->y_inicio();
                d=dev_mapa()->dir_inicio();}
        else{
                do{
                        x=rand()%MSIZEX;
                        y=rand()%MSIZEY;

                }while(!permite2(x,y,NULL));
                d=(w_dir)(rand()%4);
        };
        // Telefragg!!!
        i=0;
        while((i=hay_player(x,y,i))>=0){
            op=dev_player(i);
			if(op!=w)
				if (modo_red == MROFFLINE)
					op->danyar(200,DTELE,w);
            i++;};
        if((i=hay_enem(x,y,0))>=0){
                op=dev_enem(i); op->danyar(200,DTELE,w); };
        if(tipoj==DMCH) t=TRUE;
        w->reset(x,y,d,t);
}
void batalla::reset_warrior_red(byte tipo, byte id, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
        float x,y;
        w_dir d;
        int i;
        bool t=FALSE;
        warrior *w=0, *op=0;

		if(tipo == OPPLAYER)
			w = dev_player(id);

        if(tipoj==COOP){
                x=dev_mapa()->x_inicio(); y=dev_mapa()->y_inicio();
                d=dev_mapa()->dir_inicio();}
        else{
                do{
                        x=rand()%MSIZEX;
                        y=rand()%MSIZEY;

                }while(!permite2(x,y,NULL));
                d=(w_dir)(rand()%4);
        };
        // Telefragg!!!
        i=0;
        while((i=hay_player(x,y,i))>=0){
            op=dev_player(i);
			if(op!=w)
				envia_danyo_red(id,DTELE,200,OPPLAYER,i,aRecQueue,aSendQueue);
            i++;};

        if(tipoj==DMCH) t=TRUE;
        w->reset(x,y,d,t);
}
void batalla::abre_cierra_consola()
{
        if (modo_consola==FALSE) modo_consola=TRUE;
        else                 modo_consola=FALSE;
}
void batalla::activa_input_consola()
{
        con_input=TRUE;
}
void batalla::cierra_input_consola()
{
        con_input=FALSE;
}
int batalla::hay_player(bald x, bald y, int m)
{
      int i=m;
      while(i<nplayers){
         if((player[i]->x_pos()==x) && (player[i]->y_pos()==y)) return i;
         i++;
      };
      return -1;
}
// Retorna un puntero a un oponente cercano(con bando diferente), ya sea
// player o enemigo. Si no lo hay en la distancia indicada, retorna
// NULL.
warrior *batalla::oponente_cercano(warrior *w, float x, float y, float dist, byte bando, bool check_dist, byte *categoria, byte *id)
{
      warrior *op;
      float d2=dist;
      int i;
      i=0;
      while(i<nplayers){
         op=player[i]; dist=d2;
         if((op->special(INVIS)) && (check_dist==TRUE)) dist=dist*0.25;
         if((op->bando()!=bando) && (dist_menor(op->fx_pos(),op->fy_pos(),x,y,dist))
                && (op->estado()!=DIE) && (op->estado()!=DEST) && (op->estado()!=OUTL)
                && (!op->special(SHADOW)) && (op!=w))

                if(!obstaculo((bald)x,(bald)y,op->x_pos(),op->y_pos()))
				{
					if(categoria && id)
					{
						*categoria = OPPLAYER;
						*id = i;
					}
                    return op;
				}
         i++;
      };
      i=0;
      while((op=dev_enem(i))!=NULL){
         dist=d2;
         if(op->special(INVIS)) dist=dist*0.25;
         if((op->bando()!=bando) && (dist_menor(op->fx_pos(),op->fy_pos(),x,y,dist))
                && (op->estado()!=DIE) && (op->estado()!=DEST) && (op->estado()!=OUTL)
                && (!op->special(SHADOW)) /*&& (op->clase()!=CASUAL)*/ && (op!=w))

                if(!obstaculo((bald)x,(bald)y,op->x_pos(),op->y_pos()))
				{
					if(categoria && id)
					{
						*categoria = OPENEMY;
						*id = i;
					}
                    return op;
				}
         i++;
      };
      return NULL;
}
// Dados un origen y un destino, devuelve TRUE si hay un obst culo s¢lido en
// la linea recta que los une
bool batalla::obstaculo(bald x1, bald y1, bald x2, bald y2)
{
        bald x=x1, y=y1;
        while((x!=x2) || (y!=y2)){

                if(x<x2) x++;
                else if(x>x2) x--;
                if(y<y2) y++;
                else if(y>y2) y--;
                if(((x1!=x) || (y1!=y)) && ((x!=x2) || (y!=y2))){
                        if(!dev_mapa()->permite(x,y)) return TRUE;
                        if(hay_player(x,y,0)>=0) return TRUE;
                        if(niv->hay_enem(x,y,0)>=0) return TRUE;
                };
        };
        return FALSE;
}
int batalla::hay_bala(bald x, bald y, int m)
{
      int i=m;
      while(i<nbalas){
         if(((bald)(bbala[i]->posx())==x) && ((bald)(bbala[i]->posy())==y)) return i;
         i++;
      };
      return -1;
}
int batalla::hay_explos(bald x, bald y, int m)
{
      int i=m;
      while(i<nexplos){
         if(((bald)(bexplos[i]->posx())==x) && ((bald)(bexplos[i]->posy())==y)) return i;
         i++;
      };
      return -1;
}
// Devuelve una de las explosiones(si la hay) que dista menos que [d] de (x,y)
int batalla::dist_explos(float x, float y, float d)
{
      int i=0;
      while(i<nexplos){
         if((posit(bexplos[i]->posx()-x)<d) && (posit(bexplos[i]->posy()-y)<d)) return i;
         i++;
      };
      return -1;
}
// Devuelve una de las explosiones(si la hay) del tipo llama
int batalla::primera_llama()
{
      int i=0;
      while(i<nexplos){
         if((bexplos[i]->tipo()==ELLA) || (bexplos[i]->tipo()==ELL2) || (bexplos[i]->tipo()==ELL3)) return i;
         i++;
      };
      return -1;
}
// Suma los danyos por explosi¢n en unas coordenadas concretas del mapa
int batalla::danyo_total_explos(float x, float y, float d)
{

      int total=0;
      float dx,dy;
      for(int i=0; i<nexplos; i++){
         dx=(bexplos[i]->posx()-x); dy=(bexplos[i]->posy()-y);
         if(dx*dx+dy*dy<d*d)
            total+=bexplos[i]->fuerza();};
      return total;
}
float batalla::centroplx()
{
      float x=0;
	  if(modo_red != MROFFLINE)
	  {
		  return dev_player(jug_local)->fx_pos();
	  }
	  else
	  {
		for(int i=0; i<nplayers; i++)
              x=x+dev_player(i)->fx_pos();
		return (x/nplayers);
	  }
}
float batalla::centroply()
{
      float y=0;
	  if(modo_red != MROFFLINE)
	  {
		  return dev_player(jug_local)->fy_pos();
	  }
	  else
	  {
		for(int i=0; i<nplayers; i++)
              y=y+dev_player(i)->fy_pos();
		return (y/nplayers);
	  }
}
// Devuelve cierto si no la pantalla es £nica
bool batalla::single_screen()
{
   if (modo_red != MROFFLINE) return TRUE;

   int rx,tamx=640;//SCREEN_W;
   if(nplayers==1) return TRUE;
   if(force_split==TRUE) return FALSE;

   rx=tamx/40;

   if(nplayers==2)
      if(dist_players(0,1)>(rx*rx/2))
         return FALSE;

   if(nplayers==3)
      if((dist_players(0,2)>(rx*rx/2)) || (dist_players(1,2)>(rx*rx/2)))
         return FALSE;

   if(nplayers==4)
      if((dist_players(0,2)>(rx*rx/2)) || (dist_players(1,2)>(rx*rx/2)) ||
      (dist_players(0,3)>(rx*rx/2)) || (dist_players(1,3)>(rx*rx/2)))
         return FALSE;

   return TRUE;
}
// Devuelve TRUE si todos los players, en modo cooperativo, han salido del
// nivel
bool batalla::coop_nivel_completado()
{
        for(int i=0; i<nplayers; i++)
                if((player[i]->estado()!=OUTL) || (player[i]->contador()<WTCAE))
                        return FALSE;
        return TRUE;
}
bool batalla::partida_red_acabada()
{
	return mens_fin_recibido;
}
// Devuelve el tiempo transcurrido (en segundos) desde que empez¢ la batalla
int batalla::tiempo_batalla()
{
        return int(tcont/60);
}
long batalla::tiempo_batalla_ticks()
{
        return tcont;
}
// N£mero de frags (kills) m ximo de los jugadores humanos
int batalla::maxim_frags()
{
      int m=0;
      for(int i=0; i< nplayers; i++)
              if(player[i]->muertes()>m) m=player[i]->muertes();
      return m;
}
int batalla::diferencia_x(int tamx, int tamy, int x, int y)
{
      int cx=tamx/2, cy=tamy/2;
      return int(cx-15.0+((x-centroplx())*30.0)-((y-centroply())*30.0));
}
int batalla::diferencia_y(int tamx, int tamy, int x, int y)
{
      int cx=tamx/2, cy=tamy/2;
      return int(cy-7.0+((x-centroplx())*15.0)+((y-centroply())*15.0));
}
float batalla::dist_players(int i, int j)
{
      float dx=player[i]->fx_pos()-player[j]->fx_pos(),
            dy=player[i]->fy_pos()-player[j]->fy_pos();
      return (dx*dx)+(dy*dy);
}
float batalla::dist_centro(float x, float y)
{
      return sqrt( ((centroplx()-x)*(centroplx()-x))+
                     ((centroply()-y)*(centroply()-y)) );
}
byte batalla::num_players()
{
      return nplayers;
}
int batalla::hay_enem(bald x, bald y, int m)
{
      if(matr_enem[y][x]>0) return niv->hay_enem(x,y,m);
      else                  return -1;
}
void batalla::ingreso_o_retira_enem(bald x, bald y, int m)
{
        matr_enem[y][x]+=m;
}
mapa *batalla::dev_mapa()
{
      return niv->dev_mapa();
}
nivel *batalla::dev_nivel()
{
      return niv;
}
warrior *batalla::dev_player(int c)
{
      if(c<MAXPL) return player[c];
      else return NULL;
}
warrior *batalla::dev_enem(int c)
{
      return niv->dev_enem(c);
}
bala *batalla::dev_bala(int c)
{
     if(c<MAXBA) return bbala[c];
     else {her_error("NIVEL",ERR_NOC); return NULL;};
}
explos *batalla::dev_explos(int c)
{
     if(c<MAXEX) return bexplos[c];
     else {her_error("NIVEL",ERR_NOC); return NULL;};
}
char *batalla::directorio()
{
      return direc;
}
char *batalla::dev_mens(int i)
{
      if(i<nmens) return bmensajes[i];
      else return NULL;
}
char *batalla::dev_mens_consola(int i)
{
      if(i<ncmens) return cmensajes[i];
      else return NULL;
}
char *batalla::dev_prompt_consola()
{
      return cprompt;
}
byte batalla::mens_color(int i)
{
      if(i<nmens) return bmenscolor[i];
      else return 0;
}
byte batalla::mens_consola_color(int i)
{
      if(i<ncmens) return cmenscolor[i];
      else return 0;
}
byte batalla::mens_prio(int i)
{
      if(i<nmens) return bmensprio[i];
      else return 0;
}
warrior* batalla::colision_con_guerrero(float x, float y, warrior *w)
{
      int k;
      warrior *w2;
      // hay otro jugador
      for(k=0; k<nplayers; k++){
      if((w2=dev_player(k))!=w && dev_player(k)->activo()){
            float px=dev_player(k)->fx_pos();
            float py=dev_player(k)->fy_pos();
            if((((px-x)*(px-x))+((py-y)*(py-y))<=DCOL*DCOL)
               && (dev_player(k)->estado()!=DIE)
               && (dev_player(k)->estado()!=DEST)
               && (dev_player(k)->estado()!=OUTL)) return w2;
            };
      };
      // hay enemigo /secundario
      k=0;
      while(dev_enem(k)!=NULL){
      if((w2=dev_enem(k))!=w){
            float px=dev_enem(k)->fx_pos();
            float py=dev_enem(k)->fy_pos();
            if((((px-x)*(px-x))+((py-y)*(py-y))<=DCOL*DCOL)
               && (dev_enem(k)->estado()!=DIE)
               && (dev_enem(k)->estado()!=DEST)
               && (dev_enem(k)->estado()!=OUTL)) return w2;
            };
            k++;
      };
      return NULL;
}
bool batalla::permite(float x, float y, warrior *w)
{
      // hay obstaculo
      if(!dev_mapa()->permite((bald)x,(bald)y)) return FALSE;

      if (colision_con_guerrero(x,y,w) != NULL) return FALSE;

      return TRUE;
}
void batalla::deshacer_solapamientos_players()
{
    int i, j;
    for(i = 0; i < nplayers; i++)
    for(j = i+1; j < nplayers; j++)
    {
        warrior *w1, *w2;

        w1 = dev_player(i);
        w2 = dev_player(j);

        if( w1 && w2)
        if( w1->activo() && w1->estado() != DIE && w1->estado() != DEST && w1->estado() != OUTL &&
            w2->activo() && w2->estado() != DIE && w2->estado() != DEST && w2->estado() != OUTL )
        {

            int iterar = 5;

            while(iterar>0)
            {

                float x1 = w1->fx_pos();
                float y1 = w1->fy_pos();
                float x2 = w2->fx_pos();
                float y2 = w2->fy_pos();

                float dx = fabs(x2 - x1);
                float dy = fabs(y2 - y1);

                if( w1->estado() != DIE && w1->estado() != DEST && w1->estado() != OUTL &&
                    w2->estado() != DIE && w2->estado() != DEST && w2->estado() != OUTL &&
                    dx*dx + dy*dy <= DCOL*DCOL)
                {
                    // Solapamiento que debe deshacerse!!
                    if(dx > dy)
                    {
                        if (x1 > x2)
                        {
                            warrior *wt = w1; w1 = w2; w2 = wt;
                        }

                        w1->avanza(DIZQ, dev_mapa()->permite((bald)(w1->fx_pos() - WPASO), (bald)w1->fy_pos()), true);
                        w2->avanza(DDER, dev_mapa()->permite((bald)(w2->fx_pos() + WPASO), (bald)w2->fy_pos()), true);
                    }
                    else
                    {
                        if (y1 > y2)
                        {
                            warrior *wt = w1; w1 = w2; w2 = wt;
                        }

                        w1->avanza(DARR, dev_mapa()->permite((bald)w1->fx_pos(), (bald)(w1->fy_pos() - WPASO)), true);
                        w2->avanza(DABA, dev_mapa()->permite((bald)w2->fx_pos(), (bald)(w2->fy_pos() + WPASO)), true);
                    }

                    iterar--;

                }
                else
                {
                    iterar = 0;
                }
            }
        }
    }
}
bool batalla::enemigos_en_mapa()
{
    return enemy_map;
}
bool batalla::permite2(float x, float y, warrior *w)
{
      // No hay suelo
      byte b=dev_mapa()->consb(x,y);
      if((b==BDA1) || (b==BDA2) || (b==BMO1) || (b==BMO2) || (b==NADA)) return FALSE;
      // hay obstaculo
      b=dev_mapa()->conse(x,y);
      if((b!=VACIO) && (b!=DTRH) && (b!=DTRV) && (b!=ITEM) && (b!=TELE) && (b!=EXIT)) return FALSE;
      int i=dev_mapa()->hay_puerta(x,y);
      if(i>=0){
               if((!dev_mapa()->dev_puerta(i)->permite()) &&
                 (dev_mapa()->dev_puerta(i)->tipo()!=NORM)) return FALSE;
      };
      if(dev_mapa()->hay_barr(x,y)>=0) return FALSE;
      int k;
      // hay jugador
      for(k=0; k<nplayers; k++){
        if(dev_player(k)!=w){
            float px=dev_player(k)->fx_pos();
            float py=dev_player(k)->fy_pos();
            if((((px-x)*(px-x))+((py-y)*(py-y))<=DCOL*DCOL)
               && (dev_player(k)->estado()!=DIE)
               && (dev_player(k)->estado()!=DEST)
               && (dev_player(k)->estado()!=OUTL)) return FALSE;
            };
      };
      // hay otro enemigo /secundario
      k=0;
      while(dev_enem(k)!=NULL){
        if(dev_enem(k)!=w){
            float px=dev_enem(k)->fx_pos();
            float py=dev_enem(k)->fy_pos();
            if((((px-x)*(px-x))+((py-y)*(py-y))<=DCOL*DCOL)
               && (dev_enem(k)->estado()!=DIE)
               && (dev_enem(k)->estado()!=DEST)
               && (dev_enem(k)->estado()!=OUTL)) return FALSE;
            };
            k++;
      };
      //hay explosi¢n
      if(hay_explos(x,y,0)>=0) return FALSE;
      return TRUE;
}
void batalla::retira_bala(int c)
{
      int i;
      if(c<nbalas){
            delete bbala[c];
            for(i=c; i<MAXBA-1; i++)
                     bbala[i]=bbala[i+1];
            bbala[MAXBA-1]=NULL;
            nbalas--;
      }else her_error("MAPA",ERR_NOC);
}
void batalla::retira_explos(int c)
{
      int i;
      if(c<nexplos){
            delete bexplos[c];
            for(i=c; i<MAXEX-1; i++)
                     bexplos[i]=bexplos[i+1];
            bbala[MAXEX-1]=NULL;
            nexplos--;
      }else her_error("MAPA",ERR_NOC);
}
void batalla::desencola_mensaje(int c)
{
      for(int i=0; i<nmens-1; i++){
              sprintf(bmensajes[i],"%s",bmensajes[i+1]);
              bmenscolor[i]=bmenscolor[i+1];
              bmenstime[i]=bmenstime[i+1];
              bmensprio[i]=bmensprio[i+1];
              };
      nmens--;
}
void batalla::actualiza_ia_control(warrior *w, controlm *cont, int cual, int dif, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
        // DIF : 1-5
        warrior *op=NULL;
        char frase[100];
        int FREQ=10+((5-dif)*10), SCRITICA=((5-dif)*3), CQUANT=15+((5-dif)*10);
        float DISTVISION=7.0, DISTATAC=1.0, DISTDISP=0.5;
        byte modo=1;
        w_dir di;
        CQUANT=20;
        if(w->arma()==0) modo=0;
        if(w->clase()==OMEGA) modo=0;
        if(w->clase()==BETA)
        switch(w->arma()){
                case 7 : case 8 : case 5 : case 4 : case 1 : case 2 : case 9 : case 10 : modo=1; break;
                case 6 : case 3 : case 0 : modo=0; break;};

        // Si en peligro de caer, reacciona (o eso espero)
        if((niv->dev_mapa()->consb(w->front_x(0.5),w->front_y(0.5))<BSU1)
        || (dist_explos(w->front_x(0.5),w->front_y(0.5),0.5)>=0)
        || (dev_mapa()->hay_laser(w->front_x(0.5),w->front_y(0.5))>=0))
                w->set_contador_ia(CQUANT);
        cont->reset();
        if(w->clase()==CASUAL)
                switch(w->subclase()){

                        case CESCAP : if(w->contador_ia()==CQUANT){

                                w->nueva_dir_ia(direccion_movim(w->fx_pos(),w->fy_pos(),
                                                (bald)((niv->dev_mapa()->dev_final()->inx()+niv->dev_mapa()->dev_final()->finx())/2),
                                                (bald)((niv->dev_mapa()->dev_final()->iny()+niv->dev_mapa()->dev_final()->finy())/2),
                                                w,TRUE));
                                w->reset_contador_ia();};

                                switch(w->dir_ia()){

                                        case DARR : cont->activa(CARR); break;
                                        case DABA : cont->activa(CABA); break;
                                        case DDER : cont->activa(CDER); break;
                                        case DIZQ : cont->activa(CIZQ); break;};
                                break;

                        case CATENT :
                        case CPASEA : if(!permite2(w->front_x(0.5),w->front_y(0.5),w))

                                        switch(rand()%2){

                                                case 0 : w->nueva_dir_ia((w_dir)((w->dir_ia()+1)%4)); break;
                                                case 1 : w->nueva_dir_ia((w_dir)((w->dir_ia()-1)%4)); break;};
                                        switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                        break;

						case CINDIC : if(w->oponente()!=NULL){

						                        w->nueva_dir(dir_mirando(w->x_pos(),w->y_pos(),w->oponente()->x_pos(),w->oponente()->y_pos()));
												if(dist(w->x_pos(),w->y_pos(),w->oponente()->x_pos(),w->oponente()->y_pos())>10*10){
													w->nuevo_oponente(NULL);
													w->nueva_dir(w->dir_original());
												}
									  };
                                      break;
				};

        if(w->clase()!=CASUAL){

        //Le estan atacando; se pelear  con quien le ataca, sea del bando que sea!!!
        if((w->estado()==WALK) && (w->contador()>0)){
            if(w!=w->agresor()) w->nuevo_oponente(w->agresor());
            if (w->estado_ia() == ESPER) {
                sprintf(frase, "%s:%s", w->nombre(), w->frase());
                if (modo_red == MRSERVER)
                    encola_mensaje_red(frase, 15, PRIO1 | PRIO2 | PRIO3 | PRIO4 | PRIO5 | PRIO6 | PRIO7 | PRIO8, aRecQueue, aSendQueue);
                else
                    encola_mensaje(frase, 15, PRIO1 | PRIO2 | PRIO3 | PRIO4);
            };
            w->nuevo_estado_ia(PERSE);};

        switch(modo){
                case 0 :   switch(w->estado_ia()){

                                case PASEO : if(w->contador_ia()==CQUANT){
                                                 // Avista un enemigo
                                                 if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,w->bando(),TRUE))!=NULL){
                                                        w->nuevo_oponente(op);
                                                        w->nuevo_estado_ia(PERSE);};
                                                 w->reset_contador_ia();
											 }else{

                                              if(!permite2(w->front_x(0.5),w->front_y(0.5),w))
                                                switch(rand()%2){
                                                  case 0 : w->nueva_dir_ia((w_dir)((w->dir_ia()+1)%4)); break;
                                                  case 1 : w->nueva_dir_ia((w_dir)((w->dir_ia()-1)%4)); break;};

                                              //Le estan atacando; se pelear  con quien le ataca, sea del bando que sea!!!
                                                 if((w->estado()==WALK) && (w->contador()>0)){
                                                        w->nuevo_oponente(w->agresor());
                                                        w->nuevo_estado_ia(PERSE);};

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
											 };
                                              break;

                                case PERSE :  if(w->contador_ia()==CQUANT)
											  if(w->oponente()!=NULL){
                                              w->nueva_dir_ia(direccion_movim(w->fx_pos(),w->fy_pos(),
                                                                w->oponente()->fx_pos(),w->oponente()->fy_pos(),w,TRUE));
                                              w->reset_contador_ia();
                                              };
                                              // Demasiado lejos del oponente
											  if(w->oponente()!=NULL)
                                              if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTVISION*2))
                                                   w->nuevo_estado_ia(PASEO);
                                              // Cerca del oponente, para atacar
                                              else if(dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTATAC))
                                                   w->nuevo_estado_ia(ROTAR);
                                              // Oponente actual ya muerto
                                              op=w->oponente();
                                              if(op!=NULL)
                                              if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                              w->nuevo_estado_ia(PASEO);
                                              // En peligro, huye
                                              if(op!=NULL)
                                              if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);
                                              // Avista otro enemigo m s cerca
                                              if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,w->bando(),TRUE))!=NULL){
                                                     w->nuevo_oponente(op);};

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;

                                case ROTAR :  if(w->contador_ia()==FREQ){

                                                // Demasiado lejos del oponente
												if(w->oponente()!=NULL)
                                                if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTATAC))
                                                   w->nuevo_estado_ia(PERSE);
                                                // Oponente actual ya muerto
                                                op=w->oponente();
												if(op!=NULL)
                                                if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                                   w->nuevo_estado_ia(PASEO);
                                                // En peligro, huye
												if(w->oponente()!=NULL)
                                                if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);

                                                w->reset_contador_ia();};
                                              //Girarse para mirar hacia oponente
												if(w->oponente()!=NULL){
													di=dir_mirando(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos());
													if(w->dir()!=di) w->nueva_dir(di);
													else w->nuevo_estado_ia(ATACA);
												}
                                              break;

                                case ATACA :  if(w->contador_ia()==FREQ){
                                                // No mira hacia el adversario
												if(w->oponente()!=NULL){
													di=dir_mirando(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos());
													if(w->dir()!=di) w->nuevo_estado_ia(ROTAR);
												};
                                                // Demasiado lejos del oponente
												if(w->oponente()!=NULL)
                                                if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTATAC))
                                                   w->nuevo_estado_ia(PERSE);
                                                // En peligro, huye
												if(w->oponente()!=NULL)
                                                if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);
                                                // Oponente actual ya muerto
                                                op=w->oponente();
												if(op!=NULL)
                                                if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                                    w->nuevo_estado_ia(PASEO);
                                                w->reset_contador_ia();};

                                              // Cubrirse
                                              op=w->oponente();
                                              if(((op->estado()==PUNY) || (op->estado()==SHOT) || (op->estado()==HECH) || (op->estado()==SLAS)
                                                    || (dist_explos(w->fx_pos(),w->fy_pos(),2.0)>=0)) &&
                                                    (op->dir()==dir_mirando(op->fx_pos(),op->fy_pos(),w->fx_pos(),w->fy_pos()))){
                                                if(((rand()%(FREQ/3)==0) || (w->estado()==DEFE)) && (w->clase()==OMEGA))
                                                        cont->activa(CBU3);
                                              }else
                                              // Disparar!!!
                                              if(rand()%FREQ==0) cont->activa(CBU1);
                                              break;

                                case HUIDA :  if(w->contador_ia()==CQUANT){
											  if(w->oponente()!=NULL)
                                              w->nueva_dir_ia(direccion_movim(w->fx_pos(),w->fy_pos(),
                                                                w->oponente()->fx_pos(),w->oponente()->fy_pos(),w,FALSE));
                                              // Demasiado lejos del oponente
											  if(w->oponente()!=NULL)
                                              if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTVISION*2))
                                                   w->nuevo_estado_ia(PASEO);
                                              w->reset_contador_ia();};

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;
                                              // Boss. Empieza a atacar si uno de sus compa¤eros est  en
                                              // peligro o si le atacan
                                case ESPER : op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,(byte)(w->bando()),TRUE);
                                             if((op!=NULL) || ((w->estado()==WALK) && (w->contador()!=0))){
                                                        w->nuevo_estado_ia(PASEO);
                                                        w->reset_contador_ia();
                                                        sprintf(frase,"%s:%s",w->nombre(),w->frase());
                                                        if(modo_red == MRSERVER)
                                                            encola_mensaje_red(frase,15,ALLPRIO, aRecQueue, aSendQueue);
                                                        else
                                                            encola_mensaje(frase, 15, PRIO1 | PRIO2 | PRIO3 | PRIO4);
                                             };
                                              break;
                        }; break;

                case 1 : op=w->oponente();
                         switch(w->estado_ia()){

                                case PASEO :  if(w->contador_ia()==CQUANT){
                                                 // Avista un enemigo
                                                 if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,w->bando(),TRUE))!=NULL){
                                                        w->nuevo_oponente(op);
                                                        w->nuevo_estado_ia(APUNT);};
                                                 w->reset_contador_ia();};

                                              if(!permite2(w->front_x(0.5),w->front_y(0.5),w))
                                                switch(rand()%2){
                                                  case 0 : w->nueva_dir_ia((w_dir)((w->dir_ia()+1)%4)); break;
                                                  case 1 : w->nueva_dir_ia((w_dir)((w->dir_ia()-1)%4)); break;};

/*                                              //Le estan atacando; se pelear  con quien le ataca, sea del bando que sea!!!
                                                 if((w->estado()==WALK) && (w->contador()>0))
                                                 if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION*1.5,w->bando()))!=NULL){
                                                        w->nuevo_oponente(op);
                                                        w->nuevo_estado_ia(PERSE);};*/

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;

                                case APUNT :  //if(op!=NULL){//if(w->contador_ia()==FREQ){
											  if(w->oponente()!=NULL)
                                              w->nueva_dir_ia(dir_alinearse(w->fx_pos(),w->fy_pos(),
                                                                w->oponente()->fx_pos(),w->oponente()->fy_pos(),w,TRUE));
                                              // Demasiado lejos del oponente
											  if(w->oponente()!=NULL)
                                              if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTVISION*2))
                                                   w->nuevo_estado_ia(PASEO);
                                              // En linea con el oponente
											  if(w->oponente()!=NULL)
                                              if((en_linea(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTDISP)))
                                                w->nuevo_estado_ia(PERSE);
                                              // Oponente actual ya muerto
                                              op=w->oponente();
                                              if(op!=NULL)
                                              if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                              w->nuevo_estado_ia(PASEO);
                                              // En peligro, huye
                                              if(op!=NULL)
                                               if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);
                                              // Avista otro enemigo m s cerca
                                              if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,w->bando(),TRUE))!=NULL){
                                                     w->nuevo_oponente(op);};
                                              w->reset_contador_ia();

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;

                                case PERSE :  if(w->contador_ia()==CQUANT){
											  if(w->oponente()!=NULL)
                                              w->nueva_dir_ia(direccion_movim(w->fx_pos(),w->fy_pos(),
                                                                w->oponente()->fx_pos(),w->oponente()->fy_pos(),w,TRUE));
                                              w->reset_contador_ia();
                                              };
                                              // Demasiado lejos del oponente
											  if(w->oponente()!=NULL)
                                              if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTVISION*2))
                                                   w->nuevo_estado_ia(PASEO);
                                              // Hay espacio libre entre yo y mi oponente y estamos en linea
											  if(w->oponente()!=NULL)
                                              if((en_linea(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTDISP)) &&
                                              (!obstaculo(w->x_pos(),w->y_pos(),w->oponente()->x_pos(),w->oponente()->y_pos())))
                                                   w->nuevo_estado_ia(ROTAR);
                                              // Oponente actual ya muerto
                                              op=w->oponente();
											  if(op!=NULL)
                                              if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                              w->nuevo_estado_ia(PASEO);
                                              // En peligro, huye
											  if(w->oponente()!=NULL)
                                              if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);
                                              // Avista otro enemigo m s cerca
                                              if((op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,w->bando(),TRUE))!=NULL){
                                                     w->nuevo_oponente(op);};

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;


                                case ROTAR :  if(1){//if(w->contador_ia()==FREQ){

                                                op=w->oponente();
                                                // Demasiado lejos del oponente
												if(op!=NULL)
                                                if((!en_linea(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTDISP)) ||
                                                 (obstaculo(w->x_pos(),w->y_pos(),op->x_pos(),op->y_pos())))
                                                   w->nuevo_estado_ia(APUNT);
                                                // Oponente actual ya muerto
                                                if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                                   w->nuevo_estado_ia(PASEO);
                                                // En peligro, huye
												if(w->oponente()!=NULL)
                                                if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);

                                                w->reset_contador_ia();};
												//Girarse para mirar hacia oponente
												if(w->oponente()!=NULL){
													di=dir_mirando(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos());
													if(w->dir()!=di) w->nueva_dir(di);
													else w->nuevo_estado_ia(ATACA);
												};
												break;

                                case ATACA :  //if(w->contador_ia()==FREQ){
                                              if(1){
                                                // No mira hacia el adversario
												if(w->oponente()!=NULL)
                                                di=dir_mirando(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos());
                                                if(w->dir()!=di) w->nuevo_estado_ia(ROTAR);
                                                // No esta en linea con el oponente
												if(w->oponente()!=NULL)
                                                if((!en_linea(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTDISP))
                                                  || (obstaculo(w->x_pos(),w->y_pos(),w->oponente()->x_pos(),w->oponente()->y_pos())))
                                                   w->nuevo_estado_ia(PERSE);
                                                // En peligro, huye
												if(w->oponente()!=NULL)
                                                if((w->salud()<SCRITICA) || (w->oponente()->special(FEAR))) w->nuevo_estado_ia(HUIDA);
                                                // Oponente actual ya muerto
                                                op=w->oponente();
                                                if(op!=NULL)
                                                if((op->estado()==DIE) || (op->estado()==DEST) || (op->estado()==OUTL))
                                                    w->nuevo_estado_ia(PASEO);

                                                w->reset_contador_ia();};

                                              // Disparar!!!
                                              if(rand()%FREQ==0) cont->activa(CBU1);
                                              break;

                                case HUIDA :  if(w->contador_ia()==CQUANT){
											  if(w->oponente()!=NULL)
                                              w->nueva_dir_ia(direccion_movim(w->fx_pos(),w->fy_pos(),
                                                                w->oponente()->fx_pos(),w->oponente()->fy_pos(),w,FALSE));
                                              // Demasiado lejos del oponente
											  if(w->oponente()!=NULL)
                                              if(!dist_menor(w->fx_pos(),w->fy_pos(),w->oponente()->fx_pos(),w->oponente()->fy_pos(),DISTVISION*2))
                                                   w->nuevo_estado_ia(PASEO);
                                              w->reset_contador_ia();};

                                              switch(w->dir_ia()){
                                                case DARR : cont->activa(CARR); break;
                                                case DABA : cont->activa(CABA); break;
                                                case DDER : cont->activa(CDER); break;
                                                case DIZQ : cont->activa(CIZQ); break;};
                                              break;
                                              // Boss. Empieza a atacar si uno de sus compa¤eros est  en
                                              // peligro o si le atacan
                                case ESPER : op=oponente_cercano(w,w->fx_pos(),w->fy_pos(),DISTVISION,(byte)(w->bando()),TRUE);
                                             if((op!=NULL) || ((w->estado()==WALK) && (w->contador()!=0))){
                                                        w->nuevo_estado_ia(PASEO);
                                                        w->reset_contador_ia();
                                                        sprintf(frase,"%s:%s",w->nombre(),w->frase());
                                                        encola_mensaje(frase,15,PRIO1|PRIO2|PRIO3|PRIO4);};
                                              break;
                        }; break;
                };
        };
}
w_dir batalla::direccion_movim(float orx, float ory, float dex, float dey, warrior *w, bool ida)
{
        float difx=(dex-orx),dify=(dey-ory);
        int angle;
        bool par, pab, piz, pde;

        angle=(int)(atan2(dify,difx)*180/3.1416);

        par=permite2(orx,ory-0.5,w);
        pab=permite2(orx,ory+0.5,w);
        piz=permite2(orx-0.5,ory,w);
        pde=permite2(orx+0.5,ory,w);

        if(angle>135) return mejor_direccion(DIZQ,DABA,DARR,DDER,piz,pab,par,pde,ida);
        else if(angle>90) return mejor_direccion(DABA,DIZQ,DDER,DARR,pab,piz,pde,par,ida);
        else if(angle>45) return mejor_direccion(DABA,DDER,DIZQ,DARR,pab,pde,piz,par,ida);
        else if(angle>0) return mejor_direccion(DDER,DABA,DARR,DIZQ,pde,pab,par,piz,ida);
        else if(angle>-45) return mejor_direccion(DDER,DARR,DABA,DIZQ,pde,par,pab,piz,ida);
        else if(angle>-90) return mejor_direccion(DARR,DDER,DIZQ,DABA,par,pde,piz,pab,ida);
        else if(angle>-135) return mejor_direccion(DARR,DIZQ,DDER,DABA,par,piz,pde,pab,ida);
        else if(angle>-180) return mejor_direccion(DIZQ,DARR,DABA,DDER,piz,par,pab,pde,ida);
		return DABA;
}
w_dir batalla::dir_alinearse(float x1, float y1, float x2, float y2, warrior *w, bool ida)
{
        float dx=posit(x1-x2), dy=posit(y1-y2);
        bool par, pab, piz, pde;

        par=permite2(x1,y1-0.5,w);
        pab=permite2(x1,y1+0.5,w);
        piz=permite2(x1-0.5,y1,w);
        pde=permite2(x1+0.5,y1,w);

        if(dx>dy){
                  if(y1<y2) return mejor_direccion(DABA,DIZQ,DDER,DARR,pab,piz,pde,par,ida);
                  else      return mejor_direccion(DARR,DDER,DIZQ,DABA,par,pde,piz,pab,ida);
        }else{
                  if(x1<x2) return mejor_direccion(DDER,DARR,DABA,DIZQ,pde,par,pab,piz,ida);
                  else      return mejor_direccion(DIZQ,DABA,DARR,DDER,piz,pab,par,pde,ida);};
}
w_dir batalla::mejor_direccion(w_dir d1,w_dir d2,w_dir d3,w_dir d4,bool p1,bool p2,bool p3,bool p4,bool mejor)
{
        if(mejor==TRUE){
                if(p1) return d1;
                else if(p2) return d2;
                else if(p3) return d3;
                else if(p4) return d4;
                return d1;
        }else{
                if(p4) return d4;
                else if(p3) return d3;
                else if(p2) return d2;
                else if(p1) return d1;
                return d4;
        };
}
void batalla::actualiza_player(byte c, warrior *w, controlm *cont, bool pl)
{
      warrior *sec;
      disparador *dis;
      t_item it;
      w_dir d;
      int i, t_so=0;
      bool mover=FALSE, retira=FALSE;
      float nx=w->fx_pos(), ny=w->fy_pos(),x,y,f=1, co=w->level_coef(), cb=0.95+(0.05*w->level());
      mapa *mp=dev_mapa();
      char s[200], aux[20];
      byte prio, allprio=PRIO1|PRIO2|PRIO3|PRIO4|PRIO5|PRIO6|PRIO7|PRIO8;
      cdanyo cd;

      switch(c){
                case 0 : prio=PRIO1; break;
                case 1 : prio=PRIO2; break;
                case 2 : prio=PRIO3; break;
                case 3 : prio=PRIO4; break;
				case 4 : prio=PRIO5; break;
                case 5 : prio=PRIO6; break;
                case 6 : prio=PRIO7; break;
                case 7 : prio=PRIO8; break;};

        // Sube un nivel si exp. suficiente
        if(pl)
        if((w->exp()>=exp_level(w->level()+1)) && (w->level()<WMAXLEV))
                {w->sube_level();
                 w->nueva_salud(w->salud_maxima());
                 sprintf(s,"%s reaches level %d.",w->nombre(),w->level());
                 encola_mensaje(s,w->color(1),allprio);
                 sonido_ambiente(S_LEVE,w->fx_pos(),w->fy_pos());
                 };

      // Abre puertas que no req. interruptor
      i=mp->hay_puerta((bald)w->front_x(0.5),(bald)w->front_y(0.5));
      if(i>=0){

                switch(mp->dev_puerta(i)->tipo()){
                case NORM : mp->abre_puerta(i); break;
                case LLA1 : if(w->posee_llave(0)) mp->abre_puerta(i); break;
                case LLA2 : if(w->posee_llave(1)) mp->abre_puerta(i); break;
                case LLA3 : if(w->posee_llave(2)) mp->abre_puerta(i); break;
                case LLA4 : if(w->posee_llave(3)) mp->abre_puerta(i); break;
                case GEMA : if(gema) mp->abre_puerta(i); break;
                };
      };
      // DA¥O
      if((w->estado()!=DEST) && (w->estado()!=DIE) && (w->estado()!=OUTL) && (w->estado()!=CONG) && (w->estado()!=TELP) && (w->dios()==FALSE)){

                        // Da¤a si caminando por baldosa da¤ina
                        switch(mp->consb(w->x_pos(),w->y_pos())){
                           case BDA1:
                           case BDA2: if((w->contador()==0) &&
                                      (mp->hay_puente(w->x_pos(),w->y_pos())<0) && (!w->special(INVUL))){
                                             w->danyar(5,DSUEL,NULL);
                                             sonido_warrior(w,0);};
                                             break;
                        };

                        // Da¤a si caminando por baldosa mortal
                        switch(mp->consb(w->x_pos(),w->y_pos())){
                           case BMO1:
                           case BMO2: if((w->contador()==0) &&
                                      (mp->hay_puente(w->x_pos(),w->y_pos())<0) && (!w->special(INVUL))){
                                             w->danyar(1,DSUEL,NULL);
                                             w->nuevo_estado(DIE); }; break;
                        };
                        // Da¤a si hay explosi¢n
                        if((w->estado()!=DEFE) && ((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0) &&
                           ((w->contador()==0) || (w->estado()!=WALK))
                           && (!w->special(INVUL))){
                              if(dev_explos(i)->tipo()==EHIE)
                                   {w->nuevo_estado(CONG);}
                              else {
                                    switch(dev_explos(i)->tipo()){
                                        case EBAL : cd=DBALA; break;
                                        case ECOH : cd=DCOHE; break;
                                        case EFUE : cd=DFUEG; break;
                                        case EHIE : cd=DHIEL; break;
                                        case EELE : cd=DELEC; break;
                                        default : cd=DFUEG; break;
                                        };
                                    w->danyar(danyo_total_explos(w->fx_pos(),w->fy_pos(),1.0),cd,dev_explos(i)->creador());
                                    if(dev_explos(i)->tipo()==EELE) w->electrocutar();
                                    sonido_warrior(w,0);};
                              };
                        // Da¤a si hay barrera l ser
                        if((mp->hay_laser(w->fx_pos(),w->fy_pos())>=0) && ((w->contador()==0) || (w->estado()!=WALK))){
                           w->danyar(45,DLASE,NULL);
                           sonido_warrior(w,0);};

                        // Da¤o por electrocutado
                        if((w->electrocutado()==TRUE) && ((bcont%30)==0))
                        if(w->salud()>0) w->nueva_salud(w->salud()-1);
                        else             w->nuevo_estado(DIE);

      };
      // Hotkeys para armas
      if(c==0)
        for(i=0; i<11; i++)
                 if(cont->tecla_numero(i))
                        if(w->set_arma(i)==FALSE) encola_mensaje("Out of weapon!",w->color(1),prio);

      switch(w->estado()){

            case WALK : if(cont->arr()) {ny-=WPASO*co; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO*co; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO*co; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO*co; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        // Teletransporta, si hay teleport
                        if(mp->hay_telep((bald)nx,(bald)ny)>=0){
                              w->nueva_pos(float((bald)nx+0.5),float((bald)ny+0.5));
                              w->nuevo_estado(TELP);
                              sonido_ambiente(S_TELE,w->fx_pos(),w->fy_pos());
                              break;};

                        // Cambio de arma
                        if((cont->boton(2)) && (w->clase()!=OMEGA))
                           w->cambia_arma();

                        // Activa un interruptor del mapa
                        i=mp->hay_interr((bald)w->front_x(0.5),(bald)w->front_y(0.5));
                        if((cont->boton(3)) && (i>=0) && (mp->dev_interr(i)->permite())){
                                              mp->activa_interr(i);
                                              // Disparadores
                                              if(mp->dev_interr(i)->tobj()==DISP){
                                                   dis=mp->dev_disp(mp->hay_disp(mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy()));
                                                   ingresa_bala(dis->x_pos(),dis->y_pos(),dis->dir(),0.2,0,(b_tipo)dis->t_bala(),5,NULL);
                                                   sonido_ambiente(S_TRAM,mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy());};
                                              sonido_ambiente(S_INTER,mp->dev_interr(i)->x_pos(),mp->dev_interr(i)->y_pos());
                                              };

                        // Interactua con un pers. secundario
                        if(cont->boton(3))
                        if((i=hay_enem((bald)w->front_x(1.0),(bald)w->front_y(1.0),0))>=0){
                                sec=dev_enem(i);
                                if((sec->bando()==w->bando()) || (sec->clase()==CASUAL)){
                                   // Suelta un ¡tem
                                   it=sec->item_asociado();
                                   // Se gira hacia el jugador y pronuncia su frase
                                   sec->nuevo_oponente(w);
                                   sprintf(s,"%s:%s",sec->nombre(),sec->frase());
                                   if((sec->interactuar()==TRUE) && (sec->estado()<TELP)){
                                      if(sec->dios()==TRUE) encola_mensaje(s,64,prio);
                                      else          encola_mensaje(s,15,prio);
                                      if(it!=NING)
                                      ingresa_item_util(sec->front_x(0.75),sec->front_y(0.75),it);
                                   };

                                };
                        };
                        i=-1;

                        // Abre puerta si estamos justo en el umbral
                        if((i=mp->hay_puerta(w->x_pos(),w->y_pos()))>=0)
                              mp->abre_puerta(i);

                        // Descubre  reas secretas
                        if(pl)
                        if((i=mp->hay_secr(w->x_pos(),w->y_pos()))>=0){
                             mp->retira_secr(i);
                             encola_mensaje("Found a secret area!",w->color(1),prio);
                             sonido_ambiente(S_SECR,w->fx_pos(),w->fy_pos());
                             };

                        // Salir de la zona
                        if(mp->es_final(w->x_pos(),w->y_pos())){
                           w->nuevo_estado(OUTL);
                           sonido_ambiente(S_TELE,w->fx_pos(),w->fy_pos());
                           sprintf(s,"%s gets out of the zone.",w->nombre());
                           if(pl) encola_mensaje(s,w->color(1),allprio); break;};

                        // Cubrirse con el escudo
                        if((cont->boton(2)) && (w->clase()==OMEGA) && (w->contador()==0) && (w->escudo()>0))
                           w->nuevo_estado(DEFE);

                        // Recoger items
                        if(pl)
                        if(niv->hay_item(w->x_pos(),w->y_pos())){
                           t_so=0;
                           if(niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->es_cogible())
                           switch(niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->tipo()){
                              case BOTI : if(w->salud()<w->salud_maxima()){
                                             t_so=2;
                                             w->nueva_salud((word)(w->salud()+(w->salud_maxima()*0.3)));
                                             if(w->salud()>w->salud_maxima()) w->nueva_salud(w->salud_maxima());
                                             retira=TRUE; encola_mensaje("First-aid kit +30%",w->color(1),prio);};
                                          break;
                              case KEY1 : if(w->posee_llave(0)==FALSE){
                                             t_so=4;
                                             w->obtiene_llave(0);
                                             retira=TRUE; encola_mensaje("Iron key",w->color(1),prio);};
                                          break;
                              case KEY2 : if(w->posee_llave(1)==FALSE){
                                             t_so=4;
                                             w->obtiene_llave(1);
                                             retira=TRUE; encola_mensaje("Bronze key",w->color(1),prio);};
                                          break;
                              case KEY3 : if(w->posee_llave(2)==FALSE){
                                             t_so=4;
                                             w->obtiene_llave(2);
                                             retira=TRUE; encola_mensaje("Silver key",w->color(1),prio);};
                                          break;
                              case KEY4 : if(w->posee_llave(3)==FALSE){
                                             t_so=4;
                                             w->obtiene_llave(3);
                                             retira=TRUE; encola_mensaje("Gold key",w->color(1),prio);};
                                          break;
                              case PIST : if((w->clase()==SIGMA) && (!w->tiene_arma(1))){
                                             t_so=3;
                                             w->nueva_arma(1);
                                             retira=TRUE; encola_mensaje("Pistol",w->color(1),prio);};
                                          break;
                              case AMET : if((w->clase()==SIGMA) && (!w->tiene_arma(2))){
                                             t_so=3;
                                             w->nueva_arma(2);
                                             retira=TRUE; encola_mensaje("Machine-gun",w->color(1),prio);};
                                          break;
                              case LANZ : if((w->clase()==SIGMA) && (!w->tiene_arma(3))){
                                             t_so=3;
                                             w->nueva_arma(3);
                                             retira=TRUE; encola_mensaje("Rocket launcher",w->color(1),prio);};
                                          break;
                              case ANIQ : if((w->clase()==SIGMA) && (!w->tiene_arma(4))){
                                             t_so=3;
                                             w->nueva_arma(4);
                                             retira=TRUE; encola_mensaje("\"Annihilator\"",w->color(1),prio);};
                                          break;
                              case DRAG : if((w->clase()==SIGMA) && (!w->tiene_arma(5))){
                                             t_so=3;
                                             w->nueva_arma(5);
                                             retira=TRUE; encola_mensaje("\"Dragon\"",w->color(1),prio);};
                                          break;
                              case INFE : if((w->clase()==SIGMA) && (!w->tiene_arma(6))){
                                             t_so=3;
                                             w->nueva_arma(6);
                                             retira=TRUE; encola_mensaje("\"Inferno\"",w->color(1),prio);};
                                          break;
                              case SHOC : if((w->clase()==SIGMA) && (!w->tiene_arma(7))){
                                             t_so=3;
                                             w->nueva_arma(7);
                                             retira=TRUE; encola_mensaje("Electro-shock gun",w->color(1),prio);};
                                          break;
                              case BALA : if((w->clase()==SIGMA) && (w->municion(0)<w->municion_maxima(0))){
                                             t_so=3;
                                             w->nueva_municion(0,w->municion(0)+30);
                                             if(w->municion(0)>w->municion_maxima(0)) w->nueva_municion(0,w->municion_maxima(0));
                                             retira=TRUE; encola_mensaje("Bullets +30",w->color(1),prio);};
                                          break;
                              case COHE : if((w->clase()==SIGMA) && (w->municion(1)<w->municion_maxima(1))){
                                             t_so=3;
                                             w->nueva_municion(1,w->municion(1)+5);
                                             if(w->municion(1)>w->municion_maxima(1)) w->nueva_municion(1,w->municion_maxima(1));
                                             retira=TRUE; encola_mensaje("Rockets +5",w->color(1),prio);};
                                          break;
                              case NAPA : if((w->clase()==SIGMA) && (w->municion(2)<300)){
                                             t_so=3;
                                             w->nueva_municion(2,w->municion(2)+15);
                                             if(w->municion(2)>w->municion_maxima(2)) w->nueva_municion(2,w->municion_maxima(2));
                                             retira=TRUE; encola_mensaje("Napalm +15",w->color(1),prio);};
                                          break;
                              case DAGA : if((w->clase()==OMEGA) && (w->arma()<1)){
                                             w->nueva_arma(1); t_so=1;
                                             retira=TRUE; encola_mensaje("Dager",w->color(1),prio);};
                                          break;
                              case EXCA : if((w->clase()==OMEGA) && (w->arma()<2)){
                                             w->nueva_arma(2); t_so=1;
                                             retira=TRUE; encola_mensaje("Saber",w->color(1),prio);};
                                          break;
                              case FALC : if((w->clase()==OMEGA) && (w->arma()<3)){
                                             w->nueva_arma(3); t_so=1;
                                             retira=TRUE; encola_mensaje("Sword",w->color(1),prio);};
                                          break;
                              case MAZA : if((w->clase()==OMEGA) && (w->arma()<4)){
                                             w->nueva_arma(4); t_so=1;
                                             retira=TRUE; encola_mensaje("Maul",w->color(1),prio);};
                                          break;
                              case MART : if((w->clase()==OMEGA) && (w->arma()<5)){
                                             w->nueva_arma(5); t_so=1;
                                             retira=TRUE; encola_mensaje("Heavenly Hammer",w->color(1),prio);};
                                          break;
                              case AGUI : if((w->clase()==OMEGA) && (w->arma()<6)){
                                             w->nueva_arma(6); t_so=1;
                                             retira=TRUE; encola_mensaje("\"Golden Eagle\"",w->color(1),prio);};
                                          break;
                              case EDGE : if((w->clase()==OMEGA) && (w->arma()<7)){
                                             w->nueva_arma(7); t_so=1;
                                             retira=TRUE; encola_mensaje("\"Justice Edge\"",w->color(1),prio);};
                                          break;
                              case EMAD : if((w->clase()==OMEGA) && (w->escudo()<1)){
                                             w->nuevo_escudo(1); t_so=1;
                                             retira=TRUE; encola_mensaje("Leather shield",w->color(1),prio);};
                                          break;
                              case ECOB : if((w->clase()==OMEGA) && (w->escudo()<2)){
                                             w->nuevo_escudo(2); t_so=1;
                                             retira=TRUE; encola_mensaje("Wood shield",w->color(1),prio);};
                                          break;
                              case EACE : if((w->clase()==OMEGA) && (w->escudo()<3)){
                                             w->nuevo_escudo(3); t_so=1;
                                             retira=TRUE; encola_mensaje("Steel shield",w->color(1),prio);};
                                          break;
                              case EBRO : if((w->clase()==OMEGA) && (w->escudo()<4)){
                                             w->nuevo_escudo(4); t_so=1;
                                             retira=TRUE; encola_mensaje("Bronze shield",w->color(1),prio);};
                                          break;
                              case EPLA : if((w->clase()==OMEGA) && (w->escudo()<5)){
                                             w->nuevo_escudo(5); t_so=1;
                                             retira=TRUE; encola_mensaje("Silver shield",w->color(1),prio);};
                                          break;
                              case EORO : if((w->clase()==OMEGA) && (w->escudo()<6)){
                                             w->nuevo_escudo(6); t_so=1;
                                             retira=TRUE; encola_mensaje("Gold shield",w->color(1),prio);};
                                          break;
                              case EDIV : if((w->clase()==OMEGA) && (w->escudo()<7)){
                                             w->nuevo_escudo(7); t_so=1;
                                             retira=TRUE; encola_mensaje("\"The Protector\"",w->color(1),prio);};
                                          break;
                              case WALL : w->obtiene_special(INVUL,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("INVULNERABILITY",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case MFUE : if((w->clase()==BETA) && (w->municion(0)<w->municion_maxima(0))){
                                             t_so=5;
                                             w->nueva_municion(0,w->municion(0)+20);
                                             if(w->municion(0)>w->municion_maxima(0)) w->nueva_municion(0,w->municion_maxima(0));
                                             retira=TRUE; encola_mensaje("Fire energy +20",w->color(1),prio);};
                                          break;
                              case MHIE : if((w->clase()==BETA) && (w->municion(1)<w->municion_maxima(1))){
                                             t_so=5;
                                             w->nueva_municion(1,w->municion(1)+20);
                                             if(w->municion(1)>w->municion_maxima(1)) w->nueva_municion(1,w->municion_maxima(1));
                                             retira=TRUE; encola_mensaje("Water energy +20",w->color(1),prio);};
                                          break;
                              case MELE : if((w->clase()==BETA) && (w->municion(2)<w->municion_maxima(2))){
                                             t_so=5;
                                             w->nueva_municion(2,w->municion(2)+20);
                                             if(w->municion(2)>w->municion_maxima(2)) w->nueva_municion(2,w->municion_maxima(2));
                                             retira=TRUE; encola_mensaje("Lightning energy +20",w->color(1),prio);};
                                          break;
                              case APOC : if((w->clase()==BETA) && (!w->tiene_arma(10))){
                                             w->nueva_arma(10);
                                             retira=TRUE; encola_mensaje("Book of Apocalypse",w->color(1),prio);};
                                          break;
                              case BERS : w->obtiene_special(BERSERK,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("MYSTICAL STRENGHT",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case REPU : w->obtiene_special(FEAR,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("REPULSION",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case INVI : w->obtiene_special(INVIS,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("INVISIBILITY",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case SHAD : w->obtiene_special(SHADOW,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("FLUID OF SHADOWS",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case HEAL : w->nueva_salud(w->salud_maxima()*2);
                                          retira=TRUE; encola_mensaje("MAXIMUM HEALTH",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case MAPA : desvela_mapa();
                                          retira=TRUE; encola_mensaje("Full zone map",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case VENE : w->danyar(15,DVENE,NULL);
                                          retira=TRUE; encola_mensaje("Poison!",w->color(1),prio);
                                          break;
                              case BOLT : bolt(w->fx_pos(),w->fy_pos(),12,w->bando(),50,NULL);
                                          t_so=7;
                                          retira=TRUE; encola_mensaje("Electric Bolt!",w->color(1),prio);
                                          break;
                              case DIOS : w->nueva_salud(w->salud_maxima()*2);
                                          w->obtiene_special(INVUL,WTSPECIAL);
                                          w->obtiene_special(FEAR,WTSPECIAL);
                                          w->obtiene_special(BERSERK,WTSPECIAL);
                                          retira=TRUE; encola_mensaje("GOD'S PROTECTION",w->color(1),prio);
                                          t_so=6;
                                          break;
                              case GEM1 : if(gema==FALSE){
                                             t_so=4;
                                             gema=TRUE;
                                             retira=TRUE; encola_mensaje("Gem of Fire",w->color(1),prio);};
                                          break;
                              case GEM2 : if(gema==FALSE){
                                             t_so=4;
                                             gema=TRUE;
                                             retira=TRUE; encola_mensaje("Gem of Earth",w->color(1),prio);};
                                          break;
                              case GEM3 : if(gema==FALSE){
                                             t_so=4;
                                             gema=TRUE;
                                             retira=TRUE; encola_mensaje("Gem of Water",w->color(1),prio);};
                                          break;
                           };
                           if(retira)
                                if(tipoj==DMCH) niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->recoge();
                                else niv->retira_item(niv->primer_item(w->x_pos(),w->y_pos()));

                                switch(t_so){
                                    case 1 : sonido_ambiente(S_ITES,w->fx_pos(),w->fy_pos()); break;
                                    case 2 : sonido_ambiente(S_ITBO,w->fx_pos(),w->fy_pos()); break;
                                    case 3 : sonido_ambiente(S_ITMU,w->fx_pos(),w->fy_pos()); break;
                                    case 4 : sonido_ambiente(S_ITLL,w->fx_pos(),w->fy_pos()); break;
                                    case 5 : sonido_ambiente(S_ITEN,w->fx_pos(),w->fy_pos()); break;
                                    case 6 : sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos()); break;
                                    case 7 : sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos()); break;
                                    };
                        };

                        if(!cont->boton(0)) w->set_release(TRUE);
                        //Atacar
                        if(w->special(BERSERK)) f=3; else f=1;
                        switch(w->clase()){

                           case SIGMA : if((cont->boton(0)) && (w->release_boton()==TRUE) && (w->contador()==0) &&
                                             ((dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)) || (w->arma()==0)))){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Pu¤etazo
                                          case 0 : w->nuevo_estado(PUNY);
                                                   if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                                                      sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
                                                   break;

                                          // Pistola
                                          case 1 : w->nuevo_estado(SHOT);
                                                   if((w->municion(0)>=1) || (!pl)){
                                                      ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.2*cb,0,BULL,(word)(5*co*f),w);
                                                      if(pl) w->nueva_municion(0,w->municion(0)-1);
                                                      sonido_ambiente(S_PIST,w->fx_pos(),w->fy_pos());
                                                   } else encola_mensaje("No bullets left",w->color(1),prio);
                                                   break;

                                          // Ametralladora
                                          case 2 : w->nuevo_estado(SHOT);
                                                   if((w->municion(0)>=3) || (!pl)){
                                                      ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f),w);
                                                      ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f),w);
                                                      if(pl) w->nueva_municion(0,w->municion(0)-3);
                                                      sonido_ambiente(S_AMET,w->fx_pos(),w->fy_pos());
                                                   } else encola_mensaje("You need 3 bullets",w->color(1),prio);
                                                   break;

                                          // Lanzacohetes
                                          case 3 : w->nuevo_estado(SHOT);
                                                   if((w->municion(1)>=1) || (!pl)){
                                                      ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.09*cb,0.003,ROCK,(word)(16*co*f),w);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                      sonido_ambiente(S_LANZ,w->fx_pos(),w->fy_pos());
                                                   } else encola_mensaje("No rockets left",w->color(1),prio);
                                                   break;

                                          // Aniquilador
                                          case 4 : w->nuevo_estado(SHOT);
                                                   if((w->municion(1)>=2) || (!pl)){
                                                      ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(32*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(32*co*f),w);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-2);
                                                      sonido_ambiente(S_ANIQ,w->fx_pos(),w->fy_pos());
                                                   } else encola_mensaje("You need 2 rockets",w->color(1),prio);
                                                   break;

                                          // "Drag¢n"
                                          case 5 : w->nuevo_estado(SHOT);
                                                   if((w->municion(2)>=5) || (!pl)){
                                                      ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.11*cb,0,FUEG,(word)(24*co*f),w);
                                                      if(pl) w->nueva_municion(2,w->municion(2)-5);
                                                      sonido_ambiente(S_DRAG,w->fx_pos(),w->fy_pos());
                                                   } else encola_mensaje("You need 5 on napalm",w->color(1),prio);
                                                   break;

                                          // "Inferno"
                                          case 6 : w->nuevo_estado(SHOT);
                                                   if((w->municion(2)>=15) || (!pl)){
                                                      ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.075*cb,0,FUEG,(word)(48*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.075*cb,0,FUEG,(word)(48*co*f),w);
                                                      sonido_ambiente(S_INFE,w->fx_pos(),w->fy_pos());
                                                      if(pl) w->nueva_municion(2,w->municion(2)-15);
                                                   } else encola_mensaje("You need 15 on napalm",w->color(1),prio);
                                                   break;

                                          // Pistola de descargas
                                          case 7 : w->nuevo_estado(SHOT);
                                                   if(((w->municion(0)>=1) &&(w->municion(1)>=1)&&(w->municion(2)>=1)) || (!pl)){
                                                      ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(15*co*f),w);
                                                      sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos());
                                                      if(pl) {w->nueva_municion(0,w->municion(0)-1);
                                                              w->nueva_municion(1,w->municion(1)-1);
                                                              w->nueva_municion(2,w->municion(2)-1);};
                                                   } else encola_mensaje("You need 1 bullet, 1 rocket and 1 on napalm",w->color(1),prio);
                                                   break;
                                        };
                                        break;

                           case OMEGA : if((cont->boton(0)) && (w->release_boton()==TRUE) && (w->contador()==0)){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Pu¤etazo
                                          case 0 : w->nuevo_estado(PUNY);
                                                   if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                                                      sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
                                                   break;
                                          // Sablazo
                                          default: w->nuevo_estado(SLAS);
                                                   sonido_ambiente(S_ESPA,w->fx_pos(),w->fy_pos());
                                                   break;
                                        };};
                                        break;

                           case BETA  : if((cont->boton(0)) && (w->contador()==0) && (w->release_boton()==TRUE)){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Bastonazo
										case 0 : if(w->tiene_baston_mago())
												 {
													w->nuevo_estado(SLAS);
													sonido_ambiente(S_ESPA,w->fx_pos(),w->fy_pos());
												 }
												 else
												 {
													w->nuevo_estado(PUNY);
													if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                                                      sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
												 }
                                                 break;
                                          // Bola de fuego
                                          case 1 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=1) || (!pl)){
                                                        ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,FUEG,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(0,w->municion(0)-1);
                                                        sonido_ambiente(S_DRAG,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("No fire energy left",w->color(1),prio);
                                                   break;
                                          // Repulsi¢n temporal
                                          case 2 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=5) || (!pl)){
                                                        w->obtiene_special(FEAR,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(0,w->municion(0)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points on fire energy",w->color(1),prio);
                                                   break;
                                          // Pared de fuego
                                          case 3 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=10) || (!pl)){
                                                        if((w->dir()==DARR) || (w->dir()==DABA))
                                                        pared_de_fuego(w->front_x(1.0),w->front_y(1.0),8,(word)(25*co*f),TRUE,w);
                                                        else
                                                        pared_de_fuego(w->front_x(1.0),w->front_y(1.0),8,(word)(25*co*f),FALSE,w);
                                                        if(pl) w->nueva_municion(0,w->municion(0)-10);
                                                   }else encola_mensaje ("You need 10 points of fire energy",w->color(1),prio);
                                                   break;
                                          // Bola de hielo
                                          case 4 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=1) || (!pl)){
                                                        ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,HIEL,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                        sonido_ambiente(S_HIEL,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("No water energy left",w->color(1),prio);
                                                   break;
                                          // Invulnerabilidad temporal
                                          case 5 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=5) || (!pl)){
                                                        w->obtiene_special(INVUL,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(1,w->municion(1)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points of water energy",w->color(1),prio);
                                                   break;
                                          // Barrera de hielo
                                          case 6 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=10) || (!pl)){
                                                   dev_mapa()->anyade_barrera((bald)w->front_x(1.0),(bald)w->front_y(1.0),1);
                                                        if(pl) w->nueva_municion(1,w->municion(1)-10);
                                                        sonido_ambiente(S_EXHI,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 10 points of water energy",w->color(1),prio);
                                                   break;
                                          // Rayo el‚ctrico
                                          case 7 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=1) || (!pl)){
                                                        ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(2,w->municion(2)-1);
                                                        sonido_ambiente(S_RAYO,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("No lightning energy left",w->color(1),prio);
                                                   break;
                                          // Invisibilidad temporal
                                          case 8 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=5) || (!pl)){
                                                        w->obtiene_special(INVIS,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(2,w->municion(2)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points of lightning energy",w->color(1),prio);
                                                   break;
                                          // Descarga el‚ctrica
                                          case 9 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=10) || (!pl)){
                                                        bolt(w->fx_pos(),w->fy_pos(),(co*6),w->bando(),(word)(15*co*f),w);
                                                        if(pl) w->nueva_municion(2,w->municion(2)-10);
                                                        sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 10 points of lightning energy",w->color(1),prio);

                                                   break;

                                          // Apocalipsis
                                          case 10 : apocalipsis(w->fx_pos(),w->fy_pos(),10,(word)(30*co*f),w);
                                                    sonido_ambiente(S_APOC,w->fx_pos(),w->fy_pos());
                                                    w->nuevo_estado(HECH);
                                                    if(pl){
                                                           if(w->salud()<15) w->danyar(30,DAPOC,w);
                                                           else w->nueva_salud(w->salud()-15);

                                                    };
                                                   break;
                                          };};
                        };};
                        break;

            case HECH :
            case SLAS :
            case SHOT :
            case PUNY : nx=w->fx_pos(); ny=w->fy_pos();
                        if(cont->arr()) {ny-=WPASO; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        if(w->special(BERSERK)) f=3; else f=1;

                        //Danya por pu¤etazo
                        if((w->contador()==5) && (w->estado()==PUNY)){
                            //Personajes
                            sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),0.7,10,FALSE);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(5*f*co))){
                                sec->danyar((word)(5*co*f),DPUNY,w);
                                sonido_warrior(sec,0);
                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());

                            // Barreras
                            if((i=mp->hay_barr(w->front_x(0.5),w->front_y(0.5)))>=0)
                                mp->dev_barr(i)->danyar((word)(5*f*3*co));
                        };
                        //Da¤a por golpe de espada
                        if((w->contador()==WTSLAS-EXPTIME) && (w->estado()==SLAS)){

                            if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                               sonido_ambiente(S_CORT,w->fx_pos(),w->fy_pos());
                            //Personajes
                            sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),1.0,10,FALSE);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(co*(5+5*f*w->arma())))){
                                sec->danyar((word)(co*(5+5*f*co*w->arma())),DESPA,w);
                                sonido_warrior(sec,0);
                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());
                            // Barreras
                            if((i=mp->hay_barr(w->front_x(0.5),w->front_y(0.5)))>=0)
                                mp->dev_barr(i)->danyar((word)(co*(5+15*f*w->arma())));
                         };
                        break;

            case DEFE : nx=w->fx_pos(); ny=w->fy_pos();
                        if(cont->arr()) {ny-=WPASO; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        //Deja de cubrirse
                        if(!cont->boton(2)) w->nuevo_estado(WALK);

                        // Da¤a si hay explosi¢n que no puede cubrir
                        if(((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0) && (w->contador()==0)){
                                if(danyo_total_explos(w->fx_pos(),w->fy_pos(),1.0)>5*co*w->escudo()){
                                        w->nuevo_estado(WALK);
                                        sonido_warrior(w,0);
                                    };
                              };
                        break;

            case TELP : w->actualiza(FALSE);
                        if(w->contador()==WTTEL/2){
                        i=mp->hay_telep(w->x_pos(),w->y_pos());
                        x=mp->dev_telep(i)->x_out()+0.5;
                        y=mp->dev_telep(i)->y_out()+0.5;
                        // Telefragg!!!
                        i=0;
                        while((i=hay_player(x,y,i))>=0){
                              sec=dev_player(i); if(sec!=w) sec->danyar(300,DTELE,w);
                              i++;
                        };
                        if((i=hay_enem(x,y,0))>=0){
                                sec=dev_enem(i); sec->danyar(300,DTELE,w); };
                        w->nueva_pos(x,y);};
                        break;

            case DIE  : w->actualiza(FALSE);
                        if((cont->algun_boton()) && (w->contador()==WTCAE) && (pl))
                                reset_warrior(w);
                        if(w->contador()==WDOLWAIT){
                              // Mensajes al morir
                              if(w->agresor()!=NULL){
                                // Suicidio
                                if(w->agresor()==w) sprintf(s,"%s kills itself.",w->nombre());
                                else{
                                  switch(w->causa_danyo()){
                                    case DPUNY : sprintf(aux,"beaten"); break;
                                    case DESPA : sprintf(aux,"slain"); break;
                                    case DBALA : sprintf(aux,"shot"); break;
                                    case DCOHE : sprintf(aux,"blasted"); break;
                                    case DFUEG : sprintf(aux,"burned"); break;
                                    case DELEC : sprintf(aux,"electrocuted"); break;
                                    case DTELE : sprintf(aux,"telefragged"); break;
                                    case DHIEL : sprintf(aux,"frozen"); break;
                                    };
                                sprintf(s,"%s was %s by %s.",w->nombre(),aux,w->agresor()->nombre());
                                if(tipoj==DMCH) w->agresor()->incr_muertes();
                                if(w->bando()!=w->agresor()->bando()) w->agresor()->gana_exp(w->level());
                                };
                              }else{
                              switch(w->causa_danyo()){
                                case DSUEL : sprintf(s,"%s falled into a pit.",w->nombre()); break;
                                case DLASE : sprintf(s,"%s was killed by a laser.",w->nombre()); break;
                                case DVENE : sprintf(s,"%s was killed by a poison.",w->nombre()); break;
                                default    : sprintf(s,"%s was killed by a trap.",w->nombre()); break;
                                };
                              };

                              it=w->item_asociado();
                              if(it!=NING)
                                 ingresa_item_util(w->x_pos(),w->y_pos(),it);
                              if(pl){ encola_mensaje(s,w->color(1),allprio);
                                      encola_mensaje("Push any button to restart",w->color(1),prio);}
                              else encola_mensaje_consola(s,15);
                              sonido_warrior(w,1);};
                        break;

            case DEST  : w->actualiza(FALSE);
                        if((cont->algun_boton()) && (w->contador()==WTCAE) && (pl))
                                reset_warrior(w);
                        if(w->contador()==WDOLWAIT){
                              // Mensajes al morir
                              if(w->agresor()!=NULL){
                                // Suicidio
                                if(w->agresor()==w) sprintf(s,"%s destroys itself.",w->nombre());
                                else{
                                     switch(w->causa_danyo()){
                                        case DPUNY : sprintf(aux,"destroyed"); break;
                                        case DESPA : sprintf(aux,"eviscerated"); break;
                                        case DBALA : sprintf(aux,"riddled"); break;
                                        case DCOHE : sprintf(aux,"exploded"); break;
                                        case DFUEG : sprintf(aux,"roasted"); break;
                                        case DELEC : sprintf(aux,"electrocuted"); break;
                                        case DTELE : sprintf(aux,"telefragged"); break;
                                        case DHIEL : sprintf(aux,"frozen"); break;
                                        case DAPOC : sprintf(aux,"charmed"); break;
                                     };
                                     sprintf(s,"%s was %s by %s.",w->nombre(),aux,w->agresor()->nombre());
                                    if(tipoj==DMCH) w->agresor()->incr_muertes();
                                    if(w->bando()!=w->agresor()->bando()) w->agresor()->gana_exp(w->level());
                                };
                              }else{
                              switch(w->causa_danyo()){
                                case DLASE : sprintf(s,"%s was killed by a laser.",w->nombre()); break;
                                case DVENE : sprintf(s,"%s was killed by a poison.",w->nombre()); break;
                                default    : sprintf(s,"%s was destroyed by a trap.",w->nombre()); break;
                                };
                              };
                              it=w->item_asociado();
                              if(it!=NING)
                                 ingresa_item_util(w->x_pos(),w->y_pos(),it);
                              if(pl){ encola_mensaje(s,w->color(1),allprio);
                                      encola_mensaje("Push any button to restart",w->color(1),prio);}
                              else encola_mensaje_consola(s,15);
                              sonido_warrior(w,2);};
                        break;

            case CONG  : w->actualiza(FALSE);
                         if(w->contador()==WTCONG-EXPTIME)
                           ingresa_explos(w->fx_pos(),w->fy_pos(),EHIE,1,w);

                        // Da¤a o descongela por balas
                        if((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0){
                              if(dev_explos(i)->tipo()==ELLA)
                                   w->nuevo_estado(WALK);
                              else{
                                if(bcont%WDOLWAIT==0) w->danyar(5,DHIEL,dev_explos(i)->creador());
                              };
                        }; break;
                        break;

            case OUTL  : w->actualiza(FALSE); break;
      };
}

void batalla::envia_danyo_red(byte c, cdanyo tipo, word cantidad, byte tipo_oponente, byte id_oponente, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	TMsgDamage *lMsgDmg = DBG_NEW TMsgDamage;

	lMsgDmg->mType		= eMsgDamage;
	lMsgDmg->mSender	= getMaskFromPlayerId(c);
	lMsgDmg->mReceiver	= MASK_SERVER;
	lMsgDmg->mSourceType= OPPLAYER;
	lMsgDmg->mTargetType= tipo_oponente;
	lMsgDmg->mSourceId	= c;
	lMsgDmg->mTargetId	= id_oponente;
	lMsgDmg->mDamType	= tipo;
	lMsgDmg->mAmount	= cantidad;

	if(modo_red == MRSERVER)
		aRecQueue->queueMessage(lMsgDmg);
	else
		aSendQueue->queueMessage(lMsgDmg);
}
void batalla::incrementa_muertes_red(byte player, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
	TMsgIncreaseKills *lMsg = DBG_NEW TMsgIncreaseKills;

	lMsg->mType = eMsgIncreaseKills;
	lMsg->mSender = getMaskFromPlayerId(jug_local);
	lMsg->mReceiver = MASK_SERVER;
	lMsg->mPlayerId = player;

	if(modo_red == MRSERVER)
		aRecQueue->queueMessage(lMsg);
	else
		aSendQueue->queueMessage(lMsg);
}
void batalla::actualiza_player_red_local(byte c, warrior *w, controlm *cont, bool pl, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
      warrior *sec;
      disparador *dis;
      t_item it;
      w_dir d;
      int i, t_so=0;
      bool mover=FALSE, retira=FALSE;
      float nx=w->fx_pos(), ny=w->fy_pos(),x,y,f=1, co=w->level_coef(), cb=0.95+(0.05*w->level());
      mapa *mp=dev_mapa();
      char s[200], aux[20];
      byte prio, allprio=PRIO1|PRIO2|PRIO3|PRIO4|PRIO5|PRIO6|PRIO7|PRIO8;
      cdanyo cd;

      switch(c){
                case 0 : prio=PRIO1; break;
                case 1 : prio=PRIO2; break;
                case 2 : prio=PRIO3; break;
                case 3 : prio=PRIO4; break;
				case 4 : prio=PRIO5; break;
                case 5 : prio=PRIO6; break;
                case 6 : prio=PRIO7; break;
                case 7 : prio=PRIO8; break;};

        // Sube un nivel si exp. suficiente
        /*if(pl)
        if((w->exp()>=exp_level(w->level()+1)) && (w->level()<WMAXLEV))
                {w->sube_level();
                 w->nueva_salud(w->salud_maxima());
                 sprintf(s,"%s reaches level %d.",w->nombre(),w->level());
                 encola_mensaje(s,w->color(1),allprio);
                 sonido_ambiente(S_LEVE,w->fx_pos(),w->fy_pos());
                 };*/

      // Abre puertas que no req. interruptor
      i=mp->hay_puerta((bald)w->front_x(0.5),(bald)w->front_y(0.5));
      if(i>=0){
				bool lAbrir = false;
                switch(mp->dev_puerta(i)->tipo()){
					case NORM : lAbrir = true; break;
					case LLA1 : if(w->posee_llave(0)) lAbrir = true; break;
					case LLA2 : if(w->posee_llave(1)) lAbrir = true; break;
					case LLA3 : if(w->posee_llave(2)) lAbrir = true; break;
					case LLA4 : if(w->posee_llave(3)) lAbrir = true; break;
					case GEMA : if(gema) lAbrir = true; break;
                };
				if(!mp->dev_puerta(i)->abriendo() && lAbrir && puerta_red_espera == 0)
				{
					// Pide al server que abra la puerta
					TMsgOpenDoor *lMsg = DBG_NEW TMsgOpenDoor;
					lMsg->mType = eMsgOpenDoor;
					lMsg->mDoorId = (tbyte)i;
					lMsg->mReceiver = MASK_SERVER;
					lMsg->mSender = getMaskFromPlayerId(c);
					if(modo_red == MRSERVER)
						aRecQueue->queueMessage(lMsg);
					else
						aSendQueue->queueMessage(lMsg);

					puerta_red_espera = 30;
				}
      };
	  if(puerta_red_espera > 0) puerta_red_espera--;
      // DA¥O
      if((w->estado()!=DEST) && (w->estado()!=DIE) && (w->estado()!=OUTL) && (w->estado()!=CONG) && (w->estado()!=TELP) && (w->dios()==FALSE)){

                        // Da¤a si caminando por baldosa da¤ina
                        switch(mp->consb(w->x_pos(),w->y_pos())){
                           case BDA1:
                           case BDA2: if((w->contador()==0) &&
                                      (mp->hay_puente(w->x_pos(),w->y_pos())<0) && (!w->special(INVUL))){
                                             w->danyar(5,DSUEL,NULL);
                                             sonido_warrior(w,0);};
                                             break;
                        };

                        // Da¤a si caminando por baldosa mortal
                        switch(mp->consb(w->x_pos(),w->y_pos())){
                           case BMO1:
                           case BMO2: if((w->contador()==0) &&
                                      (mp->hay_puente(w->x_pos(),w->y_pos())<0) && (!w->special(INVUL))){
                                             w->danyar(1,DSUEL,NULL);
                                             w->nuevo_estado(DIE); }; break;
                        };
                        // Da¤a si hay explosi¢n
                        if((w->estado()!=DEFE) && ((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0) &&
                           ((w->contador()==0) || (w->estado()!=WALK))
                           && (!w->special(INVUL))){
                              if(dev_explos(i)->tipo()==EHIE)
                                   {w->nuevo_estado(CONG);}
                              else {
                                    switch(dev_explos(i)->tipo()){
										case EBAL: cd = DBALA; break;
                                        case ECOH : cd=DCOHE; break;
										case ELL3 :
										case ELL2 :
										case ELLA :
                                        case EFUE : cd=DFUEG; break;
                                        case EHIE : cd=DHIEL; break;
                                        case EELE : cd=DELEC; break;
                                        default : cd=DFUEG; break;
                                        };
                                    w->danyar(danyo_total_explos(w->fx_pos(),w->fy_pos(),1.0),cd,dev_explos(i)->creador());
                                    if(dev_explos(i)->tipo()==EELE) w->electrocutar();
                                    sonido_warrior(w,0);};
                              };
                        // Da¤a si hay barrera l ser
                        if((mp->hay_laser(w->fx_pos(),w->fy_pos())>=0) && ((w->contador()==0) || (w->estado()!=WALK))){
                           w->danyar(45,DLASE,NULL);
                           sonido_warrior(w,0);};

                        // Da¤o por electrocutado
                        if((w->electrocutado()==TRUE) && ((bcont%30)==0))
                        if(w->salud()>0) w->nueva_salud(w->salud()-1);
                        else             w->nuevo_estado(DIE);

      };
      // Hotkeys para armas
      //if(c==jug_local)
        for(i=0; i<11; i++)
                 if(cont->tecla_numero(i))
                        if(w->set_arma(i)==FALSE) encola_mensaje("Out of weapon!",w->color(1),prio);

      switch(w->estado()){

            case WALK : if(cont->arr()) {ny-=WPASO*co; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO*co; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO*co; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO*co; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        // Teletransporta, si hay teleport
                        if(mp->hay_telep((bald)nx,(bald)ny)>=0){
                              w->nueva_pos(float((bald)nx+0.5),float((bald)ny+0.5));
                              w->nuevo_estado(TELP);
                              sonido_ambiente(S_TELE,w->fx_pos(),w->fy_pos());
                              break;};

                        // Cambio de arma
                        if((cont->boton(2)) && (w->clase()!=OMEGA))
                           w->cambia_arma();

                        // Activa un interruptor del mapa
                        i=mp->hay_interr((bald)w->front_x(0.5),(bald)w->front_y(0.5));
                        if((cont->boton(3)) && (i>=0) && (mp->dev_interr(i)->permite()) && (interr_red_espera == 0)){
                                              //mp->activa_interr(i);
                                              //// Disparadores
                                              //if(mp->dev_interr(i)->tobj()==DISP){
                                              //     dis=mp->dev_disp(mp->hay_disp(mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy()));
                                              //     ingresa_bala(dis->x_pos(),dis->y_pos(),dis->dir(),0.2,0,(b_tipo)dis->t_bala(),5,NULL);
                                              //     sonido_ambiente(S_TRAM,mp->dev_interr(i)->cobjx(),mp->dev_interr(i)->cobjy());};
                                              //sonido_ambiente(S_INTER,mp->dev_interr(i)->x_pos(),mp->dev_interr(i)->y_pos());

							TMsgTurnSwitch *lMsgSwitch = DBG_NEW TMsgTurnSwitch;

							lMsgSwitch->mType = eMsgTurnSwitch;
							lMsgSwitch->mSender = getMaskFromPlayerId(c);
							lMsgSwitch->mReceiver = MASK_SERVER;
							lMsgSwitch->mSwitchId = i;

							if(modo_red == MRSERVER)
								aRecQueue->queueMessage(lMsgSwitch);
							else
								aSendQueue->queueMessage(lMsgSwitch);

							interr_red_espera = IESPERA;
                        };
						if (interr_red_espera > 0) interr_red_espera--;

                        // Interactua con un pers. secundario
                        if(cont->boton(3))
                        if((i=hay_enem((bald)w->front_x(1.0),(bald)w->front_y(1.0),0))>=0){
                                sec=dev_enem(i);
                                if((sec->bando()==w->bando()) || (sec->clase()==CASUAL)){

                                    TMsgTalkToChar *lMsgTalk = DBG_NEW TMsgTalkToChar;

                                    lMsgTalk->mType = eMsgTalkToChar;
                                    lMsgTalk->mSender = getMaskFromPlayerId(c);
                                    lMsgTalk->mReceiver = MASK_SERVER;
                                    lMsgTalk->mPlayerId = c;
                                    lMsgTalk->mEnemyId = i;

                                    if (modo_red == MRSERVER)
                                        aRecQueue->queueMessage(lMsgTalk);
                                    else
                                        aSendQueue->queueMessage(lMsgTalk);


                        //           // Suelta un ¡tem
                        //           it=sec->item_asociado();
                        //           // Se gira hacia el jugador y pronuncia su frase
                        //           sec->nuevo_oponente(w);
                        //           sprintf(s,"%s:%s",sec->nombre(),sec->frase());
                        //           if((sec->interactuar()==TRUE) && (sec->estado()<TELP)){
                        //              if(sec->dios()==TRUE) encola_mensaje(s,64,prio);
                        //              else          encola_mensaje(s,15,prio);
                        //              if(it!=NING)
                        //              ingresa_item_util(sec->front_x(0.75),sec->front_y(0.75),it);
                        //           };

                                };
                        };
                        //i=-1;

                        // Abre puerta si estamos justo en el umbral
                       /* if((i=mp->hay_puerta(w->x_pos(),w->y_pos()))>=0)
                              mp->abre_puerta(i);*/

                        // Descubre  reas secretas
                        /*if(pl)
                        if((i=mp->hay_secr(w->x_pos(),w->y_pos()))>=0){
                             mp->retira_secr(i);
                             encola_mensaje("Found a secret area!",w->color(1),prio);
                             sonido_ambiente(S_SECR,w->fx_pos(),w->fy_pos());
                             };*/

                        // Salir de la zona
                        if(mp->es_final(w->x_pos(),w->y_pos())){
                           w->nuevo_estado(OUTL);
                           sonido_ambiente(S_TELE,w->fx_pos(),w->fy_pos());
                           sprintf(s,"%s gets out of the zone.",w->nombre());
                           if(pl) encola_mensaje_red(s,w->color(1),allprio,aRecQueue,aSendQueue); break;};

                        // Cubrirse con el escudo
                        if((cont->boton(2)) && (w->clase()==OMEGA) && (w->contador()==0) && (w->escudo()>0))
                           w->nuevo_estado(DEFE);

                        // Recoger items
                        if(pl)
                        if(niv->hay_item(w->x_pos(),w->y_pos())){
                           t_so=0;
                           if(niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->es_cogible())
                           switch(niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->tipo()){
                              case BOTI : if(w->salud()<w->salud_maxima()){
                                             retira=TRUE; };
                                          break;
                              case KEY1 : if(w->posee_llave(0)==FALSE){
											retira=TRUE;
										  };
                                          break;
                              case KEY2 : if(w->posee_llave(1)==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                              case KEY3 : if(w->posee_llave(2)==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                              case KEY4 : if(w->posee_llave(3)==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                              case PIST : if((w->clase()==SIGMA) && (!w->tiene_arma(1))){
                                            retira=TRUE;
										  };
                                          break;
                              case AMET : if((w->clase()==SIGMA) && (!w->tiene_arma(2))){
                                            retira=TRUE;
										  };
                                          break;
                              case LANZ : if((w->clase()==SIGMA) && (!w->tiene_arma(3))){
                                            retira=TRUE;
										  };
                                          break;
                              case ANIQ : if((w->clase()==SIGMA) && (!w->tiene_arma(4))){
                                            retira=TRUE;
										  };
                                          break;
                              case DRAG : if((w->clase()==SIGMA) && (!w->tiene_arma(5))){
                                            retira=TRUE;
										  };
                                          break;
                              case INFE : if((w->clase()==SIGMA) && (!w->tiene_arma(6))){
                                            retira=TRUE;
										  };
                                          break;
                              case SHOC : if((w->clase()==SIGMA) && (!w->tiene_arma(7))){
                                            retira=TRUE;
										  };
                                          break;
                              case BALA : if((w->clase()==SIGMA) && (w->municion(0)<w->municion_maxima(0))){
                                            retira=TRUE;
										  };
                                          break;
                              case COHE : if((w->clase()==SIGMA) && (w->municion(1)<w->municion_maxima(1))){
                                            retira=TRUE;
										  };
                                          break;
                              case NAPA : if((w->clase()==SIGMA) && (w->municion(2)<300)){
                                            retira=TRUE;
										  };
                                          break;
                              case DAGA : if((w->clase()==OMEGA) && (w->arma()<1)){
                                            retira=TRUE;
										  };
                                          break;
                              case EXCA : if((w->clase()==OMEGA) && (w->arma()<2)){
                                            retira=TRUE;
										  };
                                          break;
                              case FALC : if((w->clase()==OMEGA) && (w->arma()<3)){
                                            retira=TRUE;
										  };
                                          break;
                              case MAZA : if((w->clase()==OMEGA) && (w->arma()<4)){
                                            retira=TRUE;
										  };
                                          break;
                              case MART : if((w->clase()==OMEGA) && (w->arma()<5)){
                                            retira=TRUE;
										  };
                                          break;
                              case AGUI : if((w->clase()==OMEGA) && (w->arma()<6)){
                                            retira=TRUE;
										  };
                                          break;
                              case EDGE : if((w->clase()==OMEGA) && (w->arma()<7)){
                                            retira=TRUE;
										  };
                                          break;
                              case EMAD : if((w->clase()==OMEGA) && (w->escudo()<1)){
                                            retira=TRUE;
										  };;
                                          break;
                              case ECOB : if((w->clase()==OMEGA) && (w->escudo()<2)){
                                            retira=TRUE;
										  };
                                          break;
                              case EACE : if((w->clase()==OMEGA) && (w->escudo()<3)){
                                            retira=TRUE;
										  };
                                          break;
                              case EBRO : if((w->clase()==OMEGA) && (w->escudo()<4)){
                                            retira=TRUE;
										  };
                                          break;
                              case EPLA : if((w->clase()==OMEGA) && (w->escudo()<5)){
                                            retira=TRUE;
										  };
                                          break;
                              case EORO : if((w->clase()==OMEGA) && (w->escudo()<6)){
                                            retira=TRUE;
										  };
                                          break;
                              case EDIV : if((w->clase()==OMEGA) && (w->escudo()<7)){
                                            retira=TRUE;
										  };
                                          break;
                              case WALL :
                                          retira=TRUE;
                                          break;
                              case MFUE : if((w->clase()==BETA) && (w->municion(0)<w->municion_maxima(0))){
                                            retira=TRUE;
										  };
                                          break;
                              case MHIE : if((w->clase()==BETA) && (w->municion(1)<w->municion_maxima(1))){
                                            retira=TRUE;
										  };
                                          break;
                              case MELE : if((w->clase()==BETA) && (w->municion(2)<w->municion_maxima(2))){
                                            retira=TRUE;
										  };
                                          break;
                              case APOC : if((w->clase()==BETA) && (!w->tiene_arma(10))){
                                            retira=TRUE;
										  };
                                          break;
                              case BERS : {
                                            retira=TRUE;
										  };
                                          break;
                              case REPU : {
                                            retira=TRUE;
										  };
                                          break;
                              case INVI : {
                                            retira=TRUE;
										  };
                                          break;
                              case SHAD : {
                                            retira=TRUE;
										  };
                                          break;
                              case HEAL : {
                                            retira=TRUE;
										  };
                                          break;
                              case MAPA : {
                                            retira=TRUE;
										  };
                                          break;
                              case VENE : {
                                            retira=TRUE;
										  };
                                          break;
                              case BOLT : {
                                            retira=TRUE;
										  };
                                          break;
                              case DIOS : {
                                            retira=TRUE;
										  };
                                          break;
                              case GEM1 : if(gema==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                              case GEM2 : if(gema==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                              case GEM3 : if(gema==FALSE){
                                            retira=TRUE;
										  };
                                          break;
                           };
						   if(retira)
						   {
							   // Pide al servidor el item para mí
							   TMsgRequestItem *lMsgItem = DBG_NEW TMsgRequestItem;
							   lMsgItem->mType = eMsgRequestItem;
							   lMsgItem->mSender = getMaskFromPlayerId(c);
							   lMsgItem->mReceiver = MASK_SERVER;
							   lMsgItem->mPlayerId = c;
							   lMsgItem->mItemId = niv->primer_item(w->x_pos(),w->y_pos());
							   if(modo_red == MRSERVER)
									aRecQueue->queueMessage(lMsgItem);
								else
									aSendQueue->queueMessage(lMsgItem);
						   }
						}
                           /*if(retira)
                                if(tipoj==DMCH) niv->dev_item(niv->primer_item(w->x_pos(),w->y_pos()))->recoge();
                                else niv->retira_item(niv->primer_item(w->x_pos(),w->y_pos()));

                                switch(t_so){
                                    case 1 : sonido_ambiente(S_ITES,w->fx_pos(),w->fy_pos()); break;
                                    case 2 : sonido_ambiente(S_ITBO,w->fx_pos(),w->fy_pos()); break;
                                    case 3 : sonido_ambiente(S_ITMU,w->fx_pos(),w->fy_pos()); break;
                                    case 4 : sonido_ambiente(S_ITLL,w->fx_pos(),w->fy_pos()); break;
                                    case 5 : sonido_ambiente(S_ITEN,w->fx_pos(),w->fy_pos()); break;
                                    case 6 : sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos()); break;
                                    case 7 : sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos()); break;
                                    };
                        };*/

                        if(!cont->boton(0)) w->set_release(TRUE);
                        //Atacar
                        if(w->special(BERSERK)) f=3; else f=1;
                        switch(w->clase()){

                           case SIGMA : if((cont->boton(0)) && (w->release_boton()==TRUE) && (w->contador()==0) &&
                                             ((dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)) || (w->arma()==0)))){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Pu¤etazo
                                          case 0 : w->nuevo_estado(PUNY);
                                                   if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                                                      sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
                                                   break;

                                          // Pistola
                                          case 1 : w->nuevo_estado(SHOT);
                                                   if((w->municion(0)>=1) || (!pl)){
                                                      crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.2*cb,0,BULL,(word)(5*co*f),pl ? OPPLAYER : OPENEMY,c,S_PIST,aRecQueue,aSendQueue);
                                                      if(pl) w->nueva_municion(0,w->municion(0)-1);
                                                   } else encola_mensaje("No bullets left",w->color(1),prio);
                                                   break;

                                          // Ametralladora
                                          case 2 : w->nuevo_estado(SHOT);
                                                   if((w->municion(0)>=3) || (!pl)){
                                                      /*ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.2*cb,0,BULL,(word)(5*co*f),w);
                                                      ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.2*cb,0,BULL,(word)(5*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.2*cb,0,BULL,(word)(5*co*f),w);
                                                      if(pl) w->nueva_municion(0,w->municion(0)-3);
                                                      sonido_ambiente(S_AMET,w->fx_pos(),w->fy_pos());*/
													  crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f), pl ? OPPLAYER : OPENEMY,c,S_AMET,aRecQueue,aSendQueue);
													  crea_bala_red(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f), pl ? OPPLAYER : OPENEMY,c,S_NULO,aRecQueue,aSendQueue);
													  crea_bala_red(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.2*cb,0,BULL,(word)(15*co*f), pl ? OPPLAYER : OPENEMY,c,S_NULO,aRecQueue,aSendQueue);
													  if(pl) w->nueva_municion(0,w->municion(0)-3);
                                                   } else encola_mensaje("You need 3 bullets",w->color(1),prio);
                                                   break;

                                          // Lanzacohetes
                                          case 3 : w->nuevo_estado(SHOT);
                                                   if((w->municion(1)>=1) || (!pl)){
                                                      /*ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.09*cb,0.003,ROCK,(word)(16*co*f),w);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                      sonido_ambiente(S_LANZ,w->fx_pos(),w->fy_pos());*/
													  crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.09*cb,0.003,ROCK,(word)(16*co*f), pl ? OPPLAYER : OPENEMY,c,S_LANZ,aRecQueue,aSendQueue);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                   } else encola_mensaje("No rockets left",w->color(1),prio);
                                                   break;

                                          // Aniquilador
                                          case 4 : w->nuevo_estado(SHOT);
                                                   if((w->municion(1)>=2) || (!pl)){
                                                      /*ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(16*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(16*co*f),w);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-2);
                                                      sonido_ambiente(S_ANIQ,w->fx_pos(),w->fy_pos());*/
													  crea_bala_red(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(32*co*f), pl ? OPPLAYER : OPENEMY,c,S_ANIQ,aRecQueue,aSendQueue);
                                                      crea_bala_red(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.12*cb,0.0015,ROCK,(word)(32*co*f), pl ? OPPLAYER : OPENEMY,c,S_NULO,aRecQueue,aSendQueue);
                                                      if(pl) w->nueva_municion(1,w->municion(1)-2);
                                                   } else encola_mensaje("You need 2 rockets",w->color(1),prio);
                                                   break;

                                          // "Drag¢n"
                                          case 5 : w->nuevo_estado(SHOT);
                                                   if((w->municion(2)>=5) || (!pl)){
                                                      /*ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.11*cb,0,FUEG,(word)(24*co*f),w);
                                                      if(pl) w->nueva_municion(2,w->municion(2)-5);
                                                      sonido_ambiente(S_DRAG,w->fx_pos(),w->fy_pos());*/
													  crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.11*cb,0,FUEG,(word)(24*co*f), pl ? OPPLAYER : OPENEMY,c,S_DRAG,aRecQueue,aSendQueue);
                                                      if(pl) w->nueva_municion(2,w->municion(2)-5);
                                                   } else encola_mensaje("You need 5 on napalm",w->color(1),prio);
                                                   break;

                                          // "Inferno"
                                          case 6 : w->nuevo_estado(SHOT);
                                                   if((w->municion(2)>=15) || (!pl)){
                                                      /*ingresa_bala(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.075*cb,0,FUEG,(word)(24*co*f),w);
                                                      ingresa_bala(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.075*cb,0,FUEG,(word)(24*co*f),w);
                                                      sonido_ambiente(S_INFE,w->fx_pos(),w->fy_pos());
                                                      if(pl) w->nueva_municion(2,w->municion(2)-15);*/
													  crea_bala_red(w->front_x_2(1,1.0),w->front_y_2(1,1.0),w->dir(),0.075*cb,0,FUEG,(word)(48*co*f), pl ? OPPLAYER : OPENEMY,c,S_INFE,aRecQueue,aSendQueue);
                                                      crea_bala_red(w->front_x_2(2,1.0),w->front_y_2(2,1.0),w->dir(),0.075*cb,0,FUEG,(word)(48*co*f), pl ? OPPLAYER : OPENEMY,c,S_NULO,aRecQueue,aSendQueue);
													  if(pl) w->nueva_municion(2,w->municion(2)-15);
                                                   } else encola_mensaje("You need 15 on napalm",w->color(1),prio);
                                                   break;

                                          // Pistola de descargas
                                          case 7 : w->nuevo_estado(SHOT);
                                                   if(((w->municion(0)>=1) &&(w->municion(1)>=1)&&(w->municion(2)>=1)) || (!pl)){
                                                      /*ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(15*co*f),w);
                                                      sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos());
                                                      if(pl) {w->nueva_municion(0,w->municion(0)-1);
                                                              w->nueva_municion(1,w->municion(1)-1);
                                                              w->nueva_municion(2,w->municion(2)-1);};*/
													  crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(15*co*f), pl ? OPPLAYER : OPENEMY,c,S_BOLT,aRecQueue,aSendQueue);
                                                      if(pl) {w->nueva_municion(0,w->municion(0)-1);
                                                              w->nueva_municion(1,w->municion(1)-1);
                                                              w->nueva_municion(2,w->municion(2)-1);};
                                                   } else encola_mensaje("You need 1 bullet, 1 rocket and 1 on napalm",w->color(1),prio);
                                                   break;
                                        };
                                        break;

                           case OMEGA : if((cont->boton(0)) && (w->release_boton()==TRUE) && (w->contador()==0)){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Pu¤etazo
                                          case 0 : w->nuevo_estado(PUNY);
                                                   if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                                                      sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
                                                   break;
                                          // Sablazo
                                          default: w->nuevo_estado(SLAS);
                                                   sonido_ambiente(S_ESPA,w->fx_pos(),w->fy_pos());
                                                   break;
                                        };};
                                        break;

                           case BETA  : if((cont->boton(0)) && (w->contador()==0) && (w->release_boton()==TRUE)){
                                        w->set_release(FALSE);
                                        switch(w->arma()){
                                          // Bastonazo
										  case 0 : if(w->tiene_baston_mago())
												   {
														w->nuevo_estado(SLAS);
														sonido_ambiente(S_ESPA,w->fx_pos(),w->fy_pos());
												   }
												   else
												   {
													    w->nuevo_estado(PUNY);
														if(!permite(w->front_x(0.5),w->front_y(0.5),w))
															sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
												   }
                                                   break;
                                          // Bola de fuego
                                          case 1 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=1) || (!pl)){
                                                       /* ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,FUEG,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(0,w->municion(0)-1);
                                                        sonido_ambiente(S_DRAG,w->fx_pos(),w->fy_pos());*/
													   crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,FUEG,(word)(10*co*f), pl ? OPPLAYER : OPENEMY,c,S_DRAG,aRecQueue,aSendQueue);
                                                       if(pl) w->nueva_municion(0,w->municion(0)-1);
                                                   }else encola_mensaje ("No fire energy left",w->color(1),prio);
                                                   break;
                                          // Repulsi¢n temporal
                                          case 2 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=5) || (!pl)){
                                                        w->obtiene_special(FEAR,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(0,w->municion(0)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points on fire energy",w->color(1),prio);
                                                   break;
                                          // Pared de fuego
                                          case 3 : w->nuevo_estado(HECH);
                                                   if((w->municion(0)>=10) || (!pl)){
                                                        if((w->dir()==DARR) || (w->dir()==DABA))
															pared_de_fuego_red(w->front_x(1.0),w->front_y(1.0),8,(word)(25*co*f),TRUE, pl ? OPPLAYER : OPENEMY,c,aRecQueue,aSendQueue);
                                                        else
															pared_de_fuego_red(w->front_x(1.0),w->front_y(1.0),8,(word)(25*co*f),FALSE, pl ? OPPLAYER : OPENEMY,c,aRecQueue,aSendQueue);
                                                        if(pl) w->nueva_municion(0,w->municion(0)-10);
                                                   }else encola_mensaje ("You need 10 points of fire energy",w->color(1),prio);
                                                   break;
                                          // Bola de hielo
                                          case 4 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=1) || (!pl)){
                                                      /*  ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,HIEL,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                        sonido_ambiente(S_HIEL,w->fx_pos(),w->fy_pos());*/
													   crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,HIEL,(word)(10*co*f), pl ? OPPLAYER : OPENEMY,c,S_HIEL,aRecQueue,aSendQueue);
                                                        if(pl) w->nueva_municion(1,w->municion(1)-1);
                                                   }else encola_mensaje ("No water energy left",w->color(1),prio);
                                                   break;
                                          // Invulnerabilidad temporal
                                          case 5 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=5) || (!pl)){
                                                        w->obtiene_special(INVUL,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(1,w->municion(1)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points of water energy",w->color(1),prio);
                                                   break;
                                          // Barrera de hielo
                                          case 6 : w->nuevo_estado(HECH);
                                                   if((w->municion(1)>=10) || (!pl)){
                                                  /* dev_mapa()->anyade_barrera((bald)w->front_x(1.0),(bald)w->front_y(1.0),1);
                                                        if(pl) w->nueva_municion(1,w->municion(1)-10);
                                                        sonido_ambiente(S_EXHI,w->fx_pos(),w->fy_pos());*/

													   TMsgNewBarrier *lMsgBarr = DBG_NEW TMsgNewBarrier;

													   lMsgBarr->mType = eMsgNewBarrier;
													   lMsgBarr->mSender = getMaskFromPlayerId(c);
													   lMsgBarr->mReceiver = MASK_SERVER;
													   lMsgBarr->mX = w->front_x(1.0);
													   lMsgBarr->mY = w->front_y(1.0);
													   lMsgBarr->mBarrType = 1;

													   if(modo_red == MRSERVER)
														   aRecQueue->queueMessage(lMsgBarr);
													   else
														   aSendQueue->queueMessage(lMsgBarr);

													   if(pl) w->nueva_municion(1,w->municion(1)-10);
                                                        sonido_ambiente(S_EXHI,w->fx_pos(),w->fy_pos());

                                                   }else encola_mensaje ("You need 10 points of water energy",w->color(1),prio);
                                                   break;
                                          // Rayo el‚ctrico
                                          case 7 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=1) || (!pl)){
                                                      /*  ingresa_bala(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(10*co*f),w);
                                                        if(pl) w->nueva_municion(2,w->municion(2)-1);
                                                        sonido_ambiente(S_RAYO,w->fx_pos(),w->fy_pos());*/
													   crea_bala_red(w->front_x(1.0),w->front_y(1.0),w->dir(),0.15*cb,0,ELEC,(word)(10*co*f), pl ? OPPLAYER : OPENEMY,c,S_RAYO,aRecQueue,aSendQueue);
                                                       if(pl) w->nueva_municion(2,w->municion(2)-1);
                                                   }else encola_mensaje ("No lightning energy left",w->color(1),prio);
                                                   break;
                                          // Invisibilidad temporal
                                          case 8 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=5) || (!pl)){
                                                        w->obtiene_special(INVIS,(word)(co*f*WTSPECIAL/3));
                                                        if(pl) w->nueva_municion(2,w->municion(2)-5);
                                                        sonido_ambiente(S_ITSP,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 5 points of lightning energy",w->color(1),prio);
                                                   break;
                                          // Descarga el‚ctrica
                                          case 9 : w->nuevo_estado(HECH);
                                                   if((w->municion(2)>=10) || (!pl)){
                                                       bolt_red(w->fx_pos(),w->fy_pos(),(co*6),w->bando(),(word)(15*co*f),OPPLAYER,c,aRecQueue,aSendQueue);
                                                        if(pl) w->nueva_municion(2,w->municion(2)-10);
                                                        sonido_ambiente(S_BOLT,w->fx_pos(),w->fy_pos());
                                                   }else encola_mensaje ("You need 10 points of lightning energy",w->color(1),prio);

                                                   break;

                                          // Apocalipsis
                                          case 10 : apocalipsis_red(w->fx_pos(),w->fy_pos(),10,(word)(30*co*f),OPPLAYER,c,aRecQueue,aSendQueue);
                                                    sonido_ambiente(S_APOC,w->fx_pos(),w->fy_pos());
                                                    w->nuevo_estado(HECH);
                                                    if(pl){
                                                           if(w->salud()<15) w->danyar(30,DAPOC,w);
                                                           else w->nueva_salud(w->salud()-15);

                                                    };
                                                   break;
                                          };};
                        };};
                        break;

            case HECH :
            case SLAS :
            case SHOT :
            case PUNY :
					{
						nx=w->fx_pos(); ny=w->fy_pos();
                        if(cont->arr()) {ny-=WPASO; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        if(w->special(BERSERK)) f=3; else f=1;

						byte lOpType, lOpId;

                        //Danya por pu¤etazo
                        if((w->contador()==5) && (w->estado()==PUNY)){
                            //Personajes
                            sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),0.7,10,FALSE, &lOpType, &lOpId);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(5*f*co))){

								envia_danyo_red(c,DPUNY,(word)(5*f*co),lOpType,lOpId,aRecQueue,aSendQueue);

                                //sonido_warrior(sec,0);

                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());

                            //// Barreras
                            if((i=mp->hay_barr(w->front_x(0.5),w->front_y(0.5)))>=0)
                                //mp->dev_barr(i)->danyar((word)(5*f*3*co));
								envia_danyo_red(c,DPUNY,(word)(5*f*3*co),OPBARR,i,aRecQueue,aSendQueue);
                        };
                        //Da¤a por golpe de espada
                        if((w->contador()==WTSLAS-EXPTIME) && (w->estado()==SLAS)){

                            if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                               sonido_ambiente(S_CORT,w->fx_pos(),w->fy_pos());
                            //Personajes
                            sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),1.0,10,FALSE, &lOpType, &lOpId);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(co*(5+5*f*w->arma())))){
                                envia_danyo_red(c,DESPA,(word)(co*(5+5*f*w->arma())),lOpType,lOpId,aRecQueue,aSendQueue);
                                //sonido_warrior(sec,0);
                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());
                            //// Barreras
                            if((i=mp->hay_barr(w->front_x(0.5),w->front_y(0.5)))>=0)
                            //    mp->dev_barr(i)->danyar((word)(co*(5+15*f*w->arma())));
								  envia_danyo_red(c,DESPA,(word)(co*(5+15*f*w->arma())),OPBARR,i,aRecQueue,aSendQueue);
                         };
						}
                        break;

            case DEFE : nx=w->fx_pos(); ny=w->fy_pos();
                        if(cont->arr()) {ny-=WPASO; d=DARR; mover=TRUE;}
                        else if(cont->aba()) {ny+=WPASO; d=DABA; mover=TRUE;}
                        else if(cont->der()) {nx+=WPASO; d=DDER; mover=TRUE;}
                        else if(cont->izq()) {nx-=WPASO; d=DIZQ; mover=TRUE;};

                        // Caminar, con "strafee" o sin
                        w->actualiza(mover);
                        if(mover) w->avanza(d,permite(nx,ny,w),cont->boton(1));

                        //Deja de cubrirse
                        if(!cont->boton(2)) w->nuevo_estado(WALK);

                        // Da¤a si hay explosi¢n que no puede cubrir
                        if(((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0) && (w->contador()==0)){
                                if(danyo_total_explos(w->fx_pos(),w->fy_pos(),1.0)>5*co*w->escudo()){
                                        w->nuevo_estado(WALK);
                                        sonido_warrior(w,0);
                                    };
                              };
                        break;

            case TELP : w->actualiza(FALSE);
                        if(w->contador()==WTTEL/2){
                        i=mp->hay_telep(w->x_pos(),w->y_pos());
                        x=mp->dev_telep(i)->x_out()+0.5;
                        y=mp->dev_telep(i)->y_out()+0.5;
                        // Telefragg!!!
                       i=0;
                        while((i=hay_player(x,y,i))>=0){
                              sec=dev_player(i);
							  if(sec!=w)
								  //sec->danyar(300,DTELE,w);
								  envia_danyo_red(c,DTELE,300,OPPLAYER,i,aRecQueue,aSendQueue);
                              i++;
                        };
                        /*if((i=hay_enem(x,y,0))>=0){
                                sec=dev_enem(i); sec->danyar(300,DTELE,w); };*/
                        w->nueva_pos(x,y);};
                        break;

            case DIE  : w->actualiza(FALSE);
                        if((cont->algun_boton()) && (w->contador()==WTCAE) && (pl))
                                reset_warrior_red(OPPLAYER,c,aRecQueue,aSendQueue);
                        if(w->contador()==WDOLWAIT){
                              // Mensajes al morir
                              if(w->agresor()!=NULL){
                                // Suicidio
                                if(w->agresor()==w) sprintf(s,"%s kills itself.",w->nombre());
                                else{
                                  switch(w->causa_danyo()){
                                    case DPUNY : sprintf(aux,"beaten"); break;
                                    case DESPA : sprintf(aux,"slain"); break;
                                    case DBALA : sprintf(aux,"shot"); break;
                                    case DCOHE : sprintf(aux,"blasted"); break;
                                    case DFUEG : sprintf(aux,"burned"); break;
                                    case DELEC : sprintf(aux,"electrocuted"); break;
                                    case DTELE : sprintf(aux,"telefragged"); break;
                                    case DHIEL : sprintf(aux,"frozen"); break;
                                    };
                                sprintf(s,"%s was %s by %s.",w->nombre(),aux,w->agresor()->nombre());
                                if(tipoj==DMCH)
								{
									byte tipo_agr;
									byte id_agr;
									identifica_guerrero(w->agresor(),tipo_agr,id_agr);
									if(tipo_agr == OPPLAYER)
										incrementa_muertes_red(id_agr, aRecQueue, aSendQueue);
								}

                                //if(w->bando()!=w->agresor()->bando()) w->agresor()->gana_exp(w->level());
                                };
                              }else{
                              switch(w->causa_danyo()){
                                case DSUEL : sprintf(s,"%s falled into a pit.",w->nombre()); break;
                                case DLASE : sprintf(s,"%s was killed by a laser.",w->nombre()); break;
                                case DVENE : sprintf(s,"%s was killed by a poison.",w->nombre()); break;
                                default    : sprintf(s,"%s was killed by a trap.",w->nombre()); break;
                                };
                              };

                             it=w->item_asociado();
                              if(it!=NING)
                                 ingresa_item_util_red(w->x_pos(),w->y_pos(),it,aRecQueue,aSendQueue);
                              if(pl){
										encola_mensaje_red(s,w->color(1),allprio,aRecQueue,aSendQueue);
										encola_mensaje_red("Push any button to restart",w->color(1),prio,aRecQueue,aSendQueue);
										//encola_mensaje(s,w->color(1),allprio);
										//encola_mensaje("Push any button to restart",w->color(1),prio);
									}
                              else encola_mensaje_consola(s,15);
                              sonido_warrior(w,1);};
                        break;

            case DEST  : w->actualiza(FALSE);
                        if((cont->algun_boton()) && (w->contador()==WTCAE) && (pl))
                                reset_warrior_red(OPPLAYER,c,aRecQueue,aSendQueue);
                        if(w->contador()==WDOLWAIT){
                              // Mensajes al morir
                              if(w->agresor()!=NULL){
                                // Suicidio
                                if(w->agresor()==w) sprintf(s,"%s destroys itself.",w->nombre());
                                else{
                                     switch(w->causa_danyo()){
                                        case DPUNY : sprintf(aux,"destroyed"); break;
                                        case DESPA : sprintf(aux,"eviscerated"); break;
                                        case DBALA : sprintf(aux,"riddled"); break;
                                        case DCOHE : sprintf(aux,"exploded"); break;
                                        case DFUEG : sprintf(aux,"roasted"); break;
                                        case DELEC : sprintf(aux,"electrocuted"); break;
                                        case DTELE : sprintf(aux,"telefragged"); break;
                                        case DHIEL : sprintf(aux,"frozen"); break;
                                        case DAPOC : sprintf(aux,"charmed"); break;
                                     };
                                     sprintf(s,"%s was %s by %s.",w->nombre(),aux,w->agresor()->nombre());
                                     if(tipoj==DMCH)
										{
											byte tipo_agr;
											byte id_agr;
											identifica_guerrero(w->agresor(),tipo_agr,id_agr);
											if(tipo_agr == OPPLAYER)
												incrementa_muertes_red(id_agr, aRecQueue, aSendQueue);
										}
                                    //if(w->bando()!=w->agresor()->bando()) w->agresor()->gana_exp(w->level());
                                };
                              }else{
                              switch(w->causa_danyo()){
                                case DLASE : sprintf(s,"%s was killed by a laser.",w->nombre()); break;
                                case DVENE : sprintf(s,"%s was killed by a poison.",w->nombre()); break;
                                default    : sprintf(s,"%s was destroyed by a trap.",w->nombre()); break;
                                };
                              };
                              it=w->item_asociado();
                              if(it!=NING)
                                 ingresa_item_util_red(w->x_pos(),w->y_pos(),it, aRecQueue, aSendQueue);
                              if(pl){

									encola_mensaje_red(s,w->color(1),allprio,aRecQueue,aSendQueue);
									encola_mensaje_red("Push any button to restart",w->color(1),prio,aRecQueue,aSendQueue);
									//encola_mensaje(s,w->color(1),allprio);
									//encola_mensaje("Push any button to restart",w->color(1),prio);
								}
                              else encola_mensaje_consola(s,15);
                              sonido_warrior(w,2);};
                        break;

            case CONG  : w->actualiza(FALSE);
                         if(w->contador()==WTCONG-EXPTIME)
                           ingresa_explos(w->fx_pos(),w->fy_pos(),EHIE,1,w);

                        // Da¤a o descongela por balas
                        if((i=dist_explos(w->fx_pos(),w->fy_pos(),1.0))>=0){
                              if(dev_explos(i)->tipo()==ELLA)
                                   w->nuevo_estado(WALK);
                              else{
                                if(bcont%WDOLWAIT==0) w->danyar(5,DHIEL,dev_explos(i)->creador());
                              };
                        }; break;
                        break;

            case OUTL  : w->actualiza(FALSE); break;
      };
}

void batalla::actualiza_caminar_player_red_remoto(byte c, warrior* w, bool pl)
{
    bool mover = FALSE;
    float nx = w->fx_pos(), ny = w->fy_pos(), co = w->level_coef();

    if (pl)
    {
        if (last_received_x[c][1] == nx && last_received_y[c][1] == ny)
        {
            mover = false;
        }
        else
        {
            mover = true;
            if (fabs(last_received_x[c][1] - nx) > fabs(last_received_y[c][1] - ny))
            {
                if (fabs(last_received_x[c][1] - nx) <= WPASO * co)
                {
                    nx = last_received_x[c][1];
                }
                else
                {
                    if (nx < last_received_x[c][1]) nx += WPASO * co;
                    else nx -= WPASO * co;
                }
            }
            else
            {
                if (fabs(last_received_y[c][1] - ny) <= WPASO * co)
                {
                    ny = last_received_y[c][1];
                }
                else
                {
                    if (ny < last_received_y[c][1]) ny += WPASO * co;
                    else ny -= WPASO * co;
                }
            }
        }
    }
    else
    {
        if (last_received_enem_x[c][1] == nx && last_received_enem_y[c][1] == ny)
        {
            mover = false;
        }
        else
        {
            mover = true;
            if (fabs(last_received_enem_x[c][1] - nx) > fabs(last_received_enem_y[c][1] - ny))
            {
                if (fabs(last_received_enem_x[c][1] - nx) <= WPASO * co)
                {
                    nx = last_received_enem_x[c][1];
                }
                else
                {
                    if (nx < last_received_enem_x[c][1]) nx += WPASO * co;
                    else nx -= WPASO * co;
                }
            }
            else
            {
                if (fabs(last_received_enem_y[c][1] - ny) <= WPASO * co)
                {
                    ny = last_received_enem_y[c][1];
                }
                else
                {
                    if (ny < last_received_enem_y[c][1]) ny += WPASO * co;
                    else ny -= WPASO * co;
                }
            }
        }
    }

    // Caminar, con "strafee" o sin
    w->actualiza(mover);
    if (mover) w->nueva_pos(nx, ny);
}

void batalla::actualiza_player_red_remoto(byte c, warrior *w, controlm *cont, bool pl)
{
      warrior *sec;
      disparador *dis;
      t_item it;
      w_dir d;
      int i, t_so=0;
      bool mover=FALSE, retira=FALSE;
      float nx=w->fx_pos(), ny=w->fy_pos(),x,y,f=1, co=w->level_coef(), cb=0.95+(0.05*w->level());
      mapa *mp=dev_mapa();
      char s[200], aux[20];
      byte prio, allprio=ALLPRIO, lOpType, lOpId;
      cdanyo cd;

      switch(c){
                case 0 : prio=PRIO1; break;
                case 1 : prio=PRIO2; break;
                case 2 : prio=PRIO3; break;
                case 3 : prio=PRIO4; break;
				case 4 : prio=PRIO5; break;
                case 5 : prio=PRIO6; break;
                case 6 : prio=PRIO7; break;
                case 7 : prio=PRIO8; break;};

	  d = w->dir();

      switch(w->estado()){

            case WALK:  actualiza_caminar_player_red_remoto(c, w, pl);

                        // Sonido de daño
                        if (w->contador() == WDOLWAIT - 10) {
                            sonido_warrior(w, 0);
                        }
                        break;

            case HECH :
            case SLAS :
            case SHOT :
            case PUNY : actualiza_caminar_player_red_remoto(c, w, pl);

						if((w->contador()==5) && (w->estado()==PUNY)){
                            //Personajes
                            sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),0.7,10,FALSE, &lOpType, &lOpId);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(5*f*co))){
								sonido_ambiente(S_PUNY,w->fx_pos(),w->fy_pos());
                                sonido_warrior(sec,0);
                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());
						}

                        //Da¤a por golpe de espada
                        if((w->contador()==WTSLAS-EXPTIME) && (w->estado()==SLAS)){

							sec=oponente_cercano(w,w->front_x(0.5),w->front_y(0.5),1.0,10,FALSE);
                            if(dev_mapa()->permite((bald)w->front_x(0.5),(bald)w->front_y(0.5)))
                            if((sec!=NULL) && (w!=sec) && (!sec->special(INVUL)) && (!sec->dios()))
                            if(sec->puede_danyar((word)(co*(5+5*f*w->arma())))){
                                sonido_warrior(sec,0);
                            }else sonido_ambiente(S_ESCU,w->fx_pos(),w->fy_pos());

                            if(!permite(w->front_x(0.5),w->front_y(0.5),w))
                               sonido_ambiente(S_CORT,w->fx_pos(),w->fy_pos());
                         };
                        break;

            case DEFE : actualiza_caminar_player_red_remoto(c, w, pl);
                        break;

            case TELP : 
                        if(pl)
                            w->nueva_pos(last_received_x[c][1],last_received_y[c][1]);
                        else
                        {
                            matr_enem[w->y_pos()][w->x_pos()]--;
                            w->nueva_pos(last_received_enem_x[c][1], last_received_enem_y[c][1]);
                            matr_enem[w->y_pos()][w->x_pos()]++;
                        }
						w->actualiza(FALSE);
                        break;

            case DIE  : if (pl)
                            w->nueva_pos(last_received_x[c][1], last_received_y[c][1]);
                      else
                        {
                            matr_enem[w->y_pos()][w->x_pos()]--;
                            w->nueva_pos(last_received_enem_x[c][1], last_received_enem_y[c][1]);
                            matr_enem[w->y_pos()][w->x_pos()]++;
                        }
						w->actualiza(FALSE);
                        if(w->contador()==WDOLWAIT){
                              sonido_warrior(w,1);};
                        break;

            case DEST  : 
                        if (pl)
                            w->nueva_pos(last_received_x[c][1], last_received_y[c][1]);
                        else
                        {
                            matr_enem[w->y_pos()][w->x_pos()]--;
                            w->nueva_pos(last_received_enem_x[c][1], last_received_enem_y[c][1]);
                            matr_enem[w->y_pos()][w->x_pos()]++;
                        }
						w->actualiza(FALSE);
                        if(w->contador()==WDOLWAIT){
                              sonido_warrior(w,2);};
                        break;

            case CONG  :if (pl)
                            w->nueva_pos(last_received_x[c][1], last_received_y[c][1]);
                        else
                        {
                            matr_enem[w->y_pos()][w->x_pos()]--;
                            w->nueva_pos(last_received_enem_x[c][1], last_received_enem_y[c][1]);
                            matr_enem[w->y_pos()][w->x_pos()]++;
                        }
						w->actualiza(FALSE);
                        break;

            case OUTL  :if (pl)
                            w->nueva_pos(last_received_x[c][1], last_received_y[c][1]);
                       else
                        {
                            matr_enem[w->y_pos()][w->x_pos()]--;
                            w->nueva_pos(last_received_enem_x[c][1], last_received_enem_y[c][1]);
                            matr_enem[w->y_pos()][w->x_pos()]++;
                        }
						w->actualiza(FALSE);
						break;
      };
}
// Actualiza la bala [c], convirtiendola en explosi¢n si es necesario
void batalla::actualiza_bala(byte c, CMessageQueue *aRecQueue, CMessageQueue *aSendQueue)
{
      bala *b=bbala[c];
      e_tipo t;

      if(b==NULL) return;
      float px=b->posx(), py=b->posy(), vel=b->vel();
      w_dir dir=b->dir();
      byte f=b->fuerza();
      switch(dir){
            case DDER : px+=vel; break;
            case DIZQ : px-=vel; break;
            case DARR : py-=vel; break;
            case DABA : py+=vel; break;
      };
      // Avanza la bala si no hay obst culo
      if((permite(px,py,NULL)) ||
        ((dev_mapa()->conse(px,py)>=DEC1) && (dev_mapa()->conse(px,py)<=DEC9)))
            b->avanza();
      else
	  {
			switch(b->tipo()){

				case BULL : t=EBAL; break;
				case ROCK : t=ECOH; break;
				case FUEG : t=EFUE; break;
				case HIEL : t=EHIE; break;
				case ELEC : t=EELE; break;
			};

			// Caso contario, se transforma en explosi¢n
			if(modo_red == MROFFLINE)
			{
				ingresa_explos(b->posx(),b->posy(),t,b->fuerza(),b->creador());
				retira_bala(c);
			}
			else if(modo_red == MRSERVER)
			{
				byte id_creador = 0;
				byte tipo_creador = OPPLAYER;
                byte id_receptor = 0;
				byte tipo_receptor = OPPLAYER;

                // Comprueba si colisionó con personaje
                warrior *w = colision_con_guerrero(px,py,0);

                if(w != NULL)
                {
                    // Daña al personaje con el que colisionó. Atajo para evitar que el jugador remoto ya se haya
                    // desplazado y por tanto cuando se cree la explosión ya no reciba el daño
                    identifica_guerrero(b->creador(), tipo_creador, id_creador);
                    identifica_guerrero(w, tipo_receptor, id_receptor);

                    cdanyo dtipo = DPUNY;

                    switch(b->tipo()){

                        case BULL : dtipo=DBALA; break;
                        case ROCK : dtipo=DCOHE; break;
                        case FUEG : dtipo=DFUEG; break;
                        case HIEL : dtipo=DHIEL; break;
                        case ELEC : dtipo=DELEC; break;
                    };

                    envia_danyo_red(id_creador, dtipo, b->fuerza(), tipo_receptor, id_receptor, aRecQueue, aSendQueue );

                }

				//TMensEndBullet
				TMsgEndBullet *lMsg = DBG_NEW TMsgEndBullet;

				lMsg->mType = eMsgEndBullet;
				lMsg->mSender = MASK_SERVER;
				lMsg->mReceiver = MASK_ALL_CLIENTS;
				lMsg->mId = c;

				aSendQueue->queueMessage(lMsg);

				identifica_guerrero(b->creador(), tipo_creador, id_creador);

				crea_explos_red(b->posx(),b->posy(),t,b->fuerza(),tipo_creador,id_creador,aRecQueue,aSendQueue);

				retira_bala(c);
			}
			else if(modo_red == MRCLIENTE)
			{
				// No hacer nada
			}
		};
}
bool batalla::mapa_activo()
{
        return modo_mapa;
}
bool batalla::consola_activa()
{
        return modo_consola;
}
bool batalla::datos_activo()
{
        return modo_datos;
}
bool batalla::consola_input_activo()
{
        return con_input;
}
bool batalla::mapa_conocido(bald x, bald y)
{
        return matr_mapa[int(x/5)][int(y/5)];
}
byte batalla::zoom_mapa()
{
        return mzoom;
}
void batalla::sonido_ambiente(s_sample s,float x, float y)
{
        bwav->play_wav_ambient(s,diferencia_x(640,400,x,y),dist_centro(x,y),single_screen());
}
void batalla::sonido_warrior(warrior *w,int s)
{
        int p;
        float x,y;
        p=w->cualspr(); x=w->fx_pos(); y=w->fy_pos();
        bwav->play_wav_warrior(p,s,diferencia_x(640,400,x,y),dist_centro(x,y),single_screen());
}
void batalla::activa_musica()
{
        bwav->play_bgm();
}
void batalla::desactiva_musica()
{
        bwav->stop_bgm();
}
void batalla::activa_lluvia()
{
      lluvia=TRUE;
      for(int i=0; i<MAXGO; i++){

         bgota[i].x=player[0]->x_pos()-8+((rand()%160)/10);
         bgota[i].y=player[0]->y_pos()-8+((rand()%160)/10);
         bgota[i].z=(LLSTARTZ/MAXGO)*i;
         bgota[i].cont=0;
      };

}
void batalla::actualiza_lluvia()
{
      int p;
      for(int i=0; i<MAXGO; i++){

         if(bgota[i].z>0) {bgota[i].z-=0.15; bgota[i].x+=0.05;}
         else {
              bgota[i].cont++;
              if(bgota[i].cont==10){
                  p=rand()%nplayers;
                  bgota[i].x=player[p]->x_pos()-8+((rand()%160)/10);
                  bgota[i].y=player[p]->y_pos()-8+((rand()%160)/10);
                  bgota[i].z=LLSTARTZ;
                  bgota[i].cont=0;
              }
         }
      };
      //Trueno!!!
      if(rand()%800==0) trueno=TRCICLO;
      if(trueno==1) bwav->play_wav_ambient(S_TRUE,0,0,FALSE);
      if(trueno>0) trueno--;
}
void batalla::stop_ambient_samples()
{
      bwav->stop_ambient_samples();
}

byte batalla::dev_modo_red()
{
	return modo_red;
}

void batalla::asigna_modo_red(byte m)
{
	modo_red = m;
}

byte batalla::dev_jug_local()
{
	return jug_local;
}

void batalla::asigna_jug_local(byte jl)
{
	jug_local = jl;
}

 bool batalla::ventana_chat_abierta()
 {
	 return ventana_chat;
 }

 char* batalla::dev_mensaje_chat()
 {
	 return chat_msg;
 }

