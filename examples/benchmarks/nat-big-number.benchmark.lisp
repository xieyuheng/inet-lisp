(import zero add1 "../datatypes/nat.lisp")
(import square "../datatypes/nat-square.lisp")
(import double "../datatypes/nat-double.lisp")
(import nat-to-int "../datatypes/nat-to-int.lisp")

(define (two) (add1 (add1 (zero))))

(nat-to-int
 (double
  (double
   (double
    (double
     (square (square (square (square (two))))))))))
