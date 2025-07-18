#include <stdio.h>
// 242-15-003|Sajib Hossain|C|CSE|01987654321|sajib242-15-003@diu.edu.bd
struct Student{
    char id[15];
    char name[100];
    char section[5];
    char dept[5];
    char mobile[15];
    char email[50];
};
struct Student student_array[10000];
int cnt = 0;
void load_data(){
    FILE *fp = fopen("students.txt", "r");
    while(fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", &student_array[cnt].id, &student_array[cnt].name, &student_array[cnt].section, &student_array[cnt].dept, &student_array[cnt].mobile, &student_array[cnt].email) != EOF){
        cnt++;
    }
    fclose(fp);
}
void search_student(char id[15]){
    load_data();
    for(int i=0; i<cnt; i++){
        printf("%s %s\n",student_array[i].id, id );
        if(!strcmp(id, student_array[i].id)) {
            printf("Found\n");
            return;
        }
    }
    printf("Not found\n");
}
int main()
{
    printf("studnet data:\n");
    // load_data();
    // for(int i = 0; i<cnt; i++){
    //     printf("Id: %s\n", student_array[i].id);
    //     printf("Name: %s\n", student_array[i].name);
    //     printf("Section: %s\n", student_array[i].section);
    //     printf("Department: %s\n", student_array[i].dept);
    //     printf("Mobile: %s\n", student_array[i].mobile);
    //     printf("Email: %s\n\n", student_array[i].email);
    // }
    // search_student("200-35-999");
    load_data();
    for(int i = 0; i<cnt; i++){
        printf("%s|", student_array[i].id);
        printf("%s|", student_array[i].name);
        printf("%s|", student_array[i].section);
        printf("%s|", student_array[i].dept);
        
        for(int j = 0; j<11; j++){
            switch(student_array[i].mobile[j])
            {
                case '0':
                    student_array[i].mobile[j] = 'P';
                    break;
                case '1':
                    student_array[i].mobile[j] = 'M';
                    break;
                case '2':
                    student_array[i].mobile[j] = '$';
                    break;
                case '3':
                    student_array[i].mobile[j] = '#';
                    break;
                case '4':
                    student_array[i].mobile[j] = 'H';
                    break;
                case '5':
                    student_array[i].mobile[j] = '@';
                    break;
                case '6':
                    student_array[i].mobile[j] = 'Z';
                    break;
                case '7':
                    student_array[i].mobile[j] = '&';
                    break;
                case '8':
                    student_array[i].mobile[j] = 'N';
                    break;
                case '9':
                    student_array[i].mobile[j] = 'L';
                    break;
            }
        }
        printf("%s|", student_array[i].mobile);
        printf("%s\n", student_array[i].email);
    }
    return 0;
}