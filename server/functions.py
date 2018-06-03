
# Convert a string hex byte values into a byte string. 
# This function is used to parse mac addresses which are 
# returned by uuid.getnode() and have such format:
#   '0xffffffffffff'
import pdb

def hex_to_byte( hex_str ):
    bytes_list = []

    hex_str = ''.join( hex_str.split(" ") )
    hex_str = hex_str[2:] # here we discard the leading '0x' and
                               # get only the value that we need
    print ("Discard '0x':\t", hex_str)

    for i in range(0, len(hex_str), 2):
        #pdb.set_trace()
        bytes_list.append( chr( int (hex_str[i:i+2], 16 ) ).encode('utf8') )
    print ('bytes_list:\t', bytes_list)
    print ('bytes_list:\t', bytes_list)

    return ''.join(bytes_list)

