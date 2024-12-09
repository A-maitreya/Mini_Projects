#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 100

typedef struct {
    int accountNumber;
    char name[50];
    float balance;
    char accountType[20];
} Account;

typedef struct {
    int accountNumber;
    char type[20];
    float amount;
    char date[11];
} Transaction;

Account accounts[MAX_ACCOUNTS];
Transaction transactions[MAX_TRANSACTIONS];
int accountCount = 0;
int transactionCount = 0;

void getCurrentDate(char *dateBuffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateBuffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

void loadAccounts() {
    FILE *file = fopen("accounts.dat", "rb");
    if (!file) {
        printf("Error loading accounts data. Starting with an empty database.\n");
        return;
    }
    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void saveAccounts() {
    FILE *file = fopen("accounts.dat", "wb");
    if (!file) {
        printf("Error saving accounts data.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), accountCount, file);
    fclose(file);
}

void createAccount() {
    if (accountCount >= MAX_ACCOUNTS) {
        printf("Maximum account limit reached.\n");
        return;
    }

    Account newAccount;
    newAccount.accountNumber = accountCount + 1;
    printf("Enter account holder's name: ");
    scanf("%49s", newAccount.name);
    printf("Enter account type (Savings/Current): ");
    scanf("%19s", newAccount.accountType);
    newAccount.balance = 0.0;

    accounts[accountCount] = newAccount;
    accountCount++;
    saveAccounts();
    printf("Account created successfully. Account Number: %d\n", newAccount.accountNumber);
}

void depositMoney() {
    int accNumber;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &accNumber);

    if (accNumber <= 0 || accNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid deposit amount.\n");
        return;
    }

    accounts[accNumber - 1].balance += amount;
    char date[11];
    getCurrentDate(date);
    Transaction newTransaction = { accNumber, "Deposit", amount, date };
    transactions[transactionCount++] = newTransaction;

    saveAccounts();
    printf("Deposited %.2f to account number %d. New balance: %.2f\n", amount, accNumber, accounts[accNumber - 1].balance);
}

void withdrawMoney() {
    int accNumber;
    float amount;
    printf("Enter account number: ");
    scanf("%d", &accNumber);

    if (accNumber <= 0 || accNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    if (amount <= 0) {
        printf("Invalid withdrawal amount.\n");
        return;
    }

    if (accounts[accNumber - 1].balance < amount) {
        printf("Insufficient funds.\n");
        return;
    }

    accounts[accNumber - 1].balance -= amount;
    char date[11];
    getCurrentDate(date);
    Transaction newTransaction = { accNumber, "Withdraw", amount, date };
    transactions[transactionCount++] = newTransaction;

    saveAccounts();
    printf("Withdrew %.2f from account number %d. New balance: %.2f\n", amount, accNumber, accounts[accNumber - 1].balance);
}

void checkBalance() {
    int accNumber;
    printf("Enter account number: ");
    scanf("%d", &accNumber);

    if (accNumber <= 0 || accNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Account number: %d, Account holder: %s, Balance: %.2f, Account type: %s\n",
           accNumber, accounts[accNumber - 1].name, accounts[accNumber - 1].balance, accounts[accNumber - 1].accountType);
}

void viewTransactions() {
    int accNumber;
    printf("Enter account number: ");
    scanf("%d", &accNumber);

    if (accNumber <= 0 || accNumber > accountCount) {
        printf("Invalid account number.\n");
        return;
    }

    printf("Transaction history for account number %d:\n", accNumber);
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == accNumber) {
            printf("%s: %.2f on %s\n", transactions[i].type, transactions[i].amount, transactions[i].date);
        }
    }
}

void displayMenu() {
    printf("\n--- Advanced Bank Management System ---\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("5. View Transactions\n");
    printf("6. Exit\n");
    printf("---------------------------------------\n");
}

int main() {
    loadAccounts();
    int choice;

    while (1) {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: viewTransactions(); break;
            case 6:
                printf("Exiting the system. Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

}