#ifndef BATERIA_HPP
#define BATERIA_HPP

#include "energia.hpp"

class Bateria {
public:
    Bateria(int capacidadeWh, int cargaInicialWh);

    int getCapacidadeWh() const;
    int getCargaWh() const;
    bool podeReceber(Energia quantidade) const;
    bool podeFornecer(Energia quantidade) const;
    bool carregar(Energia quantidade);
    bool consumir(Energia quantidade);

private:
    int capacidadeWh_;
    int cargaWh_;
};

#endif