//structure diagnosis_data is used to store the information regarding the diagnosis of the patient
struct diagnosis_data {
  char date[15];
  char symptom[256];
  char diagnosis[256];
};

//structure lab_data is used to store the information regarding the laboratory reports and tests
struct lab_data {
  char token[256];
  char date[15];
  char name[256];
  char value[256];
  char remarks[256];
  char status[256];
};

//structure health_data is used to store the information regarding the health chart of the patient
struct health_data {
  char date[15];
  char time[256];
  char pressure[256];
  char heartrate[256];
  char spo2[256];
  char temp[256];
};

//structure medication_data is used to store the information regarding the medicine being used by the patient
struct medication_data {
  char time[256];
  char dosage[256];
  char medicine[256];
  char date_started[15];
  char date_end[15];
};

//structure pharma_data is used to store the information regarding the billing of patient regarding the medicine used
struct pharma_data {
  char date[15];
  char medicine[256];
  char qty[256];
  char rate[256];
  char total[256];
};

//defining array of structures necessary to print the data in tabular format
struct diagnosis_data diagnosis[1000];
struct lab_data lab_report[1000];
struct health_data health[1000];
struct medication_data medication[1000];
struct pharma_data pharmacy[1000];

//function create_patient() can be used to create tables related to the patient data
void create_patient(char token[], char chart[]) {
  //preparing the path for the tables
  char path[256] = "";
  strcat(path, "patient/");
  strcat(path, token);
  strcat(path, "_");
  strcat(path, chart);

  //preparing the command for database module
  char cmd[256] = "CREATE TABLE;";
  strcat(cmd, path);

  //dynamically creating the path on the basis of the table
  if(strcmp(chart, "diagnosis") == 0) {
    strcat(cmd, ";symptoms,diagnosis,date");
  } else if (strcmp(chart, "lab") == 0) {
    strcat(cmd, ";token,date,name,value,remarks,status"); //remarks by lab, status ordered, done, recieved, seen
  } else if (strcmp(chart, "health") == 0) {
    strcat(cmd, ";date,time,pressure,heartrate,spo2,temp"); //time morning,afternoon,evening,night
  } else if (strcmp(chart, "medication") == 0) {
    strcat(cmd, ";time,dosage,medicine,date_started,date_end");   //time od,bd,td,sos,  dosage quantity
  } else if (strcmp(chart, "pharma") == 0) {
    strcat(cmd, ";date,medicine,qty,rate,total");
  }
  
  //if table doesnot exists create the table
  if(!table_exists(path)) _db(cmd);
}

void new_diagnosis(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_diagnosis");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  char sym[256] = "", diagno[256] = "", date[15] = "";
  printf("You are about to perform a diagnosis on the patient %s %s\nNote that the symptoms must be separated by comma and no extra spaces should be used.\n\n", fname, lname);
  printf("Enter the patient symptoms: ");
  scanf(" %s", sym);

  for(int i=0; i<strlen(sym); i++) {
    if(sym[i] == ',') sym[i] = '_';
  }

  printf("What is your diagnosis? ");
  scanf(" %s", diagno);

  get_date(date);
  
  strcpy(cmd, "INSERT ROW;");
  strcat(cmd, src);
  strcat(cmd, ";symptoms,diagnosis,date;");
  strcat(cmd, sym);
  strcat(cmd, ",");
  strcat(cmd, diagno);
  strcat(cmd, ",");
  strcat(cmd, date);
  
  _db(cmd);
  
  char log[256] = "New diagnosis was added for patient with token ";
  strcat(log, token);
  add_log(log);
}

