#include<stdio.h>
#include<string.h>
#include "headers/account.h"
#include "headers/history.h"
#include "headers/utils.h"

//Account variables
char name[20];
int amount = 50;
int accNumber;

void accountDetails(){
    printHeader("ACCOUNT DETAIILS");
    printf("\tAccount Holder: %d\n", name);
    printf("\tAccount number: %d\n", accNumber);
    printf("\tTotal Transactions: %d\n", transactionCount);
    printFooter();
}

void updateAccountName() {
    printf("Enter new name: ");
    clearInputBuffer();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Name updated successfully!\n");
}

void updateAccountNumber() {
    printf("Enter new account number: ");
    scanf("%d", &accNumber);
    clearInputBuffer();
    printf("Account number updated successfully!\n");
}