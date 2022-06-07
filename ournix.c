#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ournix-datastructure.h"

/*
  TerpConnect login ID: saipmukk
  University ID Number: 118058573
  Discussion Section: 0102

  I pledge on my honor that I have not given or received
  unauthorized assistance on this assignment/examination.

  Block comment: This file is the main file for the
  Ournix system. It contains various methods for 
  operating on a general UNIX system, such as moving
  between directories, listing all the files and
  directories in a subdirectory, etc.
*/

/*

  This function creates a new file given the parameters
  allocating memory as necessary.

*/

Path *create_new_file(Path *new_file, Ournix *filesystem, const char *name);

Path *create_new_file(Path *new_file, Ournix *filesystem, const char* name) {

  new_file = malloc(sizeof(Path));

  new_file->type = malloc(sizeof(char) * (strlen("f") + sizeof(char)));
  strcpy(new_file->type, "f");
  new_file->name = malloc(sizeof(char) * (strlen(name) + sizeof(char)));
  strcpy(new_file->name, name);
  new_file->timestamp = 1;
  new_file->subdirectory = NULL;
  new_file->parent_directory = malloc(sizeof(Path));
  new_file->parent_directory = 
    filesystem->curr->parent_directory;
  new_file->next_element = NULL;

  return new_file;

}

/*

  This function creates a new file given the parameters
  allocating memory as necessary.

*/

Path *create_new_directory(Path *new_file, Ournix *filesystem, const char *name);

Path *create_new_directory(Path *new_file, Ournix *filesystem, const char *name) {

  new_file = malloc(sizeof(Path));

  new_file->type = malloc(sizeof(char) * (strlen("d") + sizeof(char)));
  strcpy(new_file->type, "d");
  new_file->name = malloc(sizeof(char) * (strlen(name) + sizeof(char)));
  strcpy(new_file->name, name);
  new_file->subdirectory = NULL;
  new_file->parent_directory = malloc(sizeof(Path));
  new_file->parent_directory = 
    filesystem->curr->parent_directory;
  new_file->next_element = NULL;

  return new_file;

}

void mkfs(Ournix *const filesystem) {

  /*
    These two lines access the main filesystem of the 
    Ournix system, and it allocates space for the main directory
    in the heap in the size of the "Path" data structure. It then
    makes it so that the directory that is currently being looked
    at, indicated by the "curr" pointer variable in the "Ournix"
    data structure, point to the main directory, which was just 
    created.
  */

  filesystem->main_directory = malloc(sizeof(Path));
  filesystem->main_directory->type = 
    malloc(sizeof(char) * (strlen("d") + 1));
  strcpy(filesystem->main_directory->type, "d");
  filesystem->main_directory->name = NULL;
  filesystem->main_directory->subdirectory = NULL;
  filesystem->main_directory->parent_directory = 
    malloc(sizeof(Path));
  filesystem->main_directory->parent_directory = 
    filesystem->main_directory;
  filesystem->main_directory->next_element = NULL;
  filesystem->curr = malloc(sizeof(Path));
  filesystem->curr = filesystem->main_directory;

}

