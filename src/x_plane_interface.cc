#include "x_plane_interface.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "definitions.h"

WiFiUDP udp;
static char udp_receive_buffer[1024];

void x_plane_interface_initialize(void)
{
    WiFi.mode(WIFI_STA);

    WiFi.config(
        X_PLANE_CONTROLLER_IP,
        X_PLANE_COMPUTER_IP,
        { 255, 255, 255, 0 },
        X_PLANE_COMPUTER_IP
    );

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    udp.begin(X_PLANE_PORT);
}

void x_plane_interface_send_data_reference_value(const char *data_reference_path, float value)
{
    dref_packet_s packet;

    packet.value = value;
    strcpy(packet.header, "DREF");
    memset(packet.path, 0x20, sizeof(packet.path));
    strcpy(packet.path, data_reference_path);

    x_plane_interface_send_udp_packet(
        (uint8_t *) packet.header,
        sizeof(packet) - sizeof(packet.padding)
    );
}

void x_plane_interface_subscribe_to_data_reference(
    const char *data_reference_path,
    data_reference_reply_index_e data_reference_reply_index,
    uint8_t reply_frequency
)
{
    rref_packet_s packet;

    strcpy(packet.header, "RREF");
    packet.reply_frequency = reply_frequency;
    packet.reply_index = data_reference_reply_index;
    memset(packet.path, 0x20, sizeof(packet.path));
    strcpy(packet.path, data_reference_path);

    x_plane_interface_send_udp_packet(
        (uint8_t *) packet.header,
        sizeof(packet) - sizeof(packet.padding)
    );
}

void x_plane_interface_send_udp_packet(const uint8_t *packet_header, uint16_t packet_size)
{
    udp.beginPacket(X_PLANE_COMPUTER_IP, X_PLANE_PORT);
    udp.write(packet_header, packet_size);
    udp.endPacket();
}

rref_reply_packet_s x_plane_interface_poll_for_packet(void)
{
    int packet_size = udp.parsePacket();

    if (packet_size)
    {
        udp.read(udp_receive_buffer, packet_size);

        if (strncmp(udp_receive_buffer, "RREF", 5) == 0)
        {
            // for (int offset = 5; offset < packet_size; offset += 8)
            // {
            rref_reply_packet_s packet = {
                .index = (int8_t) (*((int *) (udp_receive_buffer + 5))),
                .value = *((float *) (udp_receive_buffer + 9))
            };

            return packet;
            // }
        }
    }

    return (rref_reply_packet_s) { .index = -1, .value = -1 };
}