
#include "platform.h"

#if defined SAHWINDOWS
#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <crtdbg.h>
#elif defined SAHLINUX
#include <stdio.h>
#endif


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


#define FALSE 0
#define TRUE 1

#define FNAME 13
#define MENSZ 40
#define NSPR  10
#define ANIMSP 0.1

#define N_SMP_AMBI 4

typedef unsigned char byte;

typedef short unsigned int word;

typedef byte bald;

enum w_dir {DDER=0, DABA, DIZQ, DARR};

enum terror {NO_ERR=0, ERR_MEM, ERR_FIL, ERR_NOI, ERR_COO, ERR_IMP, ERR_LLE, ERR_NOC, ERR_CAD};

extern void her_error(const char tad[], terror e);

extern char last_error_message[];

extern char last_error_module[];

extern terror last_error;

extern char NOMBRE_MODULO[];



