#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define MAX_COMPLAIN 5000

// ======= Function call =======

//for student register/login
void printCenter();
int getConsoleWidth();
void student_login_reg();
void student_registration();
void student_login();
void append_id_password(char id[], char password[]);
void password_encrypt(char *password);
void password_decrypt(char *password);
void load_students_data();
void upload_students_data();
void load_logged_student_data();
void upload_logged_student_data();
int binary_search_on_students_data(char id[]);
int binary_search_on_students_login(char id[]);
void sort_student_login_info();

// for student dashboard 
void student_dashboard(char id[]);
void submit_new_complain(char id[]);
void view_my_complain(char id[]);
void submit_annonymus_complain(char id[]);
void track_complain_by_complainID();
void my_profile(char id[]);
void student_logout(char id[]);

// for submit new complain 
void take_complain(char id[], int choice);
char* generate_complain_ID();



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
    char username[30];
    char password[30];
};

struct Admin admins[100];
int admin_count = 0;





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
void take_complain(char id[], int choice){
    struct Complain student_complain;
    switch (choice)
    {
        case 1:
            strcpy(student_complain.team, "Hall Authority");
            break;
        case 2:
            strcpy(student_complain.team, "IT Section");
            break;
        case 3:
            strcpy(student_complain.team, "Registrar Office");
            break;
        case 4:
            strcpy(student_complain.team, "Student Affairs");
            break;
        case 5:
            strcpy(student_complain.team, "Dept. Of CSE");
            break;
        case 6:
            strcpy(student_complain.team, "Dept. Of SWE");
            break;
        case 7:
            strcpy(student_complain.team, "Dept. Of BBA");
            break;
        case 8:
            strcpy(student_complain.team, "Dept. Of EEE");
            break;
        case 9:
            strcpy(student_complain.team, "BLC Support Team");
            break;
        case 10:
            strcpy(student_complain.team, "Finance and Accounts Team");
            break;
        case 11:
            strcpy(student_complain.team, "Transport Management Team");
            break;
        default:
            strcpy(student_complain.team, "Unknown Team");
            break;
    }
    system("cls");
    print_project_name();
    if(!annonymous) printCenter("Submit a New Complain\n", 10);
    else printCenter("Submit a new Complain Annonymusly\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    int found = binary_search_on_students_data(id);
    textColor(14);
            printf("User: %s(%s)                                                                                                                                                       Role:Student\n", students_data[found].name, students_data[found].id);
    textColor(7); //clear color

    textColor(11);
    printf("To: %s\n", student_complain.team);
    textColor(7); //clear color
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

    ///////================ FOR TIME===============
    time_t t;
    struct tm *tm_info;
    char time_str[20];
    char date_str[20];
    char serial_str[20];
    time(&t);
    tm_info = localtime(&t);

    strftime(time_str, sizeof(time_str), "%I:%M %p", tm_info);
    strftime(date_str, sizeof(date_str), "%d/%m/%Y", tm_info);

    strcpy(student_complain.time, time_str);
    strcpy(student_complain.date, date_str);
    strcpy(student_complain.comment, "N/A");
    if(annonymous) strcpy(student_complain.annonymous, "y");
    else strcpy(student_complain.annonymous, "n");

    strcpy(student_complain.studentID, id); // Add student ID to complaint

    printCenter("\nComplaint submitted successfully!\nYour complaint has been recorded. You will be notified upon status updates.\n", 10);
    printf("Press any key to continue....");
    _getch();

    char filename[100];
    sprintf(filename, "students_complain/%s.txt", id);
    FILE* studnetFp = fopen(filename, "a");
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

    // Write to admin file as well
    FILE* adminFp = fopen("admin/all_complain.txt", "a");
    if (adminFp != NULL) {
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

    free(cmpID);
    annonymous = 0;
}
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
    printCenter("03. Registrar Office\n", 11);
    printCenter("04. Student Affairs\n", 11);
    printCenter("05. Dept. Of CSE    \n", 11);
    printCenter("06. Dept. Of SWE    \n", 11);
    printCenter("07. Dept. Of BBA    \n", 11);
    printCenter("08. Dept. Of EEE    \n", 11);
    printCenter("09. BLC Support Team\n", 11);
    printCenter("          10. Finance and Accounts Team\n", 11);
    printCenter("          11. Transport Management Team\n", 11);
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

    // Prompt update
    system("cls");
    print_project_name();
    printCenter("Update Complaint Status and Comment\n", 11);

    printf("Complaint ID : %s\n", complaints[found].cmpID);
    printf("Current Status: %s\n", 
        strcmp(complaints[found].status, "1") == 0 ? "Pending" :
        strcmp(complaints[found].status, "2") == 0 ? "In Progress" :
        strcmp(complaints[found].status, "3") == 0 ? "Resolved" : "Unknown");
    printf("Current Comment: %s\n\n", complaints[found].comment);

    printf("Select New Status:\n1. Pending\n2. In Progress\n3. Resolved\nEnter choice: ");
    int status_choice;
    scanf("%d", &status_choice);

    if (status_choice >= 1 && status_choice <= 3) {
        sprintf(complaints[found].status, "%d", status_choice);
    }

    getchar(); // clear newline
    char new_comment[100];
    printf("Enter Comment (max 100 chars): ");
    fgets(new_comment, sizeof(new_comment), stdin);
    new_comment[strcspn(new_comment, "\n")] = '\0';

    strcpy(complaints[found].comment, new_comment);

    // Rewrite admin/all_complain.txt
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

    // Rewrite respective student file
    char stu_filename[100];
    sprintf(stu_filename, "students_complain/%s.txt", complaints[found].studentID);
    FILE *stu_fp = fopen(stu_filename, "r");
    if (stu_fp) {
        struct Complain student_complaints[200];
        int stu_count = 0;
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
            stu_count++;
        }
        fclose(stu_fp);

        // Update student complaint
        for (int i = 0; i < stu_count; i++) {
            if (strcmp(student_complaints[i].cmpID, cmp_id) == 0) {
                strcpy(student_complaints[i].status, complaints[found].status);
                strcpy(student_complaints[i].comment, complaints[found].comment);
                break;
            }
        }

        // Rewrite student file
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
    }

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
    int found = 0;
    int i;
    for (i = 0; i < count; i++) {
        if (strcmp(complaints[i].cmpID, cmp_id) == 0) {
            system("cls");
            print_project_name();
            printCenter("Complaint Details\n", 11);

            textColor(14);
            printf("\n%-20s: ", "Complaint ID"); textColor(7); printf("%s\n", complaints[i].cmpID);
            textColor(14);
            printf("%-20s: ", "Student ID"); textColor(7); printf("%s\n", complaints[i].studentID);
            textColor(14);
            printf("%-20s: ", "Title"); textColor(7); printf("%s\n", complaints[i].title);
            textColor(14);
            printf("%-20s: ", "Department"); textColor(7); printf("%s\n", complaints[i].team);
            textColor(14);
            printf("%-20s: ", "Status");
            if (strcmp(complaints[i].status, "1") == 0) { textColor(12); printf("Pending\n"); }
            else if (strcmp(complaints[i].status, "2") == 0) { textColor(6); printf("In Progress\n"); }
            else if (strcmp(complaints[i].status, "3") == 0) { textColor(10); printf("Resolved\n"); }
            else { textColor(7); printf("Unknown\n"); }
            textColor(14);
            printf("%-20s: ", "Date"); textColor(7); printf("%s\n", complaints[i].date);
            textColor(14);
            printf("%-20s: ", "Time"); textColor(7); printf("%s\n", complaints[i].time);
            textColor(14);
            printf("%-20s: ", "Comment"); textColor(7); printf("%s\n", complaints[i].comment);
            textColor(14);
            printf("%-20s: ", "Anonymous"); textColor(7); printf("%s\n", (strcmp(complaints[i].annonymous, "y") == 0 ? "Yes" : "No"));
            textColor(14);
            printf("\n%-20s:\n", "Description"); textColor(7); printf("%s\n", complaints[i].description);

            found = 1;
            break;
        }
    }

    if (!found) {
        printCenter("\nComplaint not found.\n", 4);
    }

    printCenter("\nPress any key to return...", 7);
    _getch();
    update_complaint_status_and_comment(complaints, count, complaints[i].cmpID);
}
// ================== show complaint details by ID end ==================

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



