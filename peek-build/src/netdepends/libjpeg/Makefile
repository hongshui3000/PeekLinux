VARIANT_DEFINE :=

INCLUDE  = $(SRCDIR)/netsurf/emoport
INCLUDE += $(SRCDIR)/emobiix
INCLUDE += $(SRCDIR)/netdepends/libnsfb/include

SRC  = jaricom.c
SRC += jcapimin.c
SRC += jcapistd.c
SRC += jcarith.c
SRC += jccoefct.c
SRC += jccolor.c
SRC += jcdctmgr.c 
SRC += jchuff.c
SRC += jcinit.c
SRC += jcmainct.c
SRC += jcmarker.c
SRC += jcmaster.c
SRC += jcomapi.c
SRC += jmemansi.c
SRC += jcparam.c
SRC += jcprepct.c 
SRC += jcsample.c
SRC += jctrans.c
SRC += jdapimin.c
SRC += jdapistd.c
SRC += jdarith.c
SRC += jdatadst.c
SRC += jdatasrc.c
SRC += jdcoefct.c
SRC += jdcolor.c
SRC += jddctmgr.c
SRC += jdhuff.c
SRC += jdinput.c
SRC += jdmainct.c
SRC += jdmarker.c
SRC += jdmaster.c 
SRC += jdmerge.c
SRC += jdpostct.c
SRC += jdsample.c
SRC += jdtrans.c
SRC += jerror.c
SRC += jfdctflt.c
SRC += jfdctfst.c
SRC += jfdctint.c
SRC += jidctflt.c
SRC += jidctfst.c
SRC += jidctint.c
SRC += jquant1.c
SRC += jquant2.c
SRC += jutils.c
SRC += jmemmgr.c


DEPS = $(patsubst %.o,%.d,$(OBJ))

TARGET = $(CURDIR)/libjpeg.a

all: $(TARGET)
OBJ  = $(addprefix $(CURDIR)/,$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC))))
DEPS = $(patsubst %.o,%.d,$(OBJ))

$(TARGET): $(OBJ)
	$(subst /cygdrive/c,C:,$(AR) $(ARFLAGS) $@ $?)

clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET)
	$(RM) $(DEPS)
	$(RM) $(patsubst %.o,%.pp,$(OBJ))
	$(RM) $(patsubst %.o,%.pp_,$(OBJ))

include $(BASEDIR)/Makefile.rules

