# 11911EBI020-ATV2

Provavelmente você já deve ter criado o seu ambiente de desenvolvimento bare metal na atividade anterior. Agora, como última atividade a ser realizada dessa forma (sem auxílio de IDEs), você irá incrementar um pouco o firmware criado. 

Até o momento, o firmware apenas inicializa o controlador e deixa o led da placa piscando. O que iremos mudar:

* Configure a placa para ter um pino de entrada adicional. Usuários da black pill podem usar o pino PA0, ligado em uma tactile switch presente na placa. Caso a sua placa não possua isso, use outro pino e um jumper para te ajudar na tarefa. Configure um resistor de pull up nesse pino, assim você irá ter certeza de que quando o valor do pino estiver em zero ele foi pressionado.

* A frequência de piscada do led agora irá mudar, a depender do estado desse pino de entrada. Em nível baixo ele deverá piscar o led mais lentamente e em nível alto em frequência mais alta.

Crie seu firmware e suba o resultado para o github.

# Desenvolvimento

O primeiro passo foi determinar a porta que seria utilizada, com base no esquemático do STM32F411 e como sugerido na atividade, a porta **A0** está conectada ao botão **KEY**, podendo ser utilizado para entrada e saída do sinal se configurado em push-pull.

Conforme identificado, buscou-se o endereço base do GPIOA no manual de referência e foi adicionado ao main.