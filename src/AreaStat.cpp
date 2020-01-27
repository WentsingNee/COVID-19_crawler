/**
 * @file       AreaStat.cpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#include <AreaStat.hpp>
#include <log.hpp>

#include <regex>
#include <iostream>
#include <nlohmann/json.hpp>

nlohmann::json AreaStat::getJSON(const std::string & text)
{
	LOG(INFO_LEVEL, __FUNCTION__);

	if constexpr (false) {
		constexpr const char REGEX[] = R"(<script id="getAreaStat">try \{ window.getAreaStat = ([^<]*)}catch\(e\)\{}</script>)";
		std::regex rgx(REGEX);
		std::smatch sm;

		if (!std::regex_search(text, sm, rgx) && sm.size() < 2) {
			for (int i = 0; i < sm.size(); ++i) {
				LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
			}
			throw std::runtime_error("No area statistics");
		}
		nlohmann::json json;

		try {
			json = nlohmann::json::parse(sm[1].str());
		} catch (const nlohmann::json::parse_error& e) {
			for (int i = 0; i < sm.size(); ++i) {
				LOG(FATAL_LEVEL, "[", i, "]  ", sm[i]);
			}
			throw std::runtime_error("Json parse failed!");
		}
		return json;

	} else {
		constexpr const char HEAD[] = R"(<script id="getAreaStat">try { window.getAreaStat =)";
		constexpr const char TAIL[] = R"(}catch(e){}</script>)";
		int begin = text.find(HEAD);
		int end = text.find(TAIL, begin);

		if (begin == -1 or end == -1) {
			LOG(FATAL_LEVEL, "begin: ", begin, "  end: ", end);
			throw std::runtime_error("No area statistics");
		}

		std::string content(text.begin() + begin + strlen(HEAD), text.begin() + end);
		nlohmann::json json;

		try {
			json = nlohmann::json::parse(content);
		} catch (const nlohmann::json::parse_error& e) {
			LOG(FATAL_LEVEL, "content: ", content);
			throw std::runtime_error("Json parse failed!");
		}
		return json;
	}

}

AreaStat AreaStat::get(const std::string & text)
{
	LOG(INFO_LEVEL, __FUNCTION__);
	nlohmann::json json = AreaStat::getJSON(text);
	std::cout << json.dump(4) << std::endl;
	AreaStat ret;
	return ret;
}