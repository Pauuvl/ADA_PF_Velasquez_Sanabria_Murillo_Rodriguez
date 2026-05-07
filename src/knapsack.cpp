#include "knapsack.hpp"
#include <algorithm>
#include <cmath>

// Toma las primeras 50 solicitudes activas (Churn = No) del arreglo
// ya ordenado por tenure descendente. Mapea cada una a un ItemMochila.
std::vector<ItemMochila> prepararItems(const std::vector<Solicitud>& solicitudes) {
    std::vector<ItemMochila> items;
    items.reserve(50);

    for (const auto& s : solicitudes) {
        if (s.churn) continue;                         // descartar Churn = Yes

        ItemMochila item;
        item.customerID = s.customerID;
        item.tenure     = s.tenure;

        // Peso = TotalCharges / 100, con mínimo 1 para evitar peso 0
        int pesoCalculado = (int)std::floor(s.totalCharges / 100.0);
        item.peso  = (pesoCalculado < 1) ? 1 : pesoCalculado;
        item.valor = s.monthlyCharges;

        items.push_back(item);
        if ((int)items.size() == 50) break;            // top-50 antigüedad
    }
    return items;
}

// Mochila 0-1 por Programación Dinámica (tabulación) + backtracking
ResultadoMochila mochila01(const std::vector<ItemMochila>& items, int capacidad) {
    int n = (int)items.size();

    // Tabla dp de tamaño (n+1) x (capacidad+1) inicializada en 0
    std::vector<std::vector<double>> dp(n + 1, std::vector<double>(capacidad + 1, 0.0));

    // Llenado de la tabla por filas (ítems) y columnas (capacidades)
    for (int i = 1; i <= n; ++i) {
        int    pesoActual  = items[i - 1].peso;
        double valorActual = items[i - 1].valor;

        for (int w = 0; w <= capacidad; ++w) {
            // Opción 1: no tomar el ítem i
            dp[i][w] = dp[i - 1][w];

            // Opción 2: tomar el ítem i (si cabe)
            if (pesoActual <= w) {
                double candidato = dp[i - 1][w - pesoActual] + valorActual;
                if (candidato > dp[i][w]) {
                    dp[i][w] = candidato;
                }
            }
        }
    }

    // Backtracking para recuperar qué ítems fueron tomados
    ResultadoMochila resultado;
    resultado.valorOptimo = dp[n][capacidad];

    int w = capacidad;
    for (int i = n; i >= 1; --i) {
        // Si el valor cambió respecto a no tomar el ítem, entonces fue tomado
        if (dp[i][w] != dp[i - 1][w]) {
            resultado.seleccionados.push_back(i - 1);  // índice 0-based
            w -= items[i - 1].peso;
        }
    }
    std::reverse(resultado.seleccionados.begin(), resultado.seleccionados.end());
    resultado.pesoUsado = capacidad - w;
    return resultado;
}

// Enfoque codicioso: ordena por ratio valor/peso descendente y toma
// los ítems mientras quepan. Sirve para construir el contraejemplo.
ResultadoGreedy mochilaCodiciosa(const std::vector<ItemMochila>& items, int capacidad) {
    int n = (int)items.size();

    // Vector de índices que se ordena por ratio valor/peso descendente
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i) indices[i] = i;

    std::sort(indices.begin(), indices.end(), [&](int a, int b) {
        double ratioA = items[a].valor / (double)items[a].peso;
        double ratioB = items[b].valor / (double)items[b].peso;
        return ratioA > ratioB;
    });

    ResultadoGreedy resultado;
    resultado.valorTotal = 0.0;
    resultado.pesoUsado  = 0;

    int capacidadRestante = capacidad;
    for (int idx : indices) {
        if (items[idx].peso <= capacidadRestante) {
            resultado.seleccionados.push_back(idx);
            capacidadRestante     -= items[idx].peso;
            resultado.valorTotal  += items[idx].valor;
            resultado.pesoUsado   += items[idx].peso;
        }
    }
    std::sort(resultado.seleccionados.begin(), resultado.seleccionados.end());
    return resultado;
}