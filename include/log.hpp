/**
 * @file       log.hpp
 * @brief
 * @date       2020-01-27
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef PNEUMONIAWUHAN2020_LOG_HPP
#define PNEUMONIAWUHAN2020_LOG_HPP

#include <kerbal/utility/costream.hpp>

struct DEBUG_LEVEL
{
		constexpr static const auto stream = kerbal::utility::costream::costream<std::cout>(kerbal::utility::costream::PURPLE);
		constexpr static const char * name = "DEBUG";
};

struct INFO_LEVEL
{
		constexpr static const auto stream = kerbal::utility::costream::costream<std::cout>(kerbal::utility::costream::LIGHT_BLUE);
		constexpr static const char * name = "INFO";
};

struct WARNING_LEVEL
{
		constexpr static const auto stream = kerbal::utility::costream::costream<std::cout>(kerbal::utility::costream::LIGHT_YELLOW);
		constexpr static const char * name = "WARNING";
};

struct FATAL_LEVEL
{
		constexpr static const auto stream = kerbal::utility::costream::costream<std::cout>(kerbal::utility::costream::LIGHT_RED);
		constexpr static const char * name = "FATAL";
};

template <typename Level, typename ... Args>
void __log(const char * file, int line, Args&& ... args)
{
	Level::stream << "[" << Level::name << "] " << file << ":" << line << "   ";
	(Level::stream << ... << args);
	Level::stream << std::endl;
}

#ifndef LOG
#define LOG(level, ...) \
do {\
    __log<level>(__FILE__, __LINE__, ##__VA_ARGS__);\
} while(false);
#endif

#endif //PNEUMONIAWUHAN2020_LOG_HPP
