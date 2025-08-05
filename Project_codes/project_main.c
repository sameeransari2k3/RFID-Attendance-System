
#include"header.h"
extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;

int main()
{
    signal(SIGINT, isr); // Handle Ctrl+C interrupt

    head2 = Sync(head2); // Load user data from "database.txt"
    sync();              // Load attendance data from "attendence.csv"

    puts("Opening serial port\n");

    if ((fd = serialOpen("/dev/ttyUSB0", 9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1;
    }

    puts("Serial port opened\n");
    usleep(1000000); // Give some time for initialization

scan:
    puts("Waiting for data (RFID,Date)...");

    char buffer[100];
    char id[20], date[20];
    int index = 0;

    while (1)
    {
        // Receive RFID,Date until '$' terminator is received
        while (1)
        {
            char ch = serialGetchar(fd);
            if (ch == '$')
            {
                buffer[index] = '\0';
                printf("Received: %s\n", buffer);

                sscanf(buffer, "%[^,],%s", id, date);
                printf("RFID ID : %s\n", id);
                printf("Date    : %s\n", date);
                index = 0;
                break;
            }
            else if (index < sizeof(buffer) - 1)
            {
                buffer[index++] = ch;
            }
        }

        U *temp = head2;
        char parsed_id[10];
        strcpy(parsed_id, rem(id)); // Remove prefix if any
        int numeric_id = atoi(parsed_id);

        // Admin access
        if (id[0] == 'A')
        {
            Admin = numeric_id;
            a_menu();
            goto scan;
        }

        // Date changed → reset data
        if (strcmp(date, prev_date) != 0)
        {
            printf("New date detected. Previous: %s → New: %s\n", prev_date, date);
            strcpy(prev_date, date);

            while (temp)
            {
                temp->Working_Hours = 0;
                temp->IN = 0;
                temp->OUT = 0;
                temp = temp->next;
            }
            goto scan;
        }

        int found = 0;
        temp = head2;

        // Check if ID already exists in attendance records
        for (int i = 0; i < Rcount; i++)
        {
            if (records[i].id == numeric_id)
            {
                // Match found in attendance record
                while (temp)
                {
                    if (temp->U_ID == numeric_id)
                    {
                        serialPutchar(fd, 'W'); // Welcome message
                        if (temp->status == 0)
                        {
                            in_time(numeric_id);
                            temp->status = 1;
                        }
                        else
                        {
                            out_time(numeric_id);
                            temp->status = 0;
                        }
                        found = 1;
                        break;
                    }
                    temp = temp->next;
                }
                break;
            }
        }

        // New user attendance for the day
        if (!found)
        {
            temp = head2;
            while (temp)
            {
                if (temp->U_ID == numeric_id)
                {
                    serialPutchar(fd, 'W'); // Welcome message
                    records = realloc(records, (Rcount + 1) * sizeof(Record));
                    records[Rcount].id = temp->U_ID;
                    strcpy(records[Rcount].name, temp->U_name);
                    strcpy(records[Rcount].date, date);
                    records[Rcount].a = temp->Working_Hours;
                    records[Rcount].b = temp->IN;
                    records[Rcount].c = temp->OUT;
                    Rcount++;

                    in_time(numeric_id);
                    temp->status = 1;
                    found = 1;
                    break;
                }
                temp = temp->next;
            }

            if (!found)
            {
                puts("Invalid ID");
                serialPutchar(fd, 'I'); // Send 'Invalid' signal
            }
        }
    }
}
void isr(int n)
{
    printf("In ISR... (Signal received)\n");

    puts("Saving linked list (head2) to database...");
    save(head2); // Saves user linked list to "database.txt"

    puts("Saving attendance records to CSV...");
    savecsv();   // Saves attendance records to "attendence.csv"

    exit(0); // Cleanly exit the program
}
char *rem(char *name)
{
    memmove(name, name + 1, strlen(name + 1) + 1);  // Remove first character
    name[strlen(name) - 1] = '\0';                  // Remove last character
    return name;
}

