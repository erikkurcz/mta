
GCC=g++
TARGET=mta
TARGET_DEBUG=mta_debug
PROTOBUFS_WITH_NYCT=src/protobufs/gtfs-realtime.pb.cc src/protobufs/nyct-subway.pb.cc
PROTOBUFS=src/protobufs/gtfs-realtime.pb.cc 
PARSER=src/file_parser.cpp
MAIN=src/main.cpp
FLAGS=-std=c++11 -lprotobuf

all:
	$(GCC) -o $(TARGET) $(PROTOBUFS) $(PARSER) $(MAIN) $(FLAGS)

debug:
	$(GCC) -o -g $(TARGET_DEBUG) $(PROTOBUFS) $(PARSER) $(MAIN) $(FLAGS)

clean:
	rm *.o $(TARGET) $(TARGET_DEBUG)

