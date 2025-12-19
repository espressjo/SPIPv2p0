LIBS := $$(basename $(wildcard ./release/libuics.so.1.*))
LATEST := $(shell printf "%s\n" $(LIBS) | sort -V | tail -n1)


install-24:
	@cp ./release/uics.h /usr/local/include/ 
	@cp -r ./release/uics/ /usr/local/include/
	@cp ./release/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libuics.so
install-22:
	@cp ./release-22.04/uics.h /usr/local/include/ 
	@cp -r ./release-22.04/uics/ /usr/local/include/
	@cp ./release-22.04/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libuics.so
