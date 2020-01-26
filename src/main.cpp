#include <Statistics.hpp>

#include <iostream>
#include <ctime>
#include <thread>
#include <sqlite3pp.h>

std::string unixTime2Str(time_t stamp)
{
	struct tm tm = *localtime(&stamp);
	char strTime[100];
	strftime(strTime, sizeof(strTime) - 1, "%Y-%m-%d %H:%M:%S", &tm);
	return std::string(strTime);
}

int main()
{
	using namespace std::chrono_literals;

	while (true) {
		try {
			Statistics ret = Statistics::get();
			std::cout << unixTime2Str(ret.createTime / 1000) << " | "
			          << unixTime2Str(ret.modifyTime / 1000) << " | "
			          << ret.countRemark.diagnosed << " | "
			          << ret.countRemark.suspected << " | "
			          << ret.countRemark.dead << " | "
			          << ret.countRemark.cured << std::endl;

			sqlite3pp::database db("PneumoniaWuhan2020.db");
			sqlite3pp::command(db, "CREATE TABLE IF NOT EXISTS statistics("
			                       "    createTime TEXT,"
			                       "    modifyTime TEXT PRIMARY KEY NOT NULL,"
			                       "    diagnosed INT,"
			                       "    suspected INT,"
			                       "    dead INT,"
			                       "    cured INT"
			                       ")").execute();

			sqlite3pp::command insert_cmd(db,
			                              "INSERT INTO statistics(createTime, modifyTime, diagnosed, suspected, dead, cured) VALUES(?, ?, ?, ?, ?, ?)");
			insert_cmd.bind(1, static_cast<long long int>(ret.createTime));
			insert_cmd.bind(2, static_cast<long long int>(ret.modifyTime));
			insert_cmd.bind(3, ret.countRemark.diagnosed);
			insert_cmd.bind(4, ret.countRemark.suspected);
			insert_cmd.bind(5, ret.countRemark.dead);
			insert_cmd.bind(6, ret.countRemark.cured);
			insert_cmd.execute();

		} catch (const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
		std::this_thread::sleep_for(30s);
	}
	return 0;
}
