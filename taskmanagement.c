#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct date {
    int jour;
    int mois;
    int annee;
} date;

typedef struct task {
    int id;
    char title[50];
    char assigne[50];
    int priority;
    struct date deadline;
    char status[50];
    struct task *next;
} task;

typedef struct head {
    struct task *data;
} head;

void afficherPresentation(void) {
    printf("\n");
    printf("========================================\n");
    printf("       USTHB - COMPUTER SCIENCE\n");
    printf("          2ND YEAR - ASD\n");
    printf("========================================\n\n");

    printf("     PROJECT TASK MANAGEMENT\n");
    printf("     Project 13 | Groupe 17\n\n");
    printf("     Sengui | Mohamed Ibrahim\n\n");

    printf(" Press ENTER to continue...");
    getchar();
}

// ==========================================
// HELP / USER GUIDE FUNCTION
// ==========================================
void disp_help(void) {
    printf("\n====================================================================\n");
    printf("                    USER GUIDE - HELP SECTION                       \n");
    printf("====================================================================\n");
    printf("This application manages project tasks using a Singly Linked List.\n\n");

    printf("--- DATA FORMATS ---\n");
    printf(" * Task ID   : Must be a unique integer (e.g., 101, 102).\n");
    printf(" * Priority  : 1 = Low, 2 = Medium, 3 = High.\n");
    printf(" * Deadline  : Entered as DD MM YYYY separated by spaces (e.g., 15 11 2026).\n");
    printf(" * Status    : \"To Do\", \"In Progress\", or \"Completed\".\n\n");

    printf("--- AVAILABLE OPERATIONS ---\n");
    printf("  1. Add a task        : Create a new task with a unique ID.\n");
    printf("  2. Delete a task     : Remove an existing task permanently using its ID.\n");
    printf("  3. Modify a task     : Edit title, assignee, priority, or deadline by ID.\n");
    printf("  4. Update status     : Change progress between To Do, In Progress, Completed.\n");
    printf("  5. Search by person  : List all tasks assigned to a specific team member.\n");
    printf("  6. Display all tasks : View the complete list of tasks and their details.\n");
    printf("  7. Overdue tasks     : View uncompleted tasks whose deadline has passed.\n");
    printf("  8. Sort by priority  : Order tasks from highest priority (3) to lowest (1).\n");
    printf("  9. Sort by deadline  : Order tasks chronologically (earliest to latest).\n");
    printf(" 10. Count completed   : Show total number and percentage of completed tasks.\n");
    printf(" 11. Statistics        : Summary of total, in-progress, completed, & overdue.\n");
    printf(" 12. Exit              : Free all allocated memory and close the program.\n");
    printf("====================================================================\n");
    printf("Press ENTER to return to the menu...");
    getchar();
}

// ==========================================
// MENU DISPLAY FUNCTION
// ==========================================
void afficherMenu(void) {
    printf("\n================ MENU ================\n");
    printf(" 0. Help / User Guide\n");
    printf(" 1. Add a task\n");
    printf(" 2. Delete a task\n");
    printf(" 3. Modify a task\n");
    printf(" 4. Update a task's status\n");
    printf(" 5. Search by assigned person\n");
    printf(" 6. Display all tasks\n");
    printf(" 7. Display overdue tasks\n");
    printf(" 8. Sort by priority\n");
    printf(" 9. Sort by deadline\n");
    printf("10. Count completed tasks\n");
    printf("11. Display statistics\n");
    printf("12. Exit\n");
    printf("======================================\n");
    printf("Your choice: ");
}

