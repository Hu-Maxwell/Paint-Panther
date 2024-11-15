#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <future>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "../include/ApiHandler.h"  

// ==========================
// i stole this from online and i have no idea what this does
std::string base64_encode(const std::vector<unsigned char>& data) {
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    int val = 0, valb = -6;
    for (unsigned char c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (encoded.size() % 4) encoded.push_back('=');
    return encoded;
}

std::string read_image_base64(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open image file.");
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)), {});
    return "data:image/png;base64," + base64_encode(buffer);
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// ==========================

std::string getResponse() {
    const std::string api_key = "";
    const std::string image_path = "C:/Users/deweb/source/repos/Paint Panther/Paint Panther/output.png"; // incorrect filepath for all 

    try {
        std::string base64_image = read_image_base64(image_path);

        // construct the JSON payload
        nlohmann::json payload = {
            {"model", "gpt-4o-mini"},
            {"messages", {
                {
                    {"role", "user"},
                    {"content", {
                        {{"type", "text"}, {"text", "give me one critique in one or two sentences to improve on my drawing next."}},
                        {{"type", "image_url"}, {"image_url", {{"url", base64_image}}}}
                    }}
                }
            }},
            {"max_tokens", 300}
        };

        CURL* curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("cURL initialization failed");
        }

        // creates header (input type + api key) for cURL
        std::string response;
        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

        // initializes cURL link w/ header (input type + api key)
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // turns the input (json) into string and sends w/ POST
        std::string payload_str = payload.dump();
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());

        // get response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // sends the http req
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);

        return response;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::future<std::string> getResponseAsync() {
    return std::async(std::launch::async, getResponse);
}