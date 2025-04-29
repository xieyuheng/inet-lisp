(define-node tak x! y! z result)

(define-rule (tak (int? x) (int? y) z result)
  (if (int-lt? y x)
    (tak (tak (isub x 1) y z)
         (tak (isub y 1) z x)
         (tak (isub z 1) x y)
         result)
    (begin
      (int-drop x)
      (int-drop y)
      (connect z result))))
