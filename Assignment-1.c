#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// This struct contains the basic values of an event
typedef struct Event
{
    int fromHour;
    int toHour;
    int date;
    char name[1000];
    struct Event* next;
} event;

// This struct contains the three basic values of a venue
typedef struct Venue
{
    char name[1000];
    char location[1000];
    int capacity;
    struct Venue* next;
    int noOfNodes;
    int noOfEvents;
    event* calendar[31];
} venue;

// Function for comparing strings
bool stringCompare(char *a, char *b)
{
    return strcmp(a, b) == 0;
}

// Function to add a venue
void addVenue(venue** head, char* name, char* location, int capacity)
{
    for (venue* i = *head; i != NULL; i = i->next)
    {
        if (stringCompare(i->name, name))
        {
            //printf("-1\nDuplicate name!!\n");
            printf("-1\nError\n");
            return;
        }
    }

    venue* newVenue = (venue*)malloc(sizeof(venue));
    if (newVenue == NULL)
    {
        printf("-1\nError\n");
        return;
    }
    newVenue->capacity = capacity;
    strcpy(newVenue->location, location);
    strcpy(newVenue->name, name);
    newVenue->next = NULL;

    if (*head == NULL)
    {
        *head = newVenue;
        newVenue->noOfNodes = 1;
    }
    else
    {
        // Tail of Linked list
        venue* i = *head;
        while (i->next != NULL)
        {
            i = i->next;
        }
        i->next = newVenue;
        newVenue->noOfNodes = 1;
        // Update the noOfNodes for all nodes
        i = *head;
        while (i != NULL)
        {
            i->noOfNodes += 1;
            i = i->next;
        }
    }
    // Set all node's noOfEvents to 0
    newVenue->noOfEvents = 0;
    // Initialise them all to null
    for (int j = 0; j < 31; j++)
    {
        newVenue->calendar[j] = NULL;
    }
    printf("0\n");
}

// Function to delete a venue
void deleteVenue(venue** head, char* name)
{
    venue* i = *head;
    venue* previous = NULL;

    while (i != NULL && strcmp(i->name, name) != 0)
    {
        previous = i;
        i = i->next;
    }

    if (i == NULL)
    {
        printf("-1\nError\n", name);
        return;
    }

    if (previous == NULL)
    {
        *head = i->next;
    }
    else
    {
        previous->next = i->next;
    }

    // Free events associated with the venue
    for (int j = 0; j < 31; j++)
    {
        event* e = i->calendar[j];
        while (e != NULL)
        {
            event* temp = e;
            e = e->next;
            free(temp);
        }
    }

    free(i);

    // Update the noOfNodes for the remaining nodes
    i = *head;
    while (i != NULL)
    {
        i->noOfNodes--;
        i = i->next;
    }
    printf("0\n");
}

// Function to show all venues
void showVenues(venue** head)
{
    venue* i = *head;
    if (i == NULL)
    {
        printf("0\n");
    }
    else
    {
        printf("%d\n", i->noOfNodes);
    }
    while (i != NULL)
    {
        printf("%s %s %d\n", i->name, i->location, i->capacity);
        i = i->next;
    }
}

// Function to free the memory allocated for the linked list
void freeVenues(venue* head)
{
    venue* i = head;
    while (i != NULL)
    {
        // Free events associated with the venue
        for (int j = 0; j < 31; j++)
        {
            event* e = i->calendar[j];
            while (e != NULL)
            {
                event* temp = e;
                e = e->next;
                free(temp);
            }
        }
        venue* next = i->next;
        free(i);
        i = next;
    }
}

// Function to check which command has been inputted
bool commandCheck(char *sentence, char *word)
{
    char command[100];
    // Use sscanf to extract the first word from the sentence
    sscanf(sentence, "%s", command);
    // Compare the extracted first word with the given word
    return strcmp(command, word) == 0;
}



//Function to add an event
void addEvent(venue** head, char* venueName, int date, int fromHour, int toHour, char* eventName)
{
    if (head == NULL || *head == NULL)
    {
        printf("-1\nError\n");
        return;
    }
    
    venue* i = *head;
    while (i != NULL)
    {
        if (strcmp(i->name, venueName) == 0)
        {
            break;
        }
        i = i->next;
    }
    if (i == NULL)
    {
        printf("-1\nError\n");
        return;
    }

    if (date < 1 || date > 30 || fromHour < 0 || fromHour >= 24 || toHour <= fromHour || toHour > 24)
    {
        printf("-1\nError\n");
        return;
    }

    event* current = i->calendar[date];
    while (current != NULL)
    {
        if ((fromHour >= current->fromHour && fromHour < current->toHour) ||
            (toHour > current->fromHour && toHour <= current->toHour) ||
            (fromHour <= current->fromHour && toHour >= current->toHour))
        {
            printf("-1\nError\n");
            return;
        }
        current = current->next;
    }

    event* newEvent = (event*)malloc(sizeof(event));
    if (newEvent == NULL)
    {
        printf("-1\nError\n");
        return;
    }
    newEvent->date = date;
    newEvent->fromHour = fromHour;
    newEvent->toHour = toHour;
    strcpy(newEvent->name, eventName);
    newEvent->next = NULL;

    // Insert the new event in sorted order based on the start time
    event** p = &(i->calendar[date]);
    while (*p != NULL && (*p)->fromHour < fromHour)
    {
        p = &((*p)->next);
    }
    newEvent->next = *p;
    *p = newEvent;

    i->noOfEvents += 1;
    printf("0\n");
}


