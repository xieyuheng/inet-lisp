(import four "../examples/datatypes/nat.lisp")
(import square "../examples/datatypes/nat-square.lisp")
(import nat-to-int "../examples/datatypes/nat-to-int.lisp")

(define (sixteen) (square (four)))

(nat-to-int (square (square (sixteen))))
