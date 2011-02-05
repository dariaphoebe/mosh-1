#include "terminal.hpp"

using namespace Terminal;

std::string Display::new_frame( Framebuffer &f )
{
  f.back_color_erase();

  std::string screen;
  bool cursor_was_moved = false;

  /* has window title changed? */
  if ( (!initialized)
       || (f.get_window_title() != last_frame.get_window_title()) ) {
      /* set window title */
    screen.append( "\033]0;[rtm] " );
    std::vector<wchar_t> window_title = f.get_window_title();
    for ( std::vector<wchar_t>::iterator i = window_title.begin();
	  i != window_title.end();
	  i++ ) {
      char utf8[ 8 ];
      snprintf( utf8, 8, "%lc", *i );
      screen.append( utf8 );
    }
    screen.append( "\033\\" );
  }

  /* has reverse video state changed? */
  if ( (!initialized)
       || (f.ds.reverse_video != last_frame.ds.reverse_video) ) {
    /* set reverse video */
    char rev[ 8 ];
    snprintf( rev, 8, "\033[?5%c", (f.ds.reverse_video ? 'h' : 'l') );
    screen.append( rev );
  }

  /* has size changed? */
  if ( (!initialized)
       || (f.ds.get_width() != last_frame.ds.get_width())
       || (f.ds.get_height() != last_frame.ds.get_height()) ) {
    /* clear screen */
    screen.append( "\033[0m;\033[H\033[2J" );
    initialized = false;
    cursor_was_moved = true;
    current_renditions.clear();
    current_renditions.push_back( 0 );
  }

  /* iterate for every cell */
  for ( int y = 0; y < f.ds.get_height(); y++ ) {
    for ( int x = 0; x < f.ds.get_width(); /* let charwidth handle advance */ ) {
      std::string cell_string;
      Cell *cell = f.get_cell( y, x );
      bool different = false;

      char curmove[ 32 ];
      snprintf( curmove, 32, "\033[%d;%dH", y + 1, x + 1 );
      cell_string.append( curmove );

      /* have renditions changed? */
      if ( (!initialized)
	   || (cell->renditions != last_frame.get_cell( y, x )->renditions) ) {
	different = true;
      }

      std::vector<int> cell_print_renditions;
      cell_print_renditions = cell->renditions;
      cell_print_renditions.insert( cell_print_renditions.begin(), 0 );

      if ( cell_print_renditions != current_renditions ) {
	/* print renditions */
	cell_string.append( "\033[0" );
	char rendition[ 32 ];
	for ( std::vector<int>::iterator i = cell->renditions.begin();
	      i != cell->renditions.end();
	      i++ ) {
	  snprintf( rendition, 32, ";%d", *i );
	  cell_string.append( rendition );
	}
	cell_string.append( "m" );
      }

      /* clear cell */
      cell_string.append( "\033[X" );

      /* did fallback status change? */
      if ( (!initialized)
	   || (cell->fallback != last_frame.get_cell( y, x )->fallback) ) {
	different = true;
      }

      /* cells that begin with combining character get combiner attached to no-break space */
      if ( cell->fallback ) {
	char utf8[ 8 ];
	snprintf( utf8, 8, "%lc", 0xA0 );
	cell_string.append( utf8 );
      }

      /* have cell contents changed? */
      if ( (!initialized)
	   || (cell->contents != last_frame.get_cell( y, x )->contents) ) {
	different = true;
      }

      /* always restrike the cell contents if anything changed */
      for ( std::vector<wchar_t>::iterator i = cell->contents.begin();
	    i != cell->contents.end();
	    i++ ) {
	char utf8[ 8 ];
	snprintf( utf8, 8, "%lc", *i );
	cell_string.append( utf8 );
      }

      /* if anything changed, redo cell */
      if ( different ) {
	screen.append( cell_string );
	cursor_was_moved = true;
	current_renditions = cell_print_renditions;
      }

      x += cell->width;
    }
  }

  /* has cursor location changed? */
  if ( (!initialized)
       || (f.ds.get_cursor_row() != last_frame.ds.get_cursor_row())
       || (f.ds.get_cursor_col() != last_frame.ds.get_cursor_col())
       || cursor_was_moved ) {
    char curmove[ 32 ];
    snprintf( curmove, 32, "\033[%d;%dH", f.ds.get_cursor_row() + 1,
	      f.ds.get_cursor_col() + 1 );
    screen.append( curmove );
  }

  /* has cursor visibility changed? */
  if ( (!initialized)
       || (f.ds.cursor_visible != last_frame.ds.cursor_visible) ) {
    if ( f.ds.cursor_visible ) {
      screen.append( "\033[?25h" );
    } else {
      screen.append( "\033[?25l" );
    }
  }

  last_frame = f;
  initialized = true;

  return screen;
}