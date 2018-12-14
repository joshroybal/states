CC = g++
CFLAGS = -O2
LFLAGS = -s
PROG1 = bin/states
PROG2 = bin/convert
OBJS1 = obj/states.o obj/main.o
OBJS2 = obj/convert.o
HDRS = src/states.hpp
DIRS = bin obj

all: $(PROG1) $(PROG2)

obj/%.o: src/%.cpp $(HDRS)
	$(CC) -o $@ $(CFLAGS) -c $<

$(PROG1): $(OBJS1)
	$(CC) $(LFLAGS) -o $@ $^ -lcgicc

$(PROG2): $(OBJS2)
	$(CC) $(LFLAGS) -o $@ $^

install:
	sudo -u apache -g apache cp bin/states /srv/httpd/cgi-bin/states.cgi
	bin/convert
	sudo -u apache -g apache cp states.dat /srv/httpd/cgi-bin
	sudo -u apache -g apache cp src/states.html /srv/httpd/htdocs
	sudo -u apache -g apache cp -R bin/flags /srv/httpd/htdocs

.PHONY : clean
clean:
	$(RM) $(PROG1) $(PROG2) obj/* src/*~ src/.*~ src/*.swp

$(shell mkdir -p $(DIRS))