int touch(Ournix *const filesystem, const char name[]) {

  /*
    These variables are pointers and a short that acts as a boolean
    variable. The first "Path" pointer acts as a pointer to a file in
    the Ournix system if it exists, so that it does not need to be
    accessed in a more complicated way when adjusting the timestamp
    of this file. The second "Path" pointer acts as a pointer to the
    current element that is being looked at by the loop. The third
    "Path" pointer acts as a pointer to the first element in the 
    directory that is being looked at, so that it can be readjusted
    to that specfic element at the end of iteration through all the
    elements in a directory. The char pointers are just strings for
    comparison purposes. Finally, the short variable named "directory"
    is for seeing whether or not the element the name that is the same
    as the one passed into the parameter is a directory or not.
  */

  Path *file = NULL;
  Path *current_element = NULL;
  char *name_value = NULL;
  short directory = 0;
  const char *one_dot = ".";
  const char *two_dots = "..";
  const char *slash = "/";
  const char *empty_string = "";
  const char *file_name = "f";
  const char *directory_name = "d";
  const char char_slash = '/';

  /*
    Checks for all error cases, and returns 1 if any of
    them are true.
  */

  if(filesystem == NULL) return 1;

  current_element = filesystem->curr->subdirectory;

  /*
    Checks for error cases.
  */
  
  if(strcmp(name, one_dot) == 0 || 
     strcmp(name, two_dots) == 0 || 
     strcmp(name, slash) == 0 || 
     strcmp(name, empty_string) == 0 ||
     strchr(name, char_slash) != NULL) {

    return 1;

  }

  if(current_element != NULL) {

    name_value = current_element->name;

  } else {

    Path *new_file = NULL;

    new_file = create_new_file(new_file, filesystem, name);

    filesystem->curr->subdirectory = malloc(sizeof(Path));
    filesystem->curr->subdirectory = new_file;

    return 1;

  }
  
  /*
    Iterates through every element in the current directory and
    finds the correct file in it if it exists. It then either
    sets the "file" variable to that file if it is a file, or
    sets directory to 1 if it is a directory.
  */
 
  while(current_element != NULL) {

    name_value = current_element->name;
    
    if(strcmp(name, name_value) == 0 
       && strcmp(current_element->type, file_name) == 0) {

        /*
	  If a file exists with the name parameter, then the 
	  file variable is set to the current file that is 
	  being looked at.
	*/

	file = current_element;

    } else if(strcmp(name, name_value) == 0
	      && strcmp(current_element->type, directory_name) == 0) {

        /*
	  Otherwise, if the element is a directory, the "directory"
	  variable becomes 1, indicating that the element with the
	  name in the parameter is a directory in the current
	  directory.
        */

	directory = 1;

    }

    /*
      Sets the current element being looked at to the next element
      in the linked list.
    */

    current_element = current_element->next_element;

  }

  /*
    If the file variable is not NULL after the while loop is
    done running, then that means a file was found, and the
    timestamp value of said file will be incremented.
    Otherwise, if the element found was a directory, the
    value 1 is returned.
  */

  current_element = filesystem->curr->subdirectory;

  if(directory) return 1; /* Error case */

  if(file == NULL) {

    /* 
       Inserts into the dynamically allocated
       linked list, considering all possible
       edge cases.
    */

    if(current_element->next_element == NULL) {

      Path *new_file = NULL;

      new_file = create_new_file(new_file, filesystem, name);
      
      if(strcmp(name_value, name) < 0) {

	current_element->next_element = 
	  malloc(sizeof(Path));
        current_element->next_element = 
	  new_file;

      } else {

	filesystem->curr->subdirectory = new_file;
	new_file->next_element = malloc(sizeof(Path));
	new_file->next_element = current_element;

      }

    } else {

      Path *new_file = NULL;
      Path *prev = NULL;

      new_file = create_new_file(new_file, filesystem, name);

      name_value = current_element->name;

      while(strcmp(name_value, name) < 0
	    && current_element != NULL){

	prev = current_element;
	current_element = current_element->next_element;

	if(current_element != NULL)

	  name_value = current_element->name;

      }

      if(current_element == NULL) {

	prev->next_element = malloc(sizeof(Path));
	prev->next_element = new_file;

      } else if(prev == NULL) {

	new_file->next_element = malloc(sizeof(Path));
	new_file->next_element = filesystem->curr->subdirectory;
	filesystem->curr->subdirectory = new_file;

      } else {

	prev->next_element = new_file;
	new_file->next_element = malloc(sizeof(Path));
	new_file->next_element = current_element;

      }

    }

  } else {

    file->timestamp++;

  }
  
  return 1;

}

/*

  This function is much like the touch function,
  except that it creates a directory.
  
*/

