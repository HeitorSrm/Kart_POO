#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "kart.hpp"
#include "sensor.hpp"
#include "telemetria_io.hpp"
#include "entrada.hpp"

void display(const Kart& kart) {
    std::cout << "=== TELEMETRIA DO KART ===\n\n";
    
    std::cout << "PNEUS\n";
    std::cout << "Pressao ............. " << kart.pressaoPneu() << " kPa\n";
    std::cout << "Estado .............. " << (kart.pressaoSegura() ? "OK" : "ALERTA") << "\n\n";
    
    std::cout << "MOTOR\n";
    std::cout << "Temperatura ......... " << kart.temperaturaMotor() << " C\n";
    std::cout << "Estado .............. " << (kart.temperaturaSegura() ? "OK" : "ALERTA") << "\n\n";
    
    std::cout << "AMBIENTE\n";
    std::cout << "Luminosidade ........ " << kart.luminosidade() << " lux\n";
    std::cout << "Farois .............. " << (kart.faroisLigados() ? "LIGADOS" : "desligados") << "\n\n";
    
    std::cout << "KART\n";
    std::cout << "Velocidade .......... " << kart.getVelocidadeKmh() << " km/h\n";
    std::cout << "=============================\n\n";
}

void displayStatus(const Sensor& sensor) {
    std::cout << "Sensor " << sensor.identificador() << ": " 
              << (sensor.requerAtencao() ? "ATENCAO" : "OK") << "\n";
}

// LAB08 Etapa 3: Mostra que o sensor local encerra o sensor em qualquer saída
void diagnosticarCaminhos(bool sairCedo) {
    std::unique_ptr<Sensor> sensor = std::make_unique<SensorTemperaturaMotor>( sairCedo ? "CAMINHO-CEDO" : "CAMINHO-NORMAL", 90.0,105.0);
    std::cout << "Sensor criado: " << sensor->identificador() << "\n";

    if (sairCedo) {
        std::cout << "Saindo cedo...\n";
        return; // Retorna antes de consultar o sensor
    }

    displayStatus(*sensor);
    std::cout << "Saindo normalmente...\n";
} // Dono local sai do escopo

// --- LAB09: do arquivo as leituras confiaveis ---
// Retorna 0 em sucesso, 2 se a carga falhou, 3 se o boletim nao foi escrito.
int executarLab09(const std::string& entrada, const std::string& boletim) {
    std::cout << "\n--- LAB09: Telemetria do Cometa ---\n";

    // 5a) carrega e confere os indicadores ANTES de usar qualquer dado
    const ResultadoCarga carga = carregar_arquivo(entrada);
    if (!carga.arquivo_aberto || carga.falha_leitura) {
        for (const auto& erro : carga.erros) std::cerr << erro << '\n';
        return 2; // nada e aplicado ao Kart
    }

    // 5b) so agora constroi o Kart e aplica as aceitas, na ordem recebida
    Kart cometa{"Cometa Solar", "Mario", "CH-002", "azul"};
    for (const LeituraValidada& leitura : carga.aceitas) {
        aplicar_no_kart(cometa, leitura);
    }

    // 5c) gera o boletim e confere o resultado da escrita
    if (!escrever_boletim(boletim, cometa, carga)) {
        std::cerr << "nao foi possivel escrever: " << boletim << '\n';
        return 3;
    }

    // resumo no terminal
    std::cout << "arquivo_aberto=true\n";
    std::cout << "aceitas=" << carga.aceitas.size()
              << " recusadas=" << carga.erros.size() << '\n';
    for (const auto& erro : carga.erros) std::cout << "erro " << erro << '\n';
    std::cout << "boletim=" << boletim << '\n';
    return 0;
}

template <typename T>
void testarParse(const std::string& texto) {
    T valor{};
    std::cout << "  \"" << texto << "\" -> ";
    if (parse_numero(texto, valor)) {
        std::cout << "aceito (" << valor << ")\n";
    } else {
        std::cout << "recusado\n";
    }
}

void demonstrarParseNumero() {
    std::cout << "\n--- parse_numero<T> ---\n";
    std::cout << "T = int:\n";
    testarParse<int>("12");
    testarParse<int>("12abc");
    testarParse<int>("12.5");
    std::cout << "T = double:\n";
    testarParse<double>("12.5");
    testarParse<double>("NaN");
    testarParse<double>("");
}

