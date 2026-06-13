(defun prime-factors (n)
  (labels ((aux (n d)
             (cond
               ((= n 1) nil)
               ((> (* d d) n) (list n))
               ((= (mod n d) 0)
                (cons d (aux (/ n d) d)))
               (t (aux n (+ d 1))))))
    (aux n 2)))

(let* ((args (cdr sb-ext:*posix-argv*))
       (n (parse-integer (first args))))
  (format t "~a~%" (prime-factors n)))