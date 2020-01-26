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

#include <regex>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

nlohmann::json Statistics::getJSON()
{
	constexpr const char URL[] = "https://3g.dxy.cn/newh5/view/pneumonia";
	auto r = cpr::Get(cpr::Url{URL});

	if (r.status_code != 200) {
		std::cout << "status code: " << r.status_code << std::endl;
	}

	constexpr const char REGEX[] = R"(<script id="getStatisticsService">try \{ window.getStatisticsService = ([^<]*)}catch\(e\)\{}</script>)";
	std::regex rgx(REGEX);
	std::smatch sm;

	if (!std::regex_search(r.text, sm, rgx) && sm.size() < 2) {
		std::cerr << r.text << std::endl;
		for (int i = 0; i < sm.size(); ++i) {
			std::cerr << "[" << i << "]" << sm[i] << std::endl;
		}
		throw std::runtime_error("No statistics");
	}

	nlohmann::json json;

	try {
		json = nlohmann::json::parse(sm[1].str());
	} catch (const nlohmann::json::parse_error & e) {
		for (int i = 0; i < sm.size(); ++i) {
			std::cerr << "[" << i << "]   " << sm[i] << std::endl;
		}
		throw std::runtime_error("Json parse failed!");
	}
	return json;
}


Statistics Statistics::get()
{
	nlohmann::json json = Statistics::getJSON();
	Statistics ret;
	ret.createTime = json["createTime"];
	ret.modifyTime = json["modifyTime"];
	ret.countRemark = Statistics::CountRemark(json["countRemark"]);
	return ret;
}

Statistics::CountRemark::CountRemark(const std::string & raw)
{
	using namespace std::string_literals;

	try {
		constexpr const std::pair<int Statistics::CountRemark::*, const char*> GROUP[] = {
				{&Statistics::CountRemark::diagnosed, "确诊"},
				{&Statistics::CountRemark::suspected, "疑似"},
				{&Statistics::CountRemark::dead,      "死亡"},
				{&Statistics::CountRemark::cured,     "治愈"},
		};

		for (const auto &[pro, KEY] : GROUP) {
			std::regex rgx(KEY + " (\\d+) 例"s);
			std::smatch sm;
			if (!std::regex_search(raw, sm, rgx) && sm.size() < 2) {
				std::cerr << "Failed!" << std::endl;
				std::cerr << "KEY: " << KEY << std::endl;
				throw std::runtime_error("Parse failed");
			}
			this->*pro = std::stoi(sm[1].str());
		}
	} catch (...) {
		std::cerr << raw << std::endl;
		throw;
	}
}