(import null cons "list.lisp")

(define-node list-map target! fn result)

(define-rule (list-map (null) fn result)
  (null result))

(define-rule (list-map (cons head tail) fn result)
  (= fn fn* (fn-dup fn))
  (cons (apply1 fn head) (list-map tail fn*) result))
