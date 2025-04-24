(import sole "trivial.lisp")
(import null cons append "list.lisp")

(append
 (cons (sole) (cons (sole) (cons (sole) (null))))
 (cons (sole) (cons (sole) (cons (sole) (null)))))
