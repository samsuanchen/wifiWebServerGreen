#include <fvm.h>     // ##### 1.1. load FVM the Forth virtual machine
FVM F;               // ##### 1.2. define F as an instence of FVM
void setup(void) {
  F.init( 115200 );  // ##### 3.1. init F
  F.initEval(        // ##### 3.2. 
  "\
  ( This script could be pasted into Arduino IDE console\
    input box and click \"Send\" to\
    1. draw image, lines, and characters on WIFIBOY screen.\
    2. show how much time needed to draw.\
    3. blink faster while drawing.   )\
  ( ------------------------------------------------------ )\
  ( n for ... next )\
  : for ( -- h ) compile (for) here ; immediate\
  : next ( h -- ) compile (next) here - cell/ , ; immediate\
  ( ------------------------------------------------------ )\
  ( flag if ... else ... then )\
  ( flag if ... then )\
  : if ( -- _if ) compile (if) here 0 , ; immediate\
  : else ( _if -- _else ) >r compile (else) here 0 , here r@ - cell/ r> ! ; immediate\
  : then ( _if_or_else -- ) compile (then) here over - cell/ swap ! ; immediate\
  ( ------------------------------------------------------ )\
  ( begin ... again )\
  ( begin ... flag until )\
  ( begin ... flag while ... repeat )\
  : begin ( -- _begin ) compile (begin) here ; immediate\
  : again ( _begin -- ) compile (again) here - cell/ , ; immediate\
  : until ( _begin -- ) compile (until) here - cell/ , ; immediate\
  : while ( _begin -- h_begin _while ) compile (while) here 0 , ; immediate\
  : repeat ( _begin _while -- ) compile (repeat) swap here - cell/\
    , here over - cell/ swap ! ; immediate\
  ( ------------------------------------------------------ )\
  wb_init\
  getMillis\
  0 0 128 160 img wb_drawImage ( tsuyu )\
  getMillis cr .( drawImage used ) swap - . .( ms )\
  ( ------------------------------------------------------ )\
  getMillis\
  : drawLines\
    42 for 1 1 42 r@ - 3 * 1+ 159 wbRED 1 wb_drawLine next\
    51 for 1 1 127 r@ 3 * 1+ wbRED 1 wb_drawLine next\
  ; drawLines\
  getMillis cr .( drawLines used ) swap - . .( ms )\
  ( ------------------------------------------------------ )\
  getMillis\
  wbCYAN wbCYAN wb_setTextColor\
  z\" WiFiBoy\" 28 10 1 3 wb_drawString drop\
  z\" blink03\" 42 138 2 2 wb_drawString drop\
  wbWHITE wbWHITE wb_setTextColor\
  z\" FVM\" 22 120 2 2 wb_drawString drop\
  getMillis cr .( draw Chars directly used ) swap - . .( ms )\
  ( ------------------------------------------------------ )\
  getMillis\
  : drawChars\
    wbCYAN wbCYAN wb_setTextColor\
    z\" WiFiBoy\" 28 10 1 3 wb_drawString drop\
    z\" blink03\" 42 138 2 2 wb_drawString drop\
    wbWHITE wbWHITE wb_setTextColor\
    z\" FVM\" 22 120 2 2 wb_drawString drop\
  ; drawChars\
  getMillis cr .( drawChars used ) swap - . .( ms )\
  ( ------------------------------------------------------ )\
  : words context\
    begin @ ?dup\
    while dup .id\
    repeat ; words\
  ( ------------------------------------------------------ )\
  : seeAll context\
    begin @ ?dup\
    while dup (see)\
    repeat ; seeAll\
  ( ------------------------------------------------------ )\
  : (forget) ( w -- ) ?dup 0= if exit then context\
    begin ( w a ) ?dup\
    while ( w w' ) @ 2dup =\
      if @ context ! 0\
      then\
    repeat drop ;\
  : forget ' (forget) ;\
  ( ------------------------------------------------------ )\
  27 constant screen\
  : blinks ( delay n -- ) screen output 2*\
    begin ?dup\
    while 1- screen toggle over ms\
    repeat drop ;\
  100 5 blinks\
  ");
}
void loop(void) {
}
