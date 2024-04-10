#pragma once

#include <adwaita.h>
#include "../lens-magic-window.h"

gboolean original_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);

gboolean adj_switch_state_set(GtkSwitch* sw, gboolean state, AdjustmentElements* data);
void adj_scale_change(GtkRange* range, AdjustmentElements* data);

gboolean exposure_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean brightness_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean contrast_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean highlights_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean shadows_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean temperature_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean tint_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean saturation_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);

void filter_red_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void filter_green_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void filter_blue_button_clicked(GtkToggleButton* btn, LensMagicWindow* self);
void color_hue_scale_change(GtkRange* range, LensMagicWindow* self);
void color_saturation_scale_change(GtkRange* range, LensMagicWindow* self);
void color_lightness_scale_change(GtkRange* range, LensMagicWindow* self);
