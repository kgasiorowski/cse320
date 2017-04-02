#include "sfish.h"
#include "debug.h"

static unsigned int alarm_seconds;

static void sio_reverse(char s[]){
    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

static void sio_ltoa(long v, char s[], int b){
    int c, i = 0;
    int neg = v < 0;

    if (neg)
	v = -v;

    do {
        s[i++] = ((c = (v % b)) < 10)  ?  c + '0' : c - 10 + 'a';
    } while ((v /= b) > 0);

    if (neg)
	s[i++] = '-';

    s[i] = '\0';
    sio_reverse(s);
}

static size_t sio_strlen(char s[]){
    int i = 0;

    while (s[i] != '\0')
        ++i;
    return i;
}

ssize_t sio_puts(char s[]){
    return write(STDOUT_FILENO, s, sio_strlen(s));
}

ssize_t sio_putl(long v)
{
    char s[128];

    sio_ltoa(v, s, 10);
    return sio_puts(s);
}

//Handles child signal
void child_handler(int sig, siginfo_t *info, void *flags){

	int saved_errno = errno;
	pid_t pid;
	while((pid = waitpid(-1, NULL, WNOHANG)) > 0){

		sio_puts("Child with pid: ");
		sio_putl((long)pid);
		sio_puts(" has died. It spent ");
		sio_putl((long)((double)info->si_utime + (double)info->si_stime)/(sysconf(_SC_CLK_TCK)));
		sio_puts(" seconds utilizing the cpu\n");

	}
	errno = saved_errno;

}

//Handles SIGUSR2
void sigusr2_handler(int sig){

	int saved_errno = errno;
	sio_puts("Well that was easy.");
	errno = saved_errno;

}

//Handles SIGALRM
void sigalrm_handler(int sig){

	sio_puts("Your ");
	sio_putl((long)alarm_seconds);
	sio_puts(" second timer has finished!\n");

}

void setup_signals(){

    //Implement handler for child sigaction
    struct sigaction sca, susra, slrma;

	// signal(SIGCHLD, child_handler);

    sca.sa_sigaction = &child_handler;
    sigemptyset(&sca.sa_mask);
    sca.sa_flags = SA_SIGINFO;
    if (sigaction(SIGCHLD, &sca, 0) == -1) {
      fprintf(stderr, "SIGCHILD handler could not be installed, aborting\n");
      exit(1);
    }

	// signal(SIGUSR2, sigusr2_handler);

    susra.sa_handler = &sigusr2_handler;
    sigemptyset(&susra.sa_mask);
    susra.sa_flags = 0;
    if (sigaction(SIGUSR2, &susra, 0) == -1) {
      fprintf(stderr, "SIGUSR2 handler could not be installed, aborting\n");
      exit(1);
    }

	// signal(SIGALRM, sigalrm_handler);

    slrma.sa_handler = &sigalrm_handler;
    sigemptyset(&slrma.sa_mask);
    slrma.sa_flags = 0;
    if (sigaction(SIGALRM, &slrma, 0) == -1) {
      fprintf(stderr, "SIGALRM handler could not be installed, aborting\n");
      exit(1);
    }

    //Initialize signal mask
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);

    //Block sigstop
    sigprocmask(SIG_BLOCK, &mask, NULL);

}

void my_alarm(unsigned int seconds){

	sio_puts("Starting alarm for ");
	sio_putl((long)seconds);
	sio_puts(" seconds...\n");

	alarm_seconds = seconds;

	pid_t pid = fork();

	if(pid < 0){

		exit(0);

	}else if(pid == 0){

		int sleeptime = sleep(seconds);

		if(sleeptime == 0){

			raise(SIGALRM);
			_exit(0);

		}else{

			fprintf(stderr, "Sleep was interrupted after %d seconds\n", seconds-sleeptime);

		}

	}

}
