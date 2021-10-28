void attend_today() {
    char date[15] = {};
    time_t t;
    t=time(NULL);
    strftime(date, sizeof(date), "%x", localtime(&t));

    char cmd[256] = "GET ROW;attendance;WHERE;username;";
    strcat(cmd, _env.logged_user);
    strcat(cmd, ";AND;date;");
    strcat(cmd, date);

    if(_db(cmd).code == 0) {
        strcpy(cmd, "INSERT ROW;attendance;username,role,date;");
        strcat(cmd, _env.logged_user);
        strcat(cmd, ",");
        strcat(cmd, _env.role);
        strcat(cmd, ",");
        strcat(cmd, date);
        _db(cmd);
        add_log("Attendance for today was done succssfully");
    }
}

void show_attended(char role[]) {
    char date[15] = {};
    time_t t;
    t=time(NULL);
    strftime(date, sizeof(date), "%x", localtime(&t));

    char cmd[256] = "GET ROW;attendance;WHERE;role;";
    strcat(cmd, role);
    strcat(cmd, ";AND;date;");
    strcat(cmd, date);
    
    char row[256] = {};
    strcpy(row, _db(cmd).values);

    if(strlen(row) == 0) printf("No entry found for the day.\n");

    char e_row[256][256] = {};
    explode(row, ';', e_row);

    for(int i=0; strlen(e_row[i]) != 0; i++) {
        char username[256] = {};
        int j = 0, k = 0;
        while(e_row[i][j] != ',') {
        username[k] = e_row[i][j];
        j++;
        k++;
        }
        username[strlen(username)] = '\0';
        printf("    => %s\n", username);
    }
}