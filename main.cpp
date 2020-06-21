/*
#include <iostream>
#include <string>
#include <regex>

int main ()
{
  std::string s ("<?xml version='1.0' encoding='UTF-8'?><Response><Message>Please enter your verification code:</Message></Response>");
  std::smatch m;
  std::regex e ("(.*)(<Response><Message>)(.*)(<\/Message><\/Response>)");

  std::regex_match ( s, m, e );

  for (unsigned i=0; i<m.size(); ++i) {
    std::cout << "match " << i << " (" << m[i] << ")";
    std::cout << " has a length of " << m.length(i) << std::endl;
  }

  return 0;
}
*/

#include "HTTPRequest.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>
#include "util.cpp"

int main(int argc, char* argv[]) {
    std::string address = "http://localhost:5000/bot";
    std::string number = "+4900000000000";
    std::string input = "";

    for(int i = 0; i < argc; ++i){
        if(std::string(argv[i]) == "-a" || std::string(argv[i]) == "--address"){
            if(i+1 <= argc-1){
                address = std::string(argv[i+1]);
                i++;
            }
        }else if(std::string(argv[i]) == "-n" || std::string(argv[i]) == "--number"){
            if(i+1 <= argc-1){
                number = std::string(argv[i+1]);
                i++;
            }
        }else{
            input = std::string(argv[i]);
        }
    }

    while(true){
        if(input.size() == 0){
            std::cout << "\u001b[32m<\u001b[0m ";
            std::cin >> input;
        }

        std::string resp = sendMessage(address, number, input);

        if(resp == "<no response>"){
            break;
        }

        std::cout << "\u001b[31m>\u001b[0m " << resp << std::endl;
        input = "";
    }

    return 0;

}