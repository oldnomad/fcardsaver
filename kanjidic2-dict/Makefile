OBJDIR := ../build/dict
CARDDIR := $(DESTDIR)/usr/share/fcardsaver/
DICT_ALL := kanjidic2.tsv
DICTFILE_ALL := $(OBJDIR)/$(DICT_ALL)
DICTS := G1 G2 G3 G4 G5 G6 jouyou jinmei itai hyougai
DICTNAMES := $(addsuffix .tsv,$(DICTS))
DICTFILES := $(addprefix $(OBJDIR)/kanjidic2-,$(DICTNAMES))
MKDIR := mkdir -m 755 -p
RMDIR := rmdir
INSTALL_FILE := install -m 644 -p
DELETE_FILE := rm -f

all: $(DICTFILE_ALL) $(DICTFILES)

download:
	rm kanjidic2.xml.gz
	wget -nd http://www.csse.monash.edu.au/~jwb/kanjidic2/kanjidic2.xml.gz

$(DICTFILE_ALL): kanjidic2.header kanjidic2.xslt kanjidic2.xml.gz
	gunzip -c kanjidic2.xml.gz | xsltproc --stringparam "header" "$$(cat kanjidic2.header)" -o "$@" kanjidic2.xslt -

$(DICTFILES): kanjidic2.header $(DICTFILE_ALL)
	echo "# Kanji $(patsubst $(OBJDIR)/kanjidic2-%.tsv,%,$@)" >"$@"
	cat kanjidic2.header >>"$@"
	grep -e "\"Kanji-$(patsubst $(OBJDIR)/kanjidic2-%.tsv,%,$@)\"\$$" "$(DICTFILE_ALL)" >>"$@"

install: $(DICTFILE_ALL) $(DICTFILES)
	$(MKDIR) $(CARDDIR)
	$(INSTALL_FILE) $(DICTFILE_ALL) $(DICTFILES) $(CARDDIR)

uninstall:
	$(DELETE_FILE) $addprefix($(CARDDIR),$(DICT_ALL) $(DICTNAMES))
	$(RMDIR) $(CARDDIR)

clean distclean:
	rm -f $(DICTFILE_ALL) $(DICTFILES)

.PHONY: all download install uninstall clean distclean
