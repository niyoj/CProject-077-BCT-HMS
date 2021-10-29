void attend_today() {
    char date[15] = {};
    get_date(date);

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

void day_end() {
    char date[15] = {};
    get_date(date);

    char cmd[256] = "GET ROW;leave;WHERE;username;";
    strcat(cmd, _env.logged_user);
    strcat(cmd, ";AND;date;");
    strcat(cmd, date);

    if(_db(cmd).code == 0) {
        strcpy(cmd, "INSERT ROW;leave;username,role,date;");
        strcat(cmd, _env.logged_user);
        strcat(cmd, ",");
        strcat(cmd, _env.role);
        strcat(cmd, ",");
        strcat(cmd, date);
        _db(cmd);
        add_log("Day end was successfully submitted");
        printf("Thank you for being today in you job.\n");
    } else {
        printf("You have already submitted your day end.\n");
    }
}
void show_attended(char role[]) {
    char date[15] = {};
    get_date(date);

    char cmd[256] = "GET ROW;attendance;WHERE;role;";
    strcat(cmd, role);
    strcat(cmd, ";AND;date;");
    strcat(cmd, date);
    
    char attend_row[256] = {};
    strcpy(attend_row, _db(cmd).values);

    strcpy(cmd, "GET ROW;leave;WHERE;role;");
    strcat(cmd, role);
    strcat(cmd, ";AND;date;");
    strcat(cmd, date);
    
    char leave_row[256] = {};
    strcpy(leave_row, _db(cmd).values);
    
    if(strlen(attend_row) == 0) printf("No entry found for the day.\n");
    
    char e_attend_row[256][256] = {};
    explode(attend_row, ';', e_attend_row);

    char e_leave_row[256][256] = {};
    explode(leave_row, ';', e_leave_row);

    int left = 0;
    char left_role[256][256] = {};

    for(int i=0; strlen(e_leave_row[i]) != 0; i++) left++;
    
    for(int i=0; i<left; i++) {
      char uname[256] = {};
      int j=0; int k=0;

      while(e_leave_row[i][j] != ',') {
        uname[k] = e_leave_row[i][j];
        j++;
        k++;
      }
      uname[strlen(uname)] = '\0';
      strcpy(left_role[i], uname);
    }

    for(int i=0; strlen(e_attend_row[i]) != 0; i++) {
        char username[256] = {};
        int j = 0, k = 0;
        while(e_attend_row[i][j] != ',') {
          username[k] = e_attend_row[i][j];
          j++;
          k++;
        }
        username[strlen(username)] = '\0';
        
        j = 0;
        int found = 0;
        for(; strlen(left_role[j]) != 0; j++) {
          if(strcmp(username, left_role[j]) == 0) {
            found = 1;
          }
        }
        if(found == 0) {
          printf("    => %s\n", username);
        }
    }
}