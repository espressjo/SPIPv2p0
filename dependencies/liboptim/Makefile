LIBS := $$(basename $(wildcard ./release/liboptim.so.1.*))
LATEST := $(shell printf "%s\n" $(LIBS) | sort -V | tail -n1)


install-24:
	@cp ./release/liboptim.h /usr/local/include/ 
	@cp -r ./release/optim/ /usr/local/include/
	@cp ./release/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/liboptim.so
install-22:
	@cp ./release-22.04/liboptim.h /usr/local/include/ 
	@cp -r ./release-22.04/optim/ /usr/local/include/
	@cp ./release-22.04/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/liboptim.so
