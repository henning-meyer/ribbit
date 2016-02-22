/*
 * sessions.hxx
 *
 *  Created on: Feb 22, 2016
 *      Author: hmeyer
 */

#ifndef SESSIONS_HXX_
#define SESSIONS_HXX_

#include "libmhd.hxx"


#include "pages.hxx"


#include <forward_list>

namespace plainc {
/**
 * State we keep for each user/session/browser.
 */
struct Session
{
	/**
	 * We keep all sessions in a linked list.
	 */
	struct Session *next = nullptr;

	/**
	 * Unique ID for this session.
	 */
	char sid[33] = {0};

	/**
	 * Reference counter giving the number of connections
	 * currently using this session.
	 */
	unsigned int rc = 0;

	/**
	 * Time when this session was last active.
	 */
	time_t start = 0;

	/**
	 * String submitted via form.
	 */
	char value_1[64] = {0};

	/**
	 * Another value submitted via form.
	 */
	char value_2[64] = {0};
	using iterator = Session*;
};
}

namespace moderncxx {
/**
 * State we keep for each user/session/browser.
 */
struct Session
{
	/**
	 * We keep all sessions in a linked list.
	 */
	struct Session *next = nullptr;

	/**
	 * Unique ID for this session.
	 */
	char sid[33] = {0};

	/**
	 * Reference counter giving the number of connections
	 * currently using this session.
	 */
	unsigned int rc = 0;

	/**
	 * Time when this session was last active.
	 */
	time_t start = 0;

	/**
	 * String submitted via form.
	 */
	char value_1[64] = {0};

	/**
	 * Another value submitted via form.
	 */
	char value_2[64] = {0};
	using iterator = std::forward_list<Session>::iterator;
};



}

/**
 * Data kept per request.
 */
struct Request
{
	using Session = plainc::Session;
	/**
	 * Associated session.
	 */
	Session::iterator session = nullptr;

	/**
	 * Post processor handling form data (IF this is
	 * a POST request).
	 */
	mhd::PostProcessor* pp = nullptr;

	/**
	 * URL to serve in response to this POST (if this request
	 * was a 'POST')
	 */
	const char* post_url = nullptr;

};

/**
 * Type of handler that generates a reply.
 *
 * @param cls content for the page (handler-specific)
 * @param mime mime type to use
 * @param session session information
 * @param connection connection to process
 * @param MHD_YES on success, MHD_NO on failure
 */
typedef int (*PageHandler)(const char* cls,
		const char *mime,
		Request::Session& session,
		mhd::Connection* connection);


/**
 * Entry we generate for each page served.
 */
struct Page
{
	/**
	 * Acceptable URL for this page.
	 */
	const char *url;

	/**
	 * Mime type to set for the page.
	 */
	const char *mime;

	/**
	 * Handler to call to generate response.
	 */
	PageHandler handler;

	/**
	 * Extra argument to handler.
	 */
	PageSource source;
};



#endif /* SESSIONS_HXX_ */
