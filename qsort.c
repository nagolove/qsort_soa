#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <alloca.h>

struct LEVELS {
    char ** names;
    char ** filenames;
    int num;
};

struct {
    char *key, *value;
} KONST[] = {
    { "18.lua" , "тест лазера" },
    { "14.lua" , "тест сценарий танков" },
    { "15.lua" , "тест вертолета зигзаг" },
    { "16.lua" , "тест группы деревьев" },
    { "17.lua" , "тест сетевой" },
    { "19.lua" , "тест миникарты" },
    { "20.lua" , "тест ночь" },
    { "21.lua" , "тест стрелки" },
    { "22.lua" , "тест ADSR огибающей" },
    { "23.lua" , "тест частиц" },
    { "10.lua" , "тест перестрелка" },
    { "08.lua" , "тест интерфейса" },
    { "11.lua" , "тест сценарий ангаров" },
    { "12.lua" , "тест сценарий телепортов" },
    { "13.lua" , "тест сценарий башен" },
    { "09.lua" , "тест вертолета прямые маршруты" },
};

struct LEVELS levels_init() {
    struct LEVELS tmp = {0};

    /*printf("%zu\n", sizeof(KONST));*/
    /*printf("%ld\n", ((char*)&KONST[1] - (char*)&KONST[0]));*/
    tmp.num = sizeof(KONST) / sizeof(KONST[0]);
    printf("tmp.num %d\n", tmp.num);
    tmp.filenames = calloc(tmp.num, sizeof(tmp.filenames[0]));
    tmp.names = calloc(tmp.num, sizeof(tmp.names[0]));

    for (int i = 0; i < tmp.num; ++i) {
        tmp.filenames[i] = KONST[i].key;
        tmp.names[i] = KONST[i].value;
    }

    return tmp;
}

void levels_shutdown(struct LEVELS *lvls) {
    assert(lvls);
    free(lvls->filenames);
    free(lvls->names);
}

typedef int (*QSortCmpFunc)(void *a, void *b);
typedef void (*QSortSwapFunc)(size_t index1, size_t index2, void *udata);

struct QSortCtx {
    char *arr, *arr_initial;
    void *udata;
    size_t nmemb, size;
    QSortCmpFunc cmp;
    QSortSwapFunc swap;
};

void _koh_qsort_soa(struct QSortCtx *ctx) {
    if (ctx->nmemb < 2)
        return;

    char *swap_tmp[ctx->size];
    char *_arr = ctx->arr;
    char *pivot = _arr + ctx->size * (ctx->nmemb / 2);
    size_t i, j;
    for (i = 0, j = ctx->nmemb - 1; ; i++, j--) {
        // FIXME: Обратный порядок сортировки
        while (ctx->cmp(_arr + ctx->size * i, pivot) < 0) i++;
        while (ctx->cmp(_arr + ctx->size * j, pivot) > 0) j--;
        if (i >= j) break;

        char *i_ptr = _arr + i * ctx->size;
        char *j_ptr = _arr + j * ctx->size;
        memmove(swap_tmp, i_ptr, ctx->size);
        memmove(i_ptr, j_ptr, ctx->size);
        memmove(j_ptr, swap_tmp, ctx->size);
        size_t abs_i = (i_ptr - ctx->arr_initial) / ctx->size;
        size_t abs_j = (j_ptr - ctx->arr_initial) / ctx->size;
        if (ctx->swap) ctx->swap(abs_i, abs_j, ctx->udata);
    }

    struct QSortCtx ctx1;
    ctx1.arr = ctx->arr;
    ctx1.arr_initial = ctx->arr_initial;
    ctx1.cmp = ctx->cmp;
    ctx1.nmemb = i;
    ctx1.size = ctx->size;
    ctx1.swap = ctx->swap;
    ctx1.udata = ctx->udata;
    _koh_qsort_soa(&ctx1);
    struct QSortCtx ctx2;
    ctx2.arr = ctx->arr + ctx->size * i;
    ctx2.arr_initial = ctx->arr_initial;
    ctx2.cmp = ctx->cmp;
    ctx2.nmemb = ctx->nmemb - i;
    ctx2.size = ctx->size;
    ctx2.swap = ctx->swap;
    ctx2.udata = ctx->udata;
    _koh_qsort_soa(&ctx2);
}