// ========== ADMIN DASHBOARD ==========
void admin_dashboard() {
    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("Admin Dashboard\n", 10);
        printCenter("----------------------------------------------\n", 9);
        printCenter("1. View All Complaints (Filter)\n", 11);
        printCenter("2. Track Complaint by ID\n", 11);
        printCenter("3. Add Admin\n", 11);
        printCenter("4. Add Team User\n", 11);
        printCenter("5. Logout\n", 11);
        printLeft("Enter your choice: ", 2);
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                view_all_complain_admin();
                break;
            case 2:
                // track_complaint_by_id();
                break;
            case 3:
                // add_admin();
                break;
            case 4:
                // add_team_user();
                break;
            case 5:
                return; // logout
        }
    } while (1);
}

// admin utitily functions 
void load_admins() {
    admin_count = 0;
    FILE *fp = fopen("admin/admin_login.txt", "r");
    if (fp == NULL) {
        printf("Admin file not found!\n");
        return;
    }
    while (fscanf(fp, "%[^|]|%[^\n]\n", admins[admin_count].username, admins[admin_count].password) != EOF) {
        admin_count++;
    }
    fclose(fp);
}
// ========== ADMIN LOGIN ==========
void admin_login(){
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
            printCenter("Login successful! Redirecting to dashboard...\n", 10);
            Sleep(3000);
            admin_dashboard();
            return;
        }
    }

    printCenter("Invalid credentials! Try again...\n", 12);
    Sleep(2000);
}

// ========== Main function start ==========
int main() {
    int choice;
    do {
        system("cls");
        print_project_name();
        printCenter("1. Student Login\n", 3);
        printCenter("2. Admin Login\n", 4);
        printCenter("3. Academic Cell Login\n", 5);
        printCenter("4. Hall Authority Login\n", 6);
        printCenter("5. IT Cell Login\n", 7);
        printCenter("6. Accounts Section Login\n", 8);
        printCenter("0. EXIT...\n", 9);

        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                student_login_reg(); break;
            case 2:
                admin_login(); break;
        }

    } while (choice != 0);

    return 0;
}


// ========== Main function start ==========
