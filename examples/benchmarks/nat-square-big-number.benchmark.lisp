(import four "../datatypes/nat.lisp")
(import square "../datatypes/nat-square.lisp")
(import nat-to-int "../datatypes/nat-to-int.lisp")

(define (sixteen) (square (four)))

(nat-to-int (square (square (sixteen))))
