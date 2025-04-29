(import zero add1 "nat.lisp")
(import nat-to-int "nat-to-int.lisp")

(nat-to-int (zero))
(nat-to-int (add1 (zero)))
(nat-to-int (add1 (add1 (zero))))
(nat-to-int (add1 (add1 (add1 (zero)))))
