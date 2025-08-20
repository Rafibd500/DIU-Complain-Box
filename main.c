#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define MAX_COMPLAIN 5000



int annonymous=0;
struct Student{
    char log_status[2];
    char id[15];
    char name[100];
    char section[5];
    char dept[5];
    char mobile[15];
    char email[50];
    char dob[15];
};
struct Student students_data[3000];
char current_logged_student[15]; //current logged student id
struct Login_info{
    char id[15];
    char password[20];
};
struct Login_info students_login_info[3000];

int student_cnt;
int registered_students = 0;


// structure for complain 
struct Complain{
    char cmpID[20];
    char studentID[20];
    char title[200];
    char team[20];
    char description[1000];
    char status[2];
    char date[20];
    char time[20];
    char comment[100];
    char annonymous[5];
};

struct Complain all_complains[MAX_COMPLAIN];



struct Admin {
    char name[50];
    char adminID[20];
    char email[50];
    char phone[20];
    char username[30];
    char password[30];
};
char current_logged_admin[30];
struct Admin admins[100];
int admin_count = 0;

char current_logged_dept_code[10] = "";     // e.g., "CSE"
char current_logged_dept_user[30] = "";     // username of dept user




// ======= Function call =======
// ======= Function call =======

// console / printing
void textColor(int colorCode);
int  getConsoleWidth(void);
int  fetchConsoleWidth(void);
void printCenter(const char *text, int color);
void printCenterCustom(const char *text, int color);
void printLeft(const char *text, int color);
void print_project_name(void);

// student
void student_login_reg(void);
void student_registration(void);
void student_login(void);
void append_id_password(char id[], char password[]);
void password_encrypt(char *password);
void password_decrypt(char *password);
void load_students_data(void);
void upload_students_data(void);
void load_logged_student_data(void);
void upload_logged_student_data(void);
int  binary_search_on_students_data(char id[]);
int  binary_search_on_students_login(char id[]);
void sort_student_login_info(void);

// student dashboard
void student_dashboard(char id[]);
void submit_new_complain(char id[]);
void view_all_complain(char id[]);
void submit_annonymus_complain(char id[]);
void track_complain_by_complainID(char id[]);
void my_profile(char id[]);
void student_logout(char id[]);

// complaint creation
void take_complain(char id[], int choice);
char* generate_complain_ID(void);

// admin
void admin_dashboard(void);
void admin_login(void);
void view_all_complain_admin(void);
void view_complaint_by_id_admin(void);
void create_admin_account(void);
void create_department_user(void);
void admin_logout(void);
void view_student_details_with_id(void);

// dept helpers
void department_dashboard(const char *dept_code);
const char* deptCode_to_full(const char *code);
void dept_view_all_complaints(const char *dept_code);
void dept_show_complaint_details(struct Complain complaints[], int count, const char *dept_code, char cmp_id[]);
void dept_update_status_and_comment(struct Complain complaints[], int count, const char *dept_code, char cmp_id[]);
void dept_track_by_id(const char *dept_code);
void dept_add_team_user(const char *dept_code);
void dept_view_student_profile_with_id(void);
void department_logout(void);
void department_login(const char *dept_code);




//========== text styling and alignment start ==========
void textColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

int fetchConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo)) {
        return bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1;
    }
    return 80; // Default width
}

void printCenter(const char *text, int color)
{
    int consoleWidth = getConsoleWidth();
    int len = strlen(text);
    // int spacing = (width) / 2 - 15;
    int spacing = (consoleWidth - len) / 2;
    textColor(color);
    for (int i = 0; i < spacing; i++)
        printf(" ");
    printf("%s", text);
    textColor(7); // Reset color
}
void printCenterCustom(const char *text, int color)
{
    int consoleWidth = getConsoleWidth();
    int len = strlen(text);
    // int spacing = (width) / 2 - 15;
    int spacing = (consoleWidth - len) / 2;
    textColor(color);
    for (int i = 0; i < spacing/2; i++)
        printf(" ");
    printf("%s", text);
    textColor(7); // Reset color
}
// left text with color
void printLeft(const char *text, int color)
{
    textColor(color);
    printf("%s", text);
    textColor(7); // Reset color
}

//========== text styling and alignment end ==========


void print_project_name(){
    
    int width = getConsoleWidth();
        for (int i = 0; i < width; i++)
            printLeft("*", 11);
    printCenter("DIU COMPLAIN BOX\n", 14);
        for (int i = 0; i < width; i++)
            printLeft("*", 11);
}

// ---------- load students data from file ----------
void load_students_data(){
    student_cnt = 0;  // Reset count before loading
    FILE *fp = fopen("students_data/students.txt", "r");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
        students_data[student_cnt].log_status,
        students_data[student_cnt].id,
        students_data[student_cnt].name,
        students_data[student_cnt].section,
        students_data[student_cnt].dept,
        students_data[student_cnt].mobile,
        students_data[student_cnt].email,
        students_data[student_cnt].dob) != EOF)
        {
        student_cnt++;
}

        fclose(fp);
}

// ---------- Upload students data from file ----------
void upload_students_data(){
    FILE *fp = fopen("students_data/students.txt", "w");
    for (int j = 0; j < student_cnt; j++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s|%s|%s\n",
            students_data[j].log_status,
            students_data[j].id,
            students_data[j].name,
            students_data[j].section,
            students_data[j].dept,
            students_data[j].mobile,
            students_data[j].email,
            students_data[j].dob);
    }
    fclose(fp);
}


// ---------- load login info from file ----------

void load_logged_student_data(){
    registered_students = 0;
    FILE *fp = fopen("registered_students/login_info.txt", "r");
    if (fp == NULL)
    {
        printf("File not found!\n");
        return;
    }
    while (fscanf(fp, "%[^|]|%[^\n]\n",
        students_login_info[registered_students].id,
        students_login_info[registered_students].password) != EOF)
        {
        registered_students++;
        
    }

        fclose(fp);
}

void upload_logged_student_data(){
    FILE *fp = fopen("registered_students/login_info.txt", "w");
    if (fp == NULL){
        printf("File not found!\n");
        return;
    }
    for (int j = 0; j < registered_students; j++) {
        fprintf(fp, "%s|%s\n",
            students_login_info[j].id,
            students_login_info[j].password);
    }
    fclose(fp);
}
// ---------- encrypt password ----------
void password_encrypt(char *password){
    int len = strlen(password);
    for(int i = 0; i<len; i++) password[i] ^= 5;
}
// ---------- decrypt password ----------
void password_decrypt(char *password){
    int len = strlen(password);
    for(int i = 0; i<len; i++) password[i] ^= 5;
}
// ---------- hide password ----------
void input_hidden_password(char password[]) {
    char ch;
    int i = 0;
    while ((ch = getch()) != 13) { // 13 enter key
        if (ch == 8 && i > 0) { // backspace
            printf("\b \b");
            i--;
        }
        else if (ch != 8 && i < 19) { 
            password[i++]=ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate string
    printf("\n");
}

// ---------- binary_search_on_students_data ----------
int binary_search_on_students_data(char id[]){
    load_students_data();  // Load student data from file
    int l = 0, r = student_cnt-1;
    while(l<=r){
        int mid = (l+r)/2;
        if(strcmp(students_data[mid].id, id) == 0) return mid;
        else if(strcmp(students_data[mid].id, id) < 0) l = mid+1;
        else r = mid-1;
    }
    return -1;
}

// ---------- binary_search_on_students_login ----------
int binary_search_on_students_login(char id[]){

    int l = 0, r = registered_students-1;
    while(l<=r){
        int mid = (l+r)/2;
        if(strcmp(students_login_info[mid].id, id) == 0) return mid;
        else if(strcmp(students_login_info[mid].id, id) < 0) l = mid+1;
        else r = mid-1;
    }
    return -1;
}
// ---------- sort_student_login_info ---------- bubble sort
void sort_student_login_info(){
    load_logged_student_data();
    int flag = 0;
    for(int i = 0; i<registered_students-1; i++){
        flag = 0;
        for(int j = 0; j<registered_students-i-1; j++){
            if(strcmp(students_login_info[j].id, students_login_info[j+1].id)>0){
                struct Login_info tmp = students_login_info[j];
                students_login_info[j] = students_login_info[j+1];
                students_login_info[j+1] = tmp;
                flag = 1;
            }
        }
        if(!flag) break;
    }
    upload_logged_student_data();
}


// ================== student registration / Login choice page start ==================
void student_login_reg()
{
    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("Student's Registration/Login\n", 10);
        printCenter("1. Registration\n", 11);
        printCenter("2. Login        \n", 11);
        printCenter("0. Main Menu   \n", 11);
        printLeft("Enter your choice: ", 2);
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            student_registration();
            break;
        case 2:
            student_login();
            break;
        case 0:
            break;
        default:
            printf("Wrong Choice. Please enter a valid one.\n");
            Sleep(2000);
        }

    } while (choice != 0);
}
// ================== student registration / Login choice page start ==================

// ================== student registration start ==================
void student_registration()
{
    
    load_students_data();  // Load student data from file
    system("cls");

    char id[15];
    print_project_name();
    printCenter("==== Registration ====\n", 10);
    printLeft("Enter ID: ", 10);
    scanf("%s", id);

    int found = binary_search_on_students_data(id);
    
    if (found != -1 && strcmp(students_data[found].log_status, "1") == 0) {
        printCenter("You already have an account. Please login...\n", 4);
        Sleep(3000);
        student_login();
        return;
    }

    
    // ---------- Account validation ----------
    if (found != -1) {
        char dob[15];
        printCenter("Answer security question:\n", 9);
        getchar(); 
        printLeft("Enter your Date of Birth (DD/MM/YYYY): ", 11);
        scanf("%s", dob);

        if (strcmp(dob, students_data[found].dob) != 0) {
            printCenter("Invalid Information. Please try again later\n", 4);
            Sleep(3000);
            return;
        } 
        else {
            printCenter("Account validation successful\n", 10);
            char password[20], password2[20];
            printf("\033[5;31mThe password must contains 8 character. Includeing (A-Z, a-z, special char, 0-9)\033[0m\n");
        
            // ---------- password validation ----------
            while(1){
                int valid = 1, upper = 0, lower = 0, special = 0, digit = 0;
                printLeft("Enter password: ", 11);
                input_hidden_password(password);
                if(strlen(password) < 8){
                    printf("\033[5;31mThe password must contains 8 character.\033[0m\n");
                    continue;
                }
                for(int i = 0; i<strlen(password); i++){
                    if(isupper(password[i])) upper = 1;
                    else if(islower(password[i])) lower = 1;
                    else if(ispunct(password[i])) special = 1;
                    else if(isdigit(password[i])) digit = 1;
                }
                if(!upper || !lower || !special || !digit){
                    valid =  0;
                    if(!upper) printf("\033[5;31m** Missing [A-Z] \033[0m\n");
                    if(!lower) printf("\033[5;31m** Missing [a-z] \033[0m\n");
                    if(!special) printf("\033[5;31m** Missing [@, $, &, etc] \033[0m\n");
                    if(!digit) printf("\033[5;31m** Missing [0-9] \033[0m\n");
                }

                if(valid == 1) break;
            }
            system("cls");
            print_project_name();
            printCenter("==== Registration ====\n", 10);
            printLeft("Enter ID: ", 10);
            printf("%s\n", id);
            printLeft("Enter password: ", 11);
            int sz = strlen(password);
            for(int i = 0; i<sz; i++){
                printf("*");
            }
            printf("\n");
            bool flag = false; //password hide status
            while(1){
                if(!flag){
                    getchar();
                    printLeft("Show Password?(y/n): ", 11);
                    char ch;
                    scanf("%c", &ch);
                    if(ch == 'y') {
                        printLeft("Enter password: ", 11);
                        printf("%s\n", password);
                        flag = true;
                    }
                }
                printLeft("Repeat Password: ", 11);
                input_hidden_password(password2);
                if(strcmp(password, password2) == 0){
                    break;
                }
                else{
                    printf("\033[5;31mPassword not matched. Please enter repeat password again.\033[0m\n");
                }
            }
            append_id_password(id, password);
            printCenter("Registration successful. Redirecting to login...", 10);
            strcpy(students_data[found].log_status, "1");
            upload_students_data();
            Sleep(3000);
            sort_student_login_info();
            student_login();
            
        }
    } 
    else {
        printCenter("Invalid ID. Please check your ID again\n", 4);
        Sleep(3000);
        // upload_students_data();
        return;
    }
}
// ================== student registration end ==================

