#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char name[20];
int depAmount, amount=50, accNumber, withAmount, transMoney;
int choice;

void menu();
void depositMoney();
void withdrawMoney();
void transferMoney();
void accountDetails();
void transactionDetails();


int main(){

    system("color 30");
    printf("Enter your name: ");

    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter your account No: ");
    scanf("%d", &accNumber);

    menu();
    printf("Enter your choice: ");
    scanf("%d", &choice);

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
            exit(0);
        default:
        printf("INVALID CHOICE !!!");
        break;
    }
    return 0;
}

void menu(){
    printf("\tMAIN MENU   \n\n");
    printf("1. Deposit money\n");
    printf("2. Withdraw money\n");
    printf("3. Transfer money\n");
    printf("4. Account Details\n");
    printf("5. Transaction details\n");
    printf("6. Exit\n\n");
}

void depositMoney(){
    printf("\n\tDEPOSITING MONEY \n\n");
    printf("Enter the amount: ");
    scanf("%d", &depAmount);

    amount += depAmount;
    printf("\n\tMONEY DEPOSITED\n");
    printf("Current balance: %d \n", amount);
    menu();
}

void withdrawMoney(){
    printf("\n\tWITHDRAWING MONEY\n\n");
    printf("Enter amount of money to withdraw: ");
    scanf("%d", &withAmount);

    if(withAmount < amount){
         amount -= withAmount;
    }else{
        printf("\nInsufficient funds, try a lower amount \n");
        withdrawMoney();
    }
   
    printf("New balance: %d \n", amount);
    menu();

}

void transferMoney(){
    printf("\n\tINITIATING MONEY TRANFER\n\n");
    printf("Enter the amount you of money wish to tranfer: ");
    scanf("%d", &transMoney);
    
    if(transMoney < amount){
        amount -= transMoney;
    }else{
        printf("\nInsufficient funds, try tranferring a lower amount \n");
        printf("Your current balance is: %d \n", amount);
        transferMoney();
    }
    printf("Your New balance is: %d\n", amount);
}

void accountDetails(){
    printf("\n\tDISPLAYING ACCOUNT DETAILS \n\n");
    printf("Hello %s\n", name);
    printf("Your account details are: \n");
    printf("Your account Number is %d\n", accNumber );
    printf("Your current balance is: %d\n", amount);
}

void transactionDetails(){
}