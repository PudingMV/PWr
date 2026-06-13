import System.Environment (getArgs)

egcd :: Integer -> Integer -> (Integer, Integer, Integer)
egcd a 0 = (1, 0, a)
egcd a b =
    let (x1, y1, g) = egcd b (a `mod` b)
        x = y1
        y = x1 - (a `div` b) * y1
    in (x, y, g)

main :: IO ()
main = do
    args <- getArgs
    let a = read (args !! 0)
    let b = read (args !! 1)
    print (egcd a b)