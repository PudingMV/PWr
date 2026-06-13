(defun merge-lists (a b)
  (cond
    ((null a) b)
    ((null b) a)
    ((<= (car a) (car b))
     (cons (car a) (merge-lists (cdr a) b)))
    (t
     (cons (car b) (merge-lists a (cdr b))))))

(defun split-list (lst)
  (labels ((split (i left right xs)
             (if (null xs)
                 (list (reverse left) (reverse right))
                 (if (evenp i)
                     (split (1+ i) (cons (car xs) left) right (cdr xs))
                     (split (1+ i) left (cons (car xs) right) (cdr xs))))))
    (split 0 '() '() lst)))

(defun merge-sort (lst)
  (if (or (null lst) (null (cdr lst)))
      lst
      (let* ((parts (split-list lst))
             (left (first parts))
             (right (second parts)))
        (merge-lists (merge-sort left)
                     (merge-sort right)))))

(let* ((args (cdr sb-ext:*posix-argv*))
       (lst (mapcar #'parse-integer args)))
  (format t "~{~a ~}~%" (merge-sort lst)))