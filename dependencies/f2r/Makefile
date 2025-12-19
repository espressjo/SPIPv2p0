LIBS := $$(basename $(wildcard ./release/libf2r.so.1.*))
LATEST := $(shell printf "%s\n" $(LIBS) | sort -V | tail -n1)


install-24:
	@cp ./release/libf2r.h /usr/local/include/ 
	@cp -r ./release/f2r/ /usr/local/include/
	@cp ./release/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libf2r.so
install-22:
	@cp ./release-22.04/libf2r.h /usr/local/include/ 
	@cp -r ./release-22.04/f2r/ /usr/local/include/
	@cp ./release-22.04/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libf2r.so