// ================== append student id password to file start ==================
void append_id_password(char id[], char password[]){
    password_decrypt(password);
    FILE *fp = fopen("registered_students/login_info.txt", "a");
    if (fp == NULL){
        printf("Error: Could not open login_info.txt\n");
        return;
    }
    fprintf(fp, "%s|%s\n", id, password);
    fclose(fp);
}
// ================== append student id password to file end ==================

// ================== student Login start ==================
void student_login(){
    system("cls");
    print_project_name();
    printCenter("Login\n", 12);
    printCenter("---------------------------------------------------\n", 9);
    char id[15], password[20];
    printLeft("Enter ID: ", 15);
    scanf("%s", id);
    printLeft("Enter Password: ", 15);
    input_hidden_password(password);
    load_logged_student_data();
    int found1 = binary_search_on_students_login(id);
    if(found1 != -1){ 
        if(strcmp(students_login_info[found1].id, id) == 0){
            char decrypt_pass[20];
            strcpy(decrypt_pass, students_login_info[found1].password);
            password_decrypt(decrypt_pass);
            if(strcmp(decrypt_pass, password) == 0){
                int found = binary_search_on_students_data(id);
                printCenterCustom("Welcome, ", 10);
                textColor(10);
                printf("%s! Login Successful. Redirecting to Student Dashboard...\n", students_data[found].name, 10);
                strcpy(current_logged_student, students_login_info[found1].id);
                textColor(7); // Reset color
                Sleep(4000);
                student_dashboard(id);
                return;
                
            }
            else{
                printCenter("Invalid password", 4);
                Sleep(3000);
                student_login_reg();
                return;
            }
        }
    }
    
    printCenter("ID not found", 4);
    Sleep(3000);
}
// ================== student Login end ==================

/////////////////////GENERATE ID //////////////
char* generate_complain_ID(){
    time_t t;
    struct tm *tm_info;
    char date_str[20];
    time(&t);
    tm_info = localtime(&t);
    strftime(date_str, sizeof(date_str), "%d%m%Y", tm_info);

    char *cmpID = malloc(30);
    int serial = 1;
    char last_date[20] = "00000000";
    int last_serial = 0;

    FILE *fp = fopen("serial.txt", "r");
    if (fp != NULL) {
        fscanf(fp, "%s %d", last_date, &last_serial);
        fclose(fp);
        if (strcmp(last_date, date_str) == 0) {
            serial = last_serial + 1;
        }
    }
    fp = fopen("serial.txt", "w");
    fprintf(fp, "%s %d", date_str, serial);
    fclose(fp);
    sprintf(cmpID, "CMP-%s-%04d", date_str, serial);
    return cmpID;
}


// ================== take complain fucntion start ==================
// ================== take complain function start ==================
void take_complain(char id[], int choice){
    struct Complain student_complain;
    switch (choice)
    {
        case 1: strcpy(student_complain.team, "Hall Authority"); break;
        case 2: strcpy(student_complain.team, "IT Section"); break;
        case 3: strcpy(student_complain.team, "Dept. Of CSE"); break;
        case 4: strcpy(student_complain.team, "Dept. Of SWE"); break;
        case 5: strcpy(student_complain.team, "BLC Support Team"); break;
        case 6: strcpy(student_complain.team, "Finance and Accounts Team"); break;
        case 7: strcpy(student_complain.team, "Transport Management Team"); break;
        default: strcpy(student_complain.team, "Unknown Team"); break;
    }

    system("cls");
    print_project_name();
    if(!annonymous) printCenter("Submit a New Complain\n", 10);
    else printCenter("Submit a new Complain Annonymusly\n", 10);
    printCenter("---------------------------------------------------\n", 9);

    int found = binary_search_on_students_data(id);
    textColor(14);
    printf("User: %s(%s)                                                                                                                                                       Role:Student\n", students_data[found].name, students_data[found].id);
    textColor(7);

    textColor(11);
    printf("To: %s\n", student_complain.team);
    textColor(7);

    printLeft("Enter Complain Title: ", 11);
    getchar();
    fgets(student_complain.title, sizeof(student_complain.title), stdin);
    student_complain.title[strcspn(student_complain.title, "\n")] = '\0';

    printLeft("Enter Complain Description: ", 11);
    fgets(student_complain.description, sizeof(student_complain.description), stdin);
    student_complain.description[strcspn(student_complain.description, "\n")] = '\0';

    char* cmpID = generate_complain_ID();
    strcpy(student_complain.cmpID, cmpID);
    strcpy(student_complain.status, "1");

    // TIME & DATE
    time_t t; struct tm *tm_info;
    char time_str[20], date_str[20];
    time(&t); tm_info = localtime(&t);
    strftime(time_str, sizeof(time_str), "%I:%M %p", tm_info);
    strftime(date_str, sizeof(date_str), "%d/%m/%Y", tm_info);

    strcpy(student_complain.time, time_str);
    strcpy(student_complain.date, date_str);
    strcpy(student_complain.comment, "N/A");
    if(annonymous) strcpy(student_complain.annonymous, "y");
    else strcpy(student_complain.annonymous, "n");
    strcpy(student_complain.studentID, id);

    printCenter("\nComplaint submitted successfully!\nYour complaint has been recorded. You will be notified upon status updates.\n", 10);
    printf("Press any key to continue....");
    _getch();

    // 1) Write to student personal file (keep old 9 fields for backward comp.)
    char filename[100];
    sprintf(filename, "students_complain/%s.txt", id);
    FILE* studnetFp = fopen(filename, "a");
    if (studnetFp) {
        fprintf(studnetFp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
            student_complain.cmpID,
            student_complain.title,
            student_complain.description,
            student_complain.team,
            student_complain.status,
            student_complain.date,
            student_complain.time,
            student_complain.comment,
            student_complain.annonymous
        );
        fclose(studnetFp);
    }

    // 2) Write to admin file (10 fields)
    FILE* adminFp = fopen("admin/all_complain.txt", "a");
    if (adminFp) {
        fprintf(adminFp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
            student_complain.cmpID,
            student_complain.studentID,
            student_complain.title,
            student_complain.team,
            student_complain.description,
            student_complain.status,
            student_complain.date,
            student_complain.time,
            student_complain.comment,
            student_complain.annonymous
        );
        fclose(adminFp);
    }

    // 3) Write to department file (10 fields, same format as admin)
    const char* deptCode;
    if (strcmp(student_complain.team, "IT Section") == 0) deptCode = "IT";
    else if (strcmp(student_complain.team, "Dept. Of CSE") == 0) deptCode = "CSE";
    else if (strcmp(student_complain.team, "Dept. Of SWE") == 0) deptCode = "SWE";
    else if (strcmp(student_complain.team, "BLC Support Team") == 0) deptCode = "BLC";
    else if (strcmp(student_complain.team, "Finance and Accounts Team") == 0) deptCode = "FIN";
    else if (strcmp(student_complain.team, "Transport Management Team") == 0) deptCode = "TRN";
    else if (strcmp(student_complain.team, "Hall Authority") == 0) deptCode = "HALL";
    else deptCode = "MISC";

    char deptFile[128];
    sprintf(deptFile, "others_dept/complains/%s.txt", deptCode);

    FILE* deptFp = fopen(deptFile, "a");
    if (deptFp) {
        fprintf(deptFp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
            student_complain.cmpID,
            student_complain.studentID,
            student_complain.title,
            student_complain.team,
            student_complain.description,
            student_complain.status,
            student_complain.date,
            student_complain.time,
            student_complain.comment,
            student_complain.annonymous
        );
        fclose(deptFp);
    }

    free(cmpID);
    annonymous = 0;
}
// ================== take complain function end ==================



// ================== take complain fucntion end ==================

// ================== submit new complain start ==================
void submit_new_complain(char id[]){
    system("cls");
    print_project_name();
    if(!annonymous) printCenter("Submit a New Complain\n", 10);
    else printCenter("Submit a new Complain Annonymusly\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    int found = binary_search_on_students_data(id);
    textColor(14);
        printf("User: %s(%s)                                                                                                                                                       Role:Student\n", students_data[found].name, students_data[found].id);    textColor(7);
    printCenter("Select the complain Team: \n", 10);
    int choice;
    printCenter(" 01. Hall Authority   \n", 11);
    printCenter("  02. IT Section       \n", 11);
    printCenter("03. Dept. Of CSE    \n", 11);
    printCenter("04. Dept. Of SWE    \n", 11);
    printCenter("05. BLC Support Team\n", 11);
    printCenter("          06. Finance and Accounts Team\n", 11);
    printCenter("          07. Transport Management Team\n", 11);
    printCenter("0. Back\n", 11);

    printf("Enter Choice: ");
    scanf("%d", &choice);
    if(choice == 0) return;
    take_complain(id, choice);
}
// ================== submit new complain end ==================

// ================== submit anonymous complain start ==================
void submit_annonymus_complain(char id[]){
    annonymous = 1;
    submit_new_complain(id);
}
// ================== submit anonymous complain end ==================

void spacePrint(){
    int width = getConsoleWidth();
    for(int i=0; i<width/9; i++) printf(" ");
}
// ================== view all complain start ==================
void show_complain_details_by_id(struct Complain complaints[], int count, char cmp_id[]) {
    int found = -1;

    // Search complaint ID
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) {
            found = i;
            break;
        }
    }

    //  display details
    if (found != -1) {
        system("cls");
        print_project_name();
        printCenter("Complaint Details\n", 11);

        textColor(14); printf("Complain ID     : "); textColor(7); printf("%s\n", complaints[found].cmpID);
        textColor(14); printf("Title           : "); textColor(7); printf("%s\n", complaints[found].title);
        textColor(14); printf("Description     : "); textColor(7); printf("%s\n", complaints[found].description);
        textColor(14); printf("Team            : "); textColor(7); printf("%s\n", complaints[found].team);

        textColor(14); printf("Status          : "); textColor(7);
        if (strcmp(complaints[found].status, "1") == 0) { textColor(12); printf("Pending\n"); }
        else if (strcmp(complaints[found].status, "2") == 0) { textColor(6); printf("In Progress\n"); }
        else if (strcmp(complaints[found].status, "3") == 0) { textColor(10); printf("Resolved\n"); }
        else { textColor(7); printf("Unknown\n"); }

        textColor(14); printf("Date            : "); textColor(7); printf("%s\n", complaints[found].date);
        textColor(14); printf("Time            : "); textColor(7); printf("%s\n", complaints[found].time);
        textColor(14); printf("Comment         : "); textColor(7); printf("%s\n", complaints[found].comment);
        textColor(14); printf("Anonymous       : "); textColor(7);
        printf("%s\n", strcmp(complaints[found].annonymous, "y") == 0 ? "Yes" : "No");
    } else {
        printCenter("\nComplaint ID not found.\n", 4);
    }

    printCenter("\nPress any key to return...", 7);
    _getch();
}


