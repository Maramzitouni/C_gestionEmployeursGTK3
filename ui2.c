
/*  Title : "Projet C: outil de plainification pour le service des ressources humaines"
    Author :Lesly Kamene ,Maram Zitouni, Audrey
    Class:  2i2 
    Date:   10/01/2022 
*/

/*pour compiler, exécutez ses commandes: 
gcc -Wall -c proj.c -o proj.o `pkg-config gtk+-3.0 --cflags --libs`

gcc proj.o -o proj.app `pkg-config gtk+-3.0 --libs` 
*/

/*
    gcc -c -o main.o  $(mysql_config --cflags) -lcurses  projectc.c $(mysql_config --libs) -L/usr/local/opt/openssl/lib `pkg-config gtk+-3.0 --cflags --libs`
    gcc -o main  $(mysql_config --cflags) -lcurses  main.o $(mysql_config --libs) -L/usr/local/opt/openssl/lib `pkg-config gtk+-3.0 --libs`
 
 
 
 gcc  -o main.c  $(mysql_config --cflags) -lcurses  main $(mysql_config --libs) */

#include<stdio.h>

#include<curses.h>

#include<string.h>

#include<unistd.h>

#include<stdlib.h>
#include <mysql/mysql.h>

#include <gtk/gtk.h>


struct contact

{

    int phonenumber;

    char fullname[20],address[20],emailaddress[30];

} listing;


typedef struct _db_con
{
    char *server;
    char *username;
    char *password;
    char *database;
    MYSQL *conn;

}db_con;


typedef struct login
{
   char* username1;
   char*password1 ;
   MYSQL_ROW record4;
   int ch1;
}login;


typedef struct services{
 GtkButton* add;
 GtkButton* show_all;
 GtkButton* search;
 GtkButton* edit;
 GtkButton* delete;
 GtkButton* info;
 GtkButton* confirm_holiday;
 GtkButton* askHoliday;
 int* userid;
}services;

typedef struct s_interface_v{
    GtkWindow* fenetre;
    GtkWindow* Hfenetre;
    GtkBox* boite_principale;
    GtkButton* logoff;
    GtkBox* box[9];
    GtkVBox* vbox;
    GtkButton* valider;
    GtkButton* edit2;
    GtkButton* main_menu;
    GtkButton* main_menu2;
    GtkBox *options;            /* box qui contient les boutons valider et quit */
    GtkButton *quit_b;
    GtkWidget *pEntryUsername;
    GtkWidget *pLabelUsername;
    GtkWidget *pEntryPass;
    GtkWidget *pLabelPass;
    GtkWidget *pLabelDays;
    
    GtkWidget *pEntry_new_Username;
    GtkWidget *pLabel_new_Username;
    GtkWidget *pEntry_days;


    GtkWidget *pLabelTemps;
    GtkWidget *pLabel[200];
    GtkWidget *pLabel1;
    GtkButton* b_hist;
    GtkBox* boite_principale2;
    db_con db_cn;
    int c;
    services serv;
}interface_v;


int initialisation_bd(db_con *db_cn);
void login_page_ui(interface_v* inter);
void verify_id(GtkWidget *btn, interface_v* inter);
void service_display(interface_v* inter);
void info_display(interface_v* inter);
void logout_function(GtkWidget *btn, interface_v* inter);
void add_employee(GtkWidget *btn,interface_v* inter);
void add_new_employee(GtkWidget *btn,interface_v* inter);
void show_all_function(GtkWidget *btn, interface_v* inter);
void back_to_menu(GtkWidget *btn, interface_v* inter);
void search_user(GtkWidget *btn, interface_v* inter);
void search_verif(GtkWidget *btn, interface_v* inter);
void edit_function(GtkWidget *btn, interface_v* inter);
void edit_verif(GtkWidget *btn, interface_v* inter);
void finish_edit(GtkWidget *btn, interface_v* inter);
void delete_function(GtkWidget *btn, interface_v* inter);
void delete_verif(GtkWidget *btn, interface_v* inter);
void information_function(GtkWidget *btn, interface_v* inter);
void holiday(GtkWidget *btn, interface_v* inter);
void confirmHoliday(GtkWidget *btn, interface_v* inter);
void holiday_function(GtkWidget *btn, interface_v* inter);
void add_holiday(GtkWidget *btn, interface_v* inter);
void back_to_menu2(GtkWidget *btn, interface_v* inter);

