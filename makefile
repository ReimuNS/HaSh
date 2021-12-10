# Constants
CC=gcc

LINKFLAGS=
INCLUDEFLAGS=

OTHER_FLAGS=

DEBUGFLAGS=-Wall -ggdb -fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined
RELEASEFLAGS=-O2

BUILD_DIR=build
OUTDIR=$(BUILD_DIR)/out
DEBUG_OUTDIR=$(OUTDIR)/debug
RELEASE_OUTDIR=$(OUTDIR)/release
OUTNAME=shell


# Obj output
OBJDIR=$(BUILD_DIR)/objs
DEBUG_OBJ_DIR=$(OBJDIR)/debug
RELEASE_OBJ_DIR=$(OBJDIR)/release

objs=main.o shell.o shell_command.o

debugObjs=$(addprefix $(DEBUG_OBJ_DIR)/,$(objs))
releaseObjs=$(addprefix $(RELEASE_OBJ_DIR)/,$(objs))

# Headers
headers=shell.h shell_command.h

# Debug Build
$(DEBUG_OUTDIR)/$(OUTNAME): $(debugObjs)
	$(CC) -o $@ $(debugObjs) $(LINKFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

$(DEBUG_OBJ_DIR)/%.o: %.c $(headers) | create_dirs
	$(CC) -c -o $@ $(patsubst $(DEBUG_OBJ_DIR)/%,%,$(@:.o=.c)) $(INCLUDEFLAGS) $(DEBUGFLAGS) $(OTHER_FLAGS)

# Release Build
$(RELEASE_OUTDIR)/$(OUTNAME): $(releaseObjs)
	$(CC) -o $@ $(releaseObjs) $(LINKFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)

$(RELEASE_OBJ_DIR)/%.o: %.c $(headers) | create_dirs
	$(CC) -c -o $@ $(patsubst $(RELEASE_OBJ_DIR)/%,%,$(@:.o=.c)) $(INCLUDEFLAGS) $(RELEASEFLAGS) $(OTHER_FLAGS)

# Helpers
.PHONY: debug
debug: $(DEBUG_OUTDIR)/$(OUTNAME)

.PHONY: release
release: $(RELEASE_OUTDIR)/$(OUTNAME)

.PHONY: create_dirs
create_dirs:
	-mkdir $(BUILD_DIR)
	-mkdir $(OBJDIR)
	-mkdir $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR)
	-mkdir $(OUTDIR)
	-mkdir $(DEBUG_OUTDIR) $(RELEASE_OUTDIR)

.PHONY: clean
clean:
	-rm $(DEBUG_OUTDIR)/$(OUTNAME) $(RELEASE_OUTDIR)/$(OUTNAME) $(debugObjs) $(releaseObjs)
	-rmdir $(DEBUG_OUTDIR) $(RELEASE_OUTDIR)
	-rmdir $(OUTDIR)
	-rmdir $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR)
	-rmdir $(OBJDIR)
	-rmdir $(BUILD_DIR)

