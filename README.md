# Semaforo inteligente
El objetivo de este proyecto es el diseño de un semaforo que cumpla
con las siguientes funciones:

1. Si detecta que en un carril hay vehiculos y en el otro no,
active el semaforo para el carril congestionado.
2. Si detecta que en un carril hay vehiculos a gran distancia
   y en el otro hay a menor distancia, activar el semaforo
   con mayor cantidad de vehiculos en espera
   (implementar sesgo de distancia).
3. Los tiempos de espera/transición deben variar por la hora del día,
   i.e., si hay luz del sol, transición mas lenta, si es de noche,
   transición rápida.
4. En caso de que haya congestionamiento en ambos carriles,
   debe tener un comportamiento de facto. Intervalos continuos
   aunque respetando la hora del día.
