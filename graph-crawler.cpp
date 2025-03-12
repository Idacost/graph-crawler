#include <iostream>
#include <queue>
#include <unordered_set>
#include <string>
#include <curl/curl.h>
#include <chrono>
#include <sstream>
#include "rapidjson/document.h"

using namespace std;
using namespace chrono;

string url_encode(const string& str) {
    ostringstream escaped;
    for (char c : str) {
        if (c == ' ') {
            escaped << "%20";
        } else {
            escaped << c;
        }
    }
    return escaped.str();
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string fetch_url(const string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) return "";

    string response_string;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK) ? response_string : "";
}

void bfs(const string& start_node, int max_depth) {
    queue<pair<string, int>> q;
    unordered_set<string> visited;
    
    q.push({start_node, 0});
    visited.insert(start_node);

    while (!q.empty()) {
        auto [node, depth] = q.front();
        q.pop();

        cout << "Depth " << depth << ": " << node << endl;
        if (depth >= max_depth) continue;

        string encoded_node = url_encode(node);
        string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/" + encoded_node;
        string response = fetch_url(url);
        if (response.empty()) continue;

        rapidjson::Document doc;
        if (doc.Parse(response.c_str()).HasParseError()) continue;

        if (doc.HasMember("neighbors") && doc["neighbors"].IsArray()) {
            for (auto& neighbor : doc["neighbors"].GetArray()) {
                string neighbor_name = neighbor.GetString();
                if (visited.find(neighbor_name) == visited.end()) {
                    q.push({neighbor_name, depth + 1});
                    visited.insert(neighbor_name);
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <start_node> <max_depth>" << endl;
        return 1;
    }

    string start_node = argv[1];
    int max_depth = stoi(argv[2]);

    auto start_time = high_resolution_clock::now();
    bfs(start_node, max_depth);
    auto end_time = high_resolution_clock::now();

    cout << "BFS completed in: " 
         << duration_cast<milliseconds>(end_time - start_time).count() 
         << " milliseconds" << endl;

    return 0;
}
