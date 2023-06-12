#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h>
#include <Yap/YapInterface.h>

static int db_connect(void){
    YAP_Term arg_host= YAP_ARG1;
    YAP_Term arg_user= YAP_ARG2;
    YAP_Term arg_passwd= YAP_ARG3;
    YAP_Term arg_database= YAP_ARG4;
    YAP_Term arg_conn= YAP_ARG5;


    char *host= YAP_AtomName(YAP_AtomOfTerm(arg_host));
    char *user= YAP_AtomName(YAP_AtomOfTerm(arg_user));
    char *passwd= YAP_AtomName(YAP_AtomOfTerm(arg_passwd));
    char *database= YAP_AtomName(YAP_AtomOfTerm(arg_database));


    char conninfo[1024];
    snprintf(conninfo, sizeof(conninfo), "user=%s password=%s dbname=%s host=%s",
             user, passwd, database, host);
    PGconn* conn = PQconnectdb(conninfo);


    if (PQstatus(conn) == CONNECTION_BAD) {
        
        fprintf(stderr, "Connection to database failed: %s\n",
            PQerrorMessage(conn));
        do_exit(conn);
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