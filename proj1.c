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

#define TRUE 1
#define FALSE 0

#define NO_CHAR_FOUND 0
#define FOUND_CHAR 1
#define NO_SPACING 0
#define SPACING 1

#define DESCRIPTION_MAX_CHAR 51 /* Maximum value of characters in the Task's Description string. */
#define USER_MAX_CHAR 21 /* Maximum value of characters in the Task's User string. */
#define ACTIVITY_MAX_CHAR 21 /* Maximum value of characters in the Task's Activity string. */
#define ID_MAX_CHAR 6 /* Maximum amount of character in each id. */
#define TASK_MAX 10000 /* Maximum ammount of simultaneous Tasks. */
#define ACTIVITY_MAX 10 /* Maximum ammount of Activities. */
#define USER_MAX 50 /* Maximum ammount of Users. */
#define INITIAL_ACTIVITIES 3 /* Initial number of activities. */

#define ARGUMENT_MAX_CHAR 75
#define ARGUMENTS_MAX 3

#define STRING_MAX 200 /* Maximum ammount of characters in each command. */

#define TODO "TO DO\0" /* String that represents the TO DO Activity. */
#define IN_PROGRESS "IN PROGRESS\0" /* String that represents the TO DO Activity. */
#define DONE "DONE\0" /* String that represents the TO DO Activity. */
#define TASK "task" /* TASK string */
#define DURATION "duration" /* duration string. */
#define SLACK "slack" /* slack string. */

#define ERROR_TOO_MANY_TASKS "too many tasks" /* Error string for too many tasks. */
#define ERROR_DUPLICATE_DESCRIPTION "duplicate description" /* Error string for duplicate description. */
#define ERROR_NO_SUCH_TASK "no such task" /* Error string for no such task. */
#define ERROR_INVALID_DURATION "invalid duration" /* Error string for invalid duration. */
#define ERROR_INVALID_TIME "invalid time" /* Error string for invalid time. */
#define ERROR_TOO_MANY_USERS "too many users" /* Error string for too many users. */
#define ERROR_USER_ALREADY_EXISTS "user already exists" /* Error string for duplicate user. */
#define ERROR_TASK_ALREADY_STARTED "task already started" /* Error string for task already started. */
#define ERROR_NO_SUCH_ACTIVITY "no such activity" /* Error string for no such activity. */
#define ERROR_NO_SUCH_USER "no such user" /* Error string for no such user. */
#define ERROR_DUPLICATE_ACTIVITY "duplicate activity" /* Error string for duplicate description. */
#define ERROR_INVALID_ACTIVITY "invalid description" /* Error string for invalid activity */
#define ERROR_TOO_MANY_ACTIVITIES "too many activities" /* Error string for too many activities. */

#define ERROR -1 /* Error Code. */






/* STRUCTURES */

/* Represents a Task with an id, description, user, activity, expected duration and start time. */
typedef struct {
    unsigned int id;
    char description[DESCRIPTION_MAX_CHAR];
    char user[USER_MAX_CHAR];
    char activity[ACTIVITY_MAX_CHAR];
    unsigned int expected_duration;
    unsigned int start_time;
} Task;

/* Struct with an array of type Task that stores all tasks and the task count. */
typedef struct {
    Task task[TASK_MAX];
    unsigned int count;
} Tasks;

/* Struct with an array of all activities and the ammount of current activities. */
typedef struct {
    char activity[ACTIVITY_MAX][ACTIVITY_MAX_CHAR];
    unsigned short count;
} Activities;

/* Struct with an array of all users and the ammount of current users. */
typedef struct {
    char user[USER_MAX][USER_MAX_CHAR];
    unsigned short count;
} Users;

/* Represents the Kanban Board with the current time and users, activities and tasks structures. */
typedef struct {
    unsigned int time;
    Users users;
    Activities activities;
    Tasks tasks;
    char arguments[ARGUMENTS_MAX][ARGUMENT_MAX_CHAR];
} Kanban;

/* Global variable of type Kanban that represents the kanban board. */
Kanban kanban;



/* FUNCTIONS */

/* Initializes the Kanban board with the initial values. */
void initialize_board() {
    kanban.time = 0;
    kanban.activities.count = 3;
    kanban.users.count = 0;
    kanban.tasks.count = 0;
    strcpy(kanban.activities.activity[0], TODO);
    strcpy(kanban.activities.activity[1], IN_PROGRESS);
    strcpy(kanban.activities.activity[2], DONE);
    kanban.activities.count = INITIAL_ACTIVITIES;
}

