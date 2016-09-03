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

#include <atomic>


#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "config.hxx"
#include "program_options.hxx"

namespace global {

std::atomic<bool> running;

void init() {
	running = true;
}
}

void sig_handler (int /*sig*/, siginfo_t* /*siginfo*/, void* /*context*/)
{
	global::running = false;
}

void install_signal_handler_helper(int signum) {

	struct sigaction act;
	memset (&act, '\0', sizeof(act));

	/* Use the sa_sigaction field because the handles has two additional parameters */
	act.sa_sigaction = &sig_handler;

	/* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
	act.sa_flags = SA_SIGINFO;

	if (sigaction(signum, &act, NULL) < 0) {
		perror ("failed to install signal handler");
		exit(1);
	}
}

void install_signal_handlers() {
	install_signal_handler_helper(SIGINT);
	install_signal_handler_helper(SIGTERM);
}

struct MainApp {
	int main(int argc, char*const* argv) {
		global::init();

		install_signal_handlers();

		Config config;

		ImmediateAction act = adjust_config(config, argc, argv);

		if(act == ImmediateAction::help) {
			printf("%s\n", config.data);
			return 0;
		}




		/* initialize PRNG */
		srand ((unsigned int) time (nullptr));

		mhd::Daemon* d = MHD_start_daemon (MHD_USE_DEBUG,
				config.port_num,
				nullptr, nullptr,
				&create_response, nullptr,
				MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 15,
				MHD_OPTION_NOTIFY_COMPLETED, &request_completed_callback, nullptr,
				MHD_OPTION_END);
		if (nullptr == d) {
			return 1;
		}

		while (global::running) {
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
			unsigned long long mhd_timeout;
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
};

/**
 * Call with the port number as the only argument.
 * Never terminates (other than by signals, such as CTRL-C).
 */
int
main (int argc, char *const *argv) {
	MainApp app;
	return app.main(argc, argv);
}
