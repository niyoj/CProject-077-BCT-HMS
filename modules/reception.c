//struct patient is used to store patient information
struct patient {
    char fname[256];
    char mname[256];
    char lname[256];
    char gender[2];
    int age;
    char department[256];
    char token[10];
};


//function create_patient() is used to register a new patient
struct patient create_patient() {
    struct patient new_patient;     //stores the information of new patient

    //printing the instrctions for the registration
    printf("\n\nYou are going to register a new patient to be admitted to hospital.\n");
    printf("Note that you can leave the middle name empty if there is none.\n");

    //getting the required information
    printf("\nEnter patient's first name: ");
    scanf(" %s", new_patient.fname);
    printf("Enter patient's middle name: ");
    scanf(" %s", new_patient.mname);
    printf("Enter patient's last name: ");
    scanf(" %s", new_patient.lname);

    //since making everything string makes the task easier. Here we created a string gender of size 2.
    //The first part stores the male or female value and second part stores a null character
    //null character denotes the termination of the string here
    printf("Enter patient's gender (m/f): ");
    scanf(" %c", &new_patient.gender[0]);
    new_patient.gender[1] = '\0';
    
    //getting age as a integer and it converts into string in further steps
    printf("Enter patient's age: ");
    scanf(" %d", &new_patient.age);

    printf("Assign the patient to the department: ");
    scanf(" %s", new_patient.department);

    //generating a token
    char token[100] = {};       //stores the token temporarily
    strcat(token, new_patient.fname);
    strcat(token, new_patient.mname);
    strcat(token, new_patient.lname);
    hash(new_patient.token, token);         //generating token from hashing algorithm
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

    //converting age to string
    int len = 1;
    if(new_patient.age/100 > 1) len=3; else if(new_patient.age/10 > 1) len = 2;
    char age[4] = {};

    for(int i=0; i<len; i++) {
        age[len-1-i] = new_patient.age%10+48;
        new_patient.age = new_patient.age/10;
    }
    age[len] = '\0';
    strcat(cmd, age);
    strcat(cmd, ",");
    
    strcat(cmd, new_patient.department);
    strcat(cmd, ",");
    strcat(cmd, ",admitted");

    _db(cmd);       //adding entry to the database

    //logging the event
    char log[256] = "Added new patient with token ";
    strcat(log, new_patient.token);
    add_log(log);
    
    return new_patient;
}