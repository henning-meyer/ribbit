/*
 * html_file.cxx
 *
 *  Created on: May 28, 2015
 *      Author: hmeyer
 */

#include <sstream>
#include <string>

#include "html_file.hxx"

#include "content.hxx"

const std::string doctype = "<!DOCTYPE html>";
const std::string html_xmlns = "http://www.w3.org/1999/xhtml";
const std::string stylesheet_type = "stylesheet";



namespace mime {
namespace image {
const std::string png = "image/png";
}
namespace text {
const std::string css = "text/css";
}
}

namespace tag {
const html_tag_empty link { "link" };
const html_tag title { "title" };
const html_tag body { "body" };
const html_tag head { "head" };
const html_tag_empty meta { "meta" };
const html_tag html { "html" };
const html_tag p { "p" };
const html_tag em { "em" };
const html_tag strong { "strong" };
const html_tag form { "form" };
const html_tag_empty input { "input" };
const html_tag textarea { "textarea" };

const html_tag div { "div" };
const html_tag span { "span" };
const html_tag_empty br { "br" };

const html_tag table { "table" };
const html_tag tr { "tr" };
const html_tag td { "td" };
const html_tag_empty img { "img" };

const html_tag a { "a" };
const html_tag li { "li" };
const html_tag ul { "ul" };


}

namespace attr {
const html_attr rel { "rel" };
const html_attr href { "href" };
const html_attr http_equiv { "http-equiv" };
const html_attr type { "type" };
const html_attr name { "name" };
const html_attr id { "id" };
const html_attr tabindex { "tabindex" };
const html_attr src { "src" };
const html_attr alt { "alt" };
const html_attr title { "title" };
const html_attr width { "width" };
const html_attr height { "height" };
const html_attr class_attr { "class" };

const html_attr charset { "charset" };
const html_attr xmlns { "xmlns" };
const html_attr action { "action" };
const html_attr method { "method" };
const html_attr value { "value" };
const html_attr cols { "cols" };
const html_attr rows { "rows" };
const html_attr autocomplete { "autocomplete" };
}




std::string meta_charset_utf8 = "UTF-8";

//struct attribute_value {
//	std::string attribute;
//	std::string value;
//};
//
//struct tag_attribute2 {
//	std::string tag;
//	attribute_value attribute[2];
//};
//
//tag_attribute2 meta_expires ={tag::meta, {{attr::http_equiv, "expires"}, {"content", "0"}}};



std::ostream& render_stylesheet(std::ostream& o, const std::string& css) {
	//<link rel="stylesheet" type="text/css" href="selfhtml.css">
	render_selfclosing(tag::link, attr::rel, stylesheet_type, attr::type, mime::text::css, attr::href, css, o);
	return o;
}

std::ostream& render_favicon(std::ostream& o, const std::string& type, const std::string& image) {
	//<link rel="shortcut icon" type="image/png" href="face-smile.png">
	render_selfclosing(tag::link, attr::rel, "shortcut icon", attr::type, type, attr::href, image, o);
	return o;
}


std::ostream& render_header(const std::string& title, std::ostream& o) {
	{
		scoped_tag head(tag::head, o);
		render_selfclosing(tag::meta, attr::charset, meta_charset_utf8, o);
		render_favicon(o,"image/png","face-smile.png");
		render_stylesheet(o, "default.css");
		{
			scoped_tag scoped_title(tag::title, o);
			o << title;
		}

	}
	return o;
}

std::ostream& render_body(const std::string& message, std::ostream& o) {
	{
		scoped_tag body(tag::body, o);

		{
			scoped_tag div_outer(tag::div,attr::id,"outer", o);


			render_void(tag::div,attr::id,"upbg", o);

			{
				scoped_tag div_inner(tag::div,attr::id,"inner", o);


				{
					scoped_tag div_superfixed(tag::div, attr::id, "superfixed", o);

					{
						scoped_tag div_header(tag::div, attr::id, "header", o);
						o << "Header Text" << "\n";
					}
					render_void(tag::div,attr::id,"splash", o);

					{
						scoped_tag div_menu(tag::div, attr::id, "menu", o);
						scoped_tag unordered_list(tag::ul, o);

								//o << "<li class='first'><a href='#'>Home</a></li>\n";

								{
									scoped_tag list_element(tag::li, attr::class_attr, "first", o);
									scoped_tag link(tag::a, attr::href, "#", o);
									o << "Home";
								}

								o << "	<li><a href='#'>Archives</a></li>\n";
								o << "	<li><a href='#'>Links</a></li>\n";
								o << "		<li><a href='#'>Resources</a></li>\n";
								o << "		<li><a href='#'>Contact</a></li>\n";


						{
						scoped_tag div_date(tag::div, attr::id, "date", o);
						o << "August 1, 2006" << "\n";
					}
					}

					render_void(tag::div, attr::id, "fader", o);

				}



				{
					using namespace presentation;
					scoped_tag div_primary(tag::div,attr::id,"primarycontent", o);

					for(int i = 0; i < 2; i++) {

					post some_post;
					some_post.set_date_and_message("2016 April 3", message);
					some_post.set_author("Alice", "<img src='face-smile.png' width='32' height='32' style='width:4em;height:4em' />");
					o << some_post.html();

					}
				}

				{
					scoped_tag div_secondary(tag::div,attr::id,"secondarycontent", o);

					scoped_tag div_content(tag::div,attr::class_attr,"content", o);

					scoped_tag p(tag::p, o);

					{
						scoped_tag strong(tag::strong, o);
						o << "Nullam turpis";
					}

					o << "vestibulum et sed dolore. Nulla facilisi. Sed tortor. lobortis commodo.\n";
					{
						scoped_tag a(tag::a, attr::href, "#", o);
						o << "More ...";
					}
//					<a href="#">More ...</a>
//
//					<div class="content">
//														<img src="images/pic2.jpg" class="picB" alt="" />
//														<p><strong></strong> vestibulum et sed dolore. Nulla facilisi. Sed tortor. lobortis commodo. <a href="#">More ...</a></p>
//													</div>
//
//
//								<h3>About Me</h3>
//								<div class="content">
//									<img src="images/pic2.jpg" class="picB" alt="" />
//									<p><strong>Nullam turpis</strong> vestibulum et sed dolore. Nulla facilisi. Sed tortor. lobortis commodo. <a href="#">More ...</a></p>
//								</div>
//
//								<h3>Topics</h3>
//								<div class="content">
//									<ul class="linklist">
//										<li class="first"><a href="#">Accumsan congue (32)</a></li>
//										<li><a href="#">Dignissim nec augue (14)</a></li>
//										<li><a href="#">Nunc ante elit nulla (83)</a></li>
//										<li><a href="#">Aliquam suscipit (74)</a></li>
//										<li><a href="#">Cursus sed arcu sed (14)</a></li>
//										<li><a href="#">Eu ante cras at risus (24)</a></li>
//										<li><a href="#">Donec mollis dolore (39)</a></li>
//										<li><a href="#">Aliquam suscipit (74)</a></li>
//									</ul>
//								</div>




				}

				{
					scoped_tag div_footer(tag::div,attr::id,"footer", o);
					o << __FILE__ << ": " << __DATE__ << " " << __TIME__ << "\n";
				}
			}
		}
	}
	return o;
}



std::string minimal_html(const std::string& title, const std::string& message) {
	std::ostringstream os;

	os << doctype << "\n";
	{
		scoped_tag scoped_html(tag::html, attr::xmlns, html_xmlns, os);
		render_header(title, os);
		render_body(message, os);
	}
	return os.str();
}
