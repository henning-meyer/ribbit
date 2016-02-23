/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include "sessions.hxx"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <microhttpd.h>

#include <new>

/**
 * Name of our cookie.
 */
static const std::string COOKIE_NAME = "session";

/**
 * Linked list of all active sessions.  Yes, O(n) but a
 * hash table would be overkill for a simple example...
 */

using Session = Request::Session;
//static Session* sessions = nullptr;
static std::forward_list<Session> session_list;

/**
 * Return the session handle for this connection, or
 * create one if this is a new user.
 */
static Session::iterator
get_session (mhd::Connection* connection)
{
	const char* cookie = MHD_lookup_connection_value (connection,
			MHD_COOKIE_KIND,
			COOKIE_NAME.c_str());
	if (cookie != nullptr) {

		/* find existing session */
		Session::iterator ses = session_list.begin();
		while (session_list.end() != ses) {

			if (0 == strcmp (cookie, ses->sid)) {
				break;
			}
			++ses;
		}
		if (session_list.end() != ses) {

			ses->rc++;
			return ses;
		}
	}
	/* create fresh session */
	session_list.emplace_front(Session());
	auto ret = session_list.begin();

	/* not a super-secure way to generate a random session ID,
     but should do for a simple example... */
	snprintf (ret->sid,
			sizeof (ret->sid),
			"%X%X%X%X",
			(unsigned int) rand (),
			(unsigned int) rand (),
			(unsigned int) rand (),
			(unsigned int) rand ());
	ret->rc++;
	ret->start = time (nullptr);
	//ret->next = sessions;
	//sessions = ret;
	return ret;
}


/**
 * Add header to response to set a session cookie.
 *
 * @param session session to use
 * @param response response to modify
 */
static void
add_session_cookie (Session& session,
		mhd::Response* response)
{
	char cstr[256];
	snprintf (cstr,
			sizeof (cstr),
			"%s=%s",
			COOKIE_NAME.c_str(),
			session.sid);
	if (mhd::no ==
			MHD_add_response_header (response,
					mhd::http_header::set_cookie.c_str(),
					cstr))
	{
		fprintf (stderr,
				"Failed to set session cookie header!\n");
	}
}


/**
 * Handler that returns a simple static HTTP page that
 * is passed in via 'cls'.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static int
serve_simple_form (const char* form,
		const char *mime,
		Session& session,
		mhd::Connection *connection)
{

	/* return static form */
	mhd::Response* response = MHD_create_response_from_buffer (strlen (form),
			(void *) form,
			MHD_RESPMEM_PERSISTENT);
	add_session_cookie (session, response);
	MHD_add_response_header (response,
			mhd::http_header::content_encoding.c_str(),
			mime);
	int ret = MHD_queue_response (connection,
			mhd::http_ok,
			response);
	MHD_destroy_response (response);
	return ret;
}


/**
 * Handler that adds the 'v1' value to the given HTML code.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static int
fill_v1_form (const char* form,
		const char *mime,
		Session& session,
		mhd::Connection* connection)
{

	char *reply;

	if (-1 == asprintf (&reply,
			form,
			session.value_1)) {
		// oops
		return mhd::no;
	}
	/* return static form */
	mhd::Response* response = MHD_create_response_from_buffer (strlen (reply),
			(void *) reply,
			MHD_RESPMEM_MUST_FREE);
	add_session_cookie (session, response);
	MHD_add_response_header (response,
			mhd::http_header::content_encoding.c_str(),
			mime);
	int ret = MHD_queue_response (connection,
			mhd::http_ok,
			response);
	MHD_destroy_response (response);
	return ret;
}


/**
 * Handler that adds the 'v1' and 'v2' values to the given HTML code.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static int
fill_v1_v2_form (const char* form,
		const char *mime,
		Session& session,
		mhd::Connection* connection)
{

	char *reply;

	if (-1 == asprintf (&reply,
			form,
			session.value_1,
			session.value_2)) {
		return mhd::no; // oops
	}
	/* return static form */
	mhd::Response* response = MHD_create_response_from_buffer (strlen (reply),
			(void *) reply,
			MHD_RESPMEM_MUST_FREE);
	add_session_cookie (session, response);
	MHD_add_response_header (response,
			mhd::http_header::content_encoding.c_str(),
			mime);
	int ret = MHD_queue_response (connection,
			mhd::http_ok,
			response);
	MHD_destroy_response (response);
	return ret;
}


