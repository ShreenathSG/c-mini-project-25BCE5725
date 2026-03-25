#include <stdio.h>

#define MAX_STUDENTS 100

struct Student {
    int regno;
    char name[30];
    int present;
    int total;
};

struct Student s[MAX_STUDENTS];
int n = 0;

void loadData() {
    FILE *fp;
    int r;

    fp = fopen("attendance.txt", "r");
    if (fp == NULL) {
        return;
    }

    n = 0;

    while (n < MAX_STUDENTS) {
        r = fscanf(fp, "%d %s %d %d",
                   &s[n].regno,
                   s[n].name,
                   &s[n].present,
                   &s[n].total);

        if (r == EOF) {
            break;
        }

        if (r != 4) {
            int c;
            while ((c = fgetc(fp)) != '\n' && c != EOF) {
            }
            continue;
        }

        n++;
    }

    fclose(fp);
}

void saveData() {
    FILE *fp;
    int i;
    fp = fopen("attendance.txt", "w");
    if (fp == NULL) {
        printf("File error\n");
        return;
    }
    for (i = 0; i < n; i++) {
        fprintf(fp, "%d %s %d %d\n",
                s[i].regno,
                s[i].name,
                s[i].present,
                s[i].total);
    }
    fclose(fp);
}

int searchByReg(int reg) {
    int i;
    for (i = 0; i < n; i++) {
        if (s[i].regno == reg) {
            return i;
        }
    }
    return -1;
}

void addStudent() {
    int reg, pos;
    if (n >= MAX_STUDENTS) {
        printf("Student limit reached\n");
        return;
    }
    printf("Enter register number: ");
    scanf("%d", &reg);
    pos = searchByReg(reg);
    if (pos != -1) {
        printf("This regno already exists\n");
        return;
    }
    s[n].regno = reg;
    printf("Enter name (single word): ");
    scanf("%s", s[n].name);
    s[n].present = 0;
    if (n == 0) {
        s[n].total = 0;
    } else {
        s[n].total = s[0].total;
    }
    n++;
    saveData();
    printf("Student added\n");
}

float findPercent(struct Student st) {
    float per;
    if (st.total == 0) {
        per = 0;
    } else {
        per = (st.present * 100.0f) / st.total;
    }
    return per;
}

void markTodayAttendance() {
    int i;
    char ch;
    if (n == 0) {
        printf("No students to mark\n");
        return;
    }
    printf("Mark attendance (P/A)\n");
    for (i = 0; i < n; i++) {
        printf("%d %s : ", s[i].regno, s[i].name);
        scanf(" %c", &ch);
        if (ch == 'P' || ch == 'p') {
            s[i].present = s[i].present + 1;
        }
        s[i].total = s[i].total + 1;
    }
    saveData();
    printf("Attendance updated\n");
}

void showAllStudents() {
    int i;
    float per;
    if (n == 0) {
        printf("No students\n");
        return;
    }
    printf("Regno\tName\tPresent\tTotal\tPercent\n");
    for (i = 0; i < n; i++) {
        per = findPercent(s[i]);
        printf("%d\t%s\t%d\t%d\t%.2f\n",
               s[i].regno,
               s[i].name,
               s[i].present,
               s[i].total,
               per);
    }
}

void showShortage() {
    int i, flag = 0;
    float per;
    if (n == 0) {
        printf("No students\n");
        return;
    }
    printf("Students below 75%%:\n");
    for (i = 0; i < n; i++) {
        per = findPercent(s[i]);
        if (per < 75.0f) {
            printf("%d %s -> %.2f%%\n",
                   s[i].regno,
                   s[i].name,
                   per);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("No shortage students\n");
    }
}

void classSummary() {
    int i;
    int totalPresent = 0;
    int totalPeriods = 0;
    float avg = 0;
    if (n == 0) {
        printf("No students\n");
        return;
    }
    for (i = 0; i < n; i++) {
        totalPresent = totalPresent + s[i].present;
        totalPeriods = totalPeriods + s[i].total;
    }
    if (totalPeriods > 0) {
        avg = (totalPresent * 100.0f) / totalPeriods;
    }
    printf("Total students : %d\n", n);
    printf("Total classes  : %d\n", s[0].total);
    printf("Class average  : %.2f%%\n", avg);
}

void searchStudentMenu() {
    int reg, pos;
    float per;
    printf("Enter regno to search: ");
    scanf("%d", &reg);
    pos = searchByReg(reg);
    if (pos == -1) {
        printf("Student not found\n");
    } else {
        per = findPercent(s[pos]);
        printf("Regno   : %d\n", s[pos].regno);
        printf("Name    : %s\n", s[pos].name);
        printf("Present : %d\n", s[pos].present);
        printf("Total   : %d\n", s[pos].total);
        printf("Percent : %.2f\n", per);
    }
}

void showFullAttendance() {
    int i, flag = 0;
    if (n == 0) {
        printf("No students\n");
        return;
    }
    printf("Students with 100%% attendance:\n");
    for (i = 0; i < n; i++) {
        if (s[i].total > 0 && s[i].present == s[i].total) {
            printf("%d %s\n", s[i].regno, s[i].name);
            flag = 1;
        }
    }
    if (flag == 0) {
        printf("No one has 100%%\n");
    }
}

int main() {
    int ch;
    loadData();
    while (1) {
        printf("\n--- Attendance Manager ---\n");
        printf("1. Add student\n");
        printf("2. Mark today's attendance\n");
        printf("3. Show all students\n");
        printf("4. List shortage (below 75%%)\n");
        printf("5. Class summary\n");
        printf("6. Search student by regno\n");
        printf("7. Show 100%% attendance students\n");
        printf("8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &ch);
        if (ch == 1) {
            addStudent();
        } else if (ch == 2) {
            markTodayAttendance();
        } else if (ch == 3) {
            showAllStudents();
        } else if (ch == 4) {
            showShortage();
        } else if (ch == 5) {
            classSummary();
        } else if (ch == 6) {
            searchStudentMenu();
        } else if (ch == 7) {
            showFullAttendance();
        } else if (ch == 8) {
            saveData();
            break;
        } else {
            printf("Wrong choice\n");
        }
    }
    return 0;
}

