(import mul nat-dup "nat.lisp")

(define (square x)
  (= x x* (nat-dup x))
  (mul x x*))
