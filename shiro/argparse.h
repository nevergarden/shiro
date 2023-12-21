#ifndef SHIRO_ARGPARSE_H
#define SHIRO_ARGPARSE_H

typedef enum { Interactive } program_mode;

extern program_mode parse(int argc, char *argv[]);

#endif
