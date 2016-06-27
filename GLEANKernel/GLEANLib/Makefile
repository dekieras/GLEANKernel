# Some top-level variables

LIBNAME=libglean

ROOT_DIR = ../..
SOURCE_DIR = .
LIB_DIR = $(ROOT_DIR)/../lib


# Determining the relevant source files
SOURCES = $(notdir $(wildcard $(SOURCE_DIR)/*.cpp)) \
$(notdir $(wildcard Architecture_Classes/*.cpp)) \
$(notdir $(wildcard Framework_Classes/*.cpp)) \
$(notdir $(wildcard GOMS_Model_Classes/*.cpp)) \
$(notdir $(wildcard Glean_Utilities/*.cpp)) \
$(notdir $(wildcard Model-View_classes/*.cpp)) \
$(notdir $(wildcard PCCTS_Modules/*.cpp)) \
$(notdir $(wildcard Parser_Files/*.cpp)) \
$(notdir $(wildcard Recorder_Classes/*.cpp)) \
$(notdir $(wildcard Utility_Classes/*.cpp))


# This helps keep some of the paths simple
VPATH = $(SOURCE_DIR):$(LIB_DIR):\
Architecture_Classes:\
Framework_Classes:\
GOMS_Model_Classes:\
Glean_Utilities:\
Model-View_classes:\
PCCTS_Modules:\
Parser_Files:\
Recorder_Classes:\
Utility_Classes:\

# Assembling the include arguments for the agent app
INC_ARGS = -I$(ROOT_DIR)/include \
-I. \
-IArchitecture_Classes \
-IFramework_Classes \
-IGOMS_Model_Classes \
-IGlean_Utilities \
-IModel-View_classes \
-IPCCTS_Modules \
-IParser_Files \
-IRecorder_Classes \
-IUtility_Classes

CXXFLAGS = -Wall -D_REENTRANT $(INC_ARGS) $(EXTRA_CFLAGS)

.PHONY: $(LIBNAME) clean 

###################################################
#
# TARGETS
#
###################################################
all: $(LIBNAME)

# For all the subdirectories with spaces in their names
# make a symlink replacing spaces with underscores.
# There has to be a better way to do this, but...
dirlinks:
	ln -sf Architecture\ Classes Architecture_Classes
	ln -sf Framework\ Classes Framework_Classes
	ln -sf GOMS\ Model\ Classes GOMS_Model_Classes
	ln -sf Glean\ Utilities Glean_Utilities
	ln -sf Model-View\ classes Model-View_classes
	ln -sf PCCTS\ Modules PCCTS_Modules
	ln -sf Parser\ Files Parser_Files
	ln -sf Recorder\ Classes Recorder_Classes
	ln -sf Utility\ Classes Utility_Classes

$(LIBNAME): $(LIB_DIR)/$(LIBNAME).a

$(LIB_DIR)/$(LIBNAME).a: $(SOURCES:.cpp=.o)
	ar r $@ $(SOURCES:.cpp=.o)
	ranlib $@

clean: 
	rm -f *.o *.d
	rm -f $(LIB_DIR)/$(LIBNAME).a
	rm -f Architecture_Classes
	rm -f Framework_Classes
	rm -f GOMS_Model_Classes
	rm -f Glean_Utilities
	rm -f Model-View_classes
	rm -f PCCTS_Modules
	rm -f Parser_Files
	rm -f Recorder_Classes
	rm -f Utility_Classes


ifneq ($(MAKECMDGOALS),clean)
# Including these automatically generated dependencies
include $(SOURCES:.cpp=.d)

# This rule and the depscript file in the root directory create the .d
# dependencies files automatically
%.d: %.cpp
	$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $< | sed -f $(ROOT_DIR)/depscript > $@'
	#$(SHELL) -ec '$(CXX) -MM $(CXXFLAGS) $< | sed -f $(ROOT_DIR)/depscript - > $@'
endif

