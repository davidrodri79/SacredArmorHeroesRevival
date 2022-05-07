#include"her_tipo.hpp"
#include<allegro5/allegro.h>
#include"her_cont.hpp"

byte p_tec[2][8]={{ALLEGRO_KEY_UP,ALLEGRO_KEY_DOWN, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
                   ALLEGRO_KEY_RCTRL, ALLEGRO_KEY_ALTGR, ALLEGRO_KEY_RSHIFT, ALLEGRO_KEY_SPACE},
                  {ALLEGRO_KEY_W,ALLEGRO_KEY_S,ALLEGRO_KEY_A,ALLEGRO_KEY_D,ALLEGRO_KEY_T,ALLEGRO_KEY_Y,ALLEGRO_KEY_U,ALLEGRO_KEY_I}
                   };

ALLEGRO_KEYBOARD_STATE key_state;
ALLEGRO_JOYSTICK_STATE joy_state;
ALLEGRO_MOUSE_STATE mouse_state;
ALLEGRO_EVENT_QUEUE *key_queue = 0;

controlm::controlm()
{
      carr=FALSE; caba=FALSE; cder=FALSE; cizq=FALSE;
      for(int i=0; i<CMAXB; i++) cboton[i]=FALSE;
}
void controlm::reset()
{
      carr=FALSE; caba=FALSE; cder=FALSE; cizq=FALSE;
      for(int i=0; i<CMAXB; i++) cboton[i]=FALSE;
}
void controlm::actualiza(t_control t)
{
	  int i;
	  ALLEGRO_JOYSTICK *joy;
      reset();
      switch(t){
				case JOY4 :
				case JOY3 :
				case JOY2 :
                case JOY1 : /*poll_joystick();
                            if(joy[0].stick[0].axis[0].d1) cizq=TRUE;
                            if(joy[0].stick[0].axis[0].d2) cder=TRUE;
                            if(joy[0].stick[0].axis[1].d1) carr=TRUE;
                            if(joy[0].stick[0].axis[1].d2) caba=TRUE;
                            for(i=0; i<CMAXB; i++)
                                    if(joy[0].button[i].b) cboton[i]=TRUE;
                            */
							joy = al_get_joystick(t - JOY1);
							if (joy)
							{
								al_get_joystick_state(joy,&joy_state);
								for(i = 0; i<al_get_joystick_num_sticks(joy); i++)
								{
									if (joy_state.stick[i].axis[0] > 0.1f) cder=TRUE;
									if (joy_state.stick[i].axis[0] < -0.1f) cizq=TRUE;
									if (joy_state.stick[i].axis[1] > 0.1f) caba=TRUE;
									if (joy_state.stick[i].axis[1] < -0.1f) carr=TRUE;
								}
								for(i=0; i<CMAXB; i++)
									if (joy_state.button[i] != 0) cboton[i]=TRUE;

							}
							break;

                case TEC1 : al_get_keyboard_state(&key_state);
							if(al_key_down(&key_state,p_tec[0][CIZQ])) cizq=TRUE;
                            if(al_key_down(&key_state,p_tec[0][CDER])) cder=TRUE;
                            if(al_key_down(&key_state,p_tec[0][CARR])) carr=TRUE;
                            if(al_key_down(&key_state,p_tec[0][CABA])) caba=TRUE;
                            for(i=0; i<CMAXB; i++)
                                    if(al_key_down(&key_state,p_tec[0][CBU1+i])) cboton[i]=TRUE;
                            break;

                case TEC2 :al_get_keyboard_state(&key_state);
							if(al_key_down(&key_state,p_tec[1][CIZQ])) cizq=TRUE;
                            if(al_key_down(&key_state,p_tec[1][CDER])) cder=TRUE;
                            if(al_key_down(&key_state,p_tec[1][CARR])) carr=TRUE;
                            if(al_key_down(&key_state,p_tec[1][CABA])) caba=TRUE;
                            for(i=0; i<CMAXB; i++)
                                    if(al_key_down(&key_state,p_tec[1][CBU1+i])) cboton[i]=TRUE;
                           break;

                case MOUS : al_get_mouse_state(&mouse_state);
							int w = al_get_display_width( al_get_current_display() );
							int h = al_get_display_height( al_get_current_display() );

							if((mouse_state.x<w/3) && (mouse_state.y<h/3)) cizq=TRUE;
                            if((mouse_state.x>2*w/3) && (mouse_state.y>2*h/3)) cder=TRUE;
                            if((mouse_state.x>2*w/3) && (mouse_state.y<h/3)) carr=TRUE;
                            if((mouse_state.x<w/3) && (mouse_state.y>2*h/3)) caba=TRUE;
							if(mouse_state.buttons & 1) cboton[0]=TRUE;
                            if(mouse_state.buttons & 2) cboton[3]=TRUE;
                            if(mouse_state.buttons & 4) cboton[2]=TRUE;
							break;
							/*if((mouse_x<gfx_driver->w/3) && (mouse_y<gfx_driver->h/3)) cizq=TRUE;
                            if((mouse_x>2*gfx_driver->w/3) && (mouse_y>2*gfx_driver->h/3)) cder=TRUE;
                            if((mouse_x>2*gfx_driver->w/3) && (mouse_y<gfx_driver->h/3)) carr=TRUE;
                            if((mouse_x<gfx_driver->w/3) && (mouse_y>2*gfx_driver->h/3)) caba=TRUE;
                            if(mouse_b & 1) cboton[0]=TRUE;
                            if(mouse_b & 2) cboton[3]=TRUE;
                            if((mouse_b & 1) && (mouse_b & 2)){
                             cboton[2]=TRUE;};
                            break;*/

                };
}
void controlm::activa(c_eventos c)
{
        switch(c){
                case CARR : carr=TRUE; break;
                case CABA : caba=TRUE; break;
                case CIZQ : cizq=TRUE; break;
                case CDER : cder=TRUE; break;
                case CBU1 : cboton[0]=TRUE; break;
                case CBU2 : cboton[1]=TRUE; break;
                case CBU3 : cboton[2]=TRUE; break;
                case CBU4 : cboton[3]=TRUE; break;};
}
bool controlm::arr()
{
      return carr;
}
bool controlm::aba()
{
      return caba;
}
bool controlm::der()
{
      return cder;
}
bool controlm::izq()
{
      return cizq;
}
bool controlm::boton(byte c)
{
      if(c>=CMAXB) her_error("CONTROL",ERR_NOC);
      else return cboton[c];
}
bool controlm::algun_boton()
{
      if((cboton[0]) || (cboton[1]) || (cboton[2]) || (cboton[3])) return TRUE;
      else     return FALSE;
}
bool controlm::tecla_mapa()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_TAB);
	}else return false;
}
bool controlm::tecla_consola()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_F1);
	}else return false;
}
bool controlm::tecla_datos()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_F2);
	}else return false;
}
bool controlm::tecla_mas()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_PAD_PLUS);
	}else return false;
}
bool controlm::tecla_menos()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_PAD_MINUS);
	}else return false;
}
bool controlm::tecla_chat()
{
	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		return al_key_down(&key_state,ALLEGRO_KEY_ENTER);
	}else return false;
}
bool controlm::tecla_numero(byte c)
{	if(al_is_keyboard_installed()){
		al_get_keyboard_state(&key_state);
		if(c>11) her_error("CONTROL",ERR_NOC);
		else
		  switch(c){
			default : return al_key_down(&key_state,ALLEGRO_KEY_1+c); break;
			case 9  : return al_key_down(&key_state,ALLEGRO_KEY_0); break;
			//case 10 : return al_key_down(&key_state,ALLEGRO_KEY_1+c); break;
			//case 11 : return al_key_down(&key_state,ALLEGRO_KEY_1+c); break;
		  };

	}else return false;
     /* if(c>11) her_error("CONTROL",ERR_NOC);
      else
      switch(c){
        default : return key[KEY_1+c]; break;
        case 9  : return key[KEY_0]; break;
        case 10 : return key[KEY_MINUS]; break;
        case 11 : return key[KEY_EQUALS]; break;
      };
      return FALSE;*/
}
int controlm::x_raton()
{
	  al_get_mouse_state(&mouse_state);
      return mouse_state.x;
}
int controlm::y_raton()
{
      al_get_mouse_state(&mouse_state);
      return mouse_state.y;
}

void controlm::install_keyboard_listener()
{
	 key_queue = al_create_event_queue();
	 al_register_event_source(key_queue, al_get_keyboard_event_source() );
}

void controlm::uninstall_keyboard_listener()
{
	 al_destroy_event_queue(key_queue);
}

bool controlm::b_raton(int c)
{
	 al_get_mouse_state(&mouse_state);
        switch(c){
				  case 1 : if(mouse_state.buttons & 1) return TRUE; break;
                  case 2 : if(mouse_state.buttons & 2) return TRUE; break;
				  case 3 : if(mouse_state.buttons & 4) return TRUE; break;
                  default: her_error("CONTROL",ERR_NOC);
                  };
        return FALSE;
}

bool controlm::keypressed()
{
	ALLEGRO_EVENT key;

	if(!key_queue) return false;

	if(!al_is_event_queue_empty(key_queue))
	{
		return true;
	}
	return false;
}

bool controlm::readkey(ALLEGRO_EVENT *key)
{
	if(!key_queue) return false;

	if(!al_is_event_queue_empty(key_queue))
	{
		al_get_next_event(key_queue, key);
		return true;
	}
	return false;
}

void controlm::flushallkeys()
{
	if(!key_queue) return;
	al_flush_event_queue(key_queue);
}


