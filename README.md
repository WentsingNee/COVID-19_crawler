# COVID-19_crawler
COVID-19 疫情动态爬虫

数据来源: [丁香园](https://3g.dxy.cn/newh5/view/pneumonia).


## Requirements

* a C++17 compatible compiler such as GCC or Clang.
* CMake (recommended).
* [CPR](https://github.com/whoshuu/cpr), a simple C++ wrapper around libcurl.
* [nlohmann json](https://github.com/azadkuh/nlohmann_json_release), a C++ JSON parser.
* [sqlite3pp](https://github.com/iwongu/sqlite3pp), a C++ wrapper of SQLite3 API.


# Note

If your GCC version is under 9.1.0 (7.x, 8.x, etc.), you need to link libstdc++fs.a .
