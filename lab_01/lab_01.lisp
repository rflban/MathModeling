;;;; F. Nabiev, IU7-63B, MathModeling, lab_01

;;; union all elemets of two lists
(defun unionall (l1 l2)
  (concatenate 'list l1 l2))

;;; fraction part of real number
(defun frac (num)
  (- num (truncate num)))

;;; factorial
(defun factorial (n)
  (if (>= 0 n)
    1
    (* n (factorial (- n 1)))))

;;; combination
(defun C (n k)
  (/ (factorial n)
     (* (factorial k)
        (factorial (- n k)))))

;;; integrate a monomial
;;; momonial: '(expt:real ratio1:real ratio2:real ...) =>
;;; => x^expt * (ratio1 * ratio2 * ...)
;;; expt > 0
(defun integrate_monomial (monomial)
  (cons (+ (car monomial) 1.0)
        (cons (/ 1.0 (+ (car monomial) 1.0))
              (cdr monomial)))

;;; integrate a polynomial
;;; polynimial: '(monomial1 monomial2 ...) => monomial1 + monomial2 + ...
;;; monomial: see *integrate_monomial*
(defun integrate_polynomial (polynomial)
  (loop for monomial in polynomial
    collect (integrate_monomial (eval monomial))))

;;; multiply two monomials
(defun mul_mm (monomial1 monomial2)
  (cons (+ (car monomial1) (car monomial2))
        (unionall (cdr monomial1) (cdr monomial2))))

;;; multiply monomial and polynomial
(defun mul_pm (polynomial monomial)
  (loop for monomial1 in polynomial
    collect (mul_mm (eval monomial1) monomial)))

;;; multiply two polynomials
(defun mul_pp (polynomial1 polynomial2)
  (loop with result = nil
        for monomial in polynomial2
    do (setq result (unionall result
                              (mul_pm polynomial1 monomial)))
    finally (return result)))

;;; power number: num^expn = ((expt num (- expn (truncate expn))) num num ...)
;;; num repeats (truncate expn) times
;;; expn:real >= 0
(defun power (num expn)
  (if (< 0 (frac expn))
    (cons (expt num (frac expn))
          (loop repeat (truncate expn)
            collect num))
    (loop repeat (truncate expn)
      collect num)))

;;; substitute monomial1 into monomial2
;;; monomial: see *integrate monomial*
;;; example: monomial1 = a1*x^e1; monomial2 = a2*x^e2
;;; result = a2*(a1*x^e1)^e2 = a1^e2 * a2 * x^(e1 * e2)
(defun subst_mm (monomial1 monomial2)
  (cons (* (car monomial1) (car monomial2))
        (unionall (cdr monomial2)
                  (loop with koefs = nil
                        for koef in (cdr monomial1)
                    do (setq koefs (unionall koefs
                                             (power koef
                                                    (car monomial2))))
                       finally (return koefs)))))

;;; substitute polynomial into monomial
(defun subst_pm (polynomial monomial)
  (loop for monomial1 in polynomial
    collect (subst_mm (eval monomial1) monomial)))

