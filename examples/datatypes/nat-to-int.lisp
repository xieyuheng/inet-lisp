(import zero add1 "nat.lisp")

(define-node nat-to-int target! result)

(define-rule (nat-to-int (zero) result)
  (connect result 0))

(define-rule (nat-to-int (add1 prev) result)
  (iadd 1 (nat-to-int prev) result))
