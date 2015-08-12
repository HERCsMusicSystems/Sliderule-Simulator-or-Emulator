
#include "prolog.h"
#include "gtk/gtk.h"
#include "SlideRule.h"
#include <string.h>

class sliderule_action;

static gboolean RemoveViewportIdleCode (GtkWidget * viewport) {gtk_widget_destroy (viewport); return FALSE;}

class sliderule_action : public PrologNativeCode {
public:
	PrologAtom * atom;
	int sides;
	GtkWidget * viewport, * area;
	int gtk_redrawer;
	SlideRule * sliderule;
	PrologAtom * stator, * slide, * name, * length;
	PrologAtom * background_colour, * rule_colour, * border_colour;
	PrologAtom * view;
	void draw (cairo_t * cr) {if (sliderule != 0) sliderule -> draw (cr);}
	bool remove (bool remove_gtk = true) {
		if (remove_gtk) {
			g_signal_handler_disconnect (area, gtk_redrawer);
			g_idle_add ((GSourceFunc) RemoveViewportIdleCode, viewport);
		}
		delete this;
		return true;
	}
	bool try_name (PrologAtom * selector, PrologElement * a) {
		if (name != selector) return false; if (! a -> isText ()) return false;
		if (strlen (a -> getText ()) > 127) return false;
		strcpy (sliderule -> name, a -> getText ());
		return true;
	}
	bool try_length (PrologAtom * selector, PrologElement * a) {
		if (length != selector) return false; if (! a -> isNumber ()) return false;
		sliderule -> length = a -> getNumber ();
		return true;
	}
	bool try_slide (PrologAtom * selector, PrologElement * a) {
		if (selector == slide) {
			int index = 1;
			if (a != 0) {if (! a -> isInteger ()) return false; index = a -> getInteger ();}
			Rule * r = sliderule -> current -> createRule (index);
			r -> height = 40.0;
			return true;
		}
		return false;
	}
	bool try_colour (PrologAtom * selector, PrologElement * red, PrologElement * green, PrologElement * blue, PrologElement * alpha) {
		colour c;
		if (red != 0) {if (! red -> isNumber ()) return false; c . red = red -> isDouble () ? red -> getDouble () : int_to_colour (red -> getInteger ());}
		if (green != 0) {if (! green -> isNumber ()) return false; c . green = green -> isDouble () ? green -> getDouble () : int_to_colour (green -> getInteger ());}
		if (blue != 0) {if (! blue -> isNumber ()) return false; c . blue = blue -> isDouble () ? blue -> getDouble () : int_to_colour (blue -> getInteger ());}
		if (alpha != 0) {if (! alpha -> isNumber ()) return false; c . alpha = alpha -> isDouble () ? alpha -> getDouble () : int_to_colour (alpha -> getInteger ());}
		if (selector == background_colour) {sliderule -> current -> background_colour = c; return true;}
		if (selector == rule_colour) {sliderule -> current -> rule_colour = c; return true;}
		if (selector == border_colour) {sliderule -> current -> border_colour = c; return true;}
		return false;
	}
	bool try_view (PrologAtom * selector);
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		if (parameters -> isEarth ()) return remove ();
		PrologAtom * selector = 0;
		PrologElement * a = 0, * b = 0, * c = 0, * d = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) selector = el -> getAtom ();
			else if (a == 0) a = el;
			else if (b == 0) b = el;
			else if (c == 0) c = el;
			else if (d == 0) d = el;
			parameters = parameters -> getRight ();
		}
		if (try_slide (selector, a)) return true;
		if (try_name (selector, a)) return true;
		if (try_length (selector, a)) return true;
		if (try_colour (selector, a, b, c, d)) return true;
		if (try_view (selector)) return true;
		return false;
	}
	sliderule_action (PrologDirectory * dir, PrologAtom * atom, int sides) {
		viewport = 0;
		this -> atom = atom; COLLECTOR_REFERENCE_INC (atom);
		this -> sides = sides;
		stator = dir ? dir -> searchAtom ("stator") : 0;
		slide = dir ? dir -> searchAtom ("slide") : 0;
		name = dir ? dir -> searchAtom ("name") : 0;
		length = dir ? dir -> searchAtom ("length") : 0;
		view = dir ? dir -> searchAtom ("view") : 0;
		background_colour = dir ? dir -> searchAtom ("background_colour") : 0;
		rule_colour = dir ? dir -> searchAtom ("rule_colour") : 0;
		border_colour = dir ? dir -> searchAtom ("border_colour") : 0;
		sliderule = new SlideRule ();
	}
	~ sliderule_action (void) {
		if (atom != 0) {atom -> setMachine (0); atom -> removeAtom ();}
		if (sliderule != 0) delete sliderule;
	}
};