/**
 * Handler used to generate a 404 reply.
 *
 * @param cls a 'const char *' with the HTML webpage to return
 * @param mime mime type to use
 * @param session session handle
 * @param connection connection to use
 */
static int
not_found_page (const char* /*cls*/,
		const char *mime,
		Session& /*session*/,
		mhd::Connection* connection)
{

	const std::string page_source = getPageSource(PageSource::not_found_error);
	/* unsupported HTTP method */
	mhd::Response* response = MHD_create_response_from_buffer (page_source.size(),
			const_cast<char*> (page_source.c_str()),
			MHD_RESPMEM_PERSISTENT);
	int ret = MHD_queue_response (connection,
			mhd::http_not_found,
			response);
	MHD_add_response_header (response,
			mhd::http_header::content_encoding.c_str(),
			mime);
	MHD_destroy_response (response);
	return ret;
}


/**
 * List of all pages served by this HTTP server.
 */
static struct Page pages[] =
{
		{ "/", "text/html",  &fill_v1_form, PageSource::main_page },
		{ "/2", "text/html", &fill_v1_v2_form, PageSource::second_page },
		{ "/S", "text/html", &serve_simple_form, PageSource::submit_page },
		{ "/F", "text/html", &serve_simple_form, PageSource::last_page },
		{ nullptr, nullptr, &not_found_page, PageSource::not_found_error } /* 404 */
};



/**
 * Iterator over key-value pairs where the value
 * maybe made available in increments and/or may
 * not be zero-terminated.  Used for processing
 * POST data.
 *
 * @param cls user-specified closure
 * @param kind type of the value
 * @param key 0-terminated key for the value
 * @param filename name of the uploaded file, nullptr if not known
 * @param content_type mime-type of the data, nullptr if not known
 * @param transfer_encoding encoding of the data, nullptr if not known
 * @param data pointer to size bytes of data at the
 *              specified offset
 * @param off offset of data in the overall value
 * @param size number of bytes in data available
 * @return MHD_YES to continue iterating,
 *         MHD_NO to abort the iteration
 */
static int
post_iterator (void *cls,
		enum MHD_ValueKind /*kind*/,
		const char *key,
		const char* /*filename*/,
		const char* /*content_type*/,
		const char* /*transfer_encoding*/,
		const char *data, uint64_t off, size_t size)
{
	struct Request *request = (Request*)cls;
	Session::iterator session = request->session;

	const std::string done = "DONE";
	if (done == key)
	{
		fprintf (stdout,
				"Session `%s' submitted `%s', `%s'\n",
				session->sid,
				session->value_1,
				session->value_2);
		return mhd::yes;
	}
	const std::string v1 = "v1";
	const std::string v2 = "v2";
	if (v1 == key) {

		if (size + off >= sizeof(session->value_1)) {
			size = sizeof (session->value_1) -1- off;
		}
		memcpy (&session->value_1[off], data, size);
		session->value_1[size+off] = '\0';

		return mhd::yes;
	}

	if (v2 == key) {

		if (size + off >= sizeof(session->value_2)) {
			size = sizeof (session->value_2)-1 - off;
		}
		memcpy (&session->value_2[off], data, size);
		session->value_2[size+off] = '\0';

		return mhd::yes;
	}
	fprintf (stderr, "Unsupported form value `%s'\n", key);
	return mhd::yes;
}


/**
 * Main MHD callback for handling requests.
 *
 *
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param connection handle to connection which is being processed
 * @param url the requested url
 * @param method the HTTP method used ("GET", "PUT", etc.)
 * @param version the HTTP version string (i.e. "HTTP/1.1")
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        upload_data)
 * @param upload_data_size set initially to the size of the
 *        upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;
 * @param ptr pointer that the callback can set to some
 *        address and that will be preserved by MHD for future
 *        calls for this request; since the access handler may
 *        be called many times (i.e., for a PUT/POST operation
 *        with plenty of upload data) this allows the application
 *        to easily associate some request-specific state.
 *        If necessary, this state can be cleaned up in the
 *        global "MHD_RequestCompleted" callback (which
 *        can be set with the MHD_OPTION_NOTIFY_COMPLETED).
 *        Initially, <tt>*con_cls</tt> will be nullptr.
 * @return MHS_YES if the connection was handled successfully,
 *         MHS_NO if the socket must be closed due to a serios
 *         error while handling the request
 */
