//
// Created by Nyein on 1/7/2024.
//
#include <stdio.h>
#include <string.h>

// Maximum length for username and password
#define MAX_LENGTH 20
#define MAX_USERS 100
// Structure to store user information
struct User {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    int isLoggedIn;
};

// Function to compare two strings manually
int compareStrings(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0; // Not equal
        }
        str1++;
        str2++;
    }

    // Check if both strings have reached the end simultaneously
    return (*str1 == '\0' || *str1 == '\n') && (*str2 == '\0' || *str2 == '\n');
}

// Custom function to convert a string to an integer
int stringToInt(const char *str) {
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

// Function to check if a username already exists
int usernameExists(struct User users[], int userCount, const char *username) {
    for (int i = 0; i < userCount; i++) {
        if (compareStrings(username, users[i].username)) {
            return 1; // Username already exists
        }
    }
    return 0; // Username does not exist
}

// Function to check if a password meets the specified criteria
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
            // Assuming special characters are anything other than alphanumeric
            hasSpecial = 1;
        }
        length++;
    }

    // Check if all criteria are met
    return (length >= 6 && hasLower && hasUpper && hasDigit && hasSpecial);
}

// Function to register a new user
void registerUser(struct User users[], int *userCount) {
    if (*userCount < MAX_USERS) {
        char username[MAX_LENGTH];
        char password[MAX_LENGTH];

        // Read the entire line of input for username
        printf("Enter username: ");
        fgets(username, sizeof(username), stdin);

        // Remove newline character from username
        int len = 0;
        while (username[len] != '\0' && username[len] != '\n') {
            len++;
        }
        if (len > 0 && username[len - 1] == '\n') {
            username[len - 1] = '\0';
        }

        // Check if the username already exists
        if (usernameExists(users, *userCount, username)) {
            printf("Username already exists. Registration failed.\n");
            return;
        }

        // Read the entire line of input for password
        do {
            printf("Enter password: ");
            fgets(password, sizeof(password), stdin);

            // Remove newline character from password
            len = 0;
            while (password[len] != '\0' && password[len] != '\n') {
                len++;
            }
            if (len > 0 && password[len - 1] == '\n') {
                password[len - 1] = '\0';
            }

            // Check if the password is valid
            if (!isValidPassword(password)) {
                printf("Invalid password. Please ensure it has at least 6 characters, one lowercase letter, one uppercase letter, one digit, and one special character.\n");
            }
        } while (!isValidPassword(password));

        // New user is initially not logged in
        users[*userCount].isLoggedIn = 0;

        // Copy username and password to the user struct
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

// Function to login
void login(struct User users[], int userCount) {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);

    // Remove newline character from username
    size_t len = strlen(username);
    if (len > 0 && username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);

    // Remove newline character from password
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

// Function to logout
void logout(struct User users[], int userCount) {
    char username[MAX_LENGTH];

    printf("Enter username to logout: ");
    fgets(username, sizeof(username), stdin);

    // Remove newline character from username
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

int main() {
    struct User users[MAX_USERS];
    int userCount = 0;
    int choice;
    char input[10];

    do {
        printf("\n1. Register\n2. Login\n3. Logout\n4. Exit\n");
        printf("Enter your choice: ");

        // Read the entire line of input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }

        // Convert input to integer
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
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

    } while (choice != 4);

    return 0;
}
