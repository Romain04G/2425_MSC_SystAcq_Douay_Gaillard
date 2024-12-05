# TP DE SYSTEME D'ACQUISITION ET DE COMMANDE
### Par Romain GAILLARD et Nicolas DOUAY
## <ins>But:</ins>
Le but de ce TP est de commander un moteur à courant continu à l'aide de STM32. Le moteur sera contrôler à l'aide d'un terminal dans lequel nous entrerons nos requêtes comme la vitesse souhaitée. Le moteur va également être asservi en courant et en tension.
## <ins>Déroulé du TP:</ins>
### <ins>Première Partie: Le shell</ins>
Nous devons tout d'abord créer un shell pour communiquer avec le moteur. Nous pouvons écrire dans le shell et il est capable de détecter quelle fonction est appelée pour faire les actions associées
### <ins>Deuxième Partie: Les PWMs</ins>
Afin de contrôler le moteur en vitesse, nous lui envoyons une commande en complémentaire décalée. Lorsque le rapport cyclique des PWMs est à 50%, il est à l'arrêt. Lorsque, le rapport cyclique est entre 51%et 99%, le moteur tourne dans un sens et il tourne dans l'autre sens lorsque le rapport cyclique est entre 1% et 49%. Il faut éviter les valeurs de rapport cyclique de 0% et 100%. 
