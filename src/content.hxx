/*
 * content.hxx
 *
 *  Created on: May 31, 2015
 *      Author: hmeyer
 */

#ifndef CONTENT_HXX_
#define CONTENT_HXX_

#include <memory>
#include <vector>

class content;
class comment;

struct content_helper {
	std::vector<std::shared_ptr<comment>> comments;
	std::shared_ptr<comment> first_comment();
	std::shared_ptr<comment> next_comment(const std::shared_ptr<comment> current);
	size_t size() const { return comments.size(); }
};


class content {
public:
	virtual std::shared_ptr<comment> first_comment() = 0;
	virtual std::shared_ptr<comment> next_comment(const std::shared_ptr<comment> current) = 0;

	virtual std::string html() = 0;

	virtual ~content() {}
};


class post: public content {
public:
	std::string html();

	std::shared_ptr<comment> first_comment() { return helper.first_comment(); }
	std::shared_ptr<comment> next_comment(const std::shared_ptr<comment> current) { return helper.next_comment(current); }

	void set_message(const std::string& message) { raw_text = message; }
private:
	std::string raw_text;
	content_helper helper;
};

class comment: public content {
public:
	using id_type = size_t;

	id_type get_id() const { return id; }

	bool is_special() const;

	std::string html();

	std::shared_ptr<comment> first_comment() { return helper.first_comment(); }
	std::shared_ptr<comment> next_comment(const std::shared_ptr<comment> current) { return helper.next_comment(current); }

private:
	std::string raw_text;
	content_helper helper;
	id_type id;
};



#endif /* CONTENT_HXX_ */
