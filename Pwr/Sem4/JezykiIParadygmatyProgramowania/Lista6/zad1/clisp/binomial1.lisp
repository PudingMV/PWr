(defun binomial (n k)
  (if (or (= k 0) (= k n))
      1
      (+ (binomial (- n 1) k)
         (binomial (- n 1) (- k 1)))))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args)))
       (k (parse-integer (second args))))
  (format t "~a~%" (binomial n k)))