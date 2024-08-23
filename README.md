# rRocket 🚀 

rRocket é um projeto _open-source_ de computador de bordo para minifoguetes baseado em Arduino visando a educação na área aeroespacial. 

# Características principais
- Utiliza Arduino Nano;
- Medição de altura acima do nível do solo por barômetro BMP280;
- Capacidade de registro da componente vertical da trajetória na memória permanente (1024 bytes) do Arduino;
- Registro de dados de voo com frequência de 10 Hz até o apogeu;
- Registro de dados de voo com frequência definida pelo usuário após o apogeu ($\leq$ 10 Hz);
- Capacidade de acionamento de dois paraquedas: o auxiliar (drogue) no apogeu e o principal em altura definida pelo usuário;
- Controle através do algoritmo de Máquina de Estados Finitos;
- Aplicação do [filtro de Kalman](https://www.kalmanfilter.net/default.aspx) para detecção de apogeu e filtro alfa dinâmico para oscilações de velocidade;
- [Comunicação de apogeu](#apogeu) através de sinal sonoro e luninoso;
- [Relatório de erros](#códigos-de-erros);
- Capacidade de [simulação](#simulações) do comportamento do dispositivo com base em dados de voos reais ou simulados;
- Configuração, leitura de dados e simulação de voo através de interface gráfica dedicada ([rRocket-UI](https://github.com/gbertoldo/rRocket-UI));
- Alimentação por bateria 6LR61 (9V).

# Versão
Versão 1.7.2
# Aviso :warning:
Como qualquer sistema eletrônico, o rRocket não é infalível. Deste modo, é fundamental que os usuários sigam rigorosamente as [regras de segurança para lançamentos de minifoguetes](https://abmf.bar.org.br/home). Além disso, é essencial garantir que, em caso de falha do sistema de recuperação, a queda do minifoguete não cause danos à vida ou ao patrimônio. Os autores se isentam de qualquer responsabilidade sobre a utilização do dispositivo.

# Hardware
Desenvolvido com o propósito de redução de custos, o hardware do rRocket é formado por componentes eletrônicos facilmente disponíveis no mercado. Em função disso, o computador de bordo pode ser montado por qualquer pessoa com conhecimento básico de soldagem. A figura abaixo ilustra a PCI (placa de circuito impresso) do rRocket modelo EZ, ou, simplesmente, rRocket-EZ, e a figura seguinte ilustra um rRocket-EZ montado. 

<center>
<picture>
<img src="https://github.com/user-attachments/assets/2dbd04f5-d6ca-4f60-9fca-387c09d1a4e5" width="300" />
</picture>
<br /> 
<picture>
<img src="https://github.com/user-attachments/assets/a2601c7f-ad41-4994-97f1-a8c8428a86c2" width="300" />
</picture>
</center>

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

Há dois tipos de testes de verificação mandatórios antes de se utilizar o rRocket em voos reais: simulações e experimentos de bancada. 

## Simulações
O primeiro tipo de testes envolve simulação de voos utilizando a interface gráfica [rRocket-UI](https://github.com/gbertoldo/rRocket-UI). Neste caso, os dados de altura do rRocket não são lidos pelo barômetro, mas informados pela interface gráfica. A simulação pode ser realizada com dados de voos reais obtidos por outros computadores de bordo ou obtidos através de simulações utilizando aplicativos como [Open Rocket](https://openrocket.info/index.html), por exemplo. Há um subdiretório da interface gráfica (sim) com dados de diversos voos reais ou fabricados. Exceto nos casos de voos instáveis, nos quais a trajetória difere completamente do modelo utilizado no algoritmo, o computador de bordo deve 
1. Reconhecer a decolagem
2. Detectar corretamente o apogeu e acionar o paraquedas auxiliar
3. Acionar o paraquedas principal na altura configurada
4. Detectar o pouso
   
Caso estas condições não sejam observadas, o dispositivo não deve ser utilizado. Por gentileza, [comunique-nos](https://github.com/gbertoldo/rRocket/issues) a falha.

Observação: antes da liberação de uma nova versão do rRocket, o dispositivo é submetido a testes simulados com todos os voos reais e fabricados disponíveis na interface gráfica. A versão é disponibilizada apenas se as condições acima forem satisfeitas.

## Experimentos de bancada

Os experimentos de bancada visam avaliar a integridade de _hardware_ e a sua integração com o _software_. Para isso, é necessário utilizar um ambiente onde seja possível controlar a pressão, uma vez que o sensor de altura  é barométrico, e monitorar as tensões da bateria e dos terminais de acionamento de paraquedas. Além disso, deve ser possível acionar ignitores ligados ao rRocket, mas localizados externamente ao ambiente de pressão controlada. Tudo isso é possível utilizando-se um dispositivo para testes experimentais em bancada denominado [rRocket-Monitor](https://github.com/gbertoldo/rRocket-Monitor) e sua respectiva interface gráfica, o [rRocket-Monitor-UI](https://github.com/gbertoldo/rRocket-Monitor-UI). O rRocket deve satisfazer as [condições para verificação em experimentos de bancada](https://github.com/gbertoldo/rRocket-Monitor-UI?tab=readme-ov-file#procedimento-de-teste-do-rrocket) antes de qualquer lançamento!

# Operações com o botão
Há dois botões no rRocket: um sobre o Arduino e outro soldado na placa principal. O botão do Arduino serve para reiniciá-lo e não deve ser utilizado. O segundo botão tem as seguintes funções:
- Clique curto (menos que 1 s): informa o apogeu do último voo de forma sonora e visual.
- Clique longo (mais que 3 s): apaga a memória do último voo.

 ⚠️ É necessário tomar os devidos cuidados para que nenhum dos botões seja pressionado acidentalmente durante o voo.

# Comunicação visual e sonora

## Estado do altímetro
- Bipe contínuo: indica que um erro crítico no barômetro foi detectado durante a inicialização. Nestas condições, o altímetro não deve ser utilizado.
- Bipe e piscar de LED intermitente com período de 1,5 s: a memória de voo está limpa e o altímetro está pronto para o lançamento.
- Ausência de bipe e piscar de LED: a memória de voo está preenchida. O altímetro não deve ser lançado. Recupere os dados utilizando o [rRocket-UI](https://github.com/gbertoldo/rRocket-UI) e limpe a memória de voo para o próximo lançamento.

## Apogeu
A comunicação do apogeu, em metros, é feita através de bipes e piscadas de LED. Como os dois sinais são sincronizados, apenas a codificação do sinal sonoro será explicada. A apresentação de cada algarismo que compõe o número é realizada da maior para a menor ordem. Por exemplo, no número 1203, são apresentados, nesta sequência, os algarismos 1, 0, 2 e 3. As ordens são separadas por uma breve pausa. Os algarismos são identificados pela quantidade ou tipo do bipe. Um bipe curto indica o algarismo 1, dois bipes curtos indicam o algarismo 2, e assim sucessivamente. O algarismo zero é indicado por um bipe longo. Seguem alguns exemplos:
- 1203 m: Bipe PAUSA Bipe-Bipe PAUSA Biiiiiiipe PAUSA Bipe-Bipe-Bipe
- 203 m: Bipe-Bipe PAUSA Biiiiiiipe PAUSA Bipe-Bipe-Bipe
- 80 m: Bipe-Bipe-Bipe-Bipe-Bipe-Bipe-Bipe-Bipe PAUSA Biiiiiiipe

# Códigos de erros
Em caso de erros, os seguintes códigos podem ser reportados pelo rRocket:
- Erro 1: Falha de inicialização do barômetro;
- Erro 2: Falha de inicialização de atuadores;
- Erro 3: Altura menor que o limite inferior registrável;
- Erro 4: Altura maior que o limite superior registrável;
- Erro 5: Voo iniciado com memória não apagada.