void view_all_complain(char id[]) {
    system("cls");
    print_project_name();
    printCenter("View All Complaints\n", 10);
    printCenter("---------------------------------------------------\n", 9);

    char filename[100];
    sprintf(filename, "students_complain/%s.txt", id);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printCenter("no complaints found for this student.\n", 4);
        printf("press any key to return...");
        _getch();
        return;
    }

    // load all complaints from file
    struct Complain complaints[200];
    int count = 0;

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
            complaints[count].cmpID,
            complaints[count].title,
            complaints[count].description,
            complaints[count].team,
            complaints[count].status,
            complaints[count].date,
            complaints[count].time,
            complaints[count].comment,
            complaints[count].annonymous) != EOF) {
        count++;
    }
    fclose(fp);

    int filter_choice;
    do {
        // display all complaints
        system("cls");
        print_project_name();
        printCenter("All Complaints\n", 10);
        printCenter("---------------------------------------------------\n", 9);
        int found = binary_search_on_students_data(id);
        textColor(14);
        printf("User: %s(%s)                                                                                                                                                       Role:Student\n\n", students_data[found].name, students_data[found].id);        textColor(7);
        spacePrint();
        printf("| %-25s | %-40s | %-25s | %-15s | %-15s |\n", 
               "Complain ID", "Title", "Team", "Status", "Issued Date");
        spacePrint();
        for (int i = 0; i < 138; i++) printf("-");
        printf("\n");

        for (int i = 0; i < count; i++) {
            // handle long titles
            char shortTitle[41];
            if (strlen(complaints[i].title) > 38) {
                strncpy(shortTitle, complaints[i].title, 37);
                shortTitle[37] = '.'; shortTitle[38] = '.'; shortTitle[39] = '.'; shortTitle[40] = '\0';
            } else {
                strcpy(shortTitle, complaints[i].title);
            }

            spacePrint();
            printf("| %-25s | %-40s | %-25s | ", complaints[i].cmpID, shortTitle, complaints[i].team);

            // print status
            if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-15s", "Pending"); }
            else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-15s", "In Progress"); }
            else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-15s", "Resolved"); }
            else { textColor(7); printf("%-15s", "Unknown"); }
            textColor(7);
            printf(" | %-15s |\n", complaints[i].date);
        }

        // menu for filtering
        printCenter("\nFilter Options:\n", 11);
        printCenter("1. Show Only Pending\n", 10);
        printCenter("2. Show Only In Progress\n", 10);
        printCenter("3. Show Only Resolved\n", 10);
        printCenter("4. Show Only Anonymous\n", 10);
        printCenter("5. View Full Complaint Details\n", 10);
        printCenter("0. Back\n", 10);
        printLeft("Enter your filter choice: ", 2);
        scanf("%d", &filter_choice);

        // handle filter 1-4 -- status+annonymous
        if (filter_choice >= 1 && filter_choice <= 4) {
            char status_filter[2] = "";
            int filter_anon = 0;

            if (filter_choice == 1) strcpy(status_filter, "1");
            else if (filter_choice == 2) strcpy(status_filter, "2");
            else if (filter_choice == 3) strcpy(status_filter, "3");
            else if (filter_choice == 4) filter_anon = 1;
            // display filtered complaints
            system("cls");
            print_project_name();
            if (filter_anon)
                printCenter("Filtered by: Anonymous Complaints\n", 11);
            else {
                if (strcmp(status_filter, "1") == 0) printCenter("Filtered by: Pending\n", 11);
                else if (strcmp(status_filter, "2") == 0) printCenter("Filtered by: In Progress\n", 11);
                else if (strcmp(status_filter, "3") == 0) printCenter("Filtered by: Resolved\n", 11);
            }
            textColor(14);
            printf("User: %s(%s)                                                                                                                                                       Role:Student\n\n", students_data[found].name, students_data[found].id);        textColor(7);
            spacePrint();
            printf("| %-25s | %-40s | %-25s | %-15s | %-15s |\n", "Complain ID", "Title", "Team", "Status", "Issued Date");
            spacePrint();
            for (int i = 0; i < 138; i++) printf("-");
            printf("\n");

            int filtered = 0;
            for (int i = 0; i < count; i++) {
                int match = 0;
                if (filter_anon && strcmp(complaints[i].annonymous, "y") == 0) match = 1;
                else if (!filter_anon && strcmp(complaints[i].status, status_filter) == 0) match = 1;
                if (match) {
                    char shortTitle[41];
                    if (strlen(complaints[i].title) > 38) {
                        strncpy(shortTitle, complaints[i].title, 37);
                        shortTitle[37] = '.'; shortTitle[38] = '.'; shortTitle[39] = '.'; shortTitle[40] = '\0';
                    } else {
                        strcpy(shortTitle, complaints[i].title);
                    }

                    spacePrint();
                    printf("| %-25s | %-40s | %-25s | ", complaints[i].cmpID, shortTitle, complaints[i].team);

                    if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-15s", "Pending"); }
                    else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-15s", "In Progress"); }
                    else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-15s", "Resolved"); }
                    else { textColor(7); printf("%-15s", "Unknown"); }
                    textColor(7);

                    printf(" | %-15s |\n", complaints[i].date);
                    filtered++;
                }
            }

            if (filtered == 0) {
                printCenter("\nNo complaints found for this filter.", 4);
            }

            printCenter("\nPress any key to return to filters...", 7);
            _getch();
        }

        // handle detailed complain
        else if (filter_choice == 5) {
            char cmp_id[30];
            printLeft("\nEnter Complaint ID to view details: ", 2);
            getchar(); // clear newline
            fgets(cmp_id, sizeof(cmp_id), stdin);
            cmp_id[strcspn(cmp_id, "\n")] = '\0';

            show_complain_details_by_id(complaints, count, cmp_id);
        }


    } while (filter_choice != 0);
}

// ================== view all complain end ==================





// ================== track complain by complain ID start ==================
void track_complain_by_complainID(char id[]){
    system("cls");
    print_project_name();
    printCenter("Track Complain by Complain ID\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    char filename[100];
    sprintf(filename, "students_complain/%s.txt", id);
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printCenter("no complaints found for this student.\n", 4);
        printf("press any key to return...");
        _getch();
        return;
    }

    // load all complaints from file
    struct Complain complaints[200];
    int count = 0;

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
            complaints[count].cmpID,
            complaints[count].title,
            complaints[count].description,
            complaints[count].team,
            complaints[count].status,
            complaints[count].date,
            complaints[count].time,
            complaints[count].comment,
            complaints[count].annonymous) != EOF) {
        count++;
    }
    fclose(fp);
    char cmp_id[30];
    printLeft("Enter Complain ID: ", 10);
    scanf("%s", cmp_id);
    show_complain_details_by_id(complaints, count, cmp_id);
}
// ================== submit new complain start ==================


// ================== my profile start ==================
void spacePrintProfile(){
    int width = getConsoleWidth();
    for(int i=0; i<width/2-35; i++) printf(" ");
}
void my_profile(char id[]) {
    system("cls");
    print_project_name();
    printCenter("My Profile\n", 11);
    printCenter("---------------------------------------------------\n", 9);

    int found = binary_search_on_students_data(id);
    if (found == -1) {
        textColor(12); // Red for error
        printCenter("Student not found.\n", 12);
        textColor(7);
        printf("Press any key to return...");
        _getch();
        return;
    }

    struct Student s = students_data[found];

    //head/ title
    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    spacePrintProfile(); textColor(11);
    printf("|%-25s", "");
    textColor(10);
    printf("PROFILE INFORMATION");
    textColor(11); printf("%-24s|\n", "");

    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    //show details----
    spacePrintProfile(); textColor(14); printf("| %-20s", "Student ID");     
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.id);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Name");           
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.name);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Section");        
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.section);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Department");     
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.dept);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Mobile Number");  
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.mobile);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Email");          
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.email);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Date of Birth");  
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.dob);

    
    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    
    textColor(10); 
    printCenter("\nPress any key to return to the dashboard...", 7);
    textColor(7);
    _getch();
}
// ==================  my profile end ==================


// ================== student logout start ==================
void student_logout(char id[]) {
    system("cls");
    print_project_name();
    printCenter("Logout\n", 10);
    printCenter("---------------------------------------------------\n", 9);

    // Clear the logged student variable
    strcpy(current_logged_student, "");

    printCenter("You have been logged out successfully.\n", 10);
    printCenter("Press any key to return to the main menu...", 11);
    _getch();

    return; // Go back to main()
}

// ================== student logout end ==================




// ================== student dashboard start ==================
void student_dashboard(char id[]) {
    if (strcmp(current_logged_student, "") == 0) {
        printCenter("Access denied. Please login first.\n", 12);
        printCenter("Press any key to return to main menu...", 4);
        _getch();
        return;
    }

    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("Student Dashboard\n", 10);
        printCenter("---------------------------------------------------\n", 9);
        int found = binary_search_on_students_data(id);
        textColor(14);
        printf("User: %s(%s)                                                                                                                                                       Role:Student\n", students_data[found].name, students_data[found].id);        textColor(7);
        printCenter("1. Submit a New Complain     \n", 11);
        printCenter("  2. Submit an Annonymus Complain\n", 11);
        printCenter("3. View My Complains          \n", 11);
        printCenter("  4. Track Complain By Complain ID\n", 11);
        printCenter("5. My Profile                \n", 11);
        printCenter("6. Logout                     \n", 11);
        printLeft("Enter Your Choice: ", 2);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                submit_new_complain(id); break;
            case 2:
                submit_annonymus_complain(id); break;
            case 3:
                view_all_complain(id); break;
            case 4:
                track_complain_by_complainID(id); break;
            case 5:
                my_profile(id); break;
            case 6:
                student_logout(id); return; // Logout and return to main()
            case 0:
                return; // Back to main menu
        }
    } while (1);
}

