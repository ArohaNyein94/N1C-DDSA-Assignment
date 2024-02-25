#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LENGTH 20
#define MAX_USERS 100
#define MAX_HISTORY 100

enum AccountStatus {
    ACTIVE,
    BANNED,
    SUSPENDED
};

struct User {
    int userId;
    char username[MAX_LENGTH];
    char emailAddress[MAX_LENGTH];
    char password[MAX_LENGTH];
    char phoneNumber[MAX_LENGTH];
    char address[MAX_LENGTH];
    char postalCode[MAX_LENGTH];
    int isLoggedIn;
    int points;
    int historyCount;
    char transactionHistory[MAX_HISTORY][MAX_LENGTH + 50];
    enum AccountStatus status; // Add account status field
};

// Function prototypes
void loadUsersFromFile(struct User users[], int *userCount);
void saveUsersToFile(struct User users[], int userCount);
int stringToInt(const char *str);
int compareStrings(const char *str1, const char *str2);
int isValidPassword(const char *password);
int isValidEmail(const char *email);
int isValidPhoneNumber(const char *phoneNumber);
void registerUser(struct User users[], int *userCount);
void login(struct User users[], int userCount);
void logout(struct User users[], int userCount);
void transferPoints(struct User users[], int userCount);
void checkPoints(struct User users[], int userCount);
void stringCopy(char *dest, const char *src);
int isPhoneNumberUnique(const char *phoneNumber, struct User users[], int userCount);
int checkAccountStatus(struct User user);
int usernameExists(struct User users[], int userCount, const char *username);

// Function to get current system date and time
void getCurrentDateTime(char *dateTime) {
    time_t now;
    time(&now);
    strftime(dateTime, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
}

int compareStrings(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' || *str1 == '\n') && (*str2 == '\0' || *str2 == '\n');
}

int stringToInt(const char *str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

void stringCopy(char *dest, const char *src) {
    while (*src != '\0') {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
}
int usernameExists(struct User users[], int userCount, const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(username, users[i].username)) {
            return 1;
        }
    }
    return 0;
}

int isValidPassword(const char *password) {
    int length = 0, hasLower = 0, hasUpper = 0, hasDigit = 0, hasSpecial = 0;

    while (password[length] != '\0') {
        if (password[length] >= 'a' && password[length] <= 'z') {
            hasLower = 1;
        } else if (password[length] >= 'A' && password[length] <= 'Z') {
            hasUpper = 1;
        } else if (password[length] >= '0' && password[length] <= '9') {
            hasDigit = 1;
        } else {
            hasSpecial = 1;
        }
        length++;
    }

    return (length >= 6 && hasLower && hasUpper && hasDigit && hasSpecial);
}

int isValidEmail(const char *email) {
    const char *validDomains[] = {"@gmail.com", "@apple.com", "@n1c.com", "@aroha.com", "@yahoo.com"};
    int numDomains = sizeof(validDomains) / sizeof(validDomains[0]);

    int emailLength = strlen(email);
    int validDomain = 0;

    for (int i = 0; i < numDomains; ++i) {
        const char *domain = validDomains[i];
        int domainLength = strlen(domain);

        if (emailLength >= domainLength && compareStrings(email + emailLength - domainLength, domain)) {
            validDomain = 1;
            break;
        }
    }

    return validDomain;
}

int isValidPhoneNumber(const char *phoneNumber)  {
    int length = 0;

    // Phone number must start with '09' and have at least 9 digits
    if (phoneNumber[0] == '0' && phoneNumber[1] == '9') {
        while (phoneNumber[length] != '\0') {
            if (phoneNumber[length] < '0' || phoneNumber[length] > '9') {
                return 0; // Non-digit character found
            }
            length++;
        }
        return length >= 9 ? 1 : 0;
    }
    return 0;
}

int isPhoneNumberUnique(const char *phoneNumber, struct User users[], int userCount) {
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(phoneNumber, users[i].phoneNumber)) {
            return 0; // Phone number already exists
        }
    }
    return 1; // Phone number is unique
}

int checkAccountStatus(struct User user) {
    return user.status; // Return account status
}

