/*
 * post.cxx
 *
 *  Created on: Apr 3, 2016
 *      Author: hmeyer
 */

#include "post.hxx"

std::shared_ptr<presentation::post> Post::present() const {
	auto p = std::make_shared<presentation::post>();

	std::ostringstream os;

	os << content;

	p->set_date_and_message(date.str(), os.str());
	return p;
}


