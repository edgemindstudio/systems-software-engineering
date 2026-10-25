#include "sshell/shell.h"
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int redirect_command(const SshellCommand *c)
{
    if (c->input_path!=NULL) {
        int fd=open(c->input_path,O_RDONLY);
        if (fd<0) { sshell_print_error("open",c->input_path); return -1; }
        if (dup2(fd,STDIN_FILENO)<0) { sshell_print_error("dup2",c->input_path); (void)close(fd); return -1; }
        (void)close(fd);
    }
    if (c->output_path!=NULL) {
        int flags=O_WRONLY|O_CREAT|(c->append_output?O_APPEND:O_TRUNC);
        int fd=open(c->output_path,flags,0666);
        if (fd<0) { sshell_print_error("open",c->output_path); return -1; }
        if (dup2(fd,STDOUT_FILENO)<0) { sshell_print_error("dup2",c->output_path); (void)close(fd); return -1; }
        (void)close(fd);
    }
    return 0;
}
static int parent_builtin(SshellCommand *c,SshellState *st)
{
    int in=-1,out=-1,rc=1;
    if (c->input_path!=NULL) { in=dup(STDIN_FILENO); if (in<0) { sshell_print_error("dup stdin",NULL); return 1; } }
    if (c->output_path!=NULL) {
        out=dup(STDOUT_FILENO);
        if (out<0) { sshell_print_error("dup stdout",NULL); if(in>=0)(void)close(in); return 1; }
    }
    if (redirect_command(c)==0) {
        rc=sshell_run_builtin(c,st);
        (void)fflush(stdout); (void)fflush(stderr);
    }
    if (in>=0) { if (dup2(in,STDIN_FILENO)<0) rc=1; (void)close(in); }
    if (out>=0) { if (dup2(out,STDOUT_FILENO)<0) rc=1; (void)close(out); }
    return rc;
}
static void close_pipes(int p[][2],size_t n)
{
    for (size_t i=0U;i<n;++i) {
        if(p[i][0]>=0)(void)close(p[i][0]);
        if(p[i][1]>=0)(void)close(p[i][1]);
    }
}
static void child_signals(void)
{
    struct sigaction a; a.sa_handler=SIG_DFL; (void)sigemptyset(&a.sa_mask); a.sa_flags=0;
    (void)sigaction(SIGINT,&a,NULL); (void)sigaction(SIGQUIT,&a,NULL);
}
int sshell_execute_pipeline(SshellPipeline *p,SshellState *st)
{
    if (p->count==0U) return st->last_status;
    if (p->count==1U && sshell_is_builtin(p->commands[0].argv[0])) {
        int rc=parent_builtin(&p->commands[0],st); st->last_status=rc; return rc;
    }
    int pipes[SSHELL_MAX_COMMANDS-1U][2];
    for(size_t i=0U;i<SSHELL_MAX_COMMANDS-1U;++i){pipes[i][0]=-1;pipes[i][1]=-1;}
    size_t np=p->count-1U;
    for(size_t i=0U;i<np;++i) {
        if(pipe(pipes[i])!=0){sshell_print_error("pipe",NULL);close_pipes(pipes,np);st->last_status=1;return 1;}
    }
    pid_t pids[SSHELL_MAX_COMMANDS]; size_t started=0U;
    for(size_t i=0U;i<p->count;++i) {
        pid_t pid=fork();
        if(pid<0) {
            sshell_print_error("fork",NULL); close_pipes(pipes,np);
            for(size_t j=0U;j<started;++j)(void)waitpid(pids[j],NULL,0);
            st->last_status=1; return 1;
        }
        if(pid==0) {
            child_signals();
            if(i>0U && dup2(pipes[i-1U][0],STDIN_FILENO)<0)_exit(1);
            if(i+1U<p->count && dup2(pipes[i][1],STDOUT_FILENO)<0)_exit(1);
            close_pipes(pipes,np);
            if(redirect_command(&p->commands[i])!=0)_exit(1);
            if(sshell_is_builtin(p->commands[i].argv[0])) {
                SshellState child=*st; int rc=sshell_run_builtin(&p->commands[i],&child);
                (void)fflush(stdout); (void)fflush(stderr); _exit(rc);
            }
            execvp(p->commands[i].argv[0],p->commands[i].argv);
            int saved=errno; sshell_print_error("execvp",p->commands[i].argv[0]);
            _exit(saved==ENOENT?127:126);
        }
        pids[started++]=pid;
    }
    close_pipes(pipes,np);
    int last=1;
    for(size_t i=0U;i<started;++i) {
        int ws=0; pid_t r;
        do { r=waitpid(pids[i],&ws,0); } while(r<0 && errno==EINTR);
        if(r<0){sshell_print_error("waitpid",NULL);continue;}
        if(i+1U==started)last=sshell_status_from_wait(ws);
    }
    st->last_status=last; return last;
}
