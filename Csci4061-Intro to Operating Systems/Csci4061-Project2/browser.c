#include "wrapper.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

extern int errno;
comm_channel channel[UNRECLAIMED_TAB_COUNTER];

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
    // Append code for error handling
    printf("ERROR! please enter a tab index that exists.");
  } else {
    // 4.2.3.a in the project description
    // Get the URL.
    char* uri = get_entered_uri(entry);

    // Prepare 'request' packet to send to router (/parent) process.
    // Append your code here
    child_req_to_parent new_req;
    new_req.type = NEW_URI_ENTERED;
    new_req.req.uri_req.render_in_tab = tab_index;
    strcpy(new_req.req.uri_req.uri, uri);
    write(channel.child_to_parent_fd[1], &new_req, sizeof(child_req_to_parent));
  }
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

  // Create a new request
  child_req_to_parent new_req;
  // Append your code here
  // The CONTROLLER procress sends a CREATE_TAB message to the ROUTER process.
  new_req.type = CREATE_TAB;
  new_req.req.new_tab_req.tab_index = tab_index;
  write(channel.child_to_parent_fd[1], &new_req, sizeof(child_req_to_parent));
}

int main() {
  // Append your code here
  // The ROUTER process creates two pipes for bi-directional communication with the CONTROLLER process
  pipe(channel[0].child_to_parent_fd);
  pipe(channel[0].parent_to_child_fd);
  int tab_index = 0;
  int open_tabs = 0;

  // 4.1.1.b it forks the new child process, the CONTROLLER process:
  int pid = fork();
  if (pid == 0) {
    // ************************************************************************
    // CONTROLLER PROCESS
    // ************************************************************************
    close(channel[0].child_to_parent_fd[0]);
    close(channel[0].parent_to_child_fd[1]);

    // Upon the CONTROLLER's creation, the show_browser() function is called which is a blocking call
    // which returns when the user closes the CONTROLLER window.
    browser_window* controller = NULL;
    create_browser(CONTROLLER_TAB, 0, (void(*)(void)) &new_tab_created_cb, (void(*)(void)) &uri_entered_cb,
        &controller, channel[0]);
    // The CONTROLLER process exits with the return status of 0 when the user closes the CONTROLLER window
    show_browser();
    exit(0);
    // ************************************************************************
    // END CONTROLLER PROCESS
    // ************************************************************************
  } else {
    // ************************************************************************
    // ROUTER PROCESS
    // ************************************************************************
    tab_index++;
    open_tabs++;
    close(channel[0].parent_to_child_fd[0]);
    close(channel[0].child_to_parent_fd[1]);
    // ROUTER process polls on the set of open pipe file descriptors created before forking the child process
    // in a loop.
    int flags = fcntl(channel[0].child_to_parent_fd[0], F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(channel[0].child_to_parent_fd[0], F_SETFL, flags);

    while (open_tabs > 0) {
      int i;
      for (i = 0; i < tab_index; i++) {
        // The non-blocking read reads data from te pipe and if no data is available, the read system call
        // will return immediately with return value of -1 and errno set to EAGAIN.
        child_req_to_parent new_req;
        int nread = read(channel[i].child_to_parent_fd[0], &new_req, sizeof(child_req_to_parent));
        // If the above comment occurs then it will continue with the polling.  To reduce CPU consuption in
        // usleep of 10 is used between reads 
        if (nread == -1 && errno == EAGAIN) {
          // printf("router got no messages...\n");
          usleep(10);
        } else if (nread > 0) {
          // If read returns with data, the data is read into a buffer of type child_req_to_parent
          // printf("router got a message!\n");
          // CREATE TAB *******************************************************
          if (new_req.type == CREATE_TAB) {
            // printf("router creating tab...\n");
            pipe(channel[tab_index].parent_to_child_fd);
            pipe(channel[tab_index].child_to_parent_fd);
            // Upon reciveing the message, the ROUTER process forts a URL-RENDERING process
            // The ROUTER process first creates 2 pipes for bidirectional communication with the child
            // process and then forks the URL-RENDERING child process.
            int tab_pid = fork();
            if (tab_pid == 0) {
              // **************************************************************
              // TAB PROCESS
              // **************************************************************
              // printf("tab created.\n");
              comm_channel tab_comm = channel[tab_index];
              close(tab_comm.parent_to_child_fd[1]);
              close(tab_comm.child_to_parent_fd[0]);

              // Creates the tab process
              browser_window* tab = NULL;
              create_browser(URL_RENDERING_TAB, tab_index, (void(*)(void)) &new_tab_created_cb,
                  (void(*)(void)) &uri_entered_cb, &tab, tab_comm);

              int flags = fcntl(tab_comm.parent_to_child_fd[0], F_GETFL);
              flags |= O_NONBLOCK;
              fcntl(tab_comm.parent_to_child_fd[0], F_SETFL, flags);

              int kill_tab = 0;
              while (kill_tab == 0) {
                child_req_to_parent new_req_tab;
                int nread_tab = read(tab_comm.parent_to_child_fd[0], &new_req_tab, sizeof(child_req_to_parent));
                if (nread_tab == -1 && errno == EAGAIN) {
                  usleep(10);
                  // printf("tab %i got no messages...\n", tab_index);
                } else if (nread_tab > 0) {
                  // printf("tab %i got a message!\n", tab_index);
                  // If the message type is NEW_URL_ENTERED, then the URL-RENDERING process renders the
                  // requested url by invoking render_web_page_in_tab() of  the wrapper-library
                  // NEW URI PASSED TO TAB ************************************
                  if (new_req_tab.type == NEW_URI_ENTERED) {
                    // printf("new uri requested.\n");
                    render_web_page_in_tab(new_req_tab.req.uri_req.uri, tab);
                  }
                  // If the message type is TAB_KILLED then process_all_gtk_events() function of the
                  // wrapper library is invoked and the process exits with the return status of 0
                  // TAB ASKED TO TERMINATE ***********************************
                  else if (new_req_tab.type == TAB_KILLED) {
                    // printf("closing tab...\n");
                    process_all_gtk_events();
                    close(channel[tab_index].parent_to_child_fd[0]);
                    close(channel[tab_index].child_to_parent_fd[1]);
                    kill_tab = 1;
                  } /* CREATE_TAB occurs, which is then ignored */
                  else if (new_req_tab.type == CREATE_TAB) {
                    // error? ignore
                  } else {
                    // error? no type specified
                    perror("No type specified or incorrectly specified, closing tab.");
                    process_all_gtk_events();
                    close(channel[tab_index].parent_to_child_fd[0]);
                    close(channel[tab_index].child_to_parent_fd[1]);
                    kill_tab = 1;
                  }
                } else {
                  // read didn't get any data.
                  perror("No data was supplied, please try again");
                }
                // The GTK events are processed
                process_single_gtk_event();
              }

              // printf("tab %i closed.\n", tab_index);
              exit(0);
              // **************************************************************
              // END TAB PROCESS
              // **************************************************************
            } else {
              close(channel[tab_index].parent_to_child_fd[0]);
              close(channel[tab_index].child_to_parent_fd[1]);

              int flags = fcntl(channel[tab_index].child_to_parent_fd[0], F_GETFL);
              flags |= O_NONBLOCK;
              fcntl(channel[tab_index].child_to_parent_fd[0], F_SETFL, flags);

              tab_index++;
              open_tabs++;
            }
          }
          // If the message is NEW_URI_ENTERED the message specifies the URL to be rendered along
          // with tab index on which the URL should be rendered.  Upon recieving this message, the
          // ROUTER process simply writes the message on the pipe connecting the ROUTER process and
          // the corresponding URL-RENDERING process
          // NEW URI ENTERED **************************************************
          else if (new_req.type == NEW_URI_ENTERED) {
            // printf("passing uri message to child...\n");
            write(channel[new_req.req.uri_req.render_in_tab].parent_to_child_fd[1], &new_req,
                sizeof(child_req_to_parent));
          }
          // If the message is of type TAB_KILLED then the message contains the tab-index of the
          // recently closed tab.  The ROUTER process closes the file descriptors of the corresponding
          // pipe which was created for communication with the closed tab. 
          // TAB KILLED *******************************************************
          else if (new_req.type == TAB_KILLED) {
            // printf("router closing tab %i...\n", new_req.req.killed_req.tab_index);
            // printf("tabs left: %i.\n", open_tabs - 1);
            write(channel[new_req.req.killed_req.tab_index].parent_to_child_fd[1], &new_req,
                sizeof(child_req_to_parent));

            close(channel[new_req.req.killed_req.tab_index].parent_to_child_fd[1]);
            close(channel[new_req.req.killed_req.tab_index].child_to_parent_fd[0]);
            open_tabs--;
          } else {
            // error? no type specified
            perror("No type specified or incorrectly specified");
          }
        } else {
          // error? fd is closed
          // perror("The file descriptor is closed");
          // not an error, if tab one is closed, it's file descriptors will still try and
          // be read as part of the loop.
        } // end else [if (nread == -1 && errno == EAGAIN)] [if (nread > 0)]

      } // for (i = 0; i < tab_index; i++)

    } // end while (open_tabs > 0)

  } // end else [if (pid == 0)]
  // When all browsing windows are closed, the ROUTER process exits with a return status of 0
  return 0;
  // ************************************************************************
  // ROUTER PROCESS
  // ************************************************************************
}

