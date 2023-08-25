
#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define LENS_MAGIC_TYPE_APPLICATION (lens_magic_application_get_type())

G_DECLARE_FINAL_TYPE (LensMagicApplication, lens_magic_application, LENS_MAGIC, APPLICATION, AdwApplication)

LensMagicApplication *lens_magic_application_new (const char        *application_id,
                                                  GApplicationFlags  flags);

G_END_DECLS
