WeChat: cstutorcs
QQ: 749389476
Email: tutorcs@163.com
/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

void init_ui(void);

char *prompt_line(void);
char *prompt_username(void);
char *prompt_hostname(void);
char *prompt_cwd(void);
int prompt_status(void);
unsigned int prompt_cmd_num(void);

char *read_command(void);

int key_up(int count, int key);
int key_down(int count, int key);

#endif
