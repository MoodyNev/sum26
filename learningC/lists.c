#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stud_type {
    int id;
    char firstname[20];
    char lastname[20];
    struct stud_type *next;
} stud_type;


/* Is the database empty? */
bool is_empty(stud_type const* list) {
    return !list;
}


/* Insert an element
 * 
 * Gets a pointer to a pointer, which points to the 1. element of the list
 * Gets ID, first name and last name of the student to be inserted
 *
 * Returns true if successful
 * Returns false to indicate an error
 */
bool enqueue(stud_type** students_list, int id, char const firstname[20], char const lastname[20]) {
    /* Get dynamic memory for the new list entry */
    stud_type* student = malloc(sizeof(*student));
    if (!student) {
        return false;
    }

    /* Fill the memory (beware of buffer overflows!) */
    student->id = id;
    strncpy(student->firstname, firstname, sizeof(student->firstname)-1);
    student->firstname[sizeof(student->firstname)-1] = '\0';
    strncpy(student->lastname, lastname, sizeof(student->lastname)-1);
    student->lastname[sizeof(student->lastname)-1] = '\0';

    /* Insert the new element into the list */
    stud_type* prev = NULL;
    stud_type* curr = *students_list;
    while (curr && curr->id < id) {
        prev = curr;
        curr = curr->next;
    }
    student->next = curr;
    /* Is the list empty? */
    if (prev) {
        prev->next = student;
    } else {
        *students_list = student;
    }
    return true;
}

/* Remove an element
 * 
 * Gets a pointer to a pointer, which points to the 1. element of the list
 * Gets the ID of the student to be deleted
 *
 * Returns true if successful
 * Returns false to indicate an error
 */
bool dequeue(stud_type** students_list, int id) {
    /* Check edge cases */
    if (is_empty(*students_list)) {
        return false;
    }
    
    /* Find the student */
    stud_type* prev = NULL;
    stud_type* curr = *students_list;
    while (curr && curr->id < id) {
        prev = curr;
        curr = curr->next;
    }

    /* What if the ID can't be found in the list? */
    if (!curr || curr->id != id) {
        return false;
    }
    
    /* Remove the student and free memory */
    stud_type *next = curr->next;
    free(curr);
    
    /* What must happen if the 1. element of the list is removed? */
    if (prev) {
        prev->next = next;
    } else {
        *students_list = next;
    }
    return true;
    
    /* ... */
    
}

/* Read an element 
 *
 * Gets a pointer to the list type
 * Gets the ID of the student whose data is to be read
 *
 * Writes first and last name into firstname and lastname, respectively
 */
bool get_student(stud_type const* students_list, int id, char firstname[20], char lastname[20]) {
    /* Search the DB */
    stud_type const* curr = students_list;
    while (curr && curr->id < id) {
        curr = curr->next;
    }

    if (!curr || curr->id != id) {
        /* Return value: Error */
        return false;
    } else {
        strncpy(firstname, curr->firstname, 19);
        firstname[19] = '\0';
        strncpy(lastname, curr->lastname, 19);
        lastname[19] = '\0';
        /* Return value: OK */
        return true;
    }
}

/* New structure representing a sortetd list with pointers to original students */
typedef struct stud_ind_t {
    stud_type* student;
    struct stud_ind_t* next;
} stud_ind_t;

/* Function type for comparing students */
typedef int (*stud_cmp_t)(stud_type const* a, stud_type const* b);

/* Compare by firstname */
int cmp_firstname(stud_type const* a, stud_type const* b) {
    return strcmp(a->firstname, b->firstname);
}

/* Compare by lastname */
int cmp_lastname(stud_type const* a, stud_type const* b) {
    return strcmp(a->lastname, b->lastname);
}

/* Helper functions that frees the memory used by lists */
void free_stud_type(stud_type** list) {
    for (stud_type* curr = *list; curr; ) {
        stud_type*next = curr->next;
        free(curr);
        curr = next;
    }
}

void free_stud_ind(stud_ind_t** list) {
    for (stud_ind_t* curr = *list; curr; ) {
        stud_ind_t *next = curr->next;
        free(curr);
        curr = next;
    }
}