/* Checks if any argument was given by the user. Return True if an argument was found and False otherwise. */
int argument_exists(char string[]) {
    unsigned int i;
    for (i = 1; i < strlen(string)-1; i++) {
        if (string[i] != ' ' || string[i] != '\n')
             return TRUE;
    }

    return FALSE;
}

/* Function that modifies the kanban.arguments string with the user's input. */
void get_arguments(char string[], short num_args, int spacing) {
    int j = 0, args = 0;
    unsigned int i;
    int flag = NO_CHAR_FOUND;

    for(i=2;i<=ARGUMENT_MAX_CHAR;i++)
    {
        
        if((string[i] == ' ' || string[i] == '\0') && (args < (num_args - 1) || (spacing == NO_SPACING && num_args == 1)))
        {
            if (flag == FOUND_CHAR) {
                kanban.arguments[args][j]='\0';
                args++;  
                j=0;
                flag = NO_CHAR_FOUND;
            }
        }
        else
        {  
            kanban.arguments[args][j]=string[i];
            j++;
            flag = FOUND_CHAR;
        }
    }
}

/* Checks if the given description already exists in any of the kanban tasks. 
Returns True if description already exists and False if it doesn't.  */
int is_duplicate_description(char description[]) {
    unsigned int i;

    for (i = 0; i < kanban.tasks.count ; i++)
        if (strcmp(description, kanban.tasks.task[i].description) == 0)
            return TRUE;
    
    return FALSE;
}

/* Checks if the given user already exists. 
Returns True if the user already exists and False otherwise. */
int is_duplicate_user(char user[]) {
    unsigned int i;

    for (i = 0; i < kanban.users.count; i++) 
        if (strcmp(kanban.users.user[i], user) == 0)
            return TRUE;

    return FALSE;
}


/* Checks if the given activity already exists. 
Returns True if the user already exists and False otherwise. */
int is_duplicate_activity(char activity[]) {
    unsigned int i;

    for (i = 0; i < kanban.activities.count; i++) 
        if (strcmp(kanban.activities.activity[i], activity) == 0)
            return TRUE;

    return FALSE;
}


/* Adds if possible a new Task to the Kanban board. Returns False if the arguments
are incorrect or returns True if the task was created successfully. */
int add_task(int duration, char description[]) {

    if (kanban.tasks.count >= TASK_MAX) {
        printf("%s\n", ERROR_TOO_MANY_TASKS);
        return FALSE;
    }

    else if (is_duplicate_description(description)) {
        printf("%s\n", ERROR_DUPLICATE_DESCRIPTION);
        return FALSE;
    }

    else if (duration <= 0){
        printf("%s\n", ERROR_INVALID_DURATION);
        return FALSE;
    }

    else if (strlen(description) > DESCRIPTION_MAX_CHAR) { 
        return FALSE;
    }
    
    /* Initialize Task */
    kanban.tasks.task[kanban.tasks.count].expected_duration = duration; 
    strcpy(kanban.tasks.task[kanban.tasks.count].description, description);
    kanban.tasks.task[kanban.tasks.count].id = kanban.tasks.count + 1;
    kanban.tasks.task[kanban.tasks.count].start_time = 0;
    strcpy(kanban.tasks.task[kanban.tasks.count].activity, TODO);

    printf("%s %d\n", TASK, kanban.tasks.task[kanban.tasks.count].id);
    kanban.tasks.count++;

    return TRUE;
}

/* Prints the task's details based on the given id. 
Returns True if the task exits otherwise returns False and prints the error. */
int list_task(unsigned int id) { 
    unsigned int i;

    for (i = 0; i < kanban.tasks.count; i++) {
        if (kanban.tasks.task[i].id == id) {
            printf("%d %s #%d %s\n", kanban.tasks.task[i].id, kanban.tasks.task[i].activity,\
            kanban.tasks.task[i].expected_duration, kanban.tasks.task[i].description);
            return TRUE;
        }
    }

    printf("%d: %s\n", id, ERROR_NO_SUCH_TASK);
    return FALSE;
}





