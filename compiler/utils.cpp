/* See LICENSE file for copyright and license details. */

/* include <stdio.h> */

S32
file_read(FILE *file_connection, S32 *length, char *storage) 
{
  S32 error_code = 0;
  if (!storage) {
    fseek(file_connection, 0, SEEK_END);
    *length = ftell(file_connection);
    fseek(file_connection, 0, 0);
  } else {
    error_code = fread(storage, *length, 1, file_connection);
  }
  return(error_code);
}
