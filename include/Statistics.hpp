/**
 * @file       Statistics.hpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef PNEUMONIAWUHAN2020_DXY_STATISTICS_HPP
#define PNEUMONIAWUHAN2020_DXY_STATISTICS_HPP

#include <Count.hpp>

#include <cstdint>
#include <nlohmann/json_fwd.hpp>

class Statistics
{
	public:
		std::uint64_t createTime;
		std::uint64_t modifyTime;
		std::string imgUrl;
		std::string dailyPic;

		Count count;

	private:
		static nlohmann::json getJSON(const std::string & text);
		static Count parseCount(const std::string & raw);

	public:
		static Statistics get(const std::string & text);
};

#endif //PNEUMONIAWUHAN2020_DXY_STATISTICS_HPP