/* Implementation of Selection Sort algorithm to order the tasks alphabetically */
void sort_by_description(int l, int r) {
    int i, j, min;
    Task temp;

    for (i = l; i < r; i++) {
        min = i;
        for (j = i+1; j <= r; j++)
            if (strcmp(kanban.tasks.task[j].description, kanban.tasks.task[min].description) < 0)
                min = j;
        temp = kanban.tasks.task[i];
        kanban.tasks.task[i] = kanban.tasks.task[min];
        kanban.tasks.task[min] = temp;
    }
    
}

/* Implementation of Bubble Sort algorithm to order the tasks by start time. */
void sort_by_start_time(int l, int r) {
    int i, step, done;
    Task temp;

    for (step = l; step <= r; step++) {
        done = 1;
        for (i = l; i <= r - step - 1; i++) {
            if (kanban.tasks.task[i].start_time > kanban.tasks.task[i+1].start_time) {
                temp = kanban.tasks.task[i];
                kanban.tasks.task[i] = kanban.tasks.task[i + 1];
                kanban.tasks.task[i + 1] = temp;
                done = 0;
            } 
        }
        if (done == 1)
            break;
    }
}

/* Forwards the time based on the given offset. 
Returns True if the offset is valid and False if the offset is invalid. */
int time_forward(int offset) {
    if (offset >= 0) {
        kanban.time += offset;
        printf("%d\n", kanban.time);
        return TRUE;
    }

    printf("%s\n", ERROR_INVALID_TIME);
    return FALSE;
}

/* Adds a user to the kanban board if the number of users does not exceed the
maximum ammount of simultanious users and if the user does not yet exist. 
Returns True if the user was added succefully and False otherwise. */
int add_user(char user[]) {
    if (is_duplicate_user(user)) {
        printf("%s\n", ERROR_USER_ALREADY_EXISTS);
        return FALSE;

    } else if (kanban.users.count >= USER_MAX) {
        printf("%s\n", ERROR_TOO_MANY_USERS);
        return FALSE;
    } 

    strcpy(kanban.users.user[kanban.users.count], user);
    kanban.users.count++;
    return TRUE;
}

/* Finds the index of a task with the given id. 
Returns the index if the id was found or returns ERROR if it wasn't.*/
int find_task_by_id(unsigned int id) {
    unsigned int i;
    for (i = 0; i < kanban.tasks.count; i++) {
        if (kanban.tasks.task[i].id == id) {
            return i;
        }
    }
    return ERROR;
}


int is_valid_activity(char activity[]) {
    unsigned int i;
    for (i = 0; i < strlen(activity); i++) {
        if (activity[i] >= 'a' && activity[i] <= 'z')
            return FALSE;
    }
    return TRUE;
}


void handle_command_t(char string[]) {
    unsigned short num_args = 2;
 
    get_arguments(string, num_args, SPACING);
    add_task(atoi(kanban.arguments[0]), kanban.arguments[1]);
}


void handle_command_l() {
    unsigned int i, j = 0, count = 0;
    char id[ID_MAX_CHAR];
    short flag = NO_CHAR_FOUND;
    char c = '_';

    /* Find each id inserted */
    while(c != '\n') {
        c = getchar();
        if (c == ' ' || c == '\0' || c == '\n') {
            if (flag == FOUND_CHAR) {
                id[j] = '\0';
                j = 0;
                count++;
                flag = NO_CHAR_FOUND;
                list_task(atoi(id)); /* print the task's information */
            }
        } else {
            id[j] = c;
            j++;
            flag = FOUND_CHAR;
        }
    }
    if (count == 0){
        /* List all tasks by description */
        sort_by_description(0, kanban.tasks.count-1);
        for (i = 0; i < kanban.tasks.count; i++)
            list_task(kanban.tasks.task[i].id);
    }  
    
}

void handle_command_n(char string[]) {
    get_arguments(string, 1, NO_SPACING);
    time_forward(atoi(kanban.arguments[0]));
}


void handle_command_u(char string[]) {
    unsigned int i;
    get_arguments(string, 1, NO_SPACING);

    if (argument_exists(string)) {
        add_user(kanban.arguments[0]); /* add new user to the kanban */
    } else {
        for (i = 0; i < kanban.users.count; i++) /* print all users */
            printf("%s\n", kanban.users.user[i]);
    }
}

