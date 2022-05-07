#include "profiler.h"

#ifdef PROFILER

#include<stdio.h>
#include<allegro5/allegro.h>

#define MAX_BLOCKS 40

struct profiler_block
{
	bool used;
	char name[60];
	profiler_block *parent;
	double start_time;
	double total_time;
	int level;
};

profiler_block blocks[MAX_BLOCKS];

profiler_block *current_block = 0;

void start_profiler()
{
	for(int i = 0; i < MAX_BLOCKS; i++)
		blocks[i].used = false;

	current_block = 0;
}

void enter_profiler_block(char *name)
{
	int i = 0;
	while(blocks[i].used && i < MAX_BLOCKS)
		i++;

	if(i>=MAX_BLOCKS)
		fprintf(stderr,"PROFILER ERROR: Too many blocks!!\n");
	else
	{
		blocks[i].used = true;
		sprintf(blocks[i].name, "%s", name);
		blocks[i].start_time = al_get_time();
		blocks[i].parent = current_block;
		blocks[i].level = blocks[i].parent ? blocks[i].parent->level + 1 : 0;
		current_block = &blocks[i];
	}

}

void leave_profiler_block(char *name)
{
	if(current_block)
	if( strcmp(name, current_block->name) != 0)
	{
		fprintf(stderr,"PROFILER ERROR: Exiting wrong block \"%s\"!!\n",name);
	}
	else
	{
		current_block->total_time = al_get_time() - current_block->start_time;
		current_block = current_block->parent;
	}
}

void show_profiler()
{
	int i = 0;
	fprintf(stderr,"------------------------------------------------------\n");
	while(blocks[i].used && i < MAX_BLOCKS)
	{
		for(int j = 0; j < blocks[i].level; j++)
			fprintf(stderr,"====");

		fprintf(stderr, "%s: %.04g ms\n", blocks[i].name, blocks[i].total_time * 1000);

		i++;
	}
	fprintf(stderr,"------------------------------------------------------\n");
}

#endif
