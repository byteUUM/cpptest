int *p = NULL; //初始化置NULL
p = (int *)malloc(sizeof(int)*n); //申请n个int内存空间
assert(p != NULL); //判空，防错设计
p = (int *) realloc(p, 25);//重新分配内存, p 所指向的内存块会被释放并分配一个新的内存地址
free(p);
p = NULL; //释放后置空
int *p1 = NULL; //初始化置NULL
p1 = (int *)calloc(n, sizeof(int)); //申请n个int内存空间同时初始化为0
assert(p1 != NULL); //判空，防错设计
free(p1);
p1 = NULL; //释放后置空
int *p2 = NULL; //初始化置NULL
p2 = new int[n]; //申请n个int内存空间
assert(p2 != NULL); //判空，防错设计
delete []p2;
p2 = nullptr; //释放后置空
