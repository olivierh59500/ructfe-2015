#ifndef COMMON_H
#define COMMON_H

int login_good(char* login);
int account_good(char* account);

void print_header(int show_logout);
void print_footer();

void print_accounts_redirect();
void print_bank_redirect();

#endif
