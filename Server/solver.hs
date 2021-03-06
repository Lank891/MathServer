module Rpn where

import Data.List
import Text.Read
import Foreign.C.String

solveRPN :: String -> Either [Float] String
solveRPN = foldl ff (Left []) . words
    where ff (Right x) _         = Right x
          ff (Left (x:y:ys)) "+" = Left( (y + x):ys )
          ff (Left (x:y:ys)) "-" = Left( (y - x):ys )
          ff (Left (x:y:ys)) "*" = Left( (y * x):ys )
          ff (Left (x:y:ys)) "/" = if x == 0 then Right "Division by 0" else Left( (y / x):ys )
          ff (Left (x:y:ys)) "^" = Left( (y ** x):ys )
          ff (Left (x:ys)) "ln"  = if x <= 0 then Right "Invalid Logarithm (ln)" else Left( log x:ys )
          ff (Left (x:ys)) "log" = if x <= 0 then Right "Invalid Logarithm (log)" else Left( logBase 10 x:ys )
          ff (Left xs) number    = case readMaybe number of
                                    Just num  -> Left(num:xs)
                                    Nothing -> Right ("Invalid Symbol: " ++ show number)

headToString :: (Show a) => [a] -> String
headToString []     = "Empty"
headToString (x:xs) = show x

returnAnswer :: String -> String
returnAnswer x = case solveRPN x of
                    Left n -> headToString n
                    Right n -> n

rpn :: CString -> IO CString
rpn x = do k <- peekCString x
           newCString $ returnAnswer k

foreign export ccall rpn :: CString -> IO CString