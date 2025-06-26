#include <iostream>
#include <chrono>
#include <iomanip>

#ifdef WIN32
#define LOCALTIME_SAFETY(timet, t) ::localtime_s(&t, &timet)
#elif linux
#define LOCALTIME_SAFETY(timet, t) ::localtime_r(&timet, &t)
#endif

#define LOG_INFO(msg)                                                                                                                                 \
	{                                                                                                                                                 \
		auto now = std::chrono::system_clock::now();                                                                                                  \
		auto duration = now.time_since_epoch();                                                                                                       \
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();                                                  \
		std::time_t time_now = std::chrono::system_clock::to_time_t(now);                                                                             \
		std::tm time_tm;                                                                                                                              \
		LOCALTIME_SAFETY(time_now, time_tm);                                                                                                          \
		std::cout << "[" << std::put_time(&time_tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << (milliseconds % 1000) << "] " \
				  << " " << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ") - " << "\n\033[32m[INFO] " << msg << "\033[0m" << std::endl;    \
	}

#define LOG_ERROR(msg)                                                                                                                                \
	{                                                                                                                                                 \
		auto now = std::chrono::system_clock::now();                                                                                                  \
		auto duration = now.time_since_epoch();                                                                                                       \
		auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();                                                  \
		std::time_t time_now = std::chrono::system_clock::to_time_t(now);                                                                             \
		std::tm time_tm;                                                                                                                              \
		LOCALTIME_SAFETY(time_now, time_tm);                                                                                                          \
		std::cout << "[" << std::put_time(&time_tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << (milliseconds % 1000) << "] " \
				  << " " << __FILE__ << ":" << __LINE__ << " (" << __FUNCTION__ << ") - " << "\n\033[31m[ERROR] " << msg << "\033[0m" << std::endl;   \
	}

#define LOG_WARN(msg)
