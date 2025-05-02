(import zero add1 "../datatypes/nat.lisp")
(import square "../datatypes/nat-square.lisp")
(import nat-to-int "../datatypes/nat-to-int.lisp")

(define (two) (add1 (add1 (zero))))

(nat-to-int (square (square (square (square (two))))))
