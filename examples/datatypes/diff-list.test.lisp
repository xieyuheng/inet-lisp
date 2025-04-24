(import sole "trivial.lisp")
(import diff diff-append "diff-list.lisp")
(import cons "list.lisp")

(define (sole-diff-list)
  (= front front-op (link))
  (= back value (diff front))
  (cons (sole) (cons (sole) back) front-op)
  value)

(diff-append
 (sole-diff-list)
 (sole-diff-list))
