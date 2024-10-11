listen_socket_connection.o: listen_socket_connection.h listen_socket_connection.cpp
	g++ -c listen_socket_connection.cpp -o listen_socket_connection.o

storage.o: storage.h storage.cpp string_utils.o
	g++ -c storage.cpp -o storage.o

string_utils.o: string_utils.h string_utils.cpp
	g++ -c string_utils.cpp -o string_utils.o

test: main.cpp listen_socket_connection.o storage.o string_utils.o
	g++ main.cpp \
		listen_socket_connection.o \
		storage.o \
		string_utils.o \
		-o test_app

clean:
	-rm *.o
	-rm test_app