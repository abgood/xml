#include "parse.h"

xmlNodePtr sys_user_node = NULL;
xmlNodePtr dept_node = NULL;
xmlNodePtr dept_user_node = NULL;
xmlNodePtr tmp1 = NULL;
xmlNodePtr tmp2 = NULL;
xmlNodePtr tmp3 = NULL;

char depart[1024] = {0};
char sql[2048] = {0};

void parse_dept(xmlChar *dept_id) {
    dept_node = tmp2;
    while (dept_node) {
        tmp3 = dept_node;
        xmlChar *tmp_dept_id = xmlGetProp(dept_node, BAD_CAST"DeptID");
        if (!xmlStrcmp(dept_id, tmp_dept_id)) {
            xmlChar *p_dept_id = xmlGetProp(dept_node, BAD_CAST"PDeptID");
            xmlChar *dept_name = xmlGetProp(dept_node, BAD_CAST"DeptName");
            parse_dept(p_dept_id);
            // printf("%s/", dept_name);
            strcat(depart, (char *)dept_name);
            strcat(depart, "/");
        }
        dept_node = tmp3;
        dept_node = dept_node->next;
    }
}

void parse_dept_user(xmlChar *user_id, xmlChar *user_name, xmlChar *user_name_en) {
    /* while 执行过后,dept_user_node指针被释放掉了,需要临时保存指针 */
    dept_user_node = tmp1;
    while (dept_user_node) {
        xmlChar *dept_user_id = xmlGetProp(dept_user_node, BAD_CAST"UserID");
        if (!xmlStrcmp(user_id, dept_user_id)) {
            xmlChar *dept_id = xmlGetProp(dept_user_node, BAD_CAST"DeptID");
            memset(depart, 0, 1024);
            printf("%s %s %s ", user_id, user_name_en, user_name);
            parse_dept(dept_id);
            depart[strlen(depart) - 1] = '\0';
            printf("%s", depart);
            printf("\n");
            sprintf(sql, "insert into T_ITFDP_COMPUTER_ASSIGN(usePeople_en,usePeople,department) values(\'%s\', \'%s\', \'%s\')", user_name_en, user_name, depart);
            // printf("%s\n", sql);
            // query_mysql(sql);
            return;
        }
        dept_user_node = dept_user_node->next;
    }
}

int main (int argc, char **argv) {
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;

    xmlKeepBlanksDefault(0);

    /* 打开xml文件 */
    if (!(doc = xmlReadFile(TMP, "utf-8", XML_PARSE_RECOVER))) {
        printf("open xml file error!\n");
        exit(1);
    }

    /* 获取根节点 */
    if (!(root_node = xmlDocGetRootElement(doc))) {
        printf("get root error!\n");
        exit(1);
    }

    /* 比较验证根节点 */
    if (xmlStrcmp(root_node->name, BAD_CAST"RTX2005OrLater_Exported_Data_9D373D5B-AA39-42e7-A819-BF5F7A8961FC")) {
        printf("root node error!\n");
        xmlFreeDoc(doc);
        exit(1);
    }

    /* 根节点下的子节点集 */
    xmlNodePtr node1 = root_node->xmlChildrenNode;
    /* 遍历子节点集 */
    while (node1) {
        if (!xmlStrcmp(node1->name, BAD_CAST"Database")) {
            xmlNodePtr node2 = node1->xmlChildrenNode;
            while (node2) {
                /* 保存所用元素节点指针 */
                if (!xmlStrcmp(node2->name, BAD_CAST"Sys_User")) {
                    sys_user_node = node2->xmlChildrenNode;
                }

                if (!xmlStrcmp(node2->name, BAD_CAST"RTX_Dept")) {
                    dept_node = node2->xmlChildrenNode;
                }

                if (!xmlStrcmp(node2->name, BAD_CAST"RTX_DeptUser")) {
                    dept_user_node = node2->xmlChildrenNode;
                }

                node2 = node2->next;
            }
        }
        node1 = node1->next;
    }

    /* 解析各user对应的group */
    /*
    while (sys_user_node) {
        xmlChar *user_id = xmlGetProp(sys_user_node, BAD_CAST"ID");
        xmlChar *user_name = xmlGetProp(sys_user_node, BAD_CAST"Name");
        printf("%s %s\n", user_id, user_name);
        sys_user_node = sys_user_node->next;
    }

    while (dept_node) {
        xmlChar *dept_id = xmlGetProp(dept_node, BAD_CAST"DeptID");
        xmlChar *pdept_id = xmlGetProp(dept_node, BAD_CAST"PDeptID");
        xmlChar *dept_name = xmlGetProp(dept_node, BAD_CAST"DeptName");
        printf("%s %s %s\n", dept_id, pdept_id, dept_name);
        dept_node = dept_node->next;
    }

    while (dept_user_node) {
        xmlChar *dept_id = xmlGetProp(dept_user_node, BAD_CAST"DeptID");
        xmlChar *user_id = xmlGetProp(dept_user_node, BAD_CAST"UserID");
        printf("%s %s\n", dept_id, user_id);
        dept_user_node = dept_user_node->next;
    }
    */

    tmp1 = dept_user_node;
    tmp2 = dept_node;

    while (sys_user_node) {
        xmlChar *user_id = xmlGetProp(sys_user_node, BAD_CAST"ID");
        xmlChar *user_name = xmlGetProp(sys_user_node, BAD_CAST"Name");
        xmlChar *user_name_en = xmlGetProp(sys_user_node, BAD_CAST"UserName");
        parse_dept_user(user_id, user_name, user_name_en);
        sys_user_node = sys_user_node->next;
    }

    return 0;
}
