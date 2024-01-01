//
// Created by Nyein on 12/18/2023.
//

#include <stdio.h>

#define STUDENTS 3
#define SUBJECTS 6
#define PASS_MARK 40
#define DISTINCTION_MARK 80
#define MAX_MARK 100

struct Student {
    char name[50];
    int scores[SUBJECTS];
    char results[SUBJECTS];
    char overallResult;
    int distinctionCount;
};

int main() {
    struct Student students[STUDENTS];

    for (int s = 0; s < STUDENTS; ++s) {
        // Input student's name
        printf("Enter student %d's name: ", s + 1);
        fgets(students[s].name, sizeof(students[s].name), stdin);

        // Input exam results for each subject
        for (int i = 0; i < SUBJECTS; ++i) {
            // Input validation loop for marks within the valid range (0 to 100)
            do {
                printf("Enter result for subject %d (0-100): ", i + 1);
                scanf("%d%*c", &students[s].scores[i]);  // Use %*c to consume the newline character

                // Validate input (assuming scores are non-negative)
                if (students[s].scores[i] < 0 || students[s].scores[i] > MAX_MARK) {
                    printf("Invalid input. Please enter a score between 0 and 100.\n");
                }
            } while (students[s].scores[i] < 0 || students[s].scores[i] > MAX_MARK);

            // Determine pass/fail result for each subject
            students[s].results[i] = (students[s].scores[i] >= PASS_MARK) ? 'P' : 'F';
        }

        // Determine overall result
        students[s].overallResult = 'P'; // Assume overall pass unless a fail is found
        students[s].distinctionCount = 0;

        for (int i = 0; i < SUBJECTS; ++i) {
            if (students[s].results[i] == 'F') {
                students[s].overallResult = 'F';
                break; // Exit the loop as soon as a fail is found
            }

            // Count distinctions
            if (students[s].scores[i] > DISTINCTION_MARK) {
                students[s].distinctionCount++;
            }
        }
    }

    // Display results for each student
    for (int s = 0; s < STUDENTS; ++s) {
        printf("\nStudent %s ", students[s].name);

        if (students[s].overallResult == 'P') {
            printf("Pass the Exam");

            // Display distinction information if applicable
            if (students[s].distinctionCount > 0) {
                printf(" and achieved Distinction in %d subject(s)", students[s].distinctionCount);
            }
        } else {
            printf("Fail the Exam");
        }

        printf(".\n");
    }

    return 0;
}