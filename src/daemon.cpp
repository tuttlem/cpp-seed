
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <syslog.h>

char *daemon_name = "daemond";
int daemon_running = 0;

/**
 * Puts the application into a state where it turns into a daemon
 */
void daemon_setup(void);

/**
 * Runs the main loop for the daemon
 */
void daemon_main(void);

/**
 * Performs any teardown required for the daemon
 */
void daemon_teardown();

/**
 * Handles signals from the outside word
 * @param sig The signal number to respond to
 */
void daemon_signalhandler(int sig);

/** Program entry point */
int main(int argc, char *argv[]) {

   /* setup the daemon state */
   daemon_setup();

   /* run the daemon loop */
   daemon_main();

   /* teardown the daemon */
   daemon_teardown();

   return 0;
}

/**
 */
void daemon_setup(void) {
   pid_t pid, sid;

   /* 1. fork off the parent process */
   pid = fork();

   /* if process failed to created, bail out with failure */
   if (pid < 0) {
      exit(EXIT_FAILURE);
   }

   /* if the pid is good, terminate the parent ok */
   if (pid > 0) {
      exit(EXIT_SUCCESS);
   }

   /* 2. change the file mode mask (umask) */
   umask(0);

   /* 3. open logs for writing (syslog) */
   openlog(daemon_name, LOG_PID, LOG_DAEMON);

   /* 4. set the session id for the daemon */
   sid = setsid();

   /* bug out with failure if we couldn't set the new process group */
   if (sid < 0) {
      exit(EXIT_FAILURE);
   }

   /* 5. change the current working directory to a "safe" place */
   if ((chdir("/")) < 0) {
      exit(EXIT_FAILURE);
   }

   /* 6. close all standard file handles */
   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);

   /* attach the signal handlers now */
   signal(SIGCHLD, SIG_IGN);
   signal(SIGTSTP, SIG_IGN);
   signal(SIGTTOU, SIG_IGN);
   signal(SIGTTIN, SIG_IGN);
   signal(SIGHUP, daemon_signalhandler);
   signal(SIGTERM, daemon_signalhandler);

   /* mark the server as running */
   daemon_running = 1;
}

/**
 */
void daemon_main(void) {

   syslog(LOG_INFO, "%s now starting", daemon_name);

   /* TODO: fill this out */
   while (daemon_running) {

      sleep(10);
   }

   syslog(LOG_INFO, "%s now stopping", daemon_name);
}

/**
 */
void daemon_teardown() {

   /* close the logging facility down */
   closelog();

}

/**
 */
void daemon_signalhandler(int sig) {

   /* interpret the incoming signal */
   switch (sig) {
      case SIGHUP:
         syslog(LOG_ALERT, "just received SIGHUP");
         /* perform a service restart here */
         break;

      case SIGTERM:
         syslog(LOG_ALERT, "just received SIGTERM");
         /* mark the server as no longer running */
         daemon_running = 0;
         break;
   }

}


