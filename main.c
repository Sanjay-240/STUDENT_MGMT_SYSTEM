#include <stdio.h>
#include <string.h>

struct student {
    int roll;
    char name[50];
    float marks;
};

void insertStudentInfo(void);
void displayStudentInfo(void);
void deleteStudentInfo(void);

int main() {
    int choice;
    
    while (1) {
        printf("\n--- Student Management System ---\n");
        printf("1. Insert Student\n");
        printf("2. Display Students\n");
        printf("3. Delete Student\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                insertStudentInfo();
                break;
            case 2:
                displayStudentInfo();
                break;
            case 3:
                deleteStudentInfo();
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}

void insertStudentInfo(void) {
    struct student s;
    FILE *fp = fopen("student.txt", "a");
    
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // Write header only if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "Roll No    Name      Marks\n");
    }
    
    printf("Enter Roll No: ");
    scanf("%d", &s.roll);
    
    printf("Enter Name: ");
    scanf("%s", s.name);
    
    printf("Enter Marks: ");
    scanf("%f", &s.marks);
    
    fprintf(fp, "%d    %s    %.2f\n", s.roll, s.name, s.marks);
    
    fclose(fp);
    printf("Student added successfully!\n");
}

void displayStudentInfo(void)
{
    struct student students[100];
    struct student temp;
    int count = 0;
    int choice;
    FILE *fp = fopen("student.txt", "r");
    char header[100];
    
    if (fp == NULL) {
        printf("Error: File not found!\n");
        return;
    }
    
    // Read header
    fgets(header, sizeof(header), fp);
    
    // Read all records into array
    while (fscanf(fp, "%d %s %f", &students[count].roll, 
                  students[count].name, &students[count].marks) == 3) {
        count++;
        if (count >= 100) break;  // Prevent array overflow
    }
    
    fclose(fp);
    
    if (count == 0) {
        printf("No records found!\n");
        return;
    }
    
    // Ask user for sorting preference
    printf("\n--- Sort by Marks ---\n");
    printf("1. Ascending Order (Low to High)\n");
    printf("2. Descending Order (High to Low)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    // Sorting using Bubble Sort
    if (choice == 1) {
        // Ascending Order
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].marks > students[j + 1].marks) {
                    temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\n--- Student Records (Ascending Order) ---\n");
    } else if (choice == 2) {
        // Descending Order
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (students[j].marks < students[j + 1].marks) {
                    temp = students[j];
                    students[j] = students[j + 1];
                    students[j + 1] = temp;
                }
            }
        }
        printf("\n--- Student Records (Descending Order) ---\n");
    } else {
        printf("Invalid choice!\n");
        return;
    }
    
    // Display header
    printf("%s", header);
    
    // Display sorted records
    for (int i = 0; i < count; i++) {
        printf("%d    %s    %.2f\n", students[i].roll, 
               students[i].name, students[i].marks);
    }
}
void deleteStudentInfo(void) {
    struct student s;
    int rollToDelete;
    FILE *fp, *temp;
    int found = 0;
    char header[100];
    
    // Open original file for reading
    fp = fopen("student.txt", "r");
    
    if (fp == NULL) {
        printf("Error: File not found!\n");
        return;
    }
    
    // Open temporary file for writing
    temp = fopen("temp.txt", "w");
    
    if (temp == NULL) {
        printf("Error: Cannot create temporary file!\n");
        fclose(fp);
        return;
    }
    
    // Get roll number to delete
    printf("Enter Roll No to Delete: ");
    scanf("%d", &rollToDelete);
    
    // Read and write header line
    fgets(header, sizeof(header), fp);
    fprintf(temp, "%s", header);
    
    // Read from original file and write to temp (except matching record)
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll == rollToDelete) {
            found = 1;  // Record found
            printf("Record with Roll No %d deleted successfully!\n", rollToDelete);
        } else {
            // Write non-matching records to temp file
            fprintf(temp, "%d    %s    %.2f\n", s.roll, s.name, s.marks);
        }
    }
    
    // Check if record was found
    if (!found) {
        printf("Record with Roll No %d not found!\n", rollToDelete);
    }
    
    // Close both files
    fclose(fp);
    fclose(temp);
    
    // Delete original file and rename temp file
    remove("student.txt");
    rename("temp.txt", "student.txt");
    
    printf("\nFile updated successfully!\n");
}