
#define CMAXB 4

enum t_control {NOTC=0, TEC1, TEC2, JOY1, JOY2, JOY3, JOY4, MOUS};

enum c_eventos {CARR=0, CABA, CIZQ, CDER, CBU1, CBU2, CBU3, CBU4};

extern ALLEGRO_KEYBOARD_STATE key_state;

void iniciar();

class controlm {

         private:
         bool carr, caba, cder, cizq, cboton[4];

         public:
         controlm();
         void reset();
         void actualiza(t_control);
         void activa(c_eventos);
         bool arr();
         bool aba();
         bool der();
         bool izq();
         bool boton(byte);
         bool algun_boton();
         bool tecla_mapa();
         bool tecla_consola();
         bool tecla_datos();
         bool tecla_mas();
         bool tecla_menos();
		 bool tecla_chat();
         bool tecla_numero(byte);
         int x_raton();
         int y_raton();
         bool b_raton(int);
		 static void install_keyboard_listener();
		 static void uninstall_keyboard_listener();
		 static bool keypressed();
		 static bool readkey(ALLEGRO_EVENT*);
		 static void flushallkeys();
         };

