//
// Created by Washington on 29/02/2020.
//

#ifndef ENGINE3D_SRC_LOG_HPP
#define ENGINE3D_SRC_LOG_HPP

#include <string>
#include "singleton.hpp"
#include <ostream>
#include <vector>
#include <any>

class TLog {
public:
    TLog();

    template<class... Args>
    inline void v(const std::string& tag, const std::string& format, Args&& ...args) {
        Print(tag, "V", format, args...);
    }
    template<class... Args>
    inline void d(const std::string& tag, const std::string& format, Args&& ...args) {
        Print(tag, "D", format, args...);
    }
    template<class... Args>
    inline void i(const std::string& tag, const std::string& format, Args&& ...args) {
        Print(tag, "I", format, args...);
    }
    template<class... Args>
    inline void w(const std::string& tag, const std::string& format, Args&& ...args) {
        Print(tag, "W", format, args...);
    }
    template<class... Args>
    inline void e(const std::string& tag, const std::string& format, Args&& ...args) {
        PrintError(tag, "E", format, args...);
    }

protected:
    template<class... Args>
    void Print(const std::string& tag, const std::string& type, const std::string& format, Args&& ...args) {
        std::vector<std::any> vargs{args...};
        Printf(*stdOut, type, tag, format, vargs);
    }

    template<class... Args>
    void PrintError(const std::string& tag, const std::string& type, const std::string& format, Args&& ...args) {
        std::vector<std::any> vargs{args...};
        Printf(*stdErr, type, tag, format, vargs);
    }

    std::ostream *stdOut;
    std::ostream *stdErr;

private:
    static std::string GetTimestamp();
    static void Printf(std::ostream& stream, const std::string& type, const std::string& tag, const std::string& format, const std::vector<std::any>& args);
};

static TLog Log{};

#endif //ENGINE3D_SRC_LOG_HPP
