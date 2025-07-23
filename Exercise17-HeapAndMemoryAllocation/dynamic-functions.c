#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address {
    int id;
    int set; 
    char *name;
    char *email;
    // Added to reduce string length calc and max calculations
    int name_len;
    int email_len;
};

struct Database {
    struct Address *rows;
    int max_data;
    int max_rows;
    // Required to prevent resizing from deleting used Address Rows
    int end_row;
    
};

struct Connection {
    FILE *file;
    struct Database *db;
};

/**
 * @brief       Closes the database file and frees all allocated memory 
 *              associated with the database connection.
 *
 * @param conn  Pointer to a Connection struct that holds the file and
 *              database pointer.
 */
void Database_close(struct Connection *conn){
    if(conn){
        if (conn->file)
            fclose(conn->file);
        if(conn->db){
            if (conn->db->rows){
                struct Address addr;
                for(int i = 0; i < conn->db->max_rows; i++){
                    addr = conn->db->rows[i];
                    free(addr.email);
                    free(addr.name);
                }
            }
            free(conn->db);
        }
        free(conn);
    }
}

/**
 * @brief           Closes the database file and frees all allocated memory 
 *                  associated with the database connection.
 *
 * @param message   Pointer to const char string that informs abort reason.
 * @param conn      Pointer to a Connection struct that holds the file and
 *                  database pointer.
 */
void db_abort(const char *message, struct Connection *conn){
    if (errno)
        perror(message);
    else
        printf("ERROR: %s\n", message);
    
    Database_close(conn);
    exit(EXIT_FAILURE);
}

/**
 * @brief Prints Address information contained at addr.
 *
 * @param addr  Pointer to Address struct. 
 */
void Address_print(struct Address *addr){
    printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

/**
 * @brief Allocates memory for the database and loads data from file
 * 
 * @param conn Pointer to Connection struct that holds file and database
 *             pointer.
 */
void Database_load(struct Connection *conn){

    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        db_abort("Failed to load database.", conn);

    conn->db->rows = malloc( sizeof(struct Address) * conn->db->max_rows);
    if(!conn->db->rows)
        db_abort("ERROR allocating memory for Database Rows.", conn);
    
    // Loads all addresses. Allocates memory and loads string data on set addrs
    struct Address addr;
    for(int i = 0; i < conn->db->max_rows; i++){
        addr = conn->db->rows[i];
        rc = fread(&addr, sizeof(struct Address), 1, conn->file);
        if (rc != 1)
            db_abort("Failed to load database.", conn);
        if (addr.set){
            addr.email = malloc(addr.email_len + 1);
            addr.name = malloc(addr.name_len + 1);
            if(!addr.email || !addr.name)
                db_abort("Error allocating memory for Address Strings.", conn);
            rc = fread(addr.name, sizeof(addr.name_len + 1), 1, conn->file);
            if (rc != 1)
                db_abort("Failed to load database.", conn);
            rc = fread(addr.email, sizeof(addr.email_len + 1), 1, conn->file);
            if (rc != 1)
                db_abort("Failed to load database.", conn);
        }
    }
}

/**
 * @brief Opens a database file in the specified mode and initializes the
 *        connection.
 *
 * @param filename  Null-terminated string representing the path to the 
 *                  database file.
 * @param mode      Character specifying the program mode:
 *                  - 'c': create a new database file (write-only, 
 *                         truncate existing)
 *                  - other: open existing file for reading and writing
 *
 * @return Pointer to a dynamically allocated Connection struct,
 *         or the program exits on error.
 */
struct Connection *Database_open(const char *filename, char mode){
    struct Connection *conn = malloc(sizeof(struct Connection));
    if (!conn)
        db_abort("Memory error", conn);
    conn->db = malloc(sizeof(struct Database));
    if(!conn->db)
        db_abort("Memory error", conn);
    
        if(mode == 'c'){
            // Initialize database data
            conn->db->max_data = MAX_DATA;
            conn->db->max_rows = MAX_ROWS;
            conn->db->end_row = 0;
            conn->db->rows = malloc(sizeof(struct Address) * MAX_ROWS);
            if (!conn->db->rows)
                db_abort("ERROR! Could not allocate memory for rows!", conn);

            conn->file = fopen(filename, "w");
        }else{
            conn->file = fopen(filename, "r+");
            if(conn->file){
                Database_load(conn);
            }
        }

        if(!conn->file)
            db_abort("Failed to open the file", conn);

        return conn;
}

/**
 * @brief Writes the Database data to the database file. 
 * 
 * Writes Database Struct, followed by Address struct, and if Address is set,
 * then writes name and email data to database as well.
 * 
 * @param conn Pointer to Connection struct holding file and database data.
 */
void Database_write(struct Connection *conn){
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1)
        db_abort("Failed to write database.", conn);

    struct Address *addr;
    for(int i = 0; i < conn->db->max_rows; i++){
        addr = &conn->db->rows[i];

        // Write the Address Struct
        fwrite(addr, sizeof(struct Address), 1, conn->file);

        // Write the name and email if Address is set
        if(addr->set){
            rc = fwrite(addr->name, addr->name_len, 1, conn->file);
            if(rc != 1)
                db_abort("Failed to write name to database.", conn);
            rc = fwrite(addr->email, addr->email_len, 1, conn->file);
            if(rc != 1)
                db_abort("Failed to write email to database.", conn);
        }
    }
    rc = fflush(conn->file);
    if(rc == -1)
        db_abort("Cannot flush database.", conn);
}

