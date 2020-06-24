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

![](../master/imagenes/Image.png)

Sacada de https://www.tensorflow.org/install/source#linux


**Instalación mediante conda (Anaconda, la que he usado yo):**

Parecido a la instalación mediante pip pero con más facilidades, para instalar Tensorflow gpu basta con poner el siguiente comando:

conda install tensorflow-gpu==1.13.1

Con esto instalará todos los requisitos de NVIDIA que necesita esta versión de Tensorflow, como en pip se necesitará que tengas actualizados los drives de tu gráfica, pero no tendrás que instalar uno a uno todos los componentes.

Además, en el caso de que se use la versión de CPU (intel) anaconda tiene un rendimiento bastante significativo (alrededor de 8 veces más rápido) en comparación con pip como se muestra en la siguiente figura:

![](../master/imagenes/Image%20%5B1%5D.png)

Este ejemplo se realizó con un Intel® Xeon® Gold 6130.

La información se puede encontrar aquí:

https://www.anaconda.com/tensorflow-in-anaconda/



# 2. Descargar el modelo pre entrenado de mobilenetv1

https://arxiv.org/pdf/1704.04861.pdf

https://ai.googleblog.com/2017/06/mobilenets-open-source-models-for.html
 
Mobilenetv1 son clases de modelos eficientes llamados MobileNets para aplicaciones de móviles y aplicaciones de visión empotradas. Se basa en una arquitectura optimizada que utiliza convoluciones separables en profundidad para construir redes profundas de peso ligero.

Introducen 2 hiperparámetros:

Multiplicador de anchura: Se usa para adelgazar una red de manera uniforme en cada capa. Dada una capa y un multiplicador de anchura α el número de canales de entrada M sería αM y el número de canales de salida N sería αN.
Esto tiene el efecto de reducir el coste computacional y el número de parámetros cuadráticamente alrededor de α2. Se puede aplicar a cualquier estructura de modelo para definir un nuevo modelo más pequeño.

Multiplicador de resolución (ρ): Se aplica en la imagen de entrada y la interna representación de cada capa es reducida por dicho multiplicador.
Aplicamos este hiperparámetro implícitamente cuando ajustamos la resolución de entrada, es decir al poner como resolución 224,192,160 o 128.
Esto hace reducir el coste computacional alrededor de ρ2.

La arquitectura de mobilenet se puede observar en la siguiente tabla:

![](../master/imagenes/Image%20%5B2%5D.png)

https://github.com/fchollet/deep-learning-models/releases/tag/v0.6/

Se recomienda usar mobilenet_7_5_224_tf_no_top.h5

Lo guardaremos en ~ /.keras/models/

Significa que tiene un valor alpha = 0.75 llamado multiplicador de anchura, el tamaño de la imagen de entrada es de 224x224 y sin capa del top(sin dropout ni última capa conectada con activación softmax).

Se recomienda por la siguiente tabla:

![](../master/imagenes/Image%20%5B2%5D.png)

https://github.com/fchollet/deep-learning-models/releases/tag/v0.6/
Se recomienda usar mobilenet_7_5_224_tf_no_top.h5
Lo guardaremos en ~ /.keras/models/
Significa que tiene un valor alpha = 0.75 llamado multiplicador de anchura, el tamaño de la imagen de entrada es de 224x224 y sin capa del top(sin dropout ni última capa conectada con activación softmax).
Se recomienda por la siguiente tabla:









