(import add nat-dup "nat.lisp")

(define (double x)
  (= x x* (nat-dup x))
  (add x x*))