void handle_command_m(char string[]) {
    int slack, index;
    unsigned int time_spent;
    get_arguments(string, 3, SPACING);
    index = find_task_by_id(atoi(kanban.arguments[0]));

    if (index == ERROR) {
        printf("%s\n", ERROR_NO_SUCH_TASK);
        return;

    } else if (strcmp(kanban.arguments[2], TODO) == 0) {
        if(strcmp(kanban.tasks.task[index].activity, TODO) != 0)
            printf("%s\n", ERROR_TASK_ALREADY_STARTED);
        return;

    } else if (!is_duplicate_user(kanban.arguments[1])) {
        printf("%s\n", ERROR_NO_SUCH_USER);
        return;

    } else if (!is_duplicate_activity(kanban.arguments[2])) {
        printf("%s\n", ERROR_NO_SUCH_ACTIVITY);
        return;
    }

    strcpy(kanban.tasks.task[index].user, kanban.arguments[1]);

    if (strcmp(kanban.arguments[2], kanban.tasks.task[index].activity) != 0) {
        if (strcmp(kanban.arguments[2], DONE) == 0) {

            if (strcmp(kanban.tasks.task[index].activity, TODO) == 0) {
                time_spent = 0;
                kanban.tasks.task[index].start_time = kanban.time;
            } else 
                time_spent = kanban.time - kanban.tasks.task[index].start_time;
            slack = time_spent - kanban.tasks.task[index].expected_duration;
            printf("%s=%d %s=%d\n", DURATION, time_spent, SLACK, slack);

        } else if (strcmp(kanban.tasks.task[index].activity, TODO) == 0) 
            kanban.tasks.task[index].start_time = kanban.time;
        
        strcpy(kanban.tasks.task[index].activity, kanban.arguments[2]); 
    }    
}


void handle_command_d(char string[]) {
    unsigned int i;
    get_arguments(string, 1, SPACING);

    if (!is_duplicate_activity(kanban.arguments[0])) {
        printf("%s\n", ERROR_NO_SUCH_ACTIVITY);
        return;
    }

    sort_by_description(0, kanban.tasks.count-1);
    sort_by_start_time(0, kanban.tasks.count-1);

    for (i = 0; i < kanban.tasks.count; i++) {
        if (strcmp(kanban.tasks.task[i].activity, kanban.arguments[0]) == 0) {
            printf("%d %d %s\n", kanban.tasks.task[i].id,\
            kanban.tasks.task[i].start_time, kanban.tasks.task[i].description);
        }
    }
}

/* Adds an activity to the kanban board. Returns True if the activity was added successfully or
returns False otherwise and prints the corresponding error. */
int add_activity(char activity[]) {
    if (is_duplicate_activity(activity)) {
        printf("%s\n", ERROR_DUPLICATE_ACTIVITY);
        return FALSE;

    } else if (!is_valid_activity(activity)) {
        printf("%s\n", ERROR_INVALID_ACTIVITY);
        return FALSE;
        
    } else if (kanban.activities.count >= ACTIVITY_MAX) {
        printf("%s\n", ERROR_TOO_MANY_ACTIVITIES);
        return FALSE;
    }

    strcpy(kanban.activities.activity[kanban.activities.count], activity);
    kanban.activities.count++;
    return TRUE;
}


void handle_command_a(char string[]) {
    unsigned int i;

    if (!argument_exists(string)) {
        for (i = 0; i < kanban.activities.count; i++)
            printf("%s\n", kanban.activities.activity[i]);
    } else {
        get_arguments(string, 1, SPACING);
        add_activity(kanban.arguments[0]);
    }
}

void read_line(char string[]) {
    unsigned int i = 1;  
    char c;
    while ((c = getchar()) != '\n' && i < STRING_MAX) {
        string[i] = c;
        i++;
    }
    string[i] = '\0'; /* Add null byte to the end of the string */
}


/* Checks the user's input and acts accordingly. */
int main() {
    char string[STRING_MAX] = {0};
    char command = '_';  

    initialize_board();

    while ((command = getchar()) != 'q') {

        string[0] = command;

        switch (command)
        {
         case 'q':
            return 0;

        case 't':
            read_line(string);
            handle_command_t(string);
            break;

        case 'l':
            handle_command_l();
            break;

        case 'n':
            read_line(string);
            handle_command_n(string);
            break;

        case 'u':
            read_line(string);
            handle_command_u(string);
            break;

        case 'm':
            read_line(string);
            handle_command_m(string);
            break;

        case 'd':
            read_line(string);
            handle_command_d(string);
            break;

        case 'a':
            read_line(string);
            handle_command_a(string);
            break;

        default:
            printf("Error: Invalid Command: %c\n", command);
            break;
        }
    }

    return 0;
}