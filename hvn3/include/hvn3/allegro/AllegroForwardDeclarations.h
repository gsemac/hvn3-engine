#pragma once

/*
This file contains forward declarations for classes used by Allegro.
Use this file instead of including Allegro header files in engine header files.
*/

// Utf8String.h

struct _al_tagbstring;
typedef struct _al_tagbstring ALLEGRO_USTR;
typedef struct _al_tagbstring ALLEGRO_USTR_INFO;

// EventSource.h

struct ALLEGRO_EVENT_SOURCE;

// AllegroAdapter.h

struct ALLEGRO_FONT;