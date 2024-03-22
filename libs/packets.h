#ifndef PACKETS_H
#define PACKETS_H

#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum P_KIND {
  eDISCONNECT = 0x00,
  eHANDSHAKE = 0x01,
  eLOGINSUCC = 0x02,
  eMOVE = 0x03,
  ePARTICLE = 0x04, // summoning vfx particle on client
  //...
} packet_id;

typedef struct {
  uint16_t tumbstone; //'nn' 0x4e4e || 'hh' 0x4848
  uint16_t id;
  uint16_t length;
  uint8_t *payload;
} Packet;

Packet *create_packet(uint16_t id, uint16_t length, uint8_t *payload = 0);
void decode_packet(Packet *p);
void encode_packet(Packet *p);
Packet serialize_packet(uint64_t bit_field);
Packet deserialize_packet(uint64_t bit_field);
void send_packet(Server *server, Packet *p);

#endif // PACKETS_H