int main() {
    // --- TESTES DO KART ---
    Kart kart1("Cometa", "Mario", "CH-001", "Vermelho");

    kart1.acelerar(30);
    kart1.acelerar(70);
    kart1.frear(20);
    
    const int antes = kart1.getVelocidadeKmh();
    const bool aceitou = kart1.frear(30); // Vai recusar
    const int depois = kart1.getVelocidadeKmh();

    std::cout << "Velocidade antes de frear: " << antes << " km/h" << std::endl;
    std::cout << "Tentativa de frear 30 km/h: " << (aceitou ? "Aceita" : "Recusada") << std::endl;
    std::cout << "Velocidade depois de frear: " << depois << " km/h\n" << std::endl;

    kart1.abastecer(3.0);
    kart1.registrarVolta(61.8);
    kart1.registrarVolta(60.9);
    kart1.registrarVolta(-2.0); // Vai recusar

    // --- 5. TESTE DOS SENSORES ---
    kart1.frear(kart1.getVelocidadeKmh()); 
    kart1.acelerar(50);

    // Passo 1: Valores iniciais seguros
    kart1.atualizarPressao(82.0);
    kart1.atualizarTemperatura(92.0);
    kart1.atualizarLuminosidade(700.0);
    kart1.atualizarFarois();

    std::cout << "\n--- ESTADO INICIAL (displayStatus) ---\n";
    displayStatus(kart1.sensorPressao());
    displayStatus(kart1.sensorTemperatura());
    displayStatus(kart1.sensorLuminosidade());

    std::cout << "\n--- LEITURA INICIAL ---\n";
    display(kart1);

    // Passo 2: Experiência de Referência e Virtualidade
    std::cout << "\n--- EXPERIENCIA DE REFERENCIA ---\n";
    const Sensor& ref = kart1.sensorTemperatura();
    
    std::cout << "Requer atencao antes ? " << (ref.requerAtencao() ? "true" : "false") << '\n';
    
    kart1.atualizarTemperatura(108.0); // Modifica a temperatura internamente pelo Kart
    
    std::cout << "Requer atencao depois ? " << (ref.requerAtencao() ? "true" : "false") << '\n';

    // Passo 3: Colocar o restante dos sensores em alerta
    kart1.atualizarPressao(62.0);
    kart1.atualizarLuminosidade(120.0);
    kart1.atualizarFarois();

    std::cout << "\n--- ESTADO DE ALERTA (displayStatus) ---\n";
    displayStatus(kart1.sensorPressao());
    displayStatus(kart1.sensorTemperatura());
    displayStatus(kart1.sensorLuminosidade());

    std::cout << "\n--- NOVA LEITURA ---\n";
    display(kart1);

    // --- LAB08 Etapa 1: Observadores (consultam sem possuir) ---
    std::cout << "\n--- LAB08 Etapa 1: Observadores ---\n";

    // a) lista de ponteiros emprestados: nenhum deles é dono do sensor; todos são observadores.
    std::vector<const Sensor*> observadores;
    observadores.push_back(&kart1.sensorPressao());
    observadores.push_back(&kart1.sensorTemperatura());
    observadores.push_back(&kart1.sensorLuminosidade());

    // b) percorre a interface comum
    for (const Sensor* observador : observadores) {
        displayStatus(*observador);
    }

    // c) apagam só os acessos, não os sensores originais. O Kart continua dono de seus sensores.
    observadores.clear();
    std::cout << "Observadores apos clear(): " << observadores.size() << std::endl;
    displayStatus(kart1.sensorTemperatura());

    // --- LAB08 Etapa 2: Destruição visivel (DIAG-MOTOR) ---
    std::cout << "\n--- LAB08 Etapa 2: DIAG-MOTOR ---\n";
    {
        std::unique_ptr<Sensor> diag = std::make_unique<SensorTemperaturaMotor>("DIAG-MOTOR", 90.0, 105.0);
        displayStatus(*diag);
        std::cout << "Saindo do bloco...\n";
    }
    std::cout << "Bloco encerrado. O destrutor do sensor DIAG-MOTOR foi chamado.\n";

    // --- LAB08 Etapa 3: RAII nos dois caminhos ---
    std::cout << "\n--- LAB08 Etapa 3: Caminhos ---\n";
    diagnosticarCaminhos(true);  // Sair cedo
    diagnosticarCaminhos(false); // Sair normalmente

    demonstrarParseNumero();

    // --- LAB09 ---
    const int codigo09 = executarLab09("dados/telemetria_cometa.csv", "boletim_cometa.txt");
    if (codigo09 != 0) return codigo09;

    return 0;
}
