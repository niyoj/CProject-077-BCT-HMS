struct patient_data {
  char token[256];
  char fname[256];
  char mname[256];
  char lname[256];
  char gender[2];
  char age[4];
  char department[256];
  char doctor[256];
  char status[256];
};
struct patient_data patient[1000];

//function create_patient() is used to register a new patient
struct patient_data register_patient() {
    struct patient_data new_patient;     //stores the information of new patient

    //printing the instrctions for the registration
    printf("\n\nYou are going to register a new patient to be admitted to hospital.\n");
    printf("Note that if there is no middle name enter NA.\n");

    //getting the required information
    printf("\nEnter patient's first name: ");
    scanf(" %s", new_patient.fname);
    printf("Enter patient's middle name: ");
    scanf(" %s", new_patient.mname);

    //replaces the patient middle name with empty character if the middle name is NA (Not Applicable)
    if(strcmp(new_patient.mname, "na") == 0 || strcmp(new_patient.mname, "NA") == 0) strcpy(new_patient.mname, "");

    printf("Enter patient's last name: ");
    scanf(" %s", new_patient.lname);

    //since making everything string makes the task easier. Here we created a string gender of size 2.
    //The first part stores the male or female value and second part stores a null character
    //null character denotes the termination of the string here
    printf("Enter patient's gender (m/f): ");
    scanf(" %c", &new_patient.gender[0]);
    new_patient.gender[1] = '\0';
    
    printf("Enter patient's age: ");
    scanf(" %s", new_patient.age);

    printf("Assign the patient to the department: ");
    scanf(" %s", new_patient.department);

    //generating a token
    char token[100] = {};       //stores the token temporarily
    create_token(token);
    strcpy(new_patient.token, token);       //copying the generated token to structure

    //preparing the command to add the entry in database
    char cmd[256] = "INSERT ROW;patients;token,fname,mname,lname,gender,age,department,doctor,status;";
    strcat(cmd, new_patient.token);
    strcat(cmd, ",");
    strcat(cmd, new_patient.fname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.mname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.lname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.gender);
    strcat(cmd, ",");
    strcat(cmd, new_patient.age);
    strcat(cmd, ",");    
    strcat(cmd, new_patient.department);
    strcat(cmd, ",");
    strcat(cmd, ",admitted");

    _db(cmd);       //adding entry to the database

    //logging the event
    char log[256] = "Added new patient with token ";
    strcat(log, new_patient.token);
    add_log(log);

    //creating necessary tables in database
    create_patient(token, "diagnosis");
    create_patient(token, "lab");
    create_patient(token, "health");
    create_patient(token, "medication");
    create_patient(token, "pharma");
    
    return new_patient;
}

