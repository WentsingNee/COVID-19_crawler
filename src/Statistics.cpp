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
		for (std::smatch::size_type i = 0; i < sm.size(); ++i) {
			LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
		}
		throw std::runtime_error("No statistics");
	}

	nlohmann::json json;

	try {
		json = nlohmann::json::parse(sm[1].str());
	} catch (const nlohmann::json::parse_error & e) {
		for (std::smatch::size_type i = 0; i < sm.size(); ++i) {
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
	ret.count = Statistics::parseCount(json);
	return ret;
}

Count Statistics::parseCount(const nlohmann::json & json)
{
	LOG(INFO_LEVEL, __FUNCTION__);
	using namespace std::string_literals;

	Count count{};

	try {
		constexpr const std::pair<int Count::*, const char*> GROUP[] = {
				{&Count::confirmed, "confirmedCount"},
				{&Count::suspected, "suspectedCount"},
				{&Count::dead,      "deadCount"},
				{&Count::cured,     "curedCount"},
		};

		for (const auto &[pro, KEY] : GROUP) {
			LOG(DEBUG_LEVEL, "KEY: ", KEY);
			if (json.find(KEY) == json.cend()) {
				LOG(FATAL_LEVEL, "No KEY found: ", KEY);
				throw std::runtime_error("No KEY found");
			}
			int c = json[KEY];
			LOG(INFO_LEVEL, KEY, ": ", c);
			count.*pro = c;
		}
	} catch (...) {
		LOG(FATAL_LEVEL, "json: ", json.dump());
		throw;
	}

	return count;
}