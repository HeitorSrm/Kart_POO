#ifndef ECOP13A_LAB08_KART_CONFIGURAVEL_HPP
#define ECOP13A_LAB08_KART_CONFIGURAVEL_HPP
#include "kart.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <type_traits>

// Seguranca didatica: nao permita que unique_ptr<Sensor> possua um derivado
// antes de a base declarar destrutor virtual. Exerc. 2 precede o 4.
static_assert(std::has_virtual_destructor<Sensor>::value,
              "LAB08: acrescente virtual ~Sensor() na classe-base antes de E4.");

// LAB08: Uma configuracao CONTEM o Kart anterior; nao herda dele.
// O Kart original continua copiavel e com seus tres sensores fixos.
// Somente esta nova configuracao expressa posse exclusiva dos opcionais.
class KartConfiguravel {
public:
    KartConfiguravel(std::string nome, std::string piloto,
                     std::string chassi, std::string cor);
    // A configuracao nao define clonagem dos opcionais. Nao inventamos copia
    // silenciosa nem transformamos unique_ptr em um dono compartilhado.
    KartConfiguravel(const KartConfiguravel&) = delete;
    KartConfiguravel& operator=(const KartConfiguravel&) = delete;
    KartConfiguravel(KartConfiguravel&&) = default;
    KartConfiguravel& operator=(KartConfiguravel&&) = default;

    Kart& kart();
    const Kart& kart() const;
    // LAB08 4a — contrato de inclusao: recebe posse; vazio nao entra.
    bool adicionarSensorExtra(std::unique_ptr<Sensor> sensor);
    // LAB08 4b — observar quantidade sem modificar a colecao.
    std::size_t quantidadeSensoresExtras() const;
    // LAB08 4c — acesso emprestado; indice invalido produz nullptr.
    // Valido enquanto a configuracao mantiver esse objeto e ele nao for removido.
    const Sensor* sensorExtra(std::size_t indice) const;
    // LAB08 4d — altera apenas a leitura do indice valido.
    bool atualizarSensorExtra(std::size_t indice, double leitura);
private:
    Kart kart_;
    std::vector<std::unique_ptr<Sensor>> sensoresExtras_;
};
#endif
