#!/bin/sh

function start()
{
    echo "DISABLING INTERNET ACCESS..."
    sudo ip link set wlan0 down

    echo "CREATING HOTSPOT CONFIGURATION..."
    sudo create_ap wlan0 wlan0 'XPlaneNetwork' 'xplane11net' --mkconfig /etc/create_ap.conf --freq-band 2.4

    echo "STARTING HOTSPOT..."
    sudo create_ap --daemon wlan0 wlan0 'XPlaneNetwork' 'xplane11net'
}

function stop()
{
    echo "STOPPING HOTSPOT..."
    sudo create_ap --stop wlan0

    echo "RESTORING INTERNET ACCESS..."
    sudo ip link set wlan0 up
}

if [[ $1 == "start" ]]; then
    start 
elif [[ $1 == "stop" ]]; then
    stop 
else
    echo "Invalid action. Use either 'start' or 'stop'."
fi
