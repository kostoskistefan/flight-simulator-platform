#ifndef X_PLANE_INTERFACE_H
#define X_PLANE_INTERFACE_H

#include <stdint.h>

typedef struct __attribute__((packed)) dref_packet_s
{
    char padding[3];
    char header[5];
    float value;
    char path[500];
} dref_packet_s;

typedef struct __attribute__((packed)) rref_packet_s
{
    char padding[3];
    char header[5];
    uint32_t reply_frequency;
    uint32_t reply_index;
    char path[400];
} rref_packet_s;

typedef struct rref_reply_packet_s
{
    int8_t index;
    float value;
} rref_reply_packet_s;

typedef enum data_reference_reply_index_e
{
    DATA_REFERENCE_REPLY_INDEX_AIRSPEED,
    DATA_REFERENCE_REPLY_INDEX_ALTITUDE,
    DATA_REFERENCE_REPLY_INDEX_ATTITUDE_ROLL,
    DATA_REFERENCE_REPLY_INDEX_ATTITUDE_PITCH,
    DATA_REFERENCE_REPLY_INDEX_HEADING,
    DATA_REFERENCE_REPLY_INDEX_VERTICAL_SPEED,
    DATA_REFERENCE_REPLY_INDEX_COM_FREQUENCY,
    DATA_REFERENCE_REPLY_INDEX_COM_STANDBY_FREQUENCY,
    DATA_REFERENCE_REPLY_INDEX_NAV_FREQUENCY,
    DATA_REFERENCE_REPLY_INDEX_NAV_STANDBY_FREQUENCY,
} data_reference_reply_index_e;

void x_plane_interface_initialize(void);
void x_plane_interface_send_data_reference_value(const char *data_reference_path, float value);
void x_plane_interface_subscribe_to_data_reference(
    const char *data_reference_path,
    data_reference_reply_index_e data_reference_reply_index,
    uint8_t reply_frequency
);

void x_plane_interface_poll_for_packet(rref_reply_packet_s *packets);

void x_plane_interface_send_udp_packet(const uint8_t *packet_header, uint16_t packet_size);

#endif // X_PLANE_INTERFACE_H