// ================== student dashboard end ==================


// ================== update complaint status and comment (admin) ==================
void update_complaint_status_and_comment(struct Complain complaints[], int count, char cmp_id[]) {
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printCenter("Complaint not found.\n", 4);
        printCenter("Press any key to return...", 7);
        _getch();
        return;
    }

    // Display complaint info
    system("cls");
    print_project_name();
    printCenter("Update Complaint Status and Comment\n", 11);

    printf("Complaint ID    : %s\n", complaints[found].cmpID);
    printf("Student ID      : %s\n", complaints[found].studentID);
    printf("Title           : %s\n", complaints[found].title);
    printf("Department      : %s\n", complaints[found].team);

    printf("Current Status  : ");
    if (strcmp(complaints[found].status, "1") == 0) {
        textColor(12); printf("Pending\n");
    } else if (strcmp(complaints[found].status, "2") == 0) {
        textColor(6); printf("In Progress\n");
    } else if (strcmp(complaints[found].status, "3") == 0) {
        textColor(10); printf("Resolved\n");
    } else {
        textColor(7); printf("Unknown\n");
    }
    textColor(7);
    printf("Current Comment : %s\n\n", complaints[found].comment);

    printf("Select New Status:\n1. Pending\n2. In Progress\n3. Resolved\nEnter choice: ");
    int status_choice;
    scanf("%d", &status_choice);
    getchar();

    if (status_choice >= 1 && status_choice <= 3) {
        sprintf(complaints[found].status, "%d", status_choice);
    }

    char new_comment[100];
    printf("Enter Comment (max 100 chars): ");
    fgets(new_comment, sizeof(new_comment), stdin);
    new_comment[strcspn(new_comment, "\n")] = '\0';
    strcpy(complaints[found].comment, new_comment);

    // ================= Update admin/all_complain.txt =================
    FILE *fp = fopen("admin/all_complain.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                complaints[i].cmpID,
                complaints[i].studentID,
                complaints[i].title,
                complaints[i].team,
                complaints[i].description,
                complaints[i].status,
                complaints[i].date,
                complaints[i].time,
                complaints[i].comment,
                complaints[i].annonymous);
    }
    fclose(fp);

    // ================= Update student's individual file =================
    char stu_filename[100];
    sprintf(stu_filename, "students_complain/%s.txt", complaints[found].studentID);
    struct Complain student_complaints[200];
    int stu_count = 0;

    FILE *stu_fp = fopen(stu_filename, "r");
    if (stu_fp) {
        while (fscanf(stu_fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                      student_complaints[stu_count].cmpID,
                      student_complaints[stu_count].title,
                      student_complaints[stu_count].description,
                      student_complaints[stu_count].team,
                      student_complaints[stu_count].status,
                      student_complaints[stu_count].date,
                      student_complaints[stu_count].time,
                      student_complaints[stu_count].comment,
                      student_complaints[stu_count].annonymous) != EOF) {
            strcpy(student_complaints[stu_count].studentID, complaints[found].studentID);
            stu_count++;
        }
        fclose(stu_fp);
    }

    for (int i = 0; i < stu_count; i++) {
        if (strcmp(student_complaints[i].cmpID, cmp_id) == 0) {
            strcpy(student_complaints[i].status, complaints[found].status);
            strcpy(student_complaints[i].comment, complaints[found].comment);
            break;
        }
    }

    stu_fp = fopen(stu_filename, "w");
    for (int i = 0; i < stu_count; i++) {
        fprintf(stu_fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                student_complaints[i].cmpID,
                student_complaints[i].title,
                student_complaints[i].description,
                student_complaints[i].team,
                student_complaints[i].status,
                student_complaints[i].date,
                student_complaints[i].time,
                student_complaints[i].comment,
                student_complaints[i].annonymous);
    }
    fclose(stu_fp);

    // ================= Update department file =================
    const char* deptCode;
    if (strcmp(complaints[found].team, "IT Section") == 0) deptCode = "IT";
    else if (strcmp(complaints[found].team, "Dept. Of CSE") == 0) deptCode = "CSE";
    else if (strcmp(complaints[found].team, "Dept. Of SWE") == 0) deptCode = "SWE";
    else if (strcmp(complaints[found].team, "BLC Support Team") == 0) deptCode = "BLC";
    else if (strcmp(complaints[found].team, "Finance and Accounts Team") == 0) deptCode = "FIN";
    else if (strcmp(complaints[found].team, "Transport Management Team") == 0) deptCode = "TRN";
    else if (strcmp(complaints[found].team, "Hall Authority") == 0) deptCode = "HALL";
    else deptCode = "MISC";

    char deptFile[128];
    sprintf(deptFile, "others_dept/complains/%s.txt", deptCode);

    // reload dept file
    struct Complain dept_complaints[200];
    int dept_count = 0;
    FILE *dept_fp = fopen(deptFile, "r");
    if (dept_fp) {
        while (fscanf(dept_fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                      dept_complaints[dept_count].cmpID,
                      dept_complaints[dept_count].studentID,
                      dept_complaints[dept_count].title,
                      dept_complaints[dept_count].team,
                      dept_complaints[dept_count].description,
                      dept_complaints[dept_count].status,
                      dept_complaints[dept_count].date,
                      dept_complaints[dept_count].time,
                      dept_complaints[dept_count].comment,
                      dept_complaints[dept_count].annonymous) != EOF) {
            dept_count++;
        }
        fclose(dept_fp);
    }

    for (int i = 0; i < dept_count; i++) {
        if (strcmp(dept_complaints[i].cmpID, cmp_id) == 0) {
            strcpy(dept_complaints[i].status, complaints[found].status);
            strcpy(dept_complaints[i].comment, complaints[found].comment);
            break;
        }
    }

    dept_fp = fopen(deptFile, "w");
    for (int i = 0; i < dept_count; i++) {
        fprintf(dept_fp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                dept_complaints[i].cmpID,
                dept_complaints[i].studentID,
                dept_complaints[i].title,
                dept_complaints[i].team,
                dept_complaints[i].description,
                dept_complaints[i].status,
                dept_complaints[i].date,
                dept_complaints[i].time,
                dept_complaints[i].comment,
                dept_complaints[i].annonymous);
    }
    fclose(dept_fp);

    // ================= Final success message =================
    printCenter("\nStatus and comment updated successfully!\n", 10);
    printCenter("Press any key to return...", 7);
    _getch();
}
// ================ END: update complaint status and comment ==================


void spacePrintAdmin(){
    int width = getConsoleWidth();
    for(int i=0; i<width/5; i++) printf(" ");
}
// ================== show complaint details by ID start ==================
void show_complain_details_by_id_admin(struct Complain complaints[], int count, char cmp_id[]) {
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        textColor(12);
        printCenter("Complaint not found.\n", 4);
        textColor(7);
        printCenter("Press any key to return...", 7);
        _getch();
        return;
    }

    system("cls");
    print_project_name();
    printCenter("Complaint Details\n", 11);

    textColor(14);
    printf("Complaint ID    : "); textColor(7); printf("%s\n", complaints[found].cmpID);
    textColor(14);
    printf("Student ID      : "); textColor(7); printf("%s\n", complaints[found].studentID);
    textColor(14);
    printf("Title           : "); textColor(7); printf("%s\n", complaints[found].title);
    textColor(14);
    printf("Team            : "); textColor(7); printf("%s\n", complaints[found].team);
    textColor(14);
    printf("Description     : "); textColor(7); printf("%s\n", complaints[found].description);
    textColor(14);
    printf("Status          : ");
    if (strcmp(complaints[found].status, "1") == 0) { textColor(12); printf("Pending\n"); }
    else if (strcmp(complaints[found].status, "2") == 0) { textColor(6); printf("In Progress\n"); }
    else if (strcmp(complaints[found].status, "3") == 0) { textColor(10); printf("Resolved\n"); }
    else { textColor(7); printf("Unknown\n"); }

    textColor(14);
    printf("Date            : "); textColor(7); printf("%s\n", complaints[found].date);
    textColor(14);
    printf("Time            : "); textColor(7); printf("%s\n", complaints[found].time);
    textColor(14);
    printf("Comment         : "); textColor(7); printf("%s\n", complaints[found].comment);
    textColor(14);
    printf("Anonymous       : "); textColor(7); printf("%s\n", complaints[found].annonymous);

    printf("\n");
    textColor(14);
    printf("Do you want to update this complaint? (Y/N): ");
    textColor(7);
    char confirm;
    scanf(" %c", &confirm);

    if (confirm == 'Y' || confirm == 'y') {
        update_complaint_status_and_comment(complaints, count, cmp_id);
    } else {
        printCenter("\nReturning to previous menu...", 6);
        _getch();
    }
}


// ================== filter complaints by department start ==================
void filter_complaints_by_department(struct Complain complaints[], int count) {
    char dept[30];

    system("cls");
    print_project_name();
    printCenter("Filter by Department\n", 11);
    printLeft("Enter Department Name (exact match): ", 2);
    getchar();
    fgets(dept, sizeof(dept), stdin);
    dept[strcspn(dept, "\n")] = '\0';

    system("cls");
    print_project_name();
    printf("Filtered by Department: %s\n\n", dept);
    spacePrintAdmin();
    printf("| %-15s | %-15s | %-25s | %-20s | %-15s | %-10s |\n",
           "Complain ID", "Student ID", "Title", "Team", "Status", "Date");
    spacePrintAdmin();
    for (int i = 0; i < 110; i++) printf("-");
    printf("\n");

    int hits = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].team, dept) == 0) {
            char shortTitle[26];
            if (strlen(complaints[i].title) > 23) {
                strncpy(shortTitle, complaints[i].title, 22);
                shortTitle[22] = '.'; shortTitle[23] = '.'; shortTitle[24] = '.'; shortTitle[25] = '\0';
            } else {
                strcpy(shortTitle, complaints[i].title);
            }

            spacePrintAdmin();
            printf("| %-15s | %-15s | %-25s | %-20s | ",
                   complaints[i].cmpID, complaints[i].studentID, shortTitle, complaints[i].team);

            if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-15s", "Pending"); }
            else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-15s", "In Progress"); }
            else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-15s", "Resolved"); }
            else { textColor(7); printf("%-15s", "Unknown"); }
            textColor(7);

            printf(" | %-10s |\n", complaints[i].date);
            hits++;
        }
    }

    if (!hits) {
        printCenter("\nNo complaints found for this department.\n", 4);
    }
    printCenter("\nPress any key to return to filters...", 7);
    _getch();
}
// ================== filter complaints by department end ==================


