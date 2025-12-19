

#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/macie:/usr/local/lib
export LD_PRELOAD=/usr/local/lib/libuics.so
exec /opt/spipv2p0/bin/spipdet.bin "$@"
