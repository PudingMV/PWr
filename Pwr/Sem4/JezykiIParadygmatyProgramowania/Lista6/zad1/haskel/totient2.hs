import System.Environment (getArgs)

primeFactors :: Integer -> [Integer]
primeFactors n = factor n 2
  where
    factor 1 _ = []
    factor n f
        | f * f > n      = [n]
        | n `mod` f == 0 = f : factor (n `div` f) f
        | otherwise      = factor n (f + 1)

unique :: [Integer] -> [Integer]
unique [] = []
unique (x:xs) = x : unique (filter (/= x) xs)

totient2 :: Integer -> Integer
totient2 n =
    foldl (\acc p -> acc * (p - 1) `div` p) n (unique (primeFactors n))

main :: IO ()
main = do
    args <- getArgs
    print (totient2 (read (head args)))