// ================== view all complain (admin) function start ==================
void view_all_complain_admin() {
    FILE *fp = fopen("admin/all_complain.txt", "r");
    if (fp == NULL) {
        printCenter("No complaints found.", 4);
        printf("\nPress any key to return...");
        _getch();
        return;
    }

    struct Complain complaints[500];
    int count = 0;
    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                  complaints[count].cmpID,
                  complaints[count].studentID,
                  complaints[count].title,
                  complaints[count].team,
                  complaints[count].description,
                  complaints[count].status,
                  complaints[count].date,
                  complaints[count].time,
                  complaints[count].comment,
                  complaints[count].annonymous) != EOF) {
        count++;
    }
    fclose(fp);

    int filter_choice;
    do {
        system("cls");
        print_project_name();
        printCenter("All Complaints (Admin Panel)\n", 10);
        printCenter("---------------------------------------------------\n", 9);

        spacePrintAdmin();
        printf("| %-17s | %-15s | %-30s | %-20s | %-12s | %-12s |\n",
               "Complain ID", "Student ID", "Title", "Department", "Status", "Date");
        spacePrintAdmin();
        for (int i = 0; i < 125; i++) printf("-");
        printf("\n");

        for (int i = 0; i < count; i++) {
            char shortTitle[31];
            if (strlen(complaints[i].title) > 28) {
                strncpy(shortTitle, complaints[i].title, 27);
                shortTitle[27] = '.'; shortTitle[28] = '.'; shortTitle[29] = '.'; shortTitle[30] = '\0';
            } else {
                strcpy(shortTitle, complaints[i].title);
            }

            spacePrintAdmin();
            printf("| %-15s | %-15s | %-30s | %-20s | ",
                   complaints[i].cmpID,
                   complaints[i].studentID,
                   shortTitle,
                   complaints[i].team);

            if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-12s", "Pending"); }
            else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-12s", "In Progress"); }
            else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-12s", "Resolved"); }
            else { textColor(7); printf("%-12s", "Unknown"); }
            textColor(7);

            printf(" | %-12s |\n", complaints[i].date);
        }

        printCenter("\nFilter Options:\n", 11);
        printCenter("1. Show Only Pending\n", 10);
        printCenter("2. Show Only In Progress\n", 10);
        printCenter("3. Show Only Resolved\n", 10);
        printCenter("4. Show Only Anonymous\n", 10);
        printCenter("5. Department Wise\n", 10);
        printCenter("6. View Full Complaint Details\n", 10);
        printCenter("0. Back\n", 10);
        printLeft("Enter your filter choice: ", 2);
        scanf("%d", &filter_choice);

        if (filter_choice >= 1 && filter_choice <= 4) {
            char status_filter[2] = "";
            int filter_anon = 0;

            if (filter_choice == 1) strcpy(status_filter, "1");
            else if (filter_choice == 2) strcpy(status_filter, "2");
            else if (filter_choice == 3) strcpy(status_filter, "3");
            else if (filter_choice == 4) filter_anon = 1;

            system("cls");
            print_project_name();
            if (filter_anon) printCenter("Filtered by: Anonymous Complaints\n", 11);
            else {
                if (strcmp(status_filter, "1") == 0) printCenter("Filtered by: Pending\n", 11);
                else if (strcmp(status_filter, "2") == 0) printCenter("Filtered by: In Progress\n", 11);
                else if (strcmp(status_filter, "3") == 0) printCenter("Filtered by: Resolved\n", 11);
            }

            spacePrintAdmin();
            printf("| %-15s | %-15s | %-30s | %-20s | %-12s | %-12s |\n",
                   "Complain ID", "Student ID", "Title", "Department", "Status", "Date");
            spacePrintAdmin();
            for (int i = 0; i < 120; i++) printf("-");
            printf("\n");

            int filtered = 0;
            for (int i = 0; i < count; i++) {
                int match = 0;
                if (filter_anon && strcmp(complaints[i].annonymous, "y") == 0) match = 1;
                else if (!filter_anon && strcmp(complaints[i].status, status_filter) == 0) match = 1;
                if (match) {
                    char shortTitle[31];
                    if (strlen(complaints[i].title) > 28) {
                        strncpy(shortTitle, complaints[i].title, 27);
                        shortTitle[27] = '.'; shortTitle[28] = '.'; shortTitle[29] = '.'; shortTitle[30] = '\0';
                    } else {
                        strcpy(shortTitle, complaints[i].title);
                    }

                    spacePrintAdmin();
                    printf("| %-15s | %-15s | %-30s | %-20s | ",
                           complaints[i].cmpID,
                           complaints[i].studentID,
                           shortTitle,
                           complaints[i].team);

                    if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-12s", "Pending"); }
                    else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-12s", "In Progress"); }
                    else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-12s", "Resolved"); }
                    else { textColor(7); printf("%-12s", "Unknown"); }
                    textColor(7);

                    printf(" | %-12s |\n", complaints[i].date);
                    filtered++;
                }
            }

            if (filtered == 0) {
                printCenter("\nNo complaints found for this filter.\n", 4);
            }
            printCenter("\nPress any key to return to filters...\n", 7);
            _getch();
        }
        else if (filter_choice == 5) {
            filter_complaints_by_department(complaints, count);
        }
        else if (filter_choice == 6) {
            char cmp_id[30];
            printLeft("\nEnter Complaint ID to view details: ", 2);
            getchar();
            fgets(cmp_id, sizeof(cmp_id), stdin);
            cmp_id[strcspn(cmp_id, "\n")] = '\0';
            show_complain_details_by_id_admin(complaints, count, cmp_id);
        }

    } while (filter_choice != 0);
}
// ================== view all complain (admin) function end ==================

// ================== Choice 2: View Complain by ID for Admin ==================
void view_complaint_by_id_admin() {
    FILE *fp = fopen("admin/all_complain.txt", "r");
    if (fp == NULL) {
        printCenter("No complaints found.\n", 4);
        printCenter("Press any key to return...", 7);
        _getch();
        return;
    }

    struct Complain complaints[200];
    int count = 0;

    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                  complaints[count].cmpID,
                  complaints[count].studentID,
                  complaints[count].title,
                  complaints[count].team,
                  complaints[count].description,
                  complaints[count].status,
                  complaints[count].date,
                  complaints[count].time,
                  complaints[count].comment,
                  complaints[count].annonymous) != EOF) {
        count++;
    }
    fclose(fp);

    char cmp_id[30];
    printLeft("\nEnter Complaint ID to view details: ", 2);
    getchar(); // clear newline
    fgets(cmp_id, sizeof(cmp_id), stdin);
    cmp_id[strcspn(cmp_id, "\n")] = '\0';

    show_complain_details_by_id_admin(complaints, count, cmp_id);
}

// ================== Choice 3: Add Admin ==================
void create_admin_account() {
    system("cls");
    print_project_name();
    printCenter("Create Admin Account\n", 11);

    struct Admin new_admin;
    int last_id = 0;

    // Detect the highest admin ID
    FILE *read_fp = fopen("admin/admin_account.txt", "r");
    if (read_fp != NULL) {
        while (fscanf(read_fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                      new_admin.name, new_admin.adminID, new_admin.email, new_admin.phone, new_admin.username, new_admin.password) == 6) {

            // Extract the number part of admin ID
            int current_id = 0;
            if (sscanf(new_admin.adminID, "Admin_%d", &current_id) == 1) {
                if (current_id > last_id) {
                    last_id = current_id;
                }
            }
        }
        fclose(read_fp);
    }

    // Assign new ID
    sprintf(new_admin.adminID, "Admin_%03d", last_id + 1);

    // Get admin details
    textColor(14); printf("Name         : "); textColor(7);
    getchar();
    fgets(new_admin.name, sizeof(new_admin.name), stdin);
    new_admin.name[strcspn(new_admin.name, "\n")] = '\0';

    textColor(14); printf("Email        : "); textColor(7);
    fgets(new_admin.email, sizeof(new_admin.email), stdin);
    new_admin.email[strcspn(new_admin.email, "\n")] = '\0';

    textColor(14); printf("Phone Number : "); textColor(7);
    fgets(new_admin.phone, sizeof(new_admin.phone), stdin);
    new_admin.phone[strcspn(new_admin.phone, "\n")] = '\0';

    while (1) {
        textColor(14); printf("Username     : "); textColor(7);
        fgets(new_admin.username, sizeof(new_admin.username), stdin);
        new_admin.username[strcspn(new_admin.username, "\n")] = '\0';

        // Check if username already exists
        int is_duplicate = 0;
        FILE *check_fp = fopen("admin/admin_account.txt", "r");
        if (check_fp != NULL) {
            char temp_name[100], temp_id[20], temp_email[100], temp_phone[20], temp_username[30], temp_pass[30];
            while (fscanf(check_fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                          temp_name, temp_id, temp_email, temp_phone, temp_username, temp_pass) == 6) {
                if (strcmp(temp_username, new_admin.username) == 0) {
                    is_duplicate = 1;
                    break;
                }
            }
            fclose(check_fp);
        }

        if (is_duplicate) {
            textColor(12);
            printf("Username already exists. Please choose another one.\n");
            textColor(7);
        } else {
            break;
        }
    }

    textColor(14); printf("Password     : "); textColor(7);
    fgets(new_admin.password, sizeof(new_admin.password), stdin);
    new_admin.password[strcspn(new_admin.password, "\n")] = '\0';

    password_encrypt(new_admin.password);

    FILE *fp = fopen("admin/admin_account.txt", "a");
    if (fp == NULL) {
        textColor(12);
        printCenter("Failed to open admin_account.txt\n", 12);
        textColor(7);
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
            new_admin.name,
            new_admin.adminID,
            new_admin.email,
            new_admin.phone,
            new_admin.username,
            new_admin.password);

    fclose(fp);

    textColor(10);
    printCenter("\nAdmin account created successfully!\n", 10);
    textColor(14); printf("Generated Admin ID: "); textColor(11); printf("%s\n", new_admin.adminID); textColor(7);
    printCenter("Press any key to return...", 7);
    _getch();
}



void view_student_details_with_id() {
    system("cls");
    print_project_name();
    printCenter("View Student Details by ID\n", 11);
    printCenter("---------------------------------------------------\n", 9);

    char id[20];
    textColor(14);
    printf("Enter Student ID to view details: ");
    textColor(7);
    scanf("%s", id);

    int found = binary_search_on_students_data(id);
    if (found == -1) {
        textColor(12); // Red for error
        printCenter("Student not found.\n", 12);
        textColor(7);
        printCenter("Press any key to return...", 7);
        _getch();
        return;
    }

    struct Student s = students_data[found];

    // Count complaints
    int complaint_count = 0;
    char filename[100], buffer[500];
    sprintf(filename, "students_complain/%s.txt", id);
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strcmp(buffer, "\n") == 0)
                complaint_count++;
        }
        fclose(fp);
    }

    // Header
    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    spacePrintProfile(); textColor(11);
    printf("|%-25s", "");
    textColor(10);
    printf("PROFILE INFORMATION");
    textColor(11); printf("%-24s|\n", "");

    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    // Profile details
    spacePrintProfile(); textColor(14); printf("| %-20s", "Student ID");     
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.id);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Name");           
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.name);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Section");        
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.section);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Department");     
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.dept);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Mobile Number");  
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.mobile);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Email");          
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.email);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Date of Birth");  
    textColor(15); printf(" : "); textColor(11); printf("%-43s |\n", s.dob);

    spacePrintProfile(); textColor(14); printf("| %-20s", "Total Complaints");  
    textColor(15); printf(" : "); textColor(11); printf("%-43d |\n", complaint_count);

    spacePrintProfile(); textColor(11);
    for (int i = 0; i < 70; i++) printf("=");
    printf("\n");

    textColor(10); 
    printCenter("\nPress any key to return to the dashboard...", 7);
    textColor(7);
    _getch();
}


