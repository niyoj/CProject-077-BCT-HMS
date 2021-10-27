void attend_today() {
    char date[15] = {};
    time_t t;
    t=time(NULL);
    strftime(date, sizeof(date), "%x", localtime(&t));

    char cmd[256] = "INSERT ROW;attendance;user,role,date;";
    strcat(cmd, _env.logged_user);
    strcat(cmd, ",");
    strcat(cmd, _env.role);
    strcat(cmd, ",");
    strcat(cmd, date);
    
    _db(cmd);
}