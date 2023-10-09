#include "x_plane_interface.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "definitions.h"

WiFiUDP udp;

void x_plane_interface_initialize(void)
{
    WiFi.mode(WIFI_STA);

    WiFi.config(
        X_PLANE_CONTROLLER_IP,
        X_PLANE_GATEWAY,
        { 255, 255, 255, 0 }
    );

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

#warning Add WiFi Timeout

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

void x_plane_interface_poll_for_packet(rref_reply_packet_s *packets)
{
    static char udp_receive_buffer[1024];

    int packetSize = udp.parsePacket();

    if (!packetSize)
    {
        packets[0].index = -1;
        return;
    }

    udp.read(udp_receive_buffer, packetSize);
    String type = String(udp_receive_buffer).substring(0, 4);

    if (type != "RREF")
    {
        packets[0].index = -1;
        return;
    }

    for (int offset = 5; offset < packetSize; offset += 8)
    {
        int index = *((int *) (udp_receive_buffer + offset));
        float value = *((float *) (udp_receive_buffer + offset + 4));

        packets[(offset - 5) / 8] = {
            .index = (int8_t) ((value == 0) ? -1 : index),
            .value = value
        };
    }
}
