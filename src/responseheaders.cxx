/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include <sys/types.h>

#include <sys/select.h>
#include <sys/socket.h>

#include <microhttpd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "libmhd.hxx"

#include <string>

const int PORT = 8888;
const char* FILENAME = "face-smile.png";
const char* MIMETYPE = "image/png";

const std::string errorstr = "<html><body>An internal server error has occured!</body></html>";

static int
answer_to_connection (void* /*cls*/, mhd::Connection* connection,
		const char* /*url*/, const char *method,
		const char* /*version*/, const char* /*upload_data*/,
		size_t* /*upload_data_size*/, void** /*con_cls*/)
{

	if (0 != strcmp (method, "GET")) {
		return mhd::no;
	}
	int fd = open (FILENAME, O_RDONLY);
	struct stat sbuf;
	if ( (-1 == fd ) || (0 != fstat (fd, &sbuf)) ) {
		/* error accessing file */
		if (fd != -1) {
			(void) close (fd);
		}


		mhd::Response* response =
				MHD_create_response_from_buffer (errorstr.size(), const_cast<char*> (errorstr.c_str()), MHD_RESPMEM_PERSISTENT);

		if (nullptr != response) {
			int ret = MHD_queue_response (connection, mhd::http_internal_server_error, response);
			MHD_destroy_response (response);

			return ret;
		} else {
			return mhd::no;
		}
	}

	mhd::Response* response = MHD_create_response_from_fd_at_offset (sbuf.st_size, fd, 0);
	MHD_add_response_header (response, "Content-Type", MIMETYPE);
	int ret = MHD_queue_response (connection, mhd::http_ok, response);
	MHD_destroy_response (response);

	return ret;
}


int
main ()
{
	struct MHD_Daemon *daemon;

	daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
			&answer_to_connection, NULL, MHD_OPTION_END);
	if (nullptr == daemon) {
		return 1;
	}

	(void) getchar ();

	MHD_stop_daemon (daemon);

	return 0;
}
