#ifndef HISTORY_H
#define HISTORY_H

#define MAX_TRANSACTIONS 100

// Transaction history variables (extern)
extern char transactionType[MAX_TRANSACTIONS][20];
extern int transactionAmount[MAX_TRANSACTIONS];
extern int transactionBalance[MAX_TRANSACTIONS];
extern char transactionTime[MAX_TRANSACTIONS][30];
extern int transactionCount;

// Function declarations
void transactionDetails();
void addTransaction(char *type, int transAmount);
void getCurrentTime(char *buffer);

#endif