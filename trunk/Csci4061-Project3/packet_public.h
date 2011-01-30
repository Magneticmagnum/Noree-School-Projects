#include "mm_public.h"

#define MaxPackets 10

typedef char data_t[64];

typedef struct
{
    int how_many; /* number of packets in the message */
    int which; /* which packet in the message */
    data_t data; /* packet data */
} packet_t;

/* Keeps track of packets that have arrived for the message */
typedef struct
{
    int num_packets;
    void* data[MaxPackets];
} message_t;

