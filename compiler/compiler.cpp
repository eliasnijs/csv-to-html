/* See LICENSE file for copyright and license details. */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "baselayer.cpp"
#include "config.h"
#include "utils.cpp"

struct Project { 
  char *title;
  char *description;
  B32 is_finished;
  B32 is_private;
  B32 has_sourcecode;
  char *sourcecode_link;
  struct Project *next_project;
};

S32
main(S32 argc, char *argv[])
{
  B32 no_error = true;
  
  char *in_path_name = argv[1];
  char *out_path_name = argv[2];

  S32 file_length = 0;
  char *file_content = (char *) 0;
  FILE *in_file = fopen(in_path_name, "rb");
  if (in_file) {
    file_read(in_file, &file_length, 0);
    file_content = (char *) malloc(file_length);
    file_read(in_file, &file_length, file_content);
  } else {
    printf("failed to open file for reading!/n");
    no_error = false;
  }
  fclose(in_file);

  Project *first_project = (Project *) calloc(1,sizeof(Project)); 
  first_project->next_project = (Project *) 0;
  first_project->title = file_content;
  Project *last_project = first_project; 

  char *beginning_of_last_field = file_content;
  S32 project_field_index = 0;
  for (S32 file_content_index = 0;
       file_content_index < file_length && no_error;
       ++file_content_index)
  {
    char *current_character = &file_content[file_content_index];
    switch (*current_character) {
      case (SEPERATOR): {
        *current_character = '\0';
        ++project_field_index;
        switch (project_field_index) {
          case (1): {
            last_project->title = beginning_of_last_field;
          } break;
          case (2): {
            last_project->description = beginning_of_last_field;
          } break;
          case (3): {
            if (strcmp(beginning_of_last_field, "finished") == 0) {
              last_project->is_finished = true; 
            } else if (strcmp(beginning_of_last_field, "in progress") == 0) {
              last_project->is_finished = false; 
            } else {
              // throw compiler error
              last_project->is_finished = -1; 
              no_error = false;
            }
          } break;
          case (4): {
            if (!strcmp(beginning_of_last_field, "private")) {
              last_project->is_private = true; 
            } else if (!strcmp(beginning_of_last_field, "public")) {
              last_project->is_private = false; 
            } else {
              // throw compiler error
              last_project->is_finished = -1; 
              no_error = false;
            }
          } break;
        }
        beginning_of_last_field = current_character + 1;
      } break;
      case ('\n'): {
        *current_character = '\0';
        // finishing last field of current project
        if (strlen(beginning_of_last_field) > 0) {
          last_project->has_sourcecode = true;
          last_project->sourcecode_link = beginning_of_last_field;
        }

        // create new project
        project_field_index = 0;
        if (file_content_index < file_length - 1) {
          Project *new_project = (Project *) calloc(1,sizeof(Project)); 
          last_project->next_project = new_project;
          last_project = new_project;
          beginning_of_last_field = current_character + 1;
        }
      } break;
    }
  }


#if 1
  // NOTE(Elias): output structure to terminal
  {
    Project *selected_project = first_project;
    while (selected_project) {
      printf("Project:\n");
      printf(" - title:        %s\n", selected_project->title);
      printf(" - description:  %s\n", selected_project->description);
      printf(" - is finished:  %d\n", selected_project->is_finished);
      printf(" - is private:   %d\n", selected_project->is_private);
      if (selected_project->has_sourcecode) {
        printf(" - source code:  %s\n", selected_project->sourcecode_link);
      } else {
        printf(" - source code:  Not public\n");
      }
      selected_project = selected_project->next_project;
    } 
  }
#endif

  // NOTE(Elias):  create html
  if (no_error) { 
    FILE *out_file = fopen(out_path_name, "w");
    Project *selected_project = first_project;
    fprintf(out_file,"\n");
    fprintf(out_file,"<!DOCTYPE html>\n");
    fprintf(out_file,"<html lang=\"en\">\n");
    fprintf(out_file,"<head>\n");
    fprintf(out_file,"  <title>Elias Nijs ~ Projects</title>\n");
    fprintf(out_file,"  <meta charset=\"UTF-8\">\n");
    fprintf(out_file,"  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0;\">\n");
    fprintf(out_file,"  <link rel=\"stylesheet\" href=\"style.css\">\n");
    fprintf(out_file,"</head>\n");
    fprintf(out_file,"<body>\n");
    fprintf(out_file,"<h2>Personal Projects</h2>\n");
    fprintf(out_file,"\n");
    while (selected_project) {
      if (!selected_project->is_private) {
        fprintf(out_file,"<div class=\"project\">\n");
        fprintf(out_file,"  <h3>%s</h3>\n", selected_project->title);
        fprintf(out_file,"  <p>%s</p>\n", selected_project->description);
        fprintf(out_file,"  <table>\n");
        fprintf(out_file,"  <tr>\n");
        fprintf(out_file,"    <td>Status:</td>\n");
        if (selected_project->is_finished) {
          fprintf(out_file,"    <td>Finished</td>\n");
        } else {
          fprintf(out_file,"    <td>In Progress</td>\n");
        }
        fprintf(out_file,"  </tr>\n");
        fprintf(out_file,"  <tr>\n");
        fprintf(out_file,"    <td class=\"project-property-title\">Source code:</td>\n");
        if (selected_project->has_sourcecode) {
          fprintf(out_file,"    <td><a href=\"%s\">%s</a></td>\n", 
                  selected_project->sourcecode_link,
                  selected_project->sourcecode_link);
        } else {
          fprintf(out_file,"    <td>Source code not public</td>\n"); 
        }
        fprintf(out_file,"  </tr>\n");
        fprintf(out_file,"  </table>\n");
        fprintf(out_file,"</div>\n");
      }
      selected_project = selected_project->next_project;
    }
    fprintf(out_file,"</body>\n");
    fprintf(out_file,"</html>\n");
    fclose(out_file);
  }

  // NOTE(Elias): cleanup
  { 
    Project *selected_project = first_project;
    while (selected_project) {
      Project *current_project = selected_project;
      selected_project = current_project->next_project; 
      free(current_project);
    }
    free(file_content);
  }

  return(0);
}
