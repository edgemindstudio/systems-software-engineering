#include "sshell/shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int sshell_is_builtin(const char *n)
{
    return n!=NULL && (strcmp(n,"cd")==0 || strcmp(n,"pwd")==0 ||
        strcmp(n,"status")==0 || strcmp(n,"help")==0 || strcmp(n,"exit")==0);
}
static int parse_status(const char *s,int *out)
{
    errno=0; char *end=NULL; long v=strtol(s,&end,10);
    if (errno!=0 || end==s || *end!='\0' || v<0L || v>255L) return -1;
    *out=(int)v; return 0;
}
int sshell_run_builtin(SshellCommand *c,SshellState *st)
{
    const char *n=c->argv[0];
    if (strcmp(n,"cd")==0) {
        if (c->argc>2U) { (void)fprintf(stderr,"sshell: cd: too many arguments\n"); return 2; }
        const char *target=c->argc==2U?c->argv[1]:getenv("HOME");
        if (target==NULL) { (void)fprintf(stderr,"sshell: cd: HOME is not set\n"); return 1; }
        if (chdir(target)!=0) { sshell_print_error("cd",target); return 1; }
        return 0;
    }
    if (strcmp(n,"pwd")==0) {
        char *cwd=getcwd(NULL,0U);
        if (cwd==NULL) { sshell_print_error("getcwd",NULL); return 1; }
        (void)printf("%s\n",cwd); free(cwd); return 0;
    }
    if (strcmp(n,"status")==0) { (void)printf("%d\n",st->last_status); return 0; }
    if (strcmp(n,"help")==0) {
        (void)printf("builtins: cd pwd status help exit\noperators: | < > >>\n"); return 0;
    }
    if (strcmp(n,"exit")==0) {
        int rc=st->last_status;
        if (c->argc>2U) { (void)fprintf(stderr,"sshell: exit: too many arguments\n"); return 2; }
        if (c->argc==2U && parse_status(c->argv[1],&rc)!=0) {
            (void)fprintf(stderr,"sshell: exit: status must be 0..255\n"); return 2;
        }
        st->should_exit=1; st->exit_status=rc; return rc;
    }
    return 127;
}
