#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include "matrix.h"
#include "streams.h"


int main_menu();
Matrix *input_menu(Matrix *src);
void output_menu(Matrix src, Matrix result);
Matrix *treatment_menu(Matrix *src);
void help(const char *mod);

int main() {
    help("sm");
    return main_menu();
}

int main_menu(){
    srand(time(NULL));
    Matrix *data = calloc(1, sizeof(Matrix));
    data->columns = 0;
    data->rows = 0;
    data->body = NULL;
    Matrix *changes_data  = calloc(1, sizeof(Matrix));
    changes_data->columns = 0;
    changes_data->rows = 0;
    changes_data->body = NULL;
    while (1){
        char *input = readline_();
        if (input && (strlen(input) == 1)){
            switch (input[0]) {
                case '0':
                    free(input);
                    free_all(data);
                    free_all(changes_data);
                    printf("Program end.\n");
                    return 0;
                case '1':
                    if (changes_data->body)
                        help("mh");
                    else
                        help("sh");
                    free(input);
                    break;
                case '2':
                    //free_all(data);
                    data = input_menu(data);
                    copy_matrix(changes_data, data);
                    free(input);
                    break;
                case '3':
                    if (changes_data->body)
                        output_menu(*data, *changes_data);
                    else
                        help("wrong");
                    free(input);
                    break;
                case '4':
                    if (changes_data->body){
                        //free_all(changes_data);
                        changes_data = treatment_menu(changes_data);
                    } else
                        help("wrong");
                    free(input);
                    break;
                default:
                    help("wrong");
                    free(input);
            }
        } else{
            help("wrong");
            free(input);
        }
    }
}

Matrix *input_menu(Matrix *src){
    int64_t n;
    Matrix *temp = NULL;

    help("im");
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    if(src->body)
                        help("mm");
                    else
                        help("sm");

                    free(input);
                    if (temp)
                        free_all(temp);

                    return src;
                case '1':
                    help("ih");
                    free(input);
                    break;
                case '2':
                    free(input);
                    temp = read_matrix_keyboard();

                    if (!temp->body){
                        help("im");
                        free_all(temp);
                        temp = NULL;
                        break;
                    }

                    copy_matrix(src, temp);
                    free_all(temp);
                    help("mm");

                    return src;
                case '3':
                    free(input);
                    temp = read_matrix_binary_file();

                    if(!temp->body){
                        help("im");
                        free_all(temp);
                        temp = NULL;
                        break;
                    }

                    copy_matrix(src, temp);
                    free_all(temp);
                    help("mm");

                    return src;
                case '4':
                    free(input);
                    printf("Enter size of matrix:\n");
                    input = readline_();

                    if ((n = check_natural(input)) == -1) {
                        printf("Wrong format of argument \"size of matrix\".\n");
                        help("im");
                        if (input)
                            free(input);
                        break;
                    }

                    free(input);
                    free_all(src);
                    src = gen_matrix(n);
                    printf("Matrix made.\n");
                    help("mm");

                    return src;
                default:
                    help("wrong");
                    free(input);
            }
        } else {
            help("wrong");
            free(input);
        }
    }
}

void output_menu(Matrix src, Matrix result){
    help("om");
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    help("mm");
                    free(input);
                    return;
                case '1':
                    help("oh");
                    free(input);
                    break;
                case '2':
                    print_matrix(src);
                    free(input);
                    help("om");
                    break;
                case '3':
                    print_matrix(result);
                    free(input);
                    help("om");
                    break;
                case '4':
                    print_matrix_binary_file(src);
                    free(input);
                    help("om");
                    break;
                case '5':
                    print_matrix_binary_file(result);
                    free(input);
                    help("om");
                    break;
                default:
                    help("wrong");
                    free(input);
            }
        } else {
            help("wrong");
            free(input);
        }
    }
}

