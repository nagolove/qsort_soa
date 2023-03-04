Standart C library-like qsort() function with callbacks for sorting structure of arrays data.

```c
structure S {
  int a, b, c, d;
};
size_t num = 1000;
structure S *arr = malloc(num * sizeof(structure S));
qsort(arr, ...)
```

```c

structure S {
  int *a, *b, *c, *d;
};

static swap(size_t index1, size_t index2, void *udata) {
  struct S *s = udata;
  int tmp;
  
  tmp = s->b[index1];
  s->b[index1] = s->b[index2];
  s->b[index2] = tmp;
  
  tmp = s->c[index1];
  s->c[index1] = s->c[index2];
  s->c[index2] = tmp;
  
  tmp = s->d[index1];
  s->d[index1] = s->d[index2];
  s->d[index2] = tmp;
}


size_t num = 1000;
structure S s;
// init arrays
s.a = malloc(num * sizeof(s.a[0]));
...
qsort_soa(
  s.a             // key sorting array
  num             // number of elements
  sizeof(s.a[0])  // element size
  cmp,            // compare function like standard qsort() callback
  swap,           // swap callback for non-keys array elements
  &s              // user data passed to swap function
);
```
