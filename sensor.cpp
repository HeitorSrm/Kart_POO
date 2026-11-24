#include "sensor.hpp"
#include <iostream>
#include <string>

#include "sensor.hpp"

//Sensor
Sensor::Sensor(std::string identificador, double leituraBruta)
    : identificador_(identificador), leituraBruta_(leituraBruta) {}
    Sensor::~Sensor() {
        std::cout << "[fim sensor] " << identificador_ << std::endl;
    }

std::string Sensor::identificador() const {
    return identificador_;
}

double Sensor::leituraBruta() const {
    return leituraBruta_;
}

void Sensor::atualizarLeituraBruta(double valor) {
    leituraBruta_ = valor;
}

//SensorTemperaturaMotor
SensorTemperaturaMotor::SensorTemperaturaMotor(std::string identificador, double leituraBruta, double limiteAlertaC)
    : Sensor(identificador, leituraBruta), limiteAlertaC_(limiteAlertaC) {}
    SensorTemperaturaMotor::~SensorTemperaturaMotor() {
        std::cout << "[fim sensor temperatura] " << identificador() << std::endl;
    }

double SensorTemperaturaMotor::temperaturaCelsius() const {
    return leituraBruta();
}

bool SensorTemperaturaMotor::requerAtencao() const {
    return temperaturaCelsius() > limiteAlertaC_;
}

bool SensorTemperaturaMotor::temperaturaSegura() const {
    return !requerAtencao();
}
   
//SensorPressaoPneu
SensorPressaoPneu::SensorPressaoPneu(std::string identificador, double leituraBruta, double minimoKPa, double maximoKPa)
    : Sensor(identificador, leituraBruta), minimoKPa_(minimoKPa), maximoKPa_(maximoKPa) {}

double SensorPressaoPneu::pressaoKPa() const {
    return leituraBruta();
}

bool SensorPressaoPneu::requerAtencao() const {
    double pressao = leituraBruta();
    return (pressao < minimoKPa_ || pressao > maximoKPa_);
}

bool SensorPressaoPneu::pressaoSegura() const {
    return !requerAtencao();
}

//SensorLuminosidade
SensorLuminosidade::SensorLuminosidade(std::string identificador, double leituraBruta, double limiarFarolLux)
    : Sensor(identificador, leituraBruta), limiarFarolLux_(limiarFarolLux) {}

double SensorLuminosidade::luminosidadeLux() const {
    return leituraBruta();
}

bool SensorLuminosidade::requerAtencao() const {
    return leituraBruta() < limiarFarolLux_;
}

bool SensorLuminosidade::deveAcenderFarois() const {
    return requerAtencao();
}