listen_socket_connection.o: listen_socket_connection.h listen_socket_connection.cpp
	g++ -c listen_socket_connection.cpp -o listen_socket_connection.o

storage.o: storage.h storage.cpp string_utils.o
	g++ -c storage.cpp -o storage.o

string_utils.o: string_utils.h string_utils.cpp
	g++ -c string_utils.cpp -o string_utils.o

ping.o: ping.h ping.cpp
	g++ -c ping.cpp -o ping.o

parser.o: parser.h parser.cpp
	g++ -c parser.cpp -o parser.o

test: main.cpp listen_socket_connection.o storage.o string_utils.o ping.o
	g++ main.cpp \
		listen_socket_connection.o \
		storage.o \
		string_utils.o \
		ping.o \
		-o test_app

clean:
	-rm *.o
	-rm test_app