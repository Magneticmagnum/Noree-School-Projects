#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_TABS 10

extern int errno;
comm_channel channel[UNRECLAIMED_TAB_COUNTER];

int run_tab(comm_channel channel, int index);
int run_controller(comm_channel);

// Function pointer cast for callback functions
typedef void (*G_CALLBACK)(void);

/* Array to maintain the available tab ID's.
 The array index IDs[n - 1] will be 1 if n is available, 0 else. */
int available_IDs[UNRECLAIMED_TAB_COUNTER];

/*
 * Name: getAvailableTabID
 * No input arguments.
 * Output arguments: the smallest available tab ID for the creation of a new tab.
 *      Function: This function finds and returns the smallest available tab ID.
 *      This allows the creation of a new tab without wasting unused tab IDs.
 */
int getAvailableTabID(void) {
	int i, ID = -1;
	for (i = 1; i <= UNRECLAIMED_TAB_COUNTER; i++) {
		if (available_IDs[i - 1] == 1) {
			ID = i;
			break;
		}
	}
	return ID; // No available tab ID's
}

/*
 * Name:		uri_entered_cb
 * Input arguments:'entry'-address bar where the url was entered
 *			 'data'-auxiliary data sent along with the event
 * Output arguments:void
 * Function:	When the user hits the enter after entering the url
 *			in the address bar, 'activate' event is generated
 *			for the Widget Entry, for which 'uri_entered_cb'
 *			callback is called. Controller-tab captures this event
 *			and sends the browsing request to the router(/parent)
 *			process.
 */
void uri_entered_cb(GtkWidget* entry, gpointer data) {
	if (!data)
		return;
	browser_window* b_window = (browser_window*) data;
	comm_channel channel = b_window->channel;
	// Get the tab index where the URL is to be rendered
	int tab_index = query_tab_id_for_request(entry, data);
	if (tab_index <= 0) {
		fprintf(stderr, "Error: entered bad tab ID\n");
		errno = EINVAL;
		return;
	}

	// Get the URL.
	char* uri = get_entered_uri(entry);
	// Prepare 'request' packet to send to router (/parent) process.
	// Append your code here
	child_req_to_parent new_req;
	new_req.type = NEW_URI_ENTERED;
	strcpy(new_req.req.uri_req.uri, uri);
	new_req.req.uri_req.render_in_tab = tab_index;

	// send request to router
	if (write(channel.child_to_parent_fd[1], &new_req,
			sizeof(child_req_to_parent)) == -1) {
		perror("Error sending URI change request to router");
	};
}

/*

 * Name:		new_tab_created_cb
 * Input arguments:	'button' - whose click generated this callback
 *			'data' - auxillary data passed along for handling
 *			this event.
 * Output arguments:    void
 * Function:		This is the callback function for the 'create_new_tab'
 *			event which is generated when the user clicks the '+'
 *			button in the controller-tab. The controller-tab
 *			redirects the request to the parent (/router) process
 *			which then creates a new child process for creating
 *			and managing this new tab.
 */
void new_tab_created_cb(GtkButton *button, gpointer data) {
	if (!data)
		return;
	int tab_index = ((browser_window*) data)->tab_index;
	comm_channel channel = ((browser_window*) data)->channel;

	// Create a new request of type CREATE_TAB

	child_req_to_parent new_req;
	new_req.type = CREATE_TAB;
	new_req.req.new_tab_req.tab_index = tab_index;
	//Append your code here
	// Send create tab message to the router
	if (write(channel.child_to_parent_fd[1], &new_req,
			sizeof(child_req_to_parent)) == -1) {
		perror("Error sending tab creation message to router");
	}
}

/*
 * name: run_controller
 * Input parameters: comm_channel channel
 * Output: void
 * Function: When run in a controller process, this function will initialize the GUI
 * and run the GTK blocking show_browser() function.
 */
int run_controller(comm_channel channel) {
	browser_window* b_window = NULL;
	create_browser(CONTROLLER_TAB, 0, (G_CALLBACK) (&new_tab_created_cb),
			(G_CALLBACK) (&uri_entered_cb), &b_window, channel);
	show_browser();
	// show_browser is blocking for as long as the controller window exists, 
	// so when we reach here the user has closed the window. exit the process.
	exit(0);
}

/*
 * name: run_tab
 * Input Parameters: comm_channel channel, int index
 * Output: void
 * Function: When run in a tab process, this function will set up the GUI and respond to
 *          incoming messages from the router.
 */
