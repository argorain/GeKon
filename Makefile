# COMPILERS
CC = clang
CXX = clang
AS = as
LD = clang
RM = rm
DOXY = doxygen

# BUILD FLAGS
#MCU = atmega168p
#ARCH_FLAGS	= -mmcu=$(MCU) -DF_CPU=8000000UL

CFLAGS		+= -O3 -std=c11 -funroll-loops
CFLAGS		+= -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS		+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS		+= -fno-common -ffunction-sections -fdata-sections

CXXFLAGS	+= -O0  -std=c++14
CXXFLAGS	+= -Wextra -Wredundant-decls
CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

CPPFLAGS	+= -Wall -Wundef -lstdc++
CPPFLAGS	+= -Wextra -g
#CPPFLAGS    += -Werror 
CPPFLAGS	+= -Wredundant-decls 
CPPFLAGS	+= -fno-common -ffunction-sections -fdata-sections

ASFLAGS     += -MD -Wall

LDFLAGS		+=  

# BUILD RULES
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).c

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).cpp

%.o: %.s
	$(AS) $(ASFLAGS) $(ARCH_FLAGS) -o $(*).o -c $(*).s

# PROJECT DEFINES
BINARY = gekon
SOURCE =  main.cpp
OBJS = main.o
HEADERS = 

all: $(BINARY)

$(OBJS): $(SOURCE)

$(BINARY): $(OBJS) $(HEADERS)
	$(LD) $(ARCH_FLAGS)  $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $(BINARY)

clean:
	$(RM) -f *.o *.d *.elf *.bin *.hex *.srec *.list *.map
	$(RM) $(BINARY)
	$(RM) -rf doc/html

rebuild: clean all

run: 
	./$(BINARY)

#docs:
#	$(DOXY) ./doc/Doxyfile
