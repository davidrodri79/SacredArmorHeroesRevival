//#define PROFILER

#ifndef PROFILER

#define ___START_PROFILER()			
#define ___ENTER_PROFILER_BLOCK(str)	
#define ___LEAVE_PROFILER_BLOCK(str)	
#define ___SHOW_PROFILER(str)			

#else

void start_profiler();
void enter_profiler_block(char *);
void leave_profiler_block(char *);
void show_profiler();

#define ___START_PROFILER()				start_profiler();
#define ___ENTER_PROFILER_BLOCK(str)	enter_profiler_block(str);
#define ___LEAVE_PROFILER_BLOCK(str)	leave_profiler_block(str);
#define ___SHOW_PROFILER(str)			show_profiler();


#endif