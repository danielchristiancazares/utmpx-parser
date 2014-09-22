#
# Makefile template for CSE 30 -- PA4
#

#
# You need to change these next three lines for each assignment
#

HEADERS		= mywho.h strings.h

C_SRCS		= mywho.c buildUtmpInfoTable.c displayUtmpInfo.c \
		  pa4time.c

ASM_SRCS	= calcIdleTime.s

C_OBJS		= mywho.o buildUtmpInfoTable.o displayUtmpInfo.o \
		  pa4time.o

ASM_OBJS	= calcIdleTime.o

OBJS		= ${C_OBJS} ${ASM_OBJS}

EXE		= mywho

#
# You should not need to change anything below this line
#

#
# Relevant man pages:
#
# man gcc
# man as
# man lint
#

GCC		= gcc
ASM		= $(GCC)
LINT		= lint

GCC_FLAGS	= -c -g -Wall -D__EXTENSIONS__
LINT_FLAGS1	= -c -err=warn
LINT_FLAGS2	= -u -err=warn
ASM_FLAGS	= -c -g
LD_FLAGS	= -g -Wall

#
# Standard rules
#

.s.o:
	@echo "Assembling each assembly source file separately ..."
	$(ASM) $(ASM_FLAGS) $<
	@echo ""

.c.o:
	@echo "Linting each C source file separately ..."
	$(LINT) $(LINT_FLAGS1) $<
	@echo ""
	@echo "Compiling each C source file separately ..."
	$(GCC) $(GCC_FLAGS) $<
	@echo ""

#
# Simply have our project target be a single default "a.out" executable.
#

$(EXE):	$(OBJS)
	@echo "2nd phase lint on all C source files ..."
	$(LINT) $(LINT_FLAGS2) *.ln
	@echo ""
	@echo "Linking all object modules ..."
	$(GCC) -o $(EXE) $(LD_FLAGS) $(OBJS)
	@echo ""
	@echo "Done."

${C_OBJS}:      ${HEADERS}

clean:
	@echo "Cleaning up project directory ..."
	/usr/bin/rm -f *.o a.out *.ln core $(EXE)
	@echo ""
	@echo "Clean."

new:
	make --no-print-directory clean
	make --no-print-directory

utmpx_dump:
	gcc -o utmpx_dump utmpx_dump.c

