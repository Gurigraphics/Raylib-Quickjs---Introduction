OBJDIR=.obj
CC=gcc
CFLAGS=-g -Wall -MMD -MF $(OBJDIR)/$(@F).d -Wno-array-bounds
LDFLAGS=-flto
CFLAGS_OPT=$(CFLAGS) -O2 -flto
DEFINES:=-D_GNU_SOURCE
CFLAGS+=$(DEFINES)  

LIBS=-L./quickjs/lib/ -lquickjs -lm -ldl -lpthread -L./raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm  
INCLUDES=-I./quickjs/include -I./raylib/include -I./raylibJS/include   
 
$(OBJDIR):
	mkdir -p $(OBJDIR)

game: $(OBJDIR) $(OBJDIR)/raylibJS.o $(OBJDIR)/raylibJS_main.o
	$(CC) $(LDFLAGS) $(CFLAGS_OPT) -o $@ $(OBJDIR)/raylibJS_main.o $(OBJDIR)/raylibJS.o $(LIBS)
	strip ./game.exe

$(OBJDIR)/raylibJS.o: raylibJS.c
	$(CC) $(LDFLAGS) $(CFLAGS_OPT) -c $(INCLUDES) -o $@ raylibJS.c

$(OBJDIR)/raylibJS_main.o: raylibJS_main.c
	$(CC) $(LDFLAGS) $(CFLAGS_OPT) -c $(INCLUDES) -o $@ raylibJS_main.c

raylibJS_main.c: game.js
	./quickjs/bin/qjsc -flto -e -M raylibJS,raylibJS -m -o $@ game.js
	
clean:
	rm ./raylibJS_main.c
	rm -rf $(OBJDIR)

# The option -M enables the C module to be imported from the script as following: import { open, close } from "process"
# The flag -Wl,--subsystem,windows, remove the console prompt 