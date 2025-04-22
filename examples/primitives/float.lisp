(assert (float? 0.1))
(assert (float? 0.0))
(assert (float? -0.1))

(assert (not (float? 1)))
(assert (not (float? 0)))
(assert (not (float? -1)))

(assert (eq? 0.1 0.1))
(assert (not (eq? 0.1 0.2)))
(assert (not (eq? 0.1 -0.1)))
(assert (eq? -0.1 -0.1))

(assert (eq? (fadd 0.1 0.1) 0.2))
(assert (eq? (fadd 0.2 0.2) 0.4))
(assert (eq? (fadd 0.1 -0.1) 0.0))

(assert (eq? (fdiv 0.1 0.1) 1.0))
(assert (eq? (fdiv 0.5 0.2) 2.5))
(assert (eq? (fdiv 5.0 2.0) 2.5))

(assert (eq? (float-to-int 1.0) 1))
