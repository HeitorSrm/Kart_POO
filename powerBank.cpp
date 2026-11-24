#include "powerBank.hpp"
#include "bateria.hpp"
#include "energia.hpp"

PowerBank::PowerBank(int capacidadeWh)
    : capacidadeWh_{capacidadeWh}, energiaArmazenadaWh_{0} {}

int PowerBank::getEnergiaArmazenadaWh() const {return energiaArmazenadaWh_;}
int PowerBank::getCapacidadeWh() const {return capacidadeWh_;}

bool PowerBank::carregar(Energia quantidade) {
    if (quantidade.emWh() <= 0) {
        return false;
    }

    const int aux = energiaArmazenadaWh_ + quantidade.emWh();
    if (aux > capacidadeWh_) { 
        return false;
    }
    energiaArmazenadaWh_ = aux;
    return true; 
}

bool PowerBank::transferirPara(Bateria& bateria, Energia quantidade) {
    if (quantidade.emWh() <= 0) {
        return false;
    }
    if (energiaArmazenadaWh_ < quantidade.emWh()) {
        return false;
    }
    if (!bateria.podeReceber(quantidade)) {
        return false;
    }
    if (!bateria.carregar(quantidade)) {
        return false;
    }
    energiaArmazenadaWh_ -= quantidade.emWh();
    bateria.carregar(quantidade);
    return true;
}

