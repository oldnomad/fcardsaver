ifndef PLATFORM
    include ../platform.mk
endif
OBJDIR := ../build/dict
CARDDIR := $(DESTDIR)/usr/share/fcardsaver/
DICTALL := $(OBJDIR)/kana.tsv
DICTS := hiragana katakana hiragana-ext katakana-ext hiragana-alien katakana-alien
DICTNAMES := $(addsuffix .tsv,$(DICTS))
DICTFILES := $(addprefix $(OBJDIR)/,$(DICTNAMES))

all: $(DICTFILES)

$(DICTALL): kana.xml hiragana.header katakana.header
	$(MKDIR) "$(OBJDIR)"
	$(XSLTPROC) -o "$@" kana.xslt kana.xml

$(DICTFILES): $(DICTALL)
	$(PERL) -w ../split-tsv.pl "$(DICTALL)" "$(OBJDIR)" kana.cfg

install: $(DICTFILES)
	$(MKDIR) "$(CARDDIR)"
	$(INSTALL_FILE) $(DICTFILES) $(CARDDIR)

uninstall:
	$(DELETE_FILE) $addprefix($(CARDDIR),$(DICTNAMES))
	$(RMDIR) "$(CARDDIR)"

clean distclean:
	$(DELETE_FILE) $(DICTFILES) $(DICTALL)

.PHONY: all install uninstall clean distclean
