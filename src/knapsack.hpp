#pragma once
#include "parser.hpp"
#include <vector>
#include <string>

// Representa un ítem de la mochila construido a partir de una Solicitud
struct ItemMochila {
    std::string customerID;
    int    tenure;
    int    peso;     // floor(TotalCharges / 100), mínimo 1
    double valor;    // MonthlyCharges (ingreso mensual)
};

// Resultado de la mochila resuelta por Programación Dinámica
struct ResultadoMochila {
    double valorOptimo;
    int    pesoUsado;
    std::vector<int> seleccionados;  // índices de los ítems elegidos
};

// Resultado del enfoque codicioso (para el contraejemplo)
struct ResultadoGreedy {
    double valorTotal;
    int    pesoUsado;
    std::vector<int> seleccionados;
};

// Filtra Churn=No y toma las primeras 50 del arreglo ya ordenado por tenure desc
std::vector<ItemMochila> prepararItems(const std::vector<Solicitud>& solicitudesOrdenadas);

// Resuelve Mochila 0-1 por tabulación + backtracking
ResultadoMochila mochila01(const std::vector<ItemMochila>& items, int capacidad);

// Resuelve Mochila 0-1 con enfoque codicioso por ratio valor/peso
ResultadoGreedy  mochilaCodiciosa(const std::vector<ItemMochila>& items, int capacidad);