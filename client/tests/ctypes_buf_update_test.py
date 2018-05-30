import ctypes
lib_handle = ctypes.cdll.LoadLibrary('./test.so')

write_to_buf = lib_handle.write_to_buf
buf_sz = 1<<6
buf = ctypes.create_string_buffer (buf_sz)      # here we allocate memory which is used to store payload
print ("Buf value(raw bytes):\t", repr(buf.raw))
write_to_buf (buf)
print ("Buf value(raw bytes):\t", repr(buf.raw))
