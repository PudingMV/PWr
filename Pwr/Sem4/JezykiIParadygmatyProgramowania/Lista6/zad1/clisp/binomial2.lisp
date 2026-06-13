(defun next-row (row)
  (cons 1
        (append
         (mapcar #'+ row (cdr row))
         '(1))))

(defun nth-row (n)
  (labels ((iter (i row)
             (if (= i n)
                 row
                 (iter (+ i 1)
                       (next-row row)))))
    (iter 0 '(1))))

(defun binomial2 (n k)
  (nth k (nth-row n)))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args)))
       (k (parse-integer (second args))))
  (format t "~a~%" (binomial2 n k)))