char userid[20];
char username[20];


int main() {
    /*  déclaration des variables & initialisations */
    interface_v inter;
    /* initialisation de l'environnement gtk */
    gtk_init(NULL, NULL);

    /*construction de la fenêtre*/
    login_page_ui(&inter);
    
    /*lancement de l'interface*/

    gtk_main(); // Runs the main loop until gtk_main_quit() is called.
   

}

int initialisation_bd(db_con *db_cn){
   db_cn->server= "localhost";
   db_cn->username = "root";
   db_cn->password = "";
   db_cn->database = "projetC";
   db_cn->conn = mysql_init(NULL);
   if (mysql_real_connect(db_cn->conn, db_cn->server, db_cn->username, db_cn->password, db_cn->database, 0, NULL, 0) == NULL) {
        printf("Unable to connect with MySQL server\n");
        mysql_close(db_cn->conn);
        return 0;
    }

    return 1;

}


void login_page_ui(interface_v* inter){

   /* initialisation de la base des donnees*/
   int i = initialisation_bd(&inter->db_cn); 


    /* fenêtre & son initialisation */
    inter->fenetre =(GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(inter->fenetre, "administration user");
    gtk_window_set_default_size(inter->fenetre,400, 200);
    
    /* conteneur principal & son insertion dans la fenêtre */
    inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

    /* boutons représentant l'interface et leur insertion dans le conteneur principal */
    inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);
    
    
    inter->pLabelUsername = gtk_label_new(NULL);

    char d[20]= "Username :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelUsername), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    
    
    
    inter->pLabelUsername = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    /* Creation du GtkEntry */
    inter->pEntryUsername = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntryUsername, TRUE, TRUE, 0);
   
    inter->box[1] =(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[1]), TRUE, FALSE, 0);
    
    inter->pLabelPass = gtk_label_new(NULL);

    char s[20]= "Password :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelPass), s);
    gtk_box_pack_start(GTK_BOX(inter->box[1]), inter->pLabelPass, TRUE, FALSE, 0);
    
    inter->pEntryPass = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[1]), inter->pEntryPass, TRUE, TRUE, 0);
   

    inter->options =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,100);
    gtk_box_pack_start(inter->boite_principale, GTK_WIDGET((*inter).options), FALSE, TRUE, 20);
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->options, GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    inter->quit_b =(GtkButton*)gtk_button_new_with_label("Quiter");
    gtk_box_pack_start((GtkBox*)inter->options, GTK_WIDGET((*inter).quit_b), TRUE, TRUE, 1);
    
    
 
    /* visibilité des widgets au lancement de l"interface */
    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    
    /* sortie de l'interface */
    g_signal_connect(G_OBJECT((*inter).fenetre), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(inter->quit_b), "clicked", G_CALLBACK(gtk_main_quit), NULL);
    
    //
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(verify_id), inter);
}




void verify_id(GtkWidget *btn, interface_v* inter){
 const gchar *username;
 const gchar *password;

  MYSQL_ROW record4;
  int i=0;
  int j=0;
 /* Recuperation du texte contenu dans le GtkEntry */
 username = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));
 password = gtk_entry_get_text(GTK_ENTRY((inter->pEntryPass)));
 int value;
 char buf5[1024] = {};
 char query_string5[] = { "SELECT id,username, password ,type FROM USERS WHERE username = ('%s') AND password = ('%s')"};
 sprintf(buf5, query_string5, username,password);
 if (mysql_query(inter->db_cn.conn, buf5)) {
    printf("Unable to connect into MySQL server\n");
    mysql_close(inter->db_cn.conn);
   }

 MYSQL_RES *rs4 = mysql_store_result(inter->db_cn.conn);

 if (rs4 == NULL) {
     printf("Unable to compile SQL statement\n");
     mysql_close(inter->db_cn.conn);
    
    }

 while (record4=mysql_fetch_row(rs4)) {
    i++;
     if (record4[3] > 0)
     {
       j++;
       
       strcpy(userid,record4[0]);
     }
    

                   
 }
 if (i!=0 && j==0)
 {
     gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
     service_display(inter);
 }

 else 
    if(i!=0 && j!=0){
       gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
       info_display(inter);
    }
}

