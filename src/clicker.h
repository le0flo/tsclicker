#ifndef CLICKER_H
#define CLICKER_H

#define N_INTERVALS 1000

/* TS clicker main methods */

bool tsclicker_toggle();
void tsclicker_intervals_setmode(bool isfixed);
void tsclicker_intervals_fixed_update(int updatedcps);
bool tsclicker_intervals_config_update();

#endif