static gboolean ViewportDeleteEvent (GtkWidget * viewport, GdkEvent * event, sliderule_action * action) {
	gtk_widget_destroy (action -> viewport);
	action -> remove (false);
	return FALSE;
}
static gboolean RedrawSlideRule (GtkWidget * viewport, GdkEvent * event, sliderule_action * action) {
	cairo_t * cr = gdk_cairo_create (gtk_widget_get_window (viewport));
	action -> draw (cr);
	cairo_destroy (cr);
	return FALSE;
}
static gboolean CreateSlideRuleViewport (sliderule_action * action) {
	action -> viewport = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (action -> viewport), action -> sliderule -> name);
	g_signal_connect (action -> viewport, "delete-event", G_CALLBACK (ViewportDeleteEvent), action);
	action -> area = gtk_drawing_area_new ();
	gtk_container_add (GTK_CONTAINER (action -> viewport), action -> area);
	action -> gtk_redrawer = g_signal_connect (G_OBJECT (action -> area), "expose-event", G_CALLBACK (RedrawSlideRule), action);
	gtk_window_resize (GTK_WINDOW (action -> viewport),
		(int) (action -> sliderule -> lengthWithMargins () + action -> sliderule -> horizontal_margin * 2.0),
		(int) (action -> sliderule -> totalHeight () + action -> sliderule -> vertical_margin * 2.0));

	gtk_widget_show_all (action -> viewport);
	return FALSE;
}
/*
	gtk_widget_add_events (action -> viewport, GDK_BUTTON_PRESS_MASK | GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK);
	g_signal_connect (G_OBJECT (action -> viewport), "button_press_event", G_CALLBACK (AudioModulePanelKeyon), action);
	g_signal_connect (G_OBJECT (action -> viewport), "button_release_event", G_CALLBACK (AudioModulePanelKeyoff), action);
	g_signal_connect (G_OBJECT (action -> viewport), "motion_notify_event", G_CALLBACK (AudioModulePanelMove), action);
	g_signal_connect (G_OBJECT (action -> viewport), "key-press-event", G_CALLBACK (AudioModulePanelFunctionKey), action);
	if (action -> background_image != 0) gtk_window_resize (GTK_WINDOW (action -> viewport),
											cairo_image_surface_get_width (action -> background_image),
											cairo_image_surface_get_height (action -> background_image));
	gtk_window_move (GTK_WINDOW (action -> viewport), (int) action -> location . x, (int) action -> location . y);
	//gtk_window_set_position (GTK_WINDOW (action -> viewport), GTK_WIN_POS_CENTER);
	const GtkTargetEntry targets [3] = {{"text/plain", 0, 0}, {"text/uri-list", 0, 0}, {"application/x-rootwindow-drop", 0, 0}};
	gtk_drag_dest_set (action -> area, GTK_DEST_DEFAULT_ALL, targets, 3, GDK_ACTION_COPY);
	g_signal_connect (action -> area, "drag-drop", G_CALLBACK (dnd_drop), action);
	g_signal_connect (action -> area, "drag-motion", G_CALLBACK (dnd_motion), 0);
	g_signal_connect (action -> area, "drag-data-received", G_CALLBACK (dnd_receive), action);
	g_signal_connect (action -> area, "drag-leave", G_CALLBACK (dnd_leave), 0);

*/

bool sliderule_action :: try_view (PrologAtom * selector) {
	if (selector != view) return false;
	if (this -> viewport != 0) return false;
	g_idle_add ((GSourceFunc) CreateSlideRuleViewport, this);
	return true;
}

class sliderule_code : public PrologNativeCode {
public:
	PrologDirectory * dir;
	bool code (PrologElement * parameters, PrologResolution * resolution) {
		PrologElement * atom = 0;
		PrologElement * sides = 0;
		while (parameters -> isPair ()) {
			PrologElement * el = parameters -> getLeft ();
			if (el -> isAtom ()) atom = el;
			if (el -> isVar ()) atom = el;
			if (el -> isInteger ()) sides = el;
			parameters = parameters -> getRight ();
		}
		if (atom == 0) return false;
		if (atom -> isVar ()) atom -> setAtom (new PrologAtom ());
		if (! atom -> isAtom ()) return false;
		if (atom -> getAtom () -> getMachine () != 0) return false;
		sliderule_action * machine = new sliderule_action (dir, atom -> getAtom (), sides == 0 ? 3 : sides -> getInteger ());
		if (! atom -> getAtom () -> setMachine (machine)) {delete machine; return false;}
		return true;
	}
	sliderule_code (PrologDirectory * dir) {this -> dir = dir;}
};

class SlideRuleServiceClass : public PrologServiceClass {
public:
	PrologDirectory * dir;
	void init (PrologRoot * root, PrologDirectory * directory) {this -> dir = directory;}
	PrologNativeCode * getNativeCode (char * name) {
		if (strcmp (name, "sliderule") == 0) return new sliderule_code (dir);
		return 0;
	}
};

#ifdef WINDOWS_OPERATING_SYSTEM
extern "C" {
	__declspec (dllexport) char * get_module_code (void) {
		HMODULE hm = GetModuleHandle ("SlideRule.dll");
		HRSRC resource = FindResource (hm, MAKEINTRESOURCE (104), RT_RCDATA);
		if (resource == 0) return 0;
		HGLOBAL loader = LoadResource (hm, resource);
		if (loader == 0) return 0;
		return (char *) LockResource (loader);
	}
	__declspec (dllexport) PrologServiceClass * create_service_class (void);
}
#endif

#ifdef LINUX_OPERATING_SYSTEM
extern "C" {
	extern char * get_module_code (void);
	extern PrologServiceClass * create_service_class (void);
}
extern char resource_sliderule;
char * get_module_code (void) {return & resource_sliderule;}
#endif

PrologServiceClass * create_service_class (void) {return new SlideRuleServiceClass ();}

