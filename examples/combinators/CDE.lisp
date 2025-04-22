;; C: construct
;; D: duplicate
;; E: erase

(define-node C car cdr value!)
(define-node D target! first second)
(define-node E target!)

(define-rule*
    ((C x-car x-cdr value!)
     (C y-car y-cdr value!))
  (connect x-car y-cdr)
  (connect x-cdr y-car))

(define-rule*
    ((D target! x-first x-second)
     (D target! y-first y-second))
  (connect x-first y-first)
  (connect x-second y-second))

(define-rule (E (E)))

(define-rule (E (C car cdr))
  (E car)
  (E cdr))

(define-rule (D (E) first second)
  (E first)
  (E second))

(define-rule (D (C car cdr) first second)
  (= car-first car-second (D car))
  (= cdr-first cdr-second (D cdr))
  (C car-first cdr-first first)
  (C car-second cdr-second second))
