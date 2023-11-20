# IDENTIFICACIÓN DE MOVIMIENTOS

**NOTA**: Hay que añadir un boton de presencia y otro para 'despertar'.

## Movimiento PUSH
- Aceleracion en Y >= 0.4 -> se mueve hacia delante
- Aceleracion en Y <= -0.4 -> se mueve hacia delante
- Aceleración en Z ~ 0 

## Movimiento ROTATE
- Posición Z >= 0 --> Giro hacia la
- Aceleracion en X >= 0 (+) --> Giro hacia la **derecha**
- Aceleracion en X <= 0 (-) --> Giro hacia la **izquierda**
- Aceleración en Z ~ 0 

## Movimiento FLIPPING 180º en Z
- Aceleracion en Z >= 0 (+) --> Hacia **arriba**
- Aceleracion en Z <= 0 (-) --> Hacia **abajo**

## Movimiento FLIPPING 90º en Y
- Aceleracion en Y >= 0 (+) --> Hacia **detrás**
- Aceleracion en Y <= 0 (-) --> Hacia **delante**

## Movimiento FLIPPING 90º en X
- Aceleracion en X >= 0 (+) --> Hacia la **izquierda**
- Aceleracion en X <= 0 (-) --> Hacia la **derecha**

## Movimiento SHAKING
Se lo dejamos al sensor de vibración

## Modo Reposo
Cuando no se esté pulsando el botón de presencia, debe entrar en modo **reposo** y para salir del mismo, se deberá de pulsar otro botón para 'despertar'.
