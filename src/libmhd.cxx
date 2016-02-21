/*
 * libmhd.cxx
 *
 *  Created on: Jan 27, 2016
 *      Author: hmeyer
 */

#include "libmhd.hxx"

namespace mhd {

namespace http_header {
const std::string set_cookie = MHD_HTTP_HEADER_SET_COOKIE;
const std::string content_encoding = MHD_HTTP_HEADER_CONTENT_ENCODING;
}


namespace http_method {
const std::string get = MHD_HTTP_METHOD_GET;
const std::string post = MHD_HTTP_METHOD_POST;
const std::string head = MHD_HTTP_METHOD_HEAD;
}


}



