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
    char nickname[256];
    char passwd[256];
    char role[256];
};

struct Users_data users[10000];

//function print_logo() is used to print the logo of the program which is customizable by editing file ./modules/config.c
void print_logo(void) {
    printf("\n");
    terminal_print(_config.app_name);

    printf("\n");
    for(int i=0; i<(strlen(_config.app_name)*14); i++) printf("=");
    printf("\n\n\n");
}

//function print_instruction() is used to give the instruction to the user. It takes a integer variable as an argument which signifies the level of insturction to be shown
void print_instruction(int level) {
    //level = 1 show the information about first level of instruction commands
    if(level == 1) {
        printf("\nHere are the set of code that you can use to do some basic operations.\n");

        //print the instructions on the basis of the user roles
        if(strcmp(_env.role, "admin") == 0) {
            printf("[1] => Create new user\n");
            printf("[2] => See the analytics\n");
            printf("[3] => Delete a user\n");
            printf("[4] => Clear the screen\n");
            printf("[5] => Change password\n");
            printf("[6] => See all the user details\n");
            printf("[7] => Logout\n\n\n");

        } else if (strcmp(_env.role, "reception") == 0) {
            printf("[1] => Add a new patient\n");
            printf("[2] => See todays patient\n");
            printf("[3] => See todays oncall doctors\n");
            printf("[4] => Clear the screen\n");
            printf("[5] => Change the password\n");
            printf("[6] => Logout\n");

        }  else if (strcmp(_env.role, "doctor") == 0) {
            printf("[1] => See my today patients\n");
            printf("[2] => See my patient record\n");
            printf("[3] => Diagnosis on patient\n");
            printf("[4] => Create a new report\n");
            printf("[5] => Discharge a patient\n\n\n");
        } else {
            printf("[1] => ");
            printf("[2] => ");
            printf("[3] => ");
            printf("[4] => ");
            printf("[5] => ");

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
    scanf("%s", fname);                     //getting the first name
    printf("Enter the lastname: ");
    scanf("%s", lname);                     //geting the last name
    printf("Enter the username: ");
    scanf("%s", username);                  //getting the nick name of the user which most be unique
    printf("Enter the password: ");
    scanf("%s", passwd);                    //getting the password of the user
    printf("Enter his/her role: ");
    scanf("%s", role);                      //getting the admin of the user

    char cmd[256] = "GET ROW;users;WHERE;nickname;";
    strcat(cmd, username);
 

    while(_db(cmd).code != 0) {
        printf("\nThe username you entered already exists.\n");
        printf("Please enter new username: ");
        scanf(" %s", username);
        
        strcpy(cmd, "GET ROW;users;WHERE;nickname;");
        strcat(cmd, username);
    }

    while(strlen(passwd) < 6) {
        printf("\nYour password is to weak.\n");
        printf("Please enter a new password: ");
        scanf(" %s", passwd);
    }

    strcpy(cmd, "INSERT ROW;users;fname,lname,nickname,password,role;");
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

void show_in_tables(char table[], int n) {
    char row[256] = {};
    FILE *table_fp = fopen("./.db/tables/users", "r");
    fscanf(table_fp, "%s\n", row);
    strcpy(row, "");

    for(int i=0; i<n; i++) {
        char retrn = fscanf(table_fp, "%s\n", row);

        int index = 0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
            } else {
                if(index == 0) users[i].fname[strlen(users[i].fname)] = row[j]; 
                if(index == 1) users[i].lname[strlen(users[i].lname)] = row[j]; 
                if(index == 2) users[i].nickname[strlen(users[i].nickname)] = row[j]; 
                if(index == 3) users[i].passwd[strlen(users[i].passwd)] = row[j];
                if(index == 4) users[i].role[strlen(users[i].role)] = row[j];
            }
        }
    }

    int max_fname = 9;
    int max_lname = 9;
    int max_nickname = 9;
    int max_passwd = 8;
    int max_role = 4;

    for(int i=0; i<n; i++) {
        if(strlen(users[i].fname)>max_fname) max_fname = strlen(users[i].fname);
        if(strlen(users[i].lname)>max_lname) max_lname = strlen(users[i].lname);
        if(strlen(users[i].nickname)>max_nickname) max_nickname = strlen(users[i].nickname);
        if(strlen(users[i].passwd)>max_passwd) max_passwd = strlen(users[i].passwd);
        if(strlen(users[i].role)>max_role) max_role = strlen(users[i].role);
    }

    fclose(table_fp);
    
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_nickname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   FIRSTNAME"); for(int i=0; i<(max_fname-9); i++) printf(" "); printf("   ");
    printf("|   LASTNAME"); for(int i=0; i<(max_lname-9); i++) printf(" "); printf("    ");
    printf("|   NICKNAME"); for(int i=0; i<(max_nickname-9); i++) printf(" "); printf("    ");
    printf("|   PASSWORD"); for(int i=0; i<(max_passwd-8); i++) printf(" "); printf("   ");
    printf("|   ROLE"); for(int i=0; i<(max_role-4); i++) printf(" "); printf("   |");
    printf("\n");
        
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_nickname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+");
    
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", users[i].fname); for(int j=0; j<(max_fname+5-strlen(users[i].fname)); j++) printf(" "); printf("|");
        printf(" %s", users[i].lname); for(int j=0; j<(max_lname+5-strlen(users[i].lname)); j++) printf(" "); printf("|");
        printf(" %s", users[i].nickname); for(int j=0; j<(max_nickname+5-strlen(users[i].nickname)); j++) printf(" "); printf("|");
        printf(" %s", users[i].passwd); for(int j=0; j<(max_passwd+5-strlen(users[i].passwd)); j++) printf(" "); printf("|");
        printf(" %s", users[i].role); for(int j=0; j<(max_role+5-strlen(users[i].role)); j++) printf(" "); printf("|");
        printf("\n");   
    }  
    printf("+---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_nickname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_passwd; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_role; i++) printf("-"); printf("---+"); printf("\n");
}