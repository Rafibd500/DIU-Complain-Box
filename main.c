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



int hidden=0;
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
    char status[10];
    char date[20];
    char time[20];
    char comment[100];
    char annonymous[5];
};

struct Complain all_complains[MAX_COMPLAIN];



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
// ---------- sort_student_login_info ----------
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
        printCenter("1. Registration\n", 1);
        printCenter("2. Login      \n", 1);
        printCenter("0. Main Menu   \n", 1);
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
    if(found1 != 0){ 
        if(strcmp(students_login_info[found1].id, id) == 0){
            char decrypt_pass[20];
            strcpy(decrypt_pass, students_login_info[found1].password);
            password_decrypt(decrypt_pass);
            if(strcmp(decrypt_pass, password) == 0){
                int found = binary_search_on_students_data(id);
                printCenterCustom("Welcome, ", 10);
                textColor(10);
                printf("%s! Login Successful. Redirecting to Student Dashboard...\n", students_data[found].name, 10);
                strcmp(current_logged_student, students_login_info[found1].id);
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
    Sleep(300000);
}
// ================== student Login end ==================

/////////////////////GENERATE ID //////////////
char* generate_complain_ID(){
    time_t t;
    struct tm *tm_info;
    char date_str[20];
    time(&t);
    tm_info = localtime(&t);
    int serial;
    strftime(date_str, sizeof(date_str), "%d%m%Y", tm_info);

    char *cmpID = malloc(30);
    FILE* fp = fopen("serial.txt", "r");
    if(fp == NULL){
        serial = 0;
    }
    else{
        fscanf(fp, "%d", &serial);
        fclose(fp);
    }
    fclose(fp);
    serial++;
    sprintf(cmpID, "CMP-%s-%04d", date_str, serial);
    // printf("%s\n", cmpID);
    
    fp = fopen("serial.txt", "w");
    fprintf(fp, "%d", serial);
    fclose(fp);
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
    printCenter("Submit a New Complain\n", 10);
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
    strcpy(student_complain.status, "Pending");
    

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
    //printf("%s %s\n", time_str, date_str);

    strcpy(student_complain.time, time_str);
    strcpy(student_complain.date, date_str);
    strcpy(student_complain.comment, "N/A");
    if(hidden) strcpy(student_complain.annonymous, "y");
    else strcpy(student_complain.annonymous, "n");
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
    free(cmpID);
    
}
// ================== take complain fucntion end ==================

// ================== submit new complain start ==================
void submit_new_complain(char id[]){
    system("cls");
    print_project_name();
    printCenter("Submit a New Complain\n", 10);
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
    system("cls");
    print_project_name();
    printCenter("Submit an Annonymus Complain\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
// ================== submit anonymous complain end ==================


// ================== view all complain start ==================
void view_all_complain(char id[]){
    system("cls");
    print_project_name();
    printCenter("View My Complain\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    


    _getch();

}
// ================== view all complain end ==================

// ================== view my complain start ==================
void view_my_complain(char id[]){
    int choice;
    do{
        system("cls");
        print_project_name();
        printCenter("View My Complain\n", 10);
        printCenter("---------------------------------------------------\n", 9);
        int found = binary_search_on_students_data(id);
        textColor(14);
        printf("User: %s(%s)                                                                                                                                                       Role:Student\n", students_data[found].name, students_data[found].id);        textColor(7);
        printCenter("01. View All Complains     \n", 11);
        printCenter("    02. View Only Pending Complains\n", 11);
        printCenter("       03. View Only In Progress Complains\n", 11);
        printCenter("    04. View Only Resolved Complains\n", 11);
        printCenter("      05. View Only Annonymous Complains\n", 11);
        printCenter(" 06. View Complain By Date    \n", 11);
        printCenter("07. Back to Main Menu       \n", 11);
        printLeft("Enter Your Choice: ", 2);
        scanf("%d", &choice);
        
        switch (choice){
        case 1:
            view_all_complain(id);
            break;
        case 2:
            submit_annonymus_complain(id);
            break;
        case 3:
            view_my_complain(id);
            break;
        case 4:
            track_complain_by_complainID();
            break;
        case 5:
            my_profile(id);
            break;
        case 6:
            student_logout(id);
            break;
        default:
            printCenter("Invalid choice. Try again\n", 12);
            break;
        }

    }while(choice != 0);
    Sleep(5000);
    return;
}
// ================== view my complain end ==================

// ================== track complain by complain ID start ==================
void track_complain_by_complainID(){
    system("cls");
    print_project_name();
    printCenter("Track Complain by Complain ID\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
// ================== submit new complain start ==================

// ================== my profile start ==================
void my_profile(char id[]){
    system("cls");
    print_project_name();
    printCenter("My Profile\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
// ==================  my profile end ==================

// ================== student logout start ==================
void student_logout(char id[]){
    system("cls");
    print_project_name();
    printCenter("Logout\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
// ================== student logout end ==================


// ================== student dashboard start ==================
void student_dashboard(char id[]){
    int choice;
    do{
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
        
        switch (choice){
        case 1:
            submit_new_complain(id);
            break;
        case 2:
            submit_annonymus_complain(id);
            break;
        case 3:
            view_my_complain(id);
            break;
        case 4:
            track_complain_by_complainID();
            break;
        case 5:
            my_profile(id);
            break;
        case 6:
            student_logout(id);
            break;
        default:
            printCenter("Invalid choice. Try again\n", 12);
            break;
        }

    }while(choice != 0);
    Sleep(5000);
    return;
}
// ================== student dashboard end ==================




// ========== Main function start ==========
int main()
{
    system("cls");

    int student_logged = 1;
    if(student_logged){
        // ========== DEV MODE START ==========
        // Uncomment this block to directly test student dashboard
        
        char test_student_id[] = "242-35-001";
        student_dashboard(test_student_id);
        return 0;
        
        // ========== DEV MODE END ==========
    }
    // system("cls");
    int choice;
    do
    {
        
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
        switch (choice)
        {
        case 1:
            system("cls");
            student_login_reg();
            break;

        default:
            break;
        }

    } while (choice != 0);
}
// ========== Main function start ==========
