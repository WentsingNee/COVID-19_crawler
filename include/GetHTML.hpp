/**
 * @file       GetHTML.hpp
 * @brief
 * @date       2020-01-26
 * @author     Peter
 * @copyright
 *      Peter of [ThinkSpirit Laboratory](http://thinkspirit.org/)
 *   of [Nanjing University of Information Science & Technology](http://www.nuist.edu.cn/)
 *   all rights reserved
 */

#ifndef PNEUMONIAWUHAN2020_GETHTML_HPP
#define PNEUMONIAWUHAN2020_GETHTML_HPP

#include <cpr/cpr.h>

#include <iostream>

cpr::Response getHTML()
{
	constexpr const char URL[] = "https://3g.dxy.cn/newh5/view/pneumonia";
	cpr::Response r = cpr::Get(cpr::Url{URL});

	if (r.status_code != 200) {
		std::cout << "status code: " << r.status_code << std::endl;
	}

	return r;
}

#endif //PNEUMONIAWUHAN2020_GETHTML_HPP
