Parser
===

Running
---

- Compile with `make`, `make parselist`, or `make all`.
- Run using `./parselist`
- Reads from stdin and writes to stdout

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
  - Replaced STMTS -> STMTS + STMT | STMT rule with STMTS -> STMT STMTS2
  - Added STMTS2 -> STMT STMTS2 | e rule
- Resolved left recursive ambiguity of LISTEXPR production
  - Replaced LISTEXPR -> LISTEXPR + LISTELEM | LISTELEM rule with
    LISTEXPR -> LISTELEM LISTEXPR2
  - Added LISTEXPR2 -> + LISTELEM LISTEXPR2 | e rule

```
STMTS -> STMT STMTS2
STMTS2 -> STMT STMTS2 | e
STMT -> ASSIGN ; | LISTEXPR ;
ASSIGN -> set VAR LISTEXPR
LISTEXPR -> LISTELEM LISTEXPR2
LISTEXPR2 -> + LISTELEM LISTEXPR2 | e
LISTELEM -> VAR | ( ITEMS ) | cdr LISTELEM
VAR -> char
ITEMS -> ITEM ITEMS | e
ITEM -> car LISTEXPR | LISTEXPR | str | int | real
```

First and Follow Sets
---

| Token     | First                         | Follow                          |
|-----------|-------------------------------|---------------------------------|
| STMTS     | ( cdr char set                | $                               |
| STMTS2    | ( cdr char set e              | $                               |
| STMT      | ( cdr char set                | ( cdr char set                  |
| ASSIGN    | set                           | ;                               |
| LISTEXPR  | ( cdr char                    | ; ( ) car cdr char int real str |
| LISTEXPR2 | + e                           | ; ( ) car cdr char int real str |
| LISTELEM  | ( cdr char                    | + ;                             |
| VAR       | char                          | + ; ( cdr char                  |
| ITEMS     | ( car cdr char e int real str | )                               |
| ITEM      | ( car cdr char int real str   | ( car cdr char int real str     |
| (         | (                             |                                 |
| )         | )                             |                                 |
| +         | +                             |                                 |
| ;         | ;                             |                                 |
| car       | car                           |                                 |
| cdr       | cdr                           |                                 |
| char      | char                          |                                 |
| int       | int                           |                                 |
| real      | real                          |                                 |
| set       | set                           |                                 |
| str       | str                           |                                 |
