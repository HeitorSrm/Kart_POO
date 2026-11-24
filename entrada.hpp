#ifndef ENTRADA_HPP
#define ENTRADA_HPP

#include <cmath>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>

// Converte o texto INTEIRO para um numero do tipo T (int, double, ...).
// Recusa "", "abc", "12abc" e "NaN"; para tipos inteiros recusa tambem "12.5".
// Em caso de falha devolve false e deixa 'valor' como estava.
//
// T nao pode ser deduzido dos argumentos (ele aparece so no 2o parametro,
// que e uma referencia), entao em geral e dado explicitamente: parse_numero<int>(...).
template <typename T>
bool parse_numero(const std::string& texto, T& valor) {
    static_assert(std::is_arithmetic_v<T>, "parse_numero<T>: T precisa ser numerico");

    std::istringstream entrada{texto};
    entrada.imbue(std::locale::classic());   // ponto decimal, sem depender do sistema

    T lido{};
    entrada >> lido;
    if (!entrada) return false;              // nao leu nenhum numero (ou estourou o tipo)

    entrada >> std::ws;                      // aceita espacos no fim...
    if (!entrada.eof()) return false;        // ...mas nada alem deles ("12abc" cai aqui)

    if constexpr (std::is_floating_point_v<T>) {
        if (!std::isfinite(lido)) return false;   // so existe para double/float
    }

    valor = lido;
    return true;
}

#endif
