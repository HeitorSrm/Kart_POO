#include "kart.hpp"
#include <iostream>
#include <string>
#include <cmath> 
#include "bateria.hpp"
#include "energia.hpp"
#include "sensor.hpp"


Kart::Kart(std::string nome, std::string piloto, std::string chassi, std::string cor)
    : nome_{nome}, piloto_{piloto}, chassi_{chassi}, cor_{cor}, quantidadeVoltas_{0},
     melhorVolta_{0.0}, velocidadeKmh_{0}, combustivelLitros_{5.0},
      sensorPressao_{"PNEU_1", 82.0, 70.0, 90.0},
      sensorTemperatura_{"MOTOR_1", 92.0, 105.0}, 
      sensorLuminosidade_{"LUZ_1", 700.0, 150.0},         
      faroisLigados_{false} {}

std::string Kart::getNome() const{ return nome_;}
std::string Kart::getPiloto() const{ return piloto_;}
std::string Kart::getChassi() const{ return chassi_;}
std::string Kart::getCor() const{ return cor_;}
int Kart::getVelocidadeKmh() const{ return velocidadeKmh_;}
double Kart::getCombustivelLitros() const{ return combustivelLitros_;}
int Kart::quantidadeVoltas() const{ return quantidadeVoltas_;}
double Kart::getMelhorVolta() const{ return melhorVolta_;}

void Kart::setNome(std::string novoNome) {
    nome_ = novoNome;
}

bool Kart::trocarPiloto(std::string novoPiloto) {
    if (novoPiloto.empty()) { return false; }
    piloto_ = novoPiloto;
    return true;
}

bool Kart::repintar(std::string novaCor) {
    if (novaCor.empty()) { return false; }
    cor_ = novaCor;
    return true;
}

bool Kart::acelerar(int incrementoKmh) {
    if (incrementoKmh <= 0 || velocidadeKmh_ + incrementoKmh > 120) { return false; }
    velocidadeKmh_ += incrementoKmh;
    return true;
}

bool Kart::frear(int decrementoKmh) {
    if (decrementoKmh <= 0 || decrementoKmh > velocidadeKmh_) { return false; }
    velocidadeKmh_ -= decrementoKmh;
    return true;
}

bool Kart::abastecer(double litros) {
    if (litros <= 0.0 || combustivelLitros_ + litros > 10.0) { return false; }
    combustivelLitros_ += litros;
    return true;
}

bool Kart::registrarVolta(double tempo) {
    if (tempo <= 0.0) {
        std::cout << "Volta " << getNome() << " inválida!\n" << std::endl;
        return false;
    }

    quantidadeVoltas_++;

    if (quantidadeVoltas_ == 1 || tempo < melhorVolta_) {
        melhorVolta_ = tempo;
    }

    return true;
}
    
double Kart::pressaoPneu() const {
    return sensorPressao_.pressaoKPa();
}

bool Kart::pressaoSegura() const {
    return sensorPressao_.pressaoSegura();
}

double Kart::temperaturaMotor() const {
    return sensorTemperatura_.temperaturaCelsius();
}

bool Kart::temperaturaSegura() const {
    return sensorTemperatura_.temperaturaSegura();
}

double Kart::luminosidade() const {
    return sensorLuminosidade_.luminosidadeLux();
}

bool Kart::faroisLigados() const {
    return faroisLigados_;
}

void Kart::atualizarPressao(double kPa) {
    sensorPressao_.atualizarLeituraBruta(kPa);
}

void Kart::atualizarTemperatura(double celsius) {
    sensorTemperatura_.atualizarLeituraBruta(celsius);
}

void Kart::atualizarLuminosidade(double lux) {
    sensorLuminosidade_.atualizarLeituraBruta(lux);
}

void Kart::atualizarFarois() {
    faroisLigados_ = sensorLuminosidade_.deveAcenderFarois();
}

const Sensor& Kart::sensorPressao() const {
    return sensorPressao_;
}

const Sensor& Kart::sensorTemperatura() const {
    return sensorTemperatura_;
}

const Sensor& Kart::sensorLuminosidade() const {
    return sensorLuminosidade_;
}

Duracao::Duracao(long long segundos) : segundos_{segundos} {}

long long Duracao::emSegundos() const {
    return segundos_;
}

bool Duracao::operator<(Duracao outra) const {
    return segundos_ < outra.emSegundos();
}

bool Duracao::operator>(Duracao outra) const {
    return segundos_ > outra.emSegundos();
}

bool Duracao::operator==(Duracao outra) const {
    return segundos_ == outra.emSegundos();
}

bool Duracao::operator!=(Duracao outra) const {
    return segundos_ != outra.emSegundos();
}

bool Duracao::descontarSePossivel(Duracao outra) {
    if (outra.emSegundos() > segundos_) {
        return false;
    }
    segundos_ -= outra.emSegundos();
    return true;
}

Duracao& Duracao::operator-=(Duracao outra) {
    if (outra.emSegundos() > segundos_) {
        throw std::invalid_argument("Não é possível subtrair uma duração maior do que a atual.");
    }
    segundos_ -= outra.emSegundos();
    return *this;
}

Duracao& Duracao::operator+=(Duracao outra) {
    segundos_ += outra.emSegundos();
    return *this;
}

bool Kart::ativarEBoost(Bateria& bateria, Energia energiaNecessaria, int bonusKmh) {
    if (bonusKmh <= 0 || energiaNecessaria.emWh() <= 0) {
        return false;
    }

    int velocidadeAux = velocidadeKmh_ + bonusKmh;
    if (velocidadeAux > 120) {
        return false;
    }

    if (!bateria.podeFornecer(energiaNecessaria)) {
        return false;
    }

    bateria.consumir(energiaNecessaria);
    velocidadeKmh_ = velocidadeAux;

    return true;
}