stud_ind_t* sort_students(stud_type* students, stud_cmp_t stud_cmp) {
    // check edge cases
    if(is_empty(students)) {
        return NULL;
    }
    if (!stud_cmp) {
        return NULL;
    }

    // allocate the memory for the new list
    stud_ind_t* list = malloc(sizeof(*list));
    list->student = students;
    list->next = NULL;

    // iterate through original list to process all the students
    for (stud_type* curr_stud = students->next; curr_stud; curr_stud = curr_stud->next) {
        stud_ind_t *new = malloc(sizeof(*new));
        if(!new) {
            free_stud_ind(&list);
            return NULL;
        }

        // find the neighbors
        new->student = curr_stud;
        stud_ind_t* prev_ind = NULL;
        stud_ind_t* curr_ind = list;

        // go through the new sorted list to find the place for the new student
        while (curr_ind && (*stud_cmp)(curr_ind->student, curr_stud) <= 0) {
            prev_ind = curr_ind;
            curr_ind = curr_ind->next;
        }
        new->next = curr_ind;
        if(prev_ind) {
            prev_ind->next = new;
        } else {
            list = new;
        }

    }
    return list;
}


static void test_empty(stud_type const* list) {
    printf(">>> Testing whether the student list is empty ...\n");

    if(is_empty(list)) {
        printf("    The student list is empty \n");
    } else {
        printf("    The student list is not empty \n");
    }
}

static void test_get(stud_type const* list, int id) {
    printf(">>> Testing, whether the ID %4i exists ...\n", id);

    char firstname[20];
    char lastname[20];
    if(get_student(list, id, firstname, lastname)) {
        printf("    ID %4i: %s %s\n", id, firstname, lastname);
    } else {
        printf("    ID %4i is not known\n", id);
    }
}

static void test_enqueue(stud_type** list, int id, char const* firstname, char const* lastname) {
    printf(">>> Inserting the new student into the list: %s %s [%4i] ...\n", firstname, lastname, id);
    if(enqueue(list, id, firstname, lastname)) {
        printf("    ID %4i inserted\n", id);
    } else {
        printf("    ID %4i couldn't be inserted\n", id);
    }
}

static void test_dequeue(stud_type** list, int id) {
    printf(">>> Removing ID %4i ...\n", id);

    if(dequeue(list, id)) {
        printf("    ID %4i removed\n", id);
    } else {
        printf("    ID %4i is not known\n", id);
    }
}

static void test_dump(stud_type const* list) {
    printf(">>> Printing all known students ...\n");
    for(stud_type const* curr = list; curr; curr = curr->next) {
        printf("    ID %4i: %s %s\n", curr->id, curr->firstname, curr->lastname);
    }
}

/* Test all the list functions  */
int main(void) {
    /* Initialize database */
    stud_type* students_list = NULL;

    test_empty(students_list);
    test_enqueue(&students_list, 1234, "Eddard", "Stark");
    test_get(students_list, 1234);
    test_enqueue(&students_list, 5678, "Jon", "Snow");
    test_get(students_list, 1234);
    test_enqueue(&students_list, 9999, "Tyrion", "Lannister");
    test_get(students_list, 1235);
    test_enqueue(&students_list, 1289, "Daenerys", "Targaryen");
    test_dequeue(&students_list, 1234);
    test_empty(students_list);
    test_get(students_list, 5678);
    test_dequeue(&students_list, 9998);
    test_enqueue(&students_list, 1289, "Viserys", "Targaryen");
    test_dequeue(&students_list, 5678);
    test_enqueue(&students_list, 1, "Tywin", "Lannister");
    test_dump(students_list);

    {
        /* Generate list sorted by first name */
        stud_ind_t* list = sort_students(students_list, &cmp_firstname);
        /* Print list */
        printf(">>> printing all the students sorted by firstname... \n");
        for(stud_ind_t* curr = list; curr; curr = curr->next) {
            printf("    ID %4i: %s %s \n", curr->student->id, curr->student->firstname, curr->student->lastname);
        }
        /* Delete list */
        free_stud_ind(&list);
    }

    {
        /* Generate list sorted by last name */
        stud_ind_t* list = sort_students(students_list, &cmp_lastname);
        /* Print list */
        printf(">>> printing all the students sorted by firstname... \n");
        for(stud_ind_t* curr = list; curr; curr = curr->next) {
            printf("    ID %4i: %s %s \n", curr->student->id, curr->student->firstname, curr->student->lastname);
        }
        /* Delete list */
        free_stud_ind(&list);
    }

    /* Delete students_list */
    free_stud_type(&students_list);

    return 0;
}
