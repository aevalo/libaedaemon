/* For syslog */
#include <syslog.h>

/* For exit() and atexit() */
#include <stdlib.h>

/* For sprintf() */
#include <stdio.h>

/* For open() */
#include <fcntl.h>

/* For errno */
#include <errno.h>

/* For UUID handling */
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>

/* For APR and APR-Util */
#include <apr.h>
#include <apu.h>


/* Change this to whatever your daemon is called */
#define DAEMON_NAME "aedaemon1"

/* Change this to the user under which to run */
#define RUN_AS_USER "aevalo"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void apr_daemon_shutdown()
{
  /* Terminate Apache Portable Runtime */
  apr_terminate();
}

static void child_handler(int signum)
{
  switch(signum)
  {
    case SIGALRM:
      exit(EXIT_FAILURE);
      break;
    case SIGUSR1:
      exit(EXIT_SUCCESS);
      break;
    case SIGCHLD:
      exit(EXIT_FAILURE);
      break;
  }
}

static void daemonize(const char *lockfile)
{
  char s[128];
  sprintf(s, ">>> %s", __PRETTY_FUNCTION__);
  syslog(LOG_INFO, s);

  pid_t pid, sid, parent;
  int lfp = -1;

  /* already a daemon */
  syslog(LOG_INFO, "Getting ppid!");
  if (getppid() == 1)
  {
    syslog(LOG_INFO, "Already a daemon, no need to re-daemonize!");
    return;
  }

  /* Create the lock file as the current user */
  syslog(LOG_INFO, "Checking lock files!");
  if (lockfile && lockfile[0])
  {
    syslog(LOG_INFO, "Opening lock file!");
    lfp = open(lockfile, O_RDWR|O_CREAT, 0640);
    if (lfp < 0)
    {
      syslog(LOG_INFO, "Lock file opening failed!");
      syslog(LOG_ERR, "unable to create lock file %s, code=%d (%s)",
             lockfile, errno, strerror(errno));
      exit(EXIT_FAILURE);
    }
  }

  /* Drop user if there is one, and we were run as root */
  if (getuid() == 0 || geteuid() == 0)
  {
    struct passwd *pw = (struct passwd *)getpwnam(RUN_AS_USER);
    if (pw)
    {
      syslog(LOG_NOTICE, "setting user to " RUN_AS_USER);
      setuid(pw->pw_uid);
    }
  }

  /* Trap signals that we expect to recieve */
  signal(SIGCHLD, child_handler);
  signal(SIGUSR1, child_handler);
  signal(SIGALRM, child_handler);

  /* Fork off the parent process */
  syslog(LOG_INFO, "Forking...");
  pid = fork();
  if (pid < 0)
  {
    syslog(LOG_ERR, "unable to fork daemon, code=%d (%s)",
           errno, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* If we got a good PID, then we can exit the parent process. */
  if (pid > 0)
  {
    /* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
     * for two seconds to elapse (SIGALRM).  pause() should not return. */
    alarm(2);
    pause();
    exit(EXIT_FAILURE);
  }

  /* At this point we are executing as the child process */
  parent = getppid();

  /* Cancel certain signals */
  signal(SIGCHLD, SIG_DFL); /* A child process dies */
  signal(SIGTSTP, SIG_IGN); /* Various TTY signals */
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGHUP,  SIG_IGN); /* Ignore hangup signal */
  signal(SIGTERM, SIG_DFL); /* Die on SIGTERM */

  /* Change the file mode mask */
  umask(0);

  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0)
  {
    syslog(LOG_ERR, "unable to create a new session, code %d (%s)",
           errno, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* Change the current working directory.  This prevents the current
   * directory from being locked; hence not being able to remove it. */
  if ((chdir("/")) < 0)
  {
    syslog(LOG_ERR, "unable to change directory to %s, code %d (%s)",
           "/", errno, strerror(errno));
    exit(EXIT_FAILURE);
  }

  /* Redirect standard files to /dev/null */
  freopen("/dev/null", "r", stdin);
  freopen("/dev/null", "w", stdout);
  freopen("/dev/null", "w", stderr);

  /* Tell the parent process that we are A-okay */
  syslog(LOG_INFO, "Killing parent...");
  kill(parent, SIGUSR1);

  sprintf(s, "<<< %s", __PRETTY_FUNCTION__);
  syslog(LOG_INFO, s);
}


int main(int argc, char *argv[], const char* env[])
{
  /* Initialize Apache Portable Runtime */
  apr_status_t rv;
  rv = apr_app_initialize(&argc, &argv, &env);

  atexit(apr_daemon_shutdown());

  /* Initialize the logging interface */
  openlog(DAEMON_NAME, LOG_PID, LOG_LOCAL5);
  syslog(LOG_INFO, "starting");

  /* One may wish to process command line arguments here */

  /* Daemonize */
  daemonize("/var/lock/subsys/" DAEMON_NAME);

  /* Now we are a daemon -- do the work for which we were paid */

  /* Finish up */
  syslog(LOG_INFO, "terminated");
  closelog();

  return 0;
}
