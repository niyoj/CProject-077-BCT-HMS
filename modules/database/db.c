/******************************************************************************************************
*   Program name: db.c,
*   Author name: NiyojOli,
*   Created Path: ./db.c,
*   Created Date: 08 Aug 2021, 12:21:10 (DD MON YYYY, HH:MM:SS),
*   Description: This  program acts as the database module and consists of function to interact with database module.
*******************************************************************************************************/

/*
The commands and its format to be given for the database modules are as follows;
# For Creating New Table
* CREATE TABLE;<TABLE_NAME>;<FILED1>,<FIELD2>,<FIELD3>,...
    * To be  noted that, the fieldname can contain only 0 to 1, _, A to Z or a to z.

# For Deleting Table and Table Field
* DELETE TABLE;<TABLE_NAME>
* DELETE ROW;<TABLE_NAME>;WHERE;<FIELD_NAME>;<VALUE>

# For Inserting a Row
* INSERT ROW;<TABLE_NAME>;<FIELD_NAME>,...;<VALUE>,...

# For Updating a Row 
* UPDATE ROW;<TABLE_NAME>;WHERE;<FIELD_NAME>;<VALUE>;AS;<COLUMN NAME>;<NEW VALUE>

# For Retriving a Row
* GET ROW;<TABLE_NAME>;WHERE;<FIELD_NAME>;<VALUE>

*/
#define DB ".db/tables/"    //denotes the path of the .db folder where every tables are stored in csv format

//struct databse_retrn is used to store the return information from the database
struct database_retrn {
    int rows;               //represents the number of rows affected
    int code;               //represents the return code
    char header[256];       //represents the header
    char values[256];       //represents the values
};

//declaring new type definition for database
typedef struct database_retrn db;

//function prototyping starts here
int table_exists(char[]);
int create_table(char[], char[]);
int insert_row(char[], char[], char[]);
db get_row(char[], char[], char[]);
int delete_row(char[], char[], char[]);
int delete_table(char[]);
int update_row(char[], char[], char[], char[], char[]);
//function prototyping ends here

//function explode() is used to divide the string into severa chunks from the character specified by separator argument
int explode(char string[], char separator, char retrn[][256]) {
    int i=0, j=0, k=0;
    
    while(string[i] != '\0') {
        if(string[i] == separator) { 
            k = 0;
            j++;
        } else {
            retrn[j][k] = string[i];
            k++;
        }
        i++;
    }    
}

//function _db() us the main funciton from where commands are processed and results are returned 
db _db(char cmd[]) {
    db retrn;                   //retrn structure is used to store the return values
    
    //sanitizing the structure to prevent garabage values
    retrn.rows =  0;            
    retrn.code = 0;
    strcpy(retrn.header, "");
    strcpy(retrn.values, "");

    //declaring important variables needed during the program execution
    char e_cmd[256][256] = {};
    explode(cmd, ';', e_cmd);
    
    //processing the command entered by the user
    if(strcmp(e_cmd[0], "CREATE TABLE") == 0) {
        int status = create_table(e_cmd[1], e_cmd[2]);
        retrn.code = status;
        strcpy(retrn.header, e_cmd[2]);
    } else if(strcmp(e_cmd[0], "INSERT ROW") == 0) {
        int status = insert_row(e_cmd[1], e_cmd[2], e_cmd[3]);
        retrn.code = status;
        retrn.rows = 1;
        strcpy(retrn.header, e_cmd[2]);
        strcpy(retrn.values, e_cmd[3]);
    } else if(strcmp(e_cmd[0], "GET ROW") == 0) {
        retrn = get_row(e_cmd[1], e_cmd[3], e_cmd[4]);
    } else if(strcmp(e_cmd[0], "UPDATE ROW") == 0) {
        int status = update_row(e_cmd[1], e_cmd[3], e_cmd[4], e_cmd[6], e_cmd[7]);
        retrn.rows++;
        retrn.code = status;
    } else if(strcmp(e_cmd[0], "DELETE ROW") == 0) {
        int status = delete_row(e_cmd[1], e_cmd[3], e_cmd[4]);
        retrn.code = status;
    } else if(strcmp(e_cmd[0], "DELETE TABLE") == 0) {
        int status = delete_table(e_cmd[1]);
        retrn.code = status;
    }

    //UPDATE ROW;users;where;username;niyoj;as;header;values
    return retrn; 
}

