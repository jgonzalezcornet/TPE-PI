#ifndef _safe_memory_h_
#define _safe_memory_h_

/*
    Funcion: safeMalloc
    Uso: ... = safeMalloc(...);
    ---------------------------
    Esta funcion aloca memoria al igual que la 
    funcion estandar "malloc" de stdlib.h,
    pero de manera segura, estableciendo 
    el errno en ENOMEM si no hay
    memoria suficiente.
*/

void * safeMalloc(size_t bytes);

/*
    Funcion: safeCalloc
    Uso: ... = safeCalloc(...);
    ---------------------------
    Esta funcion aloca memoria al igual que la
    funcion estandar "calloc" de stdlib.h,
    pero de manera segura, estableciendo 
    el errno en ENOMEM si no hay
    memoria suficiente.
*/

void * safeCalloc(size_t quan, size_t bytes);

#endif