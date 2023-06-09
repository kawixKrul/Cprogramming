#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int *A) {
    return *(A + row * cols + col);
}

void set(int cols, int row, int col, int *A, int value) {
    *(A + row * cols + col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    if (colsB != rowsA) {
        return;
    }
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            int val = 0;
            for (int k = 0; k < colsA; ++k) {
                val += get(colsA, i, k, A) * get(colsB, k, j, B);
            }
            set(colsB, i, j, AB, val);
        }
    }
}

void read_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            scanf("%d", &t[i * cols + j]);
        }
    }
}

void print_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            printf("%d ", t[i * cols + j]);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[]) {
    int count = 0;
    char line[BUF_SIZE];

    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        if (strcmp(line, "\n") == 0) {
            break;
        }
        array[count] = (char *) malloc(strlen(line) + 1);
        strcpy(array[count], line);
        count += 1;
    }

    return count;
}

void write_char_line(char *array[], int n) {
    printf("%s", array[n]);
}

void delete_lines(char *array[]) {
    for (int i = 0; array[i] != NULL; ++i) {
        free(array[i]);
    }
}

int read_int_lines_cont(int *ptr_array[]) {
    int count = 0;
    char line[BUF_SIZE];
    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        if (strcmp(line, "\n") == 0) {
            break;
        }
        int *data_ptr = NULL;
        data_ptr = (int*) malloc(sizeof(int) * BUF_SIZE);
        ptr_array[count] = data_ptr;
        if (ptr_array[count] == NULL) {
            return -1;
        }
        char *token = strtok(line, " ");
        while (token != NULL) {
            *data_ptr = atoi(token);
            data_ptr++;
            token = strtok(NULL, " ");
        }
        count += 1;
    }
    return count;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *line_ptr = ptr_array[n];
    do {
        printf("%d ", *line_ptr);
        line_ptr++;
    } while (*line_ptr != NULL);
    printf("\n");
}

typedef struct {
	int *values;
	int len;
	double average;
} line_type;

int read_int_lines(line_type lines_array[]) {
    int count = 0;
    char line[BUF_SIZE];
    int *data_ptr = NULL;
    while (fgets(line, BUF_SIZE, stdin) != NULL) {
        if (strcmp(line, "\n") == 0) {
            break;
        }
        int vals = 0;
        data_ptr = (int*) malloc(sizeof(int) * BUF_SIZE);
        lines_array[count].values = data_ptr;
        char *token = strtok(line, " ");
        while (token != NULL) {
            *data_ptr = atoi(token);
            data_ptr++;
            token = strtok(NULL, " ");
            vals += 1;
        }
        lines_array[count].len = vals;
        double sum = 0;
        for (int i = 0; i < vals; ++i) {
            sum += lines_array[count].values[i];
        }
        lines_array[count].average = sum/vals;
        count += 1;
    }
    return count;
}

void write_int_line(line_type lines_array[], int n) {
    for (int i = 0; i < lines_array[n].len; ++i) {
        printf("%d ", lines_array[n].values[i]);
    }
    printf("\n%.2f", lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
    for (int i = 0; i < line_count; ++i) {
        free(array[i].values);
    }
}

int cmp (const void *a, const void *b) {
    const line_type *aa = (line_type*)a;
    const line_type *bb = (line_type*)b;
    return (aa->average - bb->average) > 0 ? 1 : -1;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(*lines_array), cmp);
}

typedef struct {
	int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    for (int k = 0; k < n_triplets; ++k) {
        scanf("%d %d %d\n", &triplet_array[k].r, &triplet_array[k].c, &triplet_array[k].v);
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *tt1 = (triplet*)t1;
    triplet *tt2 = (triplet*)t2;
    int row = tt1->r - tt2->r;
    int col = tt1->c - tt2->c;
    return row != 0 ? row : col;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(*triplet_array), cmp_triplets);
    for (int i = 0; i < n_triplets; ++i) {
        C[i] = triplet_array[i].c;
        V[i] = triplet_array[i].v;
    }
    R[0] = 0;
    int sum = 0;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 0; j < n_triplets; ++j) {
            if (triplet_array[j].r == i-1) {
                sum += 1;
            }
        }
        R[i] = sum;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; ++i) {
        y[i] = 0;
        for (int j = R[i]; j < R[i+1]; ++j) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int *v, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }
}

void write_vector(int *v, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

int read_int() {
	char c_buf[BUF_SIZE];
	fgets(c_buf, BUF_SIZE, stdin);
	return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
	int to_do = read_int();

	int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
	int n, lines_counter, rowsA, colsA, rowsB, colsB;
	int rows, cols, n_triplets;
	char *char_lines_array[TAB_SIZE] = { NULL };
	int continuous_array[TAB_SIZE];
	int *ptr_array[TAB_SIZE];
	triplet triplet_array[TAB_SIZE];
	int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
	int x[TAB_SIZE], y[TAB_SIZE];
	line_type int_lines_array[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d", &rowsA, &colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d", &rowsB, &colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA, colsA, colsB, A, B, AB);
			print_mat(rowsA, colsB, AB);
			break;
		case 2:
			n = read_int() - 1; // we count from 1 :)
			ptr_array[0] = continuous_array;
			read_int_lines_cont(ptr_array);
			write_int_line_cont(ptr_array, n);
			break;
		case 3:
			n = read_int() - 1;
			read_char_lines(char_lines_array);
			write_char_line(char_lines_array, n);
			delete_lines(char_lines_array);
			break;
		case 4:
			n = read_int() - 1;
			lines_counter = read_int_lines(int_lines_array);
			sort_by_average(int_lines_array, lines_counter);
			write_int_line(int_lines_array, n);
			delete_int_lines(int_lines_array, lines_counter);
			break;
		case 5:
			scanf("%d %d %d", &rows, &cols, &n_triplets);
			n_triplets = read_sparse(triplet_array, n_triplets);
			read_vector(x, cols);
			make_CSR(triplet_array, n_triplets, rows, V, C, R);
			multiply_by_vector(rows, V, C, R, x, y);
			write_vector(V, n_triplets);
			write_vector(C, n_triplets);
			write_vector(R, rows + 1);
			write_vector(y, rows);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}


