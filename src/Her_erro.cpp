#include"her_tipo.hpp"

terror last_error=NO_ERR;
char last_error_message[40];
char last_error_module[15];

void her_error(const char tad[], terror e)
{
      char mens[40];
      switch(e){
                case ERR_MEM : sprintf(mens,"Out of RAM memory"); break;
                case ERR_FIL : sprintf(mens,"I/O File incorrect");
                case ERR_NOI : sprintf(mens,"Object not initialized"); break;
                case ERR_COO : sprintf(mens,"Coordinates out of range"); break;
                case ERR_IMP : sprintf(mens,"Impossible structure"); break;
                case ERR_LLE : sprintf(mens,"Too many items"); break;
                case ERR_NOC : sprintf(mens,"Required object does not exists"); break;
                case ERR_CAD : sprintf(mens,"String too long"); break;
                };
      sprintf(last_error_message,"%s",mens);
      sprintf(last_error_module,"%s",tad);
      last_error=e;
	  //fprintf(stderr,"Error in module %s:%s",tad,mens);
}
