ifndef PLATFORM
    include ../platform.mk
endif
BUILD_DIR := ../build/docs
MANPAGES := fcardsaver.6x fcardsaver-cardset.7
MANFILES := $(addprefix $(BUILD_DIR)/,$(MANPAGES))
MANGZFILES := $(addsuffix .gz,$(MANFILES))
HTMLDOC := $(BUILD_DIR)/fcardsaver-manual.html

all: make-dirs $(MANGZFILES) $(HTMLDOC)

make-dirs:
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/%.gz: $(BUILD_DIR)/%
	$(GZIP) -9 -c $< >$@

$(MANFILES): fcardsaver-manual.docbook
	$(XSLTPROC) -o $(BUILD_DIR)/ $(DOCBOOK_XSL)/manpages/docbook.xsl $<

$(HTMLDOC): fcardsaver-manual.docbook
	$(XSLTPROC) -o $@ $(DOCBOOK_XSL)/xhtml/docbook.xsl $<

clean distclean:
	$(DELETE_FILE) $(MANFILES)
	$(DELETE_FILE) $(MANGZFILES)
	$(DELETE_FILE) $(HTMLDOC)

install:

uninstall:

.PHONY: make-dirs all clean distclean install uninstall
