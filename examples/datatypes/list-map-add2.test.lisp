(import zero add1 "nat.lisp")
(import null cons "list.lisp")
(import list-map "list-map.lisp")

(define (add2 n)
  (add1 (add1 n)))

(define (three-more-zeros l)
  (cons (zero) (cons (zero) (cons (zero) l))))

(list-map
 (three-more-zeros
  (three-more-zeros
   (three-more-zeros (null))))
 add2)
