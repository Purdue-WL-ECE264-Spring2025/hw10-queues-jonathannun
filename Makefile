CFLAGS = -std=c99 -g -Wall -Wshadow -Wvla -Werror -Wunreachable-code
OBJS = main.o linked_list.o tile_game.o queue.o
HEADERS = linked_list.h tile_game.h
APP = hw10

.PHONY: clean

$(APP): $(OBJS)
	$(CC) $(CFLAGS) *.o -o $(APP)

clean:
	/bin/rm -rf *.o
	/bin/rm -rf $(APP)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

test1: hw10
	./hw10 testcases/1.txt > out1.txt

test2: hw10
	./hw10 testcases/2.txt > out2.txt

test3: hw10
	./hw10 testcases/3.txt > out3.txt


#Test memory leak
leak: hw10
	valgrind ./hw10 testcases/3.txt > leakout1.txt
