################################################################
#
# aetg.cpp is the name of the source code.
#
# Type "make" or "make aetg" to compile the code
#
# Type "make clean" to remove the executable (and object files)
#
################################################################
CC=g++ -std=c++11 -o3
all: aetg 
aetg:
	$(CC) -o aetg aetg.cpp
clean:
	$(RM) aetg
	$(RM) output.txt
run:
	./aetg
