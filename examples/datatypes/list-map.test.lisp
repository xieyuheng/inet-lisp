(import zero add1 "nat.lisp")
(import null cons "list.lisp")
(import list-map "list-map.lisp")

(define (add2 n)
  (add1 (add1 n)))

(define (three-more-zeros l)
  (cons (zero) (cons (zero) (cons (zero) l))))

(inspect-run
 (list-map
  (three-more-zeros
   (three-more-zeros
    (three-more-zeros (null))))
  add2))

(define (iadd2 n)
  (iadd n 2))

(define (three-more-0s l)
  (cons 0 (cons 0 (cons 0 l))))

(inspect-run
 (list-map
  (three-more-0s
   (three-more-0s
    (three-more-0s (null))))
  iadd2))