void service_display(interface_v* inter){

    inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

    //box qui contient tous les boutons
    inter->box[2] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[2]), TRUE, FALSE, 0);

    inter->serv.add =(GtkButton*)gtk_button_new_with_label("Add a new Employee");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.add), TRUE, TRUE, 1);
    
    inter->serv.show_all =(GtkButton*)gtk_button_new_with_label("List all Employees");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.show_all), TRUE, TRUE, 1);

    
    inter->serv.search =(GtkButton*)gtk_button_new_with_label("Search for Employees");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.search), TRUE, TRUE, 1);

    
    inter->serv.edit =(GtkButton*)gtk_button_new_with_label("Edit an Employee");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.edit), TRUE, TRUE, 1);
    
    
    inter->serv.delete =(GtkButton*)gtk_button_new_with_label("Delete an Employee");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.delete), TRUE, TRUE, 1);


    inter->serv.confirm_holiday =(GtkButton*)gtk_button_new_with_label("Confirm holiday");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.confirm_holiday), TRUE, TRUE, 1);


    inter->logoff =(GtkButton*)gtk_button_new_with_label("Log out");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).logoff), TRUE, TRUE, 1);



    
   gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
   g_signal_connect(G_OBJECT(inter->logoff), "clicked", G_CALLBACK(logout_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.add), "clicked", G_CALLBACK(add_employee), inter);
   g_signal_connect(G_OBJECT(inter->serv.show_all), "clicked", G_CALLBACK(show_all_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.search), "clicked", G_CALLBACK(search_user), inter);
   g_signal_connect(G_OBJECT(inter->serv.edit), "clicked", G_CALLBACK(edit_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.delete), "clicked", G_CALLBACK(delete_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.confirm_holiday), "clicked", G_CALLBACK(holiday), inter);
}


void logout_function(GtkWidget *btn, interface_v* inter){
 mysql_close(inter->db_cn.conn);
 gtk_widget_destroy(GTK_WIDGET(inter->fenetre));
}


void add_employee(GtkWidget *btn,interface_v* inter){
   gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));

   inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

   inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
   gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);
   
    inter->pLabelUsername = gtk_label_new(NULL);

    char d[30]= "Enter new Employee :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelUsername), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    
   
    /* Creation du GtkEntry */
    inter->pEntryUsername = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntryUsername, TRUE, TRUE, 0);


    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    


    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);


    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);
    

    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
   
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(add_new_employee), inter);
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);
}

// il faut toujours un 2eme parametre bouton pourque ca marche si on utilise g_signal_connect
void add_new_employee(GtkWidget *btn, interface_v* inter){
 const gchar *name;
  
 /* Recuperation du texte contenu dans le GtkEntry */
  name = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));

                fflush(stdin);

                char buf[1024] = {};
                char query_string[] = {"INSERT INTO USERS (name) VALUES ('%s')"};
                sprintf(buf, query_string, name);
                if (mysql_query(inter->db_cn.conn, buf)) {
                    printf("Unable to insert into MySQL server\n");
                    mysql_close(inter->db_cn.conn);
                }
                if (strcmp(name, "") == 0 || strcmp(name, " ") == 0)
                    printf("try");
                fflush(stdin);         

  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  service_display(inter);              

}


