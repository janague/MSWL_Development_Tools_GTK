CC = gcc # Compiler to use
OPTIONS = -O2 -g -Wall # -g for debug, -O2 for optimise and -Wall additional messages
INCLUDES = `pkg-config --cflags gtk+-3.0`  # Directory for header file
LIBS = `pkg-config --libs gtk+-3.0` 

EXEC = gImageViewer.out # The name of the executable file
OBJS = gImageViewer.o # List of objects to be build
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

#########################################################################
# Rules make
#########################################################################
# all is the default rule
all: $(EXEC)
	 
$(EXEC): $(OBJ_DIR)/$(OBJS)
	    @echo "Building ..." 
		-mkdir $(BIN_DIR)
		$(CC) $(OPTIONS) $(INCLUDES) $(OBJ_DIR)/$(OBJS) -o $(BIN_DIR)/$@ $(LIBS)
			 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c  # % pattern wildcard matching
		-mkdir $(OBJ_DIR)
	    $(CC) $(OPTIONS) -c $(SRC_DIR)/$*.c $(INCLUDES) -o $@
list:
	    @echo $(shell ls) # To print output of command 'ls'
		 
clean:
	    @echo "Cleaning up.."
		-rm -rf $(OBJ_DIR)/*.o # - prefix for ignoring errors and continue execution
		-rm -rf $(BIN_DIR)/$(EXEC)
		-rm -d $(OBJ_DIR)
		-rm -d $(BIN_DIR)
