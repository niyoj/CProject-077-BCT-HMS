/******************************************************************************************************
*   Program name: Session Module for HIMS,
*   Author name: NiyojOli(THA077BCT029), 
*   Created Path: /modules/sessions.c,
*   Created Date: 19 Oct 2021, 08:00:06 (DD MON YYYY, HH:MM:SS),
*   Description: This  program file is responsible to handle sessions in terminal.
*******************************************************************************************************/

struct Session {
    char user_name[256];
    char logged_time[256];
    char token[256];
    char role[256];
};

struct Session _session;

//function create_session() is used to create a new session in the terminal
int create_session(char user[], char role[]) {
    //if there already exists a session return already exists error message
    if(strcmp(_session.user_name, "") != 0) {
        return 0;   //means someone is logged in already
    } else {
        strcpy(_session.user_name, user);

        get_time(_session.logged_time);

        char tohash[256] = {};
        strcpy(tohash, _session.user_name);
        strcat(tohash, _session.logged_time);
        char token[256] = {};

        hash(tohash, token);
        strcpy(_session.token, token);
        strcpy(_session.role, role);

        char cmd[512] = {};
        strcpy(cmd, "INSERT ROW;session;user,time,token,role;");
        strcat(cmd, _session.user_name);
        strcat(cmd, ",");
        strcat(cmd, _session.logged_time);
        strcat(cmd, ",");
        strcat(cmd, _session.token);
        strcat(cmd, ",");
        strcat(cmd, _session.role);
        _db(cmd);

        char log[256] = "Session has been created with token ";
        strcat(log, _session.token);
        add_log(log);

        return 1;   //means login was successfull
    }
}