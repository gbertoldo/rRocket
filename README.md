# rRocket 🚀 

rRocket é um projeto _open-source_ de computador de bordo para minifoguetes baseado em Arduino visando a educação na área aeroespacial. 

# Características principais
- Medição de altura acima do nível do solo por barômetro;
- Capacidade de registro da componente vertical da trajetória na memória permanente (1024 bytes) do Arduino;
- Registro de dados de voo com frequência de 10 Hz até o apogeu;
- Registro de dados de voo com frequência definida pelo usuário após o apogeu;
- Capacidade de acionamento de dois paraquedas: o auxiliar (drogue) no apogeu e o principal em altura definida pelo usuário;
- Controle através do algoritmo de Máquina de Estados Finitos;
- Aplicação do filtro de Kalman para detecção de apogeu e filtro alfa dinâmico para oscilações de velocidade;
- Comunicação de apogeu através de sinal sonoro e luninoso;
- Relatório de erros;
- Capacidade de simulação do comportamento do dispositivo com base em dados de voos reais ou simulados;
- Configuração, leitura de dados e simulação de voo através de interface gráfica dedicada ([rRocket-UI](https://github.com/gbertoldo/rRocket-UI));
- Alimentação por bateria 6LR61 (9V).

# Versão
Versão 1.7.2
# Aviso :warning:
Como qualquer sistema eletrônico, o rRocket não é infalível. Deste modo, é fundamental que os usuários sigam rigorosamente as [regras de segurança para lançamentos de minifoguetes](https://abmf.bar.org.br/home). Além disso, é essencial garantir que, em caso de falha do sistema de recuperação, a queda do minifoguete não cause danos à vida ou ao patrimônio. Os autores se isentam de qualquer responsabilidade sobre a utilização do dispositivo.

# Hardware
Desenvolvido com o propósito de redução de custos, o hardware do rRocket é formado por componentes eletrônicos facilmente disponíveis no mercado. Em função disso, o computador de bordo pode ser montado por qualquer pessoa com conhecimento básico de soldagem. A figura abaixo ilustra a PCI (placa de circuito impresso) do rRocket modelo EZ, ou, simplesmente, rRocket-EZ, e a figura seguinte ilustra um rRocket-EZ montado. 

<picture>
<img src="https://github.com/user-attachments/assets/2dbd04f5-d6ca-4f60-9fca-387c09d1a4e5" width="300" />
</picture>

Lista completa de componentes
-----------------------------
- Placa principal :todo:
- Arduino Nano
- Módulo BMP280
- R1: resistor 330 ohms (1/4 W)
- R2: resistor 10000 ohms (1/4 W)
- R3: resistor 330 ohms (1/4 W)
- R4: resistor 470 ohms (1/4 W)
- R5: resistor 470 ohms (1/4 W)
- LED 5 mm
- D1: Diodo 1N4007
- Q1 e Q2: transistores TIP120 ou TIP122
- Key1: botão de pressionar 6 mm x 6 mm x 4 mm (push button)
- SG1: buzzer ativo (5 V)
- C9: capacitor eletrolítico (polarizado) 1000 uF (25 V)
- 4 conectores do tipo MKDS3/2-5.08

⚠️ Recomenda-se testar cada componente separadamente antes de realizar a soldagem na placa principal.

# Firmware

Uma vez montado o rRocket, é necessário carregar o _firmware_ no microcontrolador, isto é, o _software_ que fará o controle do dispositivo. O procedimento de carregamento de _firmware_, descrito a seguir, precisa ser realizado uma única vez.

Procedimento de carregamento de _firmware_
------------------------------------------
1. Baixe e instale o [Visual Studio Code/VSCode](https://code.visualstudio.com/);
1. No VSCode, instale o _plugin_ PlatformIO;
1. Baixe e descompacte a [última versão](https://github.com/gbertoldo/rRocket/tags) do código-fonte;
1. Abra o diretório do código-fonte com o VSCode;
1. Compile o código e carregue o _firmware_ clicando em _PlatformIO_ :arrow_right: _nanoatmega328_ :arrow_right: _General_ :arrow_right: _Upload_;
1. Utilize o [rRocket-UI](https://github.com/gbertoldo/rRocket-UI) para carregar os parâmetros de voo pela primeira vez na memória permanente do microcontrolador.
   
# Testes

Há dois tipos de testes mandatórios antes de se utilizar o rRocket em voos reais: simulações e experimentos de bancada. 

## Simulações
O primeiro tipo de testes envolve simulação de voos utilizando a interface gráfica [rRocket-UI](https://github.com/gbertoldo/rRocket-UI). Neste caso, os dados de altura do rRocket não são lidos pelo barômetro, mas informados pela interface gráfica. A simulação pode ser realizada com dados de voos reais obtidos por outros computadores de bordo ou obtidos através de simulações utilizando aplicativos como [Open Rocket](https://openrocket.info/index.html), por exemplo. Há um subdiretório da interface gráfica com dados de diversos voos reais ou fabricados. Exceto nos casos de voos instáveis, nos quais a trajetória difere completamente do modelo utilizado no algoritmo, o computador de bordo deve 
1. Reconhecer a decolagem
2. Detectar corretamente o apogeu e acionar o paraquedas auxiliar
3. Acionar o paraquedas principal na altura configurada
4. Detectar o pouso
   
Caso estas condições não sejam observadas, o dispositivo não deve ser utilizado. Por gentileza, [comunique-nos](https://github.com/gbertoldo/rRocket/issues) a falha.

Observação: antes da liberação de uma nova versão do rRocket, o dispositivo é submetido a testes simulados com todos os voos reais e fabricados disponíveis na interface gráfica. A versão é disponibilizada apenas se as condições acima forem satisfeitas.


## Experimentos de bancada
