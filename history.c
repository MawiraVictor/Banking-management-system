#include<stdio.h>
#include<string.h>
#include<time.h>
#include "headers/history.h"
#include "headers/utils.h"
#include "headers/account.h"

// Transaction history variables
char transactionType[MAX_TRANSACTIONS][20];
int transactionAmount[MAX_TRANSACTIONS];
int transactionBalance[MAX_TRANSACTIONS];
char transactionTime[MAX_TRANSACTIONS][30];
int transactionCount = 0;

void getCurrentTime(char *buffer) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

void addTransaction(char *type, int transAmount) {
    if (transactionCount < MAX_TRANSACTIONS) {
        strcpy(transactionType[transactionCount], type);
        transactionAmount[transactionCount] = transAmount;
        transactionBalance[transactionCount] = amount;
        getCurrentTime(transactionTime[transactionCount]);
        transactionCount++;
    } else {
        // remove oldest transactions to make room for new transactions
        for (int i = 0; i < MAX_TRANSACTIONS - 1; i++) {
            strcpy(transactionType[i], transactionType[i+1]);
            transactionAmount[i] = transactionAmount[i+1];
            transactionBalance[i] = transactionBalance[i+1];
            strcpy(transactionTime[i], transactionTime[i+1]);
        }
        // Add new transaction at the end
        strcpy(transactionType[MAX_TRANSACTIONS-1], type);
        transactionAmount[MAX_TRANSACTIONS-1] = transAmount;
        transactionBalance[MAX_TRANSACTIONS-1] = amount;
        getCurrentTime(transactionTime[MAX_TRANSACTIONS-1]);
    }
}

void transactionDetails() {
    printHeader("TRANSACTION HISTORY");
    
    if (transactionCount == 0) {
        printf("\tNo transactions yet.\n");
    } else {
        printf("\t%-4s %-12s %-8s %-8s %s\n", 
               "#", "Type", "Amount", "Balance", "Date & Time");
        printf("\t------------------------------------\n");
        
        for (int i = 0; i < transactionCount; i++) {
            printf("\t%-4d %-12s %-8d %-8d %s\n", 
                   i+1, 
                   transactionType[i], 
                   transactionAmount[i],
                   transactionBalance[i],
                   transactionTime[i]);
        }
    }
    printFooter();
}