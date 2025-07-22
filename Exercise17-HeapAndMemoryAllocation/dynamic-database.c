#include "dynamic-functions.c"


int main(int argc, char* argv[]){
    if(argc < 3)
        db_abort("USAGE: ex17-2 <dbfile> <action> [action params]", NULL);
    
    char *filename = argv[1];
    char action = argv[2][0];
    struct Connection *conn = Database_open(filename, action);
    int id = 0;

    if(argc > 3) id = atoi(argv[3]);
    char buf[1000];
    sprintf(buf, "There's not that many records{%s}.", argv[3]);
    if(id >= MAX_ROWS)
        db_abort(buf , conn);
    
    switch(action){
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;
        case 'g':
            if (argc != 4)
                db_abort("Need an id to get", conn);
            Database_get(conn, id);
            break;
        case 's':
            if (argc != 6)
                db_abort("Need id, name, email to set", conn);

            Database_set(conn, id, argv[4], argv[5]);
            Database_write(conn);
            break;
        case 'd':
            if (argc != 4)
                db_abort("Need id to delete", conn);
            
            Database_delete(conn, id);
            Database_write(conn);
            break;
        case 'l':
            Database_list(conn);
            break;
        
        case 'D':
            if (argc != 4)
                db_abort("Need MAX_DATA to set", conn);
            char *endptr = NULL;
            int max_data = (int) strtol(argv[3], &endptr, 10);
            if(*endptr != '\0' || max_data == 0){
                db_abort("Invalid input for MAX_DATA!", conn);
                return EXIT_FAILURE;
            }
            printf("New MAX_DATA: %d\n", max_data);
            break;

        case 'R':
            break;
        default:
            db_abort("Invalid action: c=create, g=get, s=set, d=del, l=list", conn);
    }
    Database_close(conn);
    return EXIT_SUCCESS;
}