# go_wrapper.py
import ctypes
import os

lib = ctypes.CDLL(os.path.abspath("../zad3/euler_c/libgo_euler.so"))

lib.goGCD.argtypes = [ctypes.c_int, ctypes.c_int]
lib.goGCD.restype = ctypes.c_int

lib.goMinimalDivisor.argtypes = [ctypes.c_int]
lib.goMinimalDivisor.restype = ctypes.c_int

lib.goTotient.argtypes = [ctypes.c_int]
lib.goTotient.restype = ctypes.c_int

lib.goDiophantic.argtypes = [
    ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_int)
]
lib.goDiophantic.restype = None


def gcd(a, b):
    return lib.goGCD(a, b)

def minimal_divisor(n):
    return lib.goMinimalDivisor(n)

def totient(n):
    return lib.goTotient(n)

def diophantic(a, b):
    x = ctypes.c_int()
    y = ctypes.c_int()
    lib.goDiophantic(a, b, ctypes.byref(x), ctypes.byref(y))
    return x.value, y.value