int
create_response (void* /*cls*/,
		mhd::Connection* connection,
		const char *url,
		const char *method,
		const char* /*version*/,
		const char *upload_data,
		size_t *upload_data_size,
		void **ptr)
{
	printf("create response\n");
	printf("url: %s\n", url);
	printf("method: %s\n", method);

	Request *request = *(Request**)ptr;
	if (nullptr == request) {
		request = new (std::nothrow) Request(session_list.before_begin() );
		if (nullptr == request) {
			fprintf (stderr, "memory error: %s\n", strerror (errno));
			return mhd::no;
		}
		*ptr = request;
		if (method == mhd::http_method::post) {
			request->pp = MHD_create_post_processor (connection, 1024,
					&post_iterator, request);
			if (nullptr == request->pp) {
				fprintf (stderr, "Failed to setup post processor for `%s'\n", url);
				return mhd::no; /* internal error */
			}
		}
		return mhd::yes;
	}
	if (session_list.before_begin() == request->session) {
		request->session = get_session (connection);
		if (session_list.before_begin() == request->session) {
			fprintf (stderr, "Failed to setup session for `%s'\n", url);
			return mhd::no; /* internal error */
		}
	}
	Session::iterator session = request->session;
	session->start = time (nullptr);
	if (method == mhd::http_method::post) {

		/* evaluate POST data */
		MHD_post_process (request->pp,
				upload_data,
				*upload_data_size);
		if (0 != *upload_data_size) {
			*upload_data_size = 0;
			return mhd::yes;
		}
		/* done with POST data, serve response */
		MHD_destroy_post_processor (request->pp);
		request->pp = nullptr;
		method = mhd::http_method::get.c_str(); /* fake 'GET' */
		if (nullptr != request->post_url) {
			url = request->post_url;
		}
	}

	if ( (method == mhd::http_method::get) || (method == mhd::http_method::head) ) {
		/* find out which page to serve */
		unsigned i = 0;
		while ( (pages[i].url != nullptr) &&
				(0 != strcmp (pages[i].url, url)) ) {
			i++;
		}
		const std::string source = getPageSource(pages[i].source);
		int ret = pages[i].handler (source.c_str(), pages[i].mime, *session, connection);
		if (ret != mhd::yes) {
			fprintf (stderr, "Failed to create page for `%s'\n", url);
		}
		return ret;
	}
	/* unsupported HTTP method */
	const std::string error_source = getPageSource(PageSource::method_error);
	mhd::Response *response = MHD_create_response_from_buffer (error_source.size(),
			const_cast<char*>(error_source.c_str()),
			MHD_RESPMEM_PERSISTENT);
	int ret = MHD_queue_response (connection,
			mhd::http_method_not_acceptable,
			response);
	MHD_destroy_response (response);
	return ret;
}


/**
 * Callback called upon completion of a request.
 * Decrements session reference counter.
 *
 * @param cls not used
 * @param connection connection that completed
 * @param con_cls session handle
 * @param toe status code
 */
void
request_completed_callback (void* /*cls*/,
		mhd::Connection* /*connection*/,
		void **con_cls,
		mhd::RequestTerminationCode /*toe*/)
{
	struct Request *request = *(Request**)con_cls;

	if (nullptr == request)
		return;
	if (session_list.before_begin() != request->session)
		request->session->rc--;
	if (nullptr != request->pp)
		MHD_destroy_post_processor (request->pp);
	delete request;
}


/**
 * Clean up handles of sessions that have been idle for
 * too long.
 */
void
expire_sessions ()
{
	time_t now = time (nullptr);
	Session::iterator prev = session_list.before_begin();
	Session::iterator pos = session_list.begin();
	while (session_list.end() != pos) {

		auto next = pos;
		next++;
		if (now - pos->start > 60 * 60) {
			/* expire sessions after 1h */
			if (session_list.before_begin() == prev) {
				session_list.pop_front();
			} else {
				session_list.erase_after(prev);
			}

		} else {
			prev = pos;
		}

		pos = next;

	}
}



