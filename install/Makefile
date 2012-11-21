KDE4_PATH := /usr/share/kde4/services/ScreenSavers
GNOME_PATH := /usr/share/applications/screensavers
MKDIR := mkdir -m 755 -p
INSTALL_FILE := install -m 644 -p
DELETE_FILE := rm -f

all clean distclean:

install: install-kde4 install-gnome

uninstall: uninstall-kde4 uninstall-gnome

install-kde4:
	$(MKDIR) $(DESTDIR)$(KDE4_PATH)
	$(INSTALL_FILE) kde4/fcardsaver.desktop $(DESTDIR)$(KDE4_PATH)

uninstall-kde4:
	$(DELETE_FILE) $(DESTDIR)$(KDE4_PATH)/fcardsaver.desktop

install-gnome:
	$(MKDIR) $(DESTDIR)$(GNOME_PATH)
	$(INSTALL_FILE) gnome/fcardsaver.desktop $(DESTDIR)$(GNOME_PATH)

uninstall-gnome:
	$(DELETE_FILE) $(DESTDIR)$(GNOME_PATH)/fcardsaver.desktop

.PHONY: all install clean distclean
.PHONY: install-kde4 install-gnome
