/*
 * post.hxx
 *
 *  Created on: Apr 3, 2016
 *      Author: hmeyer
 */

#ifndef POST_HXX_
#define POST_HXX_

#include <string>

#include "user_post_fwd.hxx"

#include "content.hxx"

struct Date {
	std::string text;

	std::string str() const { return text; }
};

struct Post {
	std::string content;
	Date date;

	std::shared_ptr<User> author;

	std::shared_ptr<presentation::post> present() const;
};

/* possible posts
 *
 * writing a blog entry
 *
 * upload a picture
 *
 * share a link
 *
 * comment on one of the above
 *
 */


#endif /* POST_HXX_ */
