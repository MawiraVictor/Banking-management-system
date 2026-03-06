#include<stdio.h>
#include<stdlib.h>
#include "headers/utils.h"
#include "headers/account.h"

void clearInputBuffer(){
    int c;
    while ((c = getchar() != '\n' && c != EOF));
}

void printHeader(char *title) { 
    printf("\t     %s\n", title);
}

void printFooter() {
    printf("\n\t====================================\n");
}

void menu() {
    system("cls");
    printHeader("MAIN MENU");
    printf("\t1. Deposit money\n");
    printf("\t2. Withdraw money\n");
    printf("\t3. Transfer money\n");
    printf("\t4. Account Details\n");
    printf("\t5. Transaction Details\n");
    printf("\t6. Exit\n\n");
    printf("\t====================================\n");
    printf("\tAccount: %s | #%d\n", name, accNumber);
    printf("\tBalance: %d\n", amount);
    printFooter();
    printf("\n\tEnter your choice: ");
}
