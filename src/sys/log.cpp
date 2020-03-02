//
// Created by Washington on 29/02/2020.
//

#include "log.hpp"
#include <chrono>
#define __STDC_WANT_LIB_EXT1__ 1
#include <ctime>
#include <iostream>

TLog::TLog() {
    stdOut = &std::cout;
    stdErr = &std::cerr;
}

std::string TLog::GetTimestamp() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::string buffer(256, 0);
    int len = std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d %H:%M:%S UTC", std::gmtime(&now));
    int i;
    for(i = len-1; i >= 0; i--) {
        if(!std::isspace(buffer[i])) break;
    }
    return buffer.substr(0, len);
}

void
TLog::Printf(std::ostream &stream, const std::string &type, const std::string &tag, const std::string &format,
             const std::vector<std::any> &args) {
    stream << GetTimestamp() << std::string(1, ' ') << type << "/" << tag <<  ": ";
    int arg = 0;
    bool isLong = false;
    for(size_t i = 0; i < format.size(); i++) {
        if(format[i] == '%') {
            i++;
            switch(format[i]) {
                case 'd':
                case 'i':
                    if(isLong) {
                        stream << std::any_cast<int>(args[arg]);
                        isLong = false;
                    } else {
                        stream << std::any_cast<long int>(args[arg]);
                    }
                    break;
                case 'u':
                    if(isLong) {
                        stream << std::any_cast<long unsigned int>(args[arg]);
                        isLong = false;
                    } else {
                        stream << std::any_cast<unsigned int>(args[arg]);
                    }
                    break;
                case 'f':
                case 'F':
                    if(isLong) {
                        stream << std::any_cast<double>(args[arg]);
                        isLong = false;
                    } else {
                        stream << std::any_cast<float>(args[arg]);
                    }
                    break;
                case 'c':
                case 'C':
                    stream << std::any_cast<char>(args[arg]);
                    break;
                case 's':
                    stream << std::any_cast<std::string>(args[arg]);
                    break;
                case 'p':
                    stream << std::any_cast<void*>(args[arg]);
                    break;
                case 'l':
                    isLong = true;
                    break;
                default:
                    throw std::runtime_error(std::string("Format %") + format[i] + "is not supported");
            }
            arg++;
        } else {
            stream << format[i];
        }
    }
    stream << std::endl;
}
