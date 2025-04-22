(import o x o^ x^ "rule-110.lisp")

;; o x o x x o (in a circle)

(define (main)
  (= l r (link))
  (connect l (o^ (x (o^ (x (x^ (o r))))))))

(main)
