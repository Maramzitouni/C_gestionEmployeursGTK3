#include<stdio.h>

#include<curses.h>
//#include <gtk/gtk.h>
#include<string.h>

#include<unistd.h>

#include<stdlib.h>
#include <mysql/mysql.h>
#include <mysql.h>
struct contact

{

    int phonenumber;

    char fullname[20],address[20],emailaddress[30];

} listing;

char query[20],fullname[20],name[20];

int i,j,ch,l,found,ch1;



int main() {
    char server[16] = "localhost";
    char username[16] = "root";
    char password[16] = "";
    char database[16] = "projetC";
    char username1[20],password1[20] ;
    char userid[20];

    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW record;
    MYSQL_ROW record1;
    MYSQL_ROW record2;
    MYSQL_ROW record4;
    MYSQL_ROW record6;

    if (mysql_real_connect(conn, server, username, password, database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(conn);
        return 1;
    }

    login:

    printf("\n\t\t\t\t\t\t\t\t################################################");
    printf("\n\t\t\t\t\t\t\t\t#    LOGIN      #");
    printf("\n\t\t\t\t\t\t\t\t################################################ \n");
    printf("\n\t\t\t\t\t\t\t\t| [1] LOGIN                      |");
    printf("\n\t\t\t\t\t\t\t\t| [0] Exit                                     |\n\t\t\t\t\t\t\t\t________________________________________________\n\t\t\t\t\t\t\t\t");

    printf("\n\t\t\t\t\t\t\t\tCHOOSE A NUMBER IN THE BOX ABOVE:");

    scanf("%d", &ch1);
    switch (ch1) {

        case 0:
            printf("\n\n\t\tAre you sure you want to exit? If so type 0 ");
            break;
        case 1:

            do {
                i = 0;
                j=0;

                printf("\n\n  Enter Username:  ");
                scanf("%s", username1);
                printf("\n\n  Enter Password:  ");
                scanf("%s", password1);

                char buf5[1024] = {};
                char query_string5[] = {
                        "SELECT * FROM USERS WHERE username = ('%s') AND password = ('%s')"};
                sprintf(buf5, query_string5, username1, password1);
                if (mysql_query(conn, buf5)) {
                    printf("Unable to connect into MySQL server\n");
                    mysql_close(conn);
                    return 1;
                }
                MYSQL_RES *rs4 = mysql_store_result(conn);

                if (rs4 == NULL) {
                    printf("Unable to compile SQL statement\n");
                    mysql_close(conn);
                    return 1;
                }

                while (record4 = mysql_fetch_row(rs4)) {
                    i++;
                    if (record4[4] != NULL)
                    {
                        j++;
                        strcpy(userid,record4[0]);
                        printf("%s\n", userid);
                    }

                }
                if (i == 0) {

                    printf("\n WRONG USERNAME AND PASSWORD!");
                    printf("\n  Try again?\n\n\t[1] Yes\t\t[0] No\n\t");

                    scanf("%d", &ch1);
                }if(i!=0 && j==0){
                    goto main ;
                }if(i!=0 && j!=0){
                    goto main2;
                }

            } while (ch1 == 1);

                break;
        default:
            printf("Invalid choice");
            break;
    }

    scanf("%d",&ch1);

    switch (ch1)

    {

        case 1:

            goto login;

        case 0:

            mysql_close(conn);
            goto logout;
            break;


        default:

            printf("Invalid choice");

            break;

    }







        main:

    system("clear");

    printf("\n\t\t\t\t\t\t\t\t################################################");
    printf("\n\t\t\t\t\t\t\t\t#    WELCOME TO Employee MANAGEMENT SYSTEM      #");
    printf("\n\t\t\t\t\t\t\t\t################################################ \n");

    printf("\n\t\t\t\t\t\t\t\tMAIN MENU\n\t\t\t\t\t\t\t\t ______________________________________________");
    printf("\n\t\t\t\t\t\t\t\t| [1] Add a new Employee                        |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________ _|");
    printf("\n\t\t\t\t\t\t\t\t| [2] List all Employees                        |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________|");
    printf("\n\t\t\t\t\t\t\t\t| [3] Search for Employees                      |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________|");
    printf("\n\t\t\t\t\t\t\t\t| [4] Edit an Employee                           |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________|");
    printf("\n\t\t\t\t\t\t\t\t| [5] Delete an Employee                         |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________|");
    printf("\n\t\t\t\t\t\t\t\t| [0] Exit                                     |\n\t\t\t\t\t\t\t\t________________________________________________\n\t\t\t\t\t\t\t\t");

    printf("\n\t\t\t\t\t\t\t\tCHOOSE A NUMBER IN THE BOX ABOVE:");

    scanf("%d", &ch);

    switch (ch) {

        case 0:

            printf("\n\n\t\tAre you sure you want to exit?");

            break;

        case 1:

            system("clear");



            for (;;) {
                fflush(stdin);

                printf("To exit enter blank space in the name input\nName :");

                scanf("%[^\n]", listing.fullname);
                char buf[1024] = {};
                char query_string[] = {"INSERT INTO USERS (name) VALUES ('%s')"};
                sprintf(buf, query_string, listing.fullname);
                if (mysql_query(conn, buf)) {
                    printf("Unable to insert into MySQL server\n");
                    mysql_close(conn);
                    return 1;
                }
                if (strcmp(listing.fullname, "") == 0 || strcmp(listing.fullname, " ") == 0)
                    break;
                fflush(stdin);

                /*

                printf("Phone:");

                scanf("%d",&listing.phonenumber);
                if (mysql_query(conn, "INSERT INTO USERS('phone') VALUES :listing.phonenumber")) {
                    printf("Unable to connect with MySQL server\n");
                    mysql_close(conn);
                    return 1;
                }

                fflush(stdin);

                printf("address:");

                scanf("%s\n",listing.address);

                fflush(stdin);

                printf("email address:");

                gets(listing.emailaddress);

                printf("\n");

                fwrite(&listing,sizeof(listing),1,fp);

            } */
           }
            break;

        case 2:

            system("clear");
            printf("\n################################################");
            printf("\n#    LIST OF CONTACTS                          #");
            printf("\n################################################ \n");
            printf("\n________________________________________________\n\n");


            if (mysql_query(conn, "SELECT * FROM USERS")) {
                printf("Unable to select table\n");
                mysql_close(conn);
                return 1;
            }

            MYSQL_RES *rs = mysql_store_result(conn);

            if (rs == NULL) {
                printf("Unable to compile SQL statement\n");
                mysql_close(conn);
                return 1;
            }

            while (record = mysql_fetch_row(rs)) {
             printf("%s %s %s\n", record[0], record[1],record[4]);
            }

            break;


        case 3:

            system("clear");


            do {
                found = 0;

                printf("\n\n\tCONTACT SEARCH\n\t################################################\n\tName of contact to search: ");

                fflush(stdin);

                scanf("%[^\n]", query);

                l = strlen(query);

                system("clear");

                printf("\n\nSearch result for '%s' \n################################################\n", query);



                char buf1[1024] = {};
                char query_string1[] = {"SELECT * FROM USERS WHERE name LIKE '%s'"};
                sprintf(buf1, query_string1, query);
                if (mysql_query(conn, buf1)) {
                    printf("Unable to fetch from MySQL server\n");
                    mysql_close(conn);
                    return 1;
                }


                MYSQL_RES *rs1 = mysql_store_result(conn);

                if (rs1 == NULL) {
                    printf("Unable to compile SQL statement\n");
                    mysql_close(conn);
                    return 1;
                }

                while (record1 = mysql_fetch_row(rs1)) {
                    found++;
                    printf("%s %s %s\n", record1[0], record1[1],record1[2]);
                }





                /*

                   while(fread(&listing,sizeof(listing),1,fp)==1)

                   {

                       for(i=0; i<=l; i++)

                           fullname[i]=listing.fullname[i];

                       fullname[l]='\0';

                       if(strcmp(fullname,query)==0)

                       {

                           printf("\nName\t: %s\nPhone\t: %d\nAddress\t: %s\nEmail\t: %s\n",listing.fullname,listing.phonenumber,listing.address,listing.emailaddress);

                           found++;*/

                if (found % 4 == 0) {

                    printf("Press any key to continue...");

                    getch();

                }


                if (found == 0)

                    printf("\n No match found!");

                else

                    printf("\n %d match(s) found!", found);


                printf("\n  Try again?\n\n\t[1] Yes\t\t[0] No\n\t");

                scanf("%d", &ch);

            } while (ch == 1);

            break;


        case 4:
            system("clear");

            do {

                found = 0;

                printf("Edit contact\n################################################\n\n\tEnter the name of contact to edit:");

                fflush(stdin);

                scanf("%[^\n]",fullname);


                system("clear");

                printf("\n\nSearch result for '%s' \n################################################\n", fullname);



                char buf2[1024] = {};
                char query_string2[] = {"SELECT * FROM USERS WHERE name LIKE '%s'"};
                sprintf(buf2, query_string2,fullname);
                if (mysql_query(conn, buf2)) {
                    printf("Unable to fetch from MySQL server\n");
                    mysql_close(conn);
                }


                MYSQL_RES *rs2 = mysql_store_result(conn);

                if (rs2 == NULL) {
                    printf("Unable to compile SQL statement\n");
                    mysql_close(conn);
                    return 1;
                }

                while (record2 = mysql_fetch_row(rs2)) {
                    found++;
                    printf("%s %s\n", record2[0], record2[1]);
                }



                if (found % 4 == 0) {

                    printf("Press any key to continue...");

                    getch();

                }

                if (found == 0)

                    printf("\n No match found!");

                else {

                    printf("\n %d match(s) found!", found);
                    printf("\n\nEditing '%s'\n\n", fullname);

                    printf("Type the new Name:");

                    scanf("%s", listing.fullname);

                    char buf3[1024] = {};
                    char query_string3[] = {"UPDATE USERS SET name ='%s' WHERE name ='%s'"};
                    sprintf(buf3, query_string3, listing.fullname, fullname);
                    if (mysql_query(conn, buf3)) {
                        printf("Unable to fetch from MySQL server\n");
                        mysql_close(conn);
                    }
                }
                printf("\n  Try again?\n\n\t[1] Yes\t\t[0] No\n\t");
                scanf("%d",&ch);

            }while(ch==1);
            break;

       case 5:
            system("clear");

            fflush(stdin);

            printf("\n\n\tDELETE A CONTACT\n\t################################################n\tEnter the name of contact to delete:");

            scanf("%s",fullname);
            char buf4[1024] = {};
            char query_string4[] = {"DELETE FROM USERS WHERE name = '%s'"};
            sprintf(buf4, query_string4,fullname);
            if (mysql_query(conn, buf4)) {
                printf("Unable to fetch from MySQL server\n");
                mysql_close(conn);
            }
            fflush(stdin);
            break;


        default:

            printf("Invalid choice");

            break;


    }
    printf("\n\n\nEnter the Choice:\n\n\t[1] Main Menu\t\t[0] Exit\n");

    scanf("%d",&ch);

    switch (ch)

    {

        case 1:

            goto main;

        case 0:

            mysql_close(conn);
            break;


        default:

            printf("Invalid choice");

            break;

    }


    main2 :

    system("clear");

    printf("\n\t\t\t\t\t\t\t\t################################################");
    printf("\n\t\t\t\t\t\t\t\t#    WELCOME TO Employee Account     #");
    printf("\n\t\t\t\t\t\t\t\t################################################ \n");

    printf("\n\t\t\t\t\t\t\t\tMAIN MENU\n\t\t\t\t\t\t\t\t ______________________________________________");
    printf("\n\t\t\t\t\t\t\t\t| [1] View Employee Information                 |");
    printf("\n\t\t\t\t\t\t\t\t| _____________________________________________ |");
    printf("\n\t\t\t\t\t\t\t\t| [0] Exit                                     |\n\t\t\t\t\t\t\t\t________________________________________________\n\t\t\t\t\t\t\t\t");

    printf("\n\t\t\t\t\t\t\t\tCHOOSE A NUMBER IN THE BOX ABOVE:");

    scanf("%d", &ch);

    switch (ch) {

        case 0:

            printf("\n\n\t\tAre you sure you want to exit?");

            break;

        case 1:

            system("clear");
            //atoi(userid);
            //printf("%s", userid);
            char buf6[1024] = {};
            char query_string6[] = {"SELECT * FROM USERS WHERE id = '%s'"};
            sprintf(buf6, query_string6,userid);
            if (mysql_query(conn, buf6)) {
                printf("Unable to fetch from MySQL server\n");
                mysql_close(conn);
            }

            MYSQL_RES *rs6 = mysql_store_result(conn);

            if (rs6 == NULL) {
                printf("Unable to compile SQL statement\n");
                mysql_close(conn);
                return 1;
            }

            while (record6 = mysql_fetch_row(rs6)) {
                printf("%s %s\n", record6[0], record6[1]);
            }

            break;

        default:
            printf("Invalid choice");
            break;
        }

    scanf("%d",&ch1);

    switch (ch1)

    {

        case 1:

            goto main2;

        case 0:

            mysql_close(conn);
            goto logout;
            break;


        default:

            printf("Invalid choice");

            break;

    }

















logout:

    return 0;

}




