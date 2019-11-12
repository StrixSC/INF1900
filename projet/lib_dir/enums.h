#ifndef ENUMS_H
#define ENUMS_H

enum Timers { TCNT0_, TCNT1_, TCNT2_ };
enum Mode {Normal, PWM_PC, CTC, FPWM_8};
enum Prescaler {P1, P8, P64, P256, P1024};
enum Couleur {Rouge, Vert, Ambre};
enum Port {A, B, C, D};
enum OutputMode {NormalOp, Toggle, LowSet, HighSet};
#endif;