(define-node t value!)
(define-node f value!)

(define-node conj x! y! z)

(define-rule (conj (t) (t) z) (t z))
(define-rule (conj (t) (f) z) (f z))
(define-rule (conj (f) (t) z) (f z))
(define-rule (conj (f) (f) z) (f z))

(define-node disj x! y! z)

(define-rule (disj (t) (t) z) (t z))
(define-rule (disj (t) (f) z) (t z))
(define-rule (disj (f) (t) z) (t z))
(define-rule (disj (f) (f) z) (f z))