// Helper function to remove the newline character from fgets()
void readString(char *buffer, int size) {
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

// Helper function: Search for a task by ID (used in Add and Modify)
task* findTaskById(head *list, int id) {
    task *curr = list->data;
    while (curr != NULL) {
        if (curr->id == id) {
            return curr; // Found
        }
        curr = curr->next;
    }
    return NULL; // Not found
}

// ==========================================
// 1. ADD A TASK
// ==========================================
void addTask(head *list) {
    int id;
    printf("\n--- ADD A NEW TASK ---\n");
    printf("Enter Task ID: ");
    scanf("%d", &id);
    getchar(); // Clear newline from buffer

    // Verify that the identifier is unique
    if (findTaskById(list, id) != NULL) {
        printf("Error: A task with ID %d already exists!\n", id);
        return;
    }

    // Allocate memory for the new task
    task *newTask = (task*)malloc(sizeof(task));
    if (newTask == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    newTask->id = id;

    printf("Enter Title: ");
    readString(newTask->title, sizeof(newTask->title));

    printf("Enter Assigned Person: ");
    readString(newTask->assigne, sizeof(newTask->assigne));

    printf("Enter Priority (1 = Low, 2 = Medium, 3 = High): ");
    scanf("%d", &newTask->priority);
    getchar(); // Clear newline

    printf("Enter Deadline (DD MM YYYY): ");
    scanf("%d %d %d", &newTask->deadline.jour, &newTask->deadline.mois, &newTask->deadline.annee);
    getchar(); // Clear newline

    int statusChoice;
    printf("Select Initial Status (1: To Do, 2: In Progress, 3: Completed): ");
    scanf("%d", &statusChoice);
    getchar();

    if (statusChoice == 2) {
        strcpy(newTask->status, "In Progress");
    } else if (statusChoice == 3) {
        strcpy(newTask->status, "Completed");
    } else {
        strcpy(newTask->status, "To Do");
    }

    newTask->next = NULL;

    // Insert at the end of the singly linked list
    if (list->data == NULL) {
        list->data = newTask;
    } else {
        task *temp = list->data;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newTask;
    }

    printf("Task (ID: %d) added successfully!\n", id);
}

// ==========================================
// 2. DELETE A TASK
// ==========================================
void deleteTask(head *list) {
    if (list->data == NULL) {
        printf("\nThe task list is empty. Nothing to delete.\n");
        return;
    }

    int id;
    printf("\n--- DELETE A TASK ---\n");
    printf("Enter Task ID to delete: ");
    scanf("%d", &id);
    getchar();

    task *curr = list->data;
    task *prev = NULL;

    // Traverse the list to find the task
    while (curr != NULL && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }

    // If task was not found
    if (curr == NULL) {
        printf("Error: Task with ID %d not found!\n", id);
        return;
    }

    // If the task to delete is the first node (head)
    if (prev == NULL) {
        list->data = curr->next;
    } else {
        prev->next = curr->next;
    }

    free(curr);
    printf("Task (ID: %d) deleted successfully!\n", id);
}

// ==========================================
// 3. MODIFY A TASK
// ==========================================
void modifyTask(head *list) {
    if (list->data == NULL) {
        printf("\nThe task list is empty.\n");
        return;
    }

    int id;
    printf("\n--- MODIFY A TASK ---\n");
    printf("Enter Task ID to modify: ");
    scanf("%d", &id);
    getchar();

    task *target = findTaskById(list, id);
    if (target == NULL) {
        printf("Error: Task with ID %d does not exist!\n", id);
        return;
    }

    int choice;
    printf("\nModifying Task #%d (%s)\n", target->id, target->title);
    printf("1. Modify Title\n");
    printf("2. Modify Assigned Person\n");
    printf("3. Modify Priority Level\n");
    printf("4. Modify Deadline\n");
    printf("5. Modify All of the Above\n");
    printf("Your choice: ");
    scanf("%d", &choice);
    getchar();

    switch (choice) {
        case 1:
            printf("Enter new Title: ");
            readString(target->title, sizeof(target->title));
            break;
        case 2:
            printf("Enter new Assigned Person: ");
            readString(target->assigne, sizeof(target->assigne));
            break;
        case 3:
            printf("Enter new Priority (1 = Low, 2 = Medium, 3 = High): ");
            scanf("%d", &target->priority);
            getchar();
            break;
        case 4:
            printf("Enter new Deadline (DD MM YYYY): ");
            scanf("%d %d %d", &target->deadline.jour, &target->deadline.mois, &target->deadline.annee);
            getchar();
            break;
        case 5:
            printf("Enter new Title: ");
            readString(target->title, sizeof(target->title));
            printf("Enter new Assigned Person: ");
            readString(target->assigne, sizeof(target->assigne));
            printf("Enter new Priority (1 = Low, 2 = Medium, 3 = High): ");
            scanf("%d", &target->priority);
            getchar();
            printf("Enter new Deadline (DD MM YYYY): ");
            scanf("%d %d %d", &target->deadline.jour, &target->deadline.mois, &target->deadline.annee);
            getchar();
            break;
        default:
            printf("Invalid choice. No changes made.\n");
            return;
    }

    printf("Task (ID: %d) updated successfully!\n", id);
}

// ==========================================
// 6. DISPLAY ALL TASKS (For testing your part)
// ==========================================
void displayAllTasks(head *list) {
    if (list->data == NULL) {
        printf("\nThe task list is currently empty.\n");
        return;
    }
    printf("\n--- ALL TASKS ---\n");
    task *curr = list->data;
    while (curr != NULL) {
        printf("[ID: %d] Title: %s | Assigned: %s | Priority: %d | Deadline: %02d/%02d/%04d | Status: %s\n",
               curr->id, curr->title, curr->assigne, curr->priority,
               curr->deadline.jour, curr->deadline.mois, curr->deadline.annee, curr->status);
        curr = curr->next;
    }
}

// ==========================================
// MAIN FUNCTION
// ==========================================
int main() {
    head myList;
    myList.data = NULL; // Initialize the linked list as empty

    // Show presentation and help screen at startup
    afficherPresentation();
    disp_help();

    int choice;
    do {
        afficherMenu();

        // Check if the user entered a valid integer
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input from buffer
            choice = -1; // Force default case
        } else {
            getchar(); // Consume the newline left by scanf
        }

        switch (choice) {
            case 0:
                disp_help();
                break;
            case 1:
                addTask(&myList);
                break;
            case 2:
                deleteTask(&myList);
                break;
            case 3:
                modifyTask(&myList);
                break;
            case 4:
                // Teammate's function: updateTaskStatus(&myList);
                printf("\n[Info] Feature 4 is being implemented by the team.\n");
                break;
            case 5:
                // Teammate's function: searchByPerson(&myList);
                printf("\n[Info] Feature 5 is being implemented by the team.\n");
                break;
            case 6:
                displayAllTasks(&myList);
                break;
            case 7:
                // Teammate's function: displayOverdueTasks(&myList);
                printf("\n[Info] Feature 7 is being implemented by the team.\n");
                break;
            case 8:
                // Teammate's function: sortByPriority(&myList);
                printf("\n[Info] Feature 8 is being implemented by the team.\n");
                break;
            case 9:
                // Teammate's function: sortByDeadline(&myList);
                printf("\n[Info] Feature 9 is being implemented by the team.\n");
                break;
            case 10:
                // Teammate's function: countCompletedTasks(&myList);
                printf("\n[Info] Feature 10 is being implemented by the team.\n");
                break;
            case 11:
                // Teammate's function: displayStatistics(&myList);
                printf("\n[Info] Feature 11 is being implemented by the team.\n");
                break;
            case 12:
                // Teammate's function: freeList(&myList);
                printf("\nExiting the application... Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please enter a number between 0 and 12.\n");
        }
    } while (choice != 12);
    return 0;
}