void show_all_function(GtkWidget *btn, interface_v* inter){
  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  MYSQL_ROW record;
  char t[600];
  inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));



  inter->box[2] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
  gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[2]), TRUE, FALSE, 0);
  inter->pLabelTemps = gtk_label_new(NULL);
 
  char t1[600];
  if (mysql_query(inter->db_cn.conn, "SELECT id,name,email FROM USERS")) {
                printf("Unable to select table\n");
                mysql_close(inter->db_cn.conn);
                //break; dialog box show
            }

            MYSQL_RES *rs = mysql_store_result(inter->db_cn.conn);

            if (rs == NULL) {
                printf("Unable to compile SQL statement\n");
                mysql_close(inter->db_cn.conn);
                //break;
            }

            while (record = mysql_fetch_row(rs)) {
                //printf("%s %s\n", record[0], record[1]);
                strcat(t1,record[0]);
                strcat(t1," ");
                strcat(t1,record[1]);
                strcat(t1,"\n");
                strcat(t1,record[2]);
                strcat(t1,"\n");

                //sprintf(t,"%s %s\n", record[0], record[1]);
               
            }
    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[2]), inter->pLabelTemps, TRUE, FALSE, 0);

    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);
    

    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));

    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);

}

void back_to_menu(GtkWidget *btn, interface_v* inter){
  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  service_display(inter); 

}

void search_user(GtkWidget *btn, interface_v* inter){
    gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));

   inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

   inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
   gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);

   inter->box[8]=NULL;
   
    inter->pLabelUsername = gtk_label_new(NULL);

    char d[30]= "Name of Employee to search :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelUsername), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    
   
    /* Creation du GtkEntry */
    inter->pEntryUsername = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntryUsername, TRUE, TRUE, 0);
    


    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);


    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(search_verif), inter);
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);

}

void search_verif(GtkWidget *btn, interface_v* inter){
  if(inter->box[8]!=NULL)
  gtk_widget_destroy(GTK_WIDGET(inter->box[8]));
 const gchar *username =NULL;
 int found =0;
 MYSQL_ROW record1;
 char t1[600]="";
 char t[600]="";

 //initialisaton interface
 inter->box[8] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
 gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[8]), TRUE, FALSE, 0);
 inter->pLabelTemps = gtk_label_new(NULL);

 /* Recuperation du texte contenu dans le GtkEntry */
 username = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));
 char buf1[1024] = {};
 char query_string1[] = {"SELECT * FROM USERS WHERE name LIKE '%s'"};
 sprintf(buf1, query_string1, username);
 if (mysql_query(inter->db_cn.conn, buf1)) {
     printf("Unable to fetch from MySQL server\n");
     mysql_close(inter->db_cn.conn);
  }


 MYSQL_RES *rs1 = mysql_store_result(inter->db_cn.conn);

 if (rs1 == NULL) {
   printf("Unable to compile SQL statement\n");
   mysql_close(inter->db_cn.conn);
  }
  else{
     while (record1 = mysql_fetch_row(rs1)) {
        found++;
        printf("%s %s\n", record1[0], record1[1]);
        strcat(t1,record1[0]);
        strcat(t1," ");
        strcat(t1,record1[1]);
        strcat(t1,"\n");
      }

      if (found == 0){
        strcat(t1,"\n No match found!");
        strcat(t1,"\n  Try again?");
      }
 
       /*else
        printf("\n %d match(s) found!", found);*/

  }

 
 
    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[8]), inter->pLabelTemps, TRUE, FALSE, 0);
    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
}


void edit_function(GtkWidget *btn, interface_v* inter){
    gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));

   inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

   inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
   gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);

   inter->box[8]=NULL;
   
    inter->pLabelUsername = gtk_label_new(NULL);

    char d[40]= "Enter the name of Employee to edit :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelUsername), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    
   
    /* Creation du GtkEntry */
    inter->pEntryUsername = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntryUsername, TRUE, TRUE, 0);
    


    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[0], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);


    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(edit_verif), inter);
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);

}