/**
 * @brief Initializes the rows array with empty Addresses with corresponding id
 * 
 * @param conn Pointer to Connection struct holding file and database data.
 */
void Database_create(struct Connection *conn){
    for(int i =0; i< conn->db->max_rows; i++){
        // struct Address is {id, set, name, email, name_len, email_len}
        conn->db->rows[i] = (struct Address) { i, 0, NULL, NULL, 0, 0};
    }
}

/**
 * @brief           Sets the Address data noted by given id.
 *
 * @param conn      Pointer to a Connection struct that holds the file and
 *                  database pointer.
 * @param id        Integer representing the location of the Address in the
 *                  database
 * @param name      String data containing the Name information
 * @param email     String data containing the Email information
 */
void Database_set(struct Connection *conn, int id, const char *name, const char *email){
    if(id >= conn->db->max_rows|| id < 1)
        db_abort("Invalid id!", conn);
    struct Address *addr = &conn->db->rows[id];
    
    if(addr->set)
        db_abort("Already set, delete it first", conn);
    
    char *tmp = realloc(addr->name, conn->db->max_data);
    if (!tmp)
        db_abort("ERROR allocating memory for Address Name!", conn);
    addr->name = tmp;

    if(!strncpy(addr->name, name, conn->db->max_data))
        db_abort("Name copy failed", conn);
    addr->name[conn->db->max_data - 1] = '\0';
    addr->name_len = strlen(addr->name) + 1;

    tmp = realloc(addr->email, conn->db->max_data);
    if (!tmp)
        db_abort("ERROR allocating memory for Address Name!", conn);
    addr->email = tmp;

    if(!strncpy(addr->email, email, conn->db->max_data))
        db_abort("Email copy failed", conn);
    addr->email[conn->db->max_data - 1] = '\0';
    addr->email_len = strlen(addr->email) + 1;
    
    addr->set = 1;
}

/**
 * @brief           Prints Address data set on specific Database id
 *
 * @param conn      Pointer to a Connection struct that holds the file and
 *                  database pointer.
 * @param id        Integer representing the location of the Address in the
 *                  database
 */
void Database_get(struct Connection *conn, int id){
    struct Address *addr = &conn->db->rows[id];

    if(addr->set)
        Address_print(addr);
    else
        db_abort("ID is not set", conn);
    
}

/**
 * @brief           Deletes the database row index Address Entry data.
 *
 * @param conn      Pointer to a Connection struct that holds the file and
 *                  database pointer.
 * @param id        Integer representing the location of the Address in the
 *                  database
 */
void Database_delete(struct Connection *conn, int id){
    
    if (!conn->db->rows[id].set)
        db_abort("ERROR! Cannot delete what is not set!", conn);

    // Free previous allocated memory
    free(conn->db->rows[id].email);
    free(conn->db->rows[id].name);
    
    conn->db->rows[id] = (struct Address) {id, 0, NULL, NULL, 0, 0};
    
}

/** 
 * @brief           Iterates the Database and prints all set addresses
 * 
 * @param conn      Pointer to a Connection struct that holds the file and
 *                  database pointer.
 * @param id        Integer representing the location of the Address in the
 *                  database
 */
void Database_list(struct Connection *conn){
    int i = 0;
    struct Database *db = conn->db;
    for(i = 0; i < MAX_ROWS; i++){
        struct Address *cur = &db->rows[i];

        if(cur->set){
            Address_print(cur);
        }
    }
}

/**
 * @brief Resizes the database row array to the provided input.
 * 
 * Reallocates the size of the rows array and changes the database max_rows
 * value. If the rows value provided is less than a currently set row, an 
 * error will be raised and a message will be thrown to delete any addresses
 * before attempting to resize into a smaller size.
 * 
 * @param conn Pointer to Connection struct containing file and database.
 * @param rows New value to resize the database rows array to.
 */
void Database_resize_rows(struct Connection *conn, int rows){

}

/**
 * @brief Resizes the name and email max string data size. 
 * 
 * Reallocates the size of the email and name strings in all set Addresses
 * and cuts off any data that is over that limit. The last bit will always
 * be null terminated.
 * 
 * @param conn Pointer to Connection struct containing file and database.
 * @param data New value to resize the database max_data value to.
 */
void Database_resize_data(struct Connection *conn, int data){
    // TODO

}