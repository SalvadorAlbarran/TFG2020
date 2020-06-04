import tensorflow as tf
from tensorflow import keras
import numpy as np


DATASET_DIR = '/home/salva/Descargas/ILSVRC2012_img_train'


base_model=tf.keras.applications.mobilenet.MobileNet(input_shape=(224, 224, 3), alpha = 0.25,depth_multiplier = 1, dropout = 0.001, pooling='avg',include_top = False, weights = "imagenet", classes = 1000)
x=base_model.output
x = tf.keras.layers.Dropout(0.001, name='dropout')(x)  #drop=0.001
preds=tf.keras.layers.Dense(1000,activation='softmax')(x) #final layer with softmax activation

model=tf.keras.Model(inputs=base_model.input,outputs=preds)

for i,layer in enumerate(model.layers):
    print(i,layer.name)

for layer in model.layers[:86]:
    layer.trainable=False
for layer in model.layers[86:]:
    layer.trainable=True


train_datagen = tf.keras.preprocessing.image.ImageDataGenerator(preprocessing_function=tf.keras.applications.mobilenet.preprocess_input)
train_generator=train_datagen.flow_from_directory(DATASET_DIR,
                                                 target_size=(224,224),
                                                 color_mode='rgb',
                                                 batch_size=256,
                                                 class_mode='categorical', shuffle=True)
model.summary()
model.compile(optimizer=tf.keras.optimizers.Adam(),loss=tf.keras.losses.categorical_crossentropy,metrics=[tf.keras.metrics.categorical_accuracy])
# Adam optimizer
# loss function will be categorical cross entropy
# evaluation metric will be accuracy


# checkpoint
class MyCbk(keras.callbacks.Callback):

    def __init__(self, model):
         self.model_to_save = model

    def on_epoch_end(self, epoch, logs=None):
        self.model_to_save.save('model_at_epoch_%d.h5' % epoch)

checkpoint = MyCbk(model)
callbacks_list = [checkpoint]

step_size_train=50  #train_generator.n//train_generator.batch_size/8
model.fit_generator(generator=train_generator,steps_per_epoch=step_size_train,callbacks=callbacks_list,epochs=20)

tf.keras.models.save_model(model,"mbnet25.h5")
#converter = lite.TFLiteConverter.from_keras_model(model)





