#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int rollno;
    char name[50];
    float marks;
};

char currentUserRole[10];  
char currentUser[50];      

int loginSystem();
void homeMenu();      // Start menu (Login / Sign Up)
void mainmenu();   
void adminMenu();
void userMenu();
void guestMenu();

void signUp();
void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    homeMenu();       // Start with home menu instead of direct login
    return 0;
}

void homeMenu() {
    int choice;
    do {
        printf("\n=== Welcome to Student Record Management System ===\n");
        printf("1. Login (Sign In)\n");
        printf("2. Sign Up (Create New Account)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (loginSystem()) {
                    mainmenu();
                } else {
                    printf("Login failed. Try again.\n");
                }
                break;
            case 2:
                signUp();
                break;
            case 3:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

int loginSystem() {
    char username[20];
    char password[20];
    char fileuser[20];
    char filepass[20];
    char fileRole[10];

    printf("\n=== Login System (Sign In) ===\n");
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Error: %s not found. Please sign up first.\n", CREDENTIAL_FILE);
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fileuser, filepass, fileRole) == 3) {
        if (strcmp(username, fileuser) == 0 && strcmp(password, filepass) == 0) {
            strcpy(currentUserRole, fileRole);
            strcpy(currentUser, fileuser);
            fclose(fp);
            printf("Login successful. Welcome, %s (%s)\n", currentUser, currentUserRole);
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid username or password.\n");
    return 0;
}

void signUp() {
    char username[20];
    char password[20];
    char role[10];
    char fileuser[20], filepass[20], fileRole[10];

    printf("\n=== Sign Up (Create New Account) ===\n");
    printf("Enter new username: ");
    scanf("%s", username);
    printf("Enter new password: ");
    scanf("%s", password);

    int roleChoice;
    printf("Select Role:\n");
    printf("1. ADMIN\n");
    printf("2. STAFF\n");
    printf("3. GUEST\n");
    printf("Enter your choice: ");
    scanf("%d", &roleChoice);

    switch (roleChoice) {
        case 1: strcpy(role, "ADMIN"); break;
        case 2: strcpy(role, "STAFF"); break;
        case 3: strcpy(role, "GUEST"); break;
        default:
            printf("Invalid role choice. Defaulting to GUEST.\n");
            strcpy(role, "GUEST");
    }

    // Check if username already exists
    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (fp) {
        while (fscanf(fp, "%s %s %s", fileuser, filepass, fileRole) == 3) {
            if (strcmp(username, fileuser) == 0) {
                printf("Username already exists. Please try a different username.\n");
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }

    fp = fopen(CREDENTIAL_FILE, "a");
    if (!fp) {
        printf("Error opening %s for writing.\n", CREDENTIAL_FILE);
        return;
    }

    fprintf(fp, "%s %s %s\n", username, password, role);
    fclose(fp);

    printf("Account created successfully! You can now login.\n");
}

void mainmenu() {    
    if (strcmp(currentUserRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentUserRole, "STAFF") == 0) {
        userMenu();
    } else {
        guestMenu();
    }
}

/* ADMIN MENU – full access */
void adminMenu() {
    int choice;
    do {
        printf("\n=== Admin Menu ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

/* STAFF MENU – same options (you can later restrict if needed) */
void userMenu() {
    int choice;
    do {
        printf("\n=== Staff Menu ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

/* GUEST MENU – same options (you can later limit to view/search only) */
void guestMenu() {
    int choice;
    do {
        printf("\n=== Guest Menu ===\n");
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logging out...\n"); return;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (1);
}

void addStudent() {
    FILE *fp = fopen(STUDENT_FILE, "a");
    struct Student st;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No: ");
    scanf("%d", &st.rollno);
    printf("Enter Name: ");
    scanf("%s", st.name);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully.\n");
}

void viewStudents() {
    FILE *fp = fopen(STUDENT_FILE, "r");
    struct Student st;

    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\n=== Student Records ===\n");
    printf("Roll No\tName\tMarks\n");

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        printf("%d\t%s\t%.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
}

void searchStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No to search: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Student Found: %d %s %.2f\n", st.rollno, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Student with Roll No %d not found.\n", rollno);

    fclose(fp);
}

void updateStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file\n");
        fclose(fp);
        return;
    }

    printf("\nEnter Roll No to update: ");
    scanf("%d", &rollno);

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Current Record: %d %s %.2f\n", st.rollno, st.name, st.marks);
            printf("Enter New Name: ");
            scanf("%s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Student with Roll No %d not found.\n", rollno);
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Record updated successfully.\n");
    }
}

void deleteStudent() {
    int rollno, found = 0;
    struct Student st;

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) {
        printf("Error opening file\n");
        return;
    }

    printf("\nEnter Roll No to delete: ");
    scanf("%d", &rollno);

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temporary file\n");
        fclose(fp);
        return;
    }

    while (fscanf(fp, "%d %s %f", &st.rollno, st.name, &st.marks) == 3) {
        if (st.rollno == rollno) {
            printf("Student Deleted: %d %s %.2f\n", st.rollno, st.name, st.marks);
            found = 1;
        } else {
            fprintf(temp, "%d %s %.2f\n", st.rollno, st.name, st.marks);
        }
    }

    fclose(fp);
    fclose(temp);

    if (!found) {
        printf("Student with Roll No %d not found.\n", rollno);
        remove("temp.txt");
    } else {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Student record deleted successfully.\n");
    }
}
