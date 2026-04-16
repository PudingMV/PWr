import ctypes
import os

lib = ctypes.CDLL(os.path.abspath("../zad1/libeuler.so"))

class Pair(ctypes.Structure):
    _fields_ = [("first", ctypes.c_int),
                ("second", ctypes.c_int)]

lib.NWD.argtypes = [ctypes.c_int, ctypes.c_int]
lib.NWD.restype = ctypes.c_int

lib.minimalDivisor.argtypes = [ctypes.c_int]
lib.minimalDivisor.restype = ctypes.c_int

lib.Totient.argtypes = [ctypes.c_int]
lib.Totient.restype = ctypes.c_int

lib.diophantic.argtypes = [ctypes.c_int, ctypes.c_int]
lib.diophantic.restype = Pair


def gcd(a, b):
    return lib.NWD(a, b)

def minimal_divisor(n):
    return lib.minimalDivisor(n)

def totient(n):
    return lib.Totient(n)

def diophantic(a, b):
    res = lib.diophantic(a, b)
    return res.first, res.second