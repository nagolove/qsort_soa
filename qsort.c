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

void koh_qsort_soa(
    void *arr, size_t nmemb, size_t size, 
    QSortCmpFunc cmp, QSortSwapFunc swap,
    void *udata
) {
    //printf("koh_qsort_soa: arr %p, nmemb %zu, size %zu\n", arr, nmemb, size);
    assert(arr);
    assert(cmp);

    if (nmemb < 2)
        return;

    char *swap_tmp[size];
    char *_arr = arr;
    char *pivot = _arr + size * (nmemb / 2);
    size_t i, j;
    for (i = 0, j = nmemb - 1; ; i++, j--) {
        // FIXME: Обратный порядок сортировки
        while (cmp(_arr + size * i, pivot) < 0) i++;
        while (cmp(_arr + size * j, pivot) > 0) j--;
        if (i >= j) break;

        memmove(swap_tmp, _arr + i * size, size);
        memmove(_arr + i * size, _arr + j * size, size);
        memmove(_arr + j * size, swap_tmp, size);
        if (swap) swap(i, j, udata);
    }

    koh_qsort_soa(arr, i, size, cmp, swap, udata);
    koh_qsort_soa(_arr + size * i, nmemb - i, size, cmp, swap, udata);
}


static int cmp(void *a, void *b) {
    char **_a = a, **_b = b;
    int res = strcmp(*_a, *_b);
    printf("cmp: %s, %s, res %d\n", *_a, *_b, res);
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

void test_num_array_sort(QSortCmpFunc cmp) {
    int arr[] = {0, 2, 3, 10, 9, 7, 6, 3, 1, -1, -2, 0, 17};
    int len = sizeof(arr) / sizeof(arr[0]);
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    printf("\n");

    koh_qsort_soa(
        arr, len, sizeof(arr[0]), cmp, 
        NULL, NULL
    );
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
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
    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    printf("\n");

    quicksort(a, n, cmp);

    for (i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    printf("\n");

    return 0;
}



int main() {
    test_levels_sort();
    test_num_array_sort(cmp_int);
    test_num_array_sort(cmp_int_rev);
    quicksort_test(cmp_int);
    quicksort_test(cmp_int_rev);
    return 0;
}
