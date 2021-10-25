struct patient {
    char fname[256];
    char mname[256];
    char lname[256];
    char gender[2];
    int age;
    char department[256];
};

int create_patient() {
    struct patient new_patient;
    printf("\n\nYou are going to register a new patient to be admitted to hospital.\n");
    printf("Note that you can leave the middle name empty if there is none.\n");

    printf("\nEnter patient's first name: ");
    scanf(" %s", new_patient.fname);
    printf("\nEnter patient's middle name: ");
    scanf(" %s", new_patient.mname);
    printf("\nEnter patient's last name: ");
    scanf(" %s", new_patient.lname);
    printf("\nEnter patient's gender (m/f): ");
    scanf(" %c", &new_patient.gender[0]);
    new_patient.gender[1] = '\0';
    printf("\nEnter patient's age: ");
    scanf(" %d", &new_patient.age);
    printf("\nAssign the patient to the department: ");
    scanf(" %s", new_patient.department);

    char cmd[256] = "INSERT ROW;patients;fname,mname,lname,gender,age,department,doctor,status;";
    strcat(cmd, new_patient.fname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.mname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.lname);
    strcat(cmd, ",");
    strcat(cmd, new_patient.gender);
    strcat(cmd, ",");
    char age[2];
    age[0] = new_patient.age;
    age[1] = '\0';
    strcat(cmd, age);
    strcat(cmd, ",");
    strcat(cmd, new_patient.department);
    strcat(cmd, ",");
    strcat(cmd, ",admitted");

    if(_db(cmd).code) {
        return 1;
    } else {
        return 0;
    }
}