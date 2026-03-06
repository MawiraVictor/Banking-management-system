#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

char name[20];
int depAmount, amount=50, accNumber, withAmount, transMoney;
int choice;

//Transaction details variables
#define MAX_TRANSACTIONS 100 // max number of transactions to store
char transactionType[MAX_TRANSACTIONS][20];
int transactionAmount[MAX_TRANSACTIONS];
int transactionBalance[MAX_TRANSACTIONS];
char transactionTime[MAX_TRANSACTIONS][30]; //store timestamp
int transactionCount = 0;


void menu();
void depositMoney();
void withdrawMoney();
void transferMoney();
void accountDetails();
void transactionDetails();
void clearInputBuffer();
void addTransaction(char *type, int transAmount);  // New function to record transactions
void getCurrentTime(char *buffer);  // New function to get timestamp

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

// Function to get current time as string
void getCurrentTime(char *buffer) {
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Function to add a transaction to history
void addTransaction(char *type, int transAmount) {
    if (transactionCount < MAX_TRANSACTIONS) {
        strcpy(transactionType[transactionCount], type);
        transactionAmount[transactionCount] = transAmount;
        transactionBalance[transactionCount] = amount;
        getCurrentTime(transactionTime[transactionCount]);
        transactionCount++;
    } else {
        // Shift transactions to make room (remove oldest)
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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
    printf("Enter the amount you wish to deposit: ");
    scanf("%d", &depAmount);
    clearInputBuffer();

    amount += depAmount;
    printf("\n\tMONEY DEPOSITED\n");
    printf("Current balance: %d \n", amount);
    menu();
}

void withdrawMoney(){
    printf("\n\tWITHDRAWING MONEY\n\n");
    printf("Enter amount of money to withdraw: ");
    scanf("%d", &withAmount);
    clearInputBuffer();


    if(withAmount < amount){
         amount -= withAmount;
    }else{
        printf("\nInsufficient funds, try a lower amount \n");
        withdrawMoney();
    }

    printf("New balance: %d \n", amount);
    clearInputBuffer();
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
    printf("\n\tTRANSACTION HISTORY\n\n");

    if (transactionCount == 0) {
        printf("\tNo transactions yet\n");
    }else{
        printf("\t%-5s %-12s %-10s %-10s %s\n",
            "#", "Type", "Amount", "Balance", "Date & Time");

        for (int i = 0; i < transactionCount; i++) {
            printf("\t%-5d %-12s %-10d %-10d %s\n",
                   i+1,
                   transactionType[i],
                   transactionAmount[i],
                   transactionBalance[i],
                   transactionTime[i]);
        }
    }
}
