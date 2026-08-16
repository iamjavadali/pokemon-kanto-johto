@ Restore the stock comparison helpers after a Golden Yellow rival map has been
@ assembled so unrelated maps retain normal engine semantics.
.purgem call_if_eq
.macro call_if_eq a:req, b, c
    trycompare call_if, EQUAL, \a, \b, \c
.endm

.purgem goto_if_eq
.macro goto_if_eq a:req, b, c
    trycompare goto_if, EQUAL, \a, \b, \c
.endm
