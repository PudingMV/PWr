import System.Environment (getArgs)

gcd' :: Integer -> Integer -> Integer
gcd' a 0 = a
gcd' a b = gcd' b (a `mod` b)

coprime :: Integer -> Integer -> Bool
coprime a b = gcd' a b == 1

totient :: Integer -> Integer
totient n = toInteger (length [k | k <- [1..n], coprime k n])

main :: IO ()
main = do
    args <- getArgs
    let n = read (head args)
    print (totient n)