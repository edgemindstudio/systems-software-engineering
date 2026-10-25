#include "sshell/shell.h"
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int install_signals(void)
{
    struct sigaction a; a.sa_handler=SIG_IGN;
    if(sigemptyset(&a.sa_mask)!=0)return -1; a.sa_flags=0;
    if(sigaction(SIGINT,&a,NULL)!=0)return -1;
    if(sigaction(SIGQUIT,&a,NULL)!=0)return -1;
    return 0;
}
static int run_line(const char *line,SshellState *st)
{
    SshellPipeline p; int pr=sshell_parse_line(line,&p);
    if(pr!=0){st->last_status=pr;return pr;}
    int rc=sshell_execute_pipeline(&p,st); sshell_pipeline_destroy(&p); return rc;
}
static int run_stream(SshellState *st)
{
    char *line=NULL; size_t cap=0U; int interactive=isatty(STDIN_FILENO);
    while(!st->should_exit) {
        if(interactive){(void)fputs("sshell$ ",stdout);(void)fflush(stdout);}
        errno=0; ssize_t n=getline(&line,&cap,stdin);
        if(n<0) {
            if(feof(stdin))break;
            if(errno==EINTR){clearerr(stdin);continue;}
            sshell_print_error("getline",NULL); st->last_status=1; break;
        }
        (void)run_line(line,st);
    }
    free(line); return st->should_exit?st->exit_status:st->last_status;
}
int main(int argc,char **argv)
{
    if(install_signals()!=0){sshell_print_error("sigaction",NULL);return 1;}
    SshellState st={0,0,0};
    if(argc==1)return run_stream(&st);
    if(argc==3 && strcmp(argv[1],"-c")==0){(void)run_line(argv[2],&st);return st.should_exit?st.exit_status:st.last_status;}
    (void)fprintf(stderr,"usage: sshell [-c COMMAND]\n"); return 2;
}
