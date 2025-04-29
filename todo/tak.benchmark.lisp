(define-node tak x! y z result)
(define-node tak-int x y! z result)
(define-node tak-if p! x y z result)

(define-rule (tak (int? x) y z result)
  (tak-int x y z result))

(define-rule (tak-int x (int? y) z result)
  (tak-if (int-lt? y x) x y z result))

(define-rule (tak-if (bool? p) x y z result)
  (if p
    (tak (tak (isub x 1) y z)
         (tak (isub y 1) z x)
         (tak (isub z 1) x y)
         result)
    (begin
      (int-drop x)
      (int-drop y)
      (connect z result))))
