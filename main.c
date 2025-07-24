#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

// ======= Function call =======

//For students ===========
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

void student_dashboard(char id[]);
void submit_new_complain(char id[]);
void view_my_complain(char id[]);
void submit_annonymus_complain(char id[]);
void track_complain_by_complainID();
void my_profile(char id[]);
void student_logout(char id[]);




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
    char cmpTitle[500];
    char cmpDescription[500];
};

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
    scanf("%s", password);
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




void submit_new_complain(char id[]){
    system("cls");
    print_project_name();
    printCenter("Submit a New Complain\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
void submit_annonymus_complain(char id[]){
    system("cls");
    print_project_name();
    printCenter("Submit an Annonymus Complain\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
void view_my_complain(char id[]){
    system("cls");
    print_project_name();
    printCenter("View My Complains\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
void track_complain_by_complainID(){
    system("cls");
    print_project_name();
    printCenter("Track Complain by Complain ID\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
void my_profile(char id[]){
    system("cls");
    print_project_name();
    printCenter("My Profile\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}
void student_logout(char id[]){
    system("cls");
    print_project_name();
    printCenter("Logout\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    printf("Press any key....");
    _getch();
}


// ================== student dashboard start ==================
void student_dashboard(char id[]){
    system("cls");
    print_project_name();
    printCenter("Student Dashboard\n", 10);
    printCenter("---------------------------------------------------\n", 9);
    int choice;
    do{
        
        printCenter("1. Submit a New Complain     \n", 11);
        printCenter("   2. Submit an Annonymus Complain\n", 11);
        printCenter("3. View My Complains        \n", 11);
        printCenter("    4. Track Complain By Complain ID\n", 11);
        printCenter("5. My Profile                \n", 11);
        printCenter("6. Logout                   \n", 11);
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

    int student_logged = 0;
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
