
#Get Makefile Current Directory
CURDIR =$(shell pwd)

CCFLAGS = -g
CXX =g++
CC  =gcc

CPPFLAGS =  
OBJ_DIR = $(CURDIR)/obj


#CPP Files Directory
vpath %.cpp ./   

#Head Files Directory    
vpath %.h ./


	 
SRC_CPP := $(wildcard *.cpp) 
	 
SRC_OBJ := $(patsubst %.cpp,%.o,$(SRC_CPP))


#Load dynamic Library
LIBDIR = 
#Load Head Files
INCDIR =



.PHONY :all
all : show edit run 

.PHONY :show
show:
	@echo "============SRC_CPP==============" 
	@echo $(SRC_CPP)
	@echo "=================================" 		
	@echo "============SRC_OBJ==============" 
	@echo $(SRC_OBJ)
	@echo "=================================" 	

.PHONY :edit
edit: $(SRC_OBJ)
$(SRC_OBJ):%.o :%.cpp
	$(CXX) $(INCDIR) -c $(CCFLAGS) $(CPPFLAGS) $< -o $@

   
.PHONY :run
run: $(SRC_OBJ)
	$(CXX)  $(SRC_OBJ) $(LIBDIR) -o autoGen
	
.PHONY :clean
clean:
	rm $(SRC_OBJ)
