#pragma once

#include <adwaita.h>
#include "../lens-magic-window.h"

gboolean original_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);

void exposure_scale_change(GtkRange* range, LensMagicWindow* self);
void brightness_scale_change(GtkRange* range, LensMagicWindow* self);
void contrast_scale_change(GtkRange* range, LensMagicWindow* self);
void highlights_scale_change(GtkRange* range, LensMagicWindow* self);
void shadows_scale_change(GtkRange* range, LensMagicWindow* self);
void temperature_scale_change(GtkRange* range, LensMagicWindow* self);
void tint_scale_change(GtkRange* range, LensMagicWindow* self);
void saturation_scale_change(GtkRange* range, LensMagicWindow* self);

void filter_red_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void filter_green_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void filter_blue_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void color_hue_scale_change(GtkRange* range, LensMagicWindow* self);
void color_saturation_scale_change(GtkRange* range, LensMagicWindow* self);
void color_lightness_scale_change(GtkRange* range, LensMagicWindow* self);

void noise_reduction_scale_change(GtkRange* range, LensMagicWindow* self);
void noise_reduction_sharpen_scale_change(GtkRange* range, LensMagicWindow* self);
