CC=gcc
# CXX=g++

editor: editor.c
	$(CC) editor.c -g -o editor -Wall -Wextra -pedantic -std=c99
