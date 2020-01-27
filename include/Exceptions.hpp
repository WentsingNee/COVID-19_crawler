/**
 * @file       Exceptions.hpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef PNEUMONIAWUHAN2020_EXCEPTIONS_HPP
#define PNEUMONIAWUHAN2020_EXCEPTIONS_HPP

#include <stdexcept>

class ParseOutJSONFailed: public std::runtime_error
{
	public:

		virtual const char * what() const override
		{
			return "No data";
		}
};

#endif //PNEUMONIAWUHAN2020_EXCEPTIONS_HPP
