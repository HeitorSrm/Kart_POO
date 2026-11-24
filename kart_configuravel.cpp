#include "kart_configuravel.hpp"
#include <utility>

KartConfiguravel::KartConfiguravel(std::string nome, std::string piloto,
                                 std::string chassi, std::string cor)
    : kart_(std::move(nome), std::move(piloto),
            std::move(chassi), std::move(cor)) {}

// Sobrecargas de consulta: nao duplicam o Kart nem mudam seus getters.
Kart& KartConfiguravel::kart() { return kart_; }
const Kart& KartConfiguravel::kart() const { return kart_; }

bool KartConfiguravel::adicionarSensorExtra(std::unique_ptr<Sensor> sensor) {
    // LAB08 Etapa: 4a — TODO 1: Contrato: um proprietario vazio deve ser recusado (false), e não vazio deve ser aceito (true).
    if (!sensor) {
        return false; // Recusa sensor nulo
    }
    sensoresExtras_.push_back(std::move(sensor)); // Aceita sensor válido
    return true;
}
std::size_t KartConfiguravel::quantidadeSensoresExtras() const {
    // LAB08 Etapa: 4b — TODO 2: Contrato: informe quantos sensores estao de fato guardados. 
    return sensoresExtras_.size();
}
const Sensor* KartConfiguravel::sensorExtra(std::size_t indice) const {
    // LAB08 Etapa: 4c — TODO 3: Contrato: indice valido oferece acesso somente para leitura ao MESMO sensor. Indice fora da colecao devolve nullptr.
    if (indice  >= sensoresExtras_.size()) {
        return nullptr; // Indice invalido
    }
    return sensoresExtras_[indice].get(); // Retorna ponteiro para o sensor
}
bool KartConfiguravel::atualizarSensorExtra(std::size_t indice, double leitura) {
    // LAB08 Etapa: 4d — TODO 4: Contrato: indice valido atualiza apenas a leitura daquele sensor e devolve true, preservando suas caracteristicas. Indice invalido devolve false.
    if (indice >= sensoresExtras_.size()) {
        return false; // Indice invalido
    }
    sensoresExtras_[indice]->atualizarLeituraBruta(leitura); // Atualiza a leitura do sensor
    return true;
}
