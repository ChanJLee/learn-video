CC = g++
CCFLAG = -std=c++11
ARCHIVES = main

build: main.o output
	$(CC) $(CCFLAG) $< -o $(ARCHIVES)

main.o: main.cpp
	$(CC) $(CCFLAG) -c $^

output:
	mkdir -p output

audio:
	ffmpeg -f s16le -ar 44.1k -ac 2 -i output/left.pcm output/left.m4a
	ffmpeg -f s16le -ar 44.1k -ac 2 -i output/right.pcm output/right.m4a
	ffmpeg -f s16le -ar 44.1k -ac 2 -i output/volumn_half.pcm output/volumn_half.m4a

clean:
	-rm -rf *.o
	-rm -rf *.out
	-rm -rf $(ARCHIVES)
	-rm -rf output