/******************************************************************************************************
*   Program name: Hospital Management System (HMS@BCT),
*   Author name: KhemrajShrestha (THA077BCT020), NiyojOli(THA077BCT029), OmPrakashSharma (THA077BCT030), PunamShrestha (THA077BCT038), 
*   Created Path: /main.c,
*   Created Date: 01 Aug 2021, 08:30:07 (DD MON YYYY, HH:MM:SS),
*   Description: This  program file is responsible to include all header files, and other files that contain user-defined functions.
*******************************************************************************************************/

//include section starts here
#include <stdio.h>      //file stdio.h contains necessary function prototypes for printf() and scanf()
#include <string.h>     //file string.h contains necessary function prototypes for various string related functions
#include <time.h>       //file time.h contains necessary funciton prototypes for various time related functions
#include <stdlib.h>     //file stdlib.h contains necessary function prototypes for some standard functions
#include <unistd.h>     //file unistd.h contains function prototype for access()

//if the system is windows then include file Windows.h else include file unistd.h
#ifdef _WIN32
#include <Windows.h>
#define  SYS "WINDOWS"
#else
#define SYS "LINUX"
#endif

//definition section starts here
#define LOGS "./.db/logs"                       //defines the path of the log file

#include "./modules/log.c"                      //contains necessary functions related to logging
#include "./modules/database/db.c"              //contains necessary functions related to database
#include "./modules/terminal_print/terminal.c"  //contains necessary functions related to printing using character
#include "./modules/rbacs.c"                    //contains necessary functions related to RBACS mdodule
#include "./modules/security.c"                 //contains necessary functions related to hashing and encryption in the program
#include "./modules/sessions.c"                 //contains necessary functions related to session in the program
#include "./modules/reception.c"                //contains necessary functions related to session in the program
#include "./modules/config.c"                   //contains necessary configurations for the app
#include "./modules/authentication.c"           //contains necessary functions related to authentication
#include "./modules/ui.c"                       //contains necessary functions related to UI
#include "./modules/attendance.c"               //contains necessary functions related to attendance
#include "./modules/analytics.c"                //contains necessary functions related to analytics of the app

int main(void) {
    system("@cls||clear");      //clearing the terminal screen
    prepare_env();              //preparing all the necessary environemnt variable
    start_logging();            //starts logging
    print_logo();               //printing the logo

    //if the user is not logged in show instructions to login
    if(!_env.is_logged_in) {
        print_instruction(2);                //printing the instructions for login
        int status = start_login();          //start login function
        if(!status) return 0;                //if login failed exit the program
    }

    attend_today();                         //does the attendance for today
    starting_phase:                         //label starting_phase denotes the point from where the actual program begins

    system("@cls||clear");      //clearing the terminal screen
    print_logo();               //printing the logo
    printf("Hello %s, glad to see you back here.\n\n", _env.logged_user);   //welcoming to the logged in user
    
    instruction_phase:          //label instruction _phase denotes the point from where the instruction is started to show
    
    print_instruction(1);       //printing the instruction to use the app

    int i_code;                 //variable i_code is used to store the code entered by the user
    printf("Enter any instruction code to continue: ");
    scanf(" %d", &i_code);
    printf("\n");

    if(strcmp(_env.role, "admin") == 0) {
        if (i_code == 1) {
            int status = create_user();

            if(status) {
                printf("\nThe user was successfully created.\n");
            } else {
                printf("\n Some unexpected error occured. Please try again later.\n");
            }
        } else if (i_code == 2) {
            printf("\nTotal users: %d", num_users());
            printf("\nTotal patients: %d\n\n", num_patients());
        } else if (i_code == 3) {
            char user[256] = {};
            char confirm = 'n';

            printf("\nEnter the user's username to delete: ");
            scanf(" %s", user);

            char cmd[256] = "DELETE ROW;users;WHERE;username;";
            strcat(cmd, user);
            
            printf("\nAre you sure you want to delete %s? (Y/n): ", user);
            scanf(" %c", &confirm);

            if(confirm == 'Y') {
                _db(cmd);
                printf("The user was deleted successfully\n");

                char log[256] = "Deleted user ";
                strcat(log, user);
                add_log(log);
            } else {
                printf("Aborting the process of deletion...\n");
            }
        } else if (i_code == 4) {
            goto starting_phase;
        } else if (i_code == 5) {
            char passwd[256] = {};
            
            printf("\nEnter a new password: ");
            scanf(" %s", passwd);

            while(strlen(passwd)<6) {
                printf("\nYour password is too weak.\nEnter a new strong password: ");
                scanf(" %s", passwd);
            }

            char cmd[256] = "UPDATE ROW;users;WHERE;username;";
            strcat(cmd, _env.logged_user);
            strcat(cmd, ";AS;password;");
            strcat(cmd, passwd);
            _db(cmd);

            printf("\nThe password was changed successfully\n");
            add_log("Password was changed successfully");
        } else if (i_code == 6) {
            int num = 1;
            printf("The table contains %d number of entries. How many do you prefer to see? ", table_exists("users")-1);
            scanf(" %d", &num);

            while(num==0 || num>table_exists("users")-1) {
                printf("\nPlease re-enter the number of entries you want to see: ");
                scanf(" %d", &num);
            }
            show_in_tables("users", num);
        } else if (i_code == 7) {
            add_log("Logout command from the user");
            return 0;
        }

        printf("\nPress enter to continue...");
        char c = getchar();
        c = getchar();
        goto instruction_phase;
    } else if(strcmp(_env.role, "reception") == 0) {
        if(i_code == 1) {
            printf("\nThe new patient was successfully created with token %s.\n", create_patient().token); 
        } else if(i_code == 2) {    
            int num = 0;
            int rows = _db("GET ROW;patients;WHERE;status;admitted").rows;
            
            if(rows<1) {
                printf("There are no entries of the patients in the database.\n");
            } else {
                printf("There are %d entries of patients. How many do you want to see? ", rows-1);
                scanf(" %d", &num);

                while(num<0 || num>(rows-1)) {
                    printf("Please re-enter the number of entries you want to see: ");
                    scanf(" %d", &num);
                }

                show_in_table_patient(num);
            }
        } else if(i_code == 3) {
            printf("Printing the today's on call doctors...\n");
            show_attended("doctor");
        } else if(i_code == 4) {
            char cmd[256] = "GET ROW;patients;WHERE;token;";
            char patient_token[256] = "";
            char doctor[256] = "";

            printf("Enter the token of the patient: ");;
            scanf(" %s", patient_token);
            strcat(cmd, patient_token);
            
            if(_db(cmd).code == 0) {
                printf("! No such patient found.\n");
            } else {
                printf("Enter the name of the doctor: ");
                scanf(" %s", doctor);

                strcpy(cmd, "UPDATE ROW;patients;WHERE;token;");
                strcat(cmd, patient_token);
                strcat(cmd, ";AS;doctor;");
                strcat(cmd, doctor);

                _db(cmd);

                printf("\nThe patient with token %s has been succesfully assigned to %s.\n", patient_token, doctor);
                
                char log[256] = "Patient with token ";
                strcat(log, patient_token);
                strcat(log, " assigned to doctor ");
                strcat(log, doctor);
                add_log(log);
            }
        }
        printf("\nPress enter to continue...");
        char c = getchar();
        c = getchar();
        goto instruction_phase;
    }
    return 0;       //Keeping the compiler happy
}