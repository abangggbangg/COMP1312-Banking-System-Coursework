#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define DATABASE_DIR "database/"
#define INDEX_FILE "database/index.txt"
#define TRANSACTION_LOG "database/transactions.log"

void showSessionInfo();
void showMenu();
void toLowerCase(char *str);

long long generateRandomAccountNumber(int minDigits, int maxDigits);
bool isAccountNumberUnique(long long accountNumber);
void appendToIndexFile(long long accountNumber);
void logTransaction(const char *message);

void createAccount();
void deleteAccount();
void depositMoney();
void withdrawMoney();
void remittanceMoney();

//MAIN FUNCTION

int main(){
    system("mkdir -p database");
    showSessionInfo();
    char choice[30];
    while (true){
        showMenu();
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0; // Remove newline character
        toLowerCase(choice);

        if (strcmp(choice, "1") == 0 || strcmp(choice, "create") == 0){
            createAccount();
        }
        else if (strcmp(choice, "2") == 0 || strcmp(choice, "delete") == 0){
            deleteAccount();
        }
        else if (strcmp(choice, "3") == 0 || strcmp(choice, "deposit") == 0){
            depositMoney();
        }
        else if (strcmp(choice, "4") == 0 || strcmp(choice, "withdraw") == 0){
            withdrawMoney();
        }
        else if (strcmp(choice, "5") == 0 || strcmp(choice, "remit") == 0){
            remittanceMoney();
        }
        else if (strcmp(choice, "6") == 0 || strcmp(choice, "exit") == 0){
            printf("\nThank you for using the banking system. Goodbye!\n");
            break;
        }
        else{
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}

void showMenu(){
    printf("\n====== Sigma Banking System ======\n");
    printf("1. Create Account (or type 'create')\n");
    printf("2. Delete Account (or type 'delete')\n");
    printf("3. Deposit Money (or type 'deposit')\n");
    printf("4. Withdrawal (or type 'withdraw')\n");
    printf("5. Remittance (or type 'remit')\n");
    printf("6. Exit (or type 'exit')\n");
    printf("Please enter your choice: ");
}

void toLowerCase(char *str){
    for (int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
}

void showSessionInfo() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    printf("Session started on: %04d-%02d-%02d %02d:%02d:%02d\n",
           t->tm_year + 1900, 
           t->tm_mon + 1, 
           t->tm_mday,
           t->tm_hour, 
           t->tm_min, 
           t->tm_sec);

    FILE *index = fopen(INDEX_FILE, "r");

    if (!index) {
        printf("Loaded accounts: 0\n\n");
        return;
    }

    int count = 0;
    char line [100];

    while (fgets(line, sizeof(line), index)) {
        count++;
    }

    fclose(index);

    printf("Loaded accounts: %d\n\n", count);
}

//ACCOUNT NUMBER FUNCTIONS

long long generateRandomAccountNumber(int minDigits, int maxDigits){
    int digits = minDigits + rand() % (maxDigits - minDigits + 1);

    long long min = 1;
    for (int i = 1; i < digits; i++){
        min *= 10;
    }

    long long max = min * 10 - 1;

    return min + rand() % (max - min + 1); 
}

bool isAccountNumberUnique(long long accountNumber){
    FILE *index = fopen(INDEX_FILE, "r");

    if (!index) return true;

    char line[50];
    char numberStr[20];
    sprintf(numberStr, "%lld", accountNumber);

    while (fgets(line, sizeof(line), index)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        if (strcmp(line, numberStr) == 0){
            fclose(index);
            return false;
        }
    }
    fclose(index);
    return true;
}

void appendToIndexFile(long long accountNumber){
    FILE *index = fopen(INDEX_FILE, "a");
    if (!index) {
        printf("Error opening index file for appending.\n");
        return;
    }

    fprintf(index, "%lld\n", accountNumber);
    fclose(index);
}

//TRANSACTION LOG FUNCTION
void logTransaction(const char *message){
    FILE *logFile = fopen(TRANSACTION_LOG, "a");

    if (!logFile) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
            t->tm_year + 1900, 
            t->tm_mon + 1, 
            t->tm_mday,
            t->tm_hour, 
            t->tm_min, 
            t->tm_sec,
            message);

    fclose(logFile);
}

bool isAlphaSpaceOnly(char *str){
    for (int i = 0; str[i]; i++){
        if (!isalpha(str[i]) && str[i] != ' '){
            return false;
        }
    }
    return true;
}

bool isEightDigitNumber(char *str){
    if (strlen(str) != 8) return false;

    for (int i = 0; i < 8; i++){
        if (!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

bool Alphabet(char *str){
    for (int i = 0; str[i]; i++){
        if (isalpha(str[i])){
            return true;
        }
    }
    return false;
}

//ACCOUNT CREATION FUNCTION

void createAccount(){
    char name[100];
    char id[50];
    char type[15];
    char pin[50];

    printf("\n--- Create New Account ---\n");

    while(true){
        printf("Enter Full Name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0; 

        if (isAlphaSpaceOnly(name) && Alphabet(name)) {
            break;
        } 
        else {
            printf("Invalid name. Please use alphabetic characters and spaces only.\n");
        }
    }

    while (true) {
        printf("Enter Identification Number (8 digits): ");
        fgets(id, sizeof(id), stdin);
        id[strcspn(id, "\n")] = 0;

        if (isEightDigitNumber(id)) {
            break;
        } 
        else {
            printf("Invalid ID. Please enter exactly 8 numeric digits.\n");
        }
    }

    while(true) {
        printf("Enter account type (Savings/Current): ");
        fgets(type, sizeof(type), stdin);
        type[strcspn(type, "\n")] = 0;
        toLowerCase(type);

        if (strcmp(type, "savings") == 0 || strcmp(type, "current") == 0) {
            break;
        } 
        else {
            printf("Invalid account type. Please enter 'Savings' or 'Current'.\n");
        }
    }

    while(true) {
        printf("Enter 4-digit PIN: ");
        fgets(pin, sizeof(pin), stdin);
        pin[strcspn(pin, "\n")] = 0;

        if (strlen(pin) == 4 &&
            isdigit(pin[0]) && 
            isdigit(pin[1]) && 
            isdigit(pin[2]) && 
            isdigit(pin[3])) {
            break;
        } 
        else {
            printf("Invalid PIN. Please enter a 4-digit numeric PIN.\n");
        }
    }

    srand(time(NULL));

    long long accountNumber;

    do {
        accountNumber = generateRandomAccountNumber(7, 9);
    } 
    while (!isAccountNumberUnique(accountNumber));

    char filePath[100];
    sprintf(filePath, "%s%lld.txt", DATABASE_DIR, accountNumber);
    
    FILE *accountFile = fopen(filePath, "w");

    if (!accountFile) {
        printf("Failed to create account file.\n");
        return;
    }

    fprintf(accountFile, "Name: %s\n", name);
    fprintf(accountFile, "ID: %s\n", id);
    fprintf(accountFile, "Account Type: %s\n", type);
    fprintf(accountFile, "PIN: %s\n", pin);
    fprintf(accountFile, "Balance: 0.00\n");

    fclose(accountFile);

    appendToIndexFile(accountNumber);

    char logMsg[200];
    sprintf(logMsg, "Account created: %lld (%s)", accountNumber, name);
    logTransaction(logMsg);

    printf("\nACCOUNT CREATED SUCCESSFULLY!\n");
    printf("Account Number: %lld\n", accountNumber);
    printf("Initial Balance: $0.00\n");
}

//ACCOUNT DELETION FUNCTION

void deleteAccount(){

    FILE *index = fopen(INDEX_FILE, "r");

    if (!index) {
        printf("No accounts found .\n");
        return;
    }

    printf("\n--- Existing Account ---\n");
    char accounts[100][20];
    int count = 0;

    while (fgets(accounts[count], sizeof(accounts[count]), index)) {
        accounts[count][strcspn(accounts[count], "\n")] = 0; 
        printf("%d. %s\n", count + 1, accounts[count]);
        count++;
    }
    fclose(index);

    if (count == 0) {
        printf("No accounts avaiable to delete.\n");
        return;
    }

    long long accountNumber;
    printf("\nEnter the account number to delete: ");
    scanf("%lld", &accountNumber);
    getchar(); 

    char filePath[100];
    sprintf(filePath, "%s%lld.txt", DATABASE_DIR, accountNumber);

    FILE *accountFile = fopen(filePath, "r");

    if (!accountFile) {
        printf("Account number %lld does not exist.\n", accountNumber);
        return;
    }

    char line[200];
    char storedID[50], storedPIN[10];

    while (fgets(line, sizeof(line), accountFile)){
        if (strncmp(line, "ID:", 3) == 0) {
            sscanf(line, "ID: %s", storedID);
        } 
        else if (strncmp(line, "PIN:", 4) == 0) {
            sscanf(line, "PIN: %s", storedPIN);
        }
    }
    fclose(accountFile);

    char last4[5];
    strcpy(last4, storedID + strlen(storedID) - 4);

    char inputLast4[10], inputPIN[10];

    printf("Enter last 4 digits of ID: ");
    fgets(inputLast4, sizeof(inputLast4), stdin);
    inputLast4[strcspn(inputLast4, "\n")] = 0;

    printf("Enter 4-digit PIN: ");
    fgets(inputPIN, sizeof(inputPIN), stdin);
    inputPIN[strcspn(inputPIN, "\n")] = 0;

    if (strcmp(last4, inputLast4) != 0 || strcmp(storedPIN, inputPIN) != 0){
        printf("Authentication failed. Account not deleted.\n");
        return;
    }

    char confirm[10];
    printf("Are you sure you want to delete your account? (type 'y' for yes, 'n' for no): ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = 0;

    toLowerCase(confirm);

    if (strcmp(confirm, "y") != 0){
        printf("Account deletion canceled.\n");
        return;
    }

    if (remove(filePath) == 0) {
        printf("Account deleted successfully.\n");
    } else {
        printf("Error deleting account.\n");
        return;
    }

    //REBUILD INDEX FILE
    index = fopen(INDEX_FILE, "r");
    FILE *temp = fopen("database/temp.txt", "w");

    char lineAcc[50];
    while (fgets(lineAcc, sizeof(lineAcc), index)){
        long long tempAcc = atoll(lineAcc);
        if (tempAcc != accountNumber){
            fprintf(temp, "%lld\n", tempAcc);
        }
}

    fclose(index);
    fclose(temp);

    remove(INDEX_FILE);
    rename("database/temp.txt", INDEX_FILE);

    char logMsg[200];
    sprintf(logMsg, "Account deleted: %lld", accountNumber);
    logTransaction(logMsg);

    printf("\nACCOUNT DELETED SUCCESSFULLY!\n");
}

//DEPOSIT MONEY FUNCTION

void depositMoney() {
    long long accountNumber;
    char pin[10];
    double amount;
    char filePath[100];

    printf("\n--- Deposit Money ---\n");

    FILE *index = fopen(INDEX_FILE, "r");
    if (!index){
        printf("No accounts avaiable to deposit. \n");
        return;
    }

    printf("\n--- Existing Account ---\n");

    char lineAccounts[50];
    int count = 0;

    while (fgets(lineAccounts, sizeof(lineAccounts), index)){
        lineAccounts[strcspn(lineAccounts, "\n")] = 0;
        printf("%d. %s\n", ++count, lineAccounts);
    }

    fclose(index);

    if(count == 0){
        printf("No accounts available to deposit.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%lld", &accountNumber);
    getchar(); // Clear buffer

    sprintf(filePath, "%s%lld.txt", DATABASE_DIR, accountNumber);

    FILE *accountFile = fopen(filePath, "r");

    if (!accountFile) {
        printf("Account not found\n");
        return;
    }

    char line[200];
    char storedPIN[10];
    double balance = 0;

    while (fgets(line, sizeof(line), accountFile)){
        if (strncmp(line, "PIN:", 4) == 0){
            sscanf(line, "PIN: %s", storedPIN);
        }
        else if (strncmp(line, "Balance:", 8) == 0){
            sscanf(line, "Balance: %lf", &balance);
        }
    }

    fclose(accountFile);

    printf("Enter PIN: ");
    fgets(pin, sizeof(pin), stdin);
    pin[strcspn(pin, "\n")] = 0;

    if (strcmp(pin, storedPIN) != 0){
        printf("Incorrect PIN.\n");
        return;
    }

    char amountStr[100];
    int attempts = 3;
    bool validAmount = false;

    while (attempts > 0){
    printf("Enter amount to deposit: ");
    fgets(amountStr, sizeof(amountStr), stdin);
    amountStr[strcspn(amountStr, "\n")] = 0;
    
    char *endptr;
    amount = strtod(amountStr, &endptr);

    if (endptr == amountStr) {
        attempts--;
        printf("Invalid amount. Please enter a numeric value. Attempts left: %d\n", attempts);
        return;
    }

    while (*endptr) {
        if(!isspace(*endptr)) {
            attempts--;
            printf("Invalid amount format. Please enter numbers only. Attempts left: %d\n", attempts);
            goto retry_continue;
        }
        endptr++;
    }

    if (amount > 0 && amount  <= 50000){
        validAmount = true;
        break;
    }

    attempts--;
    printf("Invalid amount. Deposit must be greater than $0 and not exceed $50,000. Attempts left: %d\n", attempts);

    retry_continue:;
    }

    if (!validAmount){
        printf("Too many invalid attempts. Returning to menu...\n");
        return;
    }

    balance += amount;

    accountFile = fopen(filePath, "r");
    FILE *temp = fopen("database/temp.txt", "w");

    while (fgets(line, sizeof(line), accountFile)){
        if (strncmp(line, "Balance:", 8) == 0){
            fprintf(temp, "Balance: %.2f\n", balance);
        }
        else{
            fputs(line, temp);
        }
    }

    fclose(accountFile);
    fclose(temp);

    remove(filePath);
    rename("database/temp.txt", filePath);

    char logMsg[200];
    sprintf(logMsg, "Deposit: %.2f to %lld", amount, accountNumber);
    logTransaction(logMsg);

    printf("\nDeposit successful!\n");
    printf("New Balance: $%.2f\n", balance);
}

//WITHDRAWAL MONEY FUNCTION
void withdrawMoney(){
    long long accountNumber;
    char pin[10];
    double amount;
    char filePath[100];

    printf("\n--- Withdraw Money ---\n");

    FILE *index = fopen(INDEX_FILE, "r");
    if (!index){
        printf("No accounts found. \n");
        return;
    }

    printf("\n--- Existing Account ---\n");

    char lineAccounts[50];
    int count = 0;

    while (fgets(lineAccounts, sizeof(lineAccounts), index)){
        lineAccounts[strcspn(lineAccounts, "\n")] = 0;
        printf("%d. %s\n", ++count, lineAccounts);
    }

    fclose(index);

    if (count == 0){
        printf("No accounts available to withdraw.\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%lld", &accountNumber);
    getchar();

    sprintf(filePath, "%s%lld.txt", DATABASE_DIR, accountNumber);

    FILE *accountFile = fopen(filePath, "r");
    if (!accountFile){
        printf("Account not found\n");
        return;
    }

    char line[200];
    char storedPIN[10];
    double balance = 0;

    while (fgets(line, sizeof(line), accountFile)) {
        if (strncmp(line, "PIN:", 4) == 0){
            sscanf(line, "PIN: %s", storedPIN);
        } 
        else if (strncmp(line, "Balance:", 8) == 0){
            sscanf(line, "Balance: %lf", &balance);
        }
    }

    fclose(accountFile);

    printf("Enter PIN: ");
    fgets(pin, sizeof(pin), stdin);
    pin[strcspn(pin, "\n")] = 0;

    if (strcmp(pin, storedPIN) != 0){
        printf("Incorrect PIN.\n");
        return;
    }

    printf("Current Balance: $%.2f\n", balance);
    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    getchar();

    if (amount <= 0){
        printf("Invalid amount. Withdrawal must br greater than zero. \n");
        return;
    }

    if (amount > balance){
        printf("Insufficient balance. Current balance: $%.2f. Try to earn more money.\n", balance);
        return;
    }

    balance -= amount;

    accountFile = fopen(filePath, "r");
    FILE *temp = fopen("database/temp.txt", "w");

    while (fgets(line, sizeof(line), accountFile)){
        if (strncmp(line, "Balance:", 8) == 0){
            fprintf(temp, "Balance: %.2f\n", balance);
        }
        else{
            fputs(line, temp);
        }
    }

    fclose(accountFile);
    fclose(temp);

    remove(filePath);
    rename("database/temp.txt", filePath);

    char logMsg[200];
    sprintf(logMsg, "Withdrawal: %.2f from %lld", amount, accountNumber);
    logTransaction(logMsg);

    printf("\nWithdrawal successful!\n");
    printf("New Balance: $%.2f\n", balance);
}

//REMITTANCE MONEY FUNCTION

void remittanceMoney(){
    long long senderAccountNumber, receiverAccountNumber;
    char senderPIN[10];
    char filePathSender[100], filePathReceiver[100];
    char line[200];
    char storedPINSender[10], storedPINReceiver[10];
    char typeSender[15], typeReceiver[15];
    double balanceSender = 0.0, balanceReceiver = 0.0;
    double amount, fee = 0.0;

    printf("\n--- Remittance Money ---\n");

    FILE *index = fopen(INDEX_FILE, "r");
    if (!index) {
        printf("No accounts avaiable for remittance.\n");
        return;
    }

    printf("\n--- Existing Account ---\n");
    int count = 0;
    while (fgets(line, sizeof(line), index)) {
        line[strcspn(line, "\n")] = 0;
        printf("%d. %s\n", ++count, line);
    }
    fclose(index);

    if (count == 0) {
        printf("No accounts available for remittance.\n");
        return;
    }
    
    printf("Enter Sender Account Number: ");
    scanf("%lld", &senderAccountNumber);
    getchar();

    sprintf(filePathSender, "%s%lld.txt", DATABASE_DIR, senderAccountNumber);
    FILE *fileSender = fopen(filePathSender, "r");
    if (!fileSender) {
        printf("Sender account not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fileSender)){
        if (strncmp(line, "PIN:", 4) == 0){
            sscanf(line, "PIN: %s", storedPINSender);
        }
        else if (strncmp(line, "Account Type:", 13) == 0){
            sscanf(line, "Account Type: %s", typeSender);
        }
        else if (strncmp(line, "Balance:", 8) == 0){
            sscanf(line, "Balance: %lf", &balanceSender);
        }
    }
    fclose(fileSender);

    //Normalize sender type
    toLowerCase(typeSender);
    int len = strlen(typeSender);
    while (len > 0 && isspace(typeSender[len-1])){
        typeSender[len - 1] = '\0';
        len--;
    }

    printf("Enter Sender 4-digit PIN: ");
    fgets(senderPIN, sizeof(senderPIN), stdin);
    senderPIN[strcspn(senderPIN, "\n")] = 0;

    if (strcmp(senderPIN, storedPINSender) != 0){
        printf("Incorrect PIN. Authentication failed.\n");
        return;
    }

    printf("Enter Receiver Account Number: ");
    scanf("%lld", &receiverAccountNumber);
    getchar();

    if (receiverAccountNumber == senderAccountNumber){
        printf("SENDER and RECEIVER accounts must be different.\n");
        return;
    }

    sprintf(filePathReceiver, "%s%lld.txt", DATABASE_DIR, receiverAccountNumber);
    FILE *fileReceiver = fopen(filePathReceiver, "r");
    if (!fileReceiver){
        printf("Receiver account not found.\n");
        return;
    }

    while (fgets(line, sizeof(line), fileReceiver)){
        if (strncmp(line, "PIN:", 4) == 0){
            sscanf(line, "PIN: %s", storedPINReceiver);
        }
        else if (strncmp(line, "Account Type:", 13) == 0){
            sscanf(line, "Account Type: %s", typeReceiver);
        }
        else if (strncmp(line, "Balance:", 8) == 0){
            sscanf(line, "Balance: %lf", &balanceReceiver);
        }
    }
    fclose(fileReceiver);

    //Normalize receiver type
    toLowerCase(typeReceiver);
    len = strlen(typeReceiver);
    while (len > 0 && isspace(typeReceiver[len - 1])){
        typeReceiver[len - 1] = '\0';
        len--;
    }

    printf("Current Sender Balance: $%.2f\n", balanceSender);

    printf("Enter amount to remit: ");
    scanf("%lf", &amount);
    getchar();

    if (amount <= 0){
        printf("Invalid amount. Remittance must be greater than zero.\n");
        return;
    }

    if (strcmp(typeSender, "savings") == 0 && strcmp(typeReceiver, "current") == 0){
        fee = amount * 0.02;
    }
    else if (strcmp(typeSender, "current") == 0 && strcmp(typeReceiver, "savings") == 0){
        fee = amount * 0.03;
    }
    else {
        fee = 0.0;
    }

    double totalDeduction = amount + fee;

    if (totalDeduction > balanceSender){
        printf("Insufficient balance in sender account. You need $%.2f (amount + fee).\n", totalDeduction);
        return;
    }

    balanceSender -= totalDeduction;
    balanceReceiver += amount;

    fileSender = fopen(filePathSender, "r");
    FILE *tempSender = fopen("database/temp.txt", "w");
    while (fgets(line, sizeof(line), fileSender)){
        if (strncmp(line, "Balance:", 8) == 0){
            fprintf(tempSender, "Balance: %.2f\n", balanceSender);
        }
        else{
            fputs(line, tempSender);
        }
    }
    fclose(fileSender);
    fclose(tempSender);
    remove(filePathSender);
    rename("database/temp.txt", filePathSender);

    fileReceiver = fopen(filePathReceiver, "r");
    FILE *tempReceiver = fopen("database/temp.txt", "w");
    while (fgets(line, sizeof(line), fileReceiver)){
        if (strncmp(line, "Balance:", 8) == 0){
            fprintf(tempReceiver, "Balance: %.2f\n", balanceReceiver);
        }
        else{
            fputs(line, tempReceiver);
        }
    }
    fclose(fileReceiver);
    fclose(tempReceiver);
    remove(filePathReceiver);
    rename("database/temp.txt", filePathReceiver);

    char logMsg[300];
    sprintf(logMsg, "Remittance: %.2f from %lld to %lld (fee %.2f)", amount, senderAccountNumber, receiverAccountNumber, fee);
    logTransaction(logMsg);

    printf("\nRemittance successful!\n");
    printf("Sender New Balance: $%.2f\n", balanceSender);
    printf("Receiver New Balance: $%.2f\n", balanceReceiver);
}