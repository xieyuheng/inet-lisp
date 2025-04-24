(import zero add1 add mul nat-max "nat.lisp")
(import one two three four "nat.lisp")

(define (sixteen) (mul (four) (four)))

(mul (sixteen) (sixteen))
