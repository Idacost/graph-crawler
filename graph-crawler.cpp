#include <curl/curl.h>
#include "rapidjson/document.h"
#include <string>
#include <iostream>
#include <chrono>
#include <bits/stdc++.h>
using namespace std;

size_t my_write_data(char *ptr, size_t size, size_t nmemb, void *userdata) {
    std::string* mystring = (std::string*)userdata;

    for (size_t i = 0; i<nmemb; ++i) {
        mystring->push_back(ptr[i]);
    }

    return nmemb;
}

std::string fetch_neighbors(const std::string& node) {
    CURL *curl = curl_easy_init();
    string base_url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
    char* encoded_node = curl_easy_escape(curl, node.c_str(), 0);
    std::string url = base_url + encoded_node;
    curl_free(encoded_node); 

    std::string myoutstring;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&myoutstring);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_data);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    return myoutstring;
}

void bfs(const std::string& start_node, int depth) {
    std::queue<std::string> to_visit;
    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> current_level;

    to_visit.push(start_node);
    visited.insert(start_node);

    int level = 0;
    std::cout << "\nLevel " << level << ": " << start_node << "\n";

    int max_depth = depth; 
    while (!to_visit.empty()) {
        if (level >= max_depth) {
            break;  
        }

        size_t level_size = to_visit.size();
        current_level.clear();

        for (size_t i = 0; i < level_size; ++i) {
            std::string current_node = to_visit.front();
            to_visit.pop();

            std::string myoutstring = fetch_neighbors(current_node);
            if (myoutstring.empty()) {
                continue;
            }

            using namespace rapidjson;
            Document document;
            document.Parse(myoutstring.c_str());

            const Value& neighbors = document["neighbors"];
            for (SizeType j = 0; j < neighbors.Size(); ++j) {
                std::string neighbor = neighbors[j].GetString();

                if (visited.find(neighbor) == visited.end()) {
                    visited.insert(neighbor);
                    to_visit.push(neighbor);
                    current_level.insert(neighbor);
                }
            }
        }

        if (!current_level.empty()) {
            std::cout << "\nLevel " << level + 1 << ": ";
            for (const auto& node : current_level) {
                std::cout << node << ", ";
            }
            std::cout << "\n";
        }

        ++level;
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: ./graph-crawler <start_node> <depth>" << std::endl;
        return 1;
    }

    std::string node = argv[1];
    int depth = std::stoi(argv[2]);

    // Start time measurement
    auto start_time = std::chrono::high_resolution_clock::now();

    bfs(node, depth);

    // End time measurement
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "\nBFS completed in " << duration.count() << " milliseconds.\n";

/*
    std::string myoutstring = fetch_neighbors(node);
    
    if (myoutstring.empty()) {
        return 1; 
    }
    
    using namespace rapidjson;
    Document document;
    document.Parse(myoutstring.c_str());

    std::cout << "neighbors: " << "\n";
    std::cout << "{" << "\n";
    const Value& neighbors = document["neighbors"];
    for (SizeType i = 0; i < neighbors.Size(); i++)
    {
        std::cout<< neighbors[i].GetString() << "\n";
    }
    std::cout << "}" << "\n";

    std::string hh = document["node"].GetString();
    std::cout<<hh<<"\n";
    */

    //std::cout<<myoutstring<<"\n";
    return 0;
}