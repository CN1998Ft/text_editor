CC=gcc
CXX=g++

editor: editor.cpp
	$(CXX) editor.cpp -o editor -Wall -Wextra -pedantic -std=c++17
