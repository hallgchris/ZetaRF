from libcpp cimport bool
from libc.stdint cimport uint8_t

from Si4455_PartInfo cimport Si4455_PartInfo
from Si4455_FuncInfo cimport Si4455_FuncInfo

cdef extern from "src/ZetaRF.h" namespace "ZetaRF":

    cdef cppclass ReadPacketResult:
        pass

cdef extern from "src/ZetaRF.h":

    cdef cppclass ZetaRF433_PresetPins:
        ZetaRF433_PresetPins() except +
        void sayHello()
        bool begin()
        Si4455_PartInfo readPartInformation()
        Si4455_FuncInfo readFunctionRevisionInformation()
        bool startListeningOnChannel(uint8_t channel)
        bool checkReceived()
        ReadPacketResult readFixedLengthPacket(uint8_t*& data, uint8_t byteCount)
