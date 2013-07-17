#include "parse.h"

/* 返回数据库连接状态 */
int query_mysql(const char *sql) {
    MYSQL mysql, *conn;
    const char *ip = "";
    const char *db_pawd = "";
    int port = 0;

    // 初始mysql结构
    mysql_init(&mysql);

    // 连接mysql
    if ((conn = mysql_real_connect(&mysql, ip, "root", db_pawd, "ITFP_system", port, NULL, 0)) == NULL) {
        if (mysql_error(&mysql))
            fprintf(stderr, "connection error %d : %s\n", mysql_errno(&mysql), mysql_error(&mysql));
        printf("Fail to connect mysql, ip:%s\tport:%d\n", ip, port);
        return 1;
    }

    /* 插入数据 */
    if (mysql_query(conn, sql) != 0) {
        if (mysql_error(conn))
            fprintf(stderr, "connection error %d : %s\n", mysql_errno(conn), mysql_error(conn));
        printf("fail to insert data!\n");
    }
    else
        printf("Insert data into DB success!\n");

    mysql_close(conn);
    return 0;
}
