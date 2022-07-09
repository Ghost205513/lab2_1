char *readline_();

void *read_row(char *str, size_t *n, RingInfo *ring_row);

Matrix *read_matrix_keyboard();

Matrix *read_matrix_binary_file();

void print_matrix(Matrix src);

void print_matrix_binary_file(Matrix src);

int *check_int(const char *str);

int64_t check_natural(const char *s);

Matrix *gen_matrix(size_t n, size_t m);