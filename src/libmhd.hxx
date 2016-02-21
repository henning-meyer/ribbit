/*
 * libmhd.hxx
 *
 *  Created on: Jan 24, 2016
 *      Author: hmeyer
 */

#ifndef LIBMHD_HXX_
#define LIBMHD_HXX_



#include <sys/types.h>

#include <sys/select.h>
#include <sys/socket.h>

#include <microhttpd.h>

#include <string>

namespace mhd {
enum yesno {
	yes = MHD_YES,
	no = MHD_NO
};

enum http_status {
	http_ok = MHD_HTTP_OK
};

#include <string>

namespace http_header {
extern const std::string set_cookie;
extern const std::string content_encoding;
}


namespace http_method {
extern const std::string get;
extern const std::string post;
extern const std::string head;
}


using RequestTerminationCode = MHD_RequestTerminationCode;
using Connection = MHD_Connection;
using Response = MHD_Response;
using Daemon = MHD_Daemon;
using PostProcessor = MHD_PostProcessor;
using ValueKind = MHD_ValueKind;
using AccessHandlerCallback = MHD_AccessHandlerCallback;
}



#endif /* LIBMHD_HXX_ */