void edit_verif(GtkWidget *btn, interface_v* inter){
 if(inter->box[8]!=NULL){
    gtk_widget_destroy(GTK_WIDGET(inter->box[8]));
 }

 if(inter->box[3]!=NULL)
   gtk_widget_destroy(GTK_WIDGET(inter->box[3]));



 inter->box[8] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
 gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[8]), TRUE, FALSE, 0);
 inter->pLabelTemps = gtk_label_new(NULL);

 


 const gchar *username =NULL;
 const gchar *new_username =NULL;
 int found =0;
 MYSQL_ROW record2;
 char t1[600]="";
 char t[600]="";

 /* Recuperation du texte contenu dans le GtkEntry */
 username = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));

 char buf2[1024] = {};
 char query_string2[] = {"SELECT * FROM USERS WHERE name LIKE '%s'"};
 sprintf(buf2, query_string2,username);
 if (mysql_query(inter->db_cn.conn, buf2)) {
    printf("Unable to fetch from MySQL server\n");
    mysql_close(inter->db_cn.conn);
 }


 MYSQL_RES *rs2 = mysql_store_result(inter->db_cn.conn);

 if (rs2 == NULL) {
    printf("Unable to compile SQL statement\n");
    mysql_close(inter->db_cn.conn);
    }

 while (record2 = mysql_fetch_row(rs2)) {
   found++;
   strcat(t1,"\n  Editing the name : ");
   printf("%s %s\n", record2[0], record2[1]);
  // strcat(t1,record2[0]);
   //strcat(t1," ");
   strcat(t1,record2[1]);
   strcat(t1,"\n");
 }


 if (found == 0){
    strcat(t1,"\n No match found!");
    strcat(t1,"\n  Try again");
    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[8]), inter->pLabelTemps, TRUE, FALSE, 0);

  }

  else {

    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[8]), inter->pLabelTemps, TRUE, FALSE, 0);
   

  if(inter->box[7]!=NULL){
    gtk_widget_destroy(GTK_WIDGET(inter->box[7]));
  }
  inter->box[7] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
  gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[7]), TRUE, FALSE, 0);



  inter->pLabel_new_Username = gtk_label_new(NULL);

  char d[30]= "Type the new Name :";
    
  gtk_label_set_text(GTK_LABEL(inter->pLabel_new_Username), d);
  gtk_box_pack_start(GTK_BOX(inter->box[7]), inter->pLabel_new_Username, TRUE, FALSE, 0);
    
   
  /* Creation du GtkEntry */
  inter->pEntry_new_Username = gtk_entry_new();
  /* Insertion du GtkEntry dans la GtkBox */
  gtk_box_pack_start(GTK_BOX(inter->box[7]), inter->pEntry_new_Username, TRUE, TRUE, 0);

  inter->edit2 =(GtkButton*)gtk_button_new_with_label("Edit");
  gtk_box_pack_start((GtkBox*)inter->box[7], GTK_WIDGET((*inter).edit2), TRUE, TRUE, 1);
    
  g_signal_connect(G_OBJECT(inter->edit2), "clicked", G_CALLBACK(finish_edit), inter);
   /* new_username = gtk_entry_get_text(GTK_ENTRY((inter->pEntry_new_Username)));

    char buf3[1024] = {};
    char query_string3[] = {"UPDATE USERS SET name ='%s' WHERE name ='%s'"};
    sprintf(buf3, query_string3, new_username, username);
    if (mysql_query(inter->db_cn.conn, buf3)) {
       printf("Unable to fetch from MySQL server\n");
       mysql_close(inter->db_cn.conn);
     }*/

 }

    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);

    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);

}




void finish_edit(GtkWidget *btn, interface_v* inter){
    const gchar *new_username =NULL;
    const gchar *username =NULL;
    new_username = gtk_entry_get_text(GTK_ENTRY((inter->pEntry_new_Username)));
    username = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));

    char buf3[1024] = {};
    char query_string3[] = {"UPDATE USERS SET name ='%s' WHERE name ='%s'"};
    sprintf(buf3, query_string3, new_username, username);
    if (mysql_query(inter->db_cn.conn, buf3)) {
       printf("Unable to fetch from MySQL server\n");
       mysql_close(inter->db_cn.conn);
     }

  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  service_display(inter); 

}


void delete_function(GtkWidget *btn, interface_v* inter){
    gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));

   inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

   inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
   gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);

   inter->box[8]=NULL;
   
    inter->pLabelUsername = gtk_label_new(NULL);

    char d[40]= "Enter the Name of Employee to delete :";
    
    gtk_label_set_text(GTK_LABEL(inter->pLabelUsername), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelUsername, TRUE, FALSE, 0);
    
   
    /* Creation du GtkEntry */
    inter->pEntryUsername = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntryUsername, TRUE, TRUE, 0);
    


    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);


    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(delete_verif), inter);
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);

}

