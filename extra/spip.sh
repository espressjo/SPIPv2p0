

#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/macie:/usr/local/lib
exec /opt/spipv2p0/bin/spipdet.bin "$@"
