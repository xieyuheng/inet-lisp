;; suppose we have multiple principal ports.

(define-node tak x! y! z result)
(define-node tak-if p! x y z result)

;; suppose we have typed rule, and auto dup/drop.

(define-record int-i
  :is int?
  :dup int-dup
  :drop int-drop)

(define-rule (tak (is x int-i) (is y int-i) z result)
  (tak-if (int-lt? y x) x y z result))

;; suppose we have builtin const `true` and `false`.

(define-rule (tak-if true x y z result)
  (tak (tak (isub x 1) y z)
       (tak (isub y 1) z x)
       (tak (isub z 1) x y)
       result))

(define-rule (tak-if false x y z result)
  (connect result z))

;; inline `tak-if`:

(define-rule (tak (is x int-i) (is y int-i) z result)
  (if (int-lt? y x)
    (tak (tak (isub x 1) y z)
         (tak (isub y 1) z x)
         (tak (isub z 1) x y)
         result)
    (connect result z)))
