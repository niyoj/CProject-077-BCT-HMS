/******************************************************************************************************
*   Program name: UI Module for HIMS,
*   Author name: NiyojOli(THA077BCT029), 
*   Created Path: ./modules/ui.c,
*   Created Date: 18 Oct 2021, 08:37:07 (DD MON YYYY, HH:MM:SS),
*   Description: This  program file is responsible for performing UI related operations like displaying logo etc.
*******************************************************************************************************/

struct Users_data {
    char fname[256];
    char lname[256];
    char username[256];
    char passwd[256];
    char role[256];
};

struct Users_data users[1000];

//function print_logo() is used to print the logo of the program which is customizable by editing file ./modules/config.c
void print_logo(void) {
    printf("\n");
    terminal_print(_config.app_name);       //using module terminal_print for printing purpose

    printf("\n");
    for(int i=0; i<(strlen(_config.app_name)*14); i++) printf("=");     //printing the horizontal bar
    printf("\n\n\n");
}

//function print_instruction() is used to give the instruction to the user. It takes a integer variable as an argument which signifies the level of insturction to be shown
void print_instruction(int level) {
    //level = 1 show the information about first level of instruction commands
    if(level == 1) {
        printf("\nHere are the set of code that you can use to do some basic operations.\n");

        //print the instructions on the basis of the user roles
        if(strcmp(_env.role, "admin") == 0) {
            printf("[1] => Create new user\n");     //done
            printf("[2] => See the analytics\n");   //done
            printf("[3] => Delete a user\n");       //done
            printf("[4] => Clear the screen\n");    //done
            printf("[5] => Change password\n");     //done
            printf("[6] => End the day for your job\n");     //done
            printf("[7] => See all the user details\n");    //done
            printf("[8] => Logout\n\n\n");          //done

        } else if (strcmp(_env.role, "reception") == 0) {
            printf("[1] => Add a new patient\n");   //done
            printf("[2] => See todays patient\n");  //done
            printf("[3] => See todays oncall doctors\n");   //done
            printf("[4] => Assign the patient to the doctor\n");    //done
            printf("[5] => Clear the screen\n");    //done
            printf("[6] => Change the password\n"); //done
            printf("[7] => End the day for your job\n");     //done
            printf("[8] => Logout\n\n\n");          //done

        }  else if (strcmp(_env.role, "doctor") == 0) {
            printf("[1]  => See my today patients\n");   //done
            printf("[2]  => See my patient record\n");   //done
            printf("[3]  => Diagnosis on patient\n");    //done
            printf("[4]  => Refer for new test\n");
            printf("[5]  => See daily chart of a patient\n");
            printf("[6]  => See patient labrotary result\n");
            printf("[7]  => Discharge a patient\n");
            printf("[8]  => End the day for your job\n");
            printf("[9]  => Change the password\n");
            printf("[10] => Logout\n\n\n");

        } else if(strcmp(_env.role, "nurse") == 0) {
            printf("[1] => See my patient today\n");
            printf("[2] => Update daily chart of patient\n");
            printf("[3] => See labrotary result status\n");
            printf("[4] => See medications to be used\n");
            printf("[5] => Clear the screen\n");
            printf("[6] => Change the password\n");
            printf("[7] => Logout\n");

        } else if(strcmp(_env.role, "laboratory") == 0) {
            printf("[1] => See new labs to be done\n");
            printf("[2] => Update lab result\n");
            printf("[3] => See lab result of the patient\n");
            printf("[4] => Clear the screen\n");
            printf("[5] => Change the password\n");
            printf("[6] => Logout\n");
            
        } else if(strcmp(_env.role, "pharmacy") == 0) {
            printf("[1] => See patient bill\n");
            printf("[2] => Add in the patient bill\n");
            printf("[3] => Clear the screen\n");
            printf("[4] => Change the password\n");
            printf("[5] => Logout\n");
            
        }
    } else if(level == 2) {
        //level = 2 shows the information for login to the user
        printf("Welcome to HIMS!\n");
        printf("Please enter your credentials for a successfull login:\n\n");
    }
}

//function start_login() is used to initiate the login process
int start_login() {
    add_log("Login of the user initiated");
    int repeat = 1;                             //stores number of times the login process is repeated
    while(repeat <= 3) {                        //sets the maximum amount of tries to 3
        char user_name[256] = {};               //stores the username
        char passwd[256] = {};                  //stores the password

        printf("Enter your username: ");        //asking for the username
        scanf(" %s", user_name);
        
        printf("Enter your password: ");        //asking for the password
        scanf(" %s", passwd);

        int is_true = authenticate(user_name, passwd);      //validating the credentials

        //if credentials are true create a session else repeat
        if(is_true) {
            char role[256] = {};
            get_role(user_name, role);
            
            char log[256] = "Credentials matched with ";
            strcat(log, user_name);
            strcat(log, " with the role as ");
            strcat(log, role);
            add_log(log);

            create_session(user_name, role);
            prepare_env();
            return 1;
        } else {
            printf("\nYour credentials did not matched.\n\n");
            repeat++;
        }
    }

    printf("Too many attempts.Please try again later.\n\n"); //shows the maxmium try error message
    return 0;
}

