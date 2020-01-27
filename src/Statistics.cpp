/**
 * @file       Statistics.cpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#include <Statistics.hpp>
#include <log.hpp>

#include <regex>
#include <iostream>
#include <nlohmann/json.hpp>

nlohmann::json Statistics::getJSON(const std::string & text)
{
	LOG(INFO_LEVEL, __FUNCTION__);
	constexpr const char REGEX[] = R"(<script id="getStatisticsService">try \{ window.getStatisticsService = ([^<]*)}catch\(e\)\{}</script>)";
	std::regex rgx(REGEX);
	std::smatch sm;

	if (!std::regex_search(text, sm, rgx) && sm.size() < 2) {
		for (int i = 0; i < sm.size(); ++i) {
			LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
		}
		throw std::runtime_error("No statistics");
	}

	nlohmann::json json;

	try {
		json = nlohmann::json::parse(sm[1].str());
	} catch (const nlohmann::json::parse_error & e) {
		for (int i = 0; i < sm.size(); ++i) {
			LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
		}
		throw std::runtime_error("Json parse failed!");
	}
	return json;
}


Statistics Statistics::get(const std::string & text)
{
	LOG(INFO_LEVEL, __FUNCTION__);
	nlohmann::json json = Statistics::getJSON(text);
	Statistics ret;
	ret.createTime = json["createTime"];
	ret.modifyTime = json["modifyTime"];
	ret.count = Statistics::parseCount(json["countRemark"]);
	return ret;
}

Count Statistics::parseCount(const std::string & raw)
{
	LOG(INFO_LEVEL, __FUNCTION__);
	using namespace std::string_literals;

	Count count;

	try {
		constexpr const std::pair<int Count::*, const char*> GROUP[] = {
				{&Count::confirmed, "确诊"},
				{&Count::suspected, "疑似"},
				{&Count::dead,      "死亡"},
				{&Count::cured,     "治愈"},
		};

		for (const auto &[pro, KEY] : GROUP) {
			LOG(DEBUG_LEVEL, "KEY: ", KEY);
			std::regex rgx(KEY + " (\\d+) 例"s);
			std::smatch sm;
			if (!std::regex_search(raw, sm, rgx) && sm.size() < 2) {
				LOG(FATAL_LEVEL, "KEY: ", KEY);
				for (int i = 0; i < sm.size(); ++i) {
					LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
				}
				throw std::runtime_error("Parse failed");
			}
			LOG(INFO_LEVEL, "KEY: ", KEY, "  ", sm[1].str());
			count.*pro = std::stoi(sm[1].str());
		}
	} catch (...) {
		std::cerr << raw << std::endl;
		throw;
	}

	return count;
}