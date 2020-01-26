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

#include <cstdint>
#include <iostream>

#include <nlohmann/json_fwd.hpp>

class Statistics
{
	public:
		std::uint64_t createTime;
		std::uint64_t modifyTime;
		std::string imgUrl;
		std::string dailyPic;

		struct CountRemark
		{
				int diagnosed;
				int suspected;
				int dead;
				int cured;

				CountRemark() noexcept = default;
				explicit CountRemark(const std::string & raw);
		} countRemark;

	private:
		static nlohmann::json getJSON();

	public:
		static Statistics get();
};

#endif //PNEUMONIAWUHAN2020_DXY_STATISTICS_HPP
