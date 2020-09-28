TARGET:=rastreador
SRCDIR:=src

SRCS:=$(wildcard $(SRCDIR)/*.c)
BUILDDIR:=build
OBJS:=$(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPS:=$(OBJS:%.o=%.d)

GRESOURCE_XML:=resources/rastreador.gresources.xml
GRESOURCE_C:=$(BUILDDIR)/resources.c
RESOURCES:=$(filter-out $(GRESOURCE_XML), $(wildcard resources/*))

CC:=gcc
LIBS:=-Iinclude/ `pkg-config --cflags --libs gtk+-3.0` -lm
CFLAGS:=-rdynamic

.PHONY: all run
all: $(TARGET)

run: all
	@./$(TARGET)

$(TARGET): $(OBJS) $(GRESOURCE_C)
	$(CC) $^ -o $@ $(LIBS) $(CFLAGS)

$(BUILDDIR)/:
	mkdir -p $@

$(OBJS): | $(BUILDDIR)/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -MMD -MP $< -o $@ $(LIBS) $(CFLAGS)

$(GRESOURCE_C): | $(BUILDDIR)
$(GRESOURCE_C): $(GRESOURCE_XML) $(RESOURCES)
	glib-compile-resources $< --target=$@ --sourcedir=resources/ --generate-source

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)