void delete_verif(GtkWidget *btn, interface_v* inter){
    const gchar *username =NULL;
    username = gtk_entry_get_text(GTK_ENTRY((inter->pEntryUsername)));
    
     char buf4[1024] = {};
            char query_string4[] = {"DELETE FROM USERS WHERE name = '%s'"};
            sprintf(buf4, query_string4,username);
            if (mysql_query(inter->db_cn.conn, buf4)) {
                printf("Unable to fetch from MySQL server\n");
                mysql_close(inter->db_cn.conn);
            }

  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  service_display(inter);

}

void holiday(GtkWidget *btn, interface_v* inter){
    gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  MYSQL_ROW record7;
  char t[600]="";
  inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));
  inter->box[2] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
  gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[2]), TRUE, FALSE, 0);
  inter->pLabelTemps = gtk_label_new(NULL);
  char t1[600]="";
  if (mysql_query(inter->db_cn.conn, "SELECT name,holiday FROM USERS WHERE Confirmholiday = 1")) {
                printf("Unable to select table\n");
                mysql_close(inter->db_cn.conn);
            }

            MYSQL_RES *rs7 = mysql_store_result(inter->db_cn.conn);

            if (rs7 == NULL) {
                printf("Unable to compile SQL statement\n");
                mysql_close(inter->db_cn.conn);
            }
            

            while (record7 = mysql_fetch_row(rs7)) {
                strcpy(username,record7[0]);
                strcat(t1,"Name of Employee: ");
                strcat(t1,record7[0]);
                strcat(t1," ");
                strcat(t1,"\n");
                strcat(t1,"days of holiday requested: ");
                strcat(t1,record7[1]); 
            }
    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[2]), inter->pLabelTemps, TRUE, FALSE, 0);

   inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    
    inter->main_menu =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu), TRUE, TRUE, 1);


    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(confirmHoliday), inter);
    g_signal_connect(G_OBJECT(inter->main_menu), "clicked", G_CALLBACK(back_to_menu), inter);
}



void confirmHoliday(GtkWidget *btn, interface_v* inter){
    
    
     char buf5[1024] = {};
            char query_string5[] = {"UPDATE USERS SET Confirmholiday = 0  WHERE name = '%s'"};
            sprintf(buf5, query_string5,username);
            if (mysql_query(inter->db_cn.conn, buf5)) {
                printf("Unable to fetch from MySQL server\n");
                mysql_close(inter->db_cn.conn);
            }

  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  service_display(inter);


}

void info_display(interface_v* inter){
    inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

    //box qui contient tous les boutons
    inter->box[2] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[2]), TRUE, FALSE, 0);

    inter->serv.info =(GtkButton*)gtk_button_new_with_label("show information");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.info), TRUE, TRUE, 1);

    inter->serv.askHoliday =(GtkButton*)gtk_button_new_with_label("Ask For Holiday");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).serv.askHoliday), TRUE, TRUE, 1);

    inter->logoff =(GtkButton*)gtk_button_new_with_label("Log out");
    gtk_box_pack_start((GtkBox*)inter->box[2], GTK_WIDGET((*inter).logoff), TRUE, TRUE, 1);

    
   gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
   g_signal_connect(G_OBJECT(inter->logoff), "clicked", G_CALLBACK(logout_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.info), "clicked", G_CALLBACK(information_function), inter);
   g_signal_connect(G_OBJECT(inter->serv.askHoliday), "clicked", G_CALLBACK(holiday_function), inter);
}  


void holiday_function(GtkWidget *btn, interface_v* inter){
    

gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));

   inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
   gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

   inter->box[0] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
   gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET((*inter).box[0]), TRUE, FALSE, 0);
   
    inter->pLabelDays = gtk_label_new(NULL);

    char d[30]= "Enter number of holiday days :";

    
    gtk_label_set_text(GTK_LABEL(inter->pLabelDays), d);
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pLabelDays, TRUE, FALSE, 0);
    
    /* Creation du GtkEntry */
    inter->pEntry_days = gtk_entry_new();
    /* Insertion du GtkEntry dans la GtkBox */
    gtk_box_pack_start(GTK_BOX(inter->box[0]), inter->pEntry_days, TRUE, TRUE, 0);
    

    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_HORIZONTAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    
    inter->valider =(GtkButton*)gtk_button_new_with_label("Valider");
    gtk_box_pack_start((GtkBox*)inter->box[0], GTK_WIDGET((*inter).valider), TRUE, TRUE, 1);
    
    inter->main_menu2 =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu2), TRUE, TRUE, 1);
    

    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));   
    g_signal_connect(G_OBJECT(inter->valider), "clicked", G_CALLBACK(add_holiday), inter);
    g_signal_connect(G_OBJECT(inter->main_menu2), "clicked", G_CALLBACK(back_to_menu2), inter);
}

