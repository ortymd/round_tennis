# Here we test different options of memory allocation and updates in 
# both Python and C.

import ctypes as C
from ctypes import *

lib_handle = C.cdll.LoadLibrary("./test.so")

malloc_ptr_to_payload = lib_handle.malloc_ptr_to_payload
malloc_ptr_to_payload.restype = POINTER(c_char)
malloc_ptr_to_payload.argtypes = [C.c_void_p]

update_ptr_to_payload = lib_handle.update_ptr_to_payload
update_ptr_to_payload.restype = C.c_int
update_ptr_to_payload.argtypes = [C.c_char_p]

payload = malloc_ptr_to_payload(None)
payload = 1234
print (payload)

payload = "1234"
print (payload)

payload = 'abcd123450'
print (payload)

update_ptr_to_payload (payload.ctypes.data_as(C.POINTER(C.c_char_p)))