int mkdir(Ournix *const filesystem, const char name[]) {

  Path *current_element = NULL;
  char *name_value = NULL;
  short exists = 0;
  const char *one_dot = ".";
  const char *two_dots = "..";
  const char *slash = "/";
  const char *empty_string = "";
  const char char_slash = '/';

  if(strchr(name, char_slash) != NULL 
     || filesystem == NULL) return 1;

  current_element = filesystem->curr->subdirectory;

  if(current_element != NULL) {

    name_value = current_element->name;

  } else {

    Path *new_dir = NULL;
    
    new_dir = create_new_directory(new_dir, filesystem, name);

    filesystem->curr->subdirectory = malloc(sizeof(Path));
    filesystem->curr->subdirectory = new_dir;

    return 1;

  }

  while(current_element != NULL) {

    if(strcmp(current_element->name, name) == 0) {

      exists = 1;

    }

    current_element = current_element->next_element;

  }

  /* Checks for error cases */
  
  if(strcmp(name, one_dot) == 0 || 
     strcmp(name, two_dots) == 0 || 
     strcmp(name, slash) == 0 || 
     strcmp(name, empty_string) == 0 ||
     exists) {

    return 1;

  }

  current_element = filesystem->curr->subdirectory;

  /*
    Creates a new directory based on edge cases in
    linked list creation.
  */
  
  if(current_element->next_element == NULL) {

    Path *new_dir = NULL;
    
    new_dir = create_new_directory(new_dir, filesystem, name);

    if(strcmp(name_value, name) < 0) {

      current_element->next_element = 
	malloc(sizeof(Path));
      current_element->next_element =
	new_dir;

    } else {

      filesystem->curr->subdirectory = new_dir;
      new_dir->next_element = malloc(sizeof(Path));
      new_dir->next_element = current_element;

    }

  } else {

    /*
      Inserts in the middle of a linked list.
    */

    Path *new_dir = NULL;
    Path *prev = NULL;
    
    new_dir = create_new_directory(new_dir, filesystem, name);

    while(strcmp(name_value, name) < 0
	  && current_element != NULL) {

      prev = current_element;
      current_element = current_element->next_element;

      if(current_element != NULL) 
	
	name_value = current_element->name;

    }

    if(current_element == NULL) {

      prev->next_element = malloc(sizeof(Path));
      prev->next_element = filesystem->curr->subdirectory;
      filesystem->curr->subdirectory = new_dir;

    } else if(prev == NULL) {

      new_dir->next_element = malloc(sizeof(Path));
      new_dir->next_element = filesystem->curr->subdirectory;
      filesystem->curr->subdirectory = new_dir;

    } else {

      prev->next_element = new_dir;
      new_dir->next_element = malloc(sizeof(Path));
      new_dir->next_element = current_element;

    }

  }
  
  return 1;

}

/*
  Changes the current directory of the Ournix system.
*/

int cd(Ournix *const filesystem, const char name[]) {

  Path *current_element = NULL;
  Path *found_file = NULL;
  char *name_value = NULL;
  const char *one_dot = ".";
  const char *two_dots = "..";
  const char *slash = "/";
  const char *empty_string = "";
  const char *file_name = "f";

  if(filesystem == NULL) return 1;

  current_element = filesystem->curr->subdirectory;

  if(current_element != NULL) name_value = current_element->name;

  /*
    Loops through the current directory and finds
    the file if it exists.
  */

  while(current_element != NULL) {

    name_value = current_element->name;

    if(strcmp(name_value, name) == 0)

	 found_file = current_element;

    current_element = current_element->next_element;

  }

  if(found_file == NULL) {

    return 1;

  } else {

    if(strcmp(found_file->type, "f") == 0) return 1;

    filesystem->curr = found_file;
    
    return 1;

  }

  /*
    Changes directory based on the special cases.
  */

  if(strcmp(name, one_dot) == 0
     || strcmp(found_file->type, empty_string)) {

    return 1;

  } else if(strcmp(name, two_dots) == 0) {

    if(filesystem->curr->parent_directory != NULL)

      filesystem->curr = filesystem->curr->parent_directory;
    
  } else if(strcmp(name, slash)) {

    filesystem->curr = filesystem->main_directory;

  } else if(found_file == NULL ||
     strcmp(found_file->type, file_name) == 0) {

    return 1;

  }

  return 1;

}

/*
  Lists all of the elements of a directory.
*/

int ls(Ournix *const filesystem, const char name[]) {

  Path *current_element = NULL;
  Path *found_file = NULL;
  char *name_value = NULL;
  const char *one_dot = ".";
  const char *two_dots = "..";
  const char *slash = "/";
  const char *empty_string = "";
  const char *file_name = "f";
  const char *directory_name = "d";

  if(filesystem == NULL) return 1;

  current_element = filesystem->curr->subdirectory;

  if(current_element != NULL) {

    name_value = current_element->name;

  }

  /*
    Loops through the directory to see if an
    element with the parameter name exists.
  */

  while(current_element != NULL) {

    if(strcmp(current_element->name, name) == 0) {

      found_file = current_element;
      break;

    }

    current_element = current_element->next_element;

  }

  /*
    If it does exist, it is listed along with its
    timestamp (if it's a file). If it is a
    directory, it is just listed.
  */

  if(found_file != NULL) {

    if(strcmp(found_file->type, file_name) == 0) {
	
      printf("%s %d\n", found_file->name, found_file->timestamp);
       
    } else if(strcmp(found_file->type, directory_name) == 0) {

      Path *current = found_file->subdirectory;

      while(current != NULL) {

	printf("%s\n", current->name);
	
	current = current->next_element;

      }

    }

  }

  current_element = filesystem->curr->subdirectory;

  /*
    Tests the special cases, such as a dot, two
    dots, slash, etc.
  */

  if(strcmp(name, one_dot) == 0
       || strcmp(name_value, empty_string) == 0) {

    while(current_element != NULL) {

      name_value = current_element->name;
      
      if(strcmp(current_element->type, "d") == 0) {

	printf("%s/\n", name_value);

      } else {

	printf("%s\n", name_value);

      }

      current_element = current_element->next_element;

    }

    return 1;

  } else if(strcmp(name, two_dots) == 0) {

    Path *parent_first = NULL;
    char *parent_name = NULL;

    if(current_element->parent_directory != NULL) {

      parent_first = current_element->parent_directory->subdirectory;
      parent_name = parent_first->name;

    }

    while(parent_first != NULL) {

      parent_name = parent_first->name;

      if(strcmp(parent_first->type, "d") == 0) {

	printf("%s/\n", parent_name);

      } else {

	printf("%s\n", parent_name);

      }

      parent_first = parent_first->next_element;

    }

    return 1;

  } else if(strcmp(name, slash) == 0) {

    Path *root = filesystem->main_directory->subdirectory;

    while(root != NULL) {

      if(strcmp(root->type, "d") == 0) {

	printf("%s/\n", root->name);

      } else {

	printf("%s\n", root->name);

      }
      
      root = root->next_element;

    }
    
    return 1;

  }

  return 1;

}

