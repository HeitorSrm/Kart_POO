#include "bateria.hpp"

Bateria::Bateria(int capacidadeWh, int cargaInicialWh)
    : capacidadeWh_{capacidadeWh}, cargaWh_{cargaInicialWh} {}

int Bateria::getCapacidadeWh() const { return capacidadeWh_; }
int Bateria::getCargaWh() const { return cargaWh_; }

bool Bateria::podeReceber(Energia quantidade) const {
    if (quantidade.emWh() <= 0) {
        return false;
    }
    return (cargaWh_ + quantidade.emWh()) <= capacidadeWh_;
}

bool Bateria::podeFornecer(Energia quantidade) const {
    if (quantidade.emWh() <= 0) {
        return false;
    }
    return quantidade.emWh() <= cargaWh_;
}

bool Bateria::carregar(Energia quantidade) {
    if (!podeReceber(quantidade)) {
        return false;
    }
    cargaWh_ += quantidade.emWh();
    return true;
}

bool Bateria::consumir(Energia quantidade) {
    if (!podeFornecer(quantidade)) {
        return false;
    }
    cargaWh_ -= quantidade.emWh();
    return true;
}