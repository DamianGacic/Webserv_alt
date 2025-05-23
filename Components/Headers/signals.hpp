#pragma once

extern volatile bool    sigint_pressed; // 1 after sigint was pressed, 0 before
void    set_sigint( void );