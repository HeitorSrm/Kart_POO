#ifndef KART_HPP
#define KART_HPP
#include "bateria.hpp"
#include "energia.hpp"
#include "sensor.hpp"

#include <string>

class Kart {
public:
    Kart(std::string nome, std::string piloto, std::string chassi, std::string cor);

    std::string getNome() const;
    std::string getPiloto() const;
    std::string getChassi() const;
    std::string getCor() const;
    int getVelocidadeKmh() const;
    double getCombustivelLitros() const;
    int quantidadeVoltas() const;
    double getMelhorVolta() const;

    void setNome(std::string novoNome);
    bool trocarPiloto(std::string novoPiloto);
    bool repintar(std::string novaCor);

    bool acelerar(int incrementoKmh);
    bool frear(int decrementoKmh);
    bool abastecer(double litros);
    bool registrarVolta(double tempo);

    bool ativarEBoost(Bateria& bateria, Energia energiaNecessaria, int bonusKmh);

    double pressaoPneu() const;
    bool pressaoSegura() const;
    
    double temperaturaMotor() const;
    bool temperaturaSegura() const;
    
    double luminosidade() const;
    bool faroisLigados() const;

    void atualizarPressao(double kPa);
    void atualizarTemperatura(double celsius);
    void atualizarLuminosidade(double lux);
    void atualizarFarois();

    const Sensor& sensorPressao() const;
    const Sensor& sensorTemperatura() const;
    const Sensor& sensorLuminosidade() const;
    
private:
    std::string nome_;
    std::string piloto_;
    std::string chassi_;
    std::string cor_;
    int velocidadeKmh_;
    double combustivelLitros_;
    int quantidadeVoltas_;
    double melhorVolta_; 

    SensorPressaoPneu sensorPressao_;
    SensorTemperaturaMotor sensorTemperatura_;
    SensorLuminosidade sensorLuminosidade_;
    bool faroisLigados_;
};

class Duracao {
    public:
        explicit Duracao(long long segundos);
        long long emSegundos() const;

        bool operator<(Duracao outra) const;
        bool operator>(Duracao outra) const;
        bool operator==(Duracao outra) const;
        bool operator!=(Duracao outra) const;
        bool descontarSePossivel(Duracao outra);
        Duracao& operator+=(Duracao outra);
        Duracao& operator-=(Duracao outra);

    private:
        long long segundos_;
};

#endif
