int num_users() {
    return table_exists("users")-1;
}

int num_patients() {
    return table_exists("patients")-1;
}