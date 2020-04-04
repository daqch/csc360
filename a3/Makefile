test : test01.o driver.o File.o
	gcc test01.o driver.o File.o -o test01

test01.o: ./apps/test01.c
	gcc -c ./apps/test01.c

driver.o: ./disk/driver.c ./disk/driver.h
	gcc -c ./disk/driver.c 

File.o: ./io/File.h ./io/File.c
	gcc -c ./io/File.c

