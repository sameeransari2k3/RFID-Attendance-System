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
typedef struct admin
{
	//int A_ID;
	char A_ID[10];
	char A_name[50];
        struct admin *next;
}A;
typedef struct user
{
	int U_ID;
	int status;
	//char U_ID[10];
	char U_name[50];
	float IN;
	float OUT;
	float Working_Hours;
	struct user *next;
}U;
typedef struct
        {
         int id;
         char name[50];
	 char date[50];
         float a, b, c;
        }Record;
Record *records = NULL;
int Rcount;
char prev_date[20];
A *head1=NULL;
U *head2=NULL;
FILE *fp=NULL;
void a_menu();
int Admin;
char *rem(char *name)
{
	memmove(name,name+1,strlen(name+1)+1);
	name[strlen(name)-1]='\0';
	return name;
}
float ATOF(char *str)
{
    int i,index,res=0,flag=0;
    float dec;
    if(str[0]=='-')
    {
        for(i=1;str[i];i++)
        {
            if(str[i]=='.')
            {
                index=i;
                flag=1;
            }
        }
        if(flag)
        {
            for(i=1;i<index;i++)
            {
                res=(res*10)+(str[i]-48);

            }
            for(i=strlen(str)-1;i>index;i--)
            {
                dec=(dec/10)+(str[i]-48);
            }
            return -(res+dec/10);
        }
        else
        {
            for(i=1;str[i];i++)
            {
                res=(res*10)+(str[i]-48);
            }
            return -res;
        }
    }
    else
    {
        for(i=0;str[i];i++)
        {
            if(str[i]=='.')
            {
                index=i;
                flag=1;
            }
        }
        if(flag)
        {
            for(i=0;i<index;i++)
            {
                res=(res*10)+(str[i]-48);

            }
            for(i=strlen(str)-1;i>index;i--)
            {
                dec=(dec/10)+(str[i]-48);
            }
            return res+dec/10;
        }
        else
        {
            for(i=0;str[i];i++)
            {
                res=(res*10)+(str[i]-48);
            }
            return res;
        }

    }
}
/*char *itoa(int n)
{
	char str[5];
	int i=0;
	while(n)
	{
		str[i++]=(n%10)+48;
		n=n/10;
	}
	str[i]='\0';
	return str;
}*/
U *Sync(U *head2)
{
        U *node=NULL,*temp=NULL,u;
        int size=sizeof(U)-sizeof(U*);
        FILE *fp;
        fp=fopen("database.txt","r");
        if(fp==NULL)
        {
                printf("file is not found\n");
		return 0;
        }
        else
        {
             while(fscanf(fp,"%d%d%s%f%f%f",&u.U_ID,&u.status,u.U_name,&u.IN,&u.OUT,&u.Working_Hours)==6)
                {
                        node=malloc(sizeof(U));
                        if(node==NULL)
                        {
                                printf("node is not created\n");
                        }
                        else
                        {
                                *node=u;
				printf("%d , %d , %s , %f , %f , %f \n",u.U_ID,u.status,u.U_name,u.IN,u.OUT,u.Working_Hours);
				sleep(1);
                                node->next=NULL;
                                if(head2==NULL)
                                {
                                        head2=node;
                                        node->next=NULL;
                                }
                                else
                                {
                                        temp=head2;
                                        while(temp->next)
                                        {
                                                temp=temp->next;
                                        }
                                        temp->next=node;
                                }
                        }
                }
        }
        fclose(fp);
        return head2;
}
void save(U *head2)
{
        FILE *fp;
        int size=sizeof(U)-sizeof(U*);
        fp=fopen("database.txt","w");
        while(head2)
	{    
	     printf("%d %d %s %f %f %f\n",head2->U_ID,head2->status,head2->U_name,head2->IN,head2->OUT,head2->Working_Hours);
            fprintf(fp,"%d %d %s %f %f %f\n",head2->U_ID,head2->status,head2->U_name,head2->IN,head2->OUT,head2->Working_Hours);
             head2=head2->next;

        }
}
void *Add_user()
{
        int flag=0;
        char name[20];
        int id;
        U *node=NULL;
        U *temp=NULL;
        node=malloc(sizeof(U));
        temp=head2;
        printf("enter the id:");
//      scanf("%s",node->U_name);
        scanf("%d",&id);
//      printf("enter the name:");
//      scanf("%d",&node->U_ID);
//        scanf("%s",name);
        node->IN=0;
        node->OUT=0;
        node->Working_Hours=0;
        node->next=NULL;
	if(id==Admin)
	{
		printf("You cant add this is admin id\n");
		return head2;
	}
        while(temp)
        {
                if(temp->U_ID==id)
                {
                        flag=1;
                        printf("%d id is already existing\n",id);
                        break;
                }
                temp=temp->next;
        }
        if(flag==0)
        {
                node->U_ID=id;
                printf("enter the emp name:");
                scanf("%s",node->U_name);
        }
        if(flag==0)
        {
                if(head2==NULL)
                {
                        head2=node;
                }
                else
                {
                        node->next=head2;
                        head2=node;

                }
        }
        return head2;
}
void removeid(int id)
{
	for(int i=0;i<Rcount;i++)
	{
		if(records[i].id == id)
		{
			memmove(records+i,records+1+i,(Rcount-i+1)*sizeof(Record));
			Rcount--;
			printf("ID deleted\n");
			break;
		}
	}
}
void savecsv(void);
void *Delete_user()
{
    int id, deleted = 0;
    printf("enter the id:");
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
                        printf("Id found\n");
                    if (records[i].id == id)
                    {
                        memmove(records + i, records + i + 1, (Rcount - i - 1) * sizeof(Record));
                        Rcount--;
                        printf("ID deleted from records\n");
                        savecsv();
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

                // Also delete from records array
                for (int i = 0; i < Rcount; i++)
                {
		 	printf("Id found\n");
                    if (records[i].id == id)
                    {
                        memmove(records + i, records + i + 1, (Rcount - i - 1) * sizeof(Record));
                        Rcount--;
                        printf("ID deleted from records\n");
			savecsv();
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

/*void *Delete_user()
{
	int id;
	printf("enter the id:");
	scanf("%d",&id);
	U *temp=head2;
	U *pre=head2;
	if(head2->U_ID==id)
        {
                head2=head2->next;

        }
	else
	{
        	while(temp)
        	{
                	if(temp->U_ID==id)
                	{
                        	pre->next=temp->next;
                        	temp=NULL;
                        	free(temp);
				for(int i=0;i<Rcount;i++)
       				 {
                			if(records[i].id == id)
                			{
                        			memmove(records+i,records+1+i,(Rcount-i+1)*sizeof(Record));
                        			Rcount--;
                        			printf("ID deleted\n");
                        			break;
                			}
        			}
				break;
                	}
			pre=temp;
			temp=temp->next;
	        }
	}
	if(temp==NULL)
	{
		printf("id is not matched\n");
	}
//	save(head2);
	return head2;
}*/
void Edit_user()
{
//	char ID[10];
	int id;
	printf("enter the id:");
	scanf("%d",&id);
//	strcpy(ID,itoa(id));
	U *temp=head2,*temp2=NULL;
	U *pre=head2;
	while(temp)
	{
		//printf("validating id\n");
		if(temp->U_ID==id)
		{
			printf("enter the correct name:");
			scanf("%s",temp->U_name);
			char ch;
			for(int i=0;i<Rcount;i++)
			{
				if(records[i].id == id)
				{
					printf("id  matched\n");
            				strcpy(records[i].name,temp->U_name);
        			}
		
    			}
		}
			break;
		temp=temp->next;
	}
	if(temp==NULL)
	{
		printf("ID is not found %d\n",id);
		return;
	}	
}
void Print_user()
{
	U *temp=head2;
	printf("U_ID\t U_name\t IN\t OUT\t W_H\n"); 
	while(temp)
	{
		printf("%d\t%s\t%0.2f  \t%0.2f\t %0.2f\n",temp->U_ID,temp->U_name,temp->IN,temp->OUT,temp->Working_Hours);
	 //printf("%d\t%s\t%s  \t%s\t %f\n",temp->U_ID,temp->U_name,temp->IN,temp->OUT,temp->Working_Hours);

		temp=temp->next;
	}
}
void a_menu()
{
/*	char str[50];
	int id,ch;
l1:	printf("enter the name:");
	scanf("%s",str);
	if(strcmp(str,head1->A_name)==0)
	{
l:	printf("enter the ID:");
		scanf("%d",&id);
		if(id==head1->A_ID)
		{*/
	int ch;
			while(1)
			{
				printf("---------Admin_menu-----------\n");
				printf("1-Add user,2-Delete user 3-edit user 4-print 5-quit\n");
				printf("enter your choice:");
				scanf("%d",&ch);
				switch(ch)
				{
					case 1:
						head2=Add_user();
						break;
					case 2:
						head2=Delete_user();
						break;
					case 3:
						Edit_user();
						break;
					case 4:
						Print_user();
						break;
					case 5:
						//save(head2);
						return;
					default:
						printf("INVALID CHOICE\n");
				}
			}

	/*	else
		{
			printf("Invalid id\n");
			goto l;
		}*/
	
/*	else
	{
		printf("Invalid admin name\n");
		goto l1;
	}*/

}
void update_status(int id_d,float in,float out,float Working_Hours)
{
     
        for (int i = 0; i < Rcount; i++)
        {
	  if((records[i].id==id_d)&&(strcmp(records[i].date,prev_date)==0))
	  {
		  printf("Updatind time in CSV\n");
                  records[i].a = Working_Hours;
                  records[i].b = in;
                  records[i].c = out;

	  }
	}
}

void in_time(int id)
{
	char current_time[6]; // "HH:MM"
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	sprintf(current_time, "%02d:%02d", t->tm_hour, t->tm_min);
	printf("%s\n",current_time);
	float in;
	char *p=current_time;
        int cnt=0;
        p=strtok(p,":");
        while(p)
        {
            cnt++;
            if(cnt==1)
            {
                in=ATOF(p);
            }
            else
            {
                in+=ATOF(p)/100;
            }
            p=strtok(NULL,":");
        }
//	int id;
//	printf("enter the ID:");
//	scanf("%d",&id);
	U *temp=head2;
	while(temp)
	{
		if(id==temp->U_ID)
		{
			temp->IN=in;
			printf("Update csv function call\n");
			update_status(temp->U_ID,temp->IN,temp->OUT,temp->Working_Hours);
			//strcat(itoa(t->tm_hour),".");
		//	strcat(itoa(t->tm_hour),itoa(t->tm_min));
			//strcpy(temp->IN,current_time);
		//        printf("enter the IN Time:");
                  //      scanf("%f",&temp->IN);
			break;
		}
		temp=temp->next;
	}
	if(temp==NULL)
	{
		printf("id is not matched\n");
	}
//	save(head2);
}
void out_time(int id)
{
      //  int id;
        //printf("enter the ID:");
       // scanf("%d",&id);
        char current_time[6]; // "HH:MM"
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        sprintf(current_time, "%02d:%02d", t->tm_hour, t->tm_min);
        printf("%s\n",current_time);
	char *p=current_time;
        int cnt=0;
	float out;
        p=strtok(p,":");
        while(p)
        {
            cnt++;
            if(cnt==1)
            {
                out=ATOF(p);
            }
            else
            {
                out+=ATOF(p)/100;
            }
            p=strtok(NULL,":");
        }

        U *temp=head2;
        while(temp)
        {
                if(id==temp->U_ID)
                {
			if(temp->IN>0)
			{
				temp->OUT=out;
				//printf("enter the OUT Time:");
				//scanf("%f",&temp->OUT);
				temp->Working_Hours+=(temp->OUT)-(temp->IN);
				printf("Update csv function call\n");
				update_status(temp->U_ID,temp->IN,temp->OUT,temp->Working_Hours);
				break;
			}
			else
			{
				printf("Without IN time you can't give OUT time\n");
				break;
			}	
                }
                temp=temp->next;
        }
        if(temp==NULL)
        {
                printf("id is not matched\n");
        }
//	save(head2);
}
/*void u_menu()
{
	int ch;
	while(1)
	{
		printf("1-IN time 2-OUT time 3-quit\n");
		printf("enter your choice:");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				in_time();
				break;
			case 2:
				out_time();
				break;
			case 3:
				return;
			default:
				printf("INVALID CHOICE\n");
		}
	}
}*/
/*int compare(char *name)
{
	U *temp=head2;
	while(temp)
	{
		if(strcmp(temp->U_name==name)==0)
		{
			return 1;
		}
		temp=temp->next;
	}
	return 0;
}*/
void sync()
{
	fp=fopen("attendence.csv","r");
        if(fp==NULL)
        {
                printf("file is not found\n");
                return ;
        }
        int id;
        char name[50];
        float a, b, c;
	char ch;
	char date[20];
	U *temp=head2;
         while ((ch = fgetc(fp)) != EOF)
         {
              if (ch == '\n') break;
         }
         while (fscanf(fp, "%d,%[^,],%[^,],%f,%f,%f", &id, name, date,&a, &b, &c) == 6)
          {
                        printf("inside while loop\n");
                                records = realloc(records, (Rcount + 1) *sizeof(Record));
                                records[Rcount].id = id;
                                strcpy(records[Rcount].name, name);
				strcpy(records[Rcount].date, date);
				strcpy(prev_date,date);
                                records[Rcount].a = a;
                                records[Rcount].b = b;
                                records[Rcount].c = c;
                                Rcount++;
          }
	fclose(fp);
        /*fp=NULL;
        fp = fopen("attendence.csv", "w");
        if (!fp)
        {
                printf("Error opening file for writing.\n");
                return ;
        }
        fprintf(fp, "ID,Name,Working_hour,IN,OUT\n");
        for (int i = 0; i < count; i++)
        {
                printf("copying content\n");
          fprintf(fp, "%d,%s,%.2f,%.2f,%.2f\n",records[i].id,records[i].name,records[i].a,records[i].b,records[i].c);
        }*/
   
}
void savecsv()
{
	fp=NULL;
        fp = fopen("attendence.csv", "w");
        if (!fp)
        {
                printf("Error opening file for writing.\n");
                return ;
        }
        fprintf(fp, "ID,Name,Date,Working_hour,IN,OUT\n");
        for (int i = 0; i < Rcount; i++)
        {
          fprintf(fp, "%d,%s,%s,%.2f,%.2f,%.2f\n",records[i].id,records[i].name,records[i].date,records[i].a,records[i].b,records[i].c);
        }
	printf("File save\n");
	fclose(fp);
}
int fd;
void isr(int n)
{
        printf("in isr.........\n");
        puts("head2 saving.....");
        save(head2);
        puts("savecsv.....");
        savecsv();
        exit(0);

}
int main()
{
	A *node=calloc(1,sizeof(A));
	signal(SIGINT,isr);
//	node->A_ID=786;
//	strcpy(node->A_name,"ADMIN");
//	head1=node;
        //sink
       // head=sync(head);
      //  head1=sync(head1);
        //FILE *fp=NULL;
	head2=Sync(head2);
	sync();
        int tx,rx ;
        puts("Opening serial port\n");

        if ((fd = serialOpen ("/dev/ttyUSB0",9600)) < 0)
        {

                fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
                return 1 ;
        }

        puts("serial port is opened\n");

        usleep(1000000);

         if ((fd = serialOpen("/dev/ttyUSB0", 9600)) < 0) 
	 {
        	fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        	return 1;
   	 }

scan: puts("Serial port opened. Waiting for data (RFID,Date)...");

    	char buffer[100];
    	int index = 0;
	char id[20],date[20];
	while(1)
	{
    		while (1)
    	   {
        	char ch = serialGetchar(fd);

        	if (ch == '$')
        	{
           		 buffer[index] = '\0';
            		printf("Received: %s\n", buffer);

            		char rfid[30], date1[20];
            		sscanf(buffer,"%[^,],%s", rfid, date1);

            		printf("RFID ID : %s\n", rfid);
            		printf("Date    : %s\n", date1);
			strcpy(id,rfid);
			strcpy(date,date1);
            		index = 0;
	    		break;
        	}
        	else
        	{
            		if (index < sizeof(buffer) - 1)
                	buffer[index++] = ch;
       		}
    	  }
		char f=1;
//		printf("1-Admin 2-Emp 3-exit\n");
/*		char date[20],id[10];
		printf("enter your id:");
               __fpurge(stdin);
		scanf("%s",id);
		 printf("enter the date:");
               __fpurge(stdin);
                scanf("%s",date);*/
		 U *temp=head2;
		 char kk[10];
		 int i;
		// strcpy(kk,rem(id));
		// printf("%s\n",kk);
		 if(id[0]=='A')
		 {
			 Admin=atoi(rem(id));
			 a_menu();
			 goto scan;
			 //Admin=atoi(rem(id));
		 }
		 else
		 {	
			 //SerialPutstring(fd,"Welcome!!");
			 //serialPutchar (fd,'W');
			  char kk[10];
                          strcpy(kk,rem(id));
			  printf("id=%s\n",kk);
			if(strcmp(date,prev_date)!=0)
                         {
				 printf("%s\n",prev_date);
				 strcpy(prev_date,date);
				 while(temp)
				 {
					 temp->Working_Hours=0;
					 temp->IN=0;
					 temp->OUT=0;
					 temp=temp->next;
				 }
				 temp=head2;
                                 goto l1;
                         }
			for(i=0;i<Rcount;i++)
			{
				if(records[i].id==atoi(kk))
				{
					while(temp)
					{
						if(temp->U_ID==atoi(kk))
						{
							serialPutchar (fd,'W');
							if(temp->status==0)
							{
								//temp->IN=10.30;
								in_time(atoi(id));
								temp->status=1;
							}
							else
							{
								//temp->OUT=12.30;
								out_time(atoi(id));
								temp->status=0;
							}
							break;
						}
						temp=temp->next;
					}
					printf("ID Exist\n");
				//	save(head2);
					break;
				}
			}
		 }
		       if(i==Rcount)
		     {
				
				l1: while(temp)
        	     	{
				
                	if(temp->U_ID==atoi(kk))
                	{
				serialPutchar (fd,'W');
				//temp->IN=1;
 //                       	fprintf(fp,"%d,%s,%0.2f,%0.2f,%0.2f\n",temp->U_ID,temp->U_name,temp->Working_Hours,temp->IN,temp->OUT);
                        	records = realloc(records, (Rcount + 1) *sizeof(Record));
                                records[Rcount].id = temp->U_ID;
                                strcpy(records[Rcount].name,temp->U_name);
				strcpy(records[Rcount].date,date);
                                records[Rcount].a = temp->Working_Hours;
                                records[Rcount].b = temp->IN;
                                records[Rcount].c = temp->OUT;
                                Rcount++;
				in_time(atoi(id));
				temp->status=1;
				//save(head2);
				break;
                	}
               			 temp=temp->next;
        	  	 }
			if(temp==NULL)
			{
				puts("Invalid Id\n");
			//	SerialPutstring(fd,"Invalid Id");
				serialPutchar (fd,'I');
			}
		   }
		 
	}
}