void add_holiday(GtkWidget *btn, interface_v* inter){
const gchar *days;
    int day;
  
 /* Recuperation du texte contenu dans le GtkEntry */
  days = gtk_entry_get_text(GTK_ENTRY((inter->pEntry_days)));
  day=atoi(days);
                    fflush(stdin);
            char buf8[1024] = {};
            char query_string8[] = {"UPDATE USERS SET holiday = '%d', Confirmholiday = '1'  WHERE id = '%s'"};
            sprintf(buf8,query_string8,day,userid);
            if (mysql_query(inter->db_cn.conn, buf8)) {
                printf("Unable to fetch from MySQL server\n");
                mysql_close(inter->db_cn.conn);
            }

  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  info_display(inter);
  
}

void information_function(GtkWidget *btn, interface_v* inter){
  
  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  inter->boite_principale =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(inter->fenetre), GTK_WIDGET(inter->boite_principale));

  inter->box[2] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
  gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[2]), TRUE, FALSE, 0);
  inter->pLabelTemps = gtk_label_new(NULL);
 
  MYSQL_ROW record6;
  char i[20]="";
  char t1[30]="";
  char t[30]="";
  char buf6[1024] = {};
  char query_string6[] = {"SELECT id,name,email FROM USERS WHERE id = '%s'"};
  sprintf(buf6, query_string6,userid);
  if (mysql_query(inter->db_cn.conn, buf6)) {
     printf("Unable to fetch from MySQL server\n");
     mysql_close(inter->db_cn.conn);
  }

  MYSQL_RES *rs6 = mysql_store_result(inter->db_cn.conn);

  if (rs6 == NULL) {
    printf("Unable to compile SQL statement\n");
    mysql_close(inter->db_cn.conn);
    //return 1;
  }

   while (record6 = mysql_fetch_row(rs6)) {
     
     strcat(t1,record6[0]);
     strcat(t1," ");
     strcat(t1,record6[1]);
     strcat(t1,"\n");
     strcat(t1,record6[2]);
     strcat(t1,"\n");
  }

    sprintf(t,"%s\n", t1);
    gtk_label_set_text(GTK_LABEL(inter->pLabelTemps), t);
    gtk_box_pack_start(GTK_BOX(inter->box[2]), inter->pLabelTemps, TRUE, FALSE, 0);

    inter->box[3] =(GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,4);
    gtk_box_pack_start((GtkBox*)inter->boite_principale, GTK_WIDGET(inter->box[3]), TRUE, FALSE, 0);
    //inter->logoff =(GtkButton*)gtk_button_new_with_label("Log out");
    //gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).logoff), TRUE, TRUE, 1);
    

    //gtk_widget_show_all(GTK_WIDGET(inter->fenetre));

    //g_signal_connect(G_OBJECT(inter->logoff), "clicked", G_CALLBACK(logout_function), inter);
    inter->main_menu2 =(GtkButton*)gtk_button_new_with_label("Main menu");
    gtk_box_pack_start((GtkBox*)inter->box[3], GTK_WIDGET((*inter).main_menu2), TRUE, TRUE, 1);
    gtk_widget_show_all(GTK_WIDGET(inter->fenetre));
    g_signal_connect(G_OBJECT(inter->main_menu2), "clicked", G_CALLBACK(back_to_menu2), inter);


}
void back_to_menu2(GtkWidget *btn, interface_v* inter){
  gtk_widget_destroy(GTK_WIDGET(inter->boite_principale));
  info_display(inter); 

}

