#include"header.h"
extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;
// Function: update_status
// Purpose : Updates the corresponding record in the CSV record array (if the date matches current date)
//           with new working hours, IN time, and OUT time.

void update_status(int id_d, float in, float out, float Working_Hours)
{
    for (int i = 0; i < Rcount; i++)
    {
        if ((records[i].id == id_d) && (strcmp(records[i].date, prev_date) == 0))
        {
            printf("Updating time in CSV\n");
            records[i].a = Working_Hours;
            records[i].b = in;
            records[i].c = out;
        }
    }
}


// Function: in_time
// Purpose : Captures current system time as IN time for a user,
//           converts to float (e.g., 10:30 → 10.30), and updates their entry.

void in_time(int id)
{
    char current_time[6]; // Format "HH:MM"
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(current_time, "%02d:%02d", t->tm_hour, t->tm_min);
    printf("%s\n", current_time);

    float in;
    int cnt = 0;
    char *p = strtok(current_time, ":");
    while (p)
    {
        cnt++;
        if (cnt == 1)
            in = ATOF(p);        // Extract hour
        else
            in += ATOF(p) / 100; // Add minute in decimal
        p = strtok(NULL, ":");
    }

    U *temp = head2;
    while (temp)
    {
        if (id == temp->U_ID)
        {
            temp->IN = in;
            printf("Update csv function call\n");
            update_status(temp->U_ID, temp->IN, temp->OUT, temp->Working_Hours);
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("ID is not matched\n");
    }
}


// Function: out_time
// Purpose : Captures system time as OUT time and calculates working hours based on IN time.
//           Updates values only if IN time was already recorded.

void out_time(int id)
{
    char current_time[6]; // Format "HH:MM"
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(current_time, "%02d:%02d", t->tm_hour, t->tm_min);
    printf("%s\n", current_time);

    float out;
    int cnt = 0;
    char *p = strtok(current_time, ":");
    while (p)
    {
        cnt++;
        if (cnt == 1)
            out = ATOF(p);        // Extract hour
        else
            out += ATOF(p) / 100; // Add minute in decimal
        p = strtok(NULL, ":");
    }

    U *temp = head2;
    while (temp)
    {
        if (id == temp->U_ID)
        {
            if (temp->IN > 0)
            {
                temp->OUT = out;
                temp->Working_Hours += (temp->OUT - temp->IN);
                printf("Update csv function call\n");
                update_status(temp->U_ID, temp->IN, temp->OUT, temp->Working_Hours);
                break;
            }
            else
            {
                printf("Without IN time you can't give OUT time\n");
                break;
            }
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("ID is not matched\n");
    }
}

