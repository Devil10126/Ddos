#!/bin/bash

echo -e "\033[1;36m[*] Starting HTTPS DDoS Tool Setup...\033[0m"

# Update and upgrade system
sudo apt update && sudo apt upgrade -y

# Install required dependencies
echo -e "\033[1;32m[+] Installing dependencies...\033[0m"
sudo apt install -y clang libcurl4-openssl-dev curl tsocks macchanger tor || {
    echo -e "\033[1;31m[!] Dependency installation failed. Retrying...\033[0m"
    sleep 2
    sudo apt install -y clang libcurl4-openssl-dev curl tsocks macchanger tor
}

# Start Tor service
sudo systemctl start tor

# Final instructions
echo -e "\033[1;32m[✔] Setup complete.\033[0m"
echo -e "\033[1;34mCompile: clang++ DDoS_https.cpp -o DDoS_https -lcurl -lpthread\033[0m"
echo -e "\033[1;34mRun: ./DDoS_https\033[0m"
