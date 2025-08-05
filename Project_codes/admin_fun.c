#include "header.h"
extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;

// Function: Add_user
// Purpose : Adds a new user to the user linked list (head2) after checking if the ID already exists.
//           Prevents adding if the ID is that of an admin or already in use.

void *Add_user()
{
    int flag = 0;
    char name[20];
    int id;
    U *node = NULL;
    U *temp = NULL;
    node = malloc(sizeof(U));
    temp = head2;

    printf("Enter the ID: ");
    scanf("%d", &id);

    node->IN = 0;
    node->OUT = 0;
    node->Working_Hours = 0;
    node->next = NULL;

    if (id == Admin)
    {
        printf("You can't add this; it's the admin ID.\n");
        return head2;
    }

    while (temp)
    {
        if (temp->U_ID == id)
        {
            flag = 1;
            printf("%d ID is already existing\n", id);
            break;
        }
        temp = temp->next;
    }

    if (flag == 0)
    {
        node->U_ID = id;
        printf("Enter the employee name: ");
        scanf("%s", node->U_name);

        if (head2 == NULL)
        {
            head2 = node;
        }
        else
        {
            node->next = head2;
            head2 = node;
        }
    }

    return head2;
}


// Function: removeid
// Purpose : Removes a record with the given ID from the `records` array.

void removeid(int id)
{
    for (int i = 0; i < Rcount; i++)
    {
        if (records[i].id == id)
        {
            memmove(records + i, records + i + 1, (Rcount - i - 1) * sizeof(Record));
            Rcount--;
            printf("ID deleted\n");
            break;
        }
    }
}


// Function: Delete_user
// Purpose : Deletes a user from the linked list and removes related records from the `records` array.

void *Delete_user()
{
    int id, deleted = 0;
    printf("Enter the ID: ");
    scanf("%d", &id);

    U *temp = head2;
    U *pre = head2;

    // If head node matches
    if (head2 && head2->U_ID == id)
    {
        U *toDelete = head2;
        head2 = head2->next;
        free(toDelete);
        deleted = 1;

        for (int i = 0; i < Rcount; i++)
        {
            if (records[i].id == id)
            {
                printf("ID found in records\n");
                memmove(records + i, records + i + 1, (Rcount - i - 1) * sizeof(Record));
                Rcount--;
                savecsv();
                printf("ID deleted from records\n");
                break;
            }
        }
    }
    else
    {
        while (temp)
        {
            if (temp->U_ID == id)
            {
                pre->next = temp->next;
                free(temp);
                deleted = 1;

                for (int i = 0; i < Rcount; i++)
                {
                    if (records[i].id == id)
                    {
                        printf("ID found in records\n");
                        memmove(records + i, records + i + 1, (Rcount - i - 1) * sizeof(Record));
                        Rcount--;
                        savecsv();
                        printf("ID deleted from records\n");
                        break;
                    }
                }
                printf("ID deleted from database\n");
                break;
            }
            pre = temp;
            temp = temp->next;
        }
    }

    if (!deleted)
    {
        printf("ID is not matched\n");
    }

    return head2;
}


// Function: Edit_user
// Purpose : Updates the name of a user and also updates the corresponding record if it exists.

void Edit_user()
{
    int id;
    printf("Enter the ID: ");
    scanf("%d", &id);

    U *temp = head2;

    while (temp)
    {
        if (temp->U_ID == id)
        {
            printf("Enter the correct name: ");
            scanf("%s", temp->U_name);

            for (int i = 0; i < Rcount; i++)
            {
                if (records[i].id == id)
                {
                    printf("ID matched in records\n");
                    strcpy(records[i].name, temp->U_name);
                }
            }
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("ID is not found: %d\n", id);
    }
}


// Function: Print_user
// Purpose : Displays all user records in a tabular format.

void Print_user()
{
    U *temp = head2;
    printf("U_ID\t U_name\t IN\t OUT\t W_H\n");
    while (temp)
    {
        printf("%d\t%s\t%0.2f\t%0.2f\t%0.2f\n",
               temp->U_ID,
               temp->U_name,
               temp->IN,
               temp->OUT,
               temp->Working_Hours);
        temp = temp->next;
    }
}


// Function: a_menu
// Purpose : Displays the admin menu and performs actions like add, delete, edit, and print user data.

void a_menu()
{
    int ch;
    while (1)
    {
        printf("--------- Admin Menu ---------\n");
        printf("1 - Add user\n");
        printf("2 - Delete user\n");
        printf("3 - Edit user\n");
        printf("4 - Print all users\n");
        printf("5 - Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);

        switch (ch)
        {
            case 1:
                head2 = Add_user();
                break;
            case 2:
                head2 = Delete_user();
                break;
            case 3:
                Edit_user();
                break;
            case 4:
                Print_user();
                break;
            case 5:
                return;
            default:
                printf("INVALID CHOICE\n");
        }
    }
}


