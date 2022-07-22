# CC = cl
# CFLAG = /c /Ox /W4 /Zp1
CC = icl
CFLAG = /c /D "NDEBUG" /Oy /Qsfalign16 /Qsox- /Qvc6 /Zp1 /G6 /GF /Og /Oi /Op /Ot /Qip
ASM = ml
AFLAG = /c /coff
#AFLAG = /c /coff  /Zd /Zi
LINK = link
LFLAG = user32.lib gdi32.lib /nologo /DLL /EXPORT:GetFilterTable
RC = rc

OBJ = rotate.obj
RES = rotate.res

ALL: rotate.auf

rotate.auf: $(OBJ) $(RES)
	$(LINK) $(LFLAG) /OUT:rotate.auf $(OBJ) $(RES)

rotate.res: rotate.rc
	$(RC) rotate.rc

rotate.obj: rotate.c filter.h
	$(CC) $(CFLAG) rotate.c

clean:
	DEL *.obj
	DEL *.res
	DEL *.exp
	DEL *.lib