void show_in_table_patient(int n) {
    char row[256] = {};
    FILE* table_fp;
    
    if(strcmp(_env.role, "doctor") == 0) {
        char t_cmd[256] = "GET ROW;patients;WHERE;doctor;";
        strcat(t_cmd, _env.logged_user);
        strcat(t_cmd, ";AND;status;admitted");
        char values[256] = {};
        strcpy(values, _db(t_cmd).values);
        char header[256] = {};
        
        strcpy(header, _db(t_cmd).header);

        char t_e_val[256][256] = {};
        explode(values, ';', t_e_val);

        table_fp = fopen("./.db/tables/temp", "w");
        fprintf(table_fp, "%s\n", header);
        for(int i=0; strlen(t_e_val[i]) != 0; i++) {
            fprintf(table_fp, "%s\n", t_e_val[i]);
            n++;
        }
        fclose(table_fp);

        table_fp = fopen("./.db/tables/temp", "r");     
    } else {
        table_fp = fopen("./.db/tables/patients", "r");
    }
    fscanf(table_fp, "%s\n", row);
    strcpy(row, "");

    for(int i=0; i<n; i++) {
        char retrn = fscanf(table_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {
                if(index == 0) patient[i].token[len] = row[j]; 
                if(index == 1) patient[i].fname[len] = row[j]; 
                if(index == 2) patient[i].mname[len] = row[j]; 
                if(index == 3) patient[i].lname[len] = row[j]; 
                if(index == 4) patient[i].gender[len] = row[j];
                if(index == 5) patient[i].age[len] = row[j];
                if(index == 6) patient[i].department[len] = row[j];
                if(index == 7) patient[i].doctor[len] = row[j];
                if(index == 8) patient[i].status[len] = row[j];
                len++;
            }
        }
    }

    int max_token = 5;
    int max_fname = 9;
    int max_mname = 10;
    int max_lname = 9;
    int max_gender = 6;
    int max_age = 3;
    int max_department = 11;
    int max_doctor = 6;
    int max_status = 6;

    for(int i=0; i<n; i++) {
        if(strlen(patient[i].token)>max_token) max_token = strlen(patient[i].token);
        if(strlen(patient[i].fname)>max_fname) max_fname = strlen(patient[i].fname);
        if(strlen(patient[i].mname)>max_mname) max_mname = strlen(patient[i].mname);
        if(strlen(patient[i].lname)>max_lname) max_lname = strlen(patient[i].lname);
        if(strlen(patient[i].gender)>max_gender) max_gender = strlen(patient[i].gender);
        if(strlen(patient[i].age)>max_age) max_age = strlen(patient[i].age);
        if(strlen(patient[i].department)>max_department) max_department = strlen(patient[i].department);
        if(strlen(patient[i].doctor)>max_doctor) max_doctor = strlen(patient[i].doctor);
        if(strlen(patient[i].status)>max_status) max_status = strlen(patient[i].status);        
    }

    fclose(table_fp);
    
    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_mname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_gender; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_age; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_department; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_doctor; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   TOKEN"); for(int i=0; i<(max_token-5); i++) printf(" "); printf("   ");
    printf("|   FIRSTNAME"); for(int i=0; i<(max_fname-9); i++) printf(" "); printf("   ");
    printf("|   MIDDLENAME"); for(int i=0; i<(max_mname-10); i++) printf(" "); printf("   ");
    printf("|   LASTNAME"); for(int i=0; i<(max_lname-9); i++) printf(" "); printf("    ");
    printf("|   GENDER"); for(int i=0; i<(max_gender-6); i++) printf(" "); printf("   ");
    printf("|   AGE"); for(int i=0; i<(max_age-3); i++) printf(" "); printf("   ");
    printf("|   DEPARTMENT"); for(int i=0; i<(max_department-11); i++) printf(" "); printf("    ");
    printf("|   DOCTOR"); for(int i=0; i<(max_doctor-6); i++) printf(" "); printf("   ");
    printf("|   STATUS"); for(int i=0; i<(max_status-6); i++) printf(" "); printf("   |");
    printf("\n");
        
    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_mname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_gender; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_age; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_department; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_doctor; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", patient[i].token); for(int j=0; j<(max_token+5-strlen(patient[i].token)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].fname); for(int j=0; j<(max_fname+5-strlen(patient[i].fname)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].mname); for(int j=0; j<(max_mname+5-strlen(patient[i].mname)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].lname); for(int j=0; j<(max_lname+5-strlen(patient[i].lname)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].gender); for(int j=0; j<(max_gender+5-strlen(patient[i].gender)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].age); for(int j=0; j<(max_age+5-strlen(patient[i].age)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].department); for(int j=0; j<(max_department+5-strlen(patient[i].department)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].doctor); for(int j=0; j<(max_doctor+5-strlen(patient[i].doctor)); j++) printf(" "); printf("|");
        printf(" %s", patient[i].status); for(int j=0; j<(max_status+5-strlen(patient[i].status)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_fname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_mname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_lname; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_gender; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_age; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_department; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_doctor; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");
    printf("\n");
}