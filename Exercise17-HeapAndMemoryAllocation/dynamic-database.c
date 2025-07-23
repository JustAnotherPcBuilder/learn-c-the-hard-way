#include "dynamic-functions.c"


int main(int argc, char* argv[]){
    if(argc < 3)
        db_abort("USAGE: dynamic-database <dbfile> <action> [action params]", NULL);
    
    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;
    
    switch(action){
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            if (argc != 4)
                db_abort("Need an id to get", conn);
            id = atoi(argv[3]);
            if(id >= conn->db->max_rows)
                db_abort("There's not that many records." , conn);
            Database_get(conn, id);
            break;

        case 's':
            if (argc != 6)
                db_abort("Need id, name, email to set", conn);
            id = atoi(argv[3]);
            if(id >= conn->db->max_rows)
                db_abort("There's not that many records." , conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;

        case 'd':
            if (argc != 4)
                db_abort("Need id to delete", conn);
            id = atoi(argv[3]);
            if(id >= conn->db->max_rows)
                db_abort("There's not that many records." , conn);
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;
        
        case 'D':
            if (argc != 4)
                db_abort("Need MAX_DATA to set", conn);
            int data = atoi(argv[3]);
            Database_resize_data(conn, data);
            Database_write(conn);
            break;

        case 'R':
            if (argc != 4)
                db_abort("Need MAX_ROWS to set", conn);
            int rows = atoi(argv[3]);
            Database_resize_rows(conn, rows);
            Database_write(conn);
            break;
        default:
            db_abort("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
    }
    Database_close(conn);
    return EXIT_SUCCESS;
}