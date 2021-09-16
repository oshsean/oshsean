// print_labmarks
// Sean O'Shea
// 16/5/17

#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_STUDENT_NAME_LENGTH 128
#define MAX_GRADE_STRING_LENGTH 22
#define MAX_LAB_NAME_LENGTH 32
#define MAX_LINE_LENGTH 4096

struct student {
    int              zid;
    char             name[MAX_STUDENT_NAME_LENGTH + 1];
    char             lab_name[MAX_LAB_NAME_LENGTH + 1];
    char             lab_grades[MAX_GRADE_STRING_LENGTH + 1];
    struct student   *next;
};

struct student *read_students_file(char filename[]);
struct student *read_student(FILE *stream);
double grades2labmark(char grades[]){
   int count = 0;
   double AA = 1.2;
   int A = 1; 
   double B = 0.8;
   double C = 0.5;
   double grade = 0;
   while (grades[count] != '\0'){
      if (grades[count] == 'C'){
         grade = grade + C;
      } else if (grades[count] == 'B'){
         grade = grade + B;
      } else if (grades[count] == 'A' && grades[count + 1] != '+'){
         grade = grade + A;
      } else if (grades[count] == 'A'){
         grade = grade + AA;
      }  
      count++;
   }
   if (grade > 10){
      grade = 10;
   }
   return grade;   
}
int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <marks-file>\n", argv[0]);
        return 1;
    }
    struct student *student_list = read_students_file(argv[1]);
    struct student *curr = student_list;
    while (curr != NULL){
      if (!strcmp(curr->lab_name, argv[2])){
         printf ("%d %-30s %-12s %-22s %4.1lf\n", curr->zid, curr->name, curr->lab_name, curr->lab_grades, grades2labmark(curr->lab_grades));
      }
      curr = curr->next;
    }  
    return 0;
}

// DO NOT CHANGE THE CODE BELOW HERE - DO NOT CHANGE read_students_file

// read_students_file reads a file where line contains information for 1 student
// it creates a linked of student structs containing the information
// it returns a pointer to the head of the list

struct student *read_students_file(char filename[]) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr,"warning file %s could not  be opened for reading\n", filename);
        return NULL;
    }

    struct student *first_student = NULL;
    struct student *last_student = NULL;
    struct student *s;
    while ((s = read_student(fp)) != NULL) {
        if (last_student == NULL) {
            first_student = s;
            last_student = s;
        } else {
            last_student->next = s;
            last_student = s;
        }
    }
    return first_student;
}

// DO NOT CHANGE read_student

// read_student mallocs a student struct
// and reads a line in this format:
//
// 5099703 Tsun Bordignon thu13-sitar A+A+CABAB..A.
//
// stores the values in the struct field
// and returns a pointer to the struct

struct student *read_student(FILE *stream) {
    char line[MAX_LINE_LENGTH];

    struct student *s = malloc(sizeof (struct student));
    assert(s);

    if (fgets(line, MAX_LINE_LENGTH, stream) == NULL) {
        free(s);
        return NULL;
    }

    char *newline_ptr = strchr(line, '\n');
    assert(newline_ptr);
    *newline_ptr = '\0';

    char *space_ptr = strrchr(line, ' ');
    assert(space_ptr);
    strncpy(s->lab_grades, space_ptr + 1, MAX_GRADE_STRING_LENGTH);
    s->lab_grades[MAX_GRADE_STRING_LENGTH] = '\0';
    *space_ptr = '\0';

    space_ptr = strrchr(line, ' ');
    assert(space_ptr);
    strncpy(s->lab_name, space_ptr + 1, MAX_LAB_NAME_LENGTH);
    s->lab_name[MAX_LAB_NAME_LENGTH] = '\0';
    *space_ptr = '\0';

    space_ptr = strchr(line, ' ');
    assert(space_ptr);
    strncpy(s->name, space_ptr + 1, MAX_STUDENT_NAME_LENGTH);
    s->name[MAX_STUDENT_NAME_LENGTH] = '\0';
    *space_ptr = '\0';

    s->zid = atoi(line);
    s->next = NULL;
    return s;
}

