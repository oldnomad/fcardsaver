#
# Main build file for FCardSaver
#
QMAKE       := qmake
MAKE        := make
MKDIR       := mkdir -m 755 -p

QTSPEC      := linux-g++
PLATFORM    := linux
EXEMAKEFILE := Makefile_$(PLATFORM)
EXETARGET   := release

EXEFILE     := fcardsaver
EXEPATH     := build/$(EXETARGET)-$(PLATFORM)

DICTDIRS    := kana-dict kanjidic2-dict
DICTBUILD   := $(addsuffix -build,$(DICTDIRS))
DICTINSTALL := $(addsuffix -install,$(DICTDIRS))
DICTUNINST  := $(addsuffix -uninstall,$(DICTDIRS))
DICTCLEAN   := $(addsuffix -clean,$(DICTDIRS))
DICTDCLEAN  := $(addsuffix -distclean,$(DICTDIRS))

all: exec-build man-build dict-build
install: exec-install kde4-install gnome-install dict-install
uninstall: exec-uninstall kde4-install gnome-install dict-uninstall
clean: exec-clean man-clean dict-clean
distclean: exec-distclean man-distclean dict-distclean

dict-build: $(DICTBUILD)
dict-install: $(DICTINSTALL)
dict-uninstall: $(DICTUNINST)
dict-clean: $(DICTCLEAN)
dict-distclean: $(DICTDCLEAN)

.PHONY: all install uninstall clean distclean
.PHONY: dict-build dict-install dict-uninstall dict-clean dict-distclean
.PHONY: exec-build exec-install exec-uninstall exec-clean exec-distclean
.PHONY: man-build man-clean man-distclean
.PHONY: kde4-install kde4-uninstall
.PHONY: gnome-install gnome-uninstall
.PHONY: $(DICTBUILD) $(DICTINSTALL) $(DICTUNINST) $(DICTCLEAN) $(DICTDCLEAN)

#
# Executable install/uninstall
#
exec-build: $(EXEPATH)/$(EXEFILE)

src/$(EXEMAKEFILE): src/fcardsaver.pro
	cd src && $(QMAKE) -makefile "CONFIG += $(EXETARGET)" fcardsaver.pro -r -spec $(QTSPEC)

$(EXEPATH)/$(EXEFILE): src/$(EXEMAKEFILE)
	$(MKDIR) $(EXEPATH)
	$(MAKE) -C src -f $(EXEMAKEFILE) first

exec-install: src/$(EXEMAKEFILE) $(EXEPATH)/$(EXEFILE)
	$(MAKE) -C src -f $(EXEMAKEFILE) INSTALL_ROOT=$(DESTDIR) install

exec-uninstall: src/$(EXEMAKEFILE)
	$(MAKE) -C src -f $(EXEMAKEFILE) INSTALL_ROOT=$(DESTDIR) uninstall

exec-clean: src/$(EXEMAKEFILE)
	$(MAKE) -C src -f $(EXEMAKEFILE) clean

exec-distclean: src/$(EXEMAKEFILE)
	$(MAKE) -C src -f $(EXEMAKEFILE) distclean

#
# Manual docs
#
man-build:
	$(MAKE) -C manpages all

man-clean:
	$(MAKE) -C manpages clean

man-distclean:
	$(MAKE) -C manpages distclean

#
# KDE & GNOME screensaver installation
#
kde4-install:
	$(MAKE) -C install install-kde4

kde4-uninstall:
	$(MAKE) -C install uninstall-kde4

gnome-install:
	$(MAKE) -C install install-gnome

gnome-uninstall:
	$(MAKE) -C install uninstall-gnome

#
# Dictionaries install/uninstall
#
$(DICTBUILD):
	$(MAKE) -C $(@:-build=) all

$(DICTINSTALL):
	$(MAKE) -C $(@:-install=) install

$(DICTUNINST):
	$(MAKE) -C $(@:-uninstall=) uninstall

$(DICTCLEAN):
	$(MAKE) -C $(@:-clean=) clean

$(DICTDCLEAN):
	$(MAKE) -C $(@:-distclean=) distclean
