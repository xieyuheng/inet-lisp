(assert (int? 1))
(assert (int? 0))
(assert (int? -1))

(assert (not (int? 0.1)))
(assert (not (int? 0.0)))
(assert (not (int? -0.1)))

(assert (eq? 1 1))
(assert (not (eq? 1 2)))
(assert (not (eq? 1 -1)))
(assert (eq? -1 -1))

(assert (eq? (iadd 1 1) 2))
(assert (eq? (iadd 2 2) 4))
(assert (eq? (iadd 1 -1) 0))

(assert (eq? (imul 1 1) 1))
(assert (eq? (imul 2 2) 4))
(assert (eq? (imul 2 -1) -2))

(assert (eq? (idiv 1 1) 1))
(assert (eq? (idiv 5 2) 2))
(assert (eq? (imod 5 2) 1))

(assert (eq? (int-to-float 1) 1.0))
