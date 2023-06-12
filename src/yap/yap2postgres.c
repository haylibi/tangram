#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <Yap/YapInterface.h>

static int db_connect(void)
{
    // yap terms
    YAP_Term arg_host = YAP_ARG1;
    YAP_Term arg_dbname = YAP_ARG2;
    YAP_Term arg_user = YAP_ARG3;
    YAP_Term arg_passwd = YAP_ARG4;
    YAP_Term arg_conn = YAP_ARG5;

    // postgres connection
    PGconn *conn;

    // conn variables
    char *host = YAP_AtomName(YAP_AtomOfTerm(arg_host));
    char *dbname = YAP_AtomName(YAP_AtomOfTerm(arg_dbname));
    char *user = YAP_AtomName(YAP_AtomOfTerm(arg_user));
    char *passwd = YAP_AtomName(YAP_AtomOfTerm(arg_passwd));

    // string connection
    char conninfo[1024];
    sprintf(conninfo, "host=%s dbname=%s user=%s password=%s",
            host, dbname, user, passwd);

    // establish a connection to the PostgreSQL database
    conn = PQconnectdb(conninfo);

    // Check if the connection was successful
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return FALSE;
    }

    //verifica se o yap conecta
    if(!YAP_Unify(arg_conn, YAP_MkIntTerm((long) conn)))
        return FALSE;
    return TRUE;
}

static int db_disconnect(void){
    YAP_Term arg_conn = YAP_ARG1;
    PGconn *conn= (PGconn *) YAP_IntOfTerm(arg_conn);
    PQfinish(conn);
    return TRUE;
}

int db_query(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);
    char *query = YAP_AtomName(YAP_AtomOfTerm(YAP_ARG2));

    printf("Query: %s\n", query);

    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return FALSE;
    }
    PQclear(res);
    return (YAP_Unify(YAP_ARG3, YAP_MkIntTerm((int)res)));
}

int db_row(void)
{
    PGresult *res = (PGresult *)YAP_IntOfTerm(YAP_ARG1);

    int num_rows = PQntuples(res);
    int num_columns = PQnfields(res);

    if (num_rows != 0)
    {
        YAP_Term head, list = YAP_ARG2;
        for (int i = 0; i < num_columns; i++)
        {
            head = YAP_HeadOfTerm(list);
            list = YAP_TailOfTerm(list);
            if (!YAP_Unify(YAP_ARG2, YAP_MkStringTerm(PQgetvalue(res, 1, i))))
                return FALSE;
        }
        return TRUE;
    }

    PQclear(res);
    YAP_cut_fail();
}

int db_difference(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    YAP_Term arg_source = YAP_ARG2;
    YAP_Term arg_target = YAP_ARG3;
    YAP_Term arg_result = YAP_ARG4;

    // postgres connection
    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);

    // conn variables
    char *source = YAP_AtomName(YAP_AtomOfTerm(arg_source));
    char *target = YAP_AtomName(YAP_AtomOfTerm(arg_target));
    char *result = YAP_AtomName(YAP_AtomOfTerm(arg_result));

    // Prepare the SQL statement with the ST_Difference function
    char query[500];
    snprintf(query, sizeof(query), "SELECT ST_AsText(ST_Difference('%s'::geometry, '%s'::geometry)) as difference", source, target);

    // establish a connection to the PostgreSQL database
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return FALSE;
    }

    if (PQresultStatus(res) == PGRES_TUPLES_OK)
    {
        char *result = PQgetvalue(res, 0, 0);
        printf("Difference: %s\n", result);
    }

    // Clean up
    PQclear(res);
    return (YAP_Unify(YAP_ARG3, YAP_MkIntTerm((int)result)));
}

int db_rotate(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    YAP_Term arg_geometry = YAP_ARG2;
    YAP_Term arg_angle = YAP_ARG3;
    YAP_Term arg_result = YAP_ARG4;

    // PostgreSQL connection
    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);

    // Input variables
    char *geometry = YAP_AtomName(YAP_AtomOfTerm(arg_geometry));
    double angle = YAP_FloatOfTerm(arg_angle);
    char *result;

    // Prepare the SQL statement with the ST_Rotate function
    char query[500];
    snprintf(query, sizeof(query), "SELECT ST_AsText(ST_Rotate('%s'::geometry, %f)) AS rotated_geometry", geometry, angle);

    // Execute the SQL statement
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return FALSE;
    }

    // Extract the result
    if (PQntuples(res) > 0)
    {
        result = PQgetvalue(res, 0, 0);
        printf("Rotated Geometry: %s\n", result);
    }
    else
    {
        fprintf(stderr, "No result found\n");
        PQclear(res);
        return FALSE;
    }

    // Clean up
    PQclear(res);

    // Unify the result term with the YAP argument
    return YAP_Unify(arg_result, YAP_MkAtomTerm(YAP_LookupAtom(result)));
}

int db_translate(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    YAP_Term arg_geometry = YAP_ARG2;
    YAP_Term arg_x = YAP_ARG3;
    YAP_Term arg_y = YAP_ARG4;
    YAP_Term arg_result = YAP_ARG5;

    // PostgreSQL connection
    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);

    // Input variables
    char *geometry = YAP_AtomName(YAP_AtomOfTerm(arg_geometry));
    double x = YAP_FloatOfTerm(arg_x);
    double y = YAP_FloatOfTerm(arg_y);
    char *result;

    // Prepare the SQL statement with the ST_Translate function
    char query[500];
    snprintf(query, sizeof(query), "SELECT ST_AsText(ST_Translate('%s'::geometry, %f, %f)) AS translated_geometry", geometry, x, y);

    // Execute the SQL statement
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return FALSE;
    }

    // Extract the result
    if (PQntuples(res) > 0)
    {
        result = PQgetvalue(res, 0, 0);
        printf("Translated Geometry: %s\n", result);
    }
    else
    {
        fprintf(stderr, "No result found\n");
        PQclear(res);
        return FALSE;
    }

    // Clean up
    PQclear(res);

    // Unify the result term with the YAP argument
    return YAP_Unify(arg_result, YAP_MkAtomTerm(YAP_LookupAtom(result)));
}

int db_insert_solution(void)
{
    YAP_Term arg_conn = YAP_ARG1;
    YAP_Term arg_name = YAP_ARG2;
    YAP_Term arg_geometry = YAP_ARG3;

    // PostgreSQL connection
    PGconn *conn = (PGconn *)YAP_IntOfTerm(arg_conn);

    // Input variables
    char *name = YAP_AtomName(YAP_AtomOfTerm(arg_name));
    char *geometry = YAP_AtomName(YAP_AtomOfTerm(arg_geometry));

    // Prepare the SQL statement for insertion
    char query[500];
    snprintf(query, sizeof(query), "INSERT INTO solution (name, shape) VALUES ('%s', '%s'::geometry)", name, geometry);

    // Execute the SQL statement
    PGresult *res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "Insertion failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return FALSE;
    }

    // Clean up
    PQclear(res);

    return TRUE;
}


void init_my_predicates()
{
    YAP_UserCPredicate("db_connect", db_connect, 5);
    YAP_UserCPredicate("db_disconnect", db_disconnect, 1);
    YAP_UserCPredicate("db_query", db_query, 3);
    YAP_UserCPredicate("db_row", db_row, 3);
    YAP_UserCPredicate("db_difference", db_difference, 4);
    YAP_UserCPredicate("db_rotate", db_rotate, 4);
    YAP_UserCPredicate("db_translate", db_translate, 5);
    YAP_UserCPredicate("db_insert_solution", db_insert_solution, 5);
}