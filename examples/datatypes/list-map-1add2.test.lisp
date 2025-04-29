(import null cons "list.lisp")
(import list-map "list-map.lisp")

(define (iadd2 n)
  (iadd n 2))

(define (three-more-0s l)
  (cons 0 (cons 0 (cons 0 l))))

(list-map
 (three-more-0s
  (three-more-0s
   (three-more-0s (null))))
 iadd2)
