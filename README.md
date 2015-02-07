Parser
===

Original Grammar
---

```
STMTS -> STMTS STMT | STMT
STMT -> ASSIGN ;
STMT -> LISTEXPR ;
ASSIGN -> set VAR LISTEXPR
LISTEXPR -> VAR
LISTEXPR -> CONCAT
LISTEXPR -> ( ITEMS )
LISTEXPR -> cdr LISTEXPR
VAR -> char
CONCAT -> LISTEXPR + LISTEXPR
ITEMS -> ITEMS ITEM | ITEM
ITEM -> LISTEXPR
ITEM -> str | int | real
ITEM -> car LISTEXPR
ITEM ->
```

Revised Grammar
---

__NOTE__: e is the empty string, $ is the end of stream marker.

- Fixed associativity for LISTEXPR
  - Replaced all right hand side LISTEXPR with LISTELEM 
  - Removed CONCAT -> LISTEXPR + LISTEXPR rule
  - Added LISTEXPR -> LISTEXPR + LISTELEM | LISTELEM rule
  - Replaced ITEM -> car LISTEXPR rule with ITEM -> car LISTELEM
- Resolved ambiguity of both ITEM and ITEMS going to e (epsilon)
  - Removed ITEM -> e rule
  - Added ITEMS -> e rule
- Collapse into unique LHS rules with | separators
- Resolved left recursive ambiguity of STMTS production
  - Replaced STMTS -> STMTS + STMT | STMT rule with STMTS -> STMT STMTS'
  - Added STMTS' -> STMT STMTS' | e rule
- Resolved left recursive ambiguity of LISTEXPR production
  - Replaced LISTEXPR -> LISTEXPR + LISTELEM | LISTELEM rule with
    LISTEXPR -> LISTELEM LISTEXPR'
  - Added LISTEXPR' -> + LISTELEM LISTEXPR' | e rule

```
STMTS -> STMT STMTS'
STMTS' -> STMT STMTS' | e
STMT -> ASSIGN ; | LISTEXPR ;
ASSIGN -> set VAR LISTEXPR ;
LISTEXPR -> LISTELEM LISTEXPR'
LISTEXPR' -> + LISTELEM LISTEXPR' | e
LISTELEM -> VAR | ( ITEMS ) | cdr LISTELEM
VAR -> char
ITEMS -> ITEM ITEMS | e
ITEM -> car LISTEXPR | str | int | real
```
