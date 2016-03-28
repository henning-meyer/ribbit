#ifndef HTML_FILE_HXX
#define HTML_FILE_HXX

#include <string>
#include <sstream>

struct html_file {
	std::string filename;
	std::string content;
};

class html_snippet {

};

class string_wrapper {
private:
	std::string tag;
public:
	explicit string_wrapper(const std::string& name): tag(name) {}
	explicit string_wrapper(const char* name): tag(name) {}
	operator std::string() const { return tag; }
	const std::string& str() const { return tag; }
};

class html_tag_basic: public string_wrapper {
public:
	explicit html_tag_basic(const std::string& name): string_wrapper(name) {}
	explicit html_tag_basic(const char* name): string_wrapper(name) {}
};

class html_tag_empty : public html_tag_basic {
public:
	explicit html_tag_empty(const std::string& name): html_tag_basic(name) {}
	explicit html_tag_empty(const char* name): html_tag_basic(name) {}
};

class html_tag: public html_tag_basic {
public:
	explicit html_tag(const std::string& name): html_tag_basic(name) {}
	explicit html_tag(const char* name): html_tag_basic(name) {}
};

class html_attr: public string_wrapper {
public:
	explicit html_attr(const std::string& name): string_wrapper(name) {}
	explicit html_attr(const char* name): string_wrapper(name) {}
};

inline std::ostream& render_opening(const html_tag& name, std::ostream& o) { return o << "<" << name.str() << ">\n"; }
inline std::ostream& render_closing(const html_tag& name, std::ostream& o) { return o << "</" << name.str() << ">\n"; }
inline std::ostream& render_selfclosing(const html_tag_empty& name, std::ostream& o) { return o << "<" << name.str() << "/>\n"; }

inline std::ostream& render_void(const html_tag& name, std::ostream& o) { render_opening(name, o); return render_closing(name, o); }

inline std::ostream& render_opening(const html_tag& name, const html_attr& attribute, const std::string& value, std::ostream& o) {
	return o << "<" << name.str() << " " << attribute.str() << "=\"" << value << "\">\n";
}

inline std::ostream& render_void(const html_tag& name, const html_attr& attribute, const std::string& value, std::ostream& o) { render_opening(name, attribute, value, o); return render_closing(name, o); }

inline std::ostream& render_selfclosing(const html_tag_empty& name, const html_attr& attribute, const std::string& value, std::ostream& o) {
	return o << "<" << name.str() << " " << attribute.str() << "=\"" << value << "\"/>\n";
}


inline std::ostream& render_selfclosing(const html_tag_empty& name, const html_attr& attribute, const std::string& value,
		const html_attr& attribute2, const std::string& value2,std::ostream& o) {
	return o << "<" << name.str() << " " << attribute.str() << "=\"" << value << "\" " << attribute2.str() << "=\"" << value2 << "\"/>\n";
}

inline std::ostream& render_selfclosing(const html_tag_empty& name, const html_attr& attribute, const std::string& value,
		const html_attr& attribute2, const std::string& value2,
		const html_attr& attribute3, const std::string& value3,std::ostream& o) {
	o << "<" << name.str() << " " << attribute.str() << "=\"" << value << "\" " << attribute2.str() << "=\"" << value2;
	return o << "\" " << attribute3.str() << "=\"" << value3 << "\"/>\n";
}

class scoped_tag {

	html_tag name;
	std::ostream& o;
public:
	scoped_tag(const html_tag& tag_name, std::ostream& output): name(tag_name), o(output) { render_opening(name, o); }
	scoped_tag(const html_tag& tag_name, const html_attr& attr1, const std::string& val1, std::ostream& output): name(tag_name), o(output) { render_opening(name, attr1, val1, o); }
	// scoped_tag(const tag_attribute_value& tav, std::ostream& output): name(tav.tag), o(output) {render_opening(tav.tag, tav.attribute, tav.value, o); }
	~scoped_tag() { render_closing(name, o); }
};


namespace mime {
namespace image {
extern const std::string png;
}
namespace text {
extern const std::string css;
}
}

namespace tag {
extern const html_tag_empty link;
extern const html_tag title;
extern const html_tag body;
extern const html_tag head;
extern const html_tag_empty meta;
extern const html_tag html;
extern const html_tag p;
extern const html_tag em;
extern const html_tag strong;

extern const html_tag div;
extern const html_tag span;
extern const html_tag_empty br;

extern const html_tag table;
extern const html_tag tr;
extern const html_tag td;
extern const html_tag_empty img;

extern const html_tag a;
extern const html_tag ul;
extern const html_tag li;
}

namespace attr {
extern const html_attr rel;
extern const html_attr href;
extern const html_attr http_equiv;
extern const html_attr type;
extern const html_attr name;
extern const html_attr id;
extern const html_attr tabindex;
extern const html_attr src;
extern const html_attr alt;
extern const html_attr title;
extern const html_attr width;
extern const html_attr height;
extern const html_attr class_attr;
}


std::string minimal_html(const std::string& title, const std::string& message);


#endif
