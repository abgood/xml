# 编译选项
CC = gcc
CFLAGS = -g -O0 -Wall

# 库
LIBXML = -lxml2
LIBMYSQL = `mysql_config --libs`

# 执行文件
TARGET = parse

# 所有的c文件生成o文件
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 把所有以.c结尾的文件放到列表里
SOURCES = $(wildcard *.c)
# 匹配sources列表里所有.c文件替换成.o文件放到列表里
OBJS = $(patsubst %.c,%.o,$(SOURCES))

# 生成执行文件
$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBXML) $(LIBMYSQL)
	chmod u+x $@

# 删除.o文件和执行文件
clean:
	rm $(OBJS) $(TARGET) -f
