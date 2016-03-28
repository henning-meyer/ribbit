/*
 * pages.hxx
 *
 *  Created on: Feb 21, 2016
 *      Author: hmeyer
 */

#ifndef PAGES_HXX_
#define PAGES_HXX_

#include <string>
#include <vector>
#include <memory>

enum class PageSource {
	method_error,
	not_found_error,
	main_page,
	second_page,
	submit_page,
	last_page,
	smile_image,
	css_file,
	favicon_file
};

std::shared_ptr<const std::vector<char>> getPageSource(PageSource p);


#endif /* PAGES_HXX_ */
