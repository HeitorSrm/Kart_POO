#ifndef POWERBANK_HPP
#define POWERBANK_HPP

#include "energia.hpp"
#include "bateria.hpp"

class PowerBank {
public:
    explicit PowerBank(int capacidadeWh);
    int getEnergiaArmazenadaWh() const;
    int getCapacidadeWh() const;
    bool carregar(Energia quantidade);
    bool transferirPara(Bateria& bateria, Energia quantidade);
private:
    int capacidadeWh_;
    int energiaArmazenadaWh_;
};

#endif