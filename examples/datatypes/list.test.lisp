(import sole "trivial.lisp")
(import null cons append "list.lisp")

(inspect-run
 (append
  (cons (sole) (cons (sole) (cons (sole) (null))))
  (cons (sole) (cons (sole) (cons (sole) (null))))))
