OBJDIR := ../build/dict
CARDDIR := $(DESTDIR)/usr/share/fcardsaver/
DICTS := hiragana katakana hiragana-ext katakana-ext hiragana-alien katakana-alien
DICTNAMES := $(addsuffix .tsv,$(DICTS))
DICTFILES := $(addprefix $(OBJDIR)/,$(DICTNAMES))
MKDIR := mkdir -m 755 -p
RMDIR := rmdir
INSTALL_FILE := install -m 644 -p
DELETE_FILE := rm -f

all: $(DICTFILES)

$(DICTFILES): kana.tsv hiragana.header katakana.header
	./build-kana.pl "$(OBJDIR)"

install: $(DICTFILES)
	$(MKDIR) $(CARDDIR)
	$(INSTALL_FILE) $(DICTFILES) $(CARDDIR)

uninstall:
	$(DELETE_FILE) $addprefix($(CARDDIR),$(DICTNAMES))
	$(RMDIR) $(CARDDIR)

clean distclean:
	$(DELETE_FILE) $(DICTFILES)

.PHONY: all install uninstall clean distclean
