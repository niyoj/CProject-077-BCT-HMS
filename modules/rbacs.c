void get_role(char user[], char retrn[]) {
    char e_row[256][256] = {};
    char role_cmd[256] = "GET ROW;users;WHERE;username;";
    strcat(role_cmd, user);
    explode(_db(role_cmd).values, ',', e_row);
    strcpy(retrn, e_row[4]);
}