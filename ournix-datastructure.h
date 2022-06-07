typedef struct Pathway {

  char *type;
  char *name;
  int timestamp;
  struct Pathway *subdirectory;
  struct Pathway *parent_directory;
  struct Pathway *next_element;

} Path;

typedef struct Filesystem {

  struct Pathway *main_directory;
  struct Pathway *curr;

} Ournix;
