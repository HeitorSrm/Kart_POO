#ifndef SOLAR_HPP
#define SOLAR_HPP

#include "energia.hpp"

class TempoSol {
public:
    explicit TempoSol(int minutos = 0);
    int emMinutos() const;
    
    TempoSol& operator+=(TempoSol outro);
private:
    int minutos_;
};

class PotenciaSolar {
public:
    explicit PotenciaSolar(int watts = 0);
    int emWatts() const;
private:
    int watts_;
};

class PlacaSolar {
public:
    explicit PlacaSolar(PotenciaSolar potencia);
    PotenciaSolar getPotencia() const;
    Energia gerar(TempoSol tempo) const;
private:
    PotenciaSolar potencia_;
};

#endif