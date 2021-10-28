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