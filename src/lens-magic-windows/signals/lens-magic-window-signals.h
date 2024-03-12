#pragma once

#include <adwaita.h>
#include "../lens-magic-window.h"

void exposure_change(GtkRange* range, LensMagicWindow *self);
void brightness_change(GtkRange* range, LensMagicWindow *self);
void contrast_change(GtkRange* range, LensMagicWindow *self);
void highlights_change(GtkRange* range, LensMagicWindow *self);
void shadows_change(GtkRange* range, LensMagicWindow *self);
void temperature_change(GtkRange* range, LensMagicWindow *self);
void tint_change(GtkRange* range, LensMagicWindow *self);
void saturation_change(GtkRange* range, LensMagicWindow *self);
void color_hue_change(GtkRange* range, LensMagicWindow *self);
void color_saturation_change(GtkRange* range, LensMagicWindow *self);
void color_lightness_change(GtkRange* range, LensMagicWindow *self);
