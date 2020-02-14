# distutils: language = c++
# distutils: sources = src/ZetaRF.cpp

from libcpp cimport bool

from Si4455_PartInfo cimport Si4455_PartInfo
from Si4455_FuncInfo cimport Si4455_FuncInfo
from ZetaRF433_PresetPins cimport ZetaRF433_PresetPins

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
    cdef ZetaRF433_PresetPins *thisptr
    cdef int packet_length

    def __cinit__(self, packet_length: int):
        self.thisptr = new ZetaRF433_PresetPins()
        self.packet_length = packet_length
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

    def start_listening_on_channel(self, channel: int) -> bool:
        return self.thisptr.startListeningOnChannel(channel)

    def check_received(self) -> bool:
        return self.thisptr.checkReceived()

    def read_fixed_length_packet(self) -> bytes:
        packet = bytes(self.packet_length)
        self.thisptr.readFixedLengthPacket(packet, self.packet_length)
        return packet
