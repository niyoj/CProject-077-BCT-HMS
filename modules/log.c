FILE* flog;

void start_logging() {
    time_t t;
    t=time(NULL);
    char start_time[256] = {};

    strftime(start_time, sizeof(start_time), "%x %X", localtime(&t));
    flog = fopen(LOGS, "a");

    fprintf(flog, "\n# Program is starting at time %s\n", start_time);
    fclose(flog);
}

void add_log(char log[]) {
    flog = fopen(LOGS, "a");

    fprintf(flog, "- %s\n", log);
    fclose(flog);    
}