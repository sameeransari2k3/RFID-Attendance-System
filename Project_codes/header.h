#ifndef header
#define header
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio_ext.h>
#include<string.h>
#include<time.h>
#include<signal.h>
#include<errno.h>
#include<stdint.h>
#include<stdarg.h>
#include"uart.h"

// Structure for Admin
typedef struct admin {
    char A_ID[10];
    char A_name[50];
    struct admin *next;
} A;

// Structure for User
typedef struct user {
    int U_ID;
    int status;
    char U_name[50];
    float IN;
    float OUT;
    float Working_Hours;
    struct user *next;
} U;

// Structure for CSV record
typedef struct {
    int id;
    char name[50];
    char date[50];
    float a, b, c;
} Record;

//Global Variables
/*extern Record *records;
extern int Rcount;
extern char prev_date[20];
extern A *head1;
extern U *head2 ;
extern FILE *fp;
extern int fd;
extern int Admin;
*/
//Functions prototypes for RFID based Attendence system
char *rem(char *name); // Removes unwanted characters from RFID input
float ATOF(char *str); // Custom ASCII-to-float conversion
void save(U *head2); // Saves users to file
U *Sync(U *head2); // Loads users from file
void update_status(int id, float in, float out, float hours); // Updates record in memory
void in_time(int id); // Captures and stores IN time
void out_time(int id); // Captures and stores OUT time

//ADMIN functions
void *Add_user(); // Add a new user
void *Delete_user(); // Delete a user and update memory/CSV
void Edit_user(); // Edit user name
void Print_user(); // Print all users with attendance details
		   
//Admin Menu
void a_menu(); // CLI admin operations (Add, Delete, Edit, Print)

//CSV Synchronization and saving
void sync(); // Load attendance from CSV into memory
void savecsv(); // Saves records to CSV
	     
//Signal Handling
void isr(int n); // Save data and exit safely on SIGINT (Ctrl+C)
#endif
