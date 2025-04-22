(define-node t value!)
(define-node f value!)

(define-node bool-erase target!)

(define-rule (bool-erase (t)))
(define-rule (bool-erase (f)))

(define-node conj x! y z)
(define-node conj-t y! z)

(define-rule (conj (t) y z) (conj-t y z))
(define-rule (conj (f) y z) (bool-erase y) (f z))

(define-rule (conj-t (t) z) (t z))
(define-rule (conj-t (f) z) (f z))

(define-node disj x! y z)
(define-node disj-f y! z)

(define-rule (disj (t) y z) (bool-erase y) (t z))
(define-rule (disj (f) y z) (disj-f y z))

(define-rule (disj-f (t) z) (t z))
(define-rule (disj-f (f) z) (f z))