void registerUser(struct User users[], int *userCount) {
    if (*userCount < MAX_USERS) {
        int userId = *userCount + 1; // Assign user ID

        char username[MAX_LENGTH];
        char password[MAX_LENGTH];
        char phoneNumber[MAX_LENGTH];
        char address[MAX_LENGTH];
        char postalCode[MAX_LENGTH];
        char emailAddress[MAX_LENGTH];

        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);

        size_t len = strlen(username);
        if (len > 0 && username[len - 1] == '\n') {
            username[len - 1] = '\0';
        }

        if (usernameExists(users, *userCount, username)) {
            printf("Username already exists. Registration failed.\n");
            return;
        }

        do {
            printf("Enter email address: ");
            fgets(emailAddress, sizeof(emailAddress), stdin);

            len = strlen(emailAddress);
            if (len > 0 && emailAddress[len - 1] == '\n') {
                emailAddress[len - 1] = '\0';
            }

            if (!isValidEmail(emailAddress)) {
                printf("Invalid email address. Please use one of the following domains: @gmail.com, @apple.com, @n1c.com, @aroha.com, @yahoo.com.\n");
            }
        } while (!isValidEmail(emailAddress));

        do {
            printf("Enter password: ");
            fgets(password, sizeof(password), stdin);

            len = strlen(password);
            if (len > 0 && password[len - 1] == '\n') {
                password[len - 1] = '\0';
            }

            if (!isValidPassword(password)) {
                printf("Invalid password. Please ensure it has at least 6 characters, one lowercase letter, one uppercase letter, one digit, and one special character.\n");
            }
        } while (!isValidPassword(password));

        do {
            printf("Enter phone number: ");
            fgets(phoneNumber, sizeof(phoneNumber), stdin);

            len = strlen(phoneNumber);
            if (len > 0 && phoneNumber[len - 1] == '\n') {
                phoneNumber[len - 1] = '\0';
            }

            if (!isValidPhoneNumber(phoneNumber)) {
                printf("Invalid phone number. Phone number must start with '09' and have at least 9 digits.\n");
            } else if (!isPhoneNumberUnique(phoneNumber, users, *userCount)) {
                printf("Phone number already exists. Please enter a unique phone number.\n");
            }
        } while (!isValidPhoneNumber(phoneNumber) || !isPhoneNumberUnique(phoneNumber, users, *userCount));

        printf("Enter address: ");
        fgets(address, sizeof(address), stdin);

        len = strlen(address);
        if (len > 0 && address[len - 1] == '\n') {
            address[len - 1] = '\0';
        }

        printf("Enter postal code: ");
        fgets(postalCode, sizeof(postalCode), stdin);

        len = strlen(postalCode);
        if (len > 0 && postalCode[len - 1] == '\n') {
            postalCode[len - 1] = '\0';
        }

        users[*userCount].userId = userId;
        users[*userCount].isLoggedIn = 0;
        users[*userCount].points = 500;
        users[*userCount].historyCount = 0;
        users[*userCount].status = ACTIVE; // Set default status to ACTIVE

        stringCopy(users[*userCount].username, username);
        stringCopy(users[*userCount].emailAddress, emailAddress);
        stringCopy(users[*userCount].password, password);
        stringCopy(users[*userCount].phoneNumber, phoneNumber);
        stringCopy(users[*userCount].address, address);
        stringCopy(users[*userCount].postalCode, postalCode);

        (*userCount)++;
        printf("Registration successful!\n");
    } else {
        printf("Maximum number of users reached.\n");
    }
}

void login(struct User users[], int userCount) {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);

    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);

    len = strlen(password);
    if (len > 0 && password[len - 1] == '\n') {
        password[len - 1] = '\0';
    }

    for (int i = 0; i < userCount; i++) {
        if (compareStrings(username, users[i].username) && compareStrings(password, users[i].password)) {
            if (checkAccountStatus(users[i]) != ACTIVE) {
                printf("Your account is banned or suspended. Login failed.\n");
                return;
            }
            users[i].isLoggedIn = 1;
            printf("Login successful!\n");
            return;
        }
    }

    printf("Invalid username or password. Login failed.\n");
}

void logout(struct User users[], int userCount) {
    char username[MAX_LENGTH];

    printf("Enter username to logout: ");
    fgets(username, sizeof(username), stdin);

    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    for (int i = 0; i < userCount; i++) {
        if (compareStrings(username, users[i].username)) {
            users[i].isLoggedIn = 0;
            printf("Logout successful!\n");
            return;
        }
    }

    printf("User not found. Logout failed.\n");
}

