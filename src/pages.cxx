/*
 * pages.cxx
 *
 *  Created on: Feb 21, 2016
 *      Author: hmeyer
 */

#include "pages.hxx"

#include <fstream>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "html_file.hxx"


#include <map>

namespace {
const std::string METHOD_ERROR = "<html><head><title>Illegal request</title></head><body>Go away.</body></html>";
const std::string NOT_FOUND_ERROR = "<html><head><title>Not found</title></head><body>Go away.</body></html>";
const std::string MAIN_PAGE = "<html><head><title>Welcome</title></head><body><form action=\"/2\" method=\"post\">What is your name? <input type=\"text\" name=\"v1\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>";

const std::string SECOND_PAGE = "<html><head><title>Tell me more</title></head><body><a href=\"/\">previous</a> <form action=\"/S\" method=\"post\">%s, what is your job? <input type=\"text\" name=\"v2\" value=\"%s\" /><input type=\"submit\" value=\"Next\" /></body></html>";
const std::string SUBMIT_PAGE = "<html><head><title>Ready to submit?</title></head><body><form action=\"/F\" method=\"post\"><a href=\"/2\">previous </a> <input type=\"hidden\" name=\"DONE\" value=\"yes\" /><input type=\"submit\" value=\"Submit\" /></body></html>";
const std::string LAST_PAGE = "<html><head><title>Thank you</title></head><body>Thank you. <img src='face-smile.png'/></body></html>";


}

const std::string smile_src = "face-smile.png";
const std::string fav_src = "favicon.ico";
const std::string css_src = "default.css";


std::string smile_f;
std::string favicon_ico;
std::string default_css;

std::string html_page;

std::string getHtmlPage() {
	if (!html_page.empty()) {
		return html_page;
	}


	std::ostringstream os;
	os << "Hello World!\n";

	for(int i = 1; i <= 3; i++) {
		os << i << " ";
		render_selfclosing(tag::br, os);
	}


	/*
	 * need http / https / local server, that accepts forms
	 */

	std::ostringstream no;
	no << minimal_html("Hello World",os.str()) << std::endl;
	html_page = no.str();

	return html_page;
}

enum class StaticFile {
	favicon,
	smile_image,
	css
};

std::string getSmileImage(StaticFile file) {

	std::string* s = nullptr;
	const std::string* f = nullptr;

	switch(file) {
	case StaticFile::favicon:
		s = &favicon_ico;
		f = &fav_src;
		break;
	case StaticFile::smile_image:
		s = &smile_f;
		f = &smile_src;
		break;
	case StaticFile::css:
		s = &default_css;
		f = &css_src;
		break;
	default:
		return {};
	}
	std::string& smile = *s;
	const std::string& filep = *f;
	if(s->size() != 0) {
		return smile;
	}
	int fd = open (filep.c_str(), O_RDONLY);
	if ( fd > 0 ) {

		struct stat sbuf;
		fstat (fd, &sbuf);

		ssize_t nbyte = sbuf.st_size;
		smile = std::string(nbyte, 0);

		ssize_t r = read(fd, &smile[0], nbyte);
		close(fd);
		if(r == nbyte) {
			printf("success %s %02x %02x %02x %02x:) \n", filep.c_str(),smile[0]&255, smile[1]&255, smile[2]&255, smile[3]&255);
			return smile;
		}
	}
	return {};



}

std::map<PageSource, std::shared_ptr<const std::vector<char>>> cache;

inline std::shared_ptr<const std::vector<char>> convert(const std::string& s) {
	auto p = std::make_shared<std::vector<char>>(s.size());
	std::copy(s.begin(), s.end(), p->begin());
	return p;
}

std::shared_ptr<const std::vector<char>> getPageSource(PageSource p) {
	using Page = PageSource;

	auto iter = cache.find(p);
	if(iter != cache.end()) {
		return iter->second;
	}

	std::shared_ptr<const std::vector<char>> sp;
	switch (p) {
	case Page::last_page:
		sp = convert(LAST_PAGE);
		break;
	case Page::main_page:
		sp = convert(getHtmlPage());
		break;
	case Page::method_error:
		sp = convert(METHOD_ERROR);
		break;
	case Page::not_found_error:
		sp = convert(NOT_FOUND_ERROR);
		break;
	case Page::second_page:
		sp = convert(SECOND_PAGE);
		break;
	case Page::submit_page:
		sp = convert(SUBMIT_PAGE);
		break;
	case Page::smile_image:
		sp = convert(getSmileImage(StaticFile::smile_image));
		break;
	case Page::css_file:
		sp = convert(getSmileImage(StaticFile::css));
		break;
	case Page::favicon_file:
		sp = convert(getSmileImage(StaticFile::favicon));
		break;
	default:
		sp = convert(METHOD_ERROR);
	}
	cache[p] = sp;
	return sp;
}

