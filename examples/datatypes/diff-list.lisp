(define-node diff front back value!)
(define-node diff-append target! rest result)
(define-node diff-open new-back target! old-back)

;; plug the front of the `target` to the back of `rest`.

(define-rule (diff-append (diff front back) rest result)
  (= new-back value (diff front))
  (connect value result)
  (diff-open new-back rest back))

;; with keyword argument:
;; (define-rule (diff-append (diff front back) rest result)
;;   (diff-open (diff front :value result) rest back))

(define-rule (diff-open new-back (diff front back) old-back)
  (connect back new-back)
  (connect front old-back))
