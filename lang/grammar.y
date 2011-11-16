
%stype ElementPtr

%token ZUWEISUNG LITERAL PRINT DOUBLE_ARROW ARROW

%start definitions

%debug
//%lines

%%

definitions: 
| definitions LITERAL ZUWEISUNG aussage ';' { std::string name = literal($2); eles[name] = $4; }
| definitions PRINT aussage ';' { Creater::print($3); }
;

simple_aussage: '(' aussage ')' { $$ = $2; }
| LITERAL { $$ = $1; }
;

simple_aussage_ex:
  simple_aussage_ex simple_aussage { $$ = Creater::application ($1, $2); } 
| simple_aussage { $$ = $1; }
;

aussage: simple_aussage_ex ARROW aussage { $$ = Creater::forAll ($1, $3->step(0)); } 
|        LITERAL ':' simple_aussage_ex ARROW aussage { std::string name = literal($1); $$ = Creater::namedForAll (name, $3, $5); } 
| LITERAL ':' simple_aussage_ex DOUBLE_ARROW aussage { std::string name = literal($1); $$ = Creater::namedFunction (name, $3, $5); }
| simple_aussage_ex { $$ = $1; };