//function table_exists() is used to determine wether the table already exists or not
int table_exists(char table[]) {
    //preparing the path for the table
    char src[256] = DB;
    strcat(src, table);                                   

    //checking if the system is windows or linux
    if(strcmp(SYS, "LINUX") == 0) {
        //checking if table exists or not
        if(access(src, F_OK) == 0) {
            //if table exists then get the number of rows in the table
            int num = 0;
            FILE* db_table = fopen(src, "r");
            char c;

            while((c = getc(db_table)) != EOF) {
                if(c == '\n') num++;
            }

            fclose(db_table);
            return num;     //returning the number of rows in the table
        } else {
            if( access( src, F_OK ) != -1) {
                //if table exists then get the number of rows in the table
                int num = 0;
                FILE* db_table = fopen(src, "r");
                char c;

                while((c = getc(db_table)) != EOF) {
                    if(c == '\n') num++;
                }

                fclose(db_table);
                return num;     //returning the number of rows in the table
            }
        }
    }
    return 0;
}

//function create_table() is used to create a new table in the database
int create_table(char table[], char data[]) {
    //checking if the table already exists or not
    if(!table_exists(table)) {
        //preparing the path of the table entered
        char src[256] = DB;
        strcat(src, table);

        //creating a new file with the particular name in the particular path
        FILE* db_create = fopen(src, "w");
        if(db_create == NULL) {
            return 0;       //if the file was not created return 0
        } else {
            fprintf(db_create, "%s\n", data);   //if file was created add the header values
            fclose(db_create);
            return 1;
        }
    } else {
        return 2;    //if table does exist already return 2
    }
}

//function insert_row() is used to insert a row in the  table in the database
int insert_row(char table[], char header[], char values[]) {
    //preparing for the path of the table
    char src[256] = DB;
    strcat(src, table);

    //checking if table exists or not
    if(!table_exists(table)) return 0;

    //preparing the variables needed durig the implementation
    char e_header[256][256] = {}, e_values[256][256] = {};
    explode(header, ',', e_header);
    explode(values, ',', e_values);

    char r_header[256] = {};        //stores the original header of the table
    
    FILE* db_row;
    db_row = fopen(src, "r");
    fscanf(db_row, "%s\n", r_header);       //getting the header of the table
    fclose(db_row);

    //exploding the real header on the basis of comma
    char er_header[256][256] = {};
    explode(r_header, ',', er_header);

    char toadd[256] = {};   //stores the string to be appended at last

    //comparison occurs here and string to be stored in database is generated and stored in toadd from here
    for(int i=0; strlen(er_header[i]) != 0; i++) {
        int got = 0;        //indicates whether or not the field is passed
        for(int j=0; strlen(e_header[j]) != 0; j++) {
            //checks if the real header and header passed matches or not for each column
            if(strcmp(er_header[i], e_header[j]) == 0) {
                strcat(toadd, e_values[j]);
                strcat(toadd, ",");
                got = 1;
                break;
            }
        }

        if(got == 0) strcat(toadd, ",");
    }
    toadd[strlen(toadd)-1] = '\0';      //removing extra comma

    db_row = fopen(src, "a");           //appending the string
    fprintf(db_row, "%s\n", toadd);

    fclose(db_row);
    return 1;
}

