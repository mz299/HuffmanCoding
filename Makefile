# Min Zeng     cs610 3608 prp
all: henc3608 hdec3608

henc3608:henc3608.cpp Huffman3608.o MinHeap3608.o
	g++ -o henc3608 henc3608.cpp Huffman3608.o MinHeap3608.o

hdec3608:hdec3608.cpp Huffman3608.o MinHeap3608.o
	g++ -o hdec3608 hdec3608.cpp Huffman3608.o MinHeap3608.o

Huffman3608:Huffman3608.cpp
	g++ -c Huffman3608.cpp

MinHeap3608:MinHeap3608.cpp
	g++ -c MinHeap3608.cpp

clean:
	-rm -f henc3608 hdec3608 *.o
