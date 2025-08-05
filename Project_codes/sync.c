#include"header.h"
extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;

// Function: Sync
// Purpose:  This function reads user data from "database.txt" and stores it in a linked list of type U (User).
//           It returns the head pointer to the updated linked list.
//
U *Sync(U *head2)
{
    U *node = NULL, *temp = NULL, u;
    int size = sizeof(U) - sizeof(U*);
    FILE *fp;

    fp = fopen("database.txt", "r");
    if (fp == NULL)
    {
        printf("file is not found\n");
        return 0;
    }
    else
    {
        // Read each line and populate the linked list
        while (fscanf(fp, "%d%d%s%f%f%f", &u.U_ID, &u.status, u.U_name, &u.IN, &u.OUT, &u.Working_Hours) == 6)
        {
            node = malloc(sizeof(U));
            if (node == NULL)
            {
                printf("node is not created\n");
            }
            else
            {
                *node = u;
                printf("%d , %d , %s , %f , %f , %f \n", u.U_ID, u.status, u.U_name, u.IN, u.OUT, u.Working_Hours);
                sleep(1);  // Just for visualization delay
                node->next = NULL;

                if (head2 == NULL)
                {
                    head2 = node;
                }
                else
                {
                    temp = head2;
                    while (temp->next)
                    {
                        temp = temp->next;
                    }
                    temp->next = node;
                }
            }
        }
    }
    fclose(fp);
    return head2;
}

//
// Function: sync
// Purpose:  This function reads attendance records from "attendence.csv" and stores them in a dynamic array of Record.
//           It also stores the latest date in 'prev_date' for later comparisons.
//
void sync()
{
    FILE*fp = fopen("attendence.csv", "r");
    if (fp == NULL)
    {
        printf("file is not found\n");
        return;
    }

    int id;
    char name[50];
    float a, b, c;
    char ch;
    char date[20];
    U *temp = head2;

    // Skip the header line in CSV
    while ((ch = fgetc(fp)) != EOF)
    {
        if (ch == '\n') break;
    }

    // Read the records line by line
    while (fscanf(fp, "%d,%[^,],%[^,],%f,%f,%f", &id, name, date, &a, &b, &c) == 6)
    {
        printf("inside while loop\n");

        records = realloc(records, (Rcount + 1) * sizeof(Record));
        records[Rcount].id = id;
        strcpy(records[Rcount].name, name);
        strcpy(records[Rcount].date, date);
        strcpy(prev_date, date); // Save last read date
        records[Rcount].a = a;
        records[Rcount].b = b;
        records[Rcount].c = c;
        Rcount++;
    }

    fclose(fp);
}

