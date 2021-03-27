/*
 * File:  proj1.c
 * Author:  Gonçalo Nunes (99074)
 * Description: A Kanban-like program in C.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/* CONSTANTS */
#define TASK_DESCRIPTiON_MAX 50 /* Maximum value of characters in the Task's Description string. */
#define TASK_USER_MAX 20 /* Maximum value of characters in the Task's User string. */
#define TASK_ACTIVITY_MAX 20 /* Maximum value of characters in the Task's Activity string. */
#define TASK_MAX 50 /* Maximum ammount of simultaneous Tasks. */

/* STRUCTURES */

/* Represents a Task with an id, description, user, activity, expected duration and start time. */
typedef struct {
    unsigned int id;
    char description[TASK_DESCRIPTiON_MAX];
    char user[TASK_USER_MAX];
    char activity[TASK_ACTIVITY_MAX];
    unsigned int expected_duration;
    unsigned int start_time;
} Task;

/* Array of Tasks. */
Task Tasks[TASK_MAX];
/* Number of Tasks stored in memory. */
unsigned short count = 0;


/* Checks the user's input and acts accordingly. */
int main() {
    /* char command; */

    return 0;
}