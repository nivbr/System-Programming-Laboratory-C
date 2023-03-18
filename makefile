assembler: assembler.o ./src/inputCollection.o ./src/hash.o ./src/preprocessor.o ./src/symbolChart.o ./src/pass.o ./src/util.o ./src/decodeLine.o
	gcc -g -ansi assembler.o ./src/inputCollection.o ./src/hash.o ./src/preprocessor.o ./src/symbolChart.o ./src/pass.o ./src/util.o ./src/decodeLine.o -o assembler

assembler.o: assembler.c ./src/data.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o

./src/inputCollection.o: ./src/inputCollection.c ./src/data.h
	gcc -c -Wall -ansi -pedantic ./src/inputCollection.c -o ./src/inputCollection.o 

./src/preprocessor.o: ./src/preprocessor.c ./src/data.h ./src/hash.h
	gcc -c -Wall -ansi -pedantic ./src/preprocessor.c -o ./src/preprocessor.o 

./src/hash.o: ./src/hash.c ./src/hash.h	./src/data.h
	gcc -c -Wall -ansi -pedantic ./src/hash.c -o ./src/hash.o 

./src/symbolChart.o: ./src/symbolChart.c ./src/symbolChart.h ./src/data.h
	gcc -c -Wall -ansi -pedantic ./src/symbolChart.c -o ./src/symbolChart.o 

./src/pass.o: ./src/pass.c ./src/symbolChart.h ./src/pass.h ./src/util.h ./src/decodeLine.h
	gcc -c -Wall -ansi -pedantic ./src/pass.c -o ./src/pass.o 

./src/util.o: ./src/util.c ./src/symbolChart.h ./src/util.h
	gcc -c -Wall -ansi -pedantic ./src/util.c -o ./src/util.o 

./src/decodeLine.o: ./src/decodeLine.c ./src/util.h ./src/decodeLine.h
	gcc -c -Wall -ansi -pedantic ./src/decodeLine.c -o ./src/decodeLine.o 