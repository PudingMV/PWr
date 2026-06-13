import System.Environment (getArgs)

nextRow :: [Integer] -> [Integer]
nextRow xs = zipWith (+) (0:xs) (xs ++ [0])

pascal :: [[Integer]]
pascal = iterate nextRow [1]

binomial2 :: Int -> Int -> Integer
binomial2 n k = (pascal !! n) !! k

main :: IO ()
main = do
    args <- getArgs
    let n = read (args !! 0) :: Int
    let k = read (args !! 1) :: Int
    print (binomial2 n k)