;; "interaction combinators", 1997.
;; 1.4. example: cellular automata

(define-node o r! l)
(define-node x r! l)
(define-node o^ r l!)
(define-node x^ r l!)

(define-node oo r! l)
(define-node ox r! l)
(define-node xo r! l)
(define-node xx r! l)

(define-node oo^ r l!)
(define-node ox^ r l!)
(define-node xo^ r l!)
(define-node xx^ r l!)

;; l-|p>-<q^|-r => l-<pq^|-|pq>-r
;; (define-rule (p (q^ r) l) (pq^ (pq r) l))

(define-rule (o (o^ r) l) (oo^ (oo r) l))
(define-rule (x (o^ r) l) (xo^ (xo r) l))
(define-rule (o (x^ r) l) (ox^ (ox r) l))
(define-rule (x (x^ r) l) (xx^ (xx r) l))

;; l-|pq>-<rs^|-r => l-<q'^|-|r'>-r
;; (define-rule (pq (rs^ r) l) (q'^ (r' r) l))

;; | x x x | x x o | x o x | x o o | o x x | o x o | o o x | o o o |
;; |   o   |   x   |   x   |   o   |   x   |   x   |   x   |   o   |

;; x x x x
;;   o o
(define-rule (xx (xx^ r) l) (o^ (o r) l))

;; x x x o
;;   o x
(define-rule (xx (xo^ r) l) (o^ (x r) l))

;; x x o x
;;   x x
(define-rule (xx (ox^ r) l) (x^ (x r) l))

;; x x o o
;;   x o
(define-rule (xx (oo^ r) l) (x^ (o r) l))

;; x o x x
;;   x x
(define-rule (xo (xx^ r) l) (x^ (x r) l))

;; x o x o
;;   x x
(define-rule (xo (xo^ r) l) (x^ (x r) l))

;; x o o x
;;   o x
(define-rule (xo (ox^ r) l) (o^ (x r) l))

;; x o o o
;;   o o
(define-rule (xo (oo^ r) l) (o^ (o r) l))

;; o x x x
;;   x o
(define-rule (ox (xx^ r) l) (x^ (o r) l))

;; o x x o
;;   x x
(define-rule (ox (xo^ r) l) (x^ (x r) l))

;; o x o x
;;   x x
(define-rule (ox (ox^ r) l) (x^ (x r) l))

;; o x o o
;;   x o
(define-rule (ox (oo^ r) l) (x^ (o r) l))

;; o o x x
;;   x x
(define-rule (oo (xx^ r) l) (x^ (x r) l))

;; o o x o
;;   x x
(define-rule (oo (xo^ r) l) (x^ (x r) l))

;; o o o x
;;   o x
(define-rule (oo (ox^ r) l) (o^ (x r) l))

;; o o o o
;;   o o
(define-rule (oo (oo^ r) l) (o^ (o r) l))