// Function to delete an event
void delEvent(venue** head, char* venueName, int date, int fromHour, char* eventName)
{
    if (head == NULL || *head == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }
    venue* i = *head;
    while (i != NULL)
    {
        if (strcmp(i->name, venueName) == 0)
        {
            break;
        }
        i = i->next;
    }
    if (i == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }

    if (date < 1 || date > 30 || fromHour < 0 || fromHour >= 24)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }

    event* current = i->calendar[date];
    event* prev = NULL;
    while (current != NULL && (strcmp(current->name, eventName) != 0 || current->fromHour != fromHour))
    {
        prev = current;
        current = current->next;
    }
    if (current == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }
    if (prev == NULL)
    {
        i->calendar[date] = current->next;
    }
    else
    {
        prev->next = current->next;
    }
    free(current);
    i->noOfEvents -= 1;
    printf("0\n");
}

// Function to show all events for a specific date
void showEvents(venue** head, char* venueName, int date)
{
    if (head == NULL || *head == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }
    venue* i = *head;
    while (i != NULL)
    {
        if (strcmp(i->name, venueName) == 0)
        {
            break;
        }
        i = i->next;
    }
    if (i == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }

    if (date < 1 || date > 30)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }

    event* current = i->calendar[date];
    if (current == NULL)
    {
        printf("0\n");
        return;
    }

    int count = 0;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    printf("%d\n", count);
    current = i->calendar[date];
    while (current != NULL)
    {
        printf("%s %d %d\n", current->name, current->fromHour, current->toHour);
        current = current->next;
    }
}

// Function to show the calendar of a venue
void showCalendar(venue** head, char* venueName)
{
    if (head == NULL || *head == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }
    venue* i = *head;
    while (i != NULL)
    {
        if (strcmp(i->name, venueName) == 0)
        {
            break;
        }
        i = i->next;
    }
    if (i == NULL)
    {
        printf("-1\n");
        printf("Error\n");
        return;
    }

    int totalEvents = i->noOfEvents;
    printf("%d\n", totalEvents);

    for (int j = 1; j <= 30; j++)
    {
        event* current = i->calendar[j];
        int count = 0;
        while (current != NULL)
        {
            count++;
            current = current->next;
        }
        printf("%d %d\n", j, count);
        current = i->calendar[j];
        while (current != NULL)
        {
            printf("%s %d %d\n", current->name, current->fromHour, current->toHour);
            current = current->next;
        }
    }
}

void trimLeadingWhitespace(char* str)
{
    int index = 0, i;

    // Find the first non-whitespace character
    while (isspace(str[index]))
    {
        index += 1;
    }

    // Shift all characters to the start of the string
    for (i = 0; str[index] != '\0'; i++, index++)
    {
        str[i] = str[index];
    }
    str[i] = '\0'; // Null terminate the string
}


int main()
{
    char command[1000];
    venue* vhead = NULL;
    char* end = "End";
    // For comparison of the command entered
    char* addv = "addVenue";
    char* delv = "delVenue";
    char* showv = "showVenues";
    char* adde = "addEvent";
    char* dele = "delEvent";
    char* showe = "showEvents";
    char* showc = "showCalendar";

    // Using do-while loop because this should execute at least one time before the user inputs anything.
    do
    {
        fgets(command, 1000, stdin);
        // Remove the newline character from the command for properly comparing the strings
        command[strcspn(command, "\n")] = '\0';
        // Trim the leading whitespaces
        trimLeadingWhitespace(command);

        // First check if the command is end or not.
        if (stringCompare(command, end))
        {
            break;
        }

        if (commandCheck(command, addv))
        {
            // Call addVenue
            char name[1000];
            char location[1000];
            int capacity;
            sscanf(command, "addVenue \"%[^\"]\" \"%[^\"]\" %d", name, location, &capacity);
            //if(validate_addVenue_input(command,name,location,&capacity))
            //{
                addVenue(&vhead, name, location, capacity);
            //}
        }
        else if (commandCheck(command, delv))
        {
            char name[1000];
            sscanf(command, "delVenue \"%[^\"]\"", name);
            deleteVenue(&vhead, name);
        }
        else if (commandCheck(command, showv))
        {
            // Call showVenues
            showVenues(&vhead);
        }
        else if (commandCheck(command, adde))
        {
            // Call addEvent
            char venueName[1000];
            int date, fromHour, toHour;
            char eventName[1000];
            sscanf(command, "addEvent \"%[^\"]\" %d %d %d \"%[^\"]\"", venueName, &date, &fromHour, &toHour, eventName);
            addEvent(&vhead, venueName, date, fromHour, toHour, eventName);
        }
        else if (commandCheck(command, dele))
        {
            // Call delEvent
            char venueName[1000];
            int date, fromHour;
            char eventName[1000];
            sscanf(command, "delEvent \"%[^\"]\" %d %d \"%[^\"]\"", venueName, &date, &fromHour, eventName);
            delEvent(&vhead, venueName, date, fromHour, eventName);
        }
        else if (commandCheck(command, showe))
        {
            // Call showEvents
            char venueName[1000];
            int date;
            sscanf(command, "showEvents \"%[^\"]\" %d", venueName, &date);
            showEvents(&vhead, venueName, date);
        }
        else if (commandCheck(command, showc))
        {
            // Call showCalendar
            char venueName[1000];
            sscanf(command, "showCalendar \"%[^\"]\"", venueName);
            showCalendar(&vhead, venueName);
        }
        else
        {
            printf("-1\nError\n");
        }
    } while (1);

    freeVenues(vhead);
    return 0;
}
    
