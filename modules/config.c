/******************************************************************************************************
*   Program name: Configuration Module for HIMS,
*   Author name: NiyojOli(THA077BCT029), 
*   Created Path: /modules/config.c,
*   Created Date: 19 Oct 2021, 08:01:50 (DD MON YYYY, HH:MM:SS),
*   Description: This  program file is responsible to include all configuration and environment variables.
*******************************************************************************************************/

//stuct Config is used to declare necessary configurations for the program
struct Config {
    char app_name[256];         //denotes the name of the program
    char app_version[25];       //denotes the version of the program
};

//struct Env is used to store the necessary environemnt variable
struct Env {
    int is_logged_in;
    char logged_user[256];
    char role[256];
};

struct Config _config;
struct Env _env;

//function declare_config() is used to declare the configuration variables
void declare_config(void) {
    strcpy(_config.app_name, "HIMS");        //you can replace this with your app name
    strcpy(_config.app_version, "v1.0.0");
}

//function prepare_env() is used to prepare the environment for the program
void prepare_env(void) {
    if(!table_exists("session")) _db("CREATE TABLE;session;user,time,token,role");
    if(!table_exists("users")) _db("CREATE TABLE;users;fname,lname,nickname,password,role");
    if(!table_exists("patient")) _db("CREATE TABLE;patients;fname,mname,lname,gender,age,department,doctor,status");

    declare_config();           //declaring all the configuration for the app

    if(strcmp(_session.user_name, "") == 0) {
        _env.is_logged_in = 0;
    } else {
        _env.is_logged_in = 1;
        strcpy(_env.logged_user, _session.user_name);
        strcpy(_env.role, _session.role);
    }
}