all: wordcount.c
	gcc wordcount.c -o wordcount -lpthread
