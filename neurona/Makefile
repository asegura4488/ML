DIR = .
CXX = g++
CFLAGS = -std=c++11  

SRCDIR = $(DIR)/src
INCDIR = -I$(DIR)/inc
OBJDIR = $(DIR)/obj
EXE = Neuron

#vpath %.h $(INCDIR) 

#OBJECTS:=$(wildcard $(OBJDIR)/*.o)
OBJECT = $(OBJDIR)/main.o $(OBJDIR)/neuron.o  

all: $(EXE)

$(EXE): $(OBJECT)
	$(CXX) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp  
	$(CXX) $(CFLAGS) $(INCDIR) -c $< -o $@

$(OBJDIR)/%.o: $(INCDIR)/%.h
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@rm $(EXE) $(OBJDIR)/*