void transferPoints(struct User users[], int userCount) {
    char senderUsername[MAX_LENGTH];
    char receiverUsername[MAX_LENGTH];
    int pointsToTransfer;

    printf("Enter sender's username: ");
    fgets(senderUsername, sizeof(senderUsername), stdin);

    size_t len = strlen(senderUsername);
    if (len > 0 && senderUsername[len - 1] == '\n') {
        senderUsername[len - 1] = '\0';
    }

    printf("Enter receiver's username: ");
    fgets(receiverUsername, sizeof(receiverUsername), stdin);

    len = strlen(receiverUsername);
    if (len > 0 && receiverUsername[len - 1] == '\n') {
        receiverUsername[len - 1] = '\0';
    }

    printf("Enter points to transfer: ");
    scanf("%d", &pointsToTransfer);

    getchar(); // Consume the newline character left in the input buffer by scanf

    int senderIndex = -1, receiverIndex = -1;
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(senderUsername, users[i].username)) {
            senderIndex = i;
        }
        if (compareStrings(receiverUsername, users[i].username)) {
            receiverIndex = i;
        }
    }

    if (senderIndex != -1 && receiverIndex != -1) {
        if (checkAccountStatus(users[senderIndex]) != ACTIVE || checkAccountStatus(users[receiverIndex]) != ACTIVE) {
            printf("One of the accounts involved in the transfer is banned or suspended. Transfer failed.\n");
            return;
        }
        if (users[senderIndex].points >= pointsToTransfer && pointsToTransfer > 0) {
            users[senderIndex].points -= pointsToTransfer;
            users[receiverIndex].points += pointsToTransfer;
            printf("Points transfer successful!\n");

            // Add transaction history for sender
            char dateTime[20];
            getCurrentDateTime(dateTime);
            snprintf(users[senderIndex].transactionHistory[users[senderIndex].historyCount], MAX_LENGTH + 50, "%s sent %d points to %s on %s", users[senderIndex].username, pointsToTransfer, receiverUsername, dateTime);
            users[senderIndex].historyCount++;

            // Add transaction history for receiver
            snprintf(users[receiverIndex].transactionHistory[users[receiverIndex].historyCount], MAX_LENGTH + 50, "%s received %d points from %s on %s", users[receiverIndex].username, pointsToTransfer, senderUsername, dateTime);
            users[receiverIndex].historyCount++;
        } else {
            printf("Invalid points or insufficient balance for transfer.\n");
        }
    } else {
        printf("Sender or receiver not found. Points transfer failed.\n");
    }
}

void checkPoints(struct User users[], int userCount) {
    char username[MAX_LENGTH];

    printf("Enter username to check points: ");
    fgets(username, sizeof(username), stdin);

    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    for (int i = 0; i < userCount; i++) {
        if (compareStrings(username, users[i].username)) {
            printf("Points for user %s: %d\n", users[i].username, users[i].points);
            return;
        }
    }

    printf("User not found. Unable to check points.\n");
}

void loadUsersFromFile(struct User users[], int *userCount) {
    FILE *file = fopen("database.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    *userCount = 0; // Reset user count before loading users

    // Read data until the end of file
    while (*userCount < MAX_USERS && fscanf(file, "%d", &users[*userCount].userId) == 1) {
        fscanf(file, " %s", users[*userCount].username);
        fscanf(file, " %s", users[*userCount].emailAddress);
        fscanf(file, " %s", users[*userCount].password);
        fscanf(file, " %s", users[*userCount].phoneNumber);
        fscanf(file, " %s", users[*userCount].address);
        fscanf(file, " %s", users[*userCount].postalCode);
        fscanf(file, "%d", &users[*userCount].points);
        fscanf(file, "%d", &users[*userCount].historyCount);

        // Read transaction history
        for (int i = 0; i < users[*userCount].historyCount; ++i) {
            fscanf(file, " %[^\n]s", users[*userCount].transactionHistory[i]);
        }

        // Read account status
        int status;
        fscanf(file, "%d", &status);
        users[*userCount].status = (enum AccountStatus)status;

        users[*userCount].isLoggedIn = 0;

        (*userCount)++;
    }

    fclose(file);

    // Display the number of registered users
    printf("Number of registered users read from file: %d\n", *userCount);
}

void saveUsersToFile(struct User users[], int userCount) {
    FILE *file = fopen("database.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        fprintf(file, "%d\n", users[i].userId);
        fprintf(file, "%s\n", users[i].username);
        fprintf(file, "%s\n", users[i].emailAddress);
        fprintf(file, "%s\n", users[i].password);
        fprintf(file, "%s\n", users[i].phoneNumber);
        fprintf(file, "%s\n", users[i].address);
        fprintf(file, "%s\n", users[i].postalCode);
        fprintf(file, "%d\n", users[i].points);
        fprintf(file, "%d\n", users[i].historyCount);

        // Save transaction history
        for (int j = 0; j < users[i].historyCount; ++j) {
            fprintf(file, "%s\n", users[i].transactionHistory[j]);
        }

        // Save account status
        fprintf(file, "%d\n", users[i].status);
    }

    fclose(file);
}

int main() {
    struct User users[MAX_USERS];
    int userCount = 0;
    int choice;
    char input[10];

    loadUsersFromFile(users, &userCount); // Load users from file if necessary

    do {
        printf("\n1. Register\n2. Login\n3. Logout\n4. Transfer Points\n5. Check Points\n6. Exit\n");
        printf("Enter your choice: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }

        choice = stringToInt(input);

        switch (choice) {
            case 1:
                registerUser(users, &userCount);
                break;
            case 2:
                login(users, userCount);
                break;
            case 3:
                logout(users, userCount);
                break;
            case 4:
                transferPoints(users, userCount);
                break;
            case 5:
                checkPoints(users, userCount);
                break;
            case 6:
                saveUsersToFile(users, userCount); // Save users before exiting
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
