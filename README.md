# TFG2020
## Aceleración de AI en dispositivos de bajo consumo


En este repositorio se puede encontrar tanto los modelos usados para este trabajo, como los código para la ejecución de la aplicación y el firmware utilizado en las placas Maix Go y Maix Bit.

A continuación se detalla la instalación, entrenamiento y ejecución del proyecto:



# ENTRENAR, CONVERTIR Y EJECUTAR MODELO MOBILENET
Tutorial sacado de: https://en.bbs.sipeed.com/t/topic/682/

Autor: Salvador Albarrán Tiradas

Sistema operativo: Ubuntu 18.04

Para Maix Go y Maix Bit:

TENSORFLOW Y KERAS

https://www.tensorflow.org/

https://keras.io/
 
**Tensorflow** es una plataforma de código abierto para el aprendizaje automático que cuenta con un ecosistema integral y flexible de herramientas, bibliotecas y recursos comunitarios para impulsar el estado del arte y desarrollar fácilmente aplicaciones basadas en aprendizaje automático.

**Keras** es una API de redes neuronales de alto nivel, escrita en Python y capaz de ejecutarse sobre TensorFlow, CNTK o Theano.
Admite redes convolucionales y redes recurrentes, así como combinaciones de las dos y se ejecuta sin problemas en CPU y GPU.

# 1 Instalar el entorno (Se ha elegido Keras)
Nota: La versión de tensorflow 1.x a 2.x cambia la forma de escribir los scripts, por lo que si se usan los scripts que se dan en este ejemplo con la versión 2.x no funcionará, los cambios son básicamente que ya no hay que instalar Keras ya que está implementado dentro de tensorflow, por lo que para usar una librería de keras tendríamos que hacer:

Import tensorflow as tf

tf.keras.optimizers…

más información aquí: https://www.tensorflow.org/guide/migrate/

Tenemos varias opciones:

**Docker tensorflow (Recomendada por Sipeed):**

Aquí recomiendan la versión de tensorflow 1.13.1, para usarlo se necesita actualizar los drivers de la gráfica, aquí tenéis la guía de instalación de tensorflow:
https://www.tensorflow.org/install/docker
 
**Instalación mediante pip:**

Se necesita instalar tensorflow-gpu == 1.13.1 si quieres usar la misma versión que recomiendan en sipeed con la versión de gpu, si quieres usar la versión de cpu hay que eliminar la parte de gpu: tensorflow==1.13.1.
Además, necesitaremos instalar el software de NVIDIA donde se puede encontrar la información aquí: https://www.tensorflow.org/install/gpu/
Para saber que versión instalar se puede mirar en esta gráfica:

![imagen](../master/imagenes/Image.png)

Sacada de https://www.tensorflow.org/install/source#linux


**Instalación mediante conda (Anaconda, la que he usado yo):**

Parecido a la instalación mediante pip pero con más facilidades, para instalar Tensorflow gpu basta con poner el siguiente comando:

conda install tensorflow-gpu==1.13.1

Con esto instalará todos los requisitos de NVIDIA que necesita esta versión de Tensorflow, como en pip se necesitará que tengas actualizados los drives de tu gráfica, pero no tendrás que instalar uno a uno todos los componentes.

Además, en el caso de que se use la versión de CPU (intel) anaconda tiene un rendimiento bastante significativo (alrededor de 8 veces más rápido) en comparación con pip como se muestra en la siguiente figura:





