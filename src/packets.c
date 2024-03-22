#include "../libs/packets.h"
#include <stdint.h>

#define NETWORK 0x4e4e
#define HOST 0x4848
// typedef struct {
//   uint16_t tumbstone; //'nn' 0x4e4e || 'hh' 0x4848
//   uint16_t id;
//   uint16_t length;
//   uint8_t *payload;
// } Packet;

Packet *create_packet(uint16_t id, uint16_t length, uint8_t *payload) {
  Packet *p = (Packet *)malloc(offsetof(Packet, payload) + length);
  p->tumbstone = HOST;
  p->id = id;
  p->length = length == 0 ? 1 : length;
  if (payload == NULL) {
    memset(p->payload, 0, length);
  } else {
    memset(p->payload, 0, length);
  }
  return p;
}

void encode_packet(Packet *p) {
  if (p->tumbstone == NETWORK) {
    return;
  }
  p->id = htons(p->id);
  p->length = htons(p->length);
  p->tumbstone = NETWORK;
  // encode payload
}

void decode_packet(Packet *p) {
  if (p->tumbstone == HOST) {
    return;
  }
  p->id = ntohs(p->id);
  p->length = ntohs(p->length);
  p->tumbstone = HOST;
  // decode payload
}

Packet serialize_data(uint8_t *pl) {
  /*
   *
   *
   *
   */
  return (Packet){};
}

// typedef struct {
//   packet_id id;
//   uint16_t length;
//   uint64_t bytes; <<<<
// } Packet;
//

Packet deserialize_data(uint64_t bit_field) {
  // packet_id id = bit_field & 0xFF;
  // uint16_t length = (bit_field >> 8) & 0xFFFF;
  return (Packet){0};
}
Packet serialize_packet(uint64_t bit_field);
Packet deserialize_packet(uint64_t bit_field);
void send_packet(Server *server, Packet *p);
