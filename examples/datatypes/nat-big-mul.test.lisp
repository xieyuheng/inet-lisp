(import zero add1 add mul nat-max "nat.lisp")
(import one two three four "nat.lisp")
(import nat-to-int "nat-to-int.lisp")

(define (sixteen) (mul (four) (four)))

(nat-to-int (mul (sixteen) (sixteen)))
