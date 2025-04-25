(import t f conj disj "bool.lisp")

(disj
 (conj (conj (t) (t))
       (conj (f) (t)))
 (disj (disj (t) (t))
       (disj (f) (t))))
