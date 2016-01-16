#ifndef TERMINAL_BRIDGE_HPP
#define TERMINAL_BRIDGE_HPP

#include <sys/ioctl.h>
#include <string>

#include "completeterminal.h"
#include "networktransport.h"
#include "user.h"
#include "terminaloverlay.h"


class TerminalBridge {
 private:
  int in_fd;
  int out_fd;

  std::string ip;
  std::string port;
  std::string key;

  int escape_key;
  int escape_pass_key;
  int escape_pass_key2;
  bool escape_requires_lf;
  std::wstring escape_key_help;

  struct winsize window_size;

  Terminal::Framebuffer *local_framebuffer, *new_state;
  Overlay::OverlayManager overlays;
  Network::Transport< Network::UserStream, Terminal::Complete > *network;
  Terminal::Display display;

  std::wstring connecting_notification;
  bool repaint_requested, lf_entered, quit_sequence_started;
  bool clean_shutdown;


 public:
 TerminalBridge( const int s_infd, const int s_outfd,
                 const char *s_ip, const char *s_port, const char *s_key, const char *predict_mode )
   : in_fd(s_infd), out_fd( s_outfd ),
    ip( s_ip ), port( s_port ), key( s_key ),
    escape_key( 0x1E ), escape_pass_key( '^' ), escape_pass_key2( '^' ),
    escape_requires_lf( false ), escape_key_help( L"?" ),
    window_size(),
    local_framebuffer( NULL ),
    new_state( NULL ),
    overlays(),
    network( NULL ),
    display( true ), /* use TERM environment var to initialize display */
    connecting_notification(),
    repaint_requested( false ),
    lf_entered( false ),
    quit_sequence_started( false ),
    clean_shutdown( false )
      {
	if ( predict_mode ) {
	  if ( !strcmp( predict_mode, "always" ) ) {
	    overlays.get_prediction_engine().set_display_preference( Overlay::PredictionEngine::Always );
	  } else if ( !strcmp( predict_mode, "never" ) ) {
	    overlays.get_prediction_engine().set_display_preference( Overlay::PredictionEngine::Never );
	  } else if ( !strcmp( predict_mode, "adaptive" ) ) {
	    overlays.get_prediction_engine().set_display_preference( Overlay::PredictionEngine::Adaptive );
	  } else if ( !strcmp( predict_mode, "experimental" ) ) {
	    overlays.get_prediction_engine().set_display_preference( Overlay::PredictionEngine::Experimental );
	  } else {
	    fprintf( stderr, "Unknown prediction mode %s.\n", predict_mode );
	    exit( 1 );
	  }
	}
      }

  ~TerminalBridge() {
    if ( network != NULL ) {
      delete network;
    }
  }

  bool still_connecting( void ) const
  {
    /* Initially, network == NULL */
    return network && ( network->get_remote_state_num() == 0 );
  }

  void resume( void );
  void init( void );
  void shutdown( void );
  bool main( void );

  void main_init( void );
  void process_network_input( void );
  bool process_user_input( int fd );
  bool process_resize( void );

  void output_new_frame( void );

};

#endif