Matrix *treatment_menu(Matrix *src){
    int64_t index;
    size_t n;

    help("tm");
    while(1) {
        char *input = readline_();
        if (input && (strlen(input) == 1)) {
            switch (input[0]) {
                case '0':
                    if (src->body)
                        help("mm");
                    else
                        help("sm");
                    free(input);
                    return src;
                case '1':
                    help("th");
                    free(input);
                    break;
                case '2':
                    free(input);
                    printf("Enter index row to insert (counting form zero):\n");
                    input = readline_();
                    if (!input){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    index = check_natural(input);
                    free(input);

                    if ((index < 0) || (src->rows < index)){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    printf("Enter new row:\n");
                    input = readline_();
                    if (!input){
                        printf("Wrong data for row.\n");
                        help("tm");
                        break;
                    }

                    int *row = read_row(input, &n);
                    free(input);

                    if (n != src->columns){
                        printf("Wrong amount of values in the row.\n");
                        help("tm");
                        break;
                    }

                    row_insert(src, row, index);
                    free(row);
                    printf("Row inserted.\n");
                    help("tm");
                    break;
                case '3':
                    free(input);
                    printf("Enter index column to insert (counting form zero):\n");
                    input = readline_();

                    if (!input){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    index = check_natural(input);
                    free(input);

                    if ((index < 0) || (src->columns < index)){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    printf("Enter new columns:\n");
                    input = readline_();
                    if (!input){
                        printf("Wrong data for row.\n");
                        help("tm");
                        break;
                    }

                    int *column = read_row(input, &n);
                    free(input);

                    if (n != src->rows){
                        printf("Wrong amount of values in the column.\n");
                        help("tm");
                        break;
                    }

                    column_insert(src, column, index);
                    free(column);
                    printf("Column inserted.\n");
                    help("tm");
                    break;
                case '4':
                    free(input);
                    printf("Enter index row to delete (counting form zero):\n");
                    input = readline_();

                    if (!input){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    index = check_natural(input);
                    free(input);

                    if ((index < 0) || (src->rows <= index)) {
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    row_delete(src, index);
                    printf("Row deleted.\n");
                    help("tm");
                    break;
                case '5':
                    free(input);
                    printf("Enter index column to delete (counting form zero):\n");
                    input = readline_();

                    if (!input){
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    index = check_natural(input);
                    free(input);

                    if ((index < 0) || (src->columns <= index)) {
                        printf("Wrong index.\n");
                        help("tm");
                        break;
                    }

                    column_delete(src, index);
                    printf("Column deleted.\n");
                    help("tm");
                    break;
                case '6':
                    if (src->columns != src->rows)
                        printf("Matrix isn't square.\n");
                    else
                        printf("Matrix determinant = %" PRId64 ".\n", det(src));
                    help("tm");
                    free(input);
                    break;
                default:
                    help("wrong");
                    free(input);
            }
        } else {
            help("wrong");
            free(input);
        }
    }
}

void help(const char *mod){
    switch (mod[0]) {
        case 's':
            switch (mod[1]) {
                case 'm':
                    printf("0. Close.\n"
                           "1. Help.\n"
                           "2. Input.\n");
                    break;
                case 'h':
                    printf("0. Close this program.\n"
                           "1. Print this text.\n"
                           "2. Move to input menu, for chose stream input.\n"
                           "You can't do anything else before input matrix.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
            }
            break;
        case 'm':
            switch (mod[1]) {
                case 'm':
                    printf("0. Close.\n"
                           "1. Help.\n"
                           "2. Input.\n"
                           "3. Output.\n"
                           "4. Treatment.\n");
                    break;
                case 'h':
                    printf("0. Close this program.\n"
                           "1. Print this text.\n"
                           "2. Move to input menu, for chose stream input.\n"
                           "3. Move to output menu, for chose stream output.\n"
                           "4. Move to treatment menu, for manipulate with data.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
            }
            break;
        case 'i':
            switch (mod[1]) {
                case 'm':
                    printf("0. Back.\n"
                           "1. Help.\n"
                           "2. Keyboard.\n"
                           "3. Binary file.\n"
                           "4. Random.\n");
                    break;
                case 'h':
                    printf("0. Move to previous menu.\n"
                           "1. Print this text.\n"
                           "2. Allow input matrix from keyboard:\n"
                           "\tYou need enter n rows with n integers inside.\n"
                           "\tIf you mistake matrix don't update.\n"
                           "3. Allow input square matrix from binary file:\n"
                           "\tFirst 2 values - dimension of matrix, next - values of matrix.\n"
                           "4. Input random generate of square matrix.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
            }
            break;
        case 'o':
            switch (mod[1]) {
                case 'm':
                    printf("0. Back.\n"
                           "1. Help.\n"
                           "2. Source to console.\n"
                           "3. Result to console.\n"
                           "4. Source to binary file.\n"
                           "5. Result to binary file.\n");
                    break;
                case 'h':
                    printf("0. Move to previous menu.\n"
                           "1. Print this text.\n"
                           "2. Print source square matrix to console.\n"
                           "3. Print result matrix to console.\n"
                           "4. Print source square matrix to binary file.\n"
                           "5. Print result matrix to binary file.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
            }
            break;
        case 't':
            switch (mod[1]) {
                case 'm':
                    printf("0. Back.\n"
                           "1. Help.\n"
                           "2. Sum\n"
                           "3. Multiply\n"
                           "4. Transpose\n");
                    break;
                case 'h':
                    printf("0. Move to previous menu.\n"
                           "1. Print this text.\n"
                           "2. Allow insert row to matrix.\n"
                           "3. Allow insert column to matrix.\n"
                           "4. Allow delete row from matrix.\n"
                           "5. Allow delete column from matrix.\n"
                           "6. Count square matrix determinant.\n");
                    break;
                default:
                    printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
            }
            break;
        default:
            printf("Wrong command, try again. Enter \"1\" for get more info about commands\n");
    }
}