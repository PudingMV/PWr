(defun is-prime (n d)
  (cond
    ((> (* d d) n) t)
    ((= (mod n d) 0) nil)
    (t (is-prime n (+ d 1)))))

(defun prime-p (n)
  (and (> n 1)
       (is-prime n 2)))

(defun primes-helper (i n)
  (cond
    ((> i n) nil)
    ((prime-p i)
     (cons i (primes-helper (+ i 1) n)))
    (t (primes-helper (+ i 1) n))))

(defun primes (n)
  (primes-helper 2 n))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args))))
  (format t "~a~%" (primes n)))