//function creat_user() is used to create a new user by the admin
int create_user() {
    char username[256] = {};
    char passwd[256] = {};      //password does not contains spaces
    char fname[256] = {}, lname[256] = {}, role[256] = {};

    //printing the instructions
    printf("Initializing the registration of the new user.."); 
    printf(".\nPlease note that the passsword of the user must only contain [A to Z],[a to z],[0 to 9]\n\n");

    printf("Enter the firstname: ");
    scanf(" %s", fname);                     //getting the first name
    printf("Enter the lastname: ");
    scanf(" %s", lname);                     //geting the last name
    printf("Enter the username: ");
    scanf(" %s", username);                  //getting the nick name of the user which most be unique
    printf("Enter the password: ");
    scanf(" %s", passwd);                    //getting the password of the user
    printf("Enter his/her role: ");
    scanf(" %s", role);                      //getting the admin of the user

    char cmd[256] = "GET ROW;users;WHERE;username;";
    strcat(cmd, username);
 

    while(_db(cmd).code != 0) {
        printf("\nThe username you entered already exists.\n");
        printf("Please enter new username: ");
        scanf(" %s", username);
        
        strcpy(cmd, "GET ROW;users;WHERE;username;");
        strcat(cmd, username);
    }

    while(strlen(passwd) < 6) {
        printf("\nYour password is to weak.\n");
        printf("Please enter a new password: ");
        scanf(" %s", passwd);
    }

    strcpy(cmd, "INSERT ROW;users;fname,lname,username,password,role;");
    strcat(cmd, fname);
    strcat(cmd, ",");
    strcat(cmd, lname);
    strcat(cmd, ",");
    strcat(cmd, username);
    strcat(cmd, ",");
    strcat(cmd, passwd);
    strcat(cmd, ",");
    strcat(cmd, role);
    
    db status = _db(cmd);

    if (status.code) {
        char log[256] = "New user ";
        strcat(log, username);
        strcat(log, " with the role ");
        strcat(log, role);
        strcat(log, " was created");
        add_log(log);
        return 1;
    } else {
        return 0;
    }
}

void show_in_tables(int n) {
    char row[256] = {};
    FILE *table_fp = fopen("./.db/tables/users", "r");
    fscanf(table_fp, "%s\n", row);
    strcpy(row, "");

    for(int i=0; i<n; i++) {
        fscanf(table_fp, "%s\n", row);

        int index = 0;
        int len = 0;
        for(int j=0; row[j] != 0; j++) {
            if(row[j] == ',') {
                index++; 
                len = 0;
            } else {
                if(index == 0) users[i].fname[len] = row[j]; 
                if(index == 1) users[i].lname[len] = row[j]; 
                if(index == 2) users[i].username[len] = row[j]; 
                if(index == 3) users[i].passwd[len] = row[j];
                if(index == 4) users[i].role[len] = row[j];
                len++;
            }
        }
    }

    int max_fname = 9;
    int max_lname = 9;
    int max_username = 9;
    int max_passwd = 8;
    int max_role = 4;

    for(int i=0; i<n; i++) {
        if(strlen(users[i].fname)>max_fname) max_fname = strlen(users[i].fname);
        if(strlen(users[i].lname)>max_lname) max_lname = strlen(users[i].lname);
        if(strlen(users[i].username)>max_username) max_username = strlen(users[i].username);
        if(strlen(users[i].passwd)>max_passwd) max_passwd = strlen(users[i].passwd);
        if(strlen(users[i].role)>max_role) max_role = strlen(users[i].role);
    }

    fclose(table_fp);
    
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_username; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   FIRSTNAME"); for(int i=0; i<(max_fname-9); i++) printf(" "); printf("   ");
    printf("|   LASTNAME"); for(int i=0; i<(max_lname-9); i++) printf(" "); printf("    ");
    printf("|   NICKNAME"); for(int i=0; i<(max_username-9); i++) printf(" "); printf("    ");
    printf("|   PASSWORD"); for(int i=0; i<(max_passwd-8); i++) printf(" "); printf("   ");
    printf("|   ROLE"); for(int i=0; i<(max_role-4); i++) printf(" "); printf("   |");
    printf("\n");
        
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_username; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+");
    
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", users[i].fname); for(int j=0; j<(max_fname+5-strlen(users[i].fname)); j++) printf(" "); printf("|");
        printf(" %s", users[i].lname); for(int j=0; j<(max_lname+5-strlen(users[i].lname)); j++) printf(" "); printf("|");
        printf(" %s", users[i].username); for(int j=0; j<(max_username+5-strlen(users[i].username)); j++) printf(" "); printf("|");
        printf(" %s", users[i].passwd); for(int j=0; j<(max_passwd+5-strlen(users[i].passwd)); j++) printf(" "); printf("|");
        printf(" %s", users[i].role); for(int j=0; j<(max_role+5-strlen(users[i].role)); j++) printf(" "); printf("|");
        printf("\n");   
    }  
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_username; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+"); printf("\n");
}