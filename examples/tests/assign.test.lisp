(define (main)
  (= x 1)
  (assert (eq? x 1))

  (= x 2)
  (assert (eq? x 2))

  (assign x 3)
  (assert (eq? x 3)))

(main)
