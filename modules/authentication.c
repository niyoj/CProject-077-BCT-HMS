int authenticate(char user[], char passwd[]) {
    char cmd[256] = "GET ROW;users;WHERE;nickname;";
    strcat(cmd, user);

    db data = _db(cmd);
    if (data.rows != 0) {
        char e_data[][256] = {};
        explode(data.values, ',', e_data);
        if(strcmp(e_data[3], passwd) == 0) {
            return 1;
        }
    }

    return 0;
}