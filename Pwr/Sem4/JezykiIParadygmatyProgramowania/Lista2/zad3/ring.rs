use std::fmt;
use std::ops::{Add, Sub, Mul, Div, AddAssign, SubAssign, MulAssign, DivAssign};

#[derive(Copy, Clone, Debug, PartialEq, Eq, PartialOrd, Ord)]
pub struct Ring<const N: i32> {
    value: i32,
}

impl<const N: i32> Ring<N> {
    pub fn new(v: i32) -> Self {
        let mut x = v % N;
        if x < 0 {
            x += N;
        }
        Self { value: x }
    }

    pub fn to_int(self) -> i32 {
        self.value
    }

    pub fn inverse(self) -> Option<Self> {
        let mut a = N;
        let mut b = self.value;

        let (mut x0, mut x1) = (1, 0);

        while b != 0 {
            let q = a / b;

            let tmp = a % b;
            a = b;
            b = tmp;

            let tmp = x0 - q * x1;
            x0 = x1;
            x1 = tmp;
        }

        if a != 1 {
            return None;
        }

        if x0 < 0 {
            x0 += N;
        }

        Some(Self::new(x0))
    }
}

//mathematical operations
impl<const N: i32> Add for Ring<N> {
    type Output = Self;

    fn add(self, rhs: Self) -> Self {
        Self::new(self.value + rhs.value)
    }
}

impl<const N: i32> Sub for Ring<N> {
    type Output = Self;

    fn sub(self, rhs: Self) -> Self {
        Self::new(self.value - rhs.value)
    }
}

impl<const N: i32> Mul for Ring<N> {
    type Output = Self;

    fn mul(self, rhs: Self) -> Self {
        Self::new(self.value * rhs.value)
    }
}

impl<const N: i32> Div for Ring<N> {
    type Output = Self;

    fn div(self, rhs: Self) -> Self {
        match rhs.inverse() {
            Some(inv) => self * inv,
            None => panic!("No modular inverse exists"),
        }
    }
}


//assignement operators
impl<const N: i32> AddAssign for Ring<N> {
    fn add_assign(&mut self, rhs: Self) {
        self.value = (self.value + rhs.value) % N;
        if self.value < 0 {
            self.value += N;
        }
    }
}

impl<const N: i32> SubAssign for Ring<N> {
    fn sub_assign(&mut self, rhs: Self) {
        self.value = (self.value - rhs.value) % N;
        if self.value < 0 {
            self.value += N;
        }
    }
}

impl<const N: i32> MulAssign for Ring<N> {
    fn mul_assign(&mut self, rhs: Self) {
        self.value = (self.value * rhs.value) % N;
        if self.value < 0 {
            self.value += N;
        }
    }
}

impl<const N: i32> DivAssign for Ring<N> {
    fn div_assign(&mut self, rhs: Self) {
        let inv = rhs.inverse().expect("No modular inverse exists");
        self.value = (self.value * inv.value) % N;
        if self.value < 0 {
            self.value += N;
        }
    }
}

//stream output

impl<const N: i32> fmt::Display for Ring<N> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.value)
    }
}