# Here we test hex_to_byte() performance

import sys
sys.path.append('../')

from functions import *
import uuid

mac = hex(uuid.getnode())
print ("mac in string:\t", mac)
mac_bytes = hex_to_byte(mac)
print ("mac in bytes:\t", mac)
print ('mac in bytes type is:\t', type(mac_bytes))
