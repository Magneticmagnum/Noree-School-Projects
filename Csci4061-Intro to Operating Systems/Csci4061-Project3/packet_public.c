#include "packet_public.h"
#include <string.h>
#include <unistd.h>

message_t message; /* current message structure */
mm_t MM; /* memory manager will allocate memory for packets */
int pkt_cnt = 0; /* how many packets have arrived for current message */
int pkt_total = 1; /* how many packets to be received for the message */
int NumMessages = 3; /* number of messages we will receive */

packet_t get_packet()
{
  packet_t pkt;
  static int which = 0;

  pkt.how_many = 3;
  pkt.which = which;
  if (which == 0)
    strcpy(pkt.data, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
  else if (which == 1)
    strcpy(pkt.data, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
  else
    strcpy(pkt.data, "cccccccccccccccccccccccccccccccccccccccccccccccccccccccc");
  which = (which + 1) % 3;
  return pkt;
}

void packet_handler(int sig)
{
  packet_t pkt;

  /*  fprintf (stderr, "IN PACKET HANDLER, sig=%d\n", sig); */
  pkt = get_packet();

  /* insert your code here ... stick packet in memory */
  if (pkt.which == 0) {
    pkt_total = pkt.how_many;
  }
  //allocate a packet in user created memory manager
  message.data[pkt.which] = (char *) mm_get(&MM);
  strncpy(message.data[pkt.which], pkt.data, 64);
  pkt_cnt++;
}

/* Create message from packets ... deallocate packets */
char *assemble_message()
{
  char *MSG;

  /* Allocate MSG and assemble packets into it */
  MSG = (char *) malloc(sizeof(char) * 64 * 3 + 1);
  if (MSG == NULL) {
    fprintf(stderr, "Error with malloc");
    exit(1);
  }
  // MSG[0] = 'NULL';
  int i;
  for (i = 0; i < pkt_total; i++) {
    strcat(MSG, message.data[i]);
    mm_put(&MM, message.data[i]);
  }

  /* reset these for next message */
  pkt_total = 1;
  pkt_cnt = 0;
  message.num_packets = 0;

  return MSG;
}

int main(int argc, char **argv)
{
  /* init memory manager -- turns out that 64 is the packet size! */
  if (mm_init(&MM, 10, 100) == -1) {
    fprintf(stderr, "Error initializing");

  }
  // set up alarm & mask
  struct sigaction sa;
  sa.sa_handler = packet_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGALRM, &sa, NULL) == -1) {
    perror("Error setting alarm");
  }

  /* set up alarm handler -- mask all signals within it */
  struct itimerval it;
  it.it_interval.tv_sec = INTERVAL;
  it.it_interval.tv_usec = INTERVAL_USEC;
  it.it_value.tv_sec = INTERVAL;
  it.it_value.tv_usec = INTERVAL_USEC;
  if (setitimer(ITIMER_REAL, &it, NULL) == -1)
    perror("Error setting timer");

  /* turn on alarm timer ... use  INTERVAL and INTERVAL_USEC for sec and usec values */

  message.num_packets = 0;

  int i;
  for (i = 1; i <= NumMessages; i++) {
    while (pkt_cnt < pkt_total) {
      pause(); /* block until next packet */
    }

    //    MSG = assemble_message();
    fprintf(stderr, "GOT IT: message=%s\n", assemble_message());
  }

  /* Deallocate memory manager */
  mm_release(&MM);

  return 0;
}

