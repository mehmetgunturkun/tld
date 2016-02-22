MV  := mv -f
RM  := rm -vf
CP := cp -vf
SED := sed
CC := gcc
CXX := g++
AR := ar

OPENCV_CFLAGS := `pkg-config opencv --cflags`
OPENCV_LIBS := `pkg-config opencv --libs` -L/usr/local/cuda/lib64
# LAPACK_LIBS := ./lapack_LINUX.a ./blas_LINUX.a ./libf2c.a

CPPFLAGS += -Iinclude $(OPENCV_CFLAGS)
CCFLAGS = -g -Wall --std=c99
# CXXFLAGS = -g -Wall --std=c++11
CXXFLAGS = -g -O2 -Wall --std=c++11
LDFLAGS  += $(OPENCV_LIBS)
ARFLAGS = ruv

lib2_sources := \
	src/common/Config.cc \
	src/core/Frame.cc

lib2_objects := $(patsubst %.cc,%.o,$(lib2_sources))
lib2_target  := libceng515++.a

test_target := target

tool2_sources := $(wildcard test/*.cc)
tool2_objects := $(patsubst %.cc,%.o,$(tool2_sources))
tool2_targets := $(patsubst %.cc,$(test_target)/%-app,$(notdir $(tool2_sources)))

all_targets  = $(lib2_target) $(tool2_targets)
all_objects  = $(lib2_objects) $(tool2_objects)
dependencies = $(patsubst %.o,%.d,$(all_objects))

.PHONY: all
all : $(all_targets)
#
# .PHONY: lapack_deps
# lapack_deps:
# 	make -C ext/CLAPACK-3.2.1/F2CLIBS/libf2c/
# 	make -C ext/CLAPACK-3.2.1/BLAS/SRC/
# 	make -C ext/CLAPACK-3.2.1/SRC/

$(lib_target) : $(lib_objects) Makefile
	$(AR) $(ARFLAGS) $@ $(lib_objects)
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

$(lib2_target) : $(lib2_objects) Makefile
	$(AR) $(ARFLAGS) $@ $(lib2_objects)
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

$(tool2_targets): $(test_target)/%-app: test/%.o $(lib2_target) Makefile
	$(CXX) $(CXXFLAGS) $< $(lib2_target) libjsoncpp.a $(lib_target) $(LDFLAGS) -o $@
	@echo ----------------------------------------
	@echo ... Built $@ ...
	@echo ----------------------------------------

%.o : %.c Makefile
	$(CC) $(CCFLAGS) -c $(CPPFLAGS) $< -o $@

%.o : %.cc Makefile
	$(CXX) $(CXXFLAGS) -c $(CPPFLAGS) $< -o $@

.PHONY: clean_lapack_deps
clean_lapack_deps:
	# make -C ext/CLAPACK-3.2.1/F2CLIBS/libf2c/ clean
	# make -C ext/CLAPACK-3.2.1/BLAS/SRC/ clean
	# make -C ext/CLAPACK-3.2.1/SRC/ clean

.PHONY: clean
clean : clean_lapack_deps
	@$(RM) $(all_targets)
	@find . -iname "*.o" -or -iname "*.d" -or -iname "*~" | xargs $(RM) -
	@echo ----------------------------------------
	@echo ... Clean-up completed ...
	@echo ----------------------------------------

ifneq "$(MAKECMDGOALS)" "clean"
  include $(dependencies)
endif

%.d : %.c
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@

%.d : %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -MM $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' > $@.tmp
	$(MV) $@.tmp $@
