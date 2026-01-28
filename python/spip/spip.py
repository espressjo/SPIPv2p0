from uics.uics import uics
from uics.b64decoder import create_decoder,decoder_to_structure

TELEMETRY_DECODER = create_decoder(["/usr/local/include/astro/telemetry_t.h"],"telemetry_t")
STATUS_DECODER = create_decoder(["/opt/spipv2p0/include/spip_telemetry_t.h","/usr/local/include/astro/insthandle.h","/usr/local/include/astro/telemetry_t.h"], "spip_telemetry_t")
CMDCONFFILE = '/opt/spipv2p0/config/cmd.conf'

class spip(uics):
    def __init__(self,host,port,cmd_cfg_file=CMDCONFFILE):
        super().__init__(host,port,config_file=cmd_cfg_file)    
    def getstatus(self):
        _b64_status = self.b64spipstatus()#the right status function
        try:
            length,message = _b64_status.split(" ")
            length = int(length)
            status = decoder_to_structure(message, STATUS_DECODER)
            return status
        except:
            print("Fail to fetch status")
        return None
    def telemetry_t(self):
        _b64_tlm = self.telemetry(b64=True)#the right status function
        try:
            length,message = _b64_tlm.split(" ")
            length = int(length)
            status = decoder_to_structure(message, TELEMETRY_DECODER)
            return status
        except:
            print("Fail to fetch telemetry")
        return None





if '__main__' in __name__:
    with spip("localhost", 8555) as _spip:
        _spip.start()
