Ao executar o programa, deve-se colocar o endereço dos arquivos no comando de execução. 
Use o "./a senhas/0.txt senhas/1.txt senhas/2.txt senhas/3.txt senhas/4.txt senhas/5.txt senhas/6.txt senhas/7.txt senhas/8.txt senhas/9.txt" para executar todas as senhas de todos os arquivos.
Caso deseje executar apenas um numero determinado de arquivos de senhas, coloque apenas os endereços desses aquivos na linha de execução. 

A pasta quebradas_senhas é onde vai estar as senhas criptografadas e suas respectivas senhas quebradas, o modo de escrita do aquivo está em "append", portanto, se executado o codigo mais de 1 vez
com os arquivos de senhas já criados, ele vai adicionar as mesmas senhas duplicadas nos arquivos ".txt". Certifique-se se apagar todos os arquivos antes de gerá-los novamente.

para gerar esses arquivos .txt, dentro do codigo eu utilizo a string "quebradas_" concatenada com o endereço fornecido na linha de comando, exemplo: "quebradas_" + senhas/0.txt = quebradas_senhas/0.txt.
Os caminhos são relativos ao programa, portanto verifique os endereços fornecidos para o funcionamento do codigo.