#define MAX_LINE_LENGTH          524288
#define MAX_CMD_LENGTH             2024
#define MAX_STRING_LENGTH           256
#define DEFAULT_MAX_RANGE           800.0
#define DEFAULT_MAX_USABLE_RANGE   8090.0
#define DEFAULT_ZOOM                  1.0
#define DEFAULT_RESOLUTION           10.0
#define DEFAULT_LASER_ID              0
#define DEFAULT_BORDER              100
#define MAP_STD_VAL                   0.1
#define CARMEN_MAP_STD_VAL           -1
#define MIN_ROTATION                  0.00872638    /* .5 degrees */
#define DEFAULT_ROTATION_ANGLE        0.0
#define EPSILON                       0.0001
#define ROBOT_SIZE                   50
#define ROBOT_FILL_COLOR        "yellow"
#define ROBOT_BORDER_COLOR       "black"
#define MINIMUM_MAP_SIZE             300
#define MAP_SIZE_STEP                50
#define DYNAMIC_PROB                 0.2
#define STATIC_PROB                  0.8

#ifndef MAXFLOAT
#define MAXFLOAT      3.40282347e+38F
#endif

typedef struct {
  int        width;
  int        height;
  VECTOR2    start;
  RGB     ** pixel;
} BG_IMAGE_TYPE;

typedef struct {
  double   * pixel;
} IMAGE_TYPE;

enum FORMAT_TYPE {
  GRAPHICS,
  GNUPLOT
};
  
typedef struct {
  enum FORMAT_TYPE   format;
  int                display_arrow;
  RGB                bg;
  char               bgcolor[MAX_STRING_LENGTH];
  double             darken;
  int                showpath;
  char               pathcolor[MAX_STRING_LENGTH];
  double             pathwidth;
  double             rotation_angle;
  VECTOR2            rotation_center;
  char               infilename[MAX_STRING_LENGTH];
  char               outfilename[MAX_STRING_LENGTH];
  char               filetemplate[MAX_STRING_LENGTH];
  double             usable_range;
  double             max_range;
  double             zoom;
  double             resolution_x;
  double             resolution_y;
  int                utm_correct;
  int                google_correct;
  int                google_zoom;
  double             border;
  double             unknown_val;
  int                use_odds_model;
  double             static_prob;
  double             dynamic_prob;
  int                flip;
  int                animation;
  double             anim_step;
  int                anim_skip;
  int                laser_id;
  int                endpoints;
  int                from;
  int                to;
  int                convolve;
  int                kernel_size;
  int                integrate_scans;
  int                set_size;
  int                crop_size;
  double             min_x;
  double             min_y;
  double             max_x;
  double             max_y;
  int                set_pos;
  double             pos_x;
  double             pos_y;
  double             pos_o;
  int                bgfile;
  BG_IMAGE_TYPE      background;
  int                gpspath;
  VECTOR2            bgoffset;
  RPOS2              posstart;
} SETTINGS_TYPE;

double round(double x);

extern SETTINGS_TYPE  settings;

void
map_integrate_scan( GRID_MAP2 *map, LASERSENS2_DATA data,
		    double max_range, double max_usable  );

int
map_pos_from_vec2( VECTOR2 pos, GRID_MAP2 *map, VECTOR2 *v );

void
map_compute_probs( GRID_MAP2 * map, double unknown_val );

void
filetemplate_from_filename( char * filetemplate, char * filename );

char *
dump_filename( void );

void
write_image_magick_map( GRID_MAP2 *map, REC2_DATA *rec );

void
simple_convolve_map( GRID_MAP2 *map, GAUSS_KERNEL kernel );

void
read_image_file( char * filename, BG_IMAGE_TYPE * img );

