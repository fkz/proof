
%stype ElementPtr

%token ZUWEISUNG LITERAL PRINT DOUBLE_ARROW ARROW

%start definitions

//%debug
//%lines

%%

definitions: 
|
| definitions definition
| definitions print
;

print: PRINT aussage ';' { Creater::print($2); }
;

definition: LITERAL ZUWEISUNG aussage ';' {  }
;

simple_aussage: '(' aussage ')' { $$ = $2; }
| LITERAL { $$ = $1; }
;

aussage: simple_aussage ARROW aussage { $$ = Creater::forAll ($1, $3->step(0)); } 
|        LITERAL ':' simple_aussage ARROW aussage { std::string name = literal($1); $$ = Creater::namedForAll (name, $3, $5); } 
| LITERAL ':' simple_aussage DOUBLE_ARROW aussage { std::string name = literal($1); $$ = Creater::namedFunction (name, $3, $5); } 
| simple_aussage { $$ = $1; };