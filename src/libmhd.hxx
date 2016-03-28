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
http_continue = MHD_HTTP_CONTINUE /*100*/,
http_switching_protocols = MHD_HTTP_SWITCHING_PROTOCOLS /*101*/,
http_processing = MHD_HTTP_PROCESSING /*102*/,

http_ok = MHD_HTTP_OK /*200*/,
//
//#define MHD_HTTP_CREATED 201
//#define MHD_HTTP_ACCEPTED 202
//#define MHD_HTTP_NON_AUTHORITATIVE_INFORMATION 203
//#define MHD_HTTP_NO_CONTENT 204
//#define MHD_HTTP_RESET_CONTENT 205
//#define MHD_HTTP_PARTIAL_CONTENT 206
//#define MHD_HTTP_MULTI_STATUS 207
//
//#define MHD_HTTP_MULTIPLE_CHOICES 300
//#define MHD_HTTP_MOVED_PERMANENTLY 301
//#define MHD_HTTP_FOUND 302
//#define MHD_HTTP_SEE_OTHER 303
//#define MHD_HTTP_NOT_MODIFIED 304
//#define MHD_HTTP_USE_PROXY 305
//#define MHD_HTTP_SWITCH_PROXY 306
//#define MHD_HTTP_TEMPORARY_REDIRECT 307

http_bad_request = MHD_HTTP_BAD_REQUEST /*400*/,
http_unauthorized = MHD_HTTP_UNAUTHORIZED /*401*/,
http_payment_required = MHD_HTTP_PAYMENT_REQUIRED /*402*/,
http_forbidden = MHD_HTTP_FORBIDDEN /*403*/,
http_not_found = MHD_HTTP_NOT_FOUND /*404*/,
http_method_not_allowed = MHD_HTTP_METHOD_NOT_ALLOWED /*405*/,
http_method_not_acceptable = MHD_HTTP_METHOD_NOT_ACCEPTABLE /*406*/,
//#define MHD_HTTP_PROXY_AUTHENTICATION_REQUIRED 407
//#define MHD_HTTP_REQUEST_TIMEOUT 408
//#define MHD_HTTP_CONFLICT 409
//#define MHD_HTTP_GONE 410
//#define MHD_HTTP_LENGTH_REQUIRED 411
//#define MHD_HTTP_PRECONDITION_FAILED 412
//#define MHD_HTTP_REQUEST_ENTITY_TOO_LARGE 413
//#define MHD_HTTP_REQUEST_URI_TOO_LONG 414
//#define MHD_HTTP_UNSUPPORTED_MEDIA_TYPE 415
//#define MHD_HTTP_REQUESTED_RANGE_NOT_SATISFIABLE 416
//#define MHD_HTTP_EXPECTATION_FAILED 417
//#define MHD_HTTP_UNPROCESSABLE_ENTITY 422
//#define MHD_HTTP_LOCKED 423
//#define MHD_HTTP_FAILED_DEPENDENCY 424
//#define MHD_HTTP_UNORDERED_COLLECTION 425
//#define MHD_HTTP_UPGRADE_REQUIRED 426
//#define MHD_HTTP_NO_RESPONSE 444
//#define MHD_HTTP_RETRY_WITH 449
//#define MHD_HTTP_BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS 450
//#define MHD_HTTP_UNAVAILABLE_FOR_LEGAL_REASONS 451
//
http_internal_server_error = MHD_HTTP_INTERNAL_SERVER_ERROR /*500*/
//#define MHD_HTTP_INTERNAL_SERVER_ERROR 500
//#define MHD_HTTP_NOT_IMPLEMENTED 501
//#define MHD_HTTP_BAD_GATEWAY 502
//#define MHD_HTTP_SERVICE_UNAVAILABLE 503
//#define MHD_HTTP_GATEWAY_TIMEOUT 504
//#define MHD_HTTP_HTTP_VERSION_NOT_SUPPORTED 505
//#define MHD_HTTP_VARIANT_ALSO_NEGOTIATES 506
//#define MHD_HTTP_INSUFFICIENT_STORAGE 507
//#define MHD_HTTP_BANDWIDTH_LIMIT_EXCEEDED 509
//#define MHD_HTTP_NOT_EXTENDED 510
};

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