/*
  Recursive helper method for finding the path.
*/

void pwd_recursive(Path *curr, Path *main, Ournix *filesystem) {

  /*
    Recursively calls until the current
    directory is the main directory.
  */

  if(curr == filesystem->main_directory)

    pwd_recursive(curr->parent_directory, main, filesystem);

  printf("/%s", curr->name);

}

/*
  Lists the path from the current directory
  to the main directory.
*/

void pwd(Ournix *const filesystem) {

  if(strcmp(filesystem->curr->name, "/") == 0) {

    printf("/");

  } else {

    pwd_recursive(filesystem->curr, filesystem->main_directory, filesystem);

  }

  printf("\n");

}

/*
  Frees the parameter file.
*/

void clear_file(Path *file);

void clear_file(Path *file) {

  free(file->name);
  free(file->type);
  free(file->parent_directory);
  free(file);

}

/*
  Frees the parameter directory.
*/

void clear_directory(Path *dir);

void clear_directory(Path *dir) {

  if(dir != NULL) {

    clear_directory(dir->subdirectory);
    clear_directory(dir->next_element);

  } else return;

  if(dir->type != NULL) free(dir->type);

  if(dir->name != NULL) free(dir->name);

  free(dir);

}

/*
  Removes all memory being used by the 
  Ournix system recursively.
 */

void rmfs(Ournix *const filesystem) {

  if(filesystem == NULL) {

    return;

  } else {

    if(filesystem->curr != NULL)

      clear_directory(filesystem->main_directory);

  }

}

/*
  Removes and frees memory being used by
  the parameter file.
*/

void remove_file(Path *prev, Path *curr, const char *remove);

void remove_file(Path *prev, Path *curr, const char *remove) {

  if(curr != NULL) {

    if(strcmp(curr->name, remove) == 0) {

      if(prev == NULL) {

	curr->parent_directory->subdirectory =
	  curr->next_element;

      } else if(strcmp(curr->type, "d") == 0) {

	prev->next_element = curr->next_element;

	if(curr->subdirectory == NULL) {

	  free(curr);

	} else {

	  clear_directory(curr);

	}

      }

    } else {

      remove_file(curr, curr->next_element, remove);

    }

  }

}

/*
  Removes given element.
*/

int rm(Ournix *const filesystem, const char name[]) {

  Path *current_element = NULL;
  Path *found_file = NULL;
  const char *one_dot = ".";
  const char *two_dots = "..";
  const char *slash = "/";
  const char *empty_string = "";
  const char char_slash = '/';

  if(filesystem->curr->subdirectory != NULL) {

    current_element = filesystem->curr->subdirectory;

  }

  while(current_element != NULL) {

    if(strcmp(current_element->name, name) == 0) {

      found_file = current_element;

    }

    current_element = current_element->next_element;

  }

  if(strcmp(name, one_dot) == 0 || 
     strcmp(name, two_dots) == 0 || 
     strcmp(name, slash) == 0 || 
     strcmp(name, empty_string) == 0 ||
     strchr(name, char_slash) != NULL ||
     found_file == NULL) {

    return 1;

  }

  if(found_file != NULL) {

    remove_file(NULL, 
		filesystem->curr->subdirectory, name);

  }

  return 1;

}
