import ctypes
import os

lib = ctypes.CDLL(os.path.abspath("../zad2/libada_euler.so"))

class Pair(ctypes.Structure):
    _fields_ = [("first", ctypes.c_int),
                ("second", ctypes.c_int)]

lib.adaGCD.argtypes = [ctypes.c_int, ctypes.c_int]
lib.adaGCD.restype = ctypes.c_int

lib.adaMinimalDivisor.argtypes = [ctypes.c_int]
lib.adaMinimalDivisor.restype = ctypes.c_int

lib.adaTotient.argtypes = [ctypes.c_int]
lib.adaTotient.restype = ctypes.c_int

lib.adaDiophantic.argtypes = [ctypes.c_int, ctypes.c_int]
lib.adaDiophantic.restype = Pair


def gcd(a, b):
    return lib.adaGCD(a, b)

def minimal_divisor(n):
    return lib.adaMinimalDivisor(n)

def totient(n):
    return lib.adaTotient(n)

def diophantic(a, b):
    res = lib.adaDiophantic(a, b)
    return res.first, res.second