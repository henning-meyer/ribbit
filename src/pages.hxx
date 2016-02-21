/*
 * pages.hxx
 *
 *  Created on: Feb 21, 2016
 *      Author: hmeyer
 */

#ifndef PAGES_HXX_
#define PAGES_HXX_

#include <string>

enum class PageSource {
	method_error,
	not_found_error,
	main_page,
	second_page,
	submit_page,
	last_page
};

std::string getPageSource(PageSource p);


#endif /* PAGES_HXX_ */
