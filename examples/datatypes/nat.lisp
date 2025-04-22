(define-node zero value!)
(define-node add1 prev value!)
(define-node add target! addend result)

(define-rule (add (zero) addend result)
  (connect addend result))

(define-rule (add (add1 prev) addend result)
  (add1 (add prev addend) result))

(define (one) (add1 (zero)))
(define (two) (add1 (one)))
(define (three) (add1 (two)))
(define (four) (add1 (three)))

;; to define `mul`, we first need `nat-erase` and `nat-dup`

(define-node nat-erase target!)

(define-rule (nat-erase (zero)))
(define-rule (nat-erase (add1 prev)) (nat-erase prev))

(define-node nat-dup target! first second)

(define-rule (nat-dup (zero) first second)
  (connect first (zero))
  (connect second (zero)))

(define-rule (nat-dup (add1 prev) first second)
  (= prev-first prev-second (nat-dup prev))
  (connect first (add1 prev-first))
  (connect second (add1 prev-second)))

(define-node mul target! mulend result)

(define-rule (mul (zero) mulend result)
  (nat-erase mulend)
  (zero result))

(define-rule (mul (add1 prev) mulend result)
  (= mulend-first mulend-second (nat-dup mulend))
  (add (mul mulend-second prev)
       mulend-first
       result))

;; to define `nat-max`, we need `nat-max-add1`

(define-node nat-max first! second result)
(define-node nat-max-add1 first second! result)

(define-rule (nat-max (zero) second result)
  (connect second result))

(define-rule (nat-max (add1 prev) second result)
  (nat-max-add1 prev second result))

(define-rule (nat-max-add1 first (zero) result)
  (add1 first result))

(define-rule (nat-max-add1 first (add1 prev) result)
  (add1 (nat-max first prev) result))