void see_diagnosis(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_diagnosis");

  char row[256] = "";

  char path[256] = DB;
  strcat(path, src);

  FILE *diag_fp = fopen(path, "r");
  fscanf(diag_fp, "%s\n", row);
  int n = table_exists(src)-1;

  for(int i=0; i<n; i++) {
        char retrn = fscanf(diag_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {
                if(index == 0) {
                  if(row[j] == '_') {
                    diagnosis[i].symptom[len] = ','; 
                  } else {
                    diagnosis[i].symptom[len] = row[j]; 
                  }
                }
                if(index == 1) diagnosis[i].diagnosis[len] = row[j]; 
                if(index == 2) diagnosis[i].date[len] = row[j];
                len++;
            }
        }
  }

    int max_symptom = 7;
    int max_diagnosis = 9;
    int max_date = 4;

    for(int i=0; i<n; i++) {
        if(strlen(diagnosis[i].symptom)>max_symptom) max_symptom = strlen(diagnosis[i].symptom);
        if(strlen(diagnosis[i].diagnosis)>max_diagnosis) max_diagnosis = strlen(diagnosis[i].diagnosis);
        if(strlen(diagnosis[i].date)>max_date) max_date = strlen(diagnosis[i].date);   
    }

    fclose(diag_fp);
    
    printf("+---"); for(int i=0; i<max_symptom; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_diagnosis; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   SYMPTOM"); for(int i=0; i<(max_symptom-7); i++) printf(" "); printf("   ");
    printf("|   DIAGNOSIS"); for(int i=0; i<(max_diagnosis-9); i++) printf(" "); printf("   ");
    printf("|   DATE"); for(int i=0; i<(max_date-4); i++) printf(" "); printf("   |");
    printf("\n");
    

    printf("+---"); for(int i=0; i<max_symptom; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_diagnosis; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", diagnosis[i].symptom); for(int j=0; j<(max_symptom+5-strlen(diagnosis[i].symptom)); j++) printf(" "); printf("|");
        printf(" %s", diagnosis[i].diagnosis); for(int j=0; j<(max_diagnosis+5-strlen(diagnosis[i].diagnosis)); j++) printf(" "); printf("|");
        printf(" %s", diagnosis[i].date); for(int j=0; j<(max_date+5-strlen(diagnosis[i].date)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_symptom; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_diagnosis; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("\n");
}

void create_lab(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_lab");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  char name[256] = "", date[15] = "";
  printf("You are about to refer for a new lab report to the patient %s %s\nNote that you cannot use spaces in the laboratory test name. Prefer to use of underscore.\n\n", fname, lname);

  printf("Enter the laboratory test name: ");
  scanf(" %s", name);

  get_date(date);
  
  strcpy(cmd, "INSERT ROW;");
  strcat(cmd, src);
  strcat(cmd, ";token,date,name,status;");
  char t_token[256] = "";
  create_token(t_token);
  strcat(cmd, t_token);
  strcat(cmd, ",");
  strcat(cmd, date);
  strcat(cmd, ",");
  strcat(cmd, name);
  strcat(cmd, ",");
  strcat(cmd, "ordered");
  
  _db(cmd);   
  char log[256] = "New lab report was reffered for patient with token ";
  strcat(log, token);
  add_log(log);
}

void update_lab(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_lab");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  printf("You are about to update the lab report of the patient %s %s\n\nThe lab that are to be updated are;\n", fname, lname);

  strcpy(cmd, "GET ROW;");
  strcat(cmd, src);
  strcat(cmd, ";WHERE;status;ordered");
  
  strcpy(val, _db(cmd).values);
  
  while(1) {
    int len=0; int index = 0;
    char name[256] = {}, date[256] = {}, lab_token[256] = {};
    for(int i=0; i<strlen(val); i++) {
      if(val[i] == ';' || i == strlen(val)-1) {
        printf("    => Test for %s was ordered on %s with token %s.\n", name, date, lab_token);
        strcpy(name, "");
        strcpy(date, "");
        strcpy(lab_token, "");
        len = 0;
        index = 0;
        continue;
      }

      if(val[i] == ',') {
        if(index == 0) lab_token[len] = '\0';
        if(index == 1) date[len] = '\0';
        if(index == 2) name[len] = '\0';
        index++;
        len = 0;
        continue;
      }

      if(index == 1) date[len] = val[i];
      if(index == 2) name[len] = val[i];
      if(index == 0) lab_token[len] = val[i];

      len++;
    }
    break;
  }

  char lab_token[256] = {}, result[256] = {}, remarks[256] = "";
  printf("\nEnter the laboratory token you want to update: ");
  scanf(" %s", lab_token);

  int repeat = 0;
  do {
    char t_cmd[256] = "GET ROW;";
    strcat(t_cmd, src);
    strcat(t_cmd, ";WHERE;token;");
    strcat(t_cmd, lab_token);

    if(_db(t_cmd).code == 0) {
      repeat = 1;

      printf("\nRe-Enter the laboratory token you want to update: ");
      scanf(" %s", lab_token);
    }
  } while (repeat == 1);
  
  printf("\nEnter the result of the test: ");
  scanf(" %s", result);
  
  printf("\nNote that remarks must not contain spaces.\nEnter your remarks: ");
  scanf(" %s", remarks);
  
  strcpy(cmd, "UPDATE ROW;");
  strcat(cmd, src);
  strcat(cmd, ";WHERE;token;");
  strcat(cmd, lab_token);
  strcat(cmd, ";AS;status;updated");
  _db(cmd);

  strcpy(cmd, "UPDATE ROW;");
  strcat(cmd, src);
  strcat(cmd, ";WHERE;token;");
  strcat(cmd, lab_token);
  strcat(cmd, ";AS;remarks;");
  strcat(cmd, remarks);
  _db(cmd);

  strcpy(cmd, "UPDATE ROW;");
  strcat(cmd, src);
  strcat(cmd, ";WHERE;token;");
  strcat(cmd, lab_token);
  strcat(cmd, ";AS;value;");
  strcat(cmd, result);
  _db(cmd);

  char log[256] = "Lab report with token ";
  strcat(log, lab_token); 
  strcat(log, " was update for patient with token ");
  strcat(log, token);
  add_log(log);
}

void see_lab(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_lab");


  char path[256] = DB;
  strcat(path, src);

  char row[256] = "";
  FILE *lab_fp = fopen(path, "r");
  fscanf(lab_fp, "%s\n", row);
  int n = table_exists(src)-1;

  for(int i=0; i<n; i++) {
        char retrn = fscanf(lab_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {
                if(index == 0) lab_report[i].token[len] = row[j]; 
                if(index == 1) lab_report[i].date[len] = row[j]; 
                if(index == 2) lab_report[i].name[len] = row[j];
                if(index == 3) lab_report[i].value[len] = row[j];
                if(index == 4) lab_report[i].remarks[len] = row[j];
                if(index == 5) lab_report[i].status[len] = row[j];
                len++;
            }
        }
  }

    int max_token = 5;
    int max_date = 4;
    int max_name = 4;
    int max_value = 5;
    int max_remarks = 7;
    int max_status = 6;

    for(int i=0; i<n; i++) {
        if(strlen(lab_report[i].token)>max_token) max_token = strlen(lab_report[i].token);
        if(strlen(lab_report[i].date)>max_date) max_date = strlen(lab_report[i].date);
        if(strlen(lab_report[i].name)>max_name) max_name = strlen(lab_report[i].name);   
        if(strlen(lab_report[i].value)>max_value) max_value = strlen(lab_report[i].value);   
        if(strlen(lab_report[i].remarks)>max_remarks) max_remarks = strlen(lab_report[i].remarks);   
        if(strlen(lab_report[i].status)>max_status) max_status = strlen(lab_report[i].status);   
    }

    fclose(lab_fp);
    
    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_name; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_value; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_remarks; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   TOKEN"); for(int i=0; i<(max_token-5); i++) printf(" "); printf("   ");
    printf("|   DATE"); for(int i=0; i<(max_date-4); i++) printf(" "); printf("   ");
    printf("|   NAME"); for(int i=0; i<(max_name-4); i++) printf(" "); printf("   ");
    printf("|   VALUE"); for(int i=0; i<(max_value-5); i++) printf(" "); printf("   ");
    printf("|   REMARKS"); for(int i=0; i<(max_remarks-7); i++) printf(" "); printf("   ");
    printf("|   STATUS"); for(int i=0; i<(max_status-6); i++) printf(" "); printf("   |");
    printf("\n");
    

    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_name; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_value; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_remarks; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", lab_report[i].token); for(int j=0; j<(max_token+5-strlen(lab_report[i].token)); j++) printf(" "); printf("|");
        printf(" %s", lab_report[i].date); for(int j=0; j<(max_date+5-strlen(lab_report[i].date)); j++) printf(" "); printf("|");
        printf(" %s", lab_report[i].name); for(int j=0; j<(max_name+5-strlen(lab_report[i].name)); j++) printf(" "); printf("|");
        printf(" %s", lab_report[i].value); for(int j=0; j<(max_value+5-strlen(lab_report[i].value)); j++) printf(" "); printf("|");
        printf(" %s", lab_report[i].remarks); for(int j=0; j<(max_remarks+5-strlen(lab_report[i].remarks)); j++) printf(" "); printf("|");
        printf(" %s", lab_report[i].status); for(int j=0; j<(max_status+5-strlen(lab_report[i].status)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_token; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_name; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_value; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_remarks; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_status; i++) printf("-"); printf("---+");
    printf("\n");  

    if(strcmp(_env.role, "doctor") == 0) {
      char cmd[256] = "UPDATE ROW;";
      strcat(cmd, src);
      strcat(cmd, ";WHERE;status;updated;AS;status;seen");
      _db(cmd);
    }
}

void update_health(char token[]) {
  //date,time,pressure,heartrate,spo2,temp
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_health");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  char time[256] = "", pressure[256] = "", hbrate[256] = "", spo2[256] = "", temp[256] = "", date[15] = "";
  printf("You are about to update the health status of the patient %s %s\n\n", fname, lname);
  printf("Enter the time of monitoring (morning, afternoon, evening, night): ");
  scanf(" %s", time);

  printf("What is his/her pressure? ");
  scanf(" %s", pressure);

  printf("What is his/her heartbeat rate? ");
  scanf(" %s", hbrate);

  printf("What is his/her SPO2? ");
  scanf(" %s", spo2);

  printf("What is his/her temperature? ");
  scanf(" %s", temp);

  get_date(date);
  
  strcpy(cmd, "INSERT ROW;");
  strcat(cmd, src);
  strcat(cmd, ";date,time,pressure,heartrate,spo2,temp;");
  strcat(cmd, date);
  strcat(cmd, ",");
  strcat(cmd, time);
  strcat(cmd, ",");
  strcat(cmd, pressure);
  strcat(cmd, ",");
  strcat(cmd, hbrate);
  strcat(cmd, ",");
  strcat(cmd, spo2);
  strcat(cmd, ",");
  strcat(cmd, temp);
  
  _db(cmd); 
  
  char log[256] = "Health chart was updated for patient with token ";
  strcat(log, token);
  add_log(log); 
}

void see_health(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_health");

  char path[256] = DB;
  strcat(path, src);

  char row[256] = "";
  FILE *health_fp = fopen(path, "r");
  fscanf(health_fp, "%s\n", row);
  int n = table_exists(src)-1;

  for(int i=0; i<n; i++) {
        char retrn = fscanf(health_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {
                if(index == 0) health[i].date[len] = row[j];
                if(index == 1) health[i].time[len] = row[j]; 
                if(index == 2) health[i].pressure[len] = row[j];
                if(index == 3) health[i].heartrate[len] = row[j];
                if(index == 4) health[i].spo2[len] = row[j];
                if(index == 5) health[i].temp[len] = row[j];
                len++;
            }
        }
  }

    int max_date = 4;//date,time,pressure,heartrate,spo2,temp
    int max_time = 4;
    int max_pressure = 8;
    int max_heartrate = 9;
    int max_spo2 = 4;
    int max_temp = 5;

    for(int i=0; i<n; i++) {
        if(strlen(health[i].date)>max_date) max_date = strlen(health[i].date);
        if(strlen(health[i].time)>max_time) max_time = strlen(health[i].time);
        if(strlen(health[i].pressure)>max_pressure) max_pressure = strlen(health[i].pressure);
        if(strlen(health[i].heartrate)>max_heartrate) max_heartrate = strlen(health[i].heartrate);
        if(strlen(health[i].spo2)>max_spo2) max_spo2 = strlen(health[i].spo2);
        if(strlen(health[i].temp)>max_temp) max_temp = strlen(health[i].temp);   
    }

    fclose(health_fp);
    
    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_pressure; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_heartrate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_spo2; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_temp; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   DATE"); for(int i=0; i<(max_date-4); i++) printf(" "); printf("   ");
    printf("|   TIME"); for(int i=0; i<(max_time-4); i++) printf(" "); printf("   ");
    printf("|   PRESSURE"); for(int i=0; i<(max_pressure-8); i++) printf(" "); printf("   ");
    printf("|   HEARTRATE"); for(int i=0; i<(max_heartrate-9); i++) printf(" "); printf("   ");
    printf("|   SPO2"); for(int i=0; i<(max_spo2-4); i++) printf(" "); printf("   ");
    printf("|   TEMP."); for(int i=0; i<(max_temp-5); i++) printf(" "); printf("   |");
    printf("\n");
    

    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_pressure; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_heartrate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_spo2; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_temp; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", health[i].date); for(int j=0; j<(max_date+5-strlen(health[i].date)); j++) printf(" "); printf("|");
        printf(" %s", health[i].time); for(int j=0; j<(max_time+5-strlen(health[i].time)); j++) printf(" "); printf("|");
        printf(" %s", health[i].pressure); for(int j=0; j<(max_pressure+5-strlen(health[i].pressure)); j++) printf(" "); printf("|");
        printf(" %s", health[i].heartrate); for(int j=0; j<(max_heartrate+5-strlen(health[i].heartrate)); j++) printf(" "); printf("|");
        printf(" %s", health[i].spo2); for(int j=0; j<(max_spo2+5-strlen(health[i].spo2)); j++) printf(" "); printf("|");
        printf(" %s", health[i].temp); for(int j=0; j<(max_temp+5-strlen(health[i].temp)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_pressure; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_heartrate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_spo2; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_temp; i++) printf("-"); printf("---+");
    printf("\n");
}

void update_medication(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_medication");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  char medicine[256] = "", time[256] = "", date[15] = "", dose[256] = "", end[256] = "";

  printf("You are about to update the medication of the patient %s %s\n\n", fname, lname);
  //od bd td sos
  printf("Enter the name of the medicine (no spaces): ");
  scanf(" %s", medicine);

  printf("Enter the time of the medicine intake (od/bd/td/sos): ");
  scanf(" %s", time);

  printf("Enter the dosage of medicine: ");
  scanf(" %s", dose);

  printf("Enter the date till the medicine should be taken (MM/DD/YY): ");
  scanf(" %s", end);

  get_date(date);
  
  strcpy(cmd, "INSERT ROW;");
  strcat(cmd, src);
  strcat(cmd, ";time,dosage,medicine,date_started,date_end;");
  strcat(cmd, time);
  strcat(cmd, ",");
  strcat(cmd, dose);
  strcat(cmd, ",");
  strcat(cmd, medicine);
  strcat(cmd, ",");
  strcat(cmd, date);
  strcat(cmd, ",");
  strcat(cmd, end);
  
  _db(cmd);  
  
  char log[256] = "New medication was added for patient with token ";
  strcat(log, token);
  add_log(log); 
}

void see_medication(char token[]) {
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_medication");

  char path[256] = DB;
  strcat(path, src);

  char row[256] = "";
  FILE *med_fp = fopen(path, "r");
  fscanf(med_fp, "%s\n", row);
  int n = table_exists(src)-1;

  for(int i=0; i<n; i++) {
        char retrn = fscanf(med_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {//time,dosage,medicine,date_started,date_end
                if(index == 0) medication[i].time[len] = row[j];
                if(index == 1) medication[i].dosage[len] = row[j]; 
                if(index == 2) medication[i].medicine[len] = row[j]; 
                if(index == 3) medication[i].date_started[len] = row[j];
                if(index == 4) medication[i].date_end[len] = row[j];
                len++;
            }
        }
  }

    int max_time = 4;
    int max_dosage = 6;
    int max_medicine = 8;
    int max_date_started = 10;
    int max_date_end = 8;

    for(int i=0; i<n; i++) {
        if(strlen(medication[i].time)>max_time) max_time = strlen(medication[i].time);
        if(strlen(medication[i].dosage)>max_dosage) max_dosage = strlen(medication[i].dosage);
        if(strlen(medication[i].medicine)>max_medicine) max_medicine = strlen(medication[i].medicine);
        if(strlen(medication[i].date_started)>max_date_started) max_date_started = strlen(medication[i].date_started);
        if(strlen(medication[i].date_end)>max_date_end) max_date_end = strlen(medication[i].date_end);   
    }

    fclose(med_fp);
    
    printf("+---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_dosage; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_started; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_end; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   TIME"); for(int i=0; i<(max_time-4); i++) printf(" "); printf("   ");
    printf("|   DOSAGE"); for(int i=0; i<(max_dosage-6); i++) printf(" "); printf("   ");
    printf("|   MEDICINE"); for(int i=0; i<(max_medicine-8); i++) printf(" "); printf("   ");
    printf("|   START DATE"); for(int i=0; i<(max_date_started-10); i++) printf(" "); printf("   ");
    printf("|   END DATE"); for(int i=0; i<(max_date_end-8); i++) printf(" "); printf("   |");
    printf("\n");
    
    printf("+---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_dosage; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_started; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_end; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", medication[i].time); for(int j=0; j<(max_time+5-strlen(medication[i].time)); j++) printf(" "); printf("|");
        printf(" %s", medication[i].dosage); for(int j=0; j<(max_dosage+5-strlen(medication[i].dosage)); j++) printf(" "); printf("|");
        printf(" %s", medication[i].medicine); for(int j=0; j<(max_medicine+5-strlen(medication[i].medicine)); j++) printf(" "); printf("|");
        printf(" %s", medication[i].date_started); for(int j=0; j<(max_date_started+5-strlen(medication[i].date_started)); j++) printf(" "); printf("|");
        printf(" %s", medication[i].date_end); for(int j=0; j<(max_date_end+5-strlen(medication[i].date_end)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_time; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_dosage; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_started; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_date_end; i++) printf("-"); printf("---+");
    printf("\n");
}

void update_pharma_bill(char token[]) {
  //date,medicine,qty,rate,total
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_pharma");

  char cmd[256] = "GET ROW;patients;WHERE;token;";
  strcat(cmd, token);

  char val[256] = "";
  strcpy(val, _db(cmd).values);
  char e_val[256][256] = {};
  explode(val, ',', e_val);

  char fname[256] = "", lname[256] = "";
  strcpy(fname, e_val[1]);
  strcpy(lname, e_val[3]);

  char medicine[256] = "", date[15] = "", rate[256] = "", qty[256] = "", total[256] = "";

  printf("You are about to bill medicine's for the patient %s %s\n\n", fname, lname);
  
  int repeat = 0;
  do{
  printf("Enter the name of the medicine (no spaces): ");
  scanf(" %s", medicine);

  printf("Enter the rate of the medicine: ");
  scanf(" %s", rate);

  printf("Enter the quantity of medicine: ");
  scanf(" %s", qty);

  get_date(date);

  int int_qty = atoi(qty);
  double double_rate = atof(rate);

  double double_total = double_rate*((double) int_qty);
  sprintf(total, "%f", double_total);

  strcpy(cmd, "INSERT ROW;");
  strcat(cmd, src);
  strcat(cmd, ";date,medicine,qty,rate,total;");
  strcat(cmd, date);
  strcat(cmd, ",");
  strcat(cmd, medicine);
  strcat(cmd, ",");
  strcat(cmd, qty);
  strcat(cmd, ",");
  strcat(cmd, rate);
  strcat(cmd, ",");
  strcat(cmd, total);
  
  _db(cmd);

    printf("\nDo you want to bill again? (y/n): ");
    char c;
    scanf(" %c", &c);
    if (c == 'y') repeat = 1; else repeat = 0;
    printf("\n");
  } while (repeat == 1);


  char log[256] = "New pharmacy bill was added for patient with token ";
  strcat(log, token);
  add_log(log);
}

void see_pharma_bill(char token[]) {
  //date,medicine,qty,rate,total
  char src[256] = "patient/";
  strcat(src, token);
  strcat(src, "_pharma");

  char path[256] = DB;
  strcat(path, src);

  char row[256] = "";
  FILE *pharma_fp = fopen(path, "r");
  fscanf(pharma_fp, "%s\n", row);
  int n = table_exists(src)-1;

  double double_total = 0;

  for(int i=0; i<n; i++) {
        char retrn = fscanf(pharma_fp, "%s\n", row);

        int index = 0;
        int len =  0;
        for(int j=0; j<strlen(row); j++) {
            if(row[j] == ',') {
                index++;
                len = 0;
            } else {//time,dosage,medicine,date_started,date_end
                if(index == 0) pharmacy[i].date[len] = row[j];
                if(index == 1) pharmacy[i].medicine[len] = row[j]; 
                if(index == 2) pharmacy[i].qty[len] = row[j]; 
                if(index == 3) pharmacy[i].rate[len] = row[j];
                if(index == 4) pharmacy[i].total[len] = row[j];
                len++;
            }
        }
      
      double_total += atof(pharmacy[i].total);
  }

    int max_date = 4;
    int max_medicine = 8;
    int max_qty = 4;
    int max_rate = 4;
    int max_total = 5;

    for(int i=0; i<n; i++) {
        if(strlen(pharmacy[i].date)>max_date) max_date = strlen(pharmacy[i].date);
        if(strlen(pharmacy[i].medicine)>max_medicine) max_medicine = strlen(pharmacy[i].medicine);
        if(strlen(pharmacy[i].qty)>max_qty) max_qty = strlen(pharmacy[i].qty);
        if(strlen(pharmacy[i].rate)>max_rate) max_rate = strlen(pharmacy[i].rate);
        if(strlen(pharmacy[i].total)>max_total) max_total = strlen(pharmacy[i].total);   
    }

    fclose(pharma_fp);
    
    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_qty; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_rate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_total; i++) printf("-"); printf("---+");

    printf("\n");

    printf("|   DATE"); for(int i=0; i<(max_date-4); i++) printf(" "); printf("   ");
    printf("|   MEDICINE"); for(int i=0; i<(max_medicine-8); i++) printf(" "); printf("   ");
    printf("|   QTY."); for(int i=0; i<(max_qty-4); i++) printf(" "); printf("   ");
    printf("|   RATE"); for(int i=0; i<(max_rate-4); i++) printf(" "); printf("   ");
    printf("|   TOTAL"); for(int i=0; i<(max_total-5); i++) printf(" "); printf("   |");
    printf("\n");
    
    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_qty; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_rate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_total; i++) printf("-"); printf("---+");
    printf("\n");

    for(int i=0; i<n; i++) {
        printf("| %s", pharmacy[i].date); for(int j=0; j<(max_date+5-strlen(pharmacy[i].date)); j++) printf(" "); printf("|");
        printf(" %s", pharmacy[i].medicine); for(int j=0; j<(max_medicine+5-strlen(pharmacy[i].medicine)); j++) printf(" "); printf("|");
        printf(" %s", pharmacy[i].qty); for(int j=0; j<(max_qty+5-strlen(pharmacy[i].qty)); j++) printf(" "); printf("|");
        printf(" %s", pharmacy[i].rate); for(int j=0; j<(max_rate+5-strlen(pharmacy[i].rate)); j++) printf(" "); printf("|");
        printf(" %s", pharmacy[i].total); for(int j=0; j<(max_total+5-strlen(pharmacy[i].total)); j++) printf(" "); printf("|");
        printf("\n");   
    }

    printf("+---"); for(int i=0; i<max_date; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_medicine; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_qty; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_rate; i++) printf("-"); printf("---+");
    printf("---"); for(int i=0; i<max_total; i++) printf("-"); printf("---+");
    printf("\n");

    printf("\nYour total is NRs. %.3lf Only.\n", double_total);  
}