
GCC=g++
TARGET=mta
TARGET_DEBUG=mta_debug
PROTOBUFS=src/protobufs/gtfs-realtime.pb.cc src/protobufs/nyct-subway.pb.cc
DEPS=src/basic_structures.cpp src/time_parser.cpp src/utils.cpp src/trip_map.cpp src/static_data_parser.cpp src/file_parser.cpp src/query_interface.cpp src/retriever.cpp
MAIN=src/main.cpp
FLAGS=-std=c++17 -lprotobuf -llog4cxx -lcurl

all:
	$(GCC) -o $(TARGET) $(PROTOBUFS) $(DEPS) $(MAIN) $(FLAGS)

debug:
	$(GCC) -g -o $(TARGET_DEBUG) $(PROTOBUFS) $(DEPS) $(MAIN) $(FLAGS)

clean:
	rm *.o $(TARGET) $(TARGET_DEBUG)

