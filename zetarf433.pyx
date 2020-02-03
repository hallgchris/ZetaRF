# distutils: language = c++
# distutils: sources = src/ZetaRF.cpp

cdef extern from "src/ZetaRF.h":
    cdef cppclass ZetaRF433_preset_pins:
        ZetaRF433_preset_pins()
        void sayHello()

cdef class PyZetaRF433:
    cdef ZetaRF433_preset_pins *thisptr
    def __cinit__(self):
        self.thisptr = new ZetaRF433_preset_pins()
    def __dealloc__(self):
        del self.thisptr
    def say_hello(self):
        self.thisptr.sayHello()