//function get_row() is used to get the row where desired condition is met
db get_row(char table[], char cond[], char cond_val[]) {
    char src[256] = DB;
    strcat(src, table);
    db retrn;
    int found = 0;

    if(!table_exists(table)){ retrn.code = 0; return retrn; }

    int row = table_exists(table);
    FILE* db_get = fopen(src, "r");

    char header[256] = {}, e_header[256][256] = {};
    fscanf(db_get, "%s\n", header);
    explode(header, ',', e_header);

    for(int i=0; i<row-1; i++) {
        char row[256] = {};
        fscanf(db_get, "%s\n", row);

        char e_row[256][256] = {};
        explode(row, ',', e_row);

        for(int j=0; strlen(e_header[j]) != 0; j++) {
            if(strcmp(e_header[j], cond) == 0) {
                if (strcmp(e_row[j], cond_val) == 0) {
                    strcat(retrn.values, row);
                    strcat(retrn.values, ";");
                    retrn.rows = i+2;
                    found = 1;
                }
            }
        }
    }
    retrn.values[strlen(retrn.values)-1] = '\0';
    strcpy(retrn.header, header);
    retrn.code = found;

    fclose(db_get);
    return retrn;
}

int update_row(char table[], char cond_field[], char cond_val[], char header[], char val[]) {
    if(!table_exists(table)) return 0;

    char append[256] = {};
    //getrow, prepare, delete, insert

    db row_toappend = get_row(table, cond_field, cond_val);
    char real_header[256] = {};
    strcpy(real_header, row_toappend.header); //
    char real_val[256] = {};
    strcpy(real_val, row_toappend.values);

    char e_val[256][256] = {};
    explode(real_val, ';', e_val);

    char e_real_header[256][256] = {};
    explode(real_header, ',', e_real_header);

    int target;
    for(int i=0; strlen(e_real_header[i]) != 0; i++) {
        if(strcmp(header, e_real_header[i]) == 0) target = i;
    }

    for(int i=0; strlen(e_val[i]) != 0; i++) {
        char further_e_val[256][256] = {};
        explode(e_val[i], ',', further_e_val);

        for(int j=0; strlen(further_e_val[j]) != 0; j++) {
            if (j == target) {
                strcat(append, val);
            } else {
                strcat(append, further_e_val[j]);
            }
            strcat(append, ",");
        }

        append[strlen(append)-1] = '\0';
        strcat(append, ";");
    }
    append[strlen(append)-1] = '\0';

    delete_row(table, cond_field, cond_val);

    char e_append[256][256] = {};
    explode(append, ';', e_append);

    for(int i=0; strlen(e_append[i]) != 0; i++) {
        insert_row(table, real_header, e_append[i]);
    }

    return 1;
}

int delete_row(char table[], char cond_field[], char cond_val[]) {
    char src[256] = DB;
    strcat(src, table);
    FILE* fp_r = fopen(src, "r");
    
    char tsrc[256] = DB;
    strcat(tsrc, "temp");
    FILE* fp_w = fopen(tsrc, "w");

    int target = get_row(table, cond_field, cond_val).rows;
    char row[256] = {};
    
    int read = 1;
    while(read <= table_exists(table)) {
        fscanf(fp_r, "%s\n", row);
        
        if(read != target) {
            fprintf(fp_w, "%s\n", row);
        }
        read++;
    }

    fclose(fp_r);
    fclose(fp_w);

    fp_r = fopen(tsrc, "r");
    fp_w = fopen(src, "w");
    char c = '\0';
    while((c = getc(fp_r)) != EOF) {
        putc(c, fp_w);
    }

    fclose(fp_r);
    fclose(fp_w);

    if (get_row(table, cond_field, cond_val).rows > 0) {
        return delete_row(table, cond_field, cond_val);
    }

    fp_w = fopen(tsrc, "w");
    fclose(fp_w);

    return 1;
}

int delete_table(char table[]) {
    if (!table_exists(table)) return 0;
    char src[256] = DB;
    strcat(src, table);            //setting up the src

    if(remove(src) == 0) return 1; else return 0;
}