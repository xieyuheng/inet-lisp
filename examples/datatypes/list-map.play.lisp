(import null cons "list.lisp")
(import list-map "list-map.lisp")
(import sole "trivial.lisp")

(define (iadd2 n)
  (iadd n 2))

(define (three-more-0s l)
  (cons 0 (cons 0 (cons 0 l))))

(cons (sole) (null))

(null)
(cons 0 (null))
(cons 0 (cons 0 (null)))
(cons 0 (cons 0 (cons 0 (null))))

(list-map
  (three-more-0s (null))
  iadd2)

(list-map
  (three-more-0s
   (three-more-0s
    (three-more-0s (null))))
  iadd2)
