# Author: Habib
# Date: 17/02/2024

# to prevent user from building unwanted version of the project that causes a problem, like  changing the pre-set decryption key, the user must provide the target name using "TARGET=<target name>"
# also release is the defaut build
# Cmds:
#
# 	+ Build project:
# 		make TARGET=writer 						: build release version for the writer (encryption mode + preview)
# 		make TARGET=reader 						: build release version for the reader (decryption mode)
# 		make TARGET=test   						: build and run all tests
#		make TARGET=writer release 				: build release version for the writer
#		make TARGET=writer debug debug_level=3 	: build debug version for the writer with debug level = 3 (debug levels are 0..3 and default is 0)
# 		Note: release and debug will work only with TARGET=writer and TARGET=reader (exp: make TARGET=reader debug, make TARGET=writer release)
#
# 	+ Clean project:
#		make clean 								: clean build folder (remove reader and writer, release and debug)
#		make clean TARGET=<target name> 		: only clean the target folder (writer, reader or test)


# |--Source
# 	|-- Makefile
# 	|--build			
#	|	|-- writer		
#	|	|	|-- release	
#	|	|	|-- debug		
#	|	|-- reader		
#	|	|	|-- release	
#	|	|	|-- debug
# 	|--test
# 		|--build	



#make sure to run only one goal
#MAKECMDGOALS := $(firstword $(MAKECMDGOALS))
# exit makefile with error if number of goals are greater than 1
ifeq ($(filter $(words $(MAKECMDGOALS)), 0 1),)
error_msg = "Only one option is allowed: release, debug or clean.(you passed($(words $(MAKECMDGOALS))): $(MAKECMDGOALS))"
$(error $(error_msg))
endif

$(info MAKECMDGOALS=> $(MAKECMDGOALS))

PROJECT_NAME =StoryTime
# Toolchain
CC = gcc -std=c11
# Flags
CFLAGS += -Wall -Wextra


# Target
# if clean cmd passed in make, so no need to check TARGET variable
ifeq ($(TARGET),test)
BIN_NAME = test
else ifeq ($(TARGET),writer)
BIN_NAME = $(PROJECT_NAME)_ForWriter
CFLAGS += -DBUILD_FOR_WRITER
else ifeq ($(TARGET),reader)
BIN_NAME = $(PROJECT_NAME)
else ifeq ($(filter $(MAKECMDGOALS),clean),)
# show error message only if make was for building the project
$(error "Must pass TARGET=writer or TARGET=reader or TARGET=test")
endif

# Source directories
ifeq ($(TARGET),test)
MAIN_FILE_DIR = test
#MAIN_FILE_NAME = $(notdir $(wildcard $(MAIN_FILE_DIR)/*.c))
else
MAIN_FILE_DIR = core
#MAIN_FILE_NAME = main.c
endif
$(info MAIN_FILE_DIR=> $(MAIN_FILE_DIR))

SOURCE_DIR = \
		$(MAIN_FILE_DIR) \
		core/encryption_rsa \
		core/common
$(info SOURCE_DIR=> $(SOURCE_DIR))
# Header directories
Header_DIR = \
		core/inc \
		core/encryption_rsa \
		core/common
$(info Header_DIR=> $(Header_DIR))

# Build directories
BUILD_DIR = build
RELEASE_DIR = release
DEBUG_DIR = debug

##################################################
#######  Building and Cleaning TARGET TEST #######
##################################################
# if TARGET is test, then we create the path to the build folder inside test for building or cleaning
ifeq ($(TARGET),test)
BIN_DIR = $(MAIN_FILE_DIR)/$(BUILD_DIR)
else

# if TARGET not test, then we have more option:
#		building the project:
#			+ if it's release, create path to build release version for that specific TARGET (writer or reader)
#			+ if it's debug (not release), create path to build debug version for that specific TARGET (writer or reader)
#			(note: in building state, TARGET is either writer or reader, it can't be something else, because we already did the check before)
#		cleaning the project, it doesn't matter, release or debug or neither was menstioned, this action it depends only on TARGET:
#			+ if TARGET is writer, clean writer folder
#			+ if TARGET is reader, clean reader folder
#			+ if TARGET wasn't mentioned or neither writer or reader, then clean the whole build folder


####################################################
#######  Building and Cleaning other TARGETs #######
####################################################

## CLEAN ###
ifneq ($(filter $(MAKECMDGOALS),clean),)
ifeq ($(filter $(TARGET), writer reader),$(TARGET))
# if TARGET is writer or reader or empty, create path for the specific TARGET (Note: empty TARGET still leads to the build directory which is the correct path in our case, so, no need to check if TARGET empty or not)
BIN_DIR = $(BUILD_DIR)/$(TARGET)
else
# if TARGER is something else, then abord the make cmd and show error
BIN_DIR = $(BUILD_DIR)/
$(error "Incorrect TARGET $(TARGET), set TARGET to writer, reader or test. if you want to remove the build directory use: make clean")
endif # end of if TARGET == writer or reader

## BUILD ###
else
# TARGET is either writer or reader but can't be something else, so, check if it's release to build release verison
ifneq ($(filter $(MAKECMDGOALS),debug), )
BIN_DIR = $(BUILD_DIR)/$(TARGET)/$(DEBUG_DIR)
# if it's not release version, then build debug version
else
BIN_DIR = $(BUILD_DIR)/$(TARGET)/$(RELEASE_DIR)
endif # end of if it's release
endif # end of if it's clean

endif #end of if TARGET == test
$(info BIN_DIR=> $(BIN_DIR))

BIN = $(BIN_DIR)/$(BIN_NAME)


# Source files
SOURCES := $(foreach dir, $(SOURCE_DIR), $(wildcard $(dir)/*.c))
$(info SOURCES=> $(SOURCES))
# Header files and directories
HEADERS := $(foreach dir, $(Header_DIR), $(wildcard $(dir)/*.h))
$(info HEADERS=> $(HEADERS))
#INCLUDES := $(dir $(addprefix -I,$(HEADERS)))
INCLUDES = $(addprefix -I, $(Header_DIR))
$(info INCLUDES=> $(INCLUDES))

# Objects files
OBJECT_NAMES = $(SOURCES:.c=.o)
$(info OBJECT_NAMES=> $(OBJECT_NAMES))
OBJECT_DIR = $(BIN_DIR)/obj
$(info OBJECT_DIR=> $(OBJECT_DIR))
OBJECTS = $(patsubst %, $(OBJECT_DIR)/%, $(OBJECT_NAMES))
$(info OBJECTS=> $(OBJECTS))


.PHONY: all release debug test clean

all: release
ifeq ($(TARGET),test)
	@echo "------ Run TEST ------"
	@$(BIN)
	@echo "------ TEST Finish ------"
endif

release: $(BIN)

ifeq ($(debug_level), )
debug: CFLAGS += -g -DDEBUG=0
else
debug: CFLAGS += -g -DDEBUG=$(debug_level)
endif
debug: $(BIN)


$(BIN): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	@#@mv $(BUILD_DIR)/$@ $(BIN_DIR)/$@
	@#@rm $(OBJECTS)

$(OBJECT_DIR)/%.o:  %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< $(INCLUDES) -o $@


clean:
	@echo "remove $(BIN_DIR)"
	@rm -rf $(BIN_DIR)
	

