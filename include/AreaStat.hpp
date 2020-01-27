/**
 * @file       AreaStat.hpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef PNEUMONIAWUHAN2020_AREASTAT_HPP
#define PNEUMONIAWUHAN2020_AREASTAT_HPP

#include <Count.hpp>

#include <cstdint>
#include <vector>
#include <nlohmann/json_fwd.hpp>

class AreaStat
{
	public:

		struct City
		{
				std::string name;
				Count count;
		};

		struct Province
		{
				std::string name;
				Count count;

				std::vector<City> cities;
		};

		std::vector<Province> Provinces;

	public:
		static nlohmann::json getJSON(const std::string & text);

	public:
		static AreaStat get(const std::string & text);
};

#endif //PNEUMONIAWUHAN2020_AREASTAT_HPP
