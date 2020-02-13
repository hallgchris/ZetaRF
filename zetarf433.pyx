# distutils: language = c++
# distutils: sources = src/ZetaRF.cpp

from libcpp cimport bool

from Si4455_PartInfo cimport Si4455_PartInfo
from Si4455_FuncInfo cimport Si4455_FuncInfo

cdef extern from "src/ZetaRF.h":

    cdef cppclass ZetaRF433_preset_pins:
        ZetaRF433_preset_pins() except +
        void sayHello()
        bool begin()
        Si4455_PartInfo readPartInformation()
        Si4455_FuncInfo readFunctionRevisionInformation()

cdef class PySi4455_PartInfo:
    cdef Si4455_PartInfo *thisptr

    def __cinit__(self):
        self.thisptr = new Si4455_PartInfo()
    def __dealloc__(self):
        del self.thisptr

    @property
    def chip_rev(self):
        return self.thisptr.CHIPREV

    @property
    def pbuild(self):
        return self.thisptr.PBUILD

    @property
    def customer(self):
        return self.thisptr.CUSTOMER

    @property
    def rom_id(self):
        return self.thisptr.ROMID

    @property
    def bond(self):
        return self.thisptr.BOND

cdef class PySi4455_FuncInfo:
    cdef Si4455_FuncInfo *thisptr

    def __cinit__(self):
        self.thisptr = new Si4455_FuncInfo()
    def __dealloc__(self):
        del self.thisptr

    @property
    def rev_ext(self):
        return self.thisptr.REVEXT

    @property
    def rev_branch(self):
        return self.thisptr.REVBRANCH

    @property
    def rev_int(self):
        return self.thisptr.REVINT

    @property
    def func(self):
        return self.thisptr.FUNC

    @property
    def svn_flags(self):
        return self.thisptr.SVNFLAGS

cdef class PyZetaRF433:
    cdef ZetaRF433_preset_pins *thisptr

    def __cinit__(self):
        self.thisptr = new ZetaRF433_preset_pins()
    def __dealloc__(self):
        del self.thisptr

    def say_hello(self) -> None:
        self.thisptr.sayHello()

    def begin(self) -> bool:
        return self.thisptr.begin()

    def readPartInformation(self) -> PySi4455_PartInfo:
        cdef PySi4455_PartInfo val = PySi4455_PartInfo()
        val.thisptr[0] = self.thisptr.readPartInformation()
        return val

    def readFunctionRevisionInformation(self) -> PySi4455_FuncInfo:
        cdef PySi4455_FuncInfo val = PySi4455_FuncInfo()
        val.thisptr[0] = self.thisptr.readFunctionRevisionInformation()
        return val
