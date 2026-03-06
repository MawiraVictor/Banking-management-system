#include<stdio.h>
#include<stdlib.h>
#include "headers/account.h"
#include "headers/transaction.h"
#include "headers/utils.h"
int main(){
    int choice;

    system("color 30");

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter your account No: ");
    scanf("%d", &accNumber);
    clearInputBuffer();

    addTransaction("ACCOUNT OPENED", 500);//Add initial amount the account should hold

    while(1) {
        menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice){
            case 1 :
                depositMoney();
                break;
            case 2 :
                withdrawMoney();
                break;
            case 3 :
                transferMoney();
                break;
            case 4 :
                accountDetails();
                break;
            case 5 :
                transactionDetails();
                break;
            case 6:
                printf("\nThank you for using our banking system");
                printf("Final balance: %d\n", amount);
                exit(0);
            default:
                printf("\nINVALID CHOICE !!!");

        }
        printf("\nPress Enter to continue...");
        getchar();
    }
    return 0;
}