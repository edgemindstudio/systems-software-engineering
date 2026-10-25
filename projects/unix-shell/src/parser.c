#include "sshell/shell.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { TOK_WORD,TOK_PIPE,TOK_IN,TOK_OUT,TOK_APPEND,TOK_END,TOK_ERROR } TokenType;
typedef struct { TokenType type; char *text; } Token;

static int append_char(char **buf,size_t *len,size_t *cap,char ch)
{
    if (*len + 1U >= *cap) {
        size_t n=*cap==0U?32U:*cap*2U;
        char *g=realloc(*buf,n);
        if (g==NULL) return -1;
        *buf=g; *cap=n;
    }
    (*buf)[*len]=ch; ++*len; (*buf)[*len]='\0';
    return 0;
}

static Token next_token(const char **cur)
{
    const char *p=*cur;
    while (isspace((unsigned char)*p)!=0) ++p;
    Token t={TOK_END,NULL};
    if (*p=='\0') { *cur=p; return t; }
    if (*p=='|') { t.type=TOK_PIPE; *cur=p+1; return t; }
    if (*p=='<') { t.type=TOK_IN; *cur=p+1; return t; }
    if (*p=='>') {
        if (p[1]=='>') { t.type=TOK_APPEND; *cur=p+2; }
        else { t.type=TOK_OUT; *cur=p+1; }
        return t;
    }

    char *word=NULL; size_t len=0U,cap=0U;
    while (*p!='\0' && isspace((unsigned char)*p)==0 && *p!='|' && *p!='<' && *p!='>') {
        if (*p=='\'') {
            ++p;
            while (*p!='\0' && *p!='\'') {
                if (append_char(&word,&len,&cap,*p++)!=0) goto fail;
            }
            if (*p!='\'') goto fail;
            ++p; continue;
        }
        if (*p=='"') {
            ++p;
            while (*p!='\0' && *p!='"') {
                if (*p=='\\' && p[1]!='\0') ++p;
                if (append_char(&word,&len,&cap,*p++)!=0) goto fail;
            }
            if (*p!='"') goto fail;
            ++p; continue;
        }
        if (*p=='\\') {
            ++p;
            if (*p=='\0') goto fail;
        }
        if (append_char(&word,&len,&cap,*p++)!=0) goto fail;
    }
    if (word==NULL) {
        word=malloc(1U);
        if (word==NULL) goto fail;
        word[0]='\0';
    }
    t.type=TOK_WORD; t.text=word; *cur=p; return t;
fail:
    free(word); t.type=TOK_ERROR; return t;
}

void sshell_pipeline_destroy(SshellPipeline *p)
{
    if (p==NULL) return;
    for (size_t i=0U;i<p->count;++i) {
        SshellCommand *c=&p->commands[i];
        for (size_t j=0U;j<c->argc;++j) { free(c->argv[j]); c->argv[j]=NULL; }
        free(c->input_path); free(c->output_path);
        c->input_path=NULL; c->output_path=NULL; c->argc=0U; c->append_output=0;
    }
    p->count=0U;
}

int sshell_parse_line(const char *line,SshellPipeline *p)
{
    (void)memset(p,0,sizeof *p); p->count=1U;
    const char *cur=line;
    TokenType pending=TOK_END;
    for (;;) {
        Token t=next_token(&cur);
        if (t.type==TOK_ERROR) {
            (void)fprintf(stderr,"sshell: parse error\n");
            sshell_pipeline_destroy(p); return 2;
        }
        SshellCommand *c=&p->commands[p->count-1U];
        if (pending!=TOK_END) {
            if (t.type!=TOK_WORD) {
                (void)fprintf(stderr,"sshell: redirection requires a path\n");
                free(t.text); sshell_pipeline_destroy(p); return 2;
            }
            if (pending==TOK_IN) { free(c->input_path); c->input_path=t.text; }
            else { free(c->output_path); c->output_path=t.text; c->append_output=(pending==TOK_APPEND); }
            pending=TOK_END; continue;
        }
        if (t.type==TOK_END) {
            if (c->argc==0U) {
                if (p->count==1U) { p->count=0U; return 0; }
                (void)fprintf(stderr,"sshell: pipeline ends with empty command\n");
                sshell_pipeline_destroy(p); return 2;
            }
            return 0;
        }
        if (t.type==TOK_WORD) {
            if (c->argc>=SSHELL_MAX_ARGS) {
                (void)fprintf(stderr,"sshell: too many arguments\n");
                free(t.text); sshell_pipeline_destroy(p); return 2;
            }
            c->argv[c->argc++]=t.text; c->argv[c->argc]=NULL; continue;
        }
        if (t.type==TOK_PIPE) {
            if (c->argc==0U) {
                (void)fprintf(stderr,"sshell: empty pipeline stage\n");
                sshell_pipeline_destroy(p); return 2;
            }
            if (p->count>=SSHELL_MAX_COMMANDS) {
                (void)fprintf(stderr,"sshell: too many pipeline stages\n");
                sshell_pipeline_destroy(p); return 2;
            }
            ++p->count; continue;
        }
        if (t.type==TOK_IN || t.type==TOK_OUT || t.type==TOK_APPEND) { pending=t.type; continue; }
    }
}
