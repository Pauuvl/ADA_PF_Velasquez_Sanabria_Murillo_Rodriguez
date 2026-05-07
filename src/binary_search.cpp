#include "binary_search.hpp"

int busquedaBinaria(const std::vector<Solicitud>& arr, int izq, int der, int k) {

    if (izq > der)
        return -1;

    int mid = izq + (der - izq) / 2;

    // Encontró exactamente el tenure buscado
    if (arr[mid].tenure == k)
        return mid;

    // Como está ordenado DESCENDENTE
    if (arr[mid].tenure < k)
        return busquedaBinaria(arr, izq, mid - 1, k);

    return busquedaBinaria(arr, mid + 1, der, k);
}