#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL 0
#define INTERVAL_USEC 50000

/* Fill this in */
typedef struct {
	void* start;
	void* end;

	void** queue;
        int queue_out;
        int queue_in;

	int size;
	int count;
	int clrmm;
} mm_t;

/* These will be defined in mm_public.c ... by you */
int mm_init(mm_t *MM, int hm, int sz);
void* mm_get(mm_t *MM);
void mm_put(mm_t *MM, void *chunk);
void mm_release(mm_t *MM);
double comp_time(struct timeval times, struct timeval timee);

