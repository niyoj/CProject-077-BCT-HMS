//function start_logging() is used to add the header file which denotes staring of the logging in logs file
void start_logging() {
    FILE* flog;

    //getting the date and time in string format
    time_t t;
    t=time(NULL);
    char start_time[256] = {};
    strftime(start_time, sizeof(start_time), "%x %X", localtime(&t));

    flog = fopen(LOGS, "a");        //oppening the logs file in append mode

    fprintf(flog, "\n# Program is starting at time %s\n", start_time);  //adding the header in logs file
    fclose(flog);
}

//function add_log() is used to create a new log entry
void add_log(char log[]) {
    FILE* flog;

    flog = fopen(LOGS, "a");

    fprintf(flog, "- %s\n", log);   //adding a new entry
    fclose(flog);    
}