#include "sshell/shell.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
int sshell_status_from_wait(int wait_status)
{
    if (WIFEXITED(wait_status)) return WEXITSTATUS(wait_status);
    if (WIFSIGNALED(wait_status)) return 128 + WTERMSIG(wait_status);
    return 1;
}
void sshell_print_error(const char *context, const char *detail)
{
    if (detail != NULL)
        (void)fprintf(stderr,"sshell: %s '%s': %s\n",context,detail,strerror(errno));
    else
        (void)fprintf(stderr,"sshell: %s: %s\n",context,strerror(errno));
}
