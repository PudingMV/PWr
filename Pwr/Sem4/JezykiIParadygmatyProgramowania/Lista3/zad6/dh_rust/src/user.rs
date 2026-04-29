use crate::{dhsetup::DHSetup, ring::Ring};
use rand::Rng;

pub struct User<'a, const N: u64> {
    secret: u64,
    key: Option<Ring<N>>,
    setup: &'a DHSetup<Ring<N>>,
}

impl<'a, const N: u64> User<'a, N> {
    fn random_secret() -> u64 {
        let mut rng = rand::thread_rng();
        rng.gen_range(2..(N - 2))
    }

    pub fn new(setup: &'a DHSetup<Ring<N>>) -> Self {
        Self {
            secret: Self::random_secret(),
            key: None,
            setup,
        }
    }

    pub fn get_public_key(&self) -> Ring<N> {
        self.setup.power(self.setup.get_generator(), self.secret)
    }

    pub fn set_key(&mut self, a: Ring<N>) {
        self.key = Some(self.setup.power(a, self.secret));
    }

    pub fn encrypt(&self, m: Ring<N>) -> Ring<N> {
        let k = self.key.expect("no key");
        m * k
    }

    pub fn decrypt(&self, c: Ring<N>) -> Ring<N> {
        let k = self.key.expect("no key");
        c / k
    }
}