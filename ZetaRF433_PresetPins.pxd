from libcpp cimport bool
from libc.stdint cimport uint8_t

from Si4455_PartInfo cimport Si4455_PartInfo
from Si4455_FuncInfo cimport Si4455_FuncInfo

cdef extern from "src/ZetaRF.h" namespace "ZetaRF":

    cdef cppclass ReadPacketResult:
        pass
    cdef cppclass Status:
        pass

cdef extern from "src/ZetaRF.h":

    cdef cppclass ZetaRF433_PresetPins:
        ZetaRF433_PresetPins() except +

        bool begin()
        bool beginWithPacketLengthOf(uint8_t packetLength)

        bool startListeningOnChannel(uint8_t channel)

        Si4455_PartInfo readPartInformation()
        Si4455_FuncInfo readFunctionRevisionInformation()

        bool checkFor(Status status)
        bool checkReceived()

        ReadPacketResult readPacket(uint8_t*& data, uint8_t byteCount)

        bool sendPacket(uint8_t channel, const uint8_t *data, uint8_t dataSize)
