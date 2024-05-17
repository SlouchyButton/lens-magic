
#include "config.h"

#include "lens-magic-application.h"
#include "lens-magic-windows/lens-magic-window.h"

struct _LensMagicApplication
{
	AdwApplication parent_instance;
};

G_DEFINE_TYPE (LensMagicApplication, lens_magic_application, ADW_TYPE_APPLICATION)

LensMagicApplication* lens_magic_application_new(const char* application_id, 
		GApplicationFlags  flags) {
	g_return_val_if_fail(application_id != NULL, NULL);

	return g_object_new(LENS_MAGIC_TYPE_APPLICATION,
						"application-id", application_id,
						"flags", flags,
						NULL);
}

static void lens_magic_application_activate(GApplication* app) {
	GtkWindow *window;

	g_assert(LENS_MAGIC_IS_APPLICATION (app));

	window = gtk_application_get_active_window(GTK_APPLICATION (app));

	if (window == NULL)
		window = g_object_new(LENS_MAGIC_TYPE_WINDOW,
		                       "application", app,
		                       NULL);
	
    #ifndef NDEBUG
		gtk_widget_add_css_class((GtkWidget*) window, "devel");
    #endif

	gtk_window_present (window);
}

static void lens_magic_application_class_init(LensMagicApplicationClass* klass) {
	GApplicationClass *app_class = G_APPLICATION_CLASS(klass);

	app_class->activate = lens_magic_application_activate;
}

static void lens_magic_application_about_action(GSimpleAction* action, GVariant* parameter,
		gpointer user_data) {
	static const char *developers[] = {"Ales Nezbeda", "Michele Morrone (glslSmartDeNoise)", NULL};
	LensMagicApplication *self = user_data;
	GtkWindow *window = NULL;

	g_assert(LENS_MAGIC_IS_APPLICATION(self));

	window = gtk_application_get_active_window(GTK_APPLICATION(self));

	adw_show_about_window(window,
	                       "application-name", "Lens Magic",
	                       "application-icon", "com.alesnezbeda.lens-magic",
	                       "developer-name", "Ales Nezbeda",
	                       "version", "0.1.0",
	                       "developers", developers,
	                       "copyright", "Mozilla Public License 2.0\n\nMichele Morrone - glslSmartDeNoise (BSD 2-Clause license)",
	                       NULL);
}

static void lens_magic_application_quit_action(GSimpleAction* action, GVariant* parameter,
		gpointer user_data) {
	LensMagicApplication *self = user_data;

	g_assert(LENS_MAGIC_IS_APPLICATION(self));

	g_application_quit(G_APPLICATION(self));
}

static const GActionEntry app_actions[] = {
	{ "quit", lens_magic_application_quit_action },
	{ "about", lens_magic_application_about_action },
};

static void lens_magic_application_init(LensMagicApplication *self) {
	g_action_map_add_action_entries(G_ACTION_MAP(self),
	                                 app_actions,
	                                 G_N_ELEMENTS(app_actions),
	                                 self);
	gtk_application_set_accels_for_action(GTK_APPLICATION(self),
	                                       "app.quit",
	                                       (const char *[]) { "<primary>q", NULL });
}
