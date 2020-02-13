# distutils: language = c++

cdef extern from "src/ZetaRF.h":
    cdef cppclass Si4455_PartInfo:
        Si4455_PartInfo() except +
        int CHIPREV, PBUILD, CUSTOMER, ROMID, BOND
        # UU16 not yet implemented
        #UU16 *PART, *ID
