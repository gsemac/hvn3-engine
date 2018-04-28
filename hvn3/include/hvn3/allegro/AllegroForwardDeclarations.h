#pragma once

/*
This file contains forward declarations for classes used by Allegro.
Use this file instead of including Allegro header files in engine header files.
*/

// AllegroAdapter.h

struct ALLEGRO_FONT;

// Display.h

struct ALLEGRO_DISPLAY;

// EventSource.h

struct ALLEGRO_EVENT_SOURCE;

// Utf8String.h

struct _al_tagbstring;
typedef struct _al_tagbstring ALLEGRO_USTR;
typedef struct _al_tagbstring ALLEGRO_USTR_INFO;

// Sound.h

struct ALLEGRO_SAMPLE;
struct ALLEGRO_SAMPLE_ID;
struct ALLEGRO_SAMPLE_INSTANCE;