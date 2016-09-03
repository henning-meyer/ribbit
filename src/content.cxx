/*
 * content.cxx
 *
 *  Created on: May 31, 2015
 *      Author: hmeyer
 */


#include "content.hxx"
#include "html_file.hxx"
#include <sstream>

#include <iostream>

namespace presentation {

std::shared_ptr<comment> content_helper::first_comment() {
	if (!comments.empty()) {
		return comments.front();
	} else {
		return {};
	}
}
std::shared_ptr<comment> content_helper::next_comment(const std::shared_ptr<comment> current) {
	if (current && current->get_id() + 1 < size()) {
		return comments[current->get_id() + 1];
	} else {
		return {};
	}
}

std::string post::html() {
	std::ostringstream os;
	{
		scoped_tag div_post(tag::div, attr::class_attr, "post", os);
		{
			scoped_tag div_header(tag::div, attr::class_attr, "unheader", os);

			os<< author_pic << " ";
			{

				html_attr attr_style("style");
				scoped_tag table(tag::table, attr_style,"display:inline-block", os);
				{
					{
						scoped_tag tr(tag::tr, os);
						scoped_tag td(tag::td, os);
						scoped_tag a(tag::a, attr::href, "invalid", os);
						os << author_name;
					}
					{
						scoped_tag tr(tag::tr, os);
						scoped_tag td(tag::td, os);

						os << raw_date;
					}
				}

			}

			//os << "<h3>TerraFirma<sup>1.0</sup> by NodeThirtyThree</h3>";
			//			{
			//				scoped_tag div_date(tag::div, attr::class_attr, "date", os);
			//				os << raw_date;
			//			}

		}
		{
			scoped_tag div_content(tag::div, attr::class_attr, "content", os);

			//TODO: <img src="images/pic1.jpg" class="picA floatleft" alt="" />

			// render_selfclosing(tag::img, attr::src, "face-smile.png", os);

			//<p><strong>TerraFirma</strong><sup>1.0</sup> is a free, lightweight, tableless, W3C-compliant website design by <a href="http://www.nodethirtythree.com/">NodeThirtyThree Design</a>. You're free to dissect, manipulate and use it to your heart's content. We only ask that you link back to our site in some way. If you find this design useful, feel free to let us know :)</p>
			//<p>You can find more of our free work at this site or <a href="http://www.nodethirtythree.com/">our site</a>, or some of our commercial work on <a href="http://www.4templates.com/?aff=n33">4Templates.com</a>, a commercial website template site.</p>
			scoped_tag p(tag::p, os);
			os << raw_text;

		}
		{
			scoped_tag div_footer(tag::div, attr::class_attr, "footer", os);

			//os << "<ul>\n";
			//os << "	<li class='printerfriendly'><a href='#'>Printer Friendly</a></li>\n";
			//os << "	<li class='comments'><a href='#'>Comments (18)</a></li>\n";

			//os << "<li class ='comments'>";
			//std::string SECOND_PAGE = "<form action=\"/S\" method=\"post\"> <input type=\"text\" name=\"v2\" value=\"\" /><input type=\"submit\" value=\"Next\" />";
			//os << SECOND_PAGE;

			{
				//scoped_tag li(tag::li, attr::class_attr, "comments", os);
				scoped_tag form(tag::form, attr::action, "S", attr::method, "post", attr::autocomplete, "off", os);
				//render_selfclosing(tag::input, attr::type, "text", attr::name, "v2", attr::width, "100%", os);

				{
					scoped_tag textarea(tag::textarea, attr::name, "v2", attr::cols, "66", attr::rows, "12' style='width:30em;height:4.5em", os);
				}
				render_selfclosing(tag::br, os);
				render_selfclosing(tag::input, attr::type, "submit", attr::value, "Comment", os);
			}

			//os << "</li>\n";
			//os << "	<li class='readmore'><a href='#'>Read more</a></li>\n";

			//os << "</ul>\n";

		}

	}
	return os.str();
}

void content_test() {
	post p;
	std::cout << p.html() << std::endl;
}

}