int run_tab(comm_channel channel, int index) {
	if (index <= 0) {
		errno = EINVAL;
		return -1;
	}
	browser_window *b_window = NULL;
	create_browser(URL_RENDERING_TAB, index,
			(G_CALLBACK) (&new_tab_created_cb), (G_CALLBACK) (&uri_entered_cb),
			&b_window, channel);

	child_req_to_parent incoming_req;
	int read_ret;
	while (1) {
		read_ret = read(channel.parent_to_child_fd[0], &incoming_req,
				sizeof(child_req_to_parent));
		if (read_ret == -1 && errno != EAGAIN) {
			perror("Error reading message from router to tab process");
		} else if (read_ret > 0) {
			switch (incoming_req.type) {
			case CREATE_TAB:
				// Ignore this message
				break;
			case NEW_URI_ENTERED:
				render_web_page_in_tab(incoming_req.req.uri_req.uri, b_window);
				break;
			case TAB_KILLED:
				process_all_gtk_events();
				exit(0);
				break;
			default:
				printf(
						"Error: invalid message received from router. type: %d; read_ret: %d",
						incoming_req.type, read_ret);
				break;
			}
		} else if (read_ret == 0) {
			// Router is dead, kill self.
			fprintf(stderr,
					"Router died without sending kill message. Ending processes");
			process_all_gtk_events();
			exit(1);
		}
		// Now handle GTK events
		process_single_gtk_event();
	}
}

