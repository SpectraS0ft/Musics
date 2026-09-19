#ifndef LOGGER_H
#define LOGGER_H

#include <android/log.h>
#include <string>
#include <sstream>

#define LOG_TAG "SpaceBattle"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace Utils {

class Logger {
public:
    static void debug(const std::string& msg) {
        LOGD("%s", msg.c_str());
    }
    
    static void info(const std::string& msg) {
        LOGI("%s", msg.c_str());
    }
    
    static void warning(const std::string& msg) {
        LOGW("%s", msg.c_str());
    }
    
    static void error(const std::string& msg) {
        LOGE("%s", msg.c_str());
    }
    
    template<typename T>
    static void debug(const T& val) {
        std::stringstream ss;
        ss << val;
        debug(ss.str());
    }
};

} // namespace Utils

#endif // LOGGER_H
