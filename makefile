# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
# define the C compiler to use
CC = gcc
# define any compile-time flags
CFLAGS = -Wall -g
# define any directories containing header files other than /usr/include
#INCLUDES = -I/home/newhall/include  -I../include
INCLUDES = `pkg-config --cflags --libs gtk+-3.0`
# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib
LFLAGS = -export-dynamic -rdynamic
#LFLAGS2 = `-I/usr/include/mysql -I/usr/include/mysql/mariadb`
# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -glib-2.0
LIBS =-L/usr/lib -lmariadb
# define the C source files
#SRCS = emitter.c error.c init.c lexer.c main.c symbol.c parser.c
MAIN = mysql_query

SRCS = $(MAIN).c

OBJS = $(SRCS:.c=.o)

INCLUDE_H = mysql_query.h

ICL = $(INCLUDE_H)
# define the executable file 
#MAIN = mycc
#MAIN = mysql_query


.PHONY: depend clean

all:    $(MAIN)
	@echo COMPILED compiled

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LFLAGS2) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

#$(MAIN): mysql_query.h
$(MAIN): $(ICL)

clean:
	$(RM) *.o 
#	*~ $(MAIN)

cleaner:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^
