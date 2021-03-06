
srcTop = ../..
srcdir = .

include $(srcTop)/project/ToolchainSelect.mk

AR=ar
RM=rm -f
MKDIR=mkdir
MOC=moc
RCC=rcc

ITKIncludeRoot=/usr/local/include/InsightToolkit

ITKIncludes=	-I$(ITKIncludeRoot)\
				-I$(ITKIncludeRoot)/Common\
				-I$(ITKIncludeRoot)/Algorithms\
				-I$(ITKIncludeRoot)/BasicFilters\
				-I$(ITKIncludeRoot)/IO\
				-I$(ITKIncludeRoot)/Utilities\
				-I$(ITKIncludeRoot)/Utilities/vxl/vcl\
				-I$(ITKIncludeRoot)/Utilities/vxl/core\
				-I$(ITKIncludeRoot)/gdcm/src
				
ITKLIBS= 	$(ITKLibsRoot)/libITKNumerics.a\
			$(ITKLibsRoot)/libITKNrrdIO.a\
			$(ITKLibsRoot)/libITKSpatialObject.a\
			$(ITKLibsRoot)/libITKCommon.a\
			$(ITKLibsRoot)/libitkvnl_inst.a\
			$(ITKLibsRoot)/libitkvnl_algo.a\
			$(ITKLibsRoot)/libitkvnl.a\
			$(ITKLibsRoot)/libitkv3p_netlib.a\
			$(ITKLibsRoot)/libitkvcl.a\
			$(ITKLibsRoot)/libitksys.a\
			$(ITKLibsRoot)/libITKMetaIO.a\
			$(ITKLibsRoot)/libITKEXPAT.a\
			$(ITKLibsRoot)/libITKniftiio.a\
			$(ITKLibsRoot)/libITKznz.a


########################################
#Name of this project part
NAME=		ITKPok
#Names of target sources
TARGETS=	main \
		mainWindow \
		SettingsBox

MOCINPUT=	mainWindow.h \
		SettingsBox.h

RCCINPUT=	mainWindow.qrc

M4DLIBS=	$(srcTop)/lib/libm4dWidgets.a\
		$(srcTop)/lib/libvtkIntegration.a\
		$(srcTop)/lib/libbackendForDICOM.a\
		$(srcTop)/lib/libImaging.a\
		$(srcTop)/lib/libcommon.a\
		$(srcTop)/lib/libremoteCompCommons.a\
		$(srcTop)/lib/libremoteClient.a

QTLIBS=		-lQtCore\
		-lQtGui\
		-lQtOpenGL
VTKLIBS=	-lvtkCommon\
		-lvtkDICOMParser\
		-lvtkexoIIc\
		-lvtkFiltering\
		-lvtkftgl\
		-lvtkGenericFiltering\
		-lvtkGraphics\
		-lvtkHybrid\
		-lvtkImaging\
		-lvtkIO\
		-lvtkNetCDF\
		-lvtkRendering\
		-lvtksys\
		-lvtkVolumeRendering\
		-lvtkWidgets\
		-lQVTK\
		-lwrap

		#-lvtkexpat\
		#-lvtkfreetype\
		#-lvtkjpeg\
		#-lvtkpng\
		#-lvtktiff\
		#-lvtkzlib

DCMTKLIBS=	-ldcmnet -ldcmdata -lofstd

LIBS=		$(M4DLIBS)\
		$(QTLIBS)\
		$(VTKLIBS)\
		$(DCMTKLIBS)\
		$(ITKLIBS)\
		-lboost_thread \
		-lboost_filesystem \
		-lGLU
########################################

#Name of target
OUTPUTNAME=	$(NAME)
OUTPUTDIR=	$(srcTop)/executables
OUTPUT=		$(OUTPUTDIR)/$(OUTPUTNAME)
TMP_DIR=	$(srcTop)/tmp/$(NAME)

CXXDEBUG_OPTIONS= -ggdb 

CXXFLAGS= -Wall -Wno-deprecated $(CXXDEBUG_OPTIONS)
#Dependecy file creation parameters
DEP_FILE=	$(TMP_DIR)/depend
CDEPFLAGS=	-MM 

LDFLAGS=
ARFLAGS=	-r
LIBDIRS=	-L$(srcTop)/lib\
		-L/usr/lib/qt4
		
QTINCLUDES = \
		-I/usr/include/qt4\
		-I/usr/include/qt4/Qt\
		-I/usr/include/qt4/QtCore\
		-I/usr/include/qt4/QtGui\
		-I/usr/include/Qt\
		-I/usr/include/QtGui\
		-I/usr/include/QtCore\
		
INCLUDES=	-I$(srcTop) \
		-I$(srcdir)\
		-I$(srcTop)/include\
		-I/usr/include/vtk-5.0\
		$(ITKIncludes)\
		$(QTINCLUDES)\
		-I/usr/local/include/boost-1_38


MOCOUTPUT=      $(addprefix $(TMP_DIR)/moc_, $(MOCINPUT:.h=.cpp) )

DEBUG_DEFS=	-DDEBUG_LEVEL=10\
		-DDEBUG_ADITIONAL_INFO

DEFS=		$(DEBUG_DEFS)


SRC_FILES=	$(addsuffix .cpp, $(TARGETS) ) 
OBJ_FILES=	$(addprefix $(TMP_DIR)/,   $(addprefix rcc_, $(RCCINPUT:.qrc=.o ) ) $(addprefix moc_, $(MOCINPUT:.h=.o) ) $(addsuffix .o, $(TARGETS) ) )


.PHONY: all
all:		tmpdir dep $(OUTPUT)	

.PHONY: tmpdir
tmpdir:		
		$(MKDIR) $(TMP_DIR) 2>/dev/null || true

.PHONY: build
build:		cleanall all

.PHONY: dep
dep:		
		$(CXX) $(INCLUDES) $(CDEPFLAGS) $(SRC_FILES) | \
		sed 's/\(.*\)\.o:/$$(TMP_DIR)\/\1\.o:/' > $(DEP_FILE)

$(TMP_DIR)/moc_%.cpp:   %.h
		$(MOC) $< -o $@

$(TMP_DIR)/rcc_%.cpp:   $(RCCINPUT)
		$(RCC) -name $(*F) $< -o $@

$(TMP_DIR)/%.o:	%.cpp	
		$(CXX) $(CXXFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

$(TMP_DIR)/%.o:	$(TMP_DIR)/%.cpp
		$(CXX) $(CXXFLAGS) $(DEFS) $(INCLUDES) -c $< -o $@

-include $(DEP_FILE)
		
$(OUTPUT):	$(OBJ_FILES) $(M4DLIBS)
		$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(LIBDIRS) $(LIBS) -o $(OUTPUT) 

.PHONY: clean
clean:
		$(RM) $(OBJ_FILES) $(MOCOUTPUT) $(addprefix $(TMP_DIR)/rcc_, $(RCCINPUT:.qrc=.cpp ) ) $(DEP_FILE)
		$(RM) $(OUTPUT)
