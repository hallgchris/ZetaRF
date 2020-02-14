from libcpp cimport bool

from Si4455_PartInfo cimport Si4455_PartInfo
from Si4455_FuncInfo cimport Si4455_FuncInfo

cdef extern from "src/ZetaRF.h":

    cdef cppclass ZetaRF433_PresetPins:
        ZetaRF433_PresetPins() except +
        void sayHello()
        bool begin()
        Si4455_PartInfo readPartInformation()
        Si4455_FuncInfo readFunctionRevisionInformation()
