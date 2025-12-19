BASEPATH = /usr/local

latest := $(shell ls -v macie*.tar.xz | tail -n 1)

folders: 
	@mkdir -p $(BASEPATH)/lib/macie/
	@mkdir -p ~/.local/bin/
uncompress:
	@tar -xf $(latest) --strip-components=2 -C $(BASEPATH)/lib/macie/ 

.PHONY: test
all:
	@echo "nothing to do"
install: folders uncompress
	@cp ./etc/msac ~/.local/bin/msac
	@cp ./etc/mac ~/.local/bin/mac
	@ln -sf $(BASEPATH)/lib/macie/libQt5SerialPort.so.5.10.1 $(BASEPATH)/lib/macie/libQt5SerialPort.so.5
	@ln -sf $(BASEPATH)/lib/macie/libmil.so.10.20.517 $(BASEPATH)/lib/macie/libmil.so.10
	@ln -sf $(BASEPATH)/lib/macie/libicudata.so.56.1 $(BASEPATH)/lib/macie/libicudata.so.56
	@ln -sf $(BASEPATH)/lib/macie/libicuuc.so.56.1 $(BASEPATH)/lib/macie/libicuuc.so.56 
	@ln -sf $(BASEPATH)/lib/macie/libicui18n.so.56.1 $(BASEPATH)/lib/macie/libicui18n.so.56 
	@ln -sf $(BASEPATH)/lib/macie/libQt5Core.so.5.10.1 $(BASEPATH)/lib/macie/libQt5Core.so.5 
	@ln -sf $(BASEPATH)/lib/macie/libQt5DBus.so.5.10.1 $(BASEPATH)/lib/macie/libQt5DBus.so.5 
	@ln -sf $(BASEPATH)/lib/macie/libQt5Gui.so.5.10.1 $(BASEPATH)/lib/macie/libQt5Gui.so.5 
	@ln -sf $(BASEPATH)/lib/macie/libQt5Network.so.5.10.1 $(BASEPATH)/lib/macie/libQt5Network.so.5 
	@ln -sf $(BASEPATH)/lib/macie/libQt5Widgets.so.5.10.1 $(BASEPATH)/lib/macie/libQt5Widgets.so.5 
	@ln -sf $(BASEPATH)/lib/macie/libQt5XcbQpa.so.5.10.1 $(BASEPATH)/lib/macie/libQt5XcbQpa.so.5
	@mv $(BASEPATH)/lib/macie/macie.h $(BASEPATH)/include/macie.h	
	@ln -sf $(BASEPATH)/lib/macie/libMACIE.so $(BASEPATH)/lib/libMACIE.so

clean:
	@echo "nothing to do"
	@echo "try make uninstall"
uninstall:
	@rm -rf $(BASEPATH)/lib/macie/
	@rm -f $(BASEPATH)/include/macie.h
	@rm -f ~/.local/bin/msac
	@rm -f ~/.local/bin/mac
release:
	@mkdir -p ./release/
	@cp ./Makefile ./release/
	@cp $(latest) ./release/
	@cp -r ./etc/ ./release/

		
	
