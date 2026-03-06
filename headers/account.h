#ifndef ACCOUNT_H
#define ACCOUNT_H

// Account-related global variables (use extern)
extern char name[20];
extern int amount;
extern int accNumber;
extern int choice;

// Function declarations
void accountDetails();
void updateAccountName();
void updateAccountNumber();

#endif