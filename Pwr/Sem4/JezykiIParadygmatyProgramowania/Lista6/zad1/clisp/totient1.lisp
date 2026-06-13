(defun gcd- (a b)
  (if (= b 0)
      a
      (gcd- b (mod a b))))

(defun coprime (a b)
  (= (gcd- a b) 1))

(defun totient (n &optional (i 1))
  (if (> i n)
      0
      (+ (if (coprime i n) 1 0)
         (totient n (+ i 1)))))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args))))
  (format t "~a~%" (totient n)))