Introduzione al gioco:
Una volta iniziata la partita saranno visibili:
-Un bottone per ricominciare la partita.
-Una ScrollBox che permetterà di visualizzare le mosse eseguite e che permetterà di tornare indietro tra di esse*.
-La scacchiera con tutti i pezzi nella quale si potrà svolgere la partita**.
-Una raccolta di Tile nella quale verranno mostrate le pedine mangiate nel corso della partita e anche le eventuali promozioni.

*Per quanto riguarda i bottoni "cliccabili", sarà possibile tornare indietro nelle mosse solo tramite i bottoni delle pedine bianche, mentre verranno visualizzati in rosso i bottoni che evidenziano le "mangiate" e le promozioni.
**La partita comincia sempre dal giocatore bianco e lo sfidante è esclusivamente il giocatore "Random" che però agisce in maniera pesata; innanzitutto controlla tra le mosse dei suoi pezzi quali non mettono sotto scacco il proprio re,
tra questa selezione di mosse controlla con quali riuscirebbe a "mangiare" una pedina avversaria, tramite un algoritmo ordina appunto queste mosse per mezzo di una gerarchia data dalla differenza di peso tra mangiante e mangiato. 
Come ultima cosa guarda che mangiando non venga poi mangiata di conseguenza, fatte queste considerazioni agisce.
Nel caso in cui non trovi delle mosse grazie alla quale può mangiare l'avversario si muove in maniera randomica.
