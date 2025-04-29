(import four "nat.lisp")
(import square "nat-square.lisp")
(import nat-to-int "nat-to-int.lisp")

(define (sixteen) (square (four)))

(nat-to-int (square (sixteen)))
