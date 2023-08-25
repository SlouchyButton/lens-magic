
#pragma once

#include <adwaita.h>
#include <math.h>

G_BEGIN_DECLS

#define LENS_MAGIC_TYPE_WINDOW (lens_magic_window_get_type())

G_DECLARE_FINAL_TYPE (LensMagicWindow, lens_magic_window, LENS_MAGIC, WINDOW, AdwApplicationWindow)

G_END_DECLS
