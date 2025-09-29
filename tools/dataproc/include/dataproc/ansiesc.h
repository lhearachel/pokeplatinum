#ifndef DATAPROC_ANSIESC_H
#define DATAPROC_ANSIESC_H

#include <stdarg.h>

// TEXT STYLE MODES - \033[CODEm

#define STYLE_ALL_OFF       "0"
#define STYLE_BOLD_ON       "1"
#define STYLE_BOLD_OFF      "22" // NOTE: Also DIM_OFF
#define STYLE_DIM_ON        "2"
#define STYLE_DIM_OFF       "22" // NOTE: Also BOLD_OFF
#define STYLE_ITALIC_ON     "3"
#define STYLE_ITALIC_OFF    "23"
#define STYLE_UNDERLINE_ON  "4"
#define STYLE_UNDERLINE_OFF "24"
#define STYLE_BLINKING_ON   "5"
#define STYLE_BLINKING_OFF  "25"
#define STYLE_INVERSE_ON    "7"
#define STYLE_INVERSE_OFF   "27"
#define STYLE_HIDDEN_ON     "8"
#define STYLE_HIDDEN_OFF    "28"
#define STYLE_STRIKE_ON     "9"
#define STYLE_STRIKE_OFF    "29"

#define COLOR_BLACK_FG   "30"
#define COLOR_BLACK_BG   "40"
#define COLOR_RED_FG     "31"
#define COLOR_RED_BG     "41"
#define COLOR_GREEN_FG   "32"
#define COLOR_GREEN_BG   "42"
#define COLOR_YELLOW_FG  "33"
#define COLOR_YELLOW_BG  "43"
#define COLOR_BLUE_FG    "34"
#define COLOR_BLUE_BG    "44"
#define COLOR_MAGENTA_FG "35"
#define COLOR_MAGENTA_BG "45"
#define COLOR_CYAN_FG    "36"
#define COLOR_CYAN_BG    "46"
#define COLOR_WHITE_FG   "37"
#define COLOR_WHITE_BG   "47"
#define COLOR_DEFAULT_FG "39"
#define COLOR_DEFAULT_BG "49"

#define EVAL(X)        EVAL_(X)
#define EVAL_(X)       X
#define CONCAT(A, B)   CONCAT_(A, B)
#define CONCAT_(A, B)  A ## B

#define STYLE_OFF(S)   "\033[" EVAL(CONCAT(STYLE_, CONCAT(S, _OFF))) "m"
#define STYLE_ON(S)    "\033[" EVAL(CONCAT(STYLE_, CONCAT(S, _ON)))  "m"
#define COLOR_FG(C)    "\033[" EVAL(CONCAT(COLOR_, CONCAT(C, _FG)))  "m"
#define COLOR_BG(C)    "\033[" EVAL(CONCAT(COLOR_, CONCAT(C, _BG)))  "m"

#define STYLE_NONE       STYLE_OFF(ALL)
#define STYLE_BOLD       STYLE_ON(BOLD)
#define STYLE_DIM        STYLE_ON(DIM)
#define STYLE_ITALIC     STYLE_ON(ITALIC)
#define STYLE_UNDERLINE  STYLE_ON(UNDERLINE)
#define STYLE_BLINKING   STYLE_ON(BLINKING)
#define STYLE_INVERSE    STYLE_ON(INVERSE)
#define STYLE_HIDDEN     STYLE_ON(HIDDEN)
#define STYLE_STRIKE     STYLE_ON(STRIKE)

#define COLOR_BLACK      COLOR_FG(BLACK)
#define COLOR_RED        COLOR_FG(RED)
#define COLOR_GREEN      COLOR_FG(GREEN)
#define COLOR_YELLOW     COLOR_FG(YELLOW)
#define COLOR_BLUE       COLOR_FG(BLUE)
#define COLOR_MAGENTA    COLOR_FG(MAGENTA)
#define COLOR_CYAN       COLOR_FG(CYAN)
#define COLOR_WHITE      COLOR_FG(WHITE)
#define COLOR_DEFAULT    COLOR_FG(DEFAULT)

#endif // DATAPROC_ANSIESC_H
