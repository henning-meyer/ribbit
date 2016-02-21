/*
 * pages.cxx
 *
 *  Created on: Feb 21, 2016
 *      Author: hmeyer
 */

#include "pages.hxx"

namespace {
const std::string METHOD_ERROR = "<html><head><title>Illegal request</title></head><body>Go away.</body></html>";
const std::string NOT_FOUND_ERROR = "<html><head><title>Not found</title></head><body>Go away.</body></html>";
const std::string MAIN_PAGE = "<html><head><title>Welcome</title></head><body><form action=\"/2\" method=\"post\">What is your name? <input type=\"text\" name=\"v1\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>";

const std::string SECOND_PAGE = "<html><head><title>Tell me more</title></head><body><a href=\"/\">previous</a> <form action=\"/S\" method=\"post\">%s, what is your job? <input type=\"text\" name=\"v2\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>";
const std::string SUBMIT_PAGE = "<html><head><title>Ready to submit?</title></head><body><form action=\"/F\" method=\"post\"><a href=\"/2\">previous </a> <input type=\"hidden\" name=\"DONE\" value=\"yes\" /><input type=\"submit\" value=\"Submit\" /></body></html>";
const std::string LAST_PAGE = "<html><head><title>Thank you</title></head><body>Thank you.</body></html>";


}


std::string getPageSource(PageSource p) {
	using Page = PageSource;
	switch (p) {
	case Page::last_page:
		return LAST_PAGE;
	case Page::main_page:
		return MAIN_PAGE;
	case Page::method_error:
		return METHOD_ERROR;
	case Page::not_found_error:
		return NOT_FOUND_ERROR;
	case Page::second_page:
		return SECOND_PAGE;
	case Page::submit_page:
		return SUBMIT_PAGE;
	default:
		return METHOD_ERROR;
	}
}

