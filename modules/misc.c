//function get_date() is used to get the today's date
void get_date(char retrn[]) {
    char date[15] = {};
    time_t t;
    t=time(NULL);
    strftime(date, sizeof(date), "%x", localtime(&t));
    strcpy(retrn, date);
}

//function get_time() is used to get the today's date
void get_time(char retrn[]) {
    char date[30] = {};
    time_t t;
    t=time(NULL);
    strftime(date, sizeof(date), "%x %X", localtime(&t));
    strcpy(retrn, date);
}