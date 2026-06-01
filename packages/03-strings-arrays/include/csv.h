/*
 * csv.h — Simple CSV line parser.
 *
 * Parses a single CSV line into fields. Handles basic quoting.
 */
#ifndef CSV_H
#define CSV_H

#include <stddef.h>

/*
 * Parse a single CSV line into fields.
 * Returns array of newly allocated strings.
 * *out_count receives the number of fields.
 * Returns NULL on failure.
 */
char **csv_parse_line(const char *line, size_t *out_count);

/*
 * Free the result of csv_parse_line.
 */
void csv_free_fields(char **fields, size_t count);

#endif /* CSV_H */
