ifndef PLATFORM
    include ../platform.mk
endif
OBJDIR := ../build/dict
CARDDIR := $(DESTDIR)/usr/share/fcardsaver/
DICT_ALL := kanjidic2.tsv
DICTFILE_ALL := $(OBJDIR)/$(DICT_ALL)
DICTS := G1 G2 G3 G4 G5 G6 jouyou jinmei itai hyougai
DICTNAMES := $(addsuffix .tsv,$(DICTS))
DICTFILES := $(addprefix $(OBJDIR)/kanjidic2-,$(DICTNAMES))

all: $(DICTFILE_ALL) $(DICTFILES)

download: clean-xml kanjidic2.xml.gz

clean-xml:
	$(DELETE_FILE) kanjidic2.xml.gz

kanjidic2.xml.gz:
	$(WGET) -nd http://www.csse.monash.edu.au/~jwb/kanjidic2/kanjidic2.xml.gz

$(DICTFILE_ALL): kanjidic2.header kanjidic2.xslt kanjidic2.xml.gz
	$(MKDIR) "$(OBJDIR)"
	$(GZIP) -d -c kanjidic2.xml.gz | $(XSLTPROC) --stringparam "header" "$$(cat kanjidic2.header)" -o "$@" kanjidic2.xslt -

$(DICTFILES): $(DICTFILE_ALL)
	$(PERL) -w split-kanji.pl $(DICTFILE_ALL) '$(OBJDIR)/kanjidic2-%s.tsv' $(DICTS)

install: $(DICTFILE_ALL) $(DICTFILES)
	$(MKDIR) "$(CARDDIR)"
	$(INSTALL_FILE) $(DICTFILE_ALL) $(DICTFILES) "$(CARDDIR)"

uninstall:
	$(DELETE_FILE) $addprefix($(CARDDIR),$(DICT_ALL) $(DICTNAMES))
	$(RMDIR) "$(CARDDIR)"

clean distclean:
	$(DELETE_FILE) $(DICTFILE_ALL) $(DICTFILES)

.PHONY: all download clean-xml install uninstall clean distclean
