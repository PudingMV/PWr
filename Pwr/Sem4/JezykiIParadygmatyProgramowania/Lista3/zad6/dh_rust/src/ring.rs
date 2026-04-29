use std::fmt;
use std::ops::{Add, Sub, Mul, Div, AddAssign, SubAssign, MulAssign, DivAssign};

#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct Ring<const N: u64> {
    value: i64,
}

impl<const N: u64> Ring<N> {

    pub fn new(v: i64) -> Self {
        let m = N as i64;
        let mut r = v % m;
        if r < 0 {
            r += m;
        }
        Self { value: r }
    }

    pub fn get(&self) -> i64 {
        self.value
    }

    fn mod_inverse(a: i64) -> i64 {
        let m = N as i64;
        let (mut t, mut new_t) = (0i64, 1i64);
        let (mut r, mut new_r) = (m, a);

        while new_r != 0 {
            let q = r / new_r;

            let tmp_t = t - q * new_t;
            t = new_t;
            new_t = tmp_t;

            let tmp_r = r - q * new_r;
            r = new_r;
            new_r = tmp_r;
        }

        if r != 1 {
            panic!("No inverse exists");
        }

        if t < 0 {
            t += m;
        }

        t
    }
}

// +, -, *, /
impl<const N: u64> Add for Ring<N> {
    type Output = Self;
    fn add(self, rhs: Self) -> Self {
        Self::new(self.value + rhs.value)
    }
}

impl<const N: u64> Sub for Ring<N> {
    type Output = Self;
    fn sub(self, rhs: Self) -> Self {
        Self::new(self.value - rhs.value)
    }
}

impl<const N: u64> Mul for Ring<N> {
    type Output = Self;
    fn mul(self, rhs: Self) -> Self {
        Self::new((self.value * rhs.value) % (N as i64))
    }
}

impl<const N: u64> Div for Ring<N> {
    type Output = Self;
    fn div(self, rhs: Self) -> Self {
        let inv = Self::mod_inverse(rhs.value);
        Self::new((self.value * inv) % (N as i64))
    }
}

// assign operators
impl<const N: u64> AddAssign for Ring<N> {
    fn add_assign(&mut self, rhs: Self) {
        *self = *self + rhs;
    }
}

impl<const N: u64> SubAssign for Ring<N> {
    fn sub_assign(&mut self, rhs: Self) {
        *self = *self - rhs;
    }
}

impl<const N: u64> MulAssign for Ring<N> {
    fn mul_assign(&mut self, rhs: Self) {
        *self = *self * rhs;
    }
}

impl<const N: u64> DivAssign for Ring<N> {
    fn div_assign(&mut self, rhs: Self) {
        *self = *self / rhs;
    }
}

impl<const N: u64> fmt::Display for Ring<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)
    }
}