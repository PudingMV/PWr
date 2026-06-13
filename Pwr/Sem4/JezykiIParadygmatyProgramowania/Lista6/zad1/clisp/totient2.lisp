(defun gcd- (a b)
  (if (= b 0)
      a
      (gcd- b (mod a b))))

(defun prime-factors (n d)
  (cond
    ((= n 1) nil)
    ((> (* d d) n) (list n))
    ((= (mod n d) 0)
     (cons d (prime-factors (/ n d) d)))
    (t (prime-factors n (+ d 1)))))

(defun totient2 (n)
  (let ((factors (remove-duplicates (prime-factors n 2) :test #'=)))
    (reduce (lambda (acc p)
              (* acc (/ (- p 1) p)))
            factors
            :initial-value n)))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args))))
  (format t "~a~%" (totient2 n)))