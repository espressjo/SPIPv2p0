CC = g++

TARGET = spipdet
mkdir:
	@mkdir -p /opt/spipv2p0/config
	@mkdir -p /opt/spipv2p0/bin
	@mkdir -p /opt/spipv2p0/mcd
	@mkdir -p /opt/spipv2p0/data
	@mkdir -p /opt/spipv2p0/log
BASEPATH = /opt/spipv2p0

INCL = -I./ \
       -I/usr/local/include

LIB = 	-L/usr/local/lib -L/usr/local/lib/macie -lMACIE -luics -loptim -lf2r -lastro \
			-lpthread

CFLAGS = -std=c++17 -g 

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@ 


OBJS =	main.o \
	standard_spip_cmd.o \
	spip_getstatus.o \
	spip_acquisition.o
	

all: $(OBJS)
	$(CC) -Wl,-rpath=/usr/local/lib/macie:/usr/local/lib $(CFLAGS) $(OBJS) -o $(TARGET) $(INCL) $(LIB)
clean:
	@rm -f $(TARGET) $(OBJS)

install:mkdir
	@cp $(TARGET) $(BASEPATH)/bin/spipdet.bin 
	@cp ../extra/spip.sh ~/.local/bin/spipdet.v2
