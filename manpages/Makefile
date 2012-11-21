BUILD_DIR := ../build/manpages
CTITLE := "FCardSaver manual"
RTITLE := ""
MANPAGES := fcardsaver.6x fcardsaver-cardset.7
MANFILES := $(addprefix $(BUILD_DIR)/,$(MANPAGES))
MANGZFILES := $(addsuffix .gz,$(MANFILES))
MKDIR := mkdir -m 755 -p
INSTALL_FILE := install -m 644 -p
DELETE_FILE := rm -f

all: $(MANGZFILES)

$(BUILD_DIR)/%.gz: $(BUILD_DIR)/%
	gzip -9 -c $< >$@

$(MANFILES): fcardsaver-manual.docbook
	db2x_xsltproc -s man $< | db2x_manxml --output-dir=$(BUILD_DIR)/ --list-files

clean distclean:
	$(DELETE_FILE) $(MANFILES)
	$(DELETE_FILE) $(MANGZFILES)

install:

uninstall:

.PHONY: all clean distclean install uninstall
