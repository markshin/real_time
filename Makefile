# TODO: use an automatic tool to generate Makefile?
# app.c conf.c pals_conf_helper.c pals_main.c pals_port.c
# pals_read_msg.c pals_recv.c pals_send.c pals_socket.c
# pals_time.c pals_timer.c

all: main.exec
APP = applications/mrate0.c
CONF = applications/conf_mrate.c
#-I.

main.exec: pals_main.o pals_send.o pals_recv.o conf.o app.o pals_socket.o pals_time.o pals_timer.o pals_read_msg.o pals_conf_helper.o pals_port.o pals_msg.o
	gcc -o $@ $^

pals_main.o: pals_main.c conf.h pals_task.h pals_timer.h pals_read_msg.h app.h
	gcc -c -o $@ $<

pals_send.o: pals_send.c app.h pals_task.h pals_port.h
	gcc -c -o $@ $<

pals_recv.o: pals_recv.c app.h pals_task.h
	gcc -c -o $@ $<

pals_port.o: pals_port.c pals_port.h pals_base.h pals_msg.h
	gcc -c -o $@ $<

pals_read_msg.o: pals_read_msg.c pals_read_msg.h
	gcc -c -o $@ $<

pals_conf_helper.o: pals_conf_helper.c conf.h pals_time.h
	gcc -c -o $@ $<

conf.o: $(CONF) conf.h
	gcc -c -o $@ $<

app.o: $(APP) app.h
	gcc -c -o $@ $<

pals_msg.o: pals_msg.c pals_msg.h
	gcc -c -o $@ $<

pals_socket.o: pals_socket.c pals_socket.h
	gcc -c -o $@ $<

pals_time.o: pals_time.c pals_time.h
	gcc -c -o $@ $<

pals_timer.o: pals_timer.c pals_timer.h pals_time.h
	gcc -c -o $@ $<

clean:
	rm *.o main.exec
