(set-logic NIA)

(set-option :produce-models true)
(set-option :incremental true)

; Deklarace promennych pro vstupy
; ===============================

; Parametry
(declare-fun A () Int)
(declare-fun B () Int)
(declare-fun C () Int)
(declare-fun D () Int)
(declare-fun E () Int)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;; START OF SOLUTION ;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Deklarace proměnných x, y, z
(declare-fun x () Int)
(declare-fun y () Int)
(declare-fun z () Int)

;opsané z zadánní
(assert 
    (and 
        (> D 0)
        (> E 0)
        (= x (* A B 2))
        (ite(< x E)
            (= y (+ x (* 5 B)))
            (= y (- x C))

        )
        (ite(< (+ y 2) D)
            (= z (- (* x A) (* y B)))
            (= z (+ (* x B) (* y A)))
        )
        (< z (+ E D))
    )
)

(declare-fun x2 () Int)
(declare-fun y2 () Int)
(declare-fun z2 () Int)
(declare-fun E2 () Int)
(declare-fun D2 () Int)


;druhá podmínka - logicky správně
(assert 
        (forall ((E2 Int) (D2 Int) (x2 Int) (y2 Int) (z2 Int))
            (=>
                (and 
                    (> D2 0)
                    (> E2 0)
                    (= x2 (* A B 2))
                    (ite(< x2 E2)
                        (= y2 (+ x2 (* 5 B)))
                        (= y2 (- x2 C))
                    )
                    (ite(< (+ y2 2) D2)
                        (= z2 (- (* x2 A) (* y2 B)))
                        (= z2 (+ (* x2 B) (* y2 A)))
                    )
                    (< z2 (+ E2 D2))
                    
                )

                (>= (+ E2 D2) (+ E D))
                )
        )
)





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;; END OF SOLUTION ;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Testovaci vstupy
; ================

(echo "Test 1 - vstup A=1, B=1, C=3")
(echo "a) Ocekavany vystup je sat a D+E se rovna 2")
(check-sat-assuming (
  (= A 1) (= B 1) (= C 3)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 2, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 1) (= B 1) (= C 3) (distinct (+ D E) 2)
))


(echo "Test 2 - vstup A=5, B=2, C=5")
(echo "a) Ocekavany vystup je sat a D+E se rovna 54")
(check-sat-assuming (
  (= A 5) (= B 2) (= C 5)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 54, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 5) (= B 2) (= C 5) (distinct (+ D E) 54)
))

(echo "Test 3 - vstup A=100, B=15, C=1")
(echo "a) Ocekavany vystup je sat a D+E se rovna 253876")
(check-sat-assuming (
  (= A 100) (= B 15) (= C 1)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 253876, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 100) (= B 15) (= C 1) (distinct (+ D E) 253876)
))

(echo "Test 4 - vstup A=5, B=5, C=3")
(echo "a) Ocekavany vystup je sat a D+E se rovna 51")
(check-sat-assuming (
  (= A 5) (= B 5) (= C 3)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 51, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 5) (= B 5) (= C 3) (distinct (+ D E) 51)
))

(echo "Test 5 - vstup A=2, B=1, C=2")
(echo "a) Ocekavany vystup je sat a D+E se rovna 7")
(check-sat-assuming (
  (= A 2) (= B 1) (= C 2)
))
(get-value (D E (+ D E)))
(echo "b) Neexistuje jine reseni nez 7, ocekavany vystup je unsat")
(check-sat-assuming (
  (= A 2) (= B 1) (= C 2) (distinct (+ D E) 7)
))
