#pragma once

#include <sstream>
#include <vector>
#include <regex>
#include <string>

bool isXML(std::string s){
    if(std::regex_search(s, std::regex("<\?xml.*?>"))){
        return true;
    }
    
    return false;
}

std::string unmarshalMessage(std::string s){
    //https://regex101.com/r/zLT2Kh/1
    std::regex rgx("(<Response><Message>)([\\S\\s]*)(<\/Message><\/Response>)");
    std::smatch matches;

    if(std::regex_search(s, matches, rgx)){
        if(matches.size() == 1+3){
            return matches.str(2);
        }
    }

    //https://regex101.com/r/UmAGlW/1
    if(std::regex_match(s, matches, std::regex(R"(<\?xml.*[?]><Response\s*?/>)"))){
        return "<no response>";
    }

    return std::string("");
}

std::string sendMessage(std::string address, std::string number, std::string input){
    std::string msg;

    try
    {
        http::Request request(address);
        std::map<std::string, std::string> parameters = {
            {"Body", input}, 
            {"From", "whatsapp:" + number}
        };

        const http::Response response = request.send("POST", parameters, {
            "Content-Type: application/x-www-form-urlencoded"
        });
        std::string respBody = std::string(response.body.begin(), response.body.end());
        if(!isXML(respBody)){
            throw std::runtime_error("No XML: " + respBody);
        }

        msg = unmarshalMessage(respBody);
        if(msg.size() == 0){
            printf("%s\n", respBody.c_str());
            throw std::runtime_error("Message could not be unmarshalled"); 
        }
    }
    catch (const std::exception& e)
    {
        msg = "Request failed, error: ";
        msg += e.what();
    }

    return msg;
}