#include"header.h"
extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;




// Function: savecsv
// Purpose : Writes all attendance records stored in the `records` array into the "attendence.csv" file.
//           The CSV file contains columns for ID, Name, Date, Working Hour, IN time, and OUT time.

void savecsv()
{
    fp = NULL;
    fp = fopen("attendence.csv", "w");
    if (!fp)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write CSV header
    fprintf(fp, "ID,Name,Date,Working_hour,IN,OUT\n");

    // Write each record line-by-line
    for (int i = 0; i < Rcount; i++)
    {
        fprintf(fp, "%d,%s,%s,%.2f,%.2f,%.2f\n",
            records[i].id,
            records[i].name,
            records[i].date,
            records[i].a,
            records[i].b,
            records[i].c);
    }

    printf("File save\n");
    fclose(fp);
}


// Function: save
// Purpose : Saves all user data currently in the linked list `head2` into a file named "database.txt".
//           The file is overwritten each time with the latest user information.

void save(U *head2)
{
    FILE *fp;
    int size = sizeof(U) - sizeof(U*);
    fp = fopen("database.txt", "w");

    if (!fp)
    {
        printf("Error opening database.txt for writing.\n");
        return;
    }

    // Traverse linked list and write user data to the file
    while (head2)
    {
        printf("%d %d %s %f %f %f\n",
            head2->U_ID,
            head2->status,
            head2->U_name,
            head2->IN,
            head2->OUT,
            head2->Working_Hours);

        fprintf(fp, "%d %d %s %f %f %f\n",
            head2->U_ID,
            head2->status,
            head2->U_name,
            head2->IN,
            head2->OUT,
            head2->Working_Hours);

        head2 = head2->next;
    }

    fclose(fp);
}

