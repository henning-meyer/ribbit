/*
 * main.cxx
 *
 *  Created on: Feb 23, 2016
 *      Author: hmeyer
 */

#include "sessions.hxx"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <microhttpd.h>

#include <new>

/**
 * Call with the port number as the only argument.
 * Never terminates (other than by signals, such as CTRL-C).
 */
int
main (int argc, char *const *argv)
{
	if (argc != 2) {
		printf ("%s PORT\n", argv[0]);
		return 1;
	}
	/* initialize PRNG */
	srand ((unsigned int) time (nullptr));
	mhd::Daemon* d = MHD_start_daemon (MHD_USE_DEBUG,
			atoi (argv[1]),
			nullptr, nullptr,
			&create_response, nullptr,
			MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 15,
			MHD_OPTION_NOTIFY_COMPLETED, &request_completed_callback, nullptr,
			MHD_OPTION_END);
	if (nullptr == d) {
		return 1;
	}

	while (1) {
		expire_sessions ();
		MHD_socket max = 0;
		fd_set rs;
		fd_set ws;
		fd_set es;
		FD_ZERO (&rs);
		FD_ZERO (&ws);
		FD_ZERO (&es);
		if (MHD_YES != MHD_get_fdset (d, &rs, &ws, &es, &max))
			break; /* fatal internal error */
		MHD_UNSIGNED_LONG_LONG mhd_timeout;
		struct timeval tv;
		struct timeval *tvp;
		if (MHD_get_timeout (d, &mhd_timeout) == MHD_YES) {
			tv.tv_sec = mhd_timeout / 1000;
			tv.tv_usec = (mhd_timeout - (tv.tv_sec * 1000)) * 1000;
			tvp = &tv;
		} else {
			tvp = nullptr;
		}
		if (-1 == select (max + 1, &rs, &ws, &es, tvp)) {
			if (EINTR != errno) {
				fprintf (stderr,
						"Aborting due to error during select: %s\n",
						strerror (errno));
			}
			break;
		}
		MHD_run (d);
	}
	MHD_stop_daemon (d);
	return 0;
}