void create_department_user() {
    system("cls");
    print_project_name();
    printCenter("Create Department User\n", 11);

    // Show available department codes
    textColor(14);
    printf("\nAvailable Department Codes:\n");
    printf(" IT    - IT Section\n");
    printf(" CSE   - Dept. Of CSE\n");
    printf(" SWE   - Dept. Of SWE\n");
    printf(" BLC   - BLC Support Team\n");
    printf(" FIN   - Finance and Accounts Team\n");
    printf(" TRN   - Transport Management Team\n");
    printf(" HALL  - Hall Authority\n");
    printf(" SA    - Student Affairs\n");
    textColor(7);

    // Get department code input
    char deptCode[10], fileName[100];
    textColor(14);
    printf("\nEnter Department Code (e.g., IT, CSE, SWE): ");
    textColor(7);
    scanf("%s", deptCode);

    // Build file path
    sprintf(fileName, "others_dept/login_info/%s.txt", deptCode);

    //  Count existing users and check username uniqueness
    int userCount = 0;
    char existing_username[30], buffer[300];
    FILE *read_fp = fopen(fileName, "r");
    if (read_fp == NULL) {
        // Create file if it doesn't exist
        read_fp = fopen(fileName, "w");
        if (read_fp == NULL) {
            textColor(12);
            printf("Could not create department file.\n");
            textColor(7);
            return;
        }
        fclose(read_fp);
        read_fp = fopen(fileName, "r");
    }

    char existing_line[300];
    char existing_name[50], existing_id[20], existing_role[50], existing_email[50], existing_mobile[20];
    char username_check[30];
    getchar(); // flush newline

    //Take input except password and username first
    char name[50], role[50], email[50], mobile[20], username[30], password[30];

    textColor(14); printf("Full Name     : "); textColor(7); fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = '\0';
    textColor(14); printf("Role          : "); textColor(7); fgets(role, sizeof(role), stdin); role[strcspn(role, "\n")] = '\0';
    textColor(14); printf("Email         : "); textColor(7); fgets(email, sizeof(email), stdin); email[strcspn(email, "\n")] = '\0';
    textColor(14); printf("Mobile        : "); textColor(7); fgets(mobile, sizeof(mobile), stdin); mobile[strcspn(mobile, "\n")] = '\0';

    // Check for duplicate username
    int is_duplicate = 0;
    do {
        is_duplicate = 0;
        textColor(14); printf("Username      : "); textColor(7);
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = '\0';

        rewind(read_fp);
        while (fgets(existing_line, sizeof(existing_line), read_fp)) {
            if (sscanf(existing_line, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s", 
                existing_name, existing_id, existing_role, existing_email, existing_mobile, username_check, password) == 7) {
                if (strcmp(username, username_check) == 0) {
                    textColor(12);
                    printf("Username already exists! Please enter a different one.\n");
                    textColor(7);
                    is_duplicate = 1;
                    break;
                }
            }
        }
    } while (is_duplicate);

    fclose(read_fp);

    // Get and encrypt password
    textColor(14); printf("Password      : "); textColor(7);
    input_hidden_password(password);
    password_encrypt(password);

    // Generate User ID
    sprintf(fileName, "others_dept/login_info/%s.txt", deptCode);
    read_fp = fopen(fileName, "r");
    userCount = 0;

    char userId[20];
    sprintf(userId, "%s%03d", deptCode, userCount + 1);

    // Write new user to file
    FILE *write_fp = fopen(fileName, "a");
    if (write_fp == NULL) {
        textColor(12);
        printCenter("\nError: Could not open the department login file for writing.\n", 12);
        textColor(7);
        return;
    }

    fprintf(write_fp, "%s|%s|%s|%s|%s|%s|%s\n", name, userId, role, email, mobile, username, password);
    fclose(write_fp);

    // Success message
    textColor(10);
    printCenter("\nDepartment user created successfully!\n", 10);
    textColor(14); printf("Generated User ID: "); textColor(11); printf("%s\n", userId);
    textColor(7);

    printCenter("Press any key to return...", 7);
    _getch();
}





// ================== admin logout ==================
void admin_logout() {
    system("cls");
    print_project_name();
    printCenter("Admin Logout\n", 11);
    printCenter("---------------------------------------------------\n", 9);

    textColor(10);
    Sleep(3000);
    printCenter("You have been successfully logged out.\n", 10);
    textColor(7);

    printCenter("Press any key to return to the main menu...", 7);
    _getch();
}



// ========== ADMIN DASHBOARD ==========
void admin_dashboard() {
    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("Admin Dashboard\n", 10);
        printCenter("----------------------------------------------\n", 9);
        printCenter("1. View All Complaints (With Filter)\n", 11);
        printCenter("2. Track Complaint by ID\n", 11);
        printCenter("3. Create Admin Account\n", 11);
        printCenter("4. Create Department User\n", 11);
        printCenter("5. View Student Details with Id\n", 11);
        printCenter("6. Logout\n", 11);
        printLeft("Enter your choice: ", 2);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_all_complain_admin();
                break;
            case 2:
                view_complaint_by_id_admin();
                break;
            case 3:
                create_admin_account();
                break;
            case 4:
                create_department_user();
                break;
            case 5:
                view_student_details_with_id();
                break;
            case 6:
                admin_logout();
                return;
        }
    } while (1);
}

// admin utitily functions 
void load_admins() {
    admin_count = 0;
    FILE *fp = fopen("admin/admin_account.txt", "r");
    if (fp == NULL) {
        printf("Admin account file not found!\n");
        return;
    }

    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  admins[admin_count].name,
                  admins[admin_count].adminID,
                  admins[admin_count].email,
                  admins[admin_count].phone,
                  admins[admin_count].username,
                  admins[admin_count].password) != EOF) {

        // Decrypt password before storing
        password_decrypt(admins[admin_count].password);
        admin_count++;
        if (admin_count >= 100) break; // for overflow
    }

    fclose(fp);
}

// ========== ADMIN LOGIN ==========
void admin_login() {
    system("cls");
    print_project_name();
    printCenter("Admin Login\n", 12);
    printCenter("---------------------------------------------------\n", 9);

    char username[30], password[30];

    printLeft("Enter Username: ", 15);
    scanf("%s", username);

    printLeft("Enter Password: ", 15);
    input_hidden_password(password);

    load_admins();

    for (int i = 0; i < admin_count; i++) {
        if (strcmp(admins[i].username, username) == 0 && strcmp(admins[i].password, password) == 0) {
            strcpy(current_logged_admin, username);
            printCenter("Login successful! Redirecting to dashboard...\n", 10);
            Sleep(2000);
            admin_dashboard(); 
            return;
        }
    }

    printCenter("Invalid credentials! Try again...\n", 12);
    Sleep(2000);
}

// Map short code to the full "Team" string used in files
const char* deptCode_to_full(const char *code) {
    if (!code) return "MISC";
    if (strcmp(code,"IT")==0)   return "IT Section";
    if (strcmp(code,"CSE")==0)  return "Dept. Of CSE";
    if (strcmp(code,"SWE")==0)  return "Dept. Of SWE";
    if (strcmp(code,"BLC")==0)  return "BLC Support Team";
    if (strcmp(code,"FIN")==0)  return "Finance and Accounts Team";
    if (strcmp(code,"TRN")==0)  return "Transport Management Team";
    if (strcmp(code,"HALL")==0) return "Hall Authority";
    if (strcmp(code,"SA")==0)   return "Student Affairs";
    return "MISC";
}


void department_logout() {
    system("cls");
    print_project_name();
    printCenter("Department Logout\n", 11);
    printCenter("---------------------------------------------------\n", 9);
    strcpy(current_logged_dept_code, "");
    strcpy(current_logged_dept_user, "");
    textColor(10);
    printCenter("You have been successfully logged out.\n", 10);
    textColor(7);
    printCenter("Press any key to return to the main menu...", 7);
    _getch();
}

void dept_show_table_header(int showStudentCol) {
    spacePrintAdmin();
    if (showStudentCol)
        printf("| %-17s | %-15s | %-30s | %-20s | %-12s | %-12s |\n",
               "Complain ID", "Student ID", "Title", "Department", "Status", "Date");
    else
        printf("| %-17s | %-30s | %-20s | %-12s | %-12s |\n",
               "Complain ID", "Title", "Department", "Status", "Date");
    spacePrintAdmin();
    for (int i = 0; i < (showStudentCol ? 125 : 105); i++) printf("-");
    printf("\n");
}


