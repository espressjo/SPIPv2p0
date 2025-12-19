BASE = /opt/spipv2p0

mkdir:
	@mkdir -p $(BASE)
	@mkdir -p $(BASE)/lib
	@mkdir -p $(BASE)/include
	@mkdir -p $(BASE)/bin
	@mkdir -p $(BASE)/data
	@mkdir -p $(BASE)/mcd
	@mkdir -p $(BASE)/log
	@mkdir -p $(BASE)/python

install: mkdir
	@cd ./src && make install
	@cd ./config && make install
	@cd ./mcd && make install
	@cp -r ./python/* $(BASE)/python/
	@cd $(BASE)/python && pip install -e .
clean:
	@cd ./src && make clean

all:
	@cd ./src && make all
