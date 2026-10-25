#ifndef SSHELL_SHELL_H
#define SSHELL_SHELL_H
#include <stddef.h>
#define SSHELL_MAX_COMMANDS 16U
#define SSHELL_MAX_ARGS 64U
typedef struct {
    char *argv[SSHELL_MAX_ARGS + 1U];
    size_t argc;
    char *input_path;
    char *output_path;
    int append_output;
} SshellCommand;
typedef struct {
    SshellCommand commands[SSHELL_MAX_COMMANDS];
    size_t count;
} SshellPipeline;
typedef struct {
    int last_status;
    int should_exit;
    int exit_status;
} SshellState;
int sshell_parse_line(const char *line, SshellPipeline *pipeline);
void sshell_pipeline_destroy(SshellPipeline *pipeline);
int sshell_execute_pipeline(SshellPipeline *pipeline, SshellState *state);
int sshell_is_builtin(const char *name);
int sshell_run_builtin(SshellCommand *command, SshellState *state);
int sshell_status_from_wait(int wait_status);
void sshell_print_error(const char *context, const char *detail);
#endif
