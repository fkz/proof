
%stype ElementPtr

%token ZUWEISUNG LITERAL PRINT DOUBLE_ARROW ARROW

%start definitions

//%debug
%lines

%%

definitions: 
| definitions LITERAL ZUWEISUNG aussage ';' { if (!$4->check()) { std::cerr << "error defining " << Literal::from ($2) << std::endl; } eles[Literal::from ($2)] = $4; }
| definitions PRINT aussage ';' { Creater::print($3); }
| definitions PRINT '?' aussage ';' { if ($4->check()) { Creater::print ($4->applyRecursive()); } }
| definitions LITERAL ':' aussage ZUWEISUNG aussage ';' {
  if (!$6->check()) { std::cerr << "error defining " << Literal::from ($2) << " (no valid definition)" << std::endl; }
  if (!$4->check()) { std::cerr << "error defining " << Literal::from ($2) << " (no valid type)" << std::endl; }
  if ($6->check() && $4->check() && !$4->equals_really($6->type())) { std::cerr << "error defining " << Literal::from ($2) << " (types differ)" << std::endl; }
  else eles[Literal::from ($2)] = $6;
}
| definitions LITERAL '?' aussage '?' aussage ';' { 
  if (!$4->check()) { std::cerr << "error comparing " << Literal::from ($2) << " (left side wrong)" << std::endl; }
  if (!$6->check()) { std::cerr << "error comparing " << Literal::from ($2) << " (right side wrong)" << std::endl; }
  if ($6->check() && $4->check()) {
    if ($4->equals_really ($6->copy()))
      std::cerr << Literal::from ($2) << " EQUALS" << std::endl;
    else
      std::cerr << Literal::from ($2) << " DOES NOT EQUAL" << std::endl;
  }
}
;

aussage2: '(' aussage ')' { $$ = $2; }
| LITERAL { $$ = evaluate(Literal::from ($1)); }
;

aussage1: aussage1 aussage2 { $$ = Creater::application ($1, $2); }
| aussage2 { $$ = $1; }
;

aussage: variable1 ARROW aussage { $$ = buildForAlls ($1, $3);/*$$ = Creater::forAll ($1, $3->step(0));*/ } 
| variable2 DOUBLE_ARROW aussage { $$ = buildFunctions ($1, $3);/*$$ = Creater::namedFunction (name, $1, $3);*/ }
| aussage1 { $$ = $1; };

literal_list: LITERAL { $$ = LiteralList::create (Literal::from ($1)); }
| literal_list LITERAL { $$ = $1; LiteralList::push ($1, Literal::from ($2)); }
;

variable1: aussage1 { $$ = ArgumentList::create ("_", $1); }
| variable { $$ = $1; }
;

variable2: literal_list { $$ = ArgumentList::createMany ($1, Creater::unknown()); }
| variable { $$ = $1; }
;

variable: variable_klammern { $$ = $1; }
;

variable_klammern: variable_klammern variable_klammer { $$ = ArgumentList::push ($1, $2); } 
| variable_klammer { $$ = $1; }
;

variable_klammer: '(' literal_list ':' aussage ')' { $$ = ArgumentList::createMany ($2, $4); }
;