# deleteUser
Trabalho para Tópicos Especiais em Segurança.

## Como o programa funciona

O programa cria um processo filho que será responsável por apagar toda a pasta do usuário (na realidade, sobreescrever todos os arquivos com 0, levemente dificultando a obtenção do arquivo).

Ambos os processos ficando checando se o outro está vivo. O pai aguarda a finalização do filho até que este termine de fato. Se não terminar, ele executa um novo `fork` e o processo recomeça.

No caso do filho, se o pai estiver morto, ele executa um `fork`, o filho começa do ponto onde ele parou e ele agora fica checando se este novo filho continua.

Se ambos forem mortos simultaneamente, o programa é parado e você vence o vírus (no windows é uma dificuldade razoável, enquanto no linux basta um `kill` com dois argumentos).

## NÃO EXECUTE ESSE CÓDIGO

NÃO EXECUTE ESTE CÓDIGO. A execução deste código é por sua conta e risco, pois seu funcionamento padrão é inutilização de toda a pasta do usuário.