ifndef PLATFORM
    include ../platform.mk
endif
BUILD_DIR := ../build/manpages
CTITLE := "FCardSaver manual"
RTITLE := ""
MANPAGES := fcardsaver.6x fcardsaver-cardset.7
MANFILES := $(addprefix $(BUILD_DIR)/,$(MANPAGES))
MANGZFILES := $(addsuffix .gz,$(MANFILES))

all: make-dirs $(MANGZFILES)

make-dirs:
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.gz: $(BUILD_DIR)/%
	$(GZIP) -9 -c $< >$@

$(MANFILES): fcardsaver-manual.docbook
	$(DB2X_XSLTPROC) -s man $< | $(DB2X_MANXML) --output-dir=$(BUILD_DIR)/ --list-files

clean distclean:
	$(DELETE_FILE) $(MANFILES)
	$(DELETE_FILE) $(MANGZFILES)

install:

uninstall:

.PHONY: make-dirs all clean distclean install uninstall
