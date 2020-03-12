SRC_DIR = ./src/
BIN_DIR = ./bin/
OBJ_DIR = ./obj/

OBJ_FLAGS = -c
CPP_PLAGS = -Wall -std=c++11 -g 
RELEASE_FLAGS = -std=c++11 -O1 -O3 -O2
LDFLAGS =  -lpthread -lSDL2 -lSDL2main -lSDL2_image -lpoppler-cpp -lavcodec -lavformat -lavutil -lswscale

SRC_FILES = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
FILES = $(notdir $(SRC_FILES:.cpp=))
H_FILES = $(addprefix $(SRC_DIR),$(notdir $(SRC_FILES:.cpp=.h)))
OBJ_FILES = $(addprefix $(OBJ_DIR),$(notdir $(SRC_FILES:.cpp=.o)))
OUTPUT = output

all: $(SRC_DIR)main.cpp $(FILES)
	g++ $(CPP_PLAGS) $(OBJ_FILES) $(SRC_DIR)main.cpp -o $(BIN_DIR)$(OUTPUT) $(LDFLAGS)

release: $(SRC_DIR)main.cpp $(FILES)
	g++ $(RELEASE_FLAGS) $(OBJ_FILES) $(SRC_DIR)main.cpp -o $(BIN_DIR)$(OUTPUT) $(LDFLAGS)

$(FILES): $(H_FILES)
	g++ $(CPP_PLAGS) $(OBJ_FLAGS) $(SRC_DIR)$@.cpp -o $(OBJ_DIR)$@.o

run:
	$(BIN_DIR)$(OUTPUT)

gdb:
	gdb $(BIN_DIR)$(OUTPUT)



create_workspace:
	mkdir $(SRC_DIR) $(OBJ_DIR) $(BIN_DIR)
	touch $(SRC_DIR)main.cpp $(OBJ_DIR).placeholder $(BIN_DIR).placeholder ./README.md
	echo "#include <iostream>\n\n\nusing namespace std;\n\n\n\nint main(int argc, char **argv){\n\n\n\treturn 0;\n\n}" >> $(SRC_DIR)main.cpp


remove_workspace:
	rm -R -vf $(OBJ_DIR) $(SRC_DIR) $(BIN_DIR)

create_class:
	touch $(SRC_DIR)$(name).h $(SRC_DIR)$(name).cpp
	echo "#ifndef "$(name)"_H\n#define "$(name)"_H\n\n\nclass "$(name)"{\npublic:\n\nprivate:\n};\n#endif" >> $(SRC_DIR)$(name).h
	echo '#include "$(name).h"\n\n' >> $(SRC_DIR)$(name).cpp

delete_class:
	rm -vf $(SRC_DIR)$(name).h $(SRC_DIR)$(name).cpp

clean:
		rm -vf $(OBJ_DIR)*
		rm -vf $(BIN_DIR)*
		touch $(OBJ_DIR).placeholder $(BIN_DIR).placeholder
