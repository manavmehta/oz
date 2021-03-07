oz: main.o
	g++ -lreadline main.o -o oz

main.o: main.cpp
	g++ -c -lreadline main.cpp

# executable:
#	chmod +x oz