// NOTE: Dept reads only its own department file others_dept/complains/<CODE>.txt
void dept_view_all_complaints(const char *dept_code) {
    char deptFile[128];
    sprintf(deptFile, "others_dept/complains/%s.txt", dept_code);

    FILE *fp = fopen(deptFile, "r");
    if (fp == NULL) {
        printCenter("No complaints found for your department.", 4);
        printf("\nPress any key to return...");
        _getch();
        return;
    }

    struct Complain complaints[500];
    int count = 0;
    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                  complaints[count].cmpID,
                  complaints[count].studentID,
                  complaints[count].title,
                  complaints[count].team,
                  complaints[count].description,
                  complaints[count].status,
                  complaints[count].date,
                  complaints[count].time,
                  complaints[count].comment,
                  complaints[count].annonymous) != EOF) {
        count++;
        if (count >= 500) break;
    }
    fclose(fp);

    int filter_choice;
    do {
        system("cls");
        print_project_name();
        printCenter("All Complaints (Department Panel)\n", 10);
        printCenter("---------------------------------------------------\n", 9);

        // In department view: hide Student ID if anonymous
        dept_show_table_header(/*showStudentCol=*/1);

        for (int i = 0; i < count; i++) {
            char shortTitle[31];
            if (strlen(complaints[i].title) > 28) {
                strncpy(shortTitle, complaints[i].title, 27);
                shortTitle[27] = '.'; shortTitle[28] = '.'; shortTitle[29] = '.'; shortTitle[30] = '\0';
            } else strcpy(shortTitle, complaints[i].title);

            spacePrintAdmin();

            // If anonymous, do NOT show student id (print "Hidden")
            const char *studentShown = (strcmp(complaints[i].annonymous,"y")==0) ? "Hidden" : complaints[i].studentID;

            printf("| %-15s | %-15s | %-30s | %-20s | ",
                   complaints[i].cmpID, studentShown, shortTitle, complaints[i].team);

            if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-12s", "Pending"); }
            else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-12s", "In Progress"); }
            else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-12s", "Resolved"); }
            else { textColor(7); printf("%-12s", "Unknown"); }
            textColor(7);

            printf(" | %-12s |\n", complaints[i].date);
        }

        printCenter("\nFilter Options:\n", 11);
        printCenter("1. Show Only Pending\n", 10);
        printCenter("2. Show Only In Progress\n", 10);
        printCenter("3. Show Only Resolved\n", 10);
        printCenter("4. Show Only Anonymous\n", 10);
        printCenter("5. View Full Complaint Details (and update)\n", 10);
        printCenter("0. Back\n", 10);
        printLeft("Enter your filter choice: ", 2);
        scanf("%d", &filter_choice);

        if (filter_choice >= 1 && filter_choice <= 4) {
            char status_filter[2] = "";
            int filter_anon = 0;
            if (filter_choice == 1) strcpy(status_filter,"1");
            else if (filter_choice == 2) strcpy(status_filter,"2");
            else if (filter_choice == 3) strcpy(status_filter,"3");
            else if (filter_choice == 4) filter_anon = 1;

            system("cls");
            print_project_name();
            if (filter_anon) printCenter("Filtered by: Anonymous Complaints\n", 11);
            else {
                if (strcmp(status_filter,"1")==0) printCenter("Filtered by: Pending\n", 11);
                else if (strcmp(status_filter,"2")==0) printCenter("Filtered by: In Progress\n", 11);
                else if (strcmp(status_filter,"3")==0) printCenter("Filtered by: Resolved\n", 11);
            }
            dept_show_table_header(/*showStudentCol=*/1);

            int filtered=0;
            for (int i = 0; i < count; i++) {
                int match = 0;
                if (filter_anon && strcmp(complaints[i].annonymous,"y")==0) match = 1;
                else if (!filter_anon && strcmp(complaints[i].status, status_filter)==0) match = 1;

                if (match) {
                    char shortTitle[31];
                    if (strlen(complaints[i].title) > 28) {
                        strncpy(shortTitle, complaints[i].title, 27);
                        shortTitle[27]='.'; shortTitle[28]='.'; shortTitle[29]='.'; shortTitle[30]='\0';
                    } else strcpy(shortTitle, complaints[i].title);

                    const char *studentShown = (strcmp(complaints[i].annonymous,"y")==0) ? "Hidden" : complaints[i].studentID;

                    spacePrintAdmin();
                    printf("| %-15s | %-15s | %-30s | %-20s | ",
                           complaints[i].cmpID, studentShown, shortTitle, complaints[i].team);

                    if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("%-12s","Pending"); }
                    else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("%-12s","In Progress"); }
                    else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("%-12s","Resolved"); }
                    else { textColor(7); printf("%-12s","Unknown"); }
                    textColor(7);

                    printf(" | %-12s |\n", complaints[i].date);
                    filtered++;
                }
            }
            if (!filtered) printCenter("\nNo complaints found for this filter.\n", 4);
            printCenter("\nPress any key to return...", 7);
            _getch();
        } else if (filter_choice == 5) {
            char cmp_id[30];
            printLeft("\nEnter Complaint ID to view details: ", 2);
            getchar();
            fgets(cmp_id, sizeof(cmp_id), stdin);
            cmp_id[strcspn(cmp_id, "\n")] = '\0';
            dept_show_complaint_details(complaints, count, dept_code, cmp_id);
        }
    } while (filter_choice != 0);
}

void dept_show_complaint_details(struct Complain complaints[], int count, const char *dept_code, char cmp_id[]) {
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) { found = i; break; }
    }

    if (found == -1) {
        textColor(12); printCenter("Complaint not found.\n", 12);
        textColor(7);  printCenter("Press any key to return...", 7); _getch();
        return;
    }

    // Safety: ensure it's really this dept's complaint
    const char *deptFull = deptCode_to_full(dept_code);
    if (strcmp(complaints[found].team, deptFull) != 0) {
        textColor(12); printCenter("Access denied for this complaint.\n", 12);
        textColor(7);  printCenter("Press any key to return...", 7); _getch();
        return;
    }

    system("cls");
    print_project_name();
    printCenter("Complaint Details (Department)\n", 11);

    textColor(14); printf("Complaint ID    : "); textColor(7); printf("%s\n", complaints[found].cmpID);

    // Hide student id if anonymous
    textColor(14); printf("Student ID      : "); textColor(7);
    if (strcmp(complaints[found].annonymous, "y") == 0) printf("Hidden\n");
    else printf("%s\n", complaints[found].studentID);

    textColor(14); printf("Title           : "); textColor(7); printf("%s\n", complaints[found].title);
    textColor(14); printf("Department      : "); textColor(7); printf("%s\n", complaints[found].team);
    textColor(14); printf("Description     : "); textColor(7); printf("%s\n", complaints[found].description);

    textColor(14); printf("Status          : ");
    if      (strcmp(complaints[found].status, "1") == 0) { textColor(12); printf("Pending\n"); }
    else if (strcmp(complaints[found].status, "2") == 0) { textColor(6);  printf("In Progress\n"); }
    else if (strcmp(complaints[found].status, "3") == 0) { textColor(10); printf("Resolved\n"); }
    else { textColor(7); printf("Unknown\n"); }
    textColor(7);

    textColor(14); printf("Date            : "); textColor(7); printf("%s\n", complaints[found].date);
    textColor(14); printf("Time            : "); textColor(7); printf("%s\n", complaints[found].time);
    textColor(14); printf("Comment         : "); textColor(7); printf("%s\n", complaints[found].comment);
    textColor(14); printf("Anonymous       : "); textColor(7); printf("%s\n", complaints[found].annonymous);

    printf("\n");
    textColor(14); printf("Do you want to update this complaint? (Y/N): "); textColor(7);
    char confirm;
    scanf(" %c", &confirm);  // matches your admin flow

    if (confirm == 'Y' || confirm == 'y') {
        dept_update_status_and_comment(complaints, count, dept_code, cmp_id);
    } else {
        printCenter("\nReturning...", 6);
        _getch();
    }
}


// Update status/comment from department side (writes admin file, student file, and dept file)
// Update status/comment from department side (writes admin file, student file, and dept file)
void dept_update_status_and_comment(struct Complain complaints[], int count, const char dept_code[], char cmp_id[]) {
    // Find index
    int found = -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) { found = i; break; }
    }
    if (found == -1) { printCenter("Complaint not found.\n", 4); printCenter("Press any key to return...", 7); _getch(); return; }

    system("cls");
    print_project_name();
    printCenter("Update Complaint Status & Comment (Dept)\n", 11);

    printf("Complaint ID    : %s\n", complaints[found].cmpID);
    printf("Title           : %s\n", complaints[found].title);
    printf("Department      : %s\n", complaints[found].team);
    printf("Current Status  : %s\n", complaints[found].status);
    printf("Current Comment : %s\n\n", complaints[found].comment);

    printf("Select New Status:\n1. Pending\n2. In Progress\n3. Resolved\nEnter choice: ");
    int status_choice; 
    scanf("%d", &status_choice);
    getchar();  // eat newline

    if (status_choice >= 1 && status_choice <= 3) {
        sprintf(complaints[found].status, "%d", status_choice);
    }

    char new_comment[100];
    printf("Enter Comment (max 100 chars): ");
    fgets(new_comment, sizeof(new_comment), stdin);
    new_comment[strcspn(new_comment, "\n")] = '\0';
    strcpy(complaints[found].comment, new_comment);

    // === 1) Update admin/all_complain.txt (10 fields) ===
    FILE *afp = fopen("admin/all_complain.txt", "r");
    struct Complain adminCs[500]; int ac = 0;
    if (afp) {
        while (fscanf(afp,
                      "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                      adminCs[ac].cmpID, adminCs[ac].studentID, adminCs[ac].title, adminCs[ac].team,
                      adminCs[ac].description, adminCs[ac].status, adminCs[ac].date, adminCs[ac].time,
                      adminCs[ac].comment, adminCs[ac].annonymous) != EOF) {
            ac++; if (ac >= 500) break;
        }
        fclose(afp);
    }
    for (int i = 0; i < ac; i++) {
        if (strcmp(adminCs[i].cmpID, complaints[found].cmpID) == 0) {
            strcpy(adminCs[i].status,  complaints[found].status);
            strcpy(adminCs[i].comment, complaints[found].comment);
            break;
        }
    }
    afp = fopen("admin/all_complain.txt", "w");
    if (afp) {
        for (int i = 0; i < ac; i++) {
            fprintf(afp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                    adminCs[i].cmpID, adminCs[i].studentID, adminCs[i].title, adminCs[i].team,
                    adminCs[i].description, adminCs[i].status, adminCs[i].date, adminCs[i].time,
                    adminCs[i].comment, adminCs[i].annonymous);
        }
        fclose(afp);
    }

    // === 2) Update student's file (9 fields, NO studentID) ===
    char stu_filename[128];
    sprintf(stu_filename, "students_complain/%s.txt", complaints[found].studentID);
    struct Complain sC[300]; int scnt = 0;
    FILE *sfp = fopen(stu_filename, "r");
    if (sfp) {
        while (fscanf(sfp,
                      "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                      sC[scnt].cmpID, sC[scnt].title, sC[scnt].description, sC[scnt].team,
                      sC[scnt].status, sC[scnt].date, sC[scnt].time, sC[scnt].comment, sC[scnt].annonymous) != EOF) {
            scnt++; if (scnt >= 300) break;
        }
        fclose(sfp);
    }
    for (int i = 0; i < scnt; i++) {
        if (strcmp(sC[i].cmpID, complaints[found].cmpID) == 0) {
            strcpy(sC[i].status,  complaints[found].status);
            strcpy(sC[i].comment, complaints[found].comment);
            break;
        }
    }
    sfp = fopen(stu_filename, "w");
    if (sfp) {
        for (int i = 0; i < scnt; i++) {
            fprintf(sfp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                    sC[i].cmpID, sC[i].title, sC[i].description, sC[i].team,
                    sC[i].status, sC[i].date, sC[i].time, sC[i].comment, sC[i].annonymous);
        }
        fclose(sfp);
    }

    // === 3) Update department file (10 fields, like admin) ===
    char deptFile[128];
    sprintf(deptFile, "others_dept/complains/%s.txt", dept_code);
    struct Complain dC[500]; int dc = 0;
    FILE *dfp = fopen(deptFile, "r");
    if (dfp) {
        while (fscanf(dfp,
                      "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                      dC[dc].cmpID, dC[dc].studentID, dC[dc].title, dC[dc].team,
                      dC[dc].description, dC[dc].status, dC[dc].date, dC[dc].time,
                      dC[dc].comment, dC[dc].annonymous) != EOF) {
            dc++; if (dc >= 500) break;
        }
        fclose(dfp);
    }
    for (int i = 0; i < dc; i++) {
        if (strcmp(dC[i].cmpID, complaints[found].cmpID) == 0) {
            strcpy(dC[i].status,  complaints[found].status);
            strcpy(dC[i].comment, complaints[found].comment);
            break;
        }
    }
    dfp = fopen(deptFile, "w");
    if (dfp) {
        for (int i = 0; i < dc; i++) {
            fprintf(dfp, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",
                    dC[i].cmpID, dC[i].studentID, dC[i].title, dC[i].team,
                    dC[i].description, dC[i].status, dC[i].date, dC[i].time,
                    dC[i].comment, dC[i].annonymous);
        }
        fclose(dfp);
    }

    printCenter("\nStatus and comment updated successfully!\n", 10);
    printCenter("Press any key to return...", 7);
    _getch();
}



