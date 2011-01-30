#include "mm_public.h"

/* Return usec */
double comp_time(struct timeval times, struct timeval timee)
{

  double elap = 0.0;

  if (timee.tv_sec > times.tv_sec) {
    elap += (((double) (timee.tv_sec - times.tv_sec - 1)) * 1000000.0);
    elap += timee.tv_usec + (1000000 - times.tv_usec);
  }
  else {
    elap = timee.tv_usec - times.tv_usec;
  }
  return ((unsigned long) (elap));

}

/* Write these ... */
// just init variables in MM struct
// set head to 0 and malloc how many * size of each chunk
int mm_init(mm_t *MM, int hm, int sz)
{
  /**
   * Handle of MM that already points to an already initialized mm_t struct
   */
  if (MM == NULL) {
    MM = malloc(sizeof(mm_t));
    MM->clrmm = 1;
    if (MM == NULL) {
      perror("Error allocating memory");
      return -1;
    }
  }
  /**
   * Allocate memory and set internal parameters
   */
  MM->start = malloc(hm * sz);
  if (MM->start == NULL) {
    perror("error allocating memory");
    if (MM->clrmm == 1) {
      free(MM);
      return -1;
    }
  }
  MM->end = MM->start + hm * sz;
  MM->queue = malloc(sizeof(void**) * hm);
  if (MM->queue == NULL) {
    perror("error allocating memory");
    free(MM->start);
    if (MM->clrmm == 1) {
      free(MM);
      return -1;
    }
  }
  int i = 0;
  for (; i < hm; i++) {
    MM ->queue[i] = MM->start + i * sz;
  }
  MM->queue_out = 0;
  MM->queue_in = hm;

  MM->count = hm;
  MM->size = sz;

  return 0;
}

// if available, returns a raw section of memory
// if head is equal or greater to how many, return null because there are none left
// otherwise return the current offset (start + head * size)
// and then increment head
void* mm_get(mm_t *MM)
{
  if (MM->queue_out >= MM->queue_in) {
    return (void *) NULL;
  }
  void* raw = MM->queue[MM->queue_out++ % MM->count];
  //  printf("getting %i\n", raw);
  return raw;
}

// put a raw section of memory back
// it doesn't necessarily put the memory back so much as simply
// decrement the head pointer, making that chunk of memory available
// for getting.
void mm_put(mm_t *MM, void *chunk)
{
  if (chunk >= MM->start && chunk < MM->end && MM->queue_in < MM->queue_out
      + MM->count) {
    //    printf("putting %i\n", chunk);
    MM->queue[MM->queue_in++ % MM->count] = chunk;
  }
}

// just release the allocated portion, since the struct passed will
// be taken care of when the stack frame is popped (or the program exits)
void mm_release(mm_t *MM)
{
  free(MM->start);
  free(MM->queue);
  if (MM->clrmm == 1) {
    free(MM);
  }
}

void timer_example()
{
  struct timeval time_start, time_end;
  int j;

  /* start timer */
  j = gettimeofday(&time_start, (void *) NULL);

  /* code you wish to time goes here */

  j = gettimeofday(&time_end, (void *) NULL);

  fprintf(stderr, "Time taken =  %f msec\n", comp_time(time_start, time_end)
      / 1000.0);
}

