histparser: seqparse.o histparse.o
	g++ -std=c++17  histmain.cpp seqparse.o histparse.o -O1 -l curl -o ../sheet/histparser

currparser: currency.o seqparse.o
	g++ -std=c++17 currmain.cpp seqparse.o currency.o -O1 -l curl -o ../sheet/currparser

seqparser: seqparse.o
	g++ -std=c++17 seqmain.cpp seqparse.o -O1 -l curl -o ../sheet/seqparser


currency.o:
	g++ -std=c++17 -c currency/currency.cpp -O1 -o currency.o

histparse.o: histprices/histparse.cpp histprices/histparse.hpp
	g++ -std=c++17 -c histprices/histparse.cpp -O1 -o histparse.o

seqparse.o: sequencial/seqparse.cpp sequencial/seqparse.hpp
	g++ -std=c++17 -c sequencial/seqparse.cpp -O1 -o seqparse.o


.PHONY: clear
# clear all .o file
clear:
	rm -f histparse.o
	rm -f seqparse.o
	rm -f currency.o


.PHONY: clean
# clean all output file
clean:
	rm -rf ../sheet/histparser
	rm -rf ../sheet/seqparser
	rm -rf ../sheet/currparser