int main() {
	int controller_pid, i;

	// Now in the router process
	// Begin by setting up pipes for communication between router and controller
	int router_to_controller[2], controller_to_router[2];

	int tab_to_router[2]; //pipe handles from tabs to router
	int router_to_tab[MAX_TABS][2]; //pipe handles from router to tabs
	int num_tabs = 0;

	// Set up available tab array
	for (i = 1; i <= UNRECLAIMED_TAB_COUNTER; i++) {
		available_IDs[i - 1] = 1;
	}

	if (pipe(router_to_controller) == -1)
		perror("Error creating pipe from router to controller");
	if (pipe(controller_to_router) == -1)
		perror("Error creating pipe from controller to router");
	if (pipe(tab_to_router) == -1)
		perror("Error creating pipe from tabs to router");

	// Now we can fork off the controller
	controller_pid = fork();
	if (controller_pid == 0) {
		// child (controller) here. close appropriate pipe ends for one-way comm.
		close(router_to_controller[1]);
		close(controller_to_router[0]);

		comm_channel channel;
		channel.parent_to_child_fd[0] = router_to_controller[0];
		channel.parent_to_child_fd[1] = 0;
		channel.child_to_parent_fd[0] = 0;
		channel.child_to_parent_fd[1] = controller_to_router[1];
		// now run the controller setup function.
		if (run_controller(channel) == -1) {
			perror("Error running controller process");
			exit(1);
		}
		// We should never get to this point, but clean up anyway if we do.
		exit(1);
	} else if (controller_pid < 0) {
		perror("Error forking controller process: ");
	}
	// Now we're back in the router process. Set up pipes properly for communication with controller.:
	close(router_to_controller[0]);
	close(controller_to_router[1]);

	// set up the reads on the controller->router and tab->router pipes to be nonblocking
	int flags;
	flags = fcntl(controller_to_router[0], F_GETFL, 0);
	if (flags == -1)
		perror("Error getting pipe flags");
	flags |= O_NONBLOCK;
	if (fcntl(controller_to_router[0], F_SETFL, flags) == -1)
		perror("Error setting pipe to be nonblocking");

	if ((flags = fcntl(tab_to_router[0], F_GETFL, 0)) == -1)
		perror("Error getting pipe flags");
	flags |= O_NONBLOCK;
	if (fcntl(tab_to_router[0], F_SETFL, flags) == -1)
		perror("Error setting pipe to be nonblocking");

	// the compiler was getting cranky when declaring variables inside the switch statement
	child_req_to_parent incoming_req, kill_req;
	int read_ret, killed_id, tab_id;
	int tab_pid, destination;
	while (1) {
		// check for messages from the controller
		read_ret = read(controller_to_router[0], &incoming_req,
				sizeof(child_req_to_parent));

		if (read_ret == -1 && errno != EAGAIN) {
			// Something went wrong
			perror("Error reading from controller to router pipe");
		} else if (read_ret > 0) {
			switch (incoming_req.type) {

			case CREATE_TAB:
				tab_id = getAvailableTabID();
				if (tab_id == -1) {
					fprintf(stderr,
							"Error: the maximum number of tabs allowed already exist.\n");
					break;
				}
				available_IDs[tab_id - 1] = 0;
				num_tabs++;
				/* set up pipes to/from new tab
				 * router_to_tab is indexed so that router_to_tab[n- 1] 
				 * is the pipe going from the router to tab n.
				 */
				if (pipe(router_to_tab[tab_id - 1]) == -1) {
					perror("Error creating pipe to new tab process");
				}

				// set fds to have non-blocking reads.
				flags = fcntl(router_to_tab[tab_id - 1][0], F_GETFL, 0);
				if (flags == -1)
					perror("Error reading pipe flags");
				flags |= O_NONBLOCK;
				if (fcntl(router_to_tab[tab_id - 1][0], F_SETFL, flags) == -1)
					perror("Error setting pipe to be nonblocking");
				tab_pid = fork();
				if (tab_pid == 0) {
					// new tab process
					close(tab_to_router[0]);
					close(router_to_tab[tab_id - 1][1]);
					// call tab function with file handles to_router & from_router
					comm_channel channel;
					channel.parent_to_child_fd[0]
							= router_to_tab[tab_id - 1][0];
					channel.parent_to_child_fd[1] = 0;
					channel.child_to_parent_fd[0] = 0;
					channel.child_to_parent_fd[1] = tab_to_router[1];
					if (run_tab(channel, tab_id) == -1) {
						perror("Error creating new tab");
						exit(1);
					}
					// We should never get to this point, but clean up anyway if we do.
					exit(1);
				} else if (tab_pid == -1) {
					perror("Error forking new tab process.");
				}
				break;

			case NEW_URI_ENTERED:
				// We can pass this directly on to the desired tab
				destination = incoming_req.req.uri_req.render_in_tab;
				if (destination > UNRECLAIMED_TAB_COUNTER
						|| available_IDs[destination - 1] == 1) {
					fprintf(stderr,
							"Error: tried to change URL of non-existent tab\n");
				} else {
					if (write(router_to_tab[destination - 1][1], &incoming_req,
							sizeof(child_req_to_parent)) == -1) {
						perror("Error sending URI change request to tab");
					}
				}

				break;

			case TAB_KILLED:
				killed_id = incoming_req.req.killed_req.tab_index;
				// Controller is dead - kill all tabs and exit
				kill_req.type = TAB_KILLED;
				kill_req.req.killed_req.tab_index = 0;
				/* Tabs are indexed from 1 to UNRECLAIMED_TAB_COUNTER, so loop over that
				 *  and check if the tab actually exists (available_IDs[id - 1] == 0).
				 */
				for (i = 1; i <= UNRECLAIMED_TAB_COUNTER; i++) {
					if (available_IDs[i - 1] == 0 && write(
							router_to_tab[i - 1][1], &kill_req,
							sizeof(child_req_to_parent)) == -1)
						perror("Error sending kill message to tab");
				}
				exit(0);
				break;

			default:
				perror("Error: bad message from controller process");
				fprintf(stderr, "Type: %d, read_ret = %d\n", incoming_req.type,
						read_ret);
				break;
			}
		} else if (read_ret == 0) {
			// Controller is dead but didn't send the killed message. Perform cleanup and exit...
			fprintf(stderr, "Controller crashed. Ending all processes.\n");
			child_req_to_parent kill_req;
			kill_req.type = TAB_KILLED;
			kill_req.req.killed_req.tab_index = 0;
			for (i = 1; i <= UNRECLAIMED_TAB_COUNTER; i++) {
				if (available_IDs[i - 1] == 0 && write(router_to_tab[i - 1][1],
						&kill_req, sizeof(child_req_to_parent)) == -1)
					perror("Error sending kill message to tab");
			}
			exit(0);
		}

		// Now check for any messages from tab processes (Did one kill itself?)
		do { // possibly more than one message in pipe, loop until it's empty.
			read_ret = read(tab_to_router[0], &incoming_req,
					sizeof(child_req_to_parent));

			if (read_ret == -1 && errno != EAGAIN) {
				perror("Error reading from controller to router pipe");
			} else if (read_ret != -1 && incoming_req.type == TAB_KILLED) {
				num_tabs--;
				int closed_index = incoming_req.req.killed_req.tab_index;
				available_IDs[closed_index - 1] = 1;
				if (write(router_to_tab[closed_index - 1][1], &incoming_req,
						sizeof(child_req_to_parent)) == -1)
					perror("Error sending kill request to tab");
				else
					wait(NULL); // wait so no zombie processes exist.
				close(router_to_tab[closed_index - 1][1]);
			} else if (read_ret != -1) {
				printf("Bad message from tab.");
			}
		} while (read_ret != -1); // read_ret == -1 -> pipe is empty.

		usleep(1000); // wait 1ms before checking pipes again


	}
	return 0;
}

