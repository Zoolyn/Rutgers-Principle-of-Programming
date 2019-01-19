(load "utilFuncs.ss")

(define getStartSymbol
  (lambda (grammar)
    ;; YOUR CODE GOES HERE
    (car(car grammar))
  ))
(define getNonterminals
  (lambda (grammar)
    (if
     (null? grammar)
     '()
     (cons (getStartSymbol grammar) (getNonterminals (cdr grammar)))
     )
  ))
(define getProductions
  (lambda (grammar)
    ;; YOUR CODE GOES HERE
    (if
     (null? grammar)
     '()
    (append (map (lambda(x) (cons (car(getNonterminals grammar)) (cons x '()))) (cdr (assoc (car(getNonterminals grammar)) grammar)) ) (getProductions(cdr grammar)))

    )
  ))
(define updateAssocList
  (lambda (assocList symbol set)
    ;; YOUR CODE GOES HERE
    (cond
     ((null? assocList)'())
    ((eq? (car(car assocList)) symbol) (cons (cons symbol(cons (union (car(cdr (assoc (car(car assocList)) assocList))) set) '())) (updateAssocList (cdr assocList) symbol set)))
    (else (append (cons (assoc (car(car assocList)) assocList) (updateAssocList (cdr assocList) symbol set))))
    )
  ))
(define getInitSets
  (lambda (NTs startSymbol setting)
    ;; YOUR CODE GOES HERE
    (cond
      ((null? NTs) '())
      ((eq? setting 'first) (cons (cons (car NTs) '(()))  (getInitSets (cdr NTs) startSymbol setting))) ;;for the first
      ((eq? setting 'follow) (cons (cons (car NTs) '((eof)))  (getInitSets (cdr NTs) startSymbol 'first))) ;; for the follow
      (else #f)
      )
  ))
(define genFirstFunc
  (lambda (NTs)
    (letrec ((first
              (lambda (symbolSeq firstSets)
                ;; YOUR CODE GOES HERE
                ;(cons (car symbolSeq) '()) ;This is how you have to return something in order for it to not blow up
                (if
                 (contains? NTs (car symbolSeq)) ;;if it is a nonterminal
                 (car (cdr (assoc (car symbolSeq) firstSets)))
                 (cons (car symbolSeq) '())
                 )
              )))
      first)))

(define recurseFirstSets
  (lambda (rules firstSets firstFunc)
    (if (null? rules)
         ;; YOUR CODE GOES HERE.
         firstSets ;;we return the final first set maybe
        (let ((rhsRule (cadar rules))
              (lhsRule (caar rules)))
          ;; YOUR CODE GOES HERE
          (recurseFirstSets (cdr rules) (updateAssocList firstSets lhsRule (firstFunc rhsRule firstSets)) firstFunc)                  
        ))))

(define getFirstSets
  (lambda (rules firstSets firstFunc)
      (let ((newFirstSets (recurseFirstSets rules firstSets firstFunc)))
        (if (equal? firstSets newFirstSets) ;;If the FIRST sets have no change
            firstSets ;; then return the FIRST sets
            (getFirstSets rules newFirstSets firstFunc))))) ;; Otherwise, use the new FIRST sets and recurse

(define genFollowFunc
  (lambda (NTs)
    ;; YOUR CODE GOES HERE
    (letrec ((follow
              (lambda (symbolSeq followSets trailer firstSets)
                '(eof)
           )))
   follow)))

(define recurseFollowSets
  (lambda (rules followSets firstSets followFunc)
    (if (null? rules)
         ;; YOUR CODE GOES HERE. You may delete this pair of parentheses if necessary.
        followSets
        (let* ((rhsRule (cadar rules))
               (lhsRule (caar rules))
               (trailer (cadr (assoc lhsRule followSets))))
          ;; YOUR CODE GOES HERE
          (recurseFollowSets (cdr rules) (updateAssocList followSets lhsRule (followFunc (reverse rhsRule) followSets trailer firstSets)) firstSets followFunc)
        ))))

(define getFollowSets
  (lambda (rules followSets firstSets followFunc)
    ;; YOUR CODE GOES HERE
    (let ((newFollowSets (recurseFollowSets rules followSets firstSets followFunc)))
        (if (equal? followSets newFollowSets) ;;If the FOLLOW sets have no change
            followSets ;; then return the FOLLOW sets
            (getFollowSets rules newFollowSets firstSets followFunc))))) ;; Otherwise, use the new FOLLOW sets and recurse

 
;(cons (cons (list (caar rules) "::=" (cadar rules)) (cdr (assoc 'start firstSets))) '()) This is for getPredict
(define getPredictSets
  (lambda (rules NTs firstSets followSets firstFunc)
    (if (null? rules)
        ;; YOUR CODE GOES HERE
        '()
                (let* ((rhsRule (cadar rules))
               (lhsRule (caar rules)))
          (if
           (eq? (car rhsRule) (epsilon))
           (cons (cons (list lhsRule '::= rhsRule) (cdr(assoc lhsRule followSets)) ) (getPredictSets (cdr rules) NTs firstSets followSets firstFunc))
           (cons (cons (list lhsRule '::= rhsRule ) (cons (firstFunc rhsRule firstSets) '())) (getPredictSets (cdr rules) NTs firstSets followSets firstFunc))
          )
     )   
    )))



(define grammar3 '((start (stmts))
                   (stmts (assgn morestmts))
                   (morestmts ("," stmts) ("eps"))
                   (assgn (var "=" value))
                   (var (a) (b) (c) (d) (e))
                   (value (0) (1) (2) (3) (4) (5) (6) (7) (8) (9))))
(define grammar2 '((start (expr)) (expr ("+" term term)) (term (a) (b) (c))))
(define grammar1 '((a (x b) ("eps")) (b (y a) ("eps"))))
(define grammar0 '((start (a))))
(define assocList
  '(
    (a (1 2 3))
    (b (2 3 1))
    (c (4 5 6))
    )
  )
(define set '(8 9 10))
(define sampleFollow '(( start ( eof ))( stmts ( eof ))( morestmts ( eof ))( assgn (" ," eof ))( var ("="))( value (" ," eof ))))
(define sampleFollow2
'(( a ( eof ))
( b ( eof ))))

;(getStartSymbol grammar2)
;(getNonterminals grammar2)
;(getProductions grammar3)
;(updateAssocList assocList 'a set)
;(union (car(cdr (assoc (car(car assocList)) assocList))) set)
;(getInitSets (getNonterminals grammar3) (getStartSymbol grammar3) 'follow)
;(updateAssocList (getInitSets (getNonterminals grammar2) (getStartSymbol grammar2) 'first) 'expr set)
;(recurseFirstSets (getProductions grammar3) (getInitSets (getNonterminals grammar3) (getStartSymbol grammar3) 'first) 'test)
;(genFirstFunc (getNonterminals grammar1))
;(getInitSets (getNonterminals grammar1) (getStartSymbol grammar1) 'first)
;(recurseFirstSets (getProductions grammar1) (getInitSets (getNonterminals grammar1) (getStartSymbol grammar1) 'first) (genFirstFunc (getNonterminals grammar1)))
; ((genFirstFunc (getNonterminals grammar1))(epsilon)(getInitSets (getNonterminals grammar2) (getStartSymbol grammar2) 'first))
;(getFirstSets (getProductions grammar2) (getInitSets (getNonterminals grammar2) (getStartSymbol grammar2) 'first) (genFirstFunc(getNonterminals grammar2)) )
(define grammar grammar1)
(define NTs (getNonterminals grammar))
(define rules (getProductions grammar))
(define startSymbol (getstartSymbol grammar))
(define firstFunc (genFirstFunc NTs))
(define followFunc (genFollowFunc NTs))
(define firstSets (getFirstSets rules (getInitSets NTs startSymbol 'first) firstFunc))
(define followSets (getFollowSets rules (getInitSets NTs startSymbol 'follow) firstSets followFunc))
(define predictSets (getPredictSets rules NTs firstSets sampleFollow2 firstFunc))
followSets
;firstSets
;predictSets
