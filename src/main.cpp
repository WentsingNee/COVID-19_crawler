#include <GetHTML.hpp>
#include <Statistics.hpp>
#include <AreaStat.hpp>
#include <log.hpp>

#include <iostream>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <sqlite3pp.h>
#include <nlohmann/json.hpp>

std::string unixTime2Str(time_t stamp)
{
	struct tm tm = *localtime(&stamp);
	char strTime[100];
	strftime(strTime, sizeof(strTime) - 1, "%Y-%m-%d %H:%M:%S", &tm);
	return std::string(strTime);
}

int main()
{
	using namespace std::string_literals;
	using namespace std::chrono_literals;

	LOG(DEBUG_LEVEL, "test", 123);
	LOG(INFO_LEVEL, "test", 123);
	LOG(WARNING_LEVEL, "test", 123);
	LOG(FATAL_LEVEL, "test", 123);

	try {
		cpr::Response r = getHTML();
		LOG(INFO_LEVEL, "HTML text length: ", r.text.length());

		{
			std::filesystem::create_directories("./HTML");
			std::ofstream fout("./HTML/"s + std::to_string(std::time(NULL)) + ".html", std::ios::out);
			fout << r.text << std::endl;
		}


		Statistics ret = Statistics::get(r.text);
		LOG(INFO_LEVEL,
		    unixTime2Str(ret.createTime / 1000), " | ",
		    unixTime2Str(ret.modifyTime / 1000), " | ",
		    ret.count.confirmed, " | ", ret.count.suspected, " | ",
		    ret.count.dead, " | ", ret.count.cured);

		{
			std::filesystem::create_directories("./Statistics");
			std::ofstream fout("./Statistics/"s + std::to_string(ret.modifyTime / 1000) + ".json", std::ios::out);
			fout << unixTime2Str(ret.createTime / 1000) << " | " <<
			     unixTime2Str(ret.modifyTime / 1000) << " | " <<
			     ret.count.confirmed << " | " << ret.count.suspected << " | " <<
			     ret.count.dead << " | " << ret.count.cured;
		}


		nlohmann::json json2 = AreaStat::getJSON(r.text);

		{
			std::filesystem::create_directories("./AreaStat");
			std::ofstream fout("./AreaStat/"s + std::to_string(ret.modifyTime / 1000) + ".json", std::ios::out);
			fout << json2.dump(4) << std::endl;
		}

		sqlite3pp::database db("PneumoniaWuhan2020.db");
		sqlite3pp::command(db, "CREATE TABLE IF NOT EXISTS statistics("
		                       "    createTime TEXT,"
		                       "    modifyTime TEXT PRIMARY KEY NOT NULL,"
		                       "    diagnosed INT,"
		                       "    suspected INT,"
		                       "    dead INT,"
		                       "    cured INT,"
		                       "    area TEXT"
		                       ")").execute();

		sqlite3pp::command insert_cmd(db,
		                              "INSERT INTO statistics(createTime, modifyTime, diagnosed, suspected, dead, cured, area) VALUES(?, ?, ?, ?, ?, ?, ?)");
		insert_cmd.bind(1, static_cast<long long int>(ret.createTime));
		insert_cmd.bind(2, static_cast<long long int>(ret.modifyTime));
		insert_cmd.bind(3, ret.count.confirmed);
		insert_cmd.bind(4, ret.count.suspected);
		insert_cmd.bind(5, ret.count.dead);
		insert_cmd.bind(6, ret.count.cured);
		insert_cmd.bind(7, json2.dump().c_str(), sqlite3pp::copy);
		insert_cmd.execute();



	} catch (const std::exception& e) {
		LOG(FATAL_LEVEL, e.what());
	}

	return 0;
}
