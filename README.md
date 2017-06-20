# dca1202-serverClientTools

Projeto avaliativo para a turma 2 do componente curricular DCA1202 do curso de Bacharelado em Ciência e Tecnologia da UFRN, período 2017.1.

<h1> DOCUMENTAÇÃO </h1>

<h4>Índice</h4>
<ol>
 <li>
  <a href="#screen">Client Consumer</a>
  <ol>
   <li> <a href="#screena">Atributos privados</a></li>
   <li> <a href="#screenm">Slots</a></li>
  </ol>
 </li>
</ol>

<h2 id="screen">Client Consumer</h2>

<h4 id="screena">-Atributos privados</h4>

<b>ui</b><br/>
Interface de usuário.<br/>

<b>socket</b><br/>
Socket para realizar a conexão com o servidor.<br/>

<b>timer</b><br/>
Temporizador para o envio dos dados.<br/>

<h4 id="screenm">-Slots</h4>

<b>getData()</b><br/>
Recupera os dados gravados no servidor e imprime no console de desenvolvimento.<br/>

<b>connectAction()</b><br/>
Conecta ao servidor. E imprime a lista de ips que enviaram dados para o servidor.<br/>

<b>disconnectAction()</b><br/>
Disconecta do servidor. E limpa a lista de ips.<br/>

<b>startAction()</b><br/>
Inicia a leitura de dados.<br/>

<b>stopAction()</b><br/>
Para o leitura de dados.<br/>

<b>updateAction()</b><br/>
Lê os últimos 30 set de dados gravados no servidor pela ip selecionado e os imprime no gráfico.<br/>

<b>initChart()</b><br/>
Inicio o gráfico no display.<br/>
