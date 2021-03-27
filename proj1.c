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

#define STRING_MAX 200 /* Maximum ammount of characters in each command. */

#define USERS_MAX 50 /* Maximum ammount of users */



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


/* Represents the Kanban Board with the number of existing tasks, current time and an array of all tasks. */
typedef struct {
    unsigned int task_count;
    unsigned int time;
    /* ->>>>>> Array of struct users */
    /* ->>>>>> Array of struct activities */
    Task tasks[TASK_MAX];
} Kanban;

/* Inicialize the task count and time fields of the Kanban struct. */
Kanban kanban = {0, 0};



/* Checks the user's input and acts accordingly. */
int main() {
    char string[STRING_MAX];
    char c, command = '_';  
    unsigned int i = 0;  

    while (command != 'q') {
        while ((c = getchar()) != '\n')
            string[i] = c;

        i = 0;
        command = string[0];

        switch (command)
        {
         case 'q':
            return 0;

        case 't':
            
            break;

        case 'l':
            
            break;

        case 'n':
            
            break;

        case 'u':
            
            break;

        case 'm':
            
            break;

        case 'd':
            
            break;
        case 'a':
            
            break;
        default:
            printf("Error: Invalid Command: %c\n", command);
            break;
        }
    }

    return 0;
}