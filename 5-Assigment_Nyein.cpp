//
// Created by Nyein on 1/15/2024.
//
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 20
#define MAX_USERS 100

struct User {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int isLoggedIn;
    int points;
};

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

void registerUser(struct User users[], int *userCount) {
    if (*userCount < MAX_USERS) {
        char username[MAX_LENGTH];
        char password[MAX_LENGTH];

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

        users[*userCount].isLoggedIn = 0;
        users[*userCount].points = 500;

        int i = 0;
        while (username[i] != '\0') {
            users[*userCount].username[i] = username[i];
            i++;
        }
        users[*userCount].username[i] = '\0';

        i = 0;
        while (password[i] != '\0') {
            users[*userCount].password[i] = password[i];
            i++;
        }
        users[*userCount].password[i] = '\0';

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
        if (users[senderIndex].points >= pointsToTransfer && pointsToTransfer > 0) {
            users[senderIndex].points -= pointsToTransfer;
            users[receiverIndex].points += pointsToTransfer;
            printf("Points transfer successful!\n");
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

int main() {
    struct User users[MAX_USERS];
    int userCount = 0;
    int choice;
    char input[10];

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
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 6);

    return 0;
}
