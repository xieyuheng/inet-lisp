(import add mul nat-max "nat.lisp")
(import one two "nat.lisp")

(inspect-run
 (add (add (two) (two))
      (add (two) (two))))

(inspect-run (mul (two) (two)))

(inspect-run (nat-max (one) (two)))
