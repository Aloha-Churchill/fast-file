HEADERS = file_function.h

default = lib_file_function.so

lib_file_function.so: file_function.o
	gcc file_function.o -shared -lgomp -o lib_file_function.so

file_function.o: file_function.c $(HEADERS)
	gcc -c -fPIC -fopenmp file_function.c -o file_function.o

clean:
	-rm -f file_function.o
	-rm -f lib_file_function.so
	-rm -f train_neg_compiled.txt
	-rm -f train_pos_compiled.txt