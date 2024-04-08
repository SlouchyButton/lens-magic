#pragma once

#include <adwaita.h>
#include "../lens-magic-window.h"

typedef struct Adjustments_ {
    struct adjustment_elements  exposure;
    struct adjustment_elements  brightness;
    struct adjustment_elements  contrast;
    struct adjustment_elements  highlights;
    struct adjustment_elements  shadows;
    struct adjustment_elements  temperature;
    struct adjustment_elements  tint;
    struct adjustment_elements  saturation;
    struct adjustment_elements  noise_reduction;
    struct adjustment_elements  noise_reduction_sharpen;
} Adjustments;

struct adjustment_elements {
    LensMagicWindow*    self;
    GtkSwitch*          sw;
    GtkScale*           scale;
    GtkEntry*           entry;
    gdouble*            settings_value;
};

gboolean original_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);

gboolean adj_switch_state_set(GtkSwitch* sw, gboolean state, struct adjustment_elements* data);

gboolean exposure_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean brightness_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean contrast_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean highlights_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean shadows_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean temperature_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean tint_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);
gboolean saturation_switch_state_set(GtkSwitch* sw, gboolean state, LensMagicWindow* self);

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
