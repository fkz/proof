
%stype ElementPtr

%token ZUWEISUNG LITERAL PRINT

%start definitions

%right DOUBLE_ARROW ARROW
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

definition: LITERAL ZUWEISUNG aussage ';'
;

simple_aussage: '(' aussage ')' { $$ = $2; }
| LITERAL { $$ = $1; }
;

aussage: aussage ARROW aussage { $$ = Creater::forAll ($1, $3->step(0)); } 
|        LITERAL ':' simple_aussage ARROW aussage { std::string name = literal($1); $$ = Creater::namedForAll (name, $3, $5); } 
| variable DOUBLE_ARROW aussage 
| simple_aussage;

variable: aussage { $$ = $1; } | LITERAL ':' simple_aussage {  $$ = $3; };