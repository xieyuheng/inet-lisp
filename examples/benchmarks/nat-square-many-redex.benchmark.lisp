(import zero add1 add "../datatypes/nat.lisp")
(import square "../datatypes/nat-square.lisp")
(import nat-to-int "../datatypes/nat-to-int.lisp")

(define (two) (add1 (add1 (zero))))

(define (n256) (square (square (square (two)))))

(nat-to-int
 (add (n256)
      (add (n256)
           (add (n256)
                (add (n256)
                     (add (n256)
                          (add (n256)
                               (n256))))))))
