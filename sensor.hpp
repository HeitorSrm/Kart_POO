#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string>

// Classe-base
class Sensor {
public:
    Sensor(std::string identificador, double leituraBruta);
    virtual ~Sensor(); // Destrutor virtual para permitir herança segura
    
    std::string identificador() const;
    double leituraBruta() const;
    void atualizarLeituraBruta(double valor);

    virtual bool requerAtencao() const = 0; // Método virtual puro para verificar se o sensor requer atenção

private:
    std::string identificador_;
    double leituraBruta_;
};

// Sensor de Temperatura do Motor (Herança)
class SensorTemperaturaMotor : public Sensor {
public:
    SensorTemperaturaMotor(std::string identificador, double leituraBruta, double limiteAlertaC);
    ~SensorTemperaturaMotor() override; // Destrutor virtual para permitir herança segura
    
    double temperaturaCelsius() const;
    bool temperaturaSegura() const;

    bool requerAtencao() const override; // Implementação do método virtual puro

private:
    double limiteAlertaC_;
};

// Sensor de Pressao do Pneu (Herança)
class SensorPressaoPneu : public Sensor {
public:
    SensorPressaoPneu(std::string identificador, double leituraBruta, double minimoKPa, double maximoKPa);
    
    double pressaoKPa() const;
    bool pressaoSegura() const;

    bool requerAtencao() const override; // Implementação do método virtual puro

private:
    double minimoKPa_;
    double maximoKPa_;
};

// Sensor de Luminosidade (Herança)
class SensorLuminosidade : public Sensor {
public:
    SensorLuminosidade(std::string identificador, double leituraBruta, double limiarFarolLux);
    
    double luminosidadeLux() const;
    bool deveAcenderFarois() const;

    bool requerAtencao() const override; // Implementação do método virtual puro

private:
    double limiarFarolLux_;
};

#endif