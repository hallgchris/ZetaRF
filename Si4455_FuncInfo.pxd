# distutils: language = c++

cdef extern from "src/ZetaRF.h":
    cdef cppclass Si4455_FuncInfo:
        Si4455_FuncInfo() except +
        int REVEXT, REVBRANCH, REVINT, FUNC, SVNFLAGS
        # UU16/UU32 not yet implemented
        #UU16 PATCH
        #UU32 SVNREV
