#!/bin/bash

# Update and Upgrade system
echo "[+] Updating system..."
sudo apt update && sudo apt upgrade -y

# Install required dependencies
echo "[+] Installing dependencies..."
sudo apt install -y curl libcurl4-openssl-dev libssl-dev macchanger tor build-essential

# Ensure that macchanger is available
echo "[+] Checking macchanger availability..."
if ! command -v macchanger &> /dev/null
then
    echo "[!] macchanger could not be found! Please install macchanger manually."
    exit 1
fi

# Ensure that tor is available
echo "[+] Checking Tor availability..."
if ! command -v tor &> /dev/null
then
    echo "[!] Tor could not be found! Installing Tor..."
    sudo apt install -y tor
fi

# Check if Tor is running
echo "[+] Starting Tor service..."
sudo systemctl start tor
sudo systemctl enable tor

# Ensure curl is installed and working
echo "[+] Checking curl availability..."
if ! command -v curl &> /dev/null
then
    echo "[!] curl could not be found! Installing curl..."
    sudo apt install -y curl
fi

# Setup permissions for macchanger and tor
echo "[+] Setting up sudo permissions for macchanger and tor..."
sudo usermod -aG sudo $USER

# Final message
echo "[✔] Setup complete. Dependencies installed and configured."

# Reminder for system reboot (if necessary)
echo "[+] If any issues persist, try rebooting your system: sudo reboot"
