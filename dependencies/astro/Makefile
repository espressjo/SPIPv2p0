
LATEST = libastro.so.1.1
install-24:
	@cp ./release-24.04/libastro.h /usr/local/include/ 
	@cp -r ./release-24.04/astro/ /usr/local/include/
	@cp ./release-24.04/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libastro.so
install-22:
	@cp ./release-22.04/libastro.h /usr/local/include/ 
	@cp -r ./release-22.04/astro/ /usr/local/include/
	@cp ./release-22.04/$(LATEST) /usr/local/lib/
	@ln -sf /usr/local/lib/$(LATEST) /usr/local/lib/libastro.so
