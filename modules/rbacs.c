void get_role(char user[], char retrn[]) {
    db temp;
    char cmd[256] = "GET ROW;users;WHERE;username;";
    strcat(cmd, user); 
    temp = _db(cmd);
    char e_val[][256] = {};
    explode(temp.values, ',', e_val);

    strcpy(retrn, e_val[4]);   
}