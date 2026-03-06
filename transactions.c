#include<stdio.h>
#include "headers/transaction.h"
#include "headers/account.h"
#include "headers/history.h"
#include "headers/utils.h"

void depositMoney(){
    int depAmount;
    printHeader("DEPOSITING MONEY");
    printf("\tEnter the amount: ");
    scanf("%d", &depAmount);
    clearInputBuffer();

    if (depAmount > 0) {
        amount += depAmount;
        addTransaction("DEPOSIT", depAmount);
        printf("\t     MONEY DEPOSITED SUCCESSFULLY\n");
        printf("\tAmount: +%d\n", depAmount);
        printf("\tNew balance: %d\n", amount);
    }else{
        print("\n\tInvalid amount!\n");
    }
    printFooter();
}

void withdrawMoney(){
    int withAmount;
    printHeader("WITHDRAWING MONEY");
    printf("\tEnter amount to withdraw: ");
    scanf("%d", &withAmount);
    clearInputBuffer();

     if(withAmount > 0 && withAmount <= amount) {
        amount -= withAmount;
        addTransaction("WITHDRAW", withAmount);
        
        printf("\t    WITHDRAWAL SUCCESSFUL\n");
        printf("\tAmount: -%d\n", withAmount);
        printf("\tNew balance: %d\n", amount);
    } else if (withAmount <= 0) {
        printf("\n\tInvalid amount!\n");
    } else {
        printf("\t     INSUFFICIENT FUNDS!\n");

        printf("\tYour current balance is: %d\n", amount);
    }
    printFooter();
}

void transferMoney() {
    int transAmount;
    printHeader("INITIATING MONEY TRANSFER");
    printf("\tEnter the amount to transfer: ");
    scanf("%d", &transAmount);
    clearInputBuffer();
    
    if(transAmount > 0 && transAmount <= amount) {
        amount -= transAmount;
        addTransaction("TRANSFER", transAmount);
        
        printf("\t     TRANSFER SUCCESSFUL\n");
        
        printf("\tAmount: -%d\n", transAmount);
        printf("\tNew balance: %d\n", amount);
    } else if (transAmount <= 0) {
        printf("\n\tInvalid amount!\n");
    } else {
        printf("\t     INSUFFICIENT FUNDS!\n");

        printf("\tYour current balance is: %d\n", amount);
    }
    printFooter();
}
