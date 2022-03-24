
GCC=g++
TARGET=mta
TARGET_DEBUG=mta_debug
PROTOBUFS=src/protobufs/gtfs-realtime.pb.cc src/protobufs/nyct-subway.pb.cc
DEPS=src/static_data_parser.cpp src/file_parser.cpp
MAIN=src/main.cpp
FLAGS=-std=c++11 -lprotobuf

all:
	$(GCC) -o $(TARGET) $(PROTOBUFS) $(DEPS) $(MAIN) $(FLAGS)

debug:
	$(GCC) -g -o $(TARGET_DEBUG) $(PROTOBUFS) $(DEPS) $(MAIN) $(FLAGS)

clean:
	rm *.o $(TARGET) $(TARGET_DEBUG)

