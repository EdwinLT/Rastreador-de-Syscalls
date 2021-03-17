TARGET:=rastreador
SRCDIR:=src

CC:=gcc
CFLAGS:=-rdynamic -Iinclude/ `pkg-config --cflags gtk+-3.0` -lm
LDFLAGS:=-Iinclude/ `pkg-config --libs gtk+-3.0` -lm

GRESOURCE_XML:=resources/rastreador.gresources.xml
GRESOURCE_C:=$(BUILDDIR)/resources.c
RESOURCES:=$(filter-out $(GRESOURCE_XML), $(wildcard resources/*))

SRCS:=$(wildcard $(SRCDIR)/*.c)
BUILDDIR:=build
OBJS:=$(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)
DEPS:=$(OBJS:%.o=%.d)


.PHONY: all run
all: $(TARGET)

run: all
	@./$(TARGET)

$(TARGET): $(OBJS) $(GRESOURCE_C)
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

$(BUILDDIR)/:
	mkdir -p $@

$(OBJS): | $(BUILDDIR)/
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -MMD -MP $< -o $@ -Iinclude/ $(CFLAGS)

$(GRESOURCE_C): | $(BUILDDIR)
$(GRESOURCE_C): $(GRESOURCE_XML) $(RESOURCES)
	glib-compile-resources $< --target=$@ --sourcedir=resources/ --generate-source

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(TARGET)
