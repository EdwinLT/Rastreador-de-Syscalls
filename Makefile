SRCDIR:=src
TARGET:=rastreador

SRCS:=$(wildcard $(SRCDIR)/*.c)
BUILDDIR:=build
OBJS:=$(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPS:=$(OBJS:%.o=%.d)

CC:=gcc
LIBS:=-Iinclude/ `pkg-config --cflags --libs gtk+-3.0`
CFLAGS:=-rdynamic

.PHONY: all run
all: $(TARGET)

run: all
	@./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIBS) $(CFLAGS)

$(BUILDDIR)/:
	mkdir -p $@

$(OBJS): | $(BUILDDIR)/

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -MMD -MP $< -o $@ $(LIBS) $(CFLAGS) 

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)