Pasos para realizar la compilación:
1- En cpp, debe ir al archivo llamado test.cpp y en las líneas de código N° 161 y 162 (Especificamente, en el main) debe cambiar la ruta de los strings str_data y str_queries por la ruta donde usted tenga los 
archivos llamados data_eda.npy y queries_eda.npy que vienen en esta carpeta. (Recuerde guardar el archivo luego de hacer estos cambios :D)
2- Ir a la carpeta build que se encuentra en cpp/build
3- En caso de haber archivos previamente creados, realizar el siguiente comando en la terminal: make clean
4- Posteriormente, ejecutar el siguiente comando: cmake ..
5- Como penúltimo paso, se debe ejecutar el comando: make
(Esto creará el archivo ejecutable llamado test que se encontrará en la carpeta build. Si desea corroborarlo, escriba ls en la terminal y ejecutelo)
6- Por último, ejecute el comando ./test y se deberían mostrar los resultados en la pantalla de la terminal.

Esperamos que le haya funcionado nuestro código!