void koh_qsort_soa(
    void *arr, size_t nmemb, size_t size, 
    QSortCmpFunc cmp, QSortSwapFunc swap,
    void *udata
) {
    struct QSortCtx ctx = {
        .nmemb = nmemb,
        .arr = arr,
        .arr_initial = arr,
        .cmp = cmp,
        .udata = udata,
        .swap = swap,
        .size = size,
    };
    _koh_qsort_soa(&ctx);
}

static int cmp(void *a, void *b) {
    char **_a = a, **_b = b;
    int res = strcmp(*_a, *_b);
    /*printf("cmp: %s, %s, res %d\n", *_a, *_b, res);*/
    return -1 * res;
}

static void swap(size_t index1, size_t index2, void *udata) {
    printf("swap: %zu, %zu\n", index1, index2);
    struct LEVELS *l = udata;
    char *tmp = l->names[index1];
    l->names[index1] = l->names[index2];
    l->names[index2] = tmp;
}

void levels_print(struct LEVELS *levels) {
    for (int i = 0; i < levels->num; i++) {
        printf("%s, %s\n", levels->filenames[i], levels->names[i]);
    }
}

void test_levels_sort() {
    struct LEVELS lvls = levels_init();
    levels_print(&lvls);
    printf("\n");
    /*printf("%s\n", lvls.filenames[0]);*/
    koh_qsort_soa(
        lvls.filenames, lvls.num, sizeof(lvls.filenames[0]), cmp, swap, &lvls
    );
    levels_print(&lvls);
    levels_shutdown(&lvls);
}

static int cmp_int(void *a, void *b) {
    int *_a = a, *_b = b;
    return *_a - *_b;
}

static int cmp_int_rev(void *a, void *b) {
    int *_a = a, *_b = b;
    return *_b - *_a;
}

void test_num_array_sort(int *arr, int len, QSortCmpFunc cmp) {
    printf("in: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    koh_qsort_soa(
        arr, len, sizeof(arr[0]), cmp, 
        NULL, NULL
    );
    printf("out: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void quicksort(int *A, int len, QSortCmpFunc cmp) {
    if (len < 2) return;

    int pivot = A[len / 2];

    int i, j;
    for (i = 0, j = len - 1; ; i++, j--) {
        while (cmp(&A[i], &pivot) < 0) i++;
        while (cmp(&A[j], &pivot) > 0) j--;

        if (i >= j) break;

        int temp = A[i];
        A[i]     = A[j];
        A[j]     = temp;
    }

    quicksort(A, i, cmp);
    quicksort(A + i, len - i, cmp);
}

int quicksort_test(QSortCmpFunc cmp) {
    int a[] = {4, 65, 2, -31, 0, 99, 2, 83, 782, 1};
    int n = sizeof a / sizeof a[0];

    int i;
    printf("in: ");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    quicksort(a, n, cmp);

    printf("out: ");
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    return 0;
}



int main() {
    test_levels_sort();

    int arr1[] = {0, 2, 3, 10, 9, 7, 6, 3, 1, -1, -2, 0, 17};
    int len1 = sizeof(arr1) / sizeof(arr1[0]);
    test_num_array_sort(arr1, len1, cmp_int);
    test_num_array_sort(arr1, len1, cmp_int_rev);

    int arr2[] = {0, 2, 3, 2, 9, 7, 16, 3, 2, 2, -2, 0, 27, -6};
    int len2 = sizeof(arr2) / sizeof(arr2[0]);
    test_num_array_sort(arr2, len2, cmp_int);
    test_num_array_sort(arr2, len2, cmp_int_rev);


    quicksort_test(cmp_int);
    quicksort_test(cmp_int_rev);
    return 0;
}
