# rRocket 

rRocket é um projeto open-source de computador de bordo para minifoguetes baseado em Arduino visando a educação na área aeroespacial. 

# Características principais
- Capacidade de registro da componente vertical da trajetória na memória permanente (EEPROM) do Arduino;
- Capacidade de acionamento de dois paraquedas: o auxiliar (drogue) no apogeu e o principal em uma altura programável;
- Controle através do algoritmo de Máquina de Estados Finitos;
- Aplicação do filtro de Kalman para detecção de apogeu;
- Relatório de erros;
- Configuração, leitura de dados e simulação de voo através de interface gráfica dedicada (rRocket-UI).

# Versão
Versão 1.7.2

# Aviso
Como qualquer sistema eletrônico, o rRocket não é infalível. Deste modo, é fundamental que os usuários sigam rigorosamente as regras de segurança para lançamentos de minifoguetes. Além disso, é essencial garantir que, em caso de falha do sistema de recuperação, a queda do minifoguete não cause danos à vida ou ao patrimônio. Os autores se isentam de qualquer responsabilidade sobre a utilização do dispositivo.

# Hardware
Desenvolvido com o propósito de redução de custos, o hardware do rRocket é formado por componentes eletrônicos facilmente disponíveis no mercado. Em função disso, o computador de bordo pode ser montado por qualquer pessoa com conhecimento básico de soldagem. A figura abaixo ilustra a PCI (placa de circuito impresso) do rRocket modelo EZ, ou, simplesmente, rRocket-EZ, e a figura seguinte ilustra um rRocket-EZ montado. 

<picture>
<img src="https://github.com/user-attachments/assets/2dbd04f5-d6ca-4f60-9fca-387c09d1a4e5" width="300" />
</picture>
  <center>

  ![pci-top](https://github.com/user-attachments/assets/2dbd04f5-d6ca-4f60-9fca-387c09d1a4e5)

</center>

Lista completa de componentes
-----------------------------
- [x] Arduino Nano
- [x] Módulo BMP280
- [x] R1: resistor 330 ohms (1/4 W)
- [x] R2: resistor 10000 ohms (1/4 W)
- [x] R3: resistor 330 ohms (1/4 W)
- [x] R4: resistor 470 ohms (1/4 W)
- [x] R5: resistor 470 ohms (1/4 W)
- [x] LED 5 mm
- [x] D1: Diodo 1N4007
- [x] Q1 e Q2: transistores TIP120 ou TIP122
- [x] Key1: botão de pressionar 6 mm x 6 mm x 4 mm (push button)
- [x] SG1: buzzer ativo (5 V)
- [x] C9: capacitor eletrolítico (polarizado) 1000 uF (25 V)
- [x] 4 conectores do tipo MKDS3/2-5.08

# Software

# Testes de verificação e validação
