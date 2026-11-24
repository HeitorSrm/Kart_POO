#include "solar.hpp"
#include <iostream>
#include <string>
#include <cmath>

// ---Tempo de Sol---
TempoSol::TempoSol(int minutos) 
    : minutos_{minutos} {}

int TempoSol::emMinutos() const { return minutos_; }

TempoSol& TempoSol::operator+=(TempoSol outro) {
    minutos_ += outro.minutos_;
    return *this;
}

// ---Potência Solar---
PotenciaSolar::PotenciaSolar(int watts)
    : watts_{watts} {}

int PotenciaSolar::emWatts() const { return watts_; }

// ---Placa Solar---
PlacaSolar::PlacaSolar(PotenciaSolar potencia)
    : potencia_{potencia} {}

PotenciaSolar PlacaSolar::getPotencia() const {
    return potencia_;
}

Energia PlacaSolar::gerar(TempoSol tempo) const {
    int energiaGerada = (potencia_.emWatts() * tempo.emMinutos()) / 60;
    return Energia{energiaGerada};
}

