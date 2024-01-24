// main.c
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>

void localFunction() {
    printf("Local function in the program\n");
}

double measureTime(void (*func)()) {
    clock_t start, end;
    start = clock();
    
    // Llama a la función 1 millón de veces (ajusta según sea necesario)
    for (int i = 0; i < 1000000; ++i) {
        func();
    }
    
    end = clock();
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    void *lib1 = dlopen("./liblib1.so", RTLD_NOW | RTLD_GLOBAL);
    void *lib2 = dlopen("./liblib2.so", RTLD_NOW | RTLD_GLOBAL);

    if (lib1 == NULL || lib2 == NULL) {
        fprintf(stderr, "Error al cargar las bibliotecas: %s\n", dlerror());
        return 1;
    }

    // Obtén punteros a las funciones
    void (*func1)() = dlsym(lib1, "func1");
    void (*func2)() = dlsym(lib2, "func2");

    if (func1 == NULL || func2 == NULL) {
        fprintf(stderr, "Error al obtener punteros a las funciones: %s\n", dlerror());
        return 1;
    }


    // Mide el tiempo promedio de ejecución de func1
    double avgFunc1Time = measureTime(func1);

    // Mide el tiempo promedio de ejecución de func2
    double avgFunc2Time = measureTime(func2);
    
    // Mide el tiempo promedio de ejecución de la función local
    double avgLocalTime = measureTime(localFunction);
    
    printf("Tiempo promedio de ejecución de la función local: %f segundos\n", avgLocalTime);
    printf("Tiempo promedio de ejecución de func1: %f segundos\n", avgFunc1Time);
    printf("Tiempo promedio de ejecución de func2: %f segundos\n", avgFunc2Time);

    // Cierra las bibliotecas
    dlclose(lib1);
    dlclose(lib2);

    return 0;
}
