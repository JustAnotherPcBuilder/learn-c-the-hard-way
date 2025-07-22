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
    int max; 
};

struct Database {
    struct Address *rows;
    int max_data;
    int max_rows;
    // Required to prevent resizing from deleting used Address Rows
    int end_row;
    int largest_data;
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
 * @brief           Closes the database file and frees all allocated memory associated
 *                  with the database connection.
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






void Database_load(struct Connection *conn){

    int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
    if (rc != 1)
        db_abort("Failed to load database.", conn);
    conn->db->rows = malloc( sizeof(struct Address));
}





/**
 * @brief Opens a database file in the specified mode and initializes the connection.
 *
 * @param filename  Null-terminated string representing the path to the database file.
 * @param mode      Character specifying the program mode:
 *                  - 'c': create a new database file (write-only, truncate existing)
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









void Database_write(struct Connection *conn){
    rewind(conn->file);

    int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
    if(rc != 1)
        db_abort("Failed to write database.", conn);

    rc = fflush(conn->file);
    if(rc == -1)
        db_abort("Cannot flush database.", conn);
}




void Database_create(struct Connection *conn){
    int i = 0;

    for(i =0; i< MAX_ROWS; i++){
        // make a prototype to inititalize it 
        struct Address addr = { .id = i, .set = 0};
        // then just assign it 
        conn->db->rows[i] = addr;
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
    
    char *tmp = malloc(conn->db->max_data);
    if (!tmp)
        db_abort("ERROR allocating memory for Address Name!", conn);
    addr->name = tmp;

    if(!strncpy(addr->name, name, conn->db->max_data))
        db_abort("Name copy failed", conn);
    addr->name[conn->db->max_data - 1] = '\0';

    tmp = malloc(conn->db->max_data);
    if (!tmp)
        db_abort("ERROR allocating memory for Address Name!", conn);
    addr->email = tmp;

    if(!strncpy(addr->email, email, conn->db->max_data))
        db_abort("Email copy failed", conn);
    addr->email[conn->db->max_data - 1] = '\0';
    
    int email_len = strlen(email);
    int name_len = strlen(name);
    addr->max = (name_len > email_len) ? name_len : email_len;

    if (conn->db->largest_data < conn->db->max_data){
        if (addr->max > conn->db->max_data)
            conn->db->largest_data = conn->db->max_data;
        else if(addr->max > conn->db->largest_data) 
            conn->db->largest_data = addr->max;
    }
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
    struct Address *addr = &conn->db->rows[id];
    
    // Free previous allocated memory
    free(addr->email);
    free(addr->name);
    
    addr->email = NULL;
    addr->name = NULL;

    // Recalculate db largest data size
    int largest = 0;
    for(int i = 0; i < conn->db->max_rows; i++){
        if(!conn->db->rows[i].set)
            continue;
        if(conn->db->rows[i].max > largest)
            largest = conn->db->rows[i].max;
    }
    conn->db->largest_data = largest;
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