void dept_track_by_id(const char *dept_code) {
    // open dept file
    char deptFile[128];
    sprintf(deptFile,"others_dept/complains/%s.txt", dept_code);
    FILE *fp = fopen(deptFile,"r");
    if (!fp){
        printCenter("No complaints found for this department.\n",4);
        printCenter("Press any key to return...",7); _getch(); return;
    }
    struct Complain complaints[300]; int count=0;
    while (fscanf(fp, "%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n%[^\n]\n\n",
                  complaints[count].cmpID, complaints[count].studentID, complaints[count].title, complaints[count].team,
                  complaints[count].description, complaints[count].status, complaints[count].date, complaints[count].time,
                  complaints[count].comment, complaints[count].annonymous) != EOF){
        count++; if (count>=300) break;
    }
    fclose(fp);

    char cmp_id[30];
    printLeft("Enter Complaint ID: ", 10);
    scanf("%s", cmp_id);
    dept_show_complaint_details(complaints, count, dept_code, cmp_id);
}

void dept_add_team_user(const char *dept_code) {
    // Same logic as create_department_user, but scoped to this dept
    system("cls");
    print_project_name();
    printCenter("Add Team User (Department)\n", 11);

    char fileName[128];
    sprintf(fileName,"others_dept/login_info/%s.txt", dept_code);

    FILE *read_fp = fopen(fileName,"r");
    if (!read_fp){
        // create if not exists
        read_fp = fopen(fileName,"w");
        if (!read_fp){
            textColor(12); printCenter("Could not create department file.\n",12); textColor(7); return;
        }
        fclose(read_fp);
        read_fp = fopen(fileName,"r");
    }

    char name[50], role[50], email[50], mobile[20], username[30], password[30];
    char existing_line[300];
    char existing_name[50], existing_id[20], existing_role[50], existing_email[50], existing_mobile[20];
    char username_check[30];

    getchar();
    textColor(14); printf("Full Name     : "); textColor(7); fgets(name,sizeof(name),stdin); name[strcspn(name,"\n")]='\0';
    textColor(14); printf("Role          : "); textColor(7); fgets(role,sizeof(role),stdin); role[strcspn(role,"\n")]='\0';
    textColor(14); printf("Email         : "); textColor(7); fgets(email,sizeof(email),stdin); email[strcspn(email,"\n")]='\0';
    textColor(14); printf("Mobile        : "); textColor(7); fgets(mobile,sizeof(mobile),stdin); mobile[strcspn(mobile,"\n")]='\0';

    int is_duplicate=0;
    do{
        is_duplicate=0;
        textColor(14); printf("Username      : "); textColor(7);
        fgets(username,sizeof(username),stdin); username[strcspn(username,"\n")]='\0';
        rewind(read_fp);
        while (fgets(existing_line,sizeof(existing_line),read_fp)){
            if (sscanf(existing_line,"%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%s",
                       existing_name, existing_id, existing_role, existing_email, existing_mobile, username_check, password)==7) {
                if (strcmp(username, username_check)==0) {
                    textColor(12); printf("Username already exists! Please enter a different one.\n"); textColor(7);
                    is_duplicate=1; break;
                }
            }
        }
    } while (is_duplicate);
    fclose(read_fp);

    textColor(14); printf("Password      : "); textColor(7);
    input_hidden_password(password);
    password_encrypt(password);

    // Generate simple ID (append count)
    FILE *cntfp = fopen(fileName,"r");
    int cnt=0; char dummy[400];
    if (cntfp){
        while (fgets(dummy,sizeof(dummy),cntfp)) { if (dummy[0]!='\n' && dummy[0]!=0) cnt++; }
        fclose(cntfp);
    }
    char userId[20]; sprintf(userId,"%s%03d", dept_code, (cnt/1) + 1);

    FILE *write_fp = fopen(fileName,"a");
    if (!write_fp){
        textColor(12); printCenter("Error opening department login file for writing.\n",12); textColor(7); return;
    }
    fprintf(write_fp,"%s|%s|%s|%s|%s|%s|%s\n", name, userId, role, email, mobile, username, password);
    fclose(write_fp);

    textColor(10); printCenter("\nTeam user created successfully!\n",10);
    textColor(14); printf("Generated User ID: "); textColor(11); printf("%s\n", userId); textColor(7);
    printCenter("Press any key to return...", 7); _getch();
}

void dept_view_student_profile_with_id() {
    // Reuse existing my_profile / view_student_details_with_id logic
    view_student_details_with_id();
}

void department_dashboard(const char *dept_code) {
    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("Department Dashboard\n", 10);
        printCenter("---------------------------------------------------\n", 9);
        textColor(14); printf("Logged in Dept: %s (%s)\n\n", dept_code, deptCode_to_full(dept_code)); textColor(7);

        printCenter("1. View All Complaints (With Filter)\n", 11);
        printCenter("2. Track Complaint by ID\n", 11);
        printCenter("3. Add Team User\n", 11);
        printCenter("4. View Student Profile (by ID)\n", 11);
        printCenter("5. Logout\n", 11);
        printLeft("Enter your choice: ", 2);

        scanf("%d",&choice);
        switch (choice) {
            case 1: dept_view_all_complaints(dept_code); break;
            case 2: dept_track_by_id(dept_code); break;
            case 3: dept_add_team_user(dept_code); break;
            case 4: dept_view_student_profile_with_id(); break;
            case 5: department_logout(); return;
            default: textColor(12); printf("Invalid choice! Try again.\n"); textColor(7); Sleep(1500);
        }
    } while (1);
}






// ======================= Others Dept ==========================
void department_login(const char *dept_code) {
    system("cls");
    print_project_name();

    char file_path[100];
    sprintf(file_path, "others_dept/login_info/%s.txt", dept_code);

    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        textColor(12);
        printCenter("Error: Department login file not found!\n", 12);
        textColor(7);
        _getch();
        return;
    }

    char username[30], password[30];
    char stored_name[50], stored_id[20], stored_role[50], stored_email[50], stored_mobile[20];
    char stored_username[30], stored_password[30];

    textColor(14); printf("Enter Username: "); textColor(7);
    scanf("%s", username);
    textColor(14); printf("Enter Password: "); textColor(7);
    input_hidden_password(password);

    int success = 0;
    while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  stored_name, stored_id, stored_role, stored_email, stored_mobile,
                  stored_username, stored_password) == 7) {

        password_decrypt(stored_password);

        if (strcmp(username, stored_username) == 0 && strcmp(password, stored_password) == 0) {
            success = 1;
            break;
        }
    }
    fclose(fp);

    if (success) {
        textColor(10);
        printCenter("\nLogin Successful! Redirecting to Department Dashboard...\n", 10);
        textColor(7);
        Sleep(2000);
        department_dashboard(dept_code);
    } else {
        textColor(12);
        printCenter("\nInvalid Username or Password. Please try again.\n", 12);
        textColor(7);
        Sleep(2000);
    }
}

void spacePrint1(){
    int width = getConsoleWidth();
    for(int i=0; i<width/4; i++) printf(" ");
}

// ========== Main function start ==========
int main() {
    int choice;
    // Sleep(4000);
    do {
        system("cls");
        print_project_name();
        
        spacePrint1(); puts(".----------------.  .----------------.  .----------------.  .----------------.  .----------------. ");
        spacePrint1(); puts("| .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |");
        spacePrint1();puts("| |  ________    | || |     _____    | || | _____  _____ | || |     ______   | || |   ______     | |");
        spacePrint1();puts("| | |_   ___ `.  | || |    |_   _|   | || ||_   _||_   _|| || |   .' ___  |  | || |  |_   _ \\    | |");
        spacePrint1();puts("| |   | |   `. \\ | || |      | |     | || |  | |    | |  | || |  / .'   \\_|  | || |    | |_) |   | |");
        spacePrint1();puts("| |   | |    | | | || |      | |     | || |  | '    ' |  | || |  | |         | || |    |  __'.   | |");
        spacePrint1();puts("| |  _| |___.' / | || |     _| |_    | || |   \\ `--' /   | || |  \\ `.___.'\\  | || |   _| |__) |  | |");
        spacePrint1();puts("| | |________.'  | || |    |_____|   | || |    `.__.'    | || |   `._____.'  | || |  |_______/   | |");
        spacePrint1();puts("| |              | || |              | || |              | || |              | || |              | |");
        spacePrint1();puts("| '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |");
        spacePrint1();puts(" '----------------'  '----------------'  '----------------'  '----------------'  '----------------' ");
        printCenter(" 1. Student Login\n", 3);
        printCenter(" 2. Admin Login\n", 3);
        printCenter(" 3. Other Department Login\n", 3);
        printCenter(" 0. EXIT...\n", 4);

        textColor(14);
        printf("\nEnter Choice: ");
        textColor(7);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                student_login_reg();
                break;
            case 2:
                admin_login();
                break;
            case 3: {
                int sub_choice;
                do {
                    system("cls");
                    print_project_name();
                    printCenter("Select Department Login\n", 11);
                    printCenter(" 01. Hall Authority            \n", 11);
                    printCenter(" 02. IT Section                \n", 11);
                    printCenter(" 03. Dept. Of CSE              \n", 11);
                    printCenter(" 04. Dept. Of SWE              \n", 11);
                    printCenter(" 05. BLC Support Team          \n", 11);
                    printCenter(" 06. Finance and Accounts Team\n", 11);
                    printCenter(" 07. Transport Management Team\n", 11);
                    printCenter(" 0. Back\n", 11);

                    textColor(14);
                    printf("\nEnter Department Choice: ");
                    textColor(7);
                    scanf("%d", &sub_choice);

                    switch (sub_choice) {
                        case 1:
                            department_login("HALL"); break;
                        case 2:
                            department_login("IT"); break;
                        case 3:
                            department_login("CSE"); break;
                        case 4:
                            department_login("SWE"); break;
                        case 5:
                            department_login("BLC"); break;
                        case 6:
                            department_login("FIN"); break;
                        case 7:
                            department_login("TRN"); break;
                        case 0:
                            break;
                        default:
                            textColor(12);
                            printf("Invalid choice! Try again.\n");
                            textColor(7);
                            Sleep(1500);
                            break;
                    }
                } while (sub_choice != 0);
                break;
            }

            case 0:
                textColor(10);
                printf("\nExiting the system. Goodbye!\n");
                textColor(7);
                break;

            default:
                textColor(12);
                printf("Invalid choice! Please try again.\n");
                textColor(7);
                Sleep(1500);
                break;
        }

    } while (choice != 0);

    return 0;
